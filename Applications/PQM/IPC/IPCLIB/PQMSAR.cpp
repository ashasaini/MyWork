//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
// PQMSAR.cpp: implementation of the CPQMSAR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMSAR.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>

#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "Pqm.h"
#include "PqmPrMode.h"
#include "PqmPrTRSC.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345

CPQMSAR::CPQMSAR(CPqm* l_pqm): m_pqm(l_pqm)
{
}

CPQMSAR::~CPQMSAR()
{
    SarDone();
}


//****************************Method Header************************************
//Method Name   : ResetRfl()
//Author        : PATNI\MRP
//Purpose       : Reset all rfl values.
//*****************************************************************************
void CPQMSAR::ResetRfl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::ResetRfl");
    rflParams.rflTxGain		= 0.0;
    rflParams.rflTxGainLimit = 0.0;
    rflParams.rflScaler		= 0.0;
    rflParams.rflWaveAngle	= 0;
    rflParams.rflWaveSum	= 0.0;
    rflParams.rflWavePitch	= 0.0;
    rflParams.rfampModeMax	= 0.0;
    SarInitVfTGCParams(&tgcParams);
    pqmSarStat = sarStatTsuccess;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Successful"));
}

//****************************Method Header************************************
//Method Name   : ResetSARInfo()
//Author        : PATNI\MRP
//Purpose       : Call this subroutine,when new Rfls run,new coil set or
//		  new SAR anatomy set.
//*****************************************************************************
void CPQMSAR::ResetSARInfo(
    CPqmProtocol* const f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::ResetSARInfo");
    SarInitInfo(f_protocol->GetSarInfo());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Successful"));
}

//****************************Method Header************************************
//Method Name   : RecalculateSAR()
//Author        : PATNI\MRP
//Purpose       : This functions sets the new RFL's and recalculates the SAR
//*****************************************************************************
void CPQMSAR::RecalculateSAR(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const	f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::RecalculateSAR");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if ((NULL == f_protocol) || (NULL == m_pqm)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or m_pqm pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString l_print_string(_T(""));
    l_print_string.Format(_T("Protocol (Pos, Acqorder) = (%d, %d), COPY SOURCE = %d"), f_protocol->GetPosition(), f_protocol->GetProtocol(), f_protocol->GetCopySourceProt());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    float			l_sar_save = 0.0f;
    CPqmPrMode* l_mode = f_protocol->GetPqmPrMode();

    if (m_pqm->IsSaturnCoilSystem()) {
        if (l_mode->GetAnatomy() <= 0 ||
            (f_protocol->GetSaturnCoil())->numSection <= 0) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Mode Anatomy is <= 0  or protocol's sections are <= 0"));
            return;
        }

    } else {
        if (l_mode->GetAnatomy() <= 0 ||
            l_mode->GetRecvCoilId() <= 0) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Mode Anatomy Or Receiver Coil ID is <= 0  "));

            return;
        }
    }


    sarInfo_t*		l_sarInfo = (f_protocol->GetSarInfo());
    l_sar_save = l_sarInfo->tsar[l_sarInfo->active].sar;

    //Set SAR Status As sarStatTsuccess by default..
    //This function will again set error value to SARStat if required.
    f_protocol->SetSARStat(sarStatTsuccess);

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    pqmSarSetNewCouch(f_protocol);

    if (((f_protocol->GetCopySourceProt() < 0) ||
         (f_protocol->GetcouchMethodFlg() != VFC_MOVE_COUCH_METHOD_PARENT_POSI))	&&
        (f_protocol->GetRflProt() == -1)) {

        SetNewRfl(f_vf_handle, f_protocol);

    } else {
        pqmSarSetVfRfl(f_vf_handle, f_protocol);
    }

    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    if (!f_protocol->GetSARInit()) {
        if (((f_protocol->GetCopySourceProt() < 0) ||
             (f_protocol->GetcouchMethodFlg() != VFC_MOVE_COUCH_METHOD_PARENT_POSI))	&&
            (f_protocol->GetRflProt() == -1)) {
            //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
            GetValueFromVF(f_vf_handle, f_protocol);

        } else {
            CalculateSAR(f_vf_handle, f_protocol);
        }

    } else {
        CalculateSAR(f_vf_handle, f_protocol);
    }

    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    /* Set Changed Flag for PqmPqReMakeList() */
    if (l_sar_save - l_sarInfo->tsar[l_sarInfo->active].sar >  0.00001 ||
        l_sar_save - l_sarInfo->tsar[l_sarInfo->active].sar < -0.00001) {
        f_protocol->GetPqmPrTRSC()->SetSARChanged(TRUE);
    }

}

