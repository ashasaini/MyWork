//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-779
//Redmine-778
//Redmine-780
//Redmine-781
// PqmUtilities.cpp: implementation of the CPqmUtilities class.
//
//////////////////////////////////////////////////////////////////////
//Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
#include <process.h>
#include "stdafx.h"

#if _MSC_VER == 1200
#include <iomanip>
#endif
#include <sstream>


#include <PQM/PqmUtilities.h>
//Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
#include <Psapi.h>
//Patni-PJS/2010May09/Added/JaFT# IR-141
#include <MPlusSec/MPlusUserManage/MPlusUserManageClient.h>
#include <MPlusI18n/MPlusI18nCatalog.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <libStudyManager/strcnv.h>
#include <PQM/PQMLogMgr.h>
#include "Pqm.h"
#include "PQMStudyMgr.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
//Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "Vector3D.h"				//Patni/2011Mar31/Added/V2.0/IR-181_NFD1_Prod_Req_Part1


using namespace MPlus::UserManage;
using namespace MR::ACQ::SM::COMMON;

#define AUTO_INSERT_SCRATCH_PAD _T("\\PQM.AutoInsert.imaging")
#define AUTO_GET_INTERVAL_SCRATCH_PAD _T("\\PQM.AutoGetInterval.imaging")
#define CARDIAC_SCRATCH_PAD _T("\\PQM.Cardiac.imaging")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-Patni-HAR/2010Feb18/Added/MSA0251-00176
CPqm* CPqmUtilities::m_pqm_ptr = NULL;
//+Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments
HANDLE CPqmUtilities::m_mrs_proc_handle = NULL;
HANDLE CPqmUtilities::m_mrs_thread_handle = NULL;
//-Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments

//+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
//*****************************************************************************
//Method Name   :  IsMRSProcessToolAlive()
//Author        :  PATNI/PJS
//Purpose       :  This function is used to check whether
//                 MRSProcessTool.exe is alive
//*****************************************************************************
//Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
unsigned int CPqmUtilities::IsMRSProcessToolAlive(
    void* f_parameter
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::IsMRSProcessToolAlive");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function"));
    BOOL l_bRetVal = FALSE;
    //+Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments
    /*CString	l_csprocess = _T("");
    TCHAR l_szProcessName[MAX_PATH] = {_T('\0')};

    // Get the list of process identifiers.
    DWORD l_dwaProcesses[1024] = {0}, l_dwcbNeeded = 0;
    EnumProcesses(l_dwaProcesses, sizeof(l_dwaProcesses), &l_dwcbNeeded);

    // Calculate how many process identifiers were returned.
    const DWORD l_dwcProcesses = l_dwcbNeeded / sizeof(DWORD);*/

    const DWORD TIMEOUT = 120 * 1000;
    //Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments
    //int l_pqm_found_count = 0;
    m_pqm_ptr->WriteToUI(PQM_MSG_MRSPROCESS_TOOL_CHECK_START);

    //+Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments
    if (!m_mrs_proc_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_mrs_proc_handle is NULL"));
        return FALSE;
    }

    if (WaitForSingleObject(m_mrs_proc_handle, TIMEOUT) == WAIT_TIMEOUT) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Time Out"));

        l_bRetVal = TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM is terminated"));

    }

    //-Patni-PJS/2011Mar7/Added/TMSC_REDMINE-1480/Review-Comments
    m_pqm_ptr->WriteToUI(PQM_MSG_MRSPROCESS_TOOL_CHECK_ENDS);

    if (m_mrs_proc_handle) {
        CloseHandle(m_mrs_proc_handle);
        m_mrs_proc_handle = NULL;
    }

    if (m_mrs_thread_handle) {
        CloseHandle(m_mrs_thread_handle);
        m_mrs_thread_handle = NULL;
    }

    _endthreadex(0);
    return l_bRetVal;
}


