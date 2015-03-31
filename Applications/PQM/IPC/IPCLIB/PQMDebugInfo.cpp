//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-779
//Redmine-780
//Redmine-781
// PQMDebugInfo.cpp: implementation of the CPQMDebugInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMDebugInfo.h"
#include <tami/common/machdefs.h>
#include <tami/common/basicdefs.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include "couchconfig.h"
#include "ProtocolUtil.h"
#include "pqmconfig.h"


//Patni-HAR/2010Apr12/Added/Phase#3-CDS Requirement
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "CDSConstants.h"

//#include "sarp.h"
#include "Pqm.h"
#include "PqmPrMode.h"
#include "scanprocedure.h"
#include "pqmpmacqman.h"
#include "PQMSAR.h"
#include "PqmCoilData.h"
#include "PqmPmSARManager.h"

#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345

typedef std::vector<CPqmProtocol*> protocolvec;

typedef struct vfDBSysInfo {
    struct vfDBSysInfo*	next;
    int		open_type;	/* orig. type: FILE, PIPE, etc. */
    int		open_mode;	/* O_RDONLY or O_RDWR		*/
    int		db_type;	/* One of VF_FILE or VF_MEMORY	*/
    size_t	db_size;
    union {
        int	fd;
        char* 	vaddr;
    } source;


    vfDBSysInfo() :
        next(NULL),
        open_type(0),
        open_mode(0),
        db_type(0),
        db_size(0u) {

        source.fd = 0 ;
        source.vaddr = NULL ;
    }

}* vfDB_p;

CPQMDebugInfo* CPQMDebugInfo::m_pqm_debuginfo = NULL;
//Patni-DKH/2010May13/Modified/Phase3#Redmine-210
CPqm* CPQMDebugInfo::m_pqm_ptr = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//************************************Method Header****************************
// Method Name  : CPQMDebugInfo
// Author       : PATNI
// Purpose      : Ctor of CPQMDebugInfo
//*****************************************************************************
//Patni-DKH/2010May13/Modified/Phase3#Redmine-210
CPQMDebugInfo::CPQMDebugInfo()
    : m_iCUR_DEBUG(0)
{
    //Create the Status log file
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::CPQMDebugInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMDebugInfo constructor"));
    PqmDebugStatusLogOpen();
}

//************************************Method Header****************************
// Method Name  : ~CPQMDebugInfo
// Author       : PATNI
// Purpose      : Deletes the CPQMDebugInfo object
//*****************************************************************************
CPQMDebugInfo::~CPQMDebugInfo()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::~CPQMDebugInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMDebugInfo constructor"));
    //Close the status log file
    PqmDebugStatusLogClose();
}

//*************************************************************************
//Method Name   :   GetInstance
//Author        :   PATNI/DKH
//Purpose       :   Returns the single Instance of CPQMDebugInfo class
//*************************************************************************
CPQMDebugInfo* CPQMDebugInfo::GetInstance(CPqm* f_pqmptr)
{
    //+Patni-DKH/2010May13/Modified/Phase3#Redmine-210
    if (NULL == m_pqm_ptr)
        m_pqm_ptr =  f_pqmptr;

    static CPQMDebugInfo m_pqm_debuginfo ;
    //-Patni-DKH/2010May13/Modified/Phase3#Redmine-210

    return &m_pqm_debuginfo;
}

//************************************Method Header****************************
// Method Name  : PrescanDebug()
// Author       : PATNI \ HAR
// Purpose      :
//*****************************************************************************
void CPQMDebugInfo::PrescanDebug(
    const AM_ClientToAcqMan_unicode_t& f_data
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PrescanDebug");

    CScanProcedure* l_scan_procedure = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (!l_scan_procedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Scan Procedure Pointer is NULL"));
        return;
    }

    CPqmProtocol* l_pqm_protocol = l_scan_procedure->GetScanProcProtocol();

    if (!l_pqm_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmProtocol Pointer is NULL"));
        return;
    }

    const int l_hold_scan = l_pqm_protocol->GetHoldScan();

    const int l_auto_hold = l_pqm_protocol->GetAutoHold();

    CPSAcqSetFlagVec curr_action = *(l_scan_procedure->GetCurAction());

    //+Patni-Hemant/2010Apr12/Added/Phase#3-CDS Requirement
    for (int k = 0; ((k < NUM_ACQ_FLAG) && (k < (int)curr_action.size())); k++) {
        if (ACQ_PRESCAN_CDS == f_data.param.preScan.kind[0]) {
            continue ;
        }

        if (curr_action[k]) {
            PqmDebugAcqMonitor(_T("Send PreScan : %-8s  %s"), GetStringForPrescanKind(curr_action[k]->GetTag()), GetStringForPrescanAction(curr_action[k]->GetData()));
        }
    }

    PqmDebugAcqMonitor(_T("Hold Scan : %s %s %s %s %s %s"),
                       l_hold_scan & VFC_HOLD_PRE_SCAN	? _T("PRE") : _T(""),
                       l_hold_scan & VFC_HOLD_APC 		? _T("APC") : _T(""),
                       l_hold_scan & VFC_HOLD_CFA 		? _T("CFA") : _T(""),
                       l_hold_scan & VFC_HOLD_RGN 		? _T("RGN") : _T(""),
                       l_hold_scan & VFC_HOLD_MAIN_SCAN	? _T("MAIN") : _T(""),
                       l_hold_scan & VFC_HOLD_COVERAGE	? _T("COV") : _T(""));

    PqmDebugAcqMonitor(_T("Auto Hold : %s %s %s %s %s %s"),
                       l_auto_hold & VFC_HOLD_PRE_SCAN	? _T("PRE") : _T(""),
                       l_auto_hold & VFC_HOLD_APC 		? _T("APC") : _T(""),
                       l_auto_hold & VFC_HOLD_CFA 		? _T("CFA") : _T(""),
                       l_auto_hold & VFC_HOLD_RGN 		? _T("RGN") : _T(""),
                       l_auto_hold & VFC_HOLD_MAIN_SCAN	? _T("MAIN") : _T(""),
                       l_auto_hold & VFC_HOLD_COVERAGE	? _T("COV") : _T(""));
}

//************************************Method Header****************************
// Method Name  : StartPQMDebug()
// Author       : PATNI
// Purpose      : To start the debugging of selected option
//*****************************************************************************
void CPQMDebugInfo::StartPQMDebug(
    const int f_idebugoption,
    const UINT f_buttonmask
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::StartPQMDebug");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace StartPQMDebug"));

    try {
        if (DEBUG_CLOSE == f_idebugoption) {
            if (m_logFile.hFileNull != m_logFile.m_hFile) {
                m_logFile.Close();

            }

            return ;
        }

        if (f_buttonmask == CTRL_BUTTON_MASK) {
            //if log file is open close it
            if (m_logFile.hFileNull != m_logFile.m_hFile) {
                m_logFile.Close();
            }

            //Get path of temp folder
            TCHAR		l_tmp_path[MAX_PATH] = {0};
            TCHAR		l_buf[MAX_PATH] = {0};

            if (::GetEnvironmentVariable(_T("RELEASE"), l_tmp_path, MAX_PATH * sizeof(TCHAR)) != 0) {
                //Append the the Log File name to it
                _stprintf(l_buf, _T("%s%s%s"), l_tmp_path, _T("/tmp"), PQMDEBUG_LOGFILE);

            } else {
                _stprintf(l_buf, _T("%s%s"), _T("/usr/tmp"), PQMDEBUG_LOGFILE);
            }

            //Open the file with Wt mode.
            if (FALSE == m_logFile.Open(l_buf, CFile::modeCreate | CFile::modeNoTruncate |
                                        CFile::typeText | CFile::modeReadWrite | CFile::shareDenyWrite)) {
                CString l_msg = (_T(""));
                l_msg.Format(_T("Failed to create log file : %s"), l_buf);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
            }
        }

        //Delegate the task of selected option to appropriate handler function.
        switch (f_idebugoption) {
            case DEBUG_GLOBAL:
                //display global data
                pqmDebugDisplayGlobal();
                break;

            case   DEBUG_STUDY:
                //display study related data
                pqmDebugDisplayStudy();
                break;

            case   DEBUG_LIST_COUNT:
                //display list count
                pqmDebugDisplayCount();
                break;

            case   DEBUG_LIST_ARRAY:
                //display list array data
                pqmDebugDisplayList();
                break;

            case   DEBUG_PROT_INFO:
                //display protocol related data
                pqmDebugDisplayProtData();
                break;

            case   DEBUG_SAR_INFO:
                //disply SAR information
                pqmDebugSar();
                break;

            case   DEBUG_SIZE:
                //display size of various data structures
                pqmDebugSize();
                break;

            case DEBUG_COUCH_INFO:
                //display couch info
                pqmDebugDisplayCouch();
                break;

            case   DEBUG_SHIMMING:
                //display shimming related data
                pqmDebugShimming();
                break;

            case   DEBUG_LIST_MONITOR:
                // start list monitoring
                pqmDebugPq();
                m_iCUR_DEBUG |= PQM_DEBUG_MON_LIST ;
                break;

            case   DEBUG_SAR_MONITOR:
                //start sar data monitoring
                pqmDebugSarMon();
                m_iCUR_DEBUG |= PQM_DEBUG_MON_SAR ;
                break;

            case DEBUG_ACQ_MONITOR :
                //start acquisition monitoring
                pqmDebugAcq();
                m_iCUR_DEBUG |= PQM_DEBUG_MON_ACQ ;
                break;

            case DEBUG_PRO_MONITOR :
                //start procon monitoring
                m_iCUR_DEBUG |= PQM_DEBUG_MON_PRO ;
                break;

            case   DEBUG_IRS_MONITOR:
                //start IRS monitoring
                m_iCUR_DEBUG |= PQM_DEBUG_MON_IRS ;
                break;

            case   DEBUG_RESET:
                //Acq Info
                pqmDebugReset();

            case   DEBUG_MONITOR_OFF:

                //disable all monitoring
                //+Patni-DKH/2010May17/Modified/Phase3#Redmine-210
                if (m_iCUR_DEBUG & PQM_DEBUG_MON_SAR) {
                    SarSetDebug(NULL);
                }

                //-Patni-DKH/2010May17/Modified/Phase3#Redmine-210
                m_iCUR_DEBUG = 0;
                break;

            default:
                break;
        }
    }


    catch (CMemoryException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return;

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return ;

    } catch (CException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unhandled exception occured"));
    }

    return ;
}