//****************************Method Header************************************
//Method Name   : PutRflParams()
//Author        : PATNI\MRP
//Purpose       : This functions puts the new RFL's in VF
//*****************************************************************************
status_t CPQMSAR::PutRflParams(CVarFieldHandle* const vf_handle, int f_prot)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::PutRflParams");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm pointer is NULL"));
        return E_ERROR;
    }

    CPQMStudy* pqm_study = m_pqm->GetStudy();

    if (pqm_study == NULL) {
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* l_obj = pqm_study->GetItemFromProtocol(f_prot);	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("[PqmSarPutRflParams]:");

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_obj == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_obj pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections
    ClibSARAdapter sarAdapter(vf_handle);
    sarStat_t l_sarStat = sarAdapter.SarPutVfRflParams(f_prot, &rflParams);

    if (l_sarStat != sarStatTsuccess) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarPutVfRflParams Failed"));

        l_obj->SetSARStat(l_sarStat);		/* V9.25 */
        return E_ERROR;
    }

    l_sarStat = sarAdapter.SarPutVfTGCRFoutRatioHighSAR(f_prot, &tgcParams);

    if (l_sarStat != sarStatTsuccess) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarPutVfTGCRFoutRatioHighSAR Failed"));

        l_obj->SetSARStat(l_sarStat);		/* V9.25 */
        return E_ERROR;
    }

    //DB_PRINTF("S",("Protocol : %d TGC = %g\n",prot,tgcParams.TGCRFoutRatio));

    return E_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   : FindSarMode()
//Author        : PATNI/PJS

//*****************************************************************************
CString CPQMSAR::FindSarMode(
    const int32_t f_mode
)
{
    switch (f_mode) {

        case IEC_SAR_OVER:
            return _T(" (Over)");

        case IEC_SAR_2ND_MODE:
            return _T(" (2nd)");

        case IEC_SAR_1ST_MODE:
            return _T(" (1st)");
    }

    return _T("");
}