//*****************************************************************************
//Method Name   :  InvokeMRSProcessTool()
//Author        :  PATNI/PJS
//Purpose       :  This function is to invoke
//                 MRSProcessTool.exe
//*****************************************************************************
BOOL CPqmUtilities::InvokeMRSProcessTool(
    CPqmProtocol* f_pqm_prot
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::InvokeMRSProcessTool");

    if (!f_pqm_prot) {
        return FALSE;
    }

    STARTUPINFO             si = {0};

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    CString l_mrsprocesstool_path = CPqmUtilities::GetFullAppPath(_T("MRSProcessTool.exe"));
    CString     l_commandline = _T("");
    l_commandline.Format(_T("%s -series %s"), l_mrsprocesstool_path, f_pqm_prot->GetSeriesLOID());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_commandline);


    int l_result = FALSE;
    PROCESS_INFORMATION     l_mrsprocesstool_proc_info = {0};

    if (CreateProcess(l_mrsprocesstool_path,
                      l_commandline.GetBuffer(l_commandline.GetLength()),
                      NULL,
                      NULL,
                      FALSE,
                      CREATE_NO_WINDOW,
                      NULL,
                      NULL,
                      &si,
                      &l_mrsprocesstool_proc_info)) {
        l_result = TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CreateProcess() failed."));
        l_result = FALSE;
    }

    //+Patni-PJS/2011Mar7/Commented/TMSC_REDMINE-1480/Review-Comments
    //CloseHandle(l_mrsprocesstool_proc_info.hProcess);
    m_mrs_proc_handle = l_mrsprocesstool_proc_info.hProcess;
    m_mrs_thread_handle = l_mrsprocesstool_proc_info.hThread;
    //-Patni-PJS/2011Mar7/Commented/TMSC_REDMINE-1480/Review-Comments
    l_commandline.ReleaseBuffer();
    return l_result;
}
//-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

//+Patni-Hemant/2009Mar5/Added/ACE-1/Venus Coil
//************************************Method Header************************************
// Method Name  : PQMUiStringCat
// Author       : PATNI/ HEMANT
// Purpose      : To Concatenate two strings
//***********************************************************************************
void CPqmUtilities::PQMUiStringCat(
    char* f_main_string,
    char* f_add,
    char* f_out_string
)
{
    /*********************************************************************
    * %sを置き換える。
    *********************************************************************/

    //Commented the vantage code. by Hemant
    //char		*template = NULL ;

    char l_front[256] = { '\0' };
    char l_back[256] = { '\0' };
    memset(f_out_string, 0 , sizeof(f_out_string));

    if (f_main_string != NULL) {
        if (f_add != NULL) {

            for (int i = 0; i < (int)strlen(f_main_string); i++) {
                if (f_main_string[i] == '%' && f_main_string[i + 1] == 's') {
                    break;

                } else {
                    l_front[i] = f_main_string[i];
                }
            }

            int j = 0;

            for (i = i + 2; i < (int)strlen(f_main_string); i++) {
                l_back[j] = f_main_string[i];
                j++;
            }

            sprintf(f_out_string, "%s%s%s", l_front, f_add, l_back);
        }

    } else {
        CString temp_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STR_CONCAT_ERRROR"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        int l_size = temp_str.GetLength() + 1;		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        char* l_temp = new char[l_size];			// Added By KT/ACE-2/15-06-2009/Multilingualisation
        memset(l_temp, 0, sizeof(char) * l_size);   // Added By KT/ACE-2/15-06-2009/Multilingualisation
        wcstombs(l_temp, temp_str, l_size);         // Added By KT/ACE-2/15-06-2009/Multilingualisation
        sprintf(f_out_string, l_temp);// Added By KT/ACE-2/15-06-2009/Multilingualisation

        //+Added By KT/ACE-2/15-06-2009/Multilingualisation
        DEL_PTR_ARY(l_temp);	//Patni-HAR/2009Aug11/Added/Memory Leaks

        //-Added By KT/ACE-2/15-06-2009/Multilingualisation
    }

    return;
}

//************************************Method Header************************************
// Method Name  : PQMUiErrorCat
// Author       : PATNI/ HEMANT
// Purpose      : Concatenate two strings and display Error
//***********************************************************************************
void CPqmUtilities::PQMUiErrorCat(
    char*	f_main_string,
    char* f_add,
    CString f_fileName,
    DWORD f_error_level /*= MPlus::EH::Crucial*/
)
{
    /*********************************************************************
    * エラーナビに追加メッセージ付で表示させる。
    *********************************************************************/

    //Commented the vantage code. by Hemant
    //char		*template = NULL ;

    char l_out_string[256] = { '\0' };
    //XmString	xmstr;

    PQMUiStringCat(f_main_string, f_add, l_out_string);

    //we will be displaying the error in errorview..
    CString l_str(_T(""));
    UTIL::Mbs2Wcs(&l_str, l_out_string);
    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_str, f_error_level , f_fileName, _T("PQM"));
    return ;
}


