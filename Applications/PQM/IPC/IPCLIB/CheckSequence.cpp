//REDMINE-1213_Update_03_MAY
//REDMINE-1213
//Redmine-783
//Redmine-777
//Redmine-780
//Redmine-781
// CheckSequence.cpp: implementation of the CCheckSequence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckSequence.h"

//Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space
#include <errno.h>
#include <DBServer/MpDbsBaseObjectInfo.h>
#include <DBServer/MpDbsDBServer_Defs.h>
#include <tami/libmsVf/libmsData.h>
#include <tami/libmsVf/libmsVf.h>
#include <PQM/PQMLogMgr.h>
#include "pqm.h"
#include "PqmProtocol.h"
#include "coil.h"
#include "PqmCoilData.h"
#include "PqmPrMode.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "CoilChannelMode.h"
#include "PqmSectionData.h"
using namespace MPlus::DBServer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//********************************Method Header********************************
//Method Name   :CCheckSequence()
//Author        :PATNI/Mangesh
//Purpose       :Constructor
//*****************************************************************************
CCheckSequence::CCheckSequence(CPqm* f_pqmptr):
    m_pqmptr(f_pqmptr)
{

}
//********************************Method Header********************************
//Method Name   :~CCheckSequence()
//Author        :PATNI/Mangesh
//Purpose       :Destructor
//*****************************************************************************
CCheckSequence::~CCheckSequence()
{
}

//+Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space
//**************************************************************************
//Method Name   : SkipMPlusDiskCheck
//Author        : Patni\AMT
//Purpose       : Should check file in /usr/tmp folder. If file present,
//                 PQM should skip MPlus DB Free Space check. If not PQM should
//                 Check it.
//**************************************************************************
BOOL CCheckSequence::SkipMPlusDiskCheck()const
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::SkipMPlusDiskCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    struct _stat l_file_check;
    char l_path[512] = {0};
    char* l_buf = getenv("MRMP_USR_TMP_DIR");

    strcpy(l_path, l_buf);
    strcat(l_path, "\\SkipMPlusDiskCheck\0");

    BOOL l_file_exist = FALSE;

    if (0 != _stat(l_path, &l_file_check)) {

        if (ENOENT != GetLastError()) {
            l_file_exist = TRUE;
        }

    } else {
        l_file_exist = TRUE;
    }

    CString l_log_msg = _T("");
    l_log_msg.Format(_T("File=%s-->Skip=%d"), CString(l_path), l_file_exist);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    return l_file_exist;
}
//-Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space