//****************************Method Header************************************
//Method Name   : GetValueFromVF()
//Author        : PATNI\MRP
//Purpose       : Call this sub routine after read a protocol and
//		          make the CPqmProtocol object.
//*****************************************************************************
void CPQMSAR::GetValueFromVF(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const	f_protocol
)
{

    LPCTSTR FUNC_NAME = _T("CPQMSAR::GetValueFromVF");
    PQM_SCOPED_TRACE();

    int	acqStatus = C_PR_STATUS_WAIT;

    if (f_protocol && f_protocol->GetProtocol() != (-1)) {

        CString l_str_trace(_T(""));
        l_str_trace.Format(_T("Prot (Position, Acqorder) = (%d, %d)"), f_protocol->GetPosition(), f_protocol->GetProtocol());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_trace);
        acqStatus = f_protocol->GetAcqStatus();

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Illegal Protocol Object."));
        //DB_PRINTF("",("Illegal Protocol Object.\n"));
        return;
    }

    sarVfParams_t		vfParams;
    sarVfSepParams_t	vfSepParams;
    bzero(&vfParams, sizeof(vfParams));
    bzero(&vfSepParams, sizeof(vfSepParams));

    sarStat_t			sarStat = sarStatTsuccess;
    sarInfo_t*			l_sarInfo = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (acqStatus == C_PR_STATUS_WAIT || acqStatus == C_PR_STATUS_CURRENT) {

        l_sarInfo = (f_protocol->GetSarInfo());
        //+Patni-HAR/2009Mar28/Modified/PSP1#313
        /*l_sarInfo->tsar[l_sarInfo->active].sar = 0.0;
        l_sarInfo->tsar[l_sarInfo->active].preal = 0.0;
        l_sarInfo->coilProtect = FALSE;
        l_sarInfo->scanEnable = TRUE;*/
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].sar
        = 0.0f;
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].preal
        = 0.0f;
        f_protocol->GetSarInfo()->coilProtect   = FALSE;
        f_protocol->GetSarInfo()->scanEnable    = TRUE;
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        //+Patni-HAR/2009Mar28/Modified/PSP1#313

        /* SAR Anatomy‚ª‘I‘ð‚³‚ê‚Ä‚¢‚È‚¢ê‡‚àSARŒvŽZ‚µ‚È‚¢ */
        if (m_pqm->IsSaturnCoilSystem()) {
            // V9,25 Coil Support@AMT
            if (f_protocol->GetPqmPrMode()->GetAnatomy() <= 0 ||
                (f_protocol->GetSaturnCoil())->numSection <= 0 ||
                (f_protocol->GetSarInfo())->connectedCoil[0] <= 0) {

                return;
            }

        } else {
            if (f_protocol->GetPqmPrMode()->GetAnatomy() <= 0 ||
                f_protocol->GetPqmPrMode()->GetRecvCoilId() <= 0) {

                return;
            }
        }

        /*** Get vf params from Vf. ***/
        ClibSARAdapter sarAdapter(f_vf_handle);
        sarStat = sarAdapter.SarGetVfParams(f_protocol->GetProtocol(), &vfParams);

        if (sarStat != sarStatTsuccess) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfParams Failed"));
            f_protocol->SetSARStat(sarStat);		/* V9.25 */
            return;
        }

        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfParams Done."));

        /*** Get Sep params from Vf. ***/
        sarStat = sarAdapter.SarGetVfSepParams(f_protocol->GetProtocol(), &vfSepParams);

        if (sarStat != sarStatTsuccess) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfSepParams Failed"));
            f_protocol->SetSARStat(sarStat);		/* V9.25 */
            return;
        }

        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfSepParams Done."));


        /*** Set params and RFL data to sarInfo. ***/
        sarStat = SarSetParams(l_sarInfo, &vfParams, &rflParams);

        if (sarStat != sarStatTsuccess) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetParams Failed"));

            f_protocol->SetSARStat(sarStat);		/* V9.25 */
            return;
        }

        l_sarInfo->TGCRFoutRatio = tgcParams.TGCRFoutRatio;

        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetParams Done."));

        /*** Set Sep params to sarInfo. ***/
        sarStat = SarSetSepParams(l_sarInfo, &vfSepParams);

        if (sarStat != sarStatTsuccess) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetSepParams Failed"));
            f_protocol->SetSARStat(sarStat);		/* V9.25 */
            return;
        }


        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetSepParams Done."));

        CalculateSAR(f_vf_handle, f_protocol);
        l_sarInfo = (f_protocol->GetSarInfo());


        CString l_str_prot_no(_T(""));
        l_str_prot_no.Format(_T("%g"), l_sarInfo->tsar[l_sarInfo->active].sar);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SAR = ") + l_str_prot_no);
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

    } else {
        int		l_subProtocol = 0;
        float		sar = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        BITFLD_INIT(mask, K_ALL_PR, 0);

        status_t status = m_pqm->GetStudy()->GetSubProtValues(f_protocol->GetProtocol(), l_subProtocol, mask,
                          K_PR_SAR,	&sar,	NULL,
                          NULL);

        if (status != E_NO_ERROR || BITFLD_ON(mask, K_PR_SAR)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetSubProtValues ERROR"));
        }
    }

    f_protocol->SetSARInit(TRUE);

}

