// CFShiftCheck.cpp: implementation of the CCFShiftCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFShiftCheck.h"
#include <math.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include "ShimCommon.h"
#include "PQMStudy.h"
#include "PqmShimSharedData.h"
#include "PqmProtocol.h"
#include "VarFieldHandle.h"
#include "ScanProcedure.h"

using namespace MR::ACQ::SM::COMMON;
using namespace MPlus::EH;

PQM_CFShiftTemp_s::PQM_CFShiftTemp_s()
{
    memset(&m_currtemp, 0, sizeof(m_currtemp));
    memset(&m_basetemp, 0, sizeof(m_basetemp));
}

PQM_CFShiftData_s::PQM_CFShiftData_s() : m_threshold_val(0)
{

    memset(&m_temp_coeff, 0, sizeof(m_temp_coeff));
    memset(&m_gif, 0, sizeof(m_gif));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   :CCFShiftCheck()
//Author        :PATNI / Pavan
//Purpose       :To Calculate temperature
//*****************************************************************************
CCFShiftCheck::CCFShiftCheck(CPQMStudy* f_pqm_studyptr) :
    m_resetretryexecuted(FALSE),
    m_pqm_studyptr(f_pqm_studyptr)
{

}

//****************************Method Header************************************
//Method Name   :~CCFShiftCheck()
//Author        :PATNI / Pavan
//Purpose       :To Calculate temperature
//*****************************************************************************
CCFShiftCheck::~CCFShiftCheck()
{

}


//****************************Method Header************************************
//Method Name   :CheckCFShiftValue()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
int CCFShiftCheck::CheckCFShiftValue(
    CPSAcqSetFlag::DATA f_data,
    CPqmProtocol* f_curr_protocol,
    CScanProcedure* f_scan_proc
) const
{
    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::CheckCFShiftValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_studyptr) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_studyptr is NULL"));
        return CFA_SHIFT_ERROR;
    }

    if (NULL == f_curr_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_curr_protocol is NULL"));
        return CFA_SHIFT_ERROR;
    }

    if (NULL == f_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_scan_proc is NULL"));
        return CFA_SHIFT_ERROR;
    }

    int l_base_protocol_index = 0;

    switch (f_data) {
        case VFC_PRESCAN_USE_PREVIOUS:

            if (1 == f_curr_protocol->GetPosition()) {
                l_base_protocol_index = f_curr_protocol->GetProtocol();

            } else {
                CPqmProtocol* l_prev_protocol = m_pqm_studyptr->GetPrevItemFromPos(f_curr_protocol);

                if (NULL == l_prev_protocol) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_prev_protocol pointer is NULL"));
                    return CFA_SHIFT_ERROR ;

                } else {
                    l_base_protocol_index = l_prev_protocol->GetProtocol();
                }
            }

            break;

        case VFC_PRESCAN_LOAD_VARFIELD:
            l_base_protocol_index = f_curr_protocol->GetCfaProt();
            CopyBaseTemperature(m_pqm_studyptr->GetVfDBHandle(), l_base_protocol_index, f_curr_protocol->GetProtocol());
            break;

        case VFC_PRESCAN_USE_VARFIELD:
            l_base_protocol_index = f_curr_protocol->GetProtocol();
            break;

        default:
            break;
    }

    CString l_msg = _T("");
    l_msg.Format(_T("Base temperature reading from protocol index = %d"), l_base_protocol_index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

    if (l_base_protocol_index >= 0) {
        float l_temperature = 0.0;
        float l_threshold_val = 0.0;


        BOOL l_ret = FALSE;

        switch (CalculateTemperature(l_base_protocol_index, l_threshold_val, l_temperature)) {

            case CFA_NOT_REQUIRED: {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CFA not required, continue the scan"));
                return CFA_NOT_REQUIRED;
            }

            case CFA_SHIFT_ERROR: {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Temperature is not calculated"));
                return CFA_SHIFT_ERROR;
            }

            default:
                break;
        }

        int l_shimState = 0;
        CPqmShimSharedData::GetInstance()->GetShimState(&l_shimState) ;

        l_msg.Format(_T("Shim state = %d"), l_shimState);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

        if (IsRetryCFARequired(l_threshold_val, l_temperature)) {

            if (l_shimState == SHIM_STATE_ACTIVE) {

                MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                        CPqmUtilities::GetMultiLingualString(_T("IDS_PQM_RETRY_CFA")),
                        _T("PQM"),
                        SCAN_WARN_INFORMATION,
                        SCAN_WARN_SYSMODAL,
                        SCAN_WARN_OK_CANCEL_BUTTONS);

                switch (l_response) {
                    case MPlusErrResponse_OK : {
                        return CFA_REQUIRED;
                    }

                    case MPlusErrResponse_CANCEL : {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("User has pressed cancel button"));

                        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_SCAN_ABORTED")),
                            MPlus::EH::Crucial,
                            _T("CFShiftCheck.cpp"),
                            _T("PQM"));
                        return CFA_SHIFT_ERROR;
                    }
                }

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shim is not active"));
                return CFA_REQUIRED;
            }

        } else {
            return CFA_NOT_REQUIRED;
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exit-Trace of Check Retry CFA"));
    return CFA_REQUIRED;
}