//********************************Method Header********************************
//Method Name   :CheckDiskSpace()
//Author        :PATNI/Mangesh
//Purpose       :Checks total free disk space
//*****************************************************************************
int CCheckSequence::CheckDiskSpace(CPqmProtocol* const f_protocol
                                  )const
{
    //Patni-AMT/2009Aug30/Added/Code Review
    LPCTSTR FUNC_NAME = _T("CCheckSequence::CheckDiskSpace");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Checking free space data"));

    status_t l_status = E_ERROR;

    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Fatal Error m_pqmptr is NULL"));
        return l_status;
    }

    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study_ptr = m_pqmptr->GetStudy();

    CVarFieldHandle* l_vf_handle = l_study_ptr->GetVfDBHandle();

    if (!l_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
        return E_ERROR;
    }

    // To get Protocol index which is one of parameter of msCheckFtMatrix()
    int32_t  l_protocol_index = f_protocol->GetProtocol();

    BITFLD_DECL(mask, K_ALL_PR) = {0};	//Patni-AD/2009May28/Modified/SU14 //cpp test corrections

    ClibmsCalcSizeAdapter calcSizeAdapter(l_vf_handle, l_protocol_index);

    CString l_str_msg(_T(""));
    int l_appcode = f_protocol->GetAppCode();
    int l_matrix_check = PQM_ERROR_NORMAL;

    if (l_appcode != VFC_APP_CODE_MRS &&
        l_appcode != VFC_APP_CODE_MRSCSI) {

        int32_t	l_peftmatrix = 0;
        int32_t	l_roftmatrix = 0;
        int l_status = -1;
        l_status = calcSizeAdapter.msCheckFtMatrix(&l_peftmatrix, &l_roftmatrix);

        if (l_status > 0) {
            l_str_msg.Format(_T("l_peFtMatrix =%d l_roftmatrix=%d"), l_peftmatrix, l_roftmatrix);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

            if (l_status == 2) {

                return PQM_ERROR_MATRIX_OVER;

            } else if (l_status == 1) {
                l_matrix_check = PQM_WARNING_MATRIX_OVER;
            }
        }
    }

    int16_t	l_num_channels = 0;
    unsigned int l_pac_pattern = 0u;	/* channel mapping */ //Patni-AD/2009May28/Modified/SU14 //cpp test corrections
    PqmCoil_t* l_coil_ptr = NULL;

    if (m_pqmptr->IsSaturnCoilSystem()) {
        PqmSaturnCoil_t* l_saturn_coil = f_protocol->GetSaturnCoil();

        for (int l_count = 0 ; l_count < l_saturn_coil->numSection ; l_count++) {
            l_num_channels += l_saturn_coil->Signal[l_count].num ;
        }

        l_coil_ptr = f_protocol->GetCoilDBTable()->coiltbl_p[0];

    } else {

        CPqmCoilData* l_pqmcoildata = m_pqmptr->GetDataManager()->GetPQMCoilData();
        CCoil l_coil;

        bool l_ret = l_pqmcoildata->PqmCoilGetCoil(f_protocol->GetPqmPrMode()->GetRecvCoilId(), l_coil);

        l_num_channels = l_coil_ptr->num_channels;
        l_pac_pattern = l_coil_ptr->pac_pattern;


    }

    if (l_coil_ptr != NULL) {
        UpdateCoilRelatedData(f_protocol);

        if (m_pqmptr->GetVAP()) {

            int	        l_reconmode(0), l_len = 0;
            char        l_sequence[128] = {0};
            l_len = sizeof(l_sequence);
            memset(mask, 0, sizeof(mask));
            int32_t     l_postacqmode(0), l_flyflag(0), l_mode(0);
            l_study_ptr->GetProtocolValues(l_vf_handle, l_protocol_index, mask,
                                           K_PR_POST_ACQ_MODE, &l_postacqmode, NULL,
                                           K_PR_FLY_FLAG, &l_flyflag, NULL,
                                           K_PR_SEQUENCE_NAME, l_sequence, &l_len,
                                           K_PR_IMAGING_MODE, &l_mode, NULL, NULL);


            //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window

            CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("postAcqMode = 0x%x"), l_postacqmode);

            if (l_postacqmode != VFC_MODE_NONE) {
                ClibmsVfAdapter libmsvfAdapter(l_vf_handle);
                l_status = libmsvfAdapter.msVfReconMode(l_protocol_index, &l_reconmode);

                //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
                if_no_error(l_status) {
                    int32_t	l_other(0), l_newval = 0;

                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("msVfReconMode -> reconMode = 0x%x"), l_reconmode);

                    l_other = l_postacqmode & ~(VFC_MODE_AUTO_RECON |
                                                VFC_MODE_CONCURRENT_RECON |
                                                VFC_MODE_DYNAMIC_RECON);
                    l_newval = l_reconmode | l_other;

                    if (l_reconmode & VFC_MODE_AUTO_RECON)
                        l_newval |= (l_postacqmode & VFC_MODE_DYNAMIC_RECON);

                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("newval = 0x%x"), l_newval);

                    if (l_postacqmode != l_newval) {

                        l_postacqmode = l_newval;

                        if (E_NO_ERROR != l_study_ptr->SetProtocolValues(l_vf_handle, l_protocol_index, mask,
                                K_PR_POST_ACQ_MODE, &l_postacqmode, sizeof(int32_t), NULL)) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
                        }
                    }//end if
                }

                else {
                    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msVfReconMode() failed."));
                }
            }//end if
        }//end if
    }

    uint32_t l_freesize = 0;
    l_status = calcSizeAdapter.calcTotalSize(&l_freesize);

    if (l_status != E_NO_ERROR) {

        switch (l_status) {

            case E_DISK_SPACE:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("No Disk Space (%d)"), l_freesize);
                return PQM_ERROR_HOSTDISK_SPACE;

            case E_OVER_2GB:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over 2Gbyte (%d)"), l_freesize);
                return PQM_ERROR_OVER_2GB;

                //+Patni-HAR/2009Dec3/Added/DeFT# MVC005533+MCM0229-00114
            case E_OVER_IMGSRV_MEMORY:
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over Image Srv Memory  (%d)"), l_freesize);
                return PQM_ERROR_IMGSRV_MEMORY;
                //-Patni-HAR/2009Dec3/Added/DeFT# MVC005533+MCM0229-00114

            default:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_freesize);

                return PQM_ERROR_VAP_STUDY;
        }//end switch
    }//end if

    char* l_engine = NULL;

    if ((l_engine = getenv(ENGINE)) != NULL) {
        if (!strcmp(l_engine , CONNECT) && f_protocol->GetFluoro())	{
            if (l_freesize < PLAY_REVERSE_SIZE) {
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Fluoro EngineDisk space Error"));
                return PQM_ERROR_ENGINEDISK_SPACE;
            }
        }
    }

    l_status = calcSizeAdapter.calcTotalEngineSize(&l_freesize);

    if (l_status != E_NO_ERROR) {

        switch (l_status) {

            case E_DISK_SPACE:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("No Disk Space (%d)"), l_freesize);
                return PQM_ERROR_ENGINEDISK_SPACE;

            case E_OVER_2GB:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over 2Gbyte (%d)"), l_freesize);
                return PQM_ERROR_OVER_2GB;

            default:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_freesize);
                return PQM_ERROR_VAP_STUDY;
        }
    }

    if (m_pqmptr->GetVAP()) {
        l_status = calcSizeAdapter.calcTotalVapSize(&l_freesize);

        if (l_status != E_NO_ERROR) {

            switch (l_status) {
                case E_OVER_VAP_MEMORY:
                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over VAP Memory (%d)"), l_freesize);
                    return PQM_ERROR_VAP_NO_SPACE;

                case E_OVER_MRS_DATA :
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over MRS Data (%d)"), l_freesize);
                    return PQM_ERROR_OVER_MRS_DATA;

                default:
                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_freesize);
                    return PQM_ERROR_VAP_STUDY;
            }

        }
    }

    l_status = CheckVapRatio(l_vf_handle, l_protocol_index) ;

    if (l_status != E_NO_ERROR) {
        //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Not enough VAP Space (%d)"), l_freesize);
        return PQM_ERROR_VAP_RATIO_OVER;
    }

    if (PQM_WARNING_MATRIX_OVER == l_matrix_check) {
        return l_matrix_check;
    }

    return l_status;
}