//************************************Method Header****************************
// Method Name  : PqmDebugStatusLogOpen()
// Author       : PATNI
// Purpose      : To open the staus log file
//*****************************************************************************
bool CPQMDebugInfo::PqmDebugStatusLogOpen()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmDebugStatusLogOpen");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmDebugStatusLogOpen"));

    try {
        if (m_statuslogfile.hFileNull == m_statuslogfile.m_hFile) {
            //Get path of the status log flag file

            TCHAR l_etcpath[MAX_PATH] = {0};

            CString l_stauslogflag_filepath = _T("");

            if (::GetEnvironmentVariable(_T("GP_ETC_PATH"), l_etcpath, sizeof(TCHAR) * MAX_PATH) != 0) {
                //Append the the Log File name to it
                l_stauslogflag_filepath.Format(_T("%s%s"), l_etcpath, STATUS_LOG_CTRLFILE);

            } else {
                l_stauslogflag_filepath.Format(_T("%s%s"), _T("/gp/etc/"), STATUS_LOG_CTRLFILE);
            }


            CStdioFile	l_statlogflagfile;
            l_statlogflagfile.Open(l_stauslogflag_filepath, CFile::typeText | CFile::modeRead);

            if (l_statlogflagfile.hFileNull != l_statlogflagfile.m_hFile) {
                CString l_strfileflag = _T("");
                l_statlogflagfile.ReadString(l_strfileflag);

                CString l_strdatetimestamp(_T(""));
                CString l_statusfile(_T("")) ;
                CString l_msg(_T(""));
                TCHAR   l_buff[MAX_PATH] = {0};

                if (l_strfileflag.CompareNoCase(_T("1")) == 0) {
                    for (int i = 0; ; i++) {
                        l_strdatetimestamp = GetDateTimeStamp();

                        _stprintf(l_buff, _T("%s%s.%s"), STATUS_LOG_DIR, STATUS_LOG_FILE, l_strdatetimestamp);
                        l_statusfile = l_buff ;

                        m_statuslogfile.Open(l_statusfile, CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText | CFile::modeReadWrite | CFile::shareDenyWrite);

                        if (m_statuslogfile.hFileNull == m_statuslogfile.m_hFile) {


                            if (i > 3)
                                break;

                            if (FALSE == ::CreateDirectory(STATUS_LOG_DIR, NULL)) {
                                l_msg.Format(_T("Can't open File:%s"), l_buff);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
                                break;
                            }

                        } else {
                            break;
                        }
                    }
                }
            }
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return false;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Error occured"));
        return false;
    }

    return true;
}

//**************************************************************************
//Method Name   : PqmDebugAcqLogOpen
//Author        : PATNI\DKH
//Purpose       : To open acqlog file
//**************************************************************************
void CPQMDebugInfo::PqmDebugAcqLogOpen()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmDebugAcqLogOpen");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
              , _T("Trace of PqmDebugAcqLogOpen"));

    try {

        TCHAR	l_tmp_path[MAX_PATH] = {0};
        CString l_filepath(_T(""));

        if (::GetEnvironmentVariable(_T("RELEASE"), l_tmp_path, MAX_PATH * sizeof(TCHAR)) != 0) {
            //Append the the Log File name to it
            l_filepath.Format(_T("%s%s%s"), l_tmp_path, _T("/tmp"), PQMACQDBG_FILE);

        } else {
            l_filepath.Format(_T("%s%s"), _T("/usr/tmp"), PQMACQDBG_FILE);
        }

        CStdioFile l_acqdbgfile;

        //Open the file with Wt mode.
        if (FALSE == l_acqdbgfile.Open(l_filepath, CFile::typeText | CFile::modeReadWrite)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
                      , _T("Error while opening acqlog settings file"));
            return;

        } else {
            CString l_acqdbg_filename(_T(""));

            l_acqdbgfile.ReadString(l_acqdbg_filename);

            if (l_acqdbg_filename == _T("")) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
                          , _T("Filename is not mentioned for logging data"));
                return;
            }

            const int l_lastslashpos = l_acqdbg_filename.ReverseFind(_T('\\'));

            const CString l_acqdbgfilepath = l_acqdbg_filename.Left(l_lastslashpos);

            if (FALSE == PathFileExists(l_acqdbgfilepath)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
                          , _T("File path for Acq monitor debug file does not exist."));
                return;
            }

            CString l_datetimestamp = GetDateTimeStamp();

            if (m_acqlogfile.hFileNull != m_acqlogfile.m_hFile) {
                m_acqlogfile.Close();
            }

            CString l_strdot = _T(".");
            l_acqdbg_filename += l_strdot ;
            l_acqdbg_filename += l_datetimestamp ;
            m_acqlogfile.Open(l_acqdbg_filename,
                              CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText | CFile::modeReadWrite | CFile::shareDenyWrite);

            if (m_acqlogfile.hFileNull == m_acqlogfile.m_hFile) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
                          , _T("Error while opening Acqlog file"));
            }
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unhandled Exception"));
    }
}