//************************************Method Header************************************
// Method Name  : PQMAcqPortString
// Author       : PATNI/ HEMANT
// Purpose      : To Get the Port Names
//***********************************************************************************
void CPqmUtilities::PQMAcqPortString(
    const cdb_coil_port_e f_port_type,
    const int f_portDef,
    char* f_port_string
)
{
    /*************************************************************
     * 接続可能ポートのスペース区切り文字列を作成
     *************************************************************/

    //DB_FUNC_ENTER("[pqmAcqPortString]:");

    std::string l_port_string("");

    if (f_port_type == CDB_PORT_F) {	/* Gポート */
        l_port_string = "G";

    } else if (f_port_type == CDB_PORT_G) {	/* Aポート */
        if (f_portDef & 0x0001) {	/* A1 */

            if (l_port_string.empty()) {
                l_port_string = "A1";

            } else {
                l_port_string += ",A1";
            }

        }

        if (f_portDef & 0x0002) {	/* A2 */

            if (l_port_string.empty()) {
                l_port_string = "A2";

            } else {
                l_port_string += ",A2";
            }
        }

        if (f_portDef & 0x0004) {	/* A3 */
            if (l_port_string.empty()) {
                l_port_string = "A3";

            } else {
                l_port_string += ",A3";
            }
        }

        if (f_portDef & 0x0008) {	/* A4 */
            if (l_port_string.empty()) {
                l_port_string = "A4";

            } else {
                l_port_string += ",A4";
            }
        }

        if (f_portDef & 0x0010) {	/* A5 */
            if (l_port_string.empty()) {
                l_port_string = "A5";

            } else {
                l_port_string += ",A5";
            }
        }

        if (f_portDef & 0x0020) {	/* A6 */
            if (l_port_string.empty()) {
                l_port_string = "A6";

            } else {
                l_port_string += ",A6";
            }
        }

        if (f_portDef & 0x0040) {	/* A7 */
            if (l_port_string.empty()) {
                l_port_string = "A7";

            } else {
                l_port_string += ",A7";
            }
        }
    }

    strcpy(f_port_string , l_port_string.c_str());
}
//-Patni-Hemant/2009Mar5/Added/ACE-1/Venus Coil

//************************************Method Header****************************
// Method Name  : UpdateIECInfo()
// Author       : PATNI/HAR
// Purpose      :
//*****************************************************************************
void CPqmUtilities::UpdateIECInfo(
    CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::UpdateIECInfo");

    int l_dbdt_op_mode = 0;

    //+Patni-PJS/2010June1/Modified + Added/MVC007434
    flt32_t l_curr_dbdt = 0;
    int l_status = m_pqm_ptr->GetStudy()->ReaddBdtOpMode(f_protocol, l_dbdt_op_mode, l_curr_dbdt);

    if (l_dbdt_op_mode == VFC_DBDT_IEC_OPERATING_NORMAL) {
        f_protocol->SetdBdtModeString(DBDT_NORMAL_LABEL);

    } else if (l_dbdt_op_mode == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
        f_protocol->SetdBdtModeString(DBDT_MODE1_LABEL);

    } else if (l_dbdt_op_mode == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {
        f_protocol->SetdBdtModeString(DBDT_MODE2_LABEL);
    }

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[Failed to read dB/dt Operating mode from study file]"));
        return;
    }

    //-Patni-PJS/2010June1/Modified + Added/MVC007434
    BSTR l_bstr_series_loid = f_protocol->GetSeriesLOID().AllocSysString();
    long l_series_number = 0;

    if (FALSE == m_pqm_ptr->GetDbsaStudyMgr()->ReadSeriesNo(
            l_bstr_series_loid, l_series_number)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("ReadSeriesNo() Failed"));
        return;

    }

    sarInfo_t* l_sar_info = f_protocol->GetSarInfo();
    int l_sar_op_mode = -1;

    if (m_pqm_ptr) {
        l_sar_op_mode = m_pqm_ptr->GetSARControlLicense() ? l_sar_info->sarControl.ope_mode : l_sar_info->tsar[l_sar_info->active].ope_mode;
    }

    CString l_log_str(_T(""));
    l_log_str.Format(_T("[db/dt Operating Mode : %d]\
				[SAR Operating Mode : %d][Series Number : %ld]"),
                     l_dbdt_op_mode, l_sar_op_mode, l_series_number);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              l_log_str);

    l_log_str.Format(_T("%ld\\%d\\%d"), l_series_number, l_sar_op_mode,
                     l_dbdt_op_mode);
    //l_bstr_series_loid = l_log_str.AllocSysString();
    //m_pqm_ptr->m_dbsa_study_mgr->UpdateIECInfo(l_bstr_series_loid);
    m_pqm_ptr->GetDbsaStudyMgr()->UpdateIECInfo(l_series_number, l_sar_op_mode, l_dbdt_op_mode);

    m_pqm_ptr->GetDbsaStudyMgr()->UpdateTotalSAEValue(f_protocol->GetProtocol());

    //+Patni-Sribanta/2010July08/Added/IR-149
    m_pqm_ptr->GetDbsaStudyMgr()->UpdateRealtimeSAR(f_protocol->GetProtocol());
    //-Patni-Sribanta/2010July08/Added/IR-149

    ::SysFreeString(l_bstr_series_loid);

}