//****************************Method Header************************************
//Method Name   :GetIsCFARetryExecuted()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
BOOL CCFShiftCheck::GetIsCFARetryExecuted(
) const
{
    return m_resetretryexecuted;
}


//****************************Method Header************************************
//Method Name   :SetIsCFARetryExecuted()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCFShiftCheck::SetIsCFARetryExecuted(
    const BOOL f_retrycfa
)
{
    m_resetretryexecuted = f_retrycfa;
}


//****************************Method Header************************************
//Method Name   :SetcfShiftTemp()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCFShiftCheck::SetcfShiftTemp(
    AM_AcqManToClient_t* const f_data
)
{
    memcpy(&m_cfshifttemp, &f_data->hwTemp, sizeof(f_data->hwTemp));
}


//****************************Method Header************************************
//Method Name   :ReadcfShiftInfoFile()
//Author        :PATNI / Pavan
//Purpose       :To read cfShiftInfo.dat file for temp coeff and threshold value
//*****************************************************************************
BOOL CCFShiftCheck::ReadcfShiftInfoFile(
    const TCHAR* f_file_path,
    PQM_CFShiftData_t* f_cf_shiftdata,
    int& f_temp_count
) const
{
    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::ReadcfShiftInfoFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Read temperature coefficient and Threashold value"));

    CStdioFile l_cfShiftInfo;
    l_cfShiftInfo.Open(f_file_path, CFile::typeText | CFile::modeRead);

    if (l_cfShiftInfo.hFileNull == l_cfShiftInfo.m_hFile) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("File is not present"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_CFA_FILE_ERROR")), //Error is changed as per discussion with TMSC
            MPlus::EH::Crucial,
            _T("CFShiftCheck.cpp"),
            _T("PQM"));
        return FALSE;
    }

    CString l_temp_coeff = _T("");
    CString l_threshold_val = _T("");
    CString l_threshold_cfa_val = _T("");
    CString l_gif_val = _T("");

    bool l_threshold_read = false;
    bool l_threshold_cfa_read = false;
    bool l_coeff_read = false;
    bool l_gif_read = false;

    while (true) {
        CString l_data = _T("");


        if (!l_cfShiftInfo.ReadString(l_data)) {
            break;
        }

        if (l_data.Find(TEMP_COEFF) != -1) {
            l_temp_coeff = l_data.Mid(l_data.Find(_T(":")) + 1);
            l_coeff_read = true;
        }

        if (l_data.Find(THRESHOLD_CFA) != -1) {
            l_threshold_cfa_val = l_data.Mid(l_data.Find(_T(":")) + 1);
            l_threshold_cfa_read = true;
        }

        if (l_data.Find(THRESHOLD) != -1) {
            l_threshold_val = l_data.Mid(l_data.Find(_T(":")) + 1);
            l_threshold_read = true;
        }

        if (l_data.Find(GIF) != -1) {
            l_gif_val = l_data.Mid(l_data.Find(_T(":")) + 1);
            l_gif_read = true;
        }

        if (l_threshold_read && l_coeff_read && l_gif_read && l_threshold_cfa_read) {
            break;
        }
    }

    l_cfShiftInfo.Close();

    CString l_msg = _T("");

    if (!l_threshold_read || !l_coeff_read || !l_gif_read || !l_threshold_cfa_read) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Some field is missing in cfShiftInfo.dat file"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_CFA_FILE_ERROR")),
            MPlus::EH::Crucial,
            _T("CFShiftCheck.cpp"),
            _T("PQM"));
        return FALSE;
    }

    LPTSTR l_end_ptr;
    f_cf_shiftdata->m_threshold_val = static_cast<float>(_tcstod(l_threshold_val, &l_end_ptr));
    f_cf_shiftdata->m_threshold_cfa_val = static_cast<float>(_tcstod(l_threshold_cfa_val, &l_end_ptr));

    int l_count = 0;
    int l_find = -1;
    TCHAR l_buff[MAX_TEMP] = {0};

    //Read GIF Value
    ReadcfShiftData(f_cf_shiftdata, l_gif_val, l_count);
    f_temp_count = l_count;

    //Read Threshold values
    ReadcfShiftData(f_cf_shiftdata, l_temp_coeff, l_count, l_threshold_read);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exit ReadcfShiftInfoFile()"));
    return TRUE;
}