//************************************Method Header************************************
// Method Name  : PqmDebugAcqMonitor()
// Author       : PATNI
// Purpose      : To handle acquisition monitoring
//***********************************************************************************
void CPQMDebugInfo::PqmDebugAcqMonitor(
    const CString f_csDebugData,
    ...
)
{
    LPCTSTR FUNC_NAME =  _T("CPQMDebugInfo::PqmDebugAcqMonitor");

    va_list	args;
    TCHAR l_csmsg[1024] = {0};

    try {
        va_start(args, f_csDebugData);
        _vstprintf(l_csmsg, f_csDebugData, args);
        va_end(args);

        if (m_logFile.hFileNull != m_logFile.m_hFile) {
            m_logFile.SeekToEnd();
            m_logFile.WriteString(l_csmsg);
            m_logFile.WriteString(_T("\n"));
            m_logFile.Flush();

        }

        if (m_statuslogfile.hFileNull != m_statuslogfile.m_hFile) {
            CString l_strlogmsg(_T(""));
            CString l_strdatetimestamp =  GetDateTimeStamp();
            l_strlogmsg.Format(_T("%s:%s\n"), l_strdatetimestamp, l_csmsg);

            //Write the msg in status log file
            m_statuslogfile.WriteString(l_strlogmsg);
            m_statuslogfile.Flush();
        }

        if (m_acqlogfile.hFileNull != m_acqlogfile.m_hFile) {
            //Get date time stamp
            CString l_strlogmsg(_T(""));
            CString l_strdatetimestamp =  GetDateTimeStamp(true);
            l_strlogmsg.Format(_T("%s %s\n"), l_strdatetimestamp, l_csmsg);
            m_acqlogfile.SeekToEnd();
            m_acqlogfile.WriteString(l_strlogmsg);
            m_acqlogfile.Flush();
        }

        //if acq monitor debug selected then send message to
        //display on UI.
        if (m_iCUR_DEBUG & PQM_DEBUG_MON_ACQ) {
            m_pqm_ptr->WriteDebugDataToUI(l_csmsg);
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Error occured"));
        return ;
    }
}

//************************************Method Header************************************
// Method Name  : PqmDebugIrsMonitor()
// Author       : PATNI\DKH
// Purpose      : To handle IRS monitoring
//***********************************************************************************
void CPQMDebugInfo::PqmDebugIrsMonitor(
    const CString f_csDebugData,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmDebugIrsMonitor");

    try {

        va_list	args;
        TCHAR l_csmsg[1024] = {0};
        va_start(args, f_csDebugData);
        _vstprintf(l_csmsg, f_csDebugData, args);
        va_end(args);

        if (m_logFile.hFileNull != m_logFile.m_hFile) {
            m_logFile.SeekToEnd();
            m_logFile.WriteString(l_csmsg);
            m_logFile.Flush();
        }

        if (m_statuslogfile.hFileNull != m_statuslogfile.m_hFile) {
            CString l_strlogmsg(_T(""));
            CString l_strdatetimestamp =  GetDateTimeStamp();
            l_strlogmsg.Format(_T("%s:%s\n"), l_strdatetimestamp, l_csmsg);

            //Write the msg in status log file
            m_statuslogfile.WriteString(l_strlogmsg);
            m_statuslogfile.Flush();
        }

        //if IRS monitor debug selected then send message to
        //display on UI.
        if (m_iCUR_DEBUG & PQM_DEBUG_MON_IRS) {
            m_pqm_ptr->WriteDebugDataToUI(l_csmsg);
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Error occured"));
        return ;
    }
}

//************************************Method Header************************************
// Method Name  : PqmDebugProMonitor()
// Author       : PATNI\DKH
// Purpose      : To handle prolim monitoring
//***********************************************************************************
void CPQMDebugInfo::PqmDebugProMonitor(
    const CString f_csDebugData,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmDebugProMonitor");

    try {
        va_list	args;
        TCHAR l_csmsg[1024] = {0};
        va_start(args, f_csDebugData);
        _vstprintf(l_csmsg, f_csDebugData, args);
        va_end(args);

        if (m_logFile.hFileNull != m_logFile.m_hFile) {
            m_logFile.SeekToEnd();
            m_logFile.WriteString(l_csmsg);
            m_logFile.Flush();
        }

        if (m_statuslogfile.hFileNull != m_statuslogfile.m_hFile) {
            CString l_strlogmsg(_T(""));
            CString l_strdatetimestamp =  GetDateTimeStamp();
            l_strlogmsg.Format(_T("%s:%s\n"), l_strdatetimestamp, l_csmsg);

            //Write the msg in status log file
            m_statuslogfile.WriteString(l_strlogmsg);
            m_statuslogfile.Flush();
        }

        //if prolim monitor debug selected then send message to
        //display on UI.
        if (m_iCUR_DEBUG & PQM_DEBUG_MON_PRO) {
            m_pqm_ptr->WriteDebugDataToUI(l_csmsg);
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Error occured"));
        return ;
    }
}

//************************************Method Header****************************
// Method Name  : pqmDebugDisplayCount()
// Author       : PATNI
// Purpose      : To handle the list count debug option
//*****************************************************************************
void CPQMDebugInfo::pqmDebugDisplayCount(void)
{
    //+Patni-AMT/2010Apr12/Modified/JaFT# IR-86 code review-COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    CPqmProtocol* l_pqmprotwait_obj = NULL;
    l_pqmstudy->GetTopWaitPos(&l_pqmprotwait_obj);

    int		l_wait_pos  = -1;

    if (l_pqmprotwait_obj != NULL) {
        l_wait_pos = l_pqmprotwait_obj->GetPosition();

    } else {
        l_wait_pos = 0;
    }

    //Patni-DKH/2010Mar02/Added/PH#3#/IR86/DebugWindow - WSE opening issue
    const int l_all_count = l_pqmstudy->GetProtCountFromVec();

    pqmDebugAddList(g_dbgsep);

    CPqmProtocol* l_pqmprotcur_obj = l_pqmstudy->GetCurrentProtocol();

    //-Patni-AMT/2010Apr12/Modified/JaFT# IR-86 code
    if (NULL != l_pqmprotcur_obj) {
        pqmDebugAddList(_T("all = %d, wait = %d, current = %d (%d)"),
                        l_all_count, l_wait_pos, l_pqmprotcur_obj->GetPosition(),
                        l_pqmprotcur_obj->GetProtocol());

    } else {
        pqmDebugAddList(_T("all = %d, wait = %d, current = 0 (-1)"),
                        l_all_count, l_wait_pos);
    }

    PqmPrDebug();
}

//************************************Method Header************************************
// Method Name  : pqmDebugDisplayList()
// Author       : PATNI
// Purpose      : To display debug data for list
//***********************************************************************************
void CPQMDebugInfo::pqmDebugDisplayList(
    void
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of pqmDebugDisplayList"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    protocolvec l_protocolvector = l_pqmstudy->GetProtocolvector();

    CString l_sts = _T("");
    protocolvec::iterator v1_Iter  = NULL;
    CPqmProtocol* l_pqmprotocol = NULL;

    pqmDebugAddList(g_dbgsep);

    for (v1_Iter = l_protocolvector.begin();
         v1_Iter != l_protocolvector.end();
         v1_Iter++) {
        l_pqmprotocol = (CPqmProtocol*) * v1_Iter;

        if (NULL == l_pqmprotocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprotocol pointer is NULL"));
            break;
        }

        l_sts = _T("");

        switch (l_pqmprotocol->GetAcqStatus()) {
            case C_PR_STATUS_WAIT:
                l_sts = _T("WAIT");
                break;

            case VFC_ACQ_STATUS_SUCCESS:
                l_sts = _T("SUCCESS");
                break;

            case VFC_ACQ_STATUS_FAIL:
                l_sts = _T("FAILED");
                break;

            case VFC_ACQ_STATUS_CURRENT:
                l_sts = _T("CURRENT");
                break;

            case PQ_FILLER_DONE:
                l_sts = _T("Filler Done");
                break;

            case PQ_FILLER_CURR:
                l_sts = _T("Filler Current");
                break;

            case PQ_FILLER_WAIT:
                l_sts = _T("Filler Wait");
                break;

            default:
                l_sts = _T("Unknown");
                break;
        }

        if (l_sts.CompareNoCase(_T("")) == 0) {
            continue;
        }

        CPqmPrMode* l_pqmprmode = l_pqmprotocol->GetPqmPrMode();

        if (NULL == l_pqmprmode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprmode pointer is NULL"));
            continue;
        }

        int* l_image = l_pqmprotocol->GetImageGr();

        if (NULL != l_image) {
            CString l_cstemp(_T(""));
            //+Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow
            l_cstemp.Format(_T("[%2d:%2d] (%2d,%2d)"), l_pqmprotocol->GetPosition(),
                            l_pqmprotocol->GetProtocol(), l_image[0], l_image[1]);
            //-Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow
            pqmDebugAddList(_T("%s %s"),
                            l_cstemp , l_sts);
        }
    }
}

//************************************Method Header************************************
// Method Name  : pqmDebugDisplayCouch()
// Author       : PATNI
// Purpose      : To display couch related debug info
//***********************************************************************************
void CPQMDebugInfo::pqmDebugDisplayCouch(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayCouch");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of pqmDebugDisplayCouch"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    protocolvec l_protocolvector = l_pqmstudy->GetProtocolvector();

    CString l_sts = _T("");
    protocolvec::iterator v1_Iter  = NULL;
    CPqmProtocol* l_pqmprotocol;

    pqmDebugAddList(g_dbgsep);
    pqmDebugAddList(_T("[index] scanNumber couchPos homePos couchOffset couchOffsetflag"));

    for (v1_Iter = l_protocolvector.begin();
         v1_Iter != l_protocolvector.end();
         v1_Iter++) {
        l_pqmprotocol = (CPqmProtocol*) * v1_Iter;

        if (NULL == l_pqmprotocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprotocol pointer is NULL"));
            break;
        }

        l_sts = _T("");

        switch (l_pqmprotocol->GetAcqStatus()) {
            case C_PR_STATUS_WAIT:
                l_sts = _T("WAIT");
                break;

            case VFC_ACQ_STATUS_SUCCESS:
                l_sts = _T("DONE");
                break;

            case VFC_ACQ_STATUS_FAIL:
                l_sts = _T("FAIL");
                break;

            case VFC_ACQ_STATUS_CURRENT:
                l_sts = _T("CURR");
                break;

            default:
                l_sts = _T("UNKNOWN");
                break;
        }

        if (l_sts.Compare(_T(""))) {
            CPqmPrMode* l_pqmprmode = l_pqmprotocol->GetPqmPrMode();

            if (NULL != l_pqmprmode) {
                pqmDebugAddList(_T("[%2d] %s %d %f %f %f %5d"),
                                l_pqmprotocol->GetProtocol(), l_sts, l_pqmprmode->GetScanNumber(),
                                l_pqmprmode->GetCouchPos(), l_pqmprmode->GetHomePos(),
                                l_pqmprmode->GetCouchOffset(), l_pqmprotocol->GetCouchOffsetFlag());
            }
        }
    }
}

//************************************Method Header************************************
// Method Name  : pqmDebugDisplayGlobal()
// Author       : PATNI
// Purpose      : To display global data structure data
//***********************************************************************************
void CPQMDebugInfo::pqmDebugDisplayGlobal(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayGlobal");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL != m_pqm_ptr) {
        pqmDebugAddList(g_dbgsep);

        PqmShmDebug();

        pqmDebugAddList(g_dbgsep);

        CCouchConfig* l_objCouchconfig = m_pqm_ptr->GetCouchConfigPtr();

        if (NULL != l_objCouchconfig) {
            pqmDebugAddList(_T("InsideLimit     = %f"), l_objCouchconfig->GetCouchInLimit());
            pqmDebugAddList(_T("OutsideLimit    = %f"), l_objCouchconfig->GetCouchOutLimit());
            pqmDebugAddList(_T("Minimum Move    = %f"), l_objCouchconfig->GetCouchMinimumMovement());
            pqmDebugAddList(_T("Maximum Move    = %f"), l_objCouchconfig->GetCouchMaximumMovement());
            pqmDebugAddList(_T("Slow Speed Range= %f"), l_objCouchconfig->GetCouchSpeedRange());

        }

        pqmDebugAddList(g_dbgsep);
        pqmDebugAddList(_T("mode            = %d"), m_pqm_ptr->GetMode());
        pqmDebugAddList(_T("sockIrs         = %d"), m_pqm_ptr->GetIRSScoketFD());
        pqmDebugAddList(_T("sockAcqMan      = %d"), m_pqm_ptr->GetAcqmanSocketID());
        pqmDebugAddList(_T("couchPac        = %d"), m_pqm_ptr->GetCouchpac());
        //+Patni-ARD/201APR14/Modified/PH#3#/IR86-IR-100/DebugWindow# max offset
        pqmDebugAddList(_T("maxoffset       = %d,%d"),
                        m_pqm_ptr->GetXOffsetLimit(), m_pqm_ptr->GetYOffsetLimit());
        //-Patni-ARD/201APR14/Modified/PH#3#/IR86-IR-100/DebugWindow# max offset
        pqmDebugAddList(_T("sys_type        = %d"), m_pqm_ptr->GetSysType());
        const long l_maxprotocol = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();
        CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();
        pqmDebugAddList(_T("max_protocols      = %d"), l_maxprotocol);

        study_id_t l_study ;
        l_study = m_pqm_ptr->GetAnotherPqmStudy();
        pqmDebugAddList(_T("anotherPqmStudy = Run%d.%d"), l_study.run_num, l_study.site_id);
        pqmDebugAddList(_T("autoPlan        = %d"), m_pqm_ptr->GetAutoPlan());
        pqmDebugAddList(_T("planAndScan     = %d"), m_pqm_ptr->GetPlanAndScan());
        pqmDebugAddList(_T("numChannels     = %d"), m_pqm_ptr->GetNumChannels());
        pqmDebugAddList(_T("channel_mask    = 0x%x"), m_pqm_ptr->GetChannel_mask());
        pqmDebugAddList(_T("High Speed Couch= %d"), m_pqm_ptr->GetHiSpeedCouch());

    }

    PqmCoilDebug();
    //PqmCdsDebug(pqmDebugAddList); // TODO: To be implemented after coding of CDS
}
//**************************************************************************
//Method Name   : pqmDebugDisplayStudy()
//Author        : PATNI/RAJ
//Purpose       : To display study data
//**************************************************************************
void CPQMDebugInfo::pqmDebugDisplayStudy(void)
{
    //Patni-AMT/2010Apr12/Modified/JaFT# IR-86 Code Optimization-COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    pqmDebugAddList(g_dbgsep);
    pqmDebugAddList(_T("study_name  = %s"), l_pqmstudy->GetStudyName());
    pqmDebugAddList(_T("vfHandle    = %x"), l_pqmstudy->GetVfDBHandle());
    pqmDebugAddList(_T("studyId     = Run%d.%d"), l_pqmstudy->GetRumNum(),
                    l_pqmstudy->GetSiteId());
    pqmDebugAddList(_T("weight      = %g"), l_pqmstudy->GetWeight());
    pqmDebugAddList(_T("gender      = %d"), l_pqmstudy->GetGender());
    pqmDebugAddList(_T("hasImage    = %d"), l_pqmstudy->GetHasImageInStudy());
    pqmDebugAddList(_T("firstScan   = %d"), l_pqmstudy->GetFirstScan());
    pqmDebugAddList(_T("prCount     = %d"), l_pqmstudy->GetProtCountFromVec());
}

//************************************Method Header************************************
// Method Name  : pqmDebugDisplayProtData()
// Author       : PATNI
// Purpose      : To display protocol information
//***********************************************************************************
void CPQMDebugInfo::pqmDebugDisplayProtData(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugDisplayProtData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Enter in pqmDebugDisplayProtData"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMIPCManager* l_ipcmgr = m_pqm_ptr->GetManagerPtr() ;

    if (NULL == l_ipcmgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ipcmgr pointer is NULL"));
        return ;
    }

    int* l_selectedlist = l_ipcmgr->GetSelectedList();
    const int l_count = l_ipcmgr->GetTotalListCnt();
    CPQMStudy* l_ppqmstudy =  m_pqm_ptr->GetStudy();

    if (NULL == l_ppqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("The CPQMStudy object is NULL"));
        return;
    }

    pqmDebugAddList(g_dbgsep);
    CPqmProtocol* l_pqmprotocol = NULL;

    if (l_selectedlist != NULL) {
        for (int i = 0; i < l_count; i++) {
            if ((l_pqmprotocol = l_ppqmstudy->GetItemFromPosition(l_selectedlist[i] + 1)) != NULL) {
                if (NULL == l_pqmprotocol) {
                    pqmDebugAddList(_T("[%d] -> No data."), l_selectedlist[i]);
                    continue;
                }

                pqmDebugProtData(l_pqmprotocol);
            }
        }
    }

    DEL_PTR_ARY(l_selectedlist);

}
//**************************************************************************
//Method Name   : pqmDebugAcq()
//Author        : PATNI/RAJ
//Purpose       : To display acquition related data
//**************************************************************************
void CPQMDebugInfo::pqmDebugAcq(void)
{

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugAcq");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    pqmDebugAddList(_T("Couch Position = %g"), m_pqm_ptr->GetCouchPos());

}

//************************************Method Header************************************
// Method Name  : pqmDebugSize()
// Author       : PATNI
// Purpose      : To display size and handle related dataof global structures
//***********************************************************************************
void CPQMDebugInfo::pqmDebugSize(void)
{

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugSize");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    vfDB_p	vf_p;

    pqmDebugAddList(g_dbgsep);
    pqmDebugAddList("PqmGlobal_t           = %d", sizeof(CPqm));
    pqmDebugAddList("  PqmStudy_t          = %d", sizeof(CPQMStudy));
    pqmDebugAddList("PqmPrData_t           = %d", sizeof(CPqmProtocol));
    pqmDebugAddList("  sarInfo_t           = %d", sizeof(sarInfo_t));
    pqmDebugAddList("    sarSepParams_t    = %d", sizeof(sarSepParams_t));
    pqmDebugAddList("      sarWaveParams_t = %d", sizeof(sarWaveParams_t));
    pqmDebugAddList("VfFieldSpec_t         = %d", sizeof(VfFieldSpec_t));
    pqmDebugAddList("---- File Descriptor ----");

    if (NULL != m_pqm_ptr) {
        //Status of IRS listner socket.
        const int l_irssocketid = m_pqm_ptr->GetIRSScoketFD() ;

        if (l_irssocketid != -1) {
            pqmDebugAddList(_T("[%3d] Enable (IRS)"), l_irssocketid);
        }

        //Status of Acqman socket.
        const int l_acqmansocketid = m_pqm_ptr->GetAcqmanSocketID() ;

        if (l_acqmansocketid != 0) {
            pqmDebugAddList(_T("[%3d] Enable (AcqMan)"), l_acqmansocketid);
        }
    }

    if (m_logFile.hFileNull != m_logFile.m_hFile) {
        pqmDebugAddList("[%3x] Enable (logFp)", m_logFile.m_hFile);
    }

    if (m_statuslogfile.hFileNull != m_statuslogfile.m_hFile) {
        pqmDebugAddList("[%3x] Enable (stsLogFp)", m_statuslogfile.m_hFile);
    }

    if (m_acqlogfile.hFileNull != m_acqlogfile.m_hFile) {
        pqmDebugAddList("[%3x] Enable (acqLogFp)", m_acqlogfile.m_hFile);
    }

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    for (vf_p = reinterpret_cast<vfDB_p>(l_pqmstudy->GetVfDBHandle()->Get());
         vf_p != NULL; vf_p = vf_p->next) {
        pqmDebugAddList("VF [%3d]", vf_p->source.fd);
    }
}

//**************************************************************************
//Method Name   : pqmDebugReset()
//Author        : PATNI/RAJ
//Purpose       : To diaply data on click of Acq info button from GUI
//**************************************************************************
void CPQMDebugInfo::pqmDebugReset(void)
{

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugReset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPqmPmAcqman* l_ptrpqmpmacqman = m_pqm_ptr->GetAcqManager();

    if (NULL == l_ptrpqmpmacqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptrpqmpmacqman  pointer is NULL"));
        return ;
    }

    CScanProcedure* l_ptrscanprocedure = l_ptrpqmpmacqman->GetScanProcedure();

    if (NULL == l_ptrscanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptrscanprocedure pointer is NULL"));
        return ;
    }

    pqmDebugAddList(_T("scanStart  = %d"), l_ptrscanprocedure->GetScanStart());
    pqmDebugAddList(_T("scanPause  = %d"), l_ptrscanprocedure->GetScanPause());
    pqmDebugAddList(_T("scanAbort  = %d"), l_ptrscanprocedure->GetScanAbort());
    pqmDebugAddList(_T("processing = %d"), l_ptrscanprocedure->GetProcessing());
    pqmDebugAddList(_T("request    = %d"), l_ptrscanprocedure->GetRequest());

}

//************************************Method Header************************************
// Method Name  : pqmDebugPq()
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMDebugInfo::pqmDebugPq(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugPq");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of pqmDebugPq"));

    pqmDebugAddList(g_dbgsep);
    PqmPqDebug();
}