//**************************************************************************
//Method Name   : ConvertCHAR
//Author        : PATNI\DKH
//Purpose       : to convert string to char*
//**************************************************************************
char* CPqmUtilities::ConvertCHAR(
    const CString& f_input
)
{
    const int   l_inlen = f_input.GetLength();
    char* l_retptr = new char[l_inlen + 1];

    if (l_retptr != NULL) {
        memset(l_retptr, 0, sizeof(char) * (l_inlen + 1));

        for (int l_counter = 0; l_counter < l_inlen; l_counter++) {
            l_retptr[l_counter] = (char)f_input.GetAt(l_counter);
        }

        l_retptr[l_counter] = 0;
    }

    return l_retptr;
}

//************************************Method Header**************************************
//Method Name:  ReadStringValue()
//Author:  PATNI/LK
//Purpose: Converts CPTString to char*
//**************************************************************************************/
char* CPqmUtilities::ReadStringValue(
    CPTString f_string
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::ReadStringValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReadStringValue"));
    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = f_string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));
    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    wcstombs(char_string, f_string, n);
    return char_string;
}

//*****************************************************************************
//Method Name   : CreateCurrentScratchPad
//Author        : Patni/AMT
//Purpose       : Create Scratchpad at start up of PQM for current protocol
//*****************************************************************************
BOOL CPqmUtilities::CreateCurrentScratchPad(
)
{
    char    l_cur_sp_path[256] = {0};

    CPqmUtilities::GetCurrScatchpadFileName(l_cur_sp_path);

    //Patni-AMT/2009Nov06/Deleted/MSA0248-00100-Code Review
    FILE* fp = fopen(l_cur_sp_path, "a");

    if (fp) {
        fclose(fp);
        return TRUE;

    } else {
        return FALSE;
    }
}

//+Patni-NP/2010Jul27/Added/IR-161
//*****************************************************************************
//Method Name   : PqmPrPutSwitchCoilType
//Author        : Patni/NP
//Purpose       :
//*****************************************************************************
//MEITEC/2010Aug03/Modified/Change type from bool to status_t for Warrning
status_t CPqmUtilities::PqmPrPutSwitchCoilType(CVarFieldHandle* vf_handle, const int prot, int32_t* switch_type)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities:: PqmPrPutSwitchCoilType");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmUtilities:: PqmPrPutSwitchCoilType"));

    VfPathElem_t	pelm[2];
    VfFieldSpec_t	vfReq;

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = prot;

    VFF_SET_ARGS(vfReq,
                 SVN_PLN_SWITCH_COIL_TYPE,
                 SVT_PLN_SWITCH_COIL_TYPE,
                 sizeof(int32_t),
                 switch_type,
                 VFO_REPLACE);

    int			num = 0;
    time_t		tm = 0l;
    status_t sts = vf_handle->PutFields(pelm, SVN_ST_PROTOCOL, &vfReq, 1, &num, &tm);

    if (sts != E_NO_ERROR || num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        sts = E_ERROR ;	/* エラー */
    }

    return sts ;
}
//-Patni-NP/2010Jul27/Added/IR-161

//+Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
//***************************Method Header*************************************
//Method Name    : calcReadOriVector()
//Author         :
//Purpose        :
//*****************************************************************************
void CPqmUtilities::calcReadOriVector(
    CVector3D* phase,
    CVector3D* slice,
    CVector3D* read
)
{
    if (read && phase && slice) {
        read->CrossProduct(*phase, *slice);
        read->Normalize();
    }
}
//-Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1


//****************************Method Header************************************
//Method Name	: ConvertLocalTimeStringToSystemTime()
//Author		: iGate/
//Purpose		:
//*****************************************************************************
SYSTEMTIME CPqmUtilities::ConvertLocalTimeStringToSystemTime(const char* const time_str)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::ConvertLocalTimeStringToSystemTime");

    SYSTEMTIME l_system_time = {0};


    std::stringstream ss;

#if _MSC_VER == 1200

    ss << std::dec;

#endif

    ss << time_str;

    char trashbox = '\0';

    ss >> l_system_time.wDay >> trashbox
       >> l_system_time.wMonth >> trashbox
       >> l_system_time.wYear >> trashbox
       >> l_system_time.wHour >> trashbox
       >> l_system_time.wMinute >> trashbox
       >> l_system_time.wSecond;

    if (ss.fail()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ConvertLocalTimeStringToSystemTime() failed"));
    }

    return l_system_time;

}
//************************************Method Header****************************
// Method Name  : GetPqmPtr
// Author       : PATNI/HAR
// Purpose      :
//*****************************************************************************
CPqm* CPqmUtilities::GetPqmPtr()
{
    if (m_pqm_ptr) {
        return m_pqm_ptr;

    } else {
        return NULL;
    }
}