//****************************Method Header************************************
//Method Name   :CopyBaseTemperature()
//Author        :PATNI / Pavan
//Purpose       :To copy base temperature from previous protocol to current protocol
//*****************************************************************************
int CCFShiftCheck::CopyBaseTemperature(
    CVarFieldHandle* const f_vf_handle,
    const int f_from_prot,
    const int f_to_prot
) const
{

    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::CopyBaseTemperature");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Copying base temperature from previous protocol"));

    VfFieldSpec_t   fieldSpec[2] = {0};
    VfPathElem_t	path_elem[2] = {0};

    int n = 0;
    int numDone;
    time_t l_tm = 0;
    float l_base_temp[MAX_TEMP] = {0.0};

    path_elem[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path_elem[SVD_ST_PROTOCOL - 1].index = f_from_prot;

    VFF_SET_ARGS(fieldSpec[n], SVN_EXE_G_COIL_BASE_OLP_VALUE, SVT_EXE_G_COIL_BASE_OLP_VALUE,
                 sizeof(float)*MAX_TEMP, &l_base_temp, 0);
    n++;

    BOOL status = f_vf_handle->GetFields(path_elem, SVD_ST_PROTOCOL, fieldSpec, n, &numDone);

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in reading base temperature"));
        return FALSE;
    }

    VfFieldSpec_t   fieldSpec1[2] = {0};
    path_elem[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path_elem[SVD_ST_PROTOCOL - 1].index = f_to_prot;

    numDone = 0;

    n = 0;
    VFF_SET_ARGS(fieldSpec1[n], SVN_EXE_G_COIL_BASE_OLP_VALUE, SVT_EXE_G_COIL_BASE_OLP_VALUE,
                 fieldSpec[n].size, fieldSpec[n].buffer, VFO_REPLACE | VFO_ALLOCATE);
    n++;

    status = f_vf_handle->PutFields(path_elem, SVD_ST_PROTOCOL, fieldSpec1,
                                    n, &numDone, &l_tm);

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not copy base temperature from previous protocol to current protocol"));
        return FALSE;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exit CopyBaseTemperature()"));
    return TRUE;
}