//****************************Method Header****************************
// Method Name  :GetSARValue
// Author       :PATNI/
// Purpose      :
//*********************************************************************
void CPQMSAR::GetSARValue(CPqmProtocol*  const f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::GetSARValue");
    PQM_SCOPED_TRACE();

    if (NULL == m_pqm) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm pointer is NULL"));
        return;
    }

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (f_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections
    sarInfo_t*  sarinfo = f_protocol->GetSarInfo();
    float sar[3] = {0.0, 0.0, 0.0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    flt32_t l_sar_ratio[3] = {0.0f};
    int32_t	l_sar_opemode	= 0;

    CPQMStudy* pqm_study = m_pqm->GetStudy();

    if (pqm_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pqm_study pointer is NULL"));
        return;
    }

    CVarFieldHandle* l_vf_handle = pqm_study->GetVfDBHandle();

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_vf_handle == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_vf_handle is NULL"));
        return;
    }

    VfFieldSpec_t   vfReq[7] = {0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections

    VfPathElem_t    pelm[] = {{SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0},
        {SVN_ST_IMAGE, 0}
    };

    int n = 0;
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 7);	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_SAR, SVT_SEQ_SAR, MAX_STRING_DATA, sar, VFO_VARIABLE_LEN);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_ACTIVE_SAR, SVT_SEQ_ACTIVE_SAR, sizeof(sarinfo->active), &sarinfo->active, VFO_VARIABLE_LEN);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_B1_RMS, SVT_SEQ_B1_RMS, sizeof(sarinfo->B1rms), &sarinfo->B1rms, VFO_VARIABLE_LEN);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_SAR_OPERATING_MODE, SVT_SEQ_SAR_OPERATING_MODE, sizeof(l_sar_opemode), &l_sar_opemode, 0L);
    n++;

    if (SARCTRL_LICENCE_ON == m_pqm->GetSARControlLicense()) {

        VFF_SET_ARGS(vfReq[n], SVN_PLN_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL, SVT_PLN_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL,
                     3 * sizeof(flt32_t), l_sar_ratio, 0L);
        n++;
        VFF_SET_ARGS(vfReq[n], SVN_SEQ_RTSAR_OPERATING_MODE, SVT_SEQ_RTSAR_OPERATING_MODE,
                     sizeof(sarinfo->sarControl.ope_mode), &sarinfo->sarControl.ope_mode, 0L);
        n++;
        VFF_SET_ARGS(vfReq[n], SVN_SEQ_ACTIVE_RTSAR, SVT_SEQ_ACTIVE_RTSAR,
                     sizeof(sarinfo->sarControl.active), &sarinfo->sarControl.active, 0L);
        n++;
    }

    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();
    pelm[SVD_ST_SUBPROTOCOL - 1].index = 0;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections


    int num = 0;
    status_t l_status = l_vf_handle->GetFields(pelm, SVD_ST_SUBPROTOCOL, vfReq, n, &num);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields failed."));
    }

    //+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
    //sarinfo->tsar[sarinfo->active].sar = sar[sarinfo->active];
    sarinfo->tsar[0].sar = sar[0];
    sarinfo->tsar[1].sar = sar[1];
    sarinfo->tsar[2].sar = sar[2];
    //- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
    sarinfo->tsar[sarinfo->active].ope_mode = l_sar_opemode;

    if (SARCTRL_LICENCE_ON == m_pqm->GetSARControlLicense()) {

        sarinfo->sarControl.tsarControl[sarTypeWholeBody].sar_6min	= l_sar_ratio[sarTypeWholeBody];
        sarinfo->sarControl.tsarControl[sarTypeHead].sar_6min		= l_sar_ratio[sarTypeHead];
        sarinfo->sarControl.tsarControl[sarTypePartial].sar_6min	= l_sar_ratio[sarTypePartial];
    }
}