//********************************Method Header********************************
//Method Name   :CheckMultiProtocolDiskSpace()
//Author        :PATNI/Mangesh
//Purpose       :Checks total Multiprotocol free disk space
//*****************************************************************************
int CCheckSequence::CheckMultiProtocolDiskSpace(int32_t* f_prot_tbl, const int f_num, uint32_t& f_amb_image_totalsize)const
{

    //Patni-AMT/2009Aug30/Added/Code Review
    LPCTSTR FUNC_NAME = _T("CCheckSequence::CheckMultiProtocolDiskSpace");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    ProtocolTable_t	l_protocol;

    l_protocol.num = f_num ;
    l_protocol.protocol_table = f_prot_tbl;

    CPQMStudy* l_study_ptr = m_pqmptr->GetStudy();
    ClibmsCalcSizeAMBAdapter calcSizeAMBAdapter(l_study_ptr->GetVfDBHandle(), l_protocol);

    uint32_t  l_after_amb_disksize = 0;
    status_t l_status = calcSizeAMBAdapter.calcTotalSizeAMB(&l_after_amb_disksize);

    if (l_status != E_NO_ERROR) {

        switch (l_status) {

            case E_DISK_SPACE:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Not enough Disk Space (%d)"), l_after_amb_disksize);
                return PQM_ERROR_HOSTDISK_SPACE;

            case E_OVER_2GB:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over 2Gbyte (%d)"), l_after_amb_disksize);
                return PQM_ERROR_OVER_2GB;

                //+Patni-HAR/2009Dec3/Added/DeFT# MVC005533+MCM0229-00114
            case E_OVER_IMGSRV_MEMORY:
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over Image Srv Memory (%d)"), l_after_amb_disksize);
                return PQM_ERROR_IMGSRV_MEMORY;
                //-Patni-HAR/2009Dec3/Added/DeFT# MVC005533+MCM0229-00114

            default:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_after_amb_disksize);
                return PQM_ERROR_VAP_STUDY;
        }

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("calcTotalSizeAMB() failed."));
    }

    uint32_t l_freesize = 0;
    l_status = calcSizeAMBAdapter.calcTotalEngineSizeAMB(&l_freesize);

    if (l_status != E_NO_ERROR) {
        switch (l_status) {

            case E_DISK_SPACE:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Not enough Disk Space (%d)"), l_freesize);

                return PQM_ERROR_ENGINEDISK_SPACE;

            case E_OVER_2GB:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Over 2Gbyte (%d)"), l_freesize);

                return PQM_ERROR_OVER_2GB;

            default:
                //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_freesize);
                return PQM_ERROR_VAP_STUDY;
        }

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("calcTotalEngineSizeAMB() failed."));
    }

    uint32_t  l_current_disksize = 0;
    l_status = msGetHostDiskSize(&l_current_disksize);

    if (l_status != E_NO_ERROR) {
        switch (l_status) {
            case E_ERROR:
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("/gp/queue is not exist, system call error "));
                return E_ERROR;

            case E_DISK_SPACE:
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Remaining size is 0 : (%d)"), l_current_disksize);
                return PQM_ERROR_ENGINEDISK_SPACE;

            default:
                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Study Error (%d)"), l_current_disksize);
                return PQM_ERROR_VAP_STUDY;
        }
    }

    f_amb_image_totalsize  = (l_current_disksize - l_after_amb_disksize);
    return l_status;
}
int CCheckSequence::CheckAMBDiskSpace(uint32_t f_amb_image_totalsize)
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::CheckAMBDiskSpace");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    DBS_DB_STATUS_t     l_DB_status;
    MpDbsBaseObjectInfo l_obj_base_info;

    if (MP_DBS_SUCCESS == l_obj_base_info.DBStatusCheck(l_DB_status)) {

        if (f_amb_image_totalsize <= (uint32_t)l_DB_status.lFreePixelSpace) {
            return E_NO_ERROR;

        } else {
            return E_ERROR;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBServer is not running"));
        return E_ERROR;
    }

}
//********************************Method Header********************************
//Method Name   :PqmPrSetChannelGainData()
//Author        :PATNI/Mangesh
//Purpose       :
//*****************************************************************************
int CCheckSequence::PqmPrSetChannelGainData(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* const f_protocol,
    PqmCoil_t* const l_coil_ptr
)const
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::PqmPrSetChannelGainData");

    if (!f_vf_handle) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t l_path;

    l_path.subtree_name = SVN_ST_PROTOCOL;
    l_path.index = f_protocol->GetProtocol();

    VfFieldSpec_t l_vfReq[1] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    VFF_SET_ARGS(l_vfReq[0], SVN_EXE_CHANNEL_GAIN_RATIO, SVT_EXE_CHANNEL_GAIN_RATIO,
                 sizeof(l_coil_ptr->gain_data), l_coil_ptr->gain_data, VFO_REPLACE);

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString str = _T("");
    str.Format(_T("Protocol = %d, l_coil_ptr->gain_data = %d,"), f_protocol->GetProtocol(), l_coil_ptr->gain_data);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, str);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int	l_num = 0;
    time_t l_tm = 0L;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    status_t l_status = f_vf_handle->PutFields(&l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_tm);

    if (E_NO_ERROR != l_status || 1 != l_num) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    //+Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("ChannelGainData %g %g %g %g %g %g %g %g"),
            l_coil_ptr->gain_data[0],
            l_coil_ptr->gain_data[1],
            l_coil_ptr->gain_data[2],
            l_coil_ptr->gain_data[3],
            l_coil_ptr->gain_data[4],
            l_coil_ptr->gain_data[5],
            l_coil_ptr->gain_data[6],
            l_coil_ptr->gain_data[7]);
    //-Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
    return l_status;
}