//****************************Method Header************************************
//Method Name   :CalculateTemperature()
//Author        :PATNI / Pavan
//Purpose       :To Calculate temperature
//*****************************************************************************
int CCFShiftCheck::CalculateTemperature(
    const int f_base_protocol_index,
    float& f_threshold_val,
    float& f_temperature
) const
{
    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::CalculateTemperature");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Calculate temperature from temperature coefficient"));

    TCHAR l_file_path[MAX_PATH] = {0};

    if (::GetEnvironmentVariable(_T("MRMP_TUNE_DIR"), l_file_path, MAX_PATH * sizeof(TCHAR)) != 0) {
        _tcscat(l_file_path, _T("\\CfShiftInfo.dat"));

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not get file path"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_SCAN_ABORTED")),
            MPlus::EH::Crucial,
            _T("CFShiftCheck.cpp"),
            _T("PQM"));
        return CFA_SHIFT_ERROR;
    }

    PQM_CFShiftData_t l_cf_shiftdata ;

    int l_count = 0;

    if (!ReadcfShiftInfoFile(l_file_path, &l_cf_shiftdata, l_count)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in reading csShiftInfo file"));
        return CFA_SHIFT_ERROR;
    }

    int l_index = 0;

    int l_temp_count = 0;
    float l_temperature = 0;
    float l_temperature_diff = 0;
    CString l_msg = _T("");

    CString l_base_temp = _T("Base Temperature - ");
    CString l_curr_temp = _T("\nCurrent Temperature - ");
    CString l_temp_coeff = _T("\n Temperature Coefficient");


    for (int count = 0; count < MAX_TEMP; count++) {
        CString cs = _T("");
        cs.Format(_T("%f,"), m_cfshifttemp.m_basetemp[count]);
        l_base_temp += cs;

        cs = _T("");
        cs.Format(_T("%f,"), m_cfshifttemp.m_currtemp[count]);
        l_curr_temp += cs;

        cs = _T("");
        cs.Format(_T("%f,"), l_cf_shiftdata.m_temp_coeff[count]);
        l_temp_coeff += cs;
    }

    l_msg = l_base_temp + l_curr_temp + l_temp_coeff;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
    bool l_coeff_err = true;

    while (l_index < l_count) {

        int l_gif_index = l_cf_shiftdata.m_gif[l_index];

        l_msg = _T("");
        l_msg.Format(_T("GIF index = %d, Temp_Coeff = %f, Base_temp = %f, Current_temp = %f"), l_gif_index, l_cf_shiftdata.m_temp_coeff[l_gif_index], m_cfshifttemp.m_basetemp[l_gif_index], m_cfshifttemp.m_currtemp[l_gif_index]);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

        if (!l_cf_shiftdata.m_temp_coeff[l_gif_index]) { // on 18-Nov-11
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Skipping temperature difference since temperature coefficient is 0"));
            l_index++;
            continue;
        }

        l_coeff_err = false;

        l_temperature_diff = m_cfshifttemp.m_currtemp[l_gif_index] - m_cfshifttemp.m_basetemp[l_gif_index];

        if ((fabsf(l_temperature_diff) > fabsf(l_cf_shiftdata.m_threshold_val)) ||(m_cfshifttemp.m_currtemp[l_gif_index] < OLPTEMP_NOTUSE)) {
            l_msg = _T("");
            l_msg.Format(_T("Skipping index = %d as Difference of Base temperature and current temperature (%f) is more then limit(%f)"), l_gif_index, l_temperature_diff, l_cf_shiftdata.m_threshold_val);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
            l_index++;
            continue;
        }

        l_temperature += (l_cf_shiftdata.m_temp_coeff[l_gif_index] * l_temperature_diff); // Modify on 17-Nov-11
        l_index++;
        l_temp_count++;

        if (MAX_TEMP == l_temp_count) break;
    }

    if (l_coeff_err) { // Added if all coefficient is 0 then do not abort scan, conitnue the scan as normal
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("all the coefficient is 0, Scan will continue as normally"));
        return CFA_NOT_REQUIRED;
    }

    if (0 == l_temp_count) {
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_GCOILTEMP")),
            MPlus::EH::Warning,
            _T("CFShiftCheck.cpp"),
            _T("PQM"));
        return CFA_SHIFT_ERROR;
    }

    f_temperature = (l_temperature / l_temp_count);

    f_threshold_val = l_cf_shiftdata.m_threshold_cfa_val;

    l_msg = _T("");
    l_msg.Format(_T("Calculated Temperature = %f CFShift value = %f threshold_CFA = %f and temperature count = %d"), l_temperature, f_temperature, f_threshold_val, l_temp_count);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exit CalculateTemperature()"));
    return CFA_REQUIRED;
}