//************************************Method Header****************************
// Method Name  : SetPqmPtr
// Author       : PATNI/HAR
// Purpose      :
//*****************************************************************************
void CPqmUtilities::SetPqmPtr(CPqm* f_pqm_ptr)
{
    m_pqm_ptr = f_pqm_ptr;
}

//*****************************************************************************
//Method Name   : GetCurrScatchpadFileName
//Author        : Patni/AMT
//Purpose       : returns current sratchpad file path (\gp\tmp\current_file)
//*****************************************************************************
BOOL CPqmUtilities::GetCurrScatchpadFileName(
    char* f_path
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::GetCurrScatchpadFileName");
    char*    l_buf = getenv("MRMP_GP_TMP_DIR");

    if (NULL == l_buf) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("MRMP_GP_TMP_DIR is not defined"));
        return FALSE;
    }

    strcpy(f_path, l_buf);
    //Patni-AMT/2010May07/Modified/JaFT# internal defect fix
    strcat(f_path, "/current_file\0");
    return TRUE;
}

//************************************Method Header************************************
// Method Name  : GetvalForLateralityStr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPqmUtilities::GetvalForLateralityStr(
    const CString& f_laterality_str
)
{
    if (f_laterality_str.IsEmpty()) {
        return Laterality_None ;
    }

    if (f_laterality_str.Compare(_T("R")) == 0) {
        return Laterality_Right ;
    }

    if (f_laterality_str.Compare(_T("L")) == 0) {
        return Laterality_Left ;
    }

    return Laterality_Invalid;
}


//****************************Method Header************************************
//Method Name   : GetLibVfErrorStatusString()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
CString CPqmUtilities::GetLibVfErrorStatusString(
    const status_t f_status
)
{
    CString l_multilang_string(_T(""));

    switch (f_status) {
        case E_NO_ERROR:
            l_multilang_string = _T(" no error");
            break;

        case E_ERROR:
            l_multilang_string = _T(" error");
            break;

        case E_NO_MEM:
            l_multilang_string = _T(" out of memory");
            break;

        case E_NO_SPACE:
            l_multilang_string = _T(" out of space");
            break;

        case E_BAD_SYSCALL:
            l_multilang_string = _T(" bad system call");
            break;

        case E_BAD_DESCRIPTOR:
            l_multilang_string = _T(" bad descriptor");
            break;

        case E_FILE_NOT_FOUND:
            l_multilang_string = _T(" file not found");
            break;

        case E_BAD_FORMAT:
            l_multilang_string = _T(" bad format");
            break;

        case E_BAD_OPTS:
            l_multilang_string = _T(" bad options");
            break;

        case E_BAD_PATH:
            l_multilang_string = _T(" bad path");
            break;

        case E_NOT_IMPLEMENTED:
            l_multilang_string = _T(" not implemented");
            break;

        case E_EXISTS:
            l_multilang_string = _T(" exists");
            break;

        case E_LOCK_WAIT:
            l_multilang_string = _T(" lock wait");
            break;

        case E_BAD_SIZE:
            l_multilang_string = _T(" bad size");
            break;

        default:
            l_multilang_string = _T(" Invalid Status");
    }

    return l_multilang_string;
}

//*******************************Method Header*********************************
//Method Name    : GetPrepStudyName()
//Author         : PATNI/ARD
//Purpose        :
//*****************************************************************************
void CPqmUtilities::GetPrepStudyName(
    char* filename
)
{
    char*	path  = NULL;

    if ((path = getenv("MRMP_GP_PRO_DIR")) != NULL) {
        sprintf(filename, "%s", path);

    } else {
        sprintf(filename, ".");
    }

    strcat(filename, "/prepStudy");
}

//*******************************Method Header*********************************
//Method Name    : GetProbeStudyName()
//Author         : PATNI/MSN
//Purpose        :
//*****************************************************************************
void CPqmUtilities::GetProbeStudyName(
    char* filename
)
{
    char*	path  = NULL;

    if ((path = getenv("MRMP_GP_PRO_DIR")) != NULL) {
        sprintf(filename, "%s", path);

    } else {
        sprintf(filename, ".");
    }

    strcat(filename, "/probeStudy");
}


//+Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review
//********************************Method Header********************************
//Method Name   :GetIPCConfigFileName()
//Author        :PATNI/ARD
//Purpose       :Get the config file name
//*****************************************************************************
CPTString CPqmUtilities::GetIPCConfigFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\PQMCONFIG.INI";
    return l_file_name;
}