//************************************Method Header************************************
// Method Name  : pqmDebugSar()
// Author       : PATNI
// Purpose      : To display Sar info
//***********************************************************************************
void CPQMDebugInfo::pqmDebugSar(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugSar");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMIPCManager* l_ipcmgr = m_pqm_ptr->GetManagerPtr() ;

    if (NULL == l_ipcmgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ipcmgr pointer is NULL"));
        return ;
    }

    int* l_selectedlist = l_ipcmgr->GetSelectedList();
    const int l_count = l_ipcmgr->GetTotalListCnt();

    CPQMStudy* l_ppqmstudy =  m_pqm_ptr->GetStudy();

    if (NULL == l_ppqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("The CPQMStudy object is NULL"));
        return;
    }

    CPqmProtocol* l_pqmprotocol = NULL;

    pqmDebugAddList(_T("[pqmDebugSar]:"));

    pqmDebugAddList(g_dbgsep);

#ifdef DEBUG
    PqmSarDebugRfl();
#endif

    if (l_selectedlist != NULL) {
        for (int i = 0; i < l_count; i++) {
            if ((l_pqmprotocol = l_ppqmstudy->GetItemFromPosition(l_selectedlist[i] + 1)) != NULL) {
                if (NULL == l_pqmprotocol) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("The CPqmProtocol object is NULL"));
                    return;
                }

                pqmDebugSarInfo(l_pqmprotocol);
            }
        }
    }

    DEL_PTR_ARY(l_selectedlist);

}

