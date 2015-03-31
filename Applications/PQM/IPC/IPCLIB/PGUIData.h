// PGUIData.h: interface for the CPGUIData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_DEFAULTGUIDXMLDATA_H
#define __SM_IPCLIB_DEFAULTGUIDXMLDATA_H

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TagDataInformation {
    CString m_inscan_type;
    CString m_pguid_value;
    TagDataInformation();
    ~TagDataInformation();
};

struct InscanCondition {
    CString m_inscan_tag_name;
    CString m_inscan_value;
    InscanCondition();
    ~InscanCondition();
};

struct DefaultPGUIDXMLData {
    struct TagDataInformation* m_tagdata_info;
    CList<InscanCondition*, InscanCondition*> m_inscan_cond_data_list;
    DefaultPGUIDXMLData();
    ~DefaultPGUIDXMLData();
};

struct  PGUIData {
    CList<DefaultPGUIDXMLData*, DefaultPGUIDXMLData*> m_pguidata;
    PGUIData();
    ~PGUIData();
};

#endif // #ifndef __SM_IPCLIB_DEFAULTGUIDXMLDATA_H