//********************************Method Header********************************
//Method Name   :GetMapFileName()
//Author        :PATNI/ARD
//Purpose       :Get the map file name
//*****************************************************************************
CPTString CPqmUtilities::GetMapFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"MEMORY\\MAP.INI";

    return l_file_name;
}


//********************************Method Header********************************
//Method Name   :GetScanAnatomyFileName()
//Author        :PATNI/ARD
//Purpose       :Get the Scan Anatomy file name
//*****************************************************************************
CPTString CPqmUtilities::GetScanAnatomyFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += SCAN_ANATOMY_FILENAME;

    return l_file_name;
}



//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
//********************************Method Header********************************
//Method Name   :GetPresetItemFileName()
//Author        :
//Purpose       :
//*****************************************************************************
CPTString CPqmUtilities::GetPresetItemFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += PRESET_ITEM_FILENAME;

    return l_file_name;
}
//-Patni-Ajay/2010July23/Added/TMSC-REDMINE-443

//********************************Method Header********************************
//Method Name   :GetColorSettingsFileName()
//Author        :PATNI/ARD
//Purpose       :Get PQMAPP_colorsettings file path
//*****************************************************************************
CPTString CPqmUtilities::GetColorSettingsFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += COLOR_SETTING_FILENAME;

    return l_file_name;
}

//********************************Method Header********************************
//Method Name   :GetPqmAppConfigFileName()
//Author        :PATNI/ARD
//Purpose       :Get PQMAPP_config file path
//*****************************************************************************
CPTString CPqmUtilities::GetPqmAppConfigFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buff[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buff, MAX_PATH);
    l_file_name = l_buff;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += APPCONFIG_FILENAME;

    return l_file_name;
}

//********************************Method Header********************************
//Method Name   :GetScanAppSiteFileName()
//Author        :PATNI/ARD
//Purpose       :Get the number of AutoJob tag in MplusMRSrvScanAPPSite.xml
//*****************************************************************************
CPTString CPqmUtilities::GetScanAppSiteFileName(
)
{
    CPTString l_file_name = L"";
    wchar_t l_buffer[MAX_PATH] = {0};

    if ((::GetEnvironmentVariable(_T("MPLUSCONFDIR"), l_buffer, MAX_PATH)) == NULL) {
        wsprintf(l_buffer, _T("C:\\MPlusPlatform\\Config"));
    }

    l_file_name = l_buffer;
    l_file_name += _T("\\MPlusConfig\\Site\\") + SCAN_APP_SITE_FILENAME;

    return l_file_name;
}

//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
//********************************Method Header********************************
//Method Name   :GetScanCommentFileName()
//Author        :
//Purpose       :
//*****************************************************************************
CPTString CPqmUtilities::GetScanCommentFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;

    //TA Review Comment_3
    if (l_file_name.IsEmpty()) {
        return l_file_name;
    }

    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += SCAN_COMMENT_FILENAME;

    return l_file_name;
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

//+Patni-PJS/2010May10/Added/JaFT# IR-141
//*****************************************************************************
//Method Name   :
//Author        : Patni/PJS
//Purpose       :
//*****************************************************************************
CString CPqmUtilities::GetMPlusUsrName(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmUtilities::GetMPlusUsrName");

    CUserManageClient* pUMClient    = new CUserManageClient();

    if (!pUMClient) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pUMClient is NULL"));
        return _T("");
    }

    LPTSTR lpszUserName = (LPTSTR) new WCHAR[MAX_PATH];
    LPTSTR lpszUserRole = (LPTSTR) new WCHAR[MAX_PATH];
    DWORD  dwNameLength = MAX_PATH;
    DWORD  dwRoleLength = MAX_PATH;

    // Collect the current user name using MPlus UserManagement
    if (!pUMClient->GetCurrentUserInfo(lpszUserName,
                                       lpszUserRole,
                                       dwNameLength,
                                       dwRoleLength)) {

        DEL_PTR(lpszUserName);
        DEL_PTR(lpszUserRole);
        DEL_PTR(pUMClient);

        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetCurrentUserInfo failed to get User name"));

        return _T("");
    }

    CString l_csUserName = lpszUserName;

    DEL_PTR(lpszUserName);
    DEL_PTR(lpszUserRole);
    DEL_PTR(pUMClient);

    return l_csUserName;

}
//-Patni-PJS/2010May10/Added/JaFT# IR-141