//**************************************************************************
//Method Name   : pqmDebugSarInfo()
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CPQMDebugInfo::pqmDebugSarInfo(CPqmProtocol* f_pqmprotocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugSarInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    sarInfo_t* l_sarinfo = f_pqmprotocol->GetSarInfo();

    if (NULL == l_sarinfo) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_sarinfo pointer is NULL"));
        return ;
    }

    pqmDebugAddList(_T("----------- prot = %d (%d)------------>"),
                    f_pqmprotocol->GetProtocol(), f_pqmprotocol->GetPosition());
    pqmDebugAddList(_T("mode=%d"), f_pqmprotocol->GetAcqStatus());
    pqmDebugAddList(_T("sarInfo.sar                  = %g"), l_sarinfo->tsar[0].sar);
    pqmDebugAddList(_T("sarInfo.sar                  = %g"), l_sarinfo->tsar[1].sar);
    pqmDebugAddList(_T("sarInfo.sar                  = %g"), l_sarinfo->tsar[2].sar);
    pqmDebugAddList(_T("sarInfo.preal                = %g"), l_sarinfo->tsar[0].preal);
    pqmDebugAddList(_T("sarInfo.preal                = %g"), l_sarinfo->tsar[1].preal);
    pqmDebugAddList(_T("sarInfo.preal                = %g"), l_sarinfo->tsar[2].preal);
    pqmDebugAddList(_T("sarInfo.coilProtect          = %d"),
                    l_sarinfo->coilProtect);
    pqmDebugAddList(_T("sarInfo.scanEnable           = %d"),
                    l_sarinfo->scanEnable);
    pqmDebugAddList(_T("sarInfo.rflPulse.txGain      = %g"),
                    l_sarinfo->rflPulse.txGain);
    pqmDebugAddList(_T("sarInfo.rflPulse.scaler      = %g"),
                    l_sarinfo->rflPulse.scaler);
    pqmDebugAddList(_T("sarInfo.rflPulse.numPulses   = %d"),
                    l_sarinfo->rflPulse.numPulses);

    pqmDebugAddList(_T("sarInfo.rflPulse.useQChannel = %s"),
                    (l_sarinfo->rflPulse.useQChannel) ? _T("TRUE") : _T("FALSE"));

    pqmDebugAddList(_T("-------------------"));

    for (int n = 0; n < 2; n++) {
        pqmDebugAddList(_T("sarInfo.rflPulse.wave[%d].id      = %d"),
                        n, l_sarinfo->rflPulse.wave[n].id);
        pqmDebugAddList(_T("sarInfo.rflPulse.wave[%d].sum     = %f"),
                        n, l_sarinfo->rflPulse.wave[n].sum);
        pqmDebugAddList(_T("sarInfo.rflPulse.wave[%d].pitch   = %f"),
                        n, l_sarinfo->rflPulse.wave[n].pitch);
        pqmDebugAddList(_T("sarInfo.rflPulse.wave[%d].angle   = %d"),
                        n, l_sarinfo->rflPulse.wave[n].angle);
        pqmDebugAddList(_T("sarInfo.rflPulse.wave[%d].power   = %f"),
                        n, l_sarinfo->rflPulse.wave[n].power);
    }

    for (int i = 0; i < SAR_RF_MAX; i++) {
        pqmDebugAddList(_T("===================="));
        pqmDebugAddList(_T("sarInfo.pulses[%d].txGain         = %g"),
                        i, l_sarinfo->pulses[i].txGain);
        pqmDebugAddList(_T("sarInfo.pulses[%d].scaler         = %g"),
                        i, l_sarinfo->pulses[i].scaler);
        pqmDebugAddList(_T("sarInfo.pulses[%d].numPulses      = %d"),
                        i, l_sarinfo->pulses[i].numPulses);
        pqmDebugAddList(_T("sarInfo.pulses[%d].useQChannel    = %s"),
                        i, l_sarinfo->pulses[i].useQChannel ? _T("TRUE") : _T("FALSE"));

        for (n = 0; n < 2; n++) {
            pqmDebugAddList(_T("sarInfo.pulses[%d].wave[%d].id     = %d"),
                            i, n, l_sarinfo->pulses[i].wave[n].id);
            pqmDebugAddList(_T("sarInfo.pulses[%d].wave[%d].sum    = %g"),
                            i, n, l_sarinfo->pulses[i].wave[n].sum);
            pqmDebugAddList(_T("sarInfo.pulses[%d].wave[%d].pitch  = %g"),
                            i, n, l_sarinfo->pulses[i].wave[n].pitch);
            pqmDebugAddList(_T("sarInfo.pulses[%d].wave[%d].angle  = %d"),
                            i, n, l_sarinfo->pulses[i].wave[n].angle);
            pqmDebugAddList(_T("sarInfo.pulses[%d].wave[%d].power  = %g"),
                            i, n, l_sarinfo->pulses[i].wave[n].power);
        }
    }

    pqmDebugAddList(_T("-------------------"));
    pqmDebugAddList(_T("sarInfo.totalTime               = %g"), l_sarinfo->totalTime);
    pqmDebugAddList(_T("sarInfo.transmiterCoil          = 0x%x"), l_sarinfo->transmiterCoil);

    for (n = 0; n < 10; n++) {
        pqmDebugAddList(_T(
                            "sarInfo.receiverCoil[%d]         = 0x%x"), n, l_sarinfo->receiverCoil[n]);
    }

    for (n = 0; n < 10; n++) {
        pqmDebugAddList(_T(
                            "sarInfo.connectedCoil[%d]         = 0x%x"), n, l_sarinfo->connectedCoil[n]);
    }

    pqmDebugAddList(_T("sarInfo.scanAnatomy             = %d"), l_sarinfo->scanAnatomy);
    pqmDebugAddList(_T("sarInfo.couchPosition           = %g"), l_sarinfo->couchPosition);
    pqmDebugAddList(_T("sarInfo.patientWeight           = %g"), l_sarinfo->patientWeight);
    pqmDebugAddList(_T("sarInfo.sarWeight               = %g"), l_sarinfo->sarWeight);
    pqmDebugAddList(_T("sarInfo.weight_limit = %g"), l_sarinfo->weight_limit);

    pqmDebugAddList(_T("sarInfo.params.k_value     = %g"), l_sarinfo->params.k_value);
    pqmDebugAddList(_T("sarInfo.params.p_coil      = %g"), l_sarinfo->params.p_coil);
    pqmDebugAddList(_T("sarInfo.params.rf_max      = %g"), l_sarinfo->params.rf_max);
    pqmDebugAddList(_T("sarInfo.params.f1		= %g"), l_sarinfo->params.f1);
    pqmDebugAddList(_T("sarInfo.params.f2		= %g"), l_sarinfo->params.f2);
    pqmDebugAddList(_T("sarInfo.TGCRFoutRatio	= %g"), l_sarinfo->TGCRFoutRatio);

    pqmDebugAddList(_T("sarInfo.sar(WB)            Normal = %g/%g"), l_sarinfo->tsar[0].sar, l_sarinfo->tsar[0].sar_limit[0]);
    pqmDebugAddList(_T("sarInfo.sar(Head)          Normal = %g/%g"), l_sarinfo->tsar[1].sar, l_sarinfo->tsar[1].sar_limit[0]);
    pqmDebugAddList(_T("sarInfo.sar(Partial)       Normal = %g/%g"), l_sarinfo->tsar[2].sar, l_sarinfo->tsar[2].sar_limit[0]);
    pqmDebugAddList(_T("sarInfo.sar(WB)            1st    = %g/%g"), l_sarinfo->tsar[0].sar, l_sarinfo->tsar[0].sar_limit[1]);
    pqmDebugAddList(_T("sarInfo.sar(Head)          1st    = %g/%g"), l_sarinfo->tsar[1].sar, l_sarinfo->tsar[1].sar_limit[1]);
    pqmDebugAddList(_T("sarInfo.sar(Partial)       1st    = %g/%g"), l_sarinfo->tsar[2].sar, l_sarinfo->tsar[2].sar_limit[1]);
    pqmDebugAddList(_T("sarInfo.preal(WB)          = %g/%g"), l_sarinfo->tsar[0].preal, l_sarinfo->tsar[0].preal_limit);
    pqmDebugAddList(_T("sarInfo.preal(Head)        = %g/%g"), l_sarinfo->tsar[1].preal, l_sarinfo->tsar[1].preal_limit);
    pqmDebugAddList(_T("sarInfo.preal(Partial)     = %g/%g"), l_sarinfo->tsar[2].preal, l_sarinfo->tsar[2].preal_limit);
    pqmDebugAddList(_T("sarInfo.B1rms              = %g"), l_sarinfo->B1rms);
    pqmDebugAddList(_T("sarInfo.coilProtect        = %d"), l_sarinfo->coilProtect);
    pqmDebugAddList(_T("sarInfo.scanEnable         = %d"), l_sarinfo->scanEnable);


    if (SARCTRL_LICENCE_ON == m_pqm_ptr->GetSARControlLicense()) {

        pqmDebugAddList(_T("sarInfo.fixed_ope_mode         = %d"), l_sarinfo->fixed_ope_mode);

        pqmDebugAddList(_T("===================="));

        pqmDebugAddList(_T("sarInfo.sarControl.sarProtect       = %d"), l_sarinfo->sarControl.sarProtect);
        pqmDebugAddList(_T("sarInfo.sarControl.coilProtect      = %d"), l_sarinfo->sarControl.coilProtect);
        pqmDebugAddList(_T("sarInfo.sarControl.rfampProtect     = %d"), l_sarinfo->sarControl.rfampProtect);
        pqmDebugAddList(_T("sarInfo.sarControl.scanEnable       = %d"), l_sarinfo->sarControl.scanEnable);
        pqmDebugAddList(_T("sarInfo.sarControl.waitTime         = %d"), l_sarinfo->sarControl.waitTime);

        float l_sar_limit[3] = {0.0f};

        for (int l_sar_limit_index = 0; l_sar_limit_index < SAR_NUM_TYPE; l_sar_limit_index++) {

            const l_sar_ctrl_10sec_ope_mode = (l_sarinfo->sarControl.ope_mode == IEC_SAR_1ST_MODE || l_sarinfo->sarControl.ope_mode == IEC_SAR_OVER) ? IEC_SAR_1ST_MODE : IEC_SAR_NORMAL_MODE;

            if (l_sarinfo->sarControl.tsarControl[l_sar_limit_index].sarRatio_10sec > 0 && (l_sarinfo->tsar[l_sar_limit_index].sar_limit[1] > 0)) {
                l_sar_limit[l_sar_limit_index] = ((l_sarinfo->tsar[l_sar_limit_index].sar_limit[l_sar_ctrl_10sec_ope_mode]) / (l_sarinfo->tsar[l_sar_limit_index].sar_limit[1])) * SAR_LIMIT_10SEC;
            }
        }

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_6min(WB)          = %g/%d"), l_sarinfo->sarControl.tsarControl[0].sarRatio_6min, SAR_LIMIT_6MIN);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_10sec(WB)         = %g/%g"), l_sarinfo->sarControl.tsarControl[0].sarRatio_10sec, l_sar_limit[0]);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_6min(WB)               = %g"), l_sarinfo->sarControl.tsarControl[0].sar_6min);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_10sec(WB)              = %g"), l_sarinfo->sarControl.tsarControl[0].sar_10sec);
        pqmDebugAddList(_T("sarInfo.sarControl.waitTime(WB)               = %d"), l_sarinfo->sarControl.tsarControl[0].waitTime);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_6min(Head)        = %g/%d"), l_sarinfo->sarControl.tsarControl[1].sarRatio_6min, SAR_LIMIT_6MIN);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_10sec(Head)       = %g/%g"), l_sarinfo->sarControl.tsarControl[1].sarRatio_10sec, l_sar_limit[1]);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_6min(Head)             = %g"), l_sarinfo->sarControl.tsarControl[1].sar_6min);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_10sec(Head)            = %g"), l_sarinfo->sarControl.tsarControl[1].sar_10sec);
        pqmDebugAddList(_T("sarInfo.sarControl.waitTime(Head)             = %d"), l_sarinfo->sarControl.tsarControl[1].waitTime);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_6min(Partial)     = %g/%d"), l_sarinfo->sarControl.tsarControl[2].sarRatio_6min, SAR_LIMIT_6MIN);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_10sec(Partial)    = %g/%g"), l_sarinfo->sarControl.tsarControl[2].sarRatio_10sec, l_sar_limit[2]);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_6min(Partial)          = %g"), l_sarinfo->sarControl.tsarControl[2].sar_6min);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_10sec(Partial)         = %g"), l_sarinfo->sarControl.tsarControl[2].sar_10sec);
        pqmDebugAddList(_T("sarInfo.sarControl.waitTime(Partial)          = %d"), l_sarinfo->sarControl.tsarControl[2].waitTime);

        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_6min(Preal)       = %g"), l_sarinfo->sarControl.tprealControl.prealRatio_6min);
        pqmDebugAddList(_T("sarInfo.sarControl.sarRatio_10sec(Preal)      = %g"), l_sarinfo->sarControl.tprealControl.prealRatio_10sec);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_6min(Preal)            = %g"), l_sarinfo->sarControl.tprealControl.preal_6min);
        pqmDebugAddList(_T("sarInfo.sarControl.sar_10sec(Preal)           = %g"), l_sarinfo->sarControl.tprealControl.preal_10sec);
        pqmDebugAddList(_T("sarInfo.sarControl.waitTime(Preal)            = %d"), l_sarinfo->sarControl.tprealControl.waitTime);

    }

}

//************************************Method Header************************************
// Method Name  : pqmDebugSarMon()
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMDebugInfo::pqmDebugSarMon(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugSarMon");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    pqmDebugAddList(g_dbgsep);
    //Patni-DKH/2010May13/Modified/Phase3#Redmine-210
    SarSetDebug(CPQMDebugInfo::pqmDebugSarAddList);
}

//************************************Method Header************************************
// Method Name  : pqmDebugShimming()
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMDebugInfo::pqmDebugShimming(void)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugShimming");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPqmPmAcqman* l_ptrpqmpmacqman = m_pqm_ptr->GetAcqManager();

    if (NULL == l_ptrpqmpmacqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptrpqmpmacqman  pointer is NULL"));
        return ;
    }

    CScanProcedure* l_ptrscanprocedure = l_ptrpqmpmacqman->GetScanProcedure();

    if (NULL == l_ptrscanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptrscanprocedure pointer is NULL"));
        return ;
    }

    pqmDebugAddList(g_dbgsep);
    PqmShmShimDebug();

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    PqmShim_t	shim;
    PqmPrGetShimmingOffset(l_pqmstudy->GetVfDBHandle(), NULL, SVD_ST_STUDY, &shim);
    //+Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow
    pqmDebugAddList(_T("CFValue  = %g"), shim.fsyValue);
    //-Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow

    for (int i = 0; i < PQM_GRAD_LEN; i++)
        pqmDebugAddList(_T("gradOffset[%d] = %d"), i, shim.gradOffset[i]);

    for (i = 0; i < PQM_SHIM_LEN; i++)
        pqmDebugAddList(_T("shimOffset[%d] = %d"), i, shim.shimOffset[i]);
}

//**************************************************************************
//Method Name   : pqmDebugAddList()
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CPQMDebugInfo::pqmDebugAddList(const CString f_formatstr, ...)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugAddList");

    try {
        TCHAR	l_formattedmsg[2048] = {0};
        va_list	args;
        va_start(args, f_formatstr);
        _vstprintf(l_formattedmsg, f_formatstr, args);
        va_end(args);

        if (m_logFile.hFileNull != m_logFile.m_hFile) {
            m_logFile.SeekToEnd();
            m_logFile.WriteString(l_formattedmsg);
            m_logFile.WriteString(_T("\n"));
            m_logFile.Flush();
        }

        if (m_statuslogfile.hFileNull != m_statuslogfile) {
            CString l_strlogmsg(_T(""));
            CString l_strdatetimestamp = GetDateTimeStamp();
            l_strlogmsg.Format(_T("%s:%s\n"), l_strdatetimestamp, l_formattedmsg);

            //Write the msg in status log file
            m_statuslogfile.WriteString(l_strlogmsg);
            m_statuslogfile.Flush();
        }

        //Send string to UI for Display
        m_pqm_ptr->WriteDebugDataToUI(l_formattedmsg);

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        return ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UnKnown Error"));
        return ;
    }
}