//********************************Method Header********************************
//Method Name   :CheckVapRatio()
//Author        :PATNI/Mangesh
//Purpose       :Checks Vap ratio
//*****************************************************************************
int CCheckSequence::CheckVapRatio(CVarFieldHandle* const f_vf_handle, const int f_prot)const
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::CheckVapRatio");

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    PqmAcqRatio_t	l_acqratiotbl ;

    memset(&l_acqratiotbl , 0 , sizeof(l_acqratiotbl)) ;

    if (GetNumPointsforTR(f_vf_handle , f_prot, &l_acqratiotbl) == E_ERROR)
        return E_ERROR ;

    l_acqratiotbl.m_tr *= CHECK_SCALE ;
    l_acqratiotbl.m_grad_echo_space *= CHECK_SCALE ;
    l_acqratiotbl.m_rf_echo_space *= CHECK_SCALE ;

    float	l_num_points = l_acqratiotbl.m_matrix.y
                           * l_acqratiotbl.m_rf_echoes
                           * l_acqratiotbl.m_epi_echoes
                           * l_acqratiotbl.m_num_echoes
                           * l_acqratiotbl.m_channel_prevfep ;

    if (!(l_acqratiotbl.m_app_code == VFC_APP_CODE_ANGIO ||
          l_acqratiotbl.m_app_code == VFC_APP_CODE_PHASE_SHIFT ||
          l_acqratiotbl.m_app_code == VFC_APP_CODE_FAST_FIELD_ECHO ||
          l_acqratiotbl.m_app_code == VFC_APP_CODE_FFE_EPI ||
          l_acqratiotbl.m_imaging_mode == VFC_IMAGING_MODE_3D)) {

        l_num_points *= l_acqratiotbl.m_num_slices ;
    }

    if (l_acqratiotbl.m_imaging_mode ==  VFC_IMAGING_MODE_3D &&
        l_acqratiotbl.m_num_slabs != 1) {

        l_num_points *= l_acqratiotbl.m_num_slabs ;
    }

    //DB_PRINTF("Q",("%f(us) vs %f(us) \n",
    //(acqRatiotbl.TR/num_points), (PqmGlobal.vfepProcRatio)));

    float l_vfep_proc_ratio = m_pqmptr->GetVFEPProcRatio();

    if (((l_acqratiotbl.m_tr) / l_num_points)
        < (l_vfep_proc_ratio)) {

        return  E_ERROR ;
    }

    return E_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   :GetNumPointsforTR()