//************************************Method Header************************************
// Method Name  : GetMultiLingualString
// Author       : PATNI/ KT
// Purpose      : Get String for Internationalisation
//***********************************************************************************
CString CPqmUtilities::GetMultiLingualString(
    const CString& f_str,
    const bool f_check /*= true*/	//Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
)
{
    CString err_string =  MPlus::I18n::CCatalog::instance().get(f_str);    // Added By KT/ACE-2/15-06-2009/Multilingualisation


    //+Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
    //if (err_string.IsEmpty()) {
    if (f_check && err_string.IsEmpty()) {
        //-Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163

        ASSERT(FALSE);
        err_string = f_str ;
    }

    return err_string;
}

//*****************************************************************************
//Method Name   : GetFullAppPath
//Author        : Patni/AMT
//Purpose       : Append f_app_name to %MRMP_HOME% and %MPLUSINSTTYPE%
//*****************************************************************************
CString CPqmUtilities::GetFullAppPath(
    const CString& f_app_name
)
{
    char* l_inst_type_env = getenv("MPLUSINSTTYPE");
    char* l_mrmp_home_env = getenv("MRMP_HOME");

    CString l_app_path(l_mrmp_home_env);

    l_app_path += _T("\\Bin");
    l_app_path += CString(l_inst_type_env);
    l_app_path += f_app_name;

    return l_app_path;
}

//************************************Method Header************************************
// Method Name  : GetStrForLaterality
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPqmUtilities::GetStrForLaterality(
    const int& f_laterality
)
{
    switch (f_laterality) {

        case Laterality_None:
            return _T("");

        case Laterality_Left:
            return _T("L");

        case Laterality_Right:
            return _T("R");

        default :
            return _T("");
    }

    return _T("");
}

//********************************Method Header********************************
//Method Name   :GetStudyPath()
//Author        :PATNI/LK
//Purpose       :Get Study Path file name from IPCConfig.ini
//*****************************************************************************
CString CPqmUtilities::GetStudyPath(
)
{
    //Nitin changes ipcconfig.ini dependency
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("SITE"), l_buffer, MAX_PATH);
    CString l_file_name = l_buffer;
    l_file_name += L"/";
    l_file_name += L"study/";

    return l_file_name;
}

//+Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
//********************************Method Header********************************
//Method Name   :GetCoilSarFileName()
//Author        :
//Purpose       :
//*****************************************************************************
CPTString CPqmUtilities::GetCoilSarFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;

    if (l_file_name.IsEmpty()) {
        return l_file_name;
    }

    l_file_name += _T("\\");
    l_file_name += _T("PQM\\");
    l_file_name += AUTO_INSERT_FILENAME;

    return l_file_name;
}
//-Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1

//********************************Method Header********************************
//Method Name   :GetAutoInsertProlimScratchPadPath()
//Author        :
//Purpose       :
//*****************************************************************************
char* CPqmUtilities::GetAutoInsertProlimScratchPadPath()
{
    CString f_auto_insert_scratch_pad;
    wchar_t buffer[MAX_PATH];
    ::GetEnvironmentVariable(_T("GP_TMP_DIR"), buffer, MAX_PATH);
    f_auto_insert_scratch_pad  = buffer;
    f_auto_insert_scratch_pad += AUTO_INSERT_SCRATCH_PAD;

    const int l_size = f_auto_insert_scratch_pad.GetLength();
    char* l_file_name = new char[l_size + 1];
    memset(l_file_name, 0, l_size + 1);
    wcstombs(l_file_name, f_auto_insert_scratch_pad, l_size + 1);
    return l_file_name;
}


CString CPqmUtilities::GetCharset()
{
    LPCTSTR	FUNC_NAME = _T("CPqmUtilities::GetCharset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int bufsize = 1024;
    TCHAR buf_locale[bufsize] = {'\0'};

    const TCHAR l_default_locale[] = _T("DefaultCharSet");
    const TCHAR l_default_charset[] = _T("ISO_IR 100");

    CString l_locale_file = _T(""), l_charsetfile = _T("");
    GetLocaleAndCharSetIniPath(l_locale_file, l_charsetfile);

    CString l_charset = _T("");

    if (!l_locale_file.IsEmpty()) {
        ::GetPrivateProfileString(_T("LocaleName"), _T("locale_name"),
                                  l_default_locale,
                                  buf_locale, bufsize,
                                  l_locale_file);


        if (buf_locale[0] != '\0') {
            l_charset = GetCharSetForLocale(l_charsetfile, buf_locale, _T(""));
        }
    }

    CString l_log_string = _T("");

    if (l_charset.IsEmpty()) {

        l_log_string = _T("Could Not found char set for Locale :");
        l_log_string += buf_locale ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);

        l_charset = GetCharSetForLocale(l_charsetfile, l_default_locale, l_default_charset);
    }

    if (l_charset.IsEmpty()) {

        l_log_string = _T("Using DefaultCharSet :") ;
        l_log_string += l_default_charset;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);
        l_charset = l_default_charset;
    }

    return l_charset;
}