//**************************************************************************
//Method Name   : pqmDebugProtData()
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CPQMDebugInfo::pqmDebugProtData(CPqmProtocol* f_pqmprotocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::pqmDebugProtData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPqmPrMode* l_pqmprmode = f_pqmprotocol->GetPqmPrMode();

    if (NULL == l_pqmprmode) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmPrMode pointer is NULL"));
        return ;
    }

    pqmDebugAddList(_T("------ index = %d position = %d --------->"), f_pqmprotocol->GetProtocol(), f_pqmprotocol->GetPosition());

    int* imgGr = f_pqmprotocol->GetImageGr();

    if (NULL == imgGr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("imgGr pointer is NULL"));
        return;
    }

    pqmDebugAddList(_T("imgGr          = %d,%d"), imgGr[0], imgGr[1]);
    pqmDebugAddList(_T("acqStatus      = %d"), f_pqmprotocol->GetAcqStatus());
    pqmDebugAddList(_T("holdScan       = %X"), f_pqmprotocol->GetHoldScan());
    pqmDebugAddList(_T("gating_method  = %d"), f_pqmprotocol->GetGatingMethod());
    pqmDebugAddList(_T("dynamic        = %d"), f_pqmprotocol->GetDynamic());
    pqmDebugAddList(_T("intermit       = %d (%d)"), f_pqmprotocol->GetIntermitFlag(), f_pqmprotocol->GetIntermitTime());
    pqmDebugAddList(_T("prepScan       = %d"), f_pqmprotocol->GetPrepScan());
    pqmDebugAddList(_T("movingCouch    = %d"), f_pqmprotocol->GetMovingCouch());
    pqmDebugAddList(_T("patori         = (%d,%d)"), l_pqmprmode->GetPatoriEndIn(), l_pqmprmode->GetPatoriSideUp());
    pqmDebugAddList(_T("time           = %d"), f_pqmprotocol->GetScanTime());
    pqmDebugAddList(_T("offset         = (%g,%g,%g)"),
                    f_pqmprotocol->GetOffset().x,
                    f_pqmprotocol->GetOffset().y,
                    f_pqmprotocol->GetOffset().z);
    CString l_tempdata(_T(""));

    if (NULL != f_pqmprotocol->GetScanID()) {
        l_tempdata = f_pqmprotocol->GetScanID() ;
    }

    pqmDebugAddList(_T("scanID         = %s"), l_tempdata);
    l_tempdata = _T("");

    if (NULL != f_pqmprotocol->GetScanComment()) {
        l_tempdata = f_pqmprotocol->GetScanComment();
    }

    pqmDebugAddList(_T("comment        = %s"), l_tempdata);

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        PqmSaturnCoil_t* l_saturn_coil = f_pqmprotocol->GetSaturnCoil() ;

        if (NULL != l_saturn_coil) {
            pqmDebugAddList(_T("NumSectionCoil = %d"), l_saturn_coil->numSection);

            for (int i = 0; i < l_saturn_coil->numSection; i++) {
                pqmDebugAddList(_T("SectionCoil[%d] = P[%c%c],ID[%x],Section[%d],Channel[%c]"), i,
                                l_saturn_coil->RcvCoil[i].portID[0], l_saturn_coil->RcvCoil[i].portID[1],
                                l_saturn_coil->RcvCoil[i].coilID, l_saturn_coil->RcvCoil[i].SectionID,
                                l_saturn_coil->RcvCoil[i].channel);
            }

            for (int j = 0 ; j < l_saturn_coil->numSection; j++) {
                pqmDebugAddList(_T("NumSignal      = %d"), l_saturn_coil->Signal[j].num);

                for (i = 0; i < l_saturn_coil->Signal[j].num; i++) {
                    pqmDebugAddList(_T("Signal[%d]      = no[%d],Hybrid[%d]"), i,
                                    l_saturn_coil->Signal[j].signal_no[i],
                                    l_saturn_coil->Signal[j].hybrid_signal[i]);
                }
            }

            pqmDebugAddList(_T("NumCoilList    = %d"), l_saturn_coil->CoilList.num);

            for (i = 0; i < l_saturn_coil->CoilList.num; i++) {
                pqmDebugAddList(_T("CoilList[%d]    = P[%c%c],ID[%x],coordinate[%d],Channel[%c]"), i,
                                l_saturn_coil->CoilList.coillist[i].portID[0], l_saturn_coil->CoilList.coillist[i].portID[1],
                                l_saturn_coil->CoilList.coillist[i].coilID, l_saturn_coil->CoilList.coillist[i].coordinate,
                                l_saturn_coil->CoilList.coillist[i].channel);

                pqmDebugAddList(_T("CoilList[%d]    = P[%c%c],ID[%x],coordinate in float[%.1f],Channel[%c]"), i,
                                l_saturn_coil->CoilList.coillist[i].portID[0], l_saturn_coil->CoilList.coillist[i].portID[1],
                                l_saturn_coil->CoilList.coillist[i].coilID, l_saturn_coil->CoilListcoordinate.coordinate_list[i].coordinate,
                                l_saturn_coil->CoilList.coillist[i].channel);
            }

            pqmDebugAddList(_T("numSignalDegeneracy= %d"), l_saturn_coil->numSignalDegeneracy);

            for (j = 0 ; j < l_saturn_coil->numSection; j++) {
                pqmDebugAddList(_T("NumSignalDegeneracy      = %d"), l_saturn_coil->SignalDegeneracy[j].num);

                for (i = 0; i < l_saturn_coil->SignalDegeneracy[j].num; i++) {
                    pqmDebugAddList(_T("SignalDegeneracy[%d]      = no[%d],Hybrid[%d]"), i,
                                    l_saturn_coil->SignalDegeneracy[j].signal_no[i],
                                    l_saturn_coil->SignalDegeneracy[j].hybrid_signal[i]);
                }
            }
        }

        //+Patni-HAR/2010Apr12/Added/Phase#3-CDS Requirement
        CpqmCDSInfo* l_cds_info_object = f_pqmprotocol->GetCDSInfo();
        CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();

        if (l_cds_info_object && l_cds_info_object->m_cds_flag) {

            if ((l_cds_info_object->m_cds_step != CDS_STEP4_SCAN) &&
                (l_cds_info_object->m_cds_step != CDS_STEP5_SCANDONE)) {


                if (l_pqm_cds) {
                    l_pqm_cds->PqmCdsSetCoordinatetbl(l_cds_info_object->m_locationtbl,
                                                      l_cds_info_object->m_org_coordinatetbl,
                                                      l_pqmprmode->GetCouchPos());
                }

            }

            pqmDebugAddList(_T("-----------------------------"));
            pqmDebugAddList(_T("cds_step  = %d"), l_cds_info_object->m_cds_step);
            pqmDebugAddList(_T("cds_status= %x"), l_cds_info_object->m_cds_status);

            if (l_pqm_cds) {
                pqmDebugAddList(_T("Center    = %d"),
                                l_pqm_cds->PqmCdsChangeCouch(l_pqmprmode->GetCouchPos()));
            }

            for (int i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {

                if (l_cds_info_object->m_coordinatetbl[i].detectedFlag ||
                    l_cds_info_object->m_locationtbl[i].detectedFlag) {

                    pqmDebugAddList(_T("FlagL = %d     FlagC = %d"),
                                    l_cds_info_object->m_locationtbl[i].detectedFlag,
                                    l_cds_info_object->m_coordinatetbl[i].detectedFlag);
                    pqmDebugAddList(_T("coilID         = %x"), l_cds_info_object->m_coordinatetbl[i].coilID);
                    pqmDebugAddList(_T("Location       = %d"), l_cds_info_object->m_locationtbl[i].location);
                    pqmDebugAddList(_T("Coordinate_org = %d"), l_cds_info_object->m_org_coordinatetbl[i]);
                    pqmDebugAddList(_T("Coordinate     = %d"), l_cds_info_object->m_coordinatetbl[i].location);
                }
            }

            pqmDebugAddList(_T("-----------------------------"));
        }

        //-Patni-HAR/2010Apr12/Added/Phase#3-CDS Requirement
    }


    pqmDebugAddList(_T("homePos        = %g"), l_pqmprmode->GetHomePos());
    pqmDebugAddList(_T("couchPos       = %g"), l_pqmprmode->GetCouchPos());
    pqmDebugAddList(_T("contrast       = %d"), l_pqmprmode->GetContrast());
    pqmDebugAddList(_T("anatomy        = %d"), l_pqmprmode->GetAnatomy());
    pqmDebugAddList(_T("bodypart       = %s"), l_pqmprmode->GetBodyPart());
    pqmDebugAddList(_T("scanNumber     = %d"), l_pqmprmode->GetScanNumber());
    pqmDebugAddList(_T("view_dir       = %d"), l_pqmprmode->GetViewDir());
    pqmDebugAddList(_T("processing     = %d"), f_pqmprotocol->GetProcessing());
    pqmDebugAddList(_T("shimming       = %d"), f_pqmprotocol->GetShimming());
    pqmDebugAddList(_T("fluoro         = %d"), f_pqmprotocol->GetFluoro());
    pqmDebugAddList(_T("hasImage       = %d"), f_pqmprotocol->GetHasImage());
    pqmDebugAddList(_T("hasParent      = %d"), f_pqmprotocol->GetHasParent());
    pqmDebugAddList(_T("locator images = /%d.%d/%d.%d/%d.%d/%d.%d"),
                    f_pqmprotocol->GetLocatorImages(0).subtree_name, f_pqmprotocol->GetLocatorImages(0).index,
                    f_pqmprotocol->GetLocatorImages(1).subtree_name, f_pqmprotocol->GetLocatorImages(1).index,
                    f_pqmprotocol->GetLocatorImages(2).subtree_name, f_pqmprotocol->GetLocatorImages(2).index,
                    f_pqmprotocol->GetLocatorImages(3).subtree_name, f_pqmprotocol->GetLocatorImages(3).index);

    pqmDebugAddList(_T("plan_status    = %d"), f_pqmprotocol->GetPlnStatus());
    pqmDebugAddList(_T("AutoVoice(EXE) = %X"), f_pqmprotocol->GetExeAutoVoice());
    pqmDebugAddList(_T("AutoVoice(PLN) = %X"), f_pqmprotocol->GetPlnAutoVoice());
    pqmDebugAddList(_T("dBdt Mode(Current) = %X"), f_pqmprotocol->GetCurrentdBdtMode());
    pqmDebugAddList(_T("dBdt Mode(1995) = %X"), f_pqmprotocol->GetdBdtMode());
    pqmDebugAddList(_T("dBdt Mode(2002) = %X"), f_pqmprotocol->GetdBdtMode2002());
    pqmDebugAddList(_T("dBdt Mode(Control) = %X"), f_pqmprotocol->GetdBdtControl());
    pqmDebugAddList(_T("Local CF Flag   = %d"), f_pqmprotocol->GetLocalCF());
    pqmDebugAddList(_T("SPEEDER type= %d"), f_pqmprotocol->GetSpeederType());
    pqmDebugAddList(_T("SPEEDER Intensity_type   = %d"), f_pqmprotocol->GetIntensityType());
    pqmDebugAddList(_T("SPEEDER MAP prot   = %d"), f_pqmprotocol->GetSpeederMapProtocol());
    pqmDebugAddList(_T("AMB Enable flag  = %d"), f_pqmprotocol->GetAMBEnableFlag());
    pqmDebugAddList(_T("AMB Mode flag  = %d"), f_pqmprotocol->GetAMBModeFlag());

    //+Patni-HAR/2010Apr12/Added/Phase#3-CDS Requirement
    pqmDebugAddList(_T("couchMethodFlg = %d"), f_pqmprotocol->GetcouchMethodFlg());
    pqmDebugAddList(_T("copySourceProt = %d"), f_pqmprotocol->GetCopySourceProt());
    pqmDebugAddList(_T("rflPos         = %g"), f_pqmprotocol->GetRflPos());
    pqmDebugAddList(_T("cfaPos         = %g"), f_pqmprotocol->GetCfaPos());
    pqmDebugAddList(_T("rflProt        = %d"), f_pqmprotocol->GetRflProt());
    pqmDebugAddList(_T("cfaProt        = %d"), f_pqmprotocol->GetCfaProt());
    pqmDebugAddList(_T("shimProt       = %d"), f_pqmprotocol->GetShimProt());
    pqmDebugAddList(_T("numChannels    = %d"), f_pqmprotocol->GetSystemChannels());
    pqmDebugAddList(_T("Channel Mode   = %d"), f_pqmprotocol->GetcoilChannelMode());
    //-Patni-HAR/2010Apr12/Added/Phase#3-CDS Requirement
}