//+Patni-RAJ/2010Jan18/Added/PH#3#/IR86/Debug Window
sarVfRflParams_t CPQMSAR::GetRflParams()const
{
    return rflParams;
}
//-Patni-RAJ/2010Jan18/Added/PH#3#/IR86/Debug Window

//****************************Method Header************************************
//Method Name   : GetNewRfl()
//Author        : PATNI\MRP
//Purpose       : Call this subroutine,when new Rfls run
//*****************************************************************************
void CPQMSAR::GetNewRfl(
    CVarFieldHandle* const f_vf_handle,
    const int		f_prot
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::GetNewRfl");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm pointer is NULL"));
        return;
    }

    CPQMStudy* pqm_study = m_pqm->GetStudy();

    if (pqm_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_study pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* l_obj = pqm_study->GetItemFromProtocol(f_prot);	//-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //DB_FUNC_ENTER("[PqmSarGetNewRfl]:");

    SetProtocolSARStats(l_obj, sarStatTsuccess);
    pqmSarStat = sarStatTsuccess;

    /*SarSetDebug(SarDebugStdout);*/
    /*** Get RFL data from Vf. ***/

    ClibSARAdapter sarAdapter(f_vf_handle);
    sarStat_t	l_sarStat = sarStatTsuccess;
    l_sarStat = sarAdapter.SarGetVfRflParams(f_prot, &rflParams);

    //rflParams.rflTxGain = 56.00;
    if (l_sarStat != sarStatTsuccess) {
        SetProtocolSARStats(l_obj, l_sarStat);
        pqmSarStat = l_sarStat;
        //DB_PRINTF("",("----->>> SarGetVfRflParams ERROR !!\n"));
    }

    l_sarStat = sarAdapter.SarGetVfTGCRFoutRatioHighSAR(f_prot, &tgcParams);

    //DB_PRINTF("S",("Protocol : %d TGC = %g\n",prot,tgcParams.TGCRFoutRatio));
    if (l_sarStat != sarStatTsuccess) {
        SetProtocolSARStats(l_obj, l_sarStat);
        pqmSarStat = l_sarStat;
        //DB_PRINTF("",("----->>> SarGetVfTGCRFoutRatioHighSAR ERROR !!\n"));
    }

    //PqmPqReMakeList(f_vfd,obj->pos,PQM_STS_LIST,TRUE);
}