CString CPqmUtilities::GetCharSetForLocale(
    const CString& l_inifile,
    const TCHAR f_locale_name[],
    const TCHAR f_default_charset[]
)
{

    LPCTSTR FUNC_NAME = _T("CPqmUtilities::GetCharSetForLocale") ;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int bufsize = 1024;
    TCHAR buf_charset[bufsize] = {'\0'};
    ::GetPrivateProfileString(_T("SpecificCharacterSet"), f_locale_name,
                              f_default_charset,
                              buf_charset, bufsize,
                              l_inifile);

    CString l_log_string = _T("Locale Name :");
    l_log_string += f_locale_name ;
    l_log_string += _T("  Default CharSet : ") ;
    l_log_string += f_default_charset ;
    l_log_string += _T("  Retrived CharSet : ") ;
    l_log_string += buf_charset ;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);

    return buf_charset;
}

void CPqmUtilities::GetLocaleAndCharSetIniPath(
    CString& l_locale_file,
    CString& l_charsetfile
)
{

    wchar_t buffer[MAX_PATH];
    ::GetEnvironmentVariable(_T("MPLUSCONFDIR"), buffer, MAX_PATH);

    l_locale_file = buffer ;
    l_locale_file += _T("\\MPlusI18n\\MPlusI18nConfig.ini");

    l_charsetfile = buffer ;
    l_charsetfile += _T("\\MRPIMSCommon\\PermitCharset.ini") ;
}

CString CPqmUtilities::GetNextString(CString& f_dbsavrcsstr, const CString& f_seperator)
{

    CString str = f_dbsavrcsstr ;
    const int l_positon = f_dbsavrcsstr.Find(f_seperator);

    if (l_positon > 0) {
        str = f_dbsavrcsstr.Left(l_positon);
        f_dbsavrcsstr = f_dbsavrcsstr.Right(f_dbsavrcsstr.GetLength() - l_positon - 1);

    } else {
        f_dbsavrcsstr = _T("") ;
    }

    str.TrimLeft();
    str.TrimRight();

    return str;
}

//********************************Method Header********************************
//Method Name   :GetCardiacProtScratchPadPath()
//Author        :PATNI\PJS
//Purpose       :
//*****************************************************************************
char* CPqmUtilities::GetCardiacProtScratchPadPath(
)
{
    CString l_cardiac_scratch_pad;
    wchar_t buffer[MAX_PATH];

    if (::GetEnvironmentVariable(_T("GP_TMP_DIR"), buffer, MAX_PATH) == NULL) {
        wsprintf(buffer, _T(".%s"), _T("/gp/tmp/"));
    }

    l_cardiac_scratch_pad  = buffer;
    l_cardiac_scratch_pad += CARDIAC_SCRATCH_PAD;

    const int l_size = l_cardiac_scratch_pad.GetLength();
    char* l_file_name = new char[l_size + 1];
    memset(l_file_name, 0, l_size + 1);
    wcstombs(l_file_name, l_cardiac_scratch_pad, l_size + 1);
    return l_file_name;
}

//********************************Method Header********************************
//Method Name   :GetAutoGetIntervalScratchPadPath()
//Author        :
//Purpose       :
//*****************************************************************************
char* CPqmUtilities::GetAutoGetIntervalScratchPadPath()
{
    wchar_t buffer[MAX_PATH] = {L'\0'};;
    ::GetEnvironmentVariable(_T("GP_TMP_DIR"), buffer, MAX_PATH);
    CString f_auto_insert_scratch_pad  = buffer;
    f_auto_insert_scratch_pad += AUTO_GET_INTERVAL_SCRATCH_PAD;

    const int l_size = f_auto_insert_scratch_pad.GetLength();
    char* l_file_name = new char[l_size + 1];
    memset(l_file_name, 0, l_size + 1);
    wcstombs(l_file_name, f_auto_insert_scratch_pad, l_size + 1);
    return l_file_name;
}
//********************************Method Header********************************
//Method Name   :GetPqmDefaultPGUIDFileName()
//Author        :
//Purpose       :
//*****************************************************************************
CPTString CPqmUtilities::GetPqmDefaultPGUIDFileName()
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;

    if (l_file_name.IsEmpty()) {
        return l_file_name;
    }

    l_file_name += _T("\\");
    l_file_name += _T("PQM\\");
    l_file_name += PQM_DEFAULT_PGUID;

    return l_file_name;
}