//Author        :PATNI/Mangesh
//Purpose       :
//*****************************************************************************
int CCheckSequence::GetNumPointsforTR(CVarFieldHandle* const f_vf_handle,
                                      const int f_prot, PqmAcqRatio_p const f_acqdata)const
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::GetNumPointsforTR");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!f_vf_handle) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    static VfPathElem_t l_path[] = {
        SVN_ST_PROTOCOL,  0,
        SVN_ST_SUBPROTOCOL, 0,
    };

    l_path[SVD_ST_PROTOCOL - 1].index = f_prot ;
    VfFieldSpec_t l_vfreq[12] = {0};
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 12);
    int l_n = 0;
    f32vec2_t l_matrix ;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_SEQ_ACQ_MATRIX, VFT_FLTVEC2,
                 sizeof(l_matrix), &l_matrix, 0L);
    l_n++;
    int32_t l_rf_echoes = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_NUM_RF_ECHOES, VFT_INT32,
                 sizeof(l_rf_echoes), &l_rf_echoes, 0L);
    l_n++;
    int32_t l_epi_echoes = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_NUM_EPI_ECHOES, VFT_INT32,
                 sizeof(l_epi_echoes), &l_epi_echoes, 0L);
    l_n++;
    flt32_t l_tr = 0.0f;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_SEQ_DETAIL_TR, VFT_FLT32,
                 sizeof(l_tr), &l_tr, 0L);
    l_n++;
    flt32_t l_grad_echo_space = 0.0f;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_SEQ_GRAD_ECHO_SPACE, VFT_FLT32,
                 sizeof(l_grad_echo_space), &l_grad_echo_space, 0L);
    l_n++;
    flt32_t l_rf_echo_space = 0.0f ;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_SEQ_RF_ECHO_SPACE, VFT_FLT32,
                 sizeof(l_rf_echo_space), &l_rf_echo_space, 0L);
    l_n++;
    int32_t l_channel_prevfep = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_CHANNEL_PER_VFEP, VFT_INT32,
                 sizeof(l_channel_prevfep), &l_channel_prevfep, 0L);
    l_n++;
    int32_t l_imaging_mode = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_IMAGING_MODE, VFT_INT32,
                 sizeof(l_imaging_mode), &l_imaging_mode, 0L);
    l_n++;
    int32_t l_app_code = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_APP_CODE, VFT_INT32,
                 sizeof(l_app_code), &l_app_code, 0L);
    l_n++;
    int32_t l_num_slabs = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_NUM_SLABS, VFT_INT32,
                 sizeof(l_num_slabs), &l_num_slabs, 0L);
    l_n++;
    int32_t l_num_echoes = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_NUMBER_OF_ECHOES, VFT_INT32,
                 sizeof(l_num_echoes), &l_num_echoes, 0L);
    l_n++;

    int l_num = 0;
    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vfreq, l_n, &l_num);
    if_error(l_status) {
        vfGetFuncError("msMipCreateEchoTree:", l_status);
        return E_ERROR;
    }

    if (l_vfreq[0].status == VFE_NOT_FOUND)
        return E_ERROR;

    else
        memcpy(&f_acqdata->m_matrix , &l_matrix , sizeof(l_matrix));


    if ((l_vfreq[1].status == VFE_NOT_FOUND) || (l_rf_echoes == 0))
        f_acqdata->m_rf_echoes = 1 ;

    else
        f_acqdata->m_rf_echoes = l_rf_echoes ;


    if ((l_vfreq[2].status == VFE_NOT_FOUND) || (l_epi_echoes == 0))
        f_acqdata->m_epi_echoes = 1 ;

    else
        f_acqdata->m_epi_echoes = l_epi_echoes;


    if (l_vfreq[3].status == VFE_NOT_FOUND)
        return E_ERROR;

    else
        f_acqdata->m_tr = l_tr ;


    if ((l_vfreq[4].status == VFE_NOT_FOUND) || (l_grad_echo_space == 0))
        f_acqdata->m_grad_echo_space = 0 ;

    else
        f_acqdata->m_grad_echo_space = l_grad_echo_space ;


    if ((l_vfreq[5].status == VFE_NOT_FOUND) || (l_rf_echo_space == 0))
        f_acqdata->m_rf_echo_space = 0 ;

    else
        f_acqdata->m_rf_echo_space = l_rf_echo_space ;


    if ((l_vfreq[6].status == VFE_NOT_FOUND) || (l_channel_prevfep == 0))
        f_acqdata->m_channel_prevfep = 0 ;

    else
        f_acqdata->m_channel_prevfep = l_channel_prevfep ;


    if ((l_vfreq[7].status == VFE_NOT_FOUND) || (l_imaging_mode == 0))
        f_acqdata->m_imaging_mode = 0 ;

    else
        f_acqdata->m_imaging_mode = l_imaging_mode ;


    if ((l_vfreq[8].status == VFE_NOT_FOUND) || (l_app_code == 0))
        f_acqdata->m_app_code = 0 ;

    else
        f_acqdata->m_app_code = l_app_code ;


    if ((l_vfreq[9].status == VFE_NOT_FOUND) || (l_num_slabs == 0))
        f_acqdata->m_num_slabs = 1 ;

    else
        f_acqdata->m_num_slabs = l_num_slabs ;


    if ((l_vfreq[10].status == VFE_NOT_FOUND) || (l_num_echoes == 0))
        f_acqdata->m_num_echoes = 1 ;

    else
        f_acqdata->m_num_echoes = l_num_echoes ;


    if (f_acqdata->m_matrix.y == 0  || f_acqdata->m_tr == 0)
        return E_ERROR;

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 12);
    l_n = 0 ;
    int32_t l_num_slices = 0;

    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_SLICES_IN_SP, VFT_INT32,
                 sizeof(l_num_slices), &l_num_slices, 0L);
    l_n++;

    l_status = f_vf_handle->GetFields(l_path, SVD_ST_SUBPROTOCOL, l_vfreq, l_n, &l_num);
    if_error(l_status) {
        vfGetFuncError("msMipCreateEchoTree:", l_status);
        return E_ERROR;
    }

    if ((l_vfreq[0].status == VFE_NOT_FOUND) || (l_num_slices == 0))
        f_acqdata->m_num_slices = 1 ;

    else
        f_acqdata->m_num_slices = l_num_slices ;


    return E_NO_ERROR;
}
//-Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space

