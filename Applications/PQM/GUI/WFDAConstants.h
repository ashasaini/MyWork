// WFDAConstants.h: interface for the CWFDAConstants class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_WFDACONSTANTS_H__
#define __SM_WFDACONSTANTS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define INSCAN_REPLY_FAIL	_T("E_FAIL")
#define INSCAN_REPLY_OK		_T("S_OK")
#define DATABASE_NAME		_T("local")

#define PARAMETER_SEPERATOR		_T("\\")

#define WDFA_WSE_EDITING_CANCEL	_T("Cancel")
#define WDFA_WSE_EDITING_SAVE	_T("OK")
#define WDFA_WSE_EDITING_HIDE	_T("Hide")

//Patni-PJS/2010Oct26/Added/WFDA/IR-97
#define WDFA_WSE_SHOW_SCAN_PREF_DLG _T("Open")
#define WDFA_WSE_SHOW_SCAN_BLOCK_DLG _T("Show")
#define WFDA_INSCAN_SERVER_CHANNEL_NAME				_T("inscanserver")
#define PQM_WFDA_FOR_PARAM_SETREADY_REPL _T("PQM_WFDA_FOR_PARAM_SETREADY_REPLY")

enum WFDAMessageEnum {

    WFDA_Invalid,

    WFDA_AddInScanProtocol_FW,	//FW_AddInScanProtocol
    WFDA_AddInScanProtocol_RE,	//RE_FW_AddInScanProtocol

    WFDA_Delete_FW,			//FW_DeleteInScanSequence
    WFDA_Delete_RE,			//RE_FW_DeleteInScanSequence

    WFDA_Duplicate_FW,		//FW_DuplicateInScanSequence
    WFDA_Duplicate_RE,		//RE_FW_DuplicateInScanSequence

    WFDA_ScanStart_FW,		//FW_InScanStart
    WFDA_ScanStart_RE,		//RE_FW_InScanStart

    WFDA_ScanDone_FW,		//FW_InScanDone
    WFDA_ScanDone_RE,		//RE_FW_InScanDone

    WFDA_ScanAbort_FW,		//FW_InScanAbort
    WFDA_ScanAbort_RE,		//RE_FW_InScanAbort

    WFDA_ShowInScanPreferenceDialog_FW,		//FW_ShowInScanPreferenceDialog
    WFDA_ShowInScanPreferenceDialog_RE,		//RE_FW_ShowInScanPreferenceDialog

    WFDA_RequestClosePreferenceDialog_FW,	//FW_RequestClosePreferenceDialog
    WFDA_RequestClosePreferenceDialog_RE,	//RE_FW_RequestClosePreferenceDialog

    WFDA_HideInScanPreferenceDialogOnPageHide,	//FW_RequestClosePreferenceDialog
    WFDA_ShowInScanPreferenceDialogOnPageShow,	//RE_FW_RequestClosePreferenceDialog

    WFDA_SwitchInscan_FW,
    WFDA_SwitchInscan_RE
} ;

enum PQMEventForWFDA {

    PQMEvent_PQMInvoked,	//invoking of PQM
    PQMEvent_PQMClosed,		//PQM is hidden - study closed
    PQMEvent_PQMDestroyed	//PQM is closed completely
};


struct WFDAMessageData {

    WFDAMessageEnum m_wfdamessage ;
    CString m_parameter;
    CString m_custom;

    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    WFDAMessageData() {
        m_wfdamessage = WFDA_Invalid;
        m_parameter = _T("");
        m_custom = _T("");
    }
    //-Patni-RUP/2009Aug24/Modified/cpp test corrections
};


enum WFDA_Protocol_Status {

    WFDA_Protocol_Status_Normal,

    WFDA_Protocol_ScanStart,
    WFDA_Protocol_ScanAbort,
    WFDA_Protocol_ScanDone,

    WFDA_Protocol_WSEEditing_Start,
    WFDA_Protocol_WSEEditing_Cancel,
    WFDA_Protocol_WSEEditing_Save,
    WFDA_Protocol_WSEEditing_Hide,
    WFDA_Protocol_BlockSetting_Show,
    WFDA_Protocol_Inscan_Switch,

};


#endif // #ifndef __SM_WFDACONSTANTS_H__