//**************************************************************************
//Method Name   : PqmPrDebug()
//Author        : PATNI
//Purpose       : displays pr related data
//**************************************************************************
void CPQMDebugInfo::PqmPrDebug()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmPrDebug");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmPrDebug"));

    pqmDebugAddList(_T("            |tag|flag| name        | Tag name"));
    const int prcount = sizeof(pr_init) / sizeof(pr_init[0]) ;

    CString l_prinitstr(_T(""));

    for (int i = 0; i < prcount ; i++) {
        l_prinitstr.Empty();
        l_prinitstr = pr_init[i].str ;

        if (l_prinitstr == _T(""))
            l_prinitstr = _T("(null)");

        pqmDebugAddList(_T("pr_init[%3d]|%3d|%3d |%5d(0x%04X)|%s"),
                        i, pr_init[i].tag, pr_init[i].flag,
                        pr_init[i].name, pr_init[i].name, l_prinitstr);
    }
}

//+Patni-AMT/2010Jan11/Added/DeFT# IR-86 Debug Window
//**************************************************************************
//Method Name   : PqmCoilDebug
//Author        : Patni/AMT
//Purpose       : Displays installed coil information on debug window.
//**************************************************************************
void CPQMDebugInfo::PqmCoilDebug()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmCoilDebug");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmCoilDebug"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CDataManager* l_data_mgr = m_pqm_ptr->GetDataManager();

    if (NULL == l_data_mgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_data_mgr pointer is NULL"));
        return ;
    }

    CPqmCoilData* l_coil_data = l_data_mgr->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_coil_data pointer is NULL"));
        return ;
    }

    pqmDebugAddList(_T("---  COIL LIST  ---"));
    pqmDebugAddList(_T("P C    ID    DSP   mask   ch  #MVIEW  Comb MaxCh MBF SDB Sname LABEL"));

    DllT*          l_dll = NULL;
    PqmCoil_t*     l_coil = NULL;

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        for (l_dll = l_coil_data->GetHead(); l_dll != NULL; l_dll = l_dll->next) {
            l_coil = (PqmCoil_t*)l_dll->item;

            if (NULL == l_coil) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("l_coil pointer is NULL-Skipping record"));
                continue ;
            }

            CString l_strShortname = l_coil->shortName;
            CString l_strlabel(_T(""));

            if (NULL != l_coil->label) {
                l_strlabel = l_coil->label ;
            }

            pqmDebugAddList(_T("%c %c %08X %3s %08X %2d %08X %04X   %2d   %d   %d   %s  %s"),
                            l_coil->port, l_coil->channel, l_coil->coil_id,
                            l_coil->enable ? _T("ON ") : _T("OFF"),
                            l_coil->channel_mask,
                            l_coil->num_channels,
                            l_coil->mview,
                            l_coil->combine,
                            l_coil->maxChannel,
                            l_coil->movingBed,
                            l_coil->sectionDB,
                            l_strShortname,
                            l_strlabel);
        }

    } else {

        CString l_strlabel(_T(""));

        for (l_dll = l_coil_data->GetHead(); l_dll != NULL; l_dll = l_dll->next) {

            l_strlabel.Empty();

            if (NULL != l_coil->label) {
                l_strlabel = l_coil->label ;
            }

            pqmDebugAddList(_T("%c %c %08X %3s %08X %2d %08X  %s "),
                            l_coil->port, l_coil->channel, l_coil->coil_id,
                            l_coil->enable ? _T("ON ") : _T("OFF"),
                            l_coil->channel_mask,
                            l_coil->num_channels,
                            l_coil->mview,
                            l_strlabel);
            pqmDebugAddList(_T("     --> %g %g %g %g %g %g %g %g"),
                            l_coil->gain_data[0],
                            l_coil->gain_data[1],
                            l_coil->gain_data[2],
                            l_coil->gain_data[3],
                            l_coil->gain_data[4],
                            l_coil->gain_data[5],
                            l_coil->gain_data[6],
                            l_coil->gain_data[7]);
        }
    }
}
//-Patni-AMT/2010Jan11/Added/DeFT# IR-86 Debug Window

//**************************************************************************
//Method Name   : PqmPqDebug()
//Author        : PATNI/RAJ
//Purpose       :
//**************************************************************************
void CPQMDebugInfo::PqmPqDebug()
{
    //Data Not Found
    /*PQM_TRACE( USER_FUNC_MARKER, _T("CPQMDebugInfo::PqmPqDebug"), _T("Trace"));

       pqmDebugAddList(_T("processing = %c%c%c%c%c",
               PqmPqGlobal.processing & PQM_PQ_SELECT ? 'S' : '-',
               PqmPqGlobal.processing & PQM_PQ_MOVING ? 'M' : '-',
               PqmPqGlobal.processing & PQM_PQ_INSERT ? 'I' : '-',
               PqmPqGlobal.processing & PQM_PQ_BREAK  ? 'B' : '-',
               PqmPqGlobal.processing & PQM_PQ_FILLER ? 'F' : '-');
    pqmDebugAddList(_T("SELECT (pos = %d status = %d)",
               PqmPqGlobal.select.pos, PqmPqGlobal.select.status);
    pqmDebugAddList(_T("MOVE   (pos = %d status = %d)",
               PqmPqGlobal.move.pos, PqmPqGlobal.move.status);*/

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmPqDebug");

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqm_ptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    int l_wait_pos = 0;
    CPqmProtocol* l_top_most_wait = NULL;
    m_pqm_ptr->GetStudy()->GetTopWaitPos(&l_top_most_wait);

    if (l_top_most_wait != NULL) {
        l_wait_pos = l_top_most_wait->GetPosition();
    }

    //Patni-DKH/2010Mar02/Added/PH#3#/IR86/DebugWindow WSE opening issue
    const int l_all_count = l_pqmstudy->GetProtCountFromVec();

    //+Patni-ARD/2010Mar31/Modified/Ph-3# IR-100 code review
    char    l_cur_sp_path[256] = {0};
    CPqmUtilities::GetCurrScatchpadFileName(l_cur_sp_path);
    //+Patni-Ravindra Acharya/2010May07/Modified/Redmine184
    CString l_current_file(l_cur_sp_path);
    //l_current_file.Format(_T("%s"), l_cur_sp_path);
    //-Patni-Ravindra Acharya/2010May07/Modified/Redmine184
    //-Patni-ARD/2010Mar31/Modified/Ph-3# IR-100 code review

    pqmDebugAddList(_T("wait_pos = %d"), l_wait_pos);
    pqmDebugAddList(_T("last_pos = %d"), l_all_count);
    pqmDebugAddList(_T("<%s>"), l_current_file);
}


//**************************************************************************
//Method Name   : PqmSarDebugRfl()
//Author        : PATNI/RAJ
//Purpose       : To display sar rfl related data.
//**************************************************************************
void CPQMDebugInfo::PqmSarDebugRfl()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmSarDebugRfl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmSarDebugRfl"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    CPQMSAR* l_pqmSARPtr = m_pqm_ptr->GetSAR();

    if (NULL == l_pqmSARPtr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMSAR pointer is NULL"));
        return;
    }

    pqmDebugAddList(_T("rflParams.rflTxGain       = %f"),  l_pqmSARPtr->GetRflParams().rflTxGain);
    pqmDebugAddList(_T("rflParams.rflTxGainLimit  = %f"),  l_pqmSARPtr->GetRflParams().rflTxGainLimit);
    pqmDebugAddList(_T("rflParams.rflScaler       = %f"),  l_pqmSARPtr->GetRflParams().rflScaler);
    pqmDebugAddList(_T("rflParams.rflWaveAngle    = %d"),  l_pqmSARPtr->GetRflParams().rflWaveAngle);
    pqmDebugAddList(_T("rflParams.rflWaveSum      = %f"),  l_pqmSARPtr->GetRflParams().rflWaveSum);
    pqmDebugAddList(_T("rflParams.rflWavePitch    = %f"),  l_pqmSARPtr->GetRflParams().rflWavePitch);
    pqmDebugAddList(_T("rflParams.rfampModeMax    = %f"),  l_pqmSARPtr->GetRflParams().rfampModeMax);

}

//**************************************************************************
//Method Name   : PqmShmShimDebug()
//Author        : PATNI/RAJ
//Purpose       : To display shim related data
//**************************************************************************

int CPQMDebugInfo::PqmShmShimDebug()
{

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmShmShimDebug");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmShmShimDebug"));

    CPqmShimSharedData* l_pqmshimshareddata = CPqmShimSharedData::GetInstance();

    if (NULL != l_pqmshimshareddata) {

        VfPathElem_t	l_hrshimpath[SVD_ST_PROCESSED] = {0};
        l_pqmshimshareddata->GetHRShimPath(l_hrshimpath);
        VfPathElem_t	l_activeshimpath[SVD_ST_PROCESSED] = {0};//Patni-RAJ/2010Apr16/Added/PH#3# IR86_CDR_11
        l_pqmshimshareddata->GetShimPath(l_activeshimpath);//Patni-RAJ/2010Apr16/Added/PH#3# IR86_CDR_11

        int l_shimstate = 0;
        int l_shimfsyflag = -1;
        l_pqmshimshareddata->GetShimState(&l_shimstate);
        l_pqmshimshareddata->GetShimFsyFlag(&l_shimfsyflag);
        pqmDebugAddList(_T("shimState        = %d"), l_shimstate);
        pqmDebugAddList(_T("shimFsyFlag      = %d"), l_shimfsyflag);

        //+Patni-RAJ/2010Apr16/Added/PH#3# IR86_CDR_11
        pqmDebugAddList(_T("activeShimPath   = /%d.%d/%d.%d"),
                        l_activeshimpath[0].subtree_name,
                        l_activeshimpath[0].index,
                        l_activeshimpath[1].subtree_name,
                        l_activeshimpath[1].index);
        //-Patni-RAJ/2010Apr16/Added/PH#3# IR86_CDR_11

        pqmDebugAddList(_T("activeHRShimPath = /%d.%d/%d.%d"),
                        l_hrshimpath[0].subtree_name,
                        l_hrshimpath[0].index,
                        l_hrshimpath[1].subtree_name,
                        l_hrshimpath[1].index);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmshimshareddata pointer is NULL"));
    }

    return 0;
}