//-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
//****************************Method Header************************************
//Method Name   : SetNewRfl()
//Author        : PATNI\MRP
//Purpose       : This functions sets the new RFL's and recalculates the SAR
//*****************************************************************************
void CPQMSAR::SetNewRfl(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::SetNewRfl");
    PQM_SCOPED_TRACE();

    CString l_print_string(_T(""));

    if ((NULL == f_protocol) || (NULL == f_vf_handle)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle or f_protocol is NULL"));
        return;
    }

    l_print_string.Format(_T("Protocol (Position, Acqorder) = (%d, %d)"), f_protocol->GetPosition(), f_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    sarStat_t l_sarStat = sarStatTsuccess;
    sarInfo_t* l_sarInfo = (f_protocol->GetSarInfo());
    // Set only RFL data to sarInfo.

    l_sarStat = SarSetRflParams(l_sarInfo, &rflParams);

    if (l_sarStat != sarStatTsuccess) {

        //+Patni-HAR/2009Mar28/Modified/PSP1#313
        /*l_sarInfo->tsar[l_sarInfo->active].sar = 0.0;
        l_sarInfo->tsar[l_sarInfo->active].preal = 0.0;
        l_sarInfo->coilProtect = FALSE;
        l_sarInfo->scanEnable = TRUE;*/
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].sar
        = 0.0f;
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].preal
        = 0.0f;
        f_protocol->GetSarInfo()->coilProtect   = FALSE;
        f_protocol->GetSarInfo()->scanEnable    = TRUE;
        //+Patni-HAR/2009Mar28/Modified/PSP1#313

        f_protocol->SetSARStat(l_sarStat) ;		/* V9.25 */
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetRflParams ERROR"));

        return;
    }

    l_sarInfo->TGCRFoutRatio = tgcParams.TGCRFoutRatio;

    ClibSARAdapter sarAdapter(f_vf_handle);
    l_sarStat = sarAdapter.SarPutVfRflParams(f_protocol->GetProtocol(), &rflParams);

    if (l_sarStat != sarStatTsuccess) {

        //+Patni-HAR/2009Mar28/Modified/PSP1#313
        /*l_sarInfo->tsar[l_sarInfo->active].sar = 0.0;
        l_sarInfo->tsar[l_sarInfo->active].preal = 0.0;
        l_sarInfo->coilProtect = FALSE;
        l_sarInfo->scanEnable = TRUE;*/
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].sar
        = 0.0f;
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].preal
        = 0.0f;
        f_protocol->GetSarInfo()->coilProtect   = FALSE;
        f_protocol->GetSarInfo()->scanEnable    = TRUE;
        //+Patni-HAR/2009Mar28/Modified/PSP1#313

        f_protocol->SetSARStat(l_sarStat) ;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarPutVfRflParams ERROR"));

    }

    l_sarStat = sarAdapter.SarPutVfTGCRFoutRatioHighSAR(f_protocol->GetProtocol(), &tgcParams);

    if (l_sarStat != sarStatTsuccess) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarPutVfTGCRFoutRatioHighSAR  ERROR"));

        f_protocol->SetSARStat(l_sarStat);
    }

    l_print_string.Format(_T("Protocol Acqorder = %d : TGC = %g"), f_protocol->GetProtocol(), tgcParams.TGCRFoutRatio);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

}
//-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