//****************************Method Header************************************
//Method Name   :IsRetryCFARequired()
//Author        :PATNI / Pavan
//Purpose       :Compare temperature with threashold value
//*****************************************************************************
BOOL CCFShiftCheck::IsRetryCFARequired(
    const float f_threshold_val,
    const float f_temperature
) const
{

    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::IsRetryCFARequired");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString l_msg = _T("");
    l_msg.Format(_T("CFShiftValue = %f and Threashold Value = %f"), f_temperature, f_threshold_val);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

    return fabsf(f_temperature) > fabsf(f_threshold_val) ? TRUE : FALSE;
}

//****************************Method Header************************************
//Method Name   :IsRetryCFARequired()
//Author        :PATNI / Pavan
//Purpose       :Read Cfshift data, Threshold or GIF values
//*****************************************************************************
void CCFShiftCheck::ReadcfShiftData(PQM_CFShiftData_t* f_cf_shiftdata, CString f_data, int& f_count, bool f_threshold_read) const
{
    LPCTSTR FUNC_NAME = _T("CCFShiftCheck::ReadcfShiftData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    LPTSTR l_end_ptr;
    int l_count = 0;
    int l_find = -1;
    TCHAR l_buff[MAX_TEMP] = {0};
    CString l_msg = _T("");

    while (true) {

        if (MAX_TEMP == l_count) {
            break;
        }

        memset(l_buff, 0, sizeof(l_buff));

        l_find = f_data.Find(FIND_COMMA);

        if (l_find != -1) {
            CString l_cs = f_data.Left(l_find);
            l_cs.TrimLeft(TRIM_SPACE);
            l_cs.TrimRight(TRIM_SPACE);

            if (l_cs.IsEmpty())  {
                break;
            }

            _tcscpy(l_buff, f_data.Left(l_find));

            if (f_threshold_read) {
                int l_gif_index = f_cf_shiftdata->m_gif[l_count++];
                float l_threshold_val = static_cast<float>(_tcstod(l_buff, &l_end_ptr));
                f_cf_shiftdata->m_temp_coeff[l_gif_index] = l_threshold_val;
                l_msg = _T("");
                l_msg.Format(_T("Temperature coefficient read from file = %f"), f_cf_shiftdata->m_temp_coeff[l_gif_index]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

            } else {
                f_cf_shiftdata->m_gif[l_count++] = _ttoi(l_buff);
                l_msg = _T("");
                l_msg.Format(_T("GIF read from file = %d"), f_cf_shiftdata->m_gif[l_count - 1]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
            }

            f_data = f_data.Mid(l_find + 1);
            f_data.TrimLeft(TRIM_SPACE);
            f_data.TrimRight(TRIM_SPACE);

        } else {
            f_data.TrimLeft(TRIM_SPACE);
            f_data.TrimRight(TRIM_SPACE);

            if (f_data.IsEmpty()) {
                break;
            }

            _tcscpy(l_buff, f_data);

            if (f_threshold_read) {
                int l_gif_index = f_cf_shiftdata->m_gif[l_count++];
                float l_threshold_val = static_cast<float>(_tcstod(l_buff, &l_end_ptr));
                f_cf_shiftdata->m_temp_coeff[l_gif_index] = l_threshold_val;

                l_msg = _T("");
                l_msg.Format(_T("Temperature coefficient read from file = %f"), f_cf_shiftdata->m_temp_coeff[l_gif_index]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

            } else {
                f_cf_shiftdata->m_gif[l_count++] = _ttoi(l_buff);

                l_msg = _T("");
                l_msg.Format(_T("GIF read from file = %d"), f_cf_shiftdata->m_gif[l_count - 1]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
            }

            break;
        }
    }

    f_count = l_count;
}