//**************************************************************************
//Method Name   : PqmShmDebug()
//Author        : PATNI/RAJ
//Purpose       : to display Shim related data
//**************************************************************************
int CPQMDebugInfo::PqmShmDebug()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmShmDebug");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmShmDebug"));

    //+Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    float l_cfa_position	= 0.0f;
    float l_shim_position	= 0.0f;
    //-Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187

    CPqmShimSharedData* l_pqmshimshareddata = CPqmShimSharedData::GetInstance();

    //+Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    l_pqmshimshareddata->GetCfaPos(&l_cfa_position);
    l_pqmshimshareddata->GetShimPos(&l_shim_position);
    //-Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187

    if (NULL != l_pqmshimshareddata) {
        pqmDebugAddList(_T("---SCHEDULE---"));

        pqmDebugAddList(_T("shm.study       = Run%d.%d"), l_pqmshimshareddata->GetScheduleActive().run_num,
                        l_pqmshimshareddata->GetScheduleActive().site_id);
        pqmDebugAddList("active          = %d",  l_pqmshimshareddata->GetScheduleActive());

        pqmDebugAddList(_T("---IMAGING----"));

        pqmDebugAddList(_T("shm.study       = Run%d.%d"), l_pqmshimshareddata->GetImagingActive().run_num,
                        l_pqmshimshareddata->GetImagingActive().site_id);
        pqmDebugAddList(_T("active          = %d"), l_pqmshimshareddata->GetImagingActive());
        pqmDebugAddList(_T("protocol        = %d"), l_pqmshimshareddata->GetCurrentProtocolNumber());
        pqmDebugAddList(_T("shm.scanNumner  = %d"), l_pqmshimshareddata->GetScanNumber());
        pqmDebugAddList(_T("shm.couchPos    = %f"), l_pqmshimshareddata->GetCouchPos());
        pqmDebugAddList(_T("shm.homePos     = %f"), l_pqmshimshareddata->GetHomePos());
        pqmDebugAddList(_T("shm.txtunePos   = %f"), l_pqmshimshareddata->Gettxtunepos());
        pqmDebugAddList(_T("shm.rflPos      = %f"), l_pqmshimshareddata->Getrflpos());
        //+Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
        pqmDebugAddList(_T("shm.cfaPos      = %f"), l_cfa_position);
        pqmDebugAddList(_T("shm.shimPos     = %f"), l_shim_position);
        //-Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    }

    return 0;
}

//**************************************************************************
//Method Name   : PqmDebugStatusLogClose
//Author        : PATNI\DKH
//Purpose       : To close status log file
//**************************************************************************
void CPQMDebugInfo::PqmDebugStatusLogClose()
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmDebugStatusLogClose");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              , _T("Trace of PqmDebugStatusLogClose"));

    try {
        if (m_statuslogfile.hFileNull != m_statuslogfile.m_hFile) {
            m_statuslogfile.Close();
        }

    } catch (CFileException* e) {
        TCHAR l_excp[g_expmsgsize] = {0};
        e->GetErrorMessage(l_excp, g_expmsgsize);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_excp);
        return ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Unhandled exception"));
        return;
    }
}

//+Patni-DKH/2010May13/Modified/Phase3#Redmine-210
//**************************************************************************
//Method Name   : pqmDebugSarAddList
//Author        : PATNI/DKH
//Purpose       : To display debug data during sar calculation
//**************************************************************************
void CPQMDebugInfo::pqmDebugSarAddList(char* f_formatstr, ...)
{
    char	l_formattedmsg[2048] = {0};
    va_list	args;

    va_start(args, f_formatstr);
    vsprintf(l_formattedmsg, f_formatstr, args);
    va_end(args);

    CPQMDebugInfo* l_pqmdebuginfo = CPQMDebugInfo::GetInstance(m_pqm_ptr);
    CString l_strformattedmsg(l_formattedmsg);

    if (NULL != l_pqmdebuginfo)
        l_pqmdebuginfo->pqmDebugAddList(l_strformattedmsg);
}
//-Patni-DKH/2010May13/Modified/Phase3#Redmine-210

//**************************************************************************
//Method Name   : PqmPrGetShimmingOffset()
//Author        : PATNI/RAJ
//Purpose       : displays PqmPrGetShimmingOffset related data
//**************************************************************************
int CPQMDebugInfo::PqmPrGetShimmingOffset(CVarFieldHandle* const vf_handle,
        VfPathElem_t*	path,
        int32_t			depth,
        PqmShim_t*		shim)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::PqmPrGetShimmingOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmPrGetShimmingOffset"));

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return E_ERROR;
    }

    VfFieldSpec_t	vfReq[4] = {0};

    DB_FUNC_ENTER("[PqmPrGetShimmingOffset]:");

    VFF_SET_ARGS(vfReq[0], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int32_t) * PQM_SHIM_LEN, shim->shimOffset,
                 VFO_VARIABLE_LEN);
    VFF_SET_ARGS(vfReq[1], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int32_t) * PQM_GRAD_LEN, shim->gradOffset,
                 VFO_VARIABLE_LEN);
    VFF_SET_ARGS(vfReq[2], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(flt64_t), &shim->fsyValue, 0);

    int			num = 0;
    status_t status = vf_handle->GetFields(path, depth, vfReq, 3, &num);

    if (status != E_NO_ERROR ||
        num != 3 ||
        vfReq[0].status != E_NO_ERROR ||
        vfReq[1].status != E_NO_ERROR ||
        vfReq[2].status != E_NO_ERROR) {
        DB_PRINTF("", ("Get Data Error. (%d/3) [%s,%s,%s]\n",
                       num,
                       VfFieldErrorStr(vfReq[0].status, NULL, NULL),
                       VfFieldErrorStr(vfReq[1].status, NULL, NULL),
                       VfFieldErrorStr(vfReq[2].status, NULL, NULL)));
        bzero(shim, sizeof(PqmShim_t));
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//************************************Method Header****************************
// Method Name  : GetStringForPrescanKind()
// Author       : PATNI \ HAR
// Purpose      :
//*****************************************************************************
CString CPQMDebugInfo::GetStringForPrescanKind(const CPSAcqSetFlag::TAG /*int*/ kind)
{

    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::GetStringForPrescanKind");

    CString l_return_string(_T("")), l_log_string(_T(""));


    switch (kind) {

        case K_PR_PRESCAN_FSY:
            l_log_string = CString(_T("ACQ_PRESCAN_CFA"));
            l_return_string = _T("cfa");
            break;

        case ACQ_PRESCAN_TXCOIL:
            l_log_string = CString(_T("ACQ_PRESCAN_TXCOIL"));
            l_return_string = _T("txCoil");
            break;

        case ACQ_PRESCAN_RXCOIL:
            l_log_string = CString(_T("ACQ_PRESCAN_RXCOIL"));
            l_return_string = _T("rxCoil");
            break;

        case K_PR_PRESCAN_RFL:
            l_log_string = CString(_T("ACQ_PRESCAN_RFLEVEL"));
            l_return_string = _T("rfl");
            break;

        case K_PR_PRESCAN_RGN:
            l_log_string = CString(_T("ACQ_PRESCAN_RXGAIN"));
            l_return_string = _T("rxGain");
            break;

        case ACQ_PRESCAN_CDS:
            l_log_string = CString(_T("ACQ_PRESCAN_CDS"));
            l_return_string = _T("");
            break;

        default:
            l_return_string = _T("");
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);

    return l_return_string;
}

//************************************Method Header****************************
// Method Name  : GetStringForPrescanAction()
// Author       : PATNI \ HAR
// Purpose      :
//*****************************************************************************
CString CPQMDebugInfo::GetStringForPrescanAction(const int action)
{
    LPCTSTR FUNC_NAME = _T("CPQMDebugInfo::GetStringForPrescanAction");

    CString l_return_string(_T("")), l_log_string(_T(""));

    switch (action) {

        case VFC_PRESCAN_CLINICAL:
            l_log_string = CString(_T("PQM_VFC_PRESCAN_CLINICAL"));
            l_return_string = _T("USE_CLINICAL");
            break;

        case VFC_PRESCAN_USE_PREVIOUS:
            l_log_string = CString(_T("PQM_VFC_PRESCAN_USE_PREVIOUS"));
            l_return_string = _T("USE_PREVIOUS");
            break;

        case VFC_PRESCAN_USE_VARFIELD:
            l_log_string = CString(_T("PQM_VFC_PRESCAN_USE_VARFIELD"));
            l_return_string = _T("USE_VARFIELD");
            break;

        case VFC_PRESCAN_LOAD_VARFIELD:
            l_log_string = CString(_T("PQM_VFC_PRESCAN_LOAD_VARFIELD"));
            l_return_string = _T("LOAD_VARFIELD");
            break;

        default:
            l_return_string = _T("");
    }


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);

    return l_return_string;
}
//-Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187

//**************************************************************************
//Method Name   : GetDateTimeStamp
//Author        : PATNI\DKH
//Purpose       : To get date time stamp
//**************************************************************************
const CString CPQMDebugInfo::GetDateTimeStamp(const bool f_bhourminformat)
{
    TCHAR l_ctime[128] = {0};
    time_t  f_time = 0L, s_time = 0L;

    f_time = time(&s_time);
    struct tm*  ptms = localtime((time_t*)(&f_time));

    if (!f_bhourminformat) {
        //+Patni-Ravindra Acharya/2010May07/Modified/Redmine184
        _tcsftime(l_ctime, sizeof(l_ctime), _T("%Y%m%d%H%M%S"), ptms);
        //+Patni-Ravindra Acharya/2010May07/Modified/Redmine184

    } else {
        _tcsftime(l_ctime, sizeof(l_ctime), _T("%H%M"), ptms);
    }

    CString l_cstemp(l_ctime);

    return l_cstemp;
}

//**************************************************************************
//Method Name   : PqmCdsDebug()
//Author        : PATNI/RAJ
//Purpose       :
//**************************************************************************

// TODO: To be implemented after coding of CDS

/*

 void CPQMDebugInfo::PqmCdsDebug()
{
    pqmDebugAddList(_T("------------CDS Param---------------");
    pqmDebugAddList(_T("moveCouchLimit      = %d", moveCouchLimit);
    pqmDebugAddList(_T("couchToProjDis      = %d", couchToProjDis);
    pqmDebugAddList(_T("couchLabelSize      = %d", couchLabelSize);
    pqmDebugAddList(_T("couchEndMargin      = %d", couchEndMargin);
    pqmDebugAddList(_T("couchRangeIn        = %d", couchRangeIn);
    pqmDebugAddList(_T("couchRangeOut       = %d", couchRangeOut);
    pqmDebugAddList(_T("coilOverLapLimit    = %d", coilOverLapLimit);
    pqmDebugAddList(_T("autoIn              = %d", autoIn);
    pqmDebugAddList(_T("-------------------------------------");
}
*/