//****************************Method Header************************************
//Method Name   : CalculateSAR()
//Author        : PATNI\MRP
//Purpose       : Calc sep gain,and calc sar value
//*****************************************************************************
void CPQMSAR::CalculateSAR(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const	f_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMSAR::CalculateSAR");
    PQM_SCOPED_TRACE();

    if (f_protocol == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return;
    }

    CString l_str_trace ;
    l_str_trace.Format(_T("Protocol (Pos, Acqorder) = (%d, %d)"), f_protocol->GetPosition(), f_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_trace);

    //DB_PRINTF("S",("Protocol : %d\n",prObj_p->prot));
    sarInfo_t*		l_sarInfo = (f_protocol->GetSarInfo());

    if (rflParams.rflTxGain < MIN_SAR) {

        l_str_trace.Format(_T("rflTxGain is too small %g"), rflParams.rflTxGain);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_trace);

        //DB_PRINTF("S",("rflTxGain is too small, %g\n",rflParams.rflTxGain));
        //+Patni-HAR/2009Mar28/Modified/PSP1#313
        /*l_sarInfo->tsar[l_sarInfo->active].sar = 0.0;
        l_sarInfo->tsar[l_sarInfo->active].preal = 0.0;
        l_sarInfo->coilProtect = FALSE;
        l_sarInfo->scanEnable = TRUE;*/
        //Patni-SS/MODDI/2009June04/JFT#249
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].sar = 0.0f;
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].preal = 0.0f;
        f_protocol->GetSarInfo()->coilProtect = FALSE;
        f_protocol->GetSarInfo()->scanEnable = TRUE;
        //+Patni-HAR/2009Mar28/Modified/PSP1#313
        //DB_FUNC_EXIT();
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        return;
    }

    sarInfo_p		l_pInfo = (f_protocol->GetSarInfo());

    l_pInfo->patientWeight = m_pqm->GetStudy()->GetWeight();
    l_pInfo->patientHeight = m_pqm->GetStudy()->GetHeight();	/**** V9.01 add  ****/
    //l_pInfo->patientHeight = 180;

    /*** Calc Sep gain. ***/
    sarStat_t       l_sarStat = sarStatTsuccess;
    l_sarStat = SarCalcSepGain(l_pInfo);

    if (l_sarStat != sarStatTsuccess) {
        l_pInfo->tsar[l_pInfo->active].sar = 0.0;
        l_pInfo->tsar[l_pInfo->active].preal = 0.0;
        l_pInfo->coilProtect = FALSE;
        l_pInfo->scanEnable = TRUE;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("====>>> pqmSarCalc: SarCalcSepGain ERROR ")));

        f_protocol->SetSARStat(l_sarStat);		/* V9.25 */
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        return;
    }

    /*** Calc SAR values. ***/
    CPQMStudy* l_pqm_study = m_pqm->GetStudy();

    if (NULL == l_pqm_study) {
        return ;
    }

    l_pInfo->sae.sae_prev = l_pqm_study->GetTotalSAE(l_pqm_study->GetPrevItemFromPos(f_protocol));

    ClibSARAdapter sarAdapter(f_vf_handle);

    sarVfPreScanParam_t l_vf_prescan_param = {0};

    if (VFC_AMB_MODE_SLAVE != f_protocol->GetAMBModeFlag()) {

        sarAdapter.SarGetVfPreScanParam(f_protocol->GetProtocol(), &l_vf_prescan_param);
    }

    ::SarSetPreScanParam(l_sarInfo, &l_vf_prescan_param);

    if (!m_pqm->GetAllCoverageRgn()) {
        l_sarInfo->preScanParam.RGNParam.numCoverage = 1;
    }

    int l_prot = f_protocol->GetProtocol();
    l_sarStat = sarAdapter.SarCalcAndPutVf(l_prot, l_pInfo);

    if (l_sarStat != sarStatTsuccess) {
        l_pInfo->tsar[l_pInfo->active].sar = 0.0;
        l_pInfo->tsar[l_pInfo->active].preal = 0.0;
        l_pInfo->coilProtect = FALSE;
        l_pInfo->scanEnable = TRUE;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("====>>> SarCalcAndPutVf ERROR ")));
        //DB_PRINTF("",("====>>> SarCalcAndPutVf ERROR\n"));
        f_protocol->SetSARStat(l_sarStat);	/* V9.25 */
        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
        //m_pqm->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
    }

    //    DB_PRINTF("S",("====>>> SAR =========\n"));
    //    DB_PRINTF("S",("sar = %f\n",pInfo->tsar[pInfo->active].sar));
    //    DB_PRINTF("S",("preal = %f\n",pInfo->tsar[pInfo->active].preal));
    //    DB_PRINTF("S",("coilProtect = %d\n",(int)pInfo->coilProtect));
    //    DB_PRINTF("S",("scanEnable = %d\n",(int)pInfo->scanEnable));
    //
    //    DB_FUNC_EXIT();

}

//****************************Method Header************************************
//Method Name   : DownReCalcSAR()
//Author        : PATNI\MRP
//Purpose       : This functions sets the new RFL's and recalculates the SAR
//*****************************************************************************
void CPQMSAR::DownReCalcSAR(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const	f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::DownReCalcSAR");
    PQM_SCOPED_TRACE();

    if (NULL == f_protocol) {
        return;
    }

    rflParams.rflTxGain *= (float)0.9 ;
    f_protocol->SetSARInit(FALSE);

    if (m_pqm) {
        CPQMStudy* pqm_study = m_pqm->GetStudy();

        if (pqm_study) {
            pqm_study->AddPrescanTagsForNextProtocol(
                pqm_study->GetVfDBHandle(),
                NULL,
                f_protocol->GetProtocol());
        }
    }

    RecalculateSAR(f_vf_handle, f_protocol) ;
}

void CPQMSAR::pqmSarSetNewCouch(CPqmProtocol* const	f_protocol)
{
    int		no = 0;
    float	couchPos = 0.0f, homepos = 0.0f;
    CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();
    l_pqmshimshareddat->CopyCouchInfo(&no, &couchPos, &homepos);
    f_protocol->GetSarInfo()->couchPosition = couchPos;
}