int CCheckSequence::UpdateCoilRelatedData(CPqmProtocol* const f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CCheckSequence::CheckDiskSpace");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Checking free space data"));

    status_t l_status = E_ERROR;

    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Fatal Error m_pqmptr is NULL"));
        return l_status;
    }

    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study_ptr = m_pqmptr->GetStudy();

    CVarFieldHandle* l_vf_handle = l_study_ptr->GetVfDBHandle();

    if (!l_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
        return E_ERROR;
    }

    // To get Protocol index which is one of parameter of msCheckFtMatrix()
    int32_t  l_protocol_index = f_protocol->GetProtocol();

    BITFLD_DECL(mask, K_ALL_PR) = {0};	//Patni-AD/2009May28/Modified/SU14 //cpp test corrections

    ClibmsCalcSizeAdapter calcSizeAdapter(l_vf_handle, l_protocol_index);

    CString l_str_msg(_T(""));
    /*int l_appcode = f_protocol->GetAppCode();

    if (l_appcode != VFC_APP_CODE_MRS &&
        l_appcode != VFC_APP_CODE_MRSCSI) {

        int32_t	l_peftmatrix = 0;
        int32_t	l_roftmatrix = 0;
        int l_status = -1;
        l_status = calcSizeAdapter.msCheckFtMatrix(&l_peftmatrix, &l_roftmatrix);

        if (l_status > 0) {
            l_str_msg.Format(_T("l_peFtMatrix =%d l_roftmatrix=%d"), l_peftmatrix, l_roftmatrix);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

            if (l_status == 2) {

                return PQM_ERROR_MATRIX_OVER;

            } else if (l_status == 1) {
                return PQM_WARNING_MATRIX_OVER;

            }
        }
    }*/

    int16_t	l_num_channels = 0;
    unsigned int l_pac_pattern = 0u;	/* channel mapping */ //Patni-AD/2009May28/Modified/SU14 //cpp test corrections
    PqmCoil_t* l_coil_ptr = NULL;

    if (m_pqmptr->IsSaturnCoilSystem()) {
        PqmSaturnCoil_t* l_saturn_coil = f_protocol->GetSaturnCoil();

        for (int l_count = 0 ; l_count < l_saturn_coil->numSection ; l_count++) {
            l_num_channels += l_saturn_coil->Signal[l_count].num ;
        }

        l_coil_ptr = f_protocol->GetCoilDBTable()->coiltbl_p[0];

    } else {

        CPqmCoilData* l_pqmcoildata = m_pqmptr->GetDataManager()->GetPQMCoilData();
        CCoil l_coil;

        bool l_ret = l_pqmcoildata->PqmCoilGetCoil(f_protocol->GetPqmPrMode()->GetRecvCoilId(), l_coil);

        l_num_channels = l_coil_ptr->num_channels;
        l_pac_pattern = l_coil_ptr->pac_pattern;


    }

    if (l_coil_ptr != NULL) {
        //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
        //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
        int32_t     l_postacqmode(0), l_flyflag(0), l_mode(0), /*l_channelpervfep*/ l_channels_per_vfep = 0;
        int	        l_reconmode(0), l_len = 0;
        bool_t      l_pac_scan_flag(FALSE) ;		/* V6.20 for MRS */
        int32_t     l_pc_vap_mode       = 0;
        char        l_sequence[128] = {0};
        //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

        //+Patni-HAR/2010Jun29/Added/V1.30#IR-158

        PqmSaturnRcvCoil_t*  l_saturn_recv_coil = f_protocol->GetCoilDBTable();

        if (NULL != l_saturn_recv_coil) {
            l_str_msg.Empty();
            l_str_msg.Format(_T("[PqmSaturnRcvCoil_t->numCoil = %d]"),
                             l_saturn_recv_coil->numCoil);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);
        }

        //-Patni-HAR/2010Jun29/Added/V1.30#IR-158

        if (m_pqmptr->IsSaturnCoilSystem())	{	/* Saturn */

            //+Patni-HAR/2010Jun29/Added/V1.30#IR-158
            /*if ((NULL != l_saturn_recv_coil) && (l_saturn_recv_coil->numCoil == 1)) {
            if((((l_num_channels - 1) / RFSB_INPUT_CHANNELS) != 0) &&
            (((l_num_channels - 1) / RFSB_INPUT_CHANNELS) < (m_pqmptr->GetNumChannels() / PRB3_BOARD_MAX_CHANNELS)))
            {
            l_channelpervfep    = PRB_BOARD_MAX_CHANNELS;
            l_pc_vap_mode       = 1;
            }
            } else {
            l_channelpervfep = MIN(l_num_channels , m_pqmptr->GetChannelPerVFEP());
            }*/
            //-Patni-HAR/2010Jun29/Added/V1.30#IR-158
            //+Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1
            //+Patni-AJS/2010Jul15/Added/IR-158
            const int RFSB_INPUT_CHANNELS = 4;
            const int PRB_BOARD_MAX_CHANNELS = 4;
            const int PRB3_BOARD_MAX_CHANNELS = 8;
            //-Patni-AJS/2010Jul15/Added/IR-158
            // IR-193, Confirmed from TMSC, Num channel calculation will done from channel mode selected from coil dialog and number of channel will be used from sm.cf file
            int	l_channel = m_pqmptr->GetChannelPerVFEP() * m_pqmptr->GetVAPChannels();

            if ((NULL != l_saturn_recv_coil) && (l_saturn_recv_coil->numCoil == 1) &&
                (((l_num_channels - 1) / RFSB_INPUT_CHANNELS) != 0) &&
                (((l_num_channels - 1) / RFSB_INPUT_CHANNELS) < (l_channel/*m_pqmptr->GetNumChannels()*/ / PRB3_BOARD_MAX_CHANNELS))) { // Changed as per above comment
                l_channels_per_vfep    = PRB_BOARD_MAX_CHANNELS;
                l_pc_vap_mode       = 1;

            } else {
                l_channels_per_vfep = MIN(l_num_channels , m_pqmptr->GetChannelPerVFEP());
            }

            //-Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1

        } else {
            const int l_vap_channels = m_pqmptr->GetVAPChannels();
            l_channels_per_vfep = 1 ;//Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments

            if (l_num_channels > l_vap_channels) {
                //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
                l_channels_per_vfep = (l_num_channels / l_vap_channels) ;

                if (l_num_channels % l_vap_channels)
                    //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
                    l_channels_per_vfep++ ;

                //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
                if (l_channels_per_vfep > m_pqmptr->GetChannelPerVFEP())
                    return PQM_ERROR_CHANNEL_OVER;
            }

            memset(mask, 0, sizeof(mask));
            l_status = l_study_ptr->SetProtocolValues(l_vf_handle, l_protocol_index, mask,
                       K_PR_PAC_PATTERN, &l_pac_pattern, sizeof(unsigned int), NULL);

            if (E_NO_ERROR != l_status) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
            }
        }

        memset(mask, 0, sizeof(mask));
        l_pac_scan_flag = (l_num_channels > 1 ? TRUE : FALSE) ;
        //+Patni-HAR/2010Jun29/Added/V1.30#IR-158
        l_str_msg.Empty();
        l_str_msg.Format(_T("[Number of Channels : %d][Channel Per VAP : %d]\
            [PAC Scan Flag : %d][PC VAP Mode : %d]"), l_num_channels,
                         l_channels_per_vfep, l_pac_scan_flag, l_pc_vap_mode);//Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);
        //-Patni-HAR/2010Jun29/Added/V1.30#IR-158
        //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
        l_status = l_study_ptr->SetProtocolValues(l_vf_handle, l_protocol_index, mask,
                   K_PR_NUM_PAC_SCAN, &l_num_channels, sizeof(int16_t),
                   K_PR_CHANNEL_PER_VFEP, &l_channels_per_vfep, sizeof(int32_t),
                   K_PR_PAC_SCAN_FLAG, &l_pac_scan_flag, sizeof(bool_t),
                   //Patni-HAR/2010Jun29/Added/V1.30#IR-158
                   K_PR_PC_VAP_MODE, &l_pc_vap_mode, sizeof(int32_t),
                   NULL);

        //Patni-PJS/2010Aug11/Modified/V1.30#TMSC-REDMINE-SM_1/Review_comments
        if (E_NO_ERROR != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }

        if (!(m_pqmptr->IsSaturnCoilSystem())) {	/* Saturn */
            PqmPrSetChannelGainData(l_vf_handle, f_protocol, l_coil_ptr);
        }

        /*if (m_pqmptr->GetVAP()) {

            l_len = sizeof(l_sequence);
            memset(mask, 0, sizeof(mask));
            l_study_ptr->GetProtocolValues(l_vf_handle, l_protocol_index, mask,
                K_PR_POST_ACQ_MODE, &l_postacqmode, NULL,
                K_PR_FLY_FLAG, &l_flyflag, NULL,
                K_PR_SEQUENCE_NAME, l_sequence, &l_len,
                K_PR_IMAGING_MODE, &l_mode, NULL, NULL);


            //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window

            CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("postAcqMode = 0x%x"), l_postacqmode);

            if (l_postacqmode != VFC_MODE_NONE) {
                ClibmsVfAdapter libmsvfAdapter(l_vf_handle);
                l_status = libmsvfAdapter.msVfReconMode(l_protocol_index, &l_reconmode);

                //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
                if_no_error(l_status) {
                    int32_t	l_other(0), l_newval = 0;

                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("msVfReconMode -> reconMode = 0x%x"), l_reconmode);

                    l_other = l_postacqmode & ~(VFC_MODE_AUTO_RECON |
                        VFC_MODE_CONCURRENT_RECON |
                        VFC_MODE_DYNAMIC_RECON);
                    l_newval = l_reconmode | l_other;

                    if (l_reconmode & VFC_MODE_AUTO_RECON)
                        l_newval |= (l_postacqmode & VFC_MODE_DYNAMIC_RECON);

                    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("newval = 0x%x"), l_newval);

                    if (l_postacqmode != l_newval) {

                        l_postacqmode = l_newval;

                        if (E_NO_ERROR != l_study_ptr->SetProtocolValues(l_vf_handle, l_protocol_index, mask,
                            K_PR_POST_ACQ_MODE, &l_postacqmode, sizeof(int32_t), NULL)) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
                        }
                    }//end if
                }

                else {
                    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msVfReconMode() failed."));
                }
            }//end if
        }//end if*/
    }

    return l_status;

}