//**************************************************************************
//Method Name   : pqmSarSetVfRfl
//Author        : PATNI/Manish
//Purpose       :
//**************************************************************************
void CPQMSAR::pqmSarSetVfRfl(CVarFieldHandle* const f_vf_handle, CPqmProtocol* const	f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::pqmSarSetVfRfl");
    PQM_SCOPED_TRACE();

    sarStat_t		sarStat = sarStatTsuccess;
    sarVfRflParams_t	vfRflParams;
    sarVfTGCParams_t	vfTgcParams;

    bzero(&vfRflParams, sizeof(vfRflParams));
    bzero(&vfTgcParams, sizeof(vfTgcParams));

    //+Patni-HAR/2010Jun03/Added/MVC008094
    ClibSARAdapter sarAdapter(f_vf_handle);
    sarStat = sarAdapter.SarGetVfRflParams(f_protocol->GetProtocol(), &vfRflParams);
    //-Patni-HAR/2010Jun03/Added/MVC008094

    if (sarStat != sarStatTsuccess) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfRflParams ERROR"));

        f_protocol->SetSARStat(sarStat);
        return;
    }

    //+Patni-HAR/2010Jun03/Added/MVC008094
    sarStat = sarAdapter.SarGetVfTGCRFoutRatioHighSAR(f_protocol->GetProtocol(), &vfTgcParams);
    //-Patni-HAR/2010Jun03/Added/MVC008094

    if (sarStat != sarStatTsuccess) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarGetVfTGCRFoutRatioHighSAR  ERROR"));

        f_protocol->SetSARStat(sarStat);
        return;
    }

    sarInfo_t* l_sarInfo = f_protocol->GetSarInfo();

    sarStat = SarSetRflParams(l_sarInfo, &vfRflParams);

    if (sarStat != sarStatTsuccess) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarSetRflParams  ERROR"));

        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].sar = 0.0;
        f_protocol->GetSarInfo()->tsar[f_protocol->GetSarInfo()->active].preal = 0.0;
        f_protocol->GetSarInfo()->coilProtect = FALSE;
        f_protocol->GetSarInfo()->scanEnable = TRUE;
        f_protocol->SetSARStat(sarStat);
        return;
    }

    //+Patni-HAR/2010Jun03/Added/MVC008094
    sarStat = sarAdapter.SarPutVfTGCRFoutRatioHighSAR(f_protocol->GetProtocol(), &vfTgcParams);
    //-Patni-HAR/2010Jun03/Added/MVC008094
    //f_protocol->GetSarInfo()->TGCRFoutRatio = vfTgcParams.TGCRFoutRatio;
}

//****************************Method Header************************************
//Method Name   : GetSarStat()
//Author        : PATNI\MRP
//Purpose       : This functions returns the SARStatus
//*****************************************************************************
sarStat_t CPQMSAR::GetSarStat(
) const
{
    return pqmSarStat;
}

//****************************Method Header************************************
//Method Name   : SetProtocolSARStats()
//Author        : PATNI\MRP
//Purpose       : This functions sets the sar status for all the protocols after it
//*****************************************************************************
void CPQMSAR::SetProtocolSARStats(
    CPqmProtocol* const l_startobj,
    const sarStat_t l_sarStat
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSAR::SetProtocolSARStats");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    if (l_startobj == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_startobj pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections

    CPQMStudy* l_study = m_pqm->GetStudy();
    //DB_FUNC_ENTER("[PqmSarReCalc]:");

    l_startobj->SetSARStat(l_sarStat);
    CPqmProtocol*	 l_obj = l_startobj;
    CPqmProtocol*	 l_next = NULL;

    while ((l_next = l_study->GetNextItem(l_obj)) != NULL) {

        if (l_next == NULL || l_next->GetProtocol() == PQ_FILLER)
            continue;

        else {
            l_next->SetSARStat(l_sarStat);
        }

        l_obj = l_next;
    }

    return;
}







