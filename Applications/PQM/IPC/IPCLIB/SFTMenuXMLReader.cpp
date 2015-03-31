//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
// SFTMenuXMLReader.cpp: implementation of the CSFTMenuXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTMenuXMLReader.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/Resource.h>
#include <PQM/PqmUtilities.h>
#include <PDataParser/XMLTag.h>
#include <PDataParser/MSXML4DomParser.h>
#include <PQM/SFTMenuConstants.h>

//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSFTMenuXMLReader::CSFTMenuXMLReader()
{

}

CSFTMenuXMLReader::~CSFTMenuXMLReader()
{

}


BOOL CSFTMenuXMLReader::LoadXMLFile(
    CPTString filename
)
{
    LPCTSTR FUNC_NAME = _T("CSFTMenuXMLReader::LoadXMLFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CSFTMenuXMLReader::LoadXMLFile"));


    struct _stat statusstruct;
    int filestatus = _wstat(filename, &statusstruct);

    if (filestatus != 0) {

        CString string = L"";
        string.LoadString(IDS_ERR_XMLFILE_LOADING_FAILED);

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_XMLFILE_LOADING_FAILED")),
            _T("l_RemoteConsole.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);


        return FALSE;
    }

    CMSXML4DomParser* domparser = new CMSXML4DomParser();

    try {
        domparser->LoadFromFile(filename);

    }   catch (...) {


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_VALUES_IN_XML_FILE")),
            _T("CSFTMenuXMLReader.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);

        //Patni-AJS+Ajay/2010Aug23/Commented/IR-152_Bug3
        //throw;

        return FALSE;
    }

    LoadPopupMenu((CXMLTag*) domparser->GetRoot(), -1, &m_main_popup);

    return TRUE;
}

void CSFTMenuXMLReader::GetSFTMenuXMLData(
    VARIANT** pData
)
{
    if (pData && *pData) {
        (*pData)->byref = &m_main_popup ;
    }
}


void CSFTMenuXMLReader::LoadPopupMenu(
    CXMLTag* root,
    const int& child_num,
    PopupMenu* l_popup
)const
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(child_num);

    //Now iterating popupmenu here...
    const int child_count = root->GetChildPSCount();

    if (child_count <= 0) {
        return ;
    }

    CPTString l_menu_item = g_menu_item_name;
    CPTString l_popup_item = g_popup_item;

    //Patni-Hemant/2010July23/Modified/IR-152-Part 3
    const CString l_xml_comment = g_xml_comment;

    CXMLTag* child;
    CPTString attribname, attribvalue, child_name ;

    for (int i = 0 ; i < child_count ; i++) {

        child = NULL;
        // get the child tag
        child = (CXMLTag*) root->GetChildPS(i);

        //+Patni-Hemant/2010July23/Modified/IR-152-Part 3
        if (child) {

            CXMLTag::NV_POS pos = child->GetFirstAttribute(attribname, attribvalue);             // get the first attribute
            child_name = child->GetName();

            if (child_name == l_xml_comment) {
                continue ;
            }

            PopupMenu* l_menu = new PopupMenu;

            if (child_name == l_menu_item) {

                //Its only a menu item.. its not popupmenu
                LoadMenuItem(child, l_menu);

            } else if (child_name == l_popup_item) {

                l_menu->m_popup_menu_string = attribvalue ;
                LoadPopupMenu(child, i, l_menu);

            } else {

                l_menu->m_popup_menu_string = attribvalue ;
                LoadPopupMenu(child, i, l_menu);
            }

            l_popup->m_menu_list.AddTail(l_menu);
        }

        //-Patni-Hemant/2010July23/Modified/IR-152-Part 3
    }
}


void CSFTMenuXMLReader::FillMenuItem(
    MenuData* f_menu_item,
    CPTString f_param_name,
    CPTString f_param_val
)const
{
    CString l_val = f_param_val;

    CPTString l_menu_item_name = g_menu_item_name;
    CPTString l_cmd = g_cmd;
    CPTString l_param = g_param;
    CPTString l_sync = g_sync;
    CPTString l_RemoteConsole = g_RemoteConsole;
    CPTString l_interact = g_interact;  //Patni-RAJ/2010AUG23/Added/V1.30/IR-152-Part4

    if (f_param_name == l_menu_item_name) {
        f_menu_item->m_menu_item_name = l_val;

    } else if (f_param_name == l_cmd) {
        f_menu_item->m_cmd = l_val;

    } else if (f_param_name == l_param) {
        f_menu_item->m_param = l_val;

    } else if (f_param_name == l_sync) {

        if ((l_val.CompareNoCase(_T("0")) == 0) || (l_val.CompareNoCase(_T("1")) == 0)) {
            f_menu_item->m_sync = l_val;

        } else {
            f_menu_item->m_sync = _T("0");
        }

    } else if (f_param_name == l_RemoteConsole) {
        if ((l_val.CompareNoCase(_T("0")) == 0) || (l_val.CompareNoCase(_T("1")) == 0)) {
            f_menu_item->RemoteConsole = l_val;

        } else {
            f_menu_item->RemoteConsole = _T("1");
        }

        //+Patni-RAJ/2010AUG23/Added/V1.30/IR-152-Part4

    } else if (f_param_name == l_interact) {
        if ((l_val.CompareNoCase(_T("0")) == 0) || (l_val.CompareNoCase(_T("1")) == 0)) {
            f_menu_item->m_interact = l_val;

        } else {
            f_menu_item->m_interact = _T("0");
        }

        //-Patni-RAJ/2010AUG23/Added/V1.30/IR-152-Part4

    } else {
        ASSERT(FALSE);
    }
}


BOOL CSFTMenuXMLReader::LoadMenuItem(
    CXMLTag* child,
    PopupMenu* l_popup
)const
{

    if (!l_popup || !child) {
        return FALSE;
    }

    CPTString chil_name, sub_child_name;

    CPTString attribvalue;
    CPTString attribname = child->GetName();

    CPTString l_menu_item = g_menu_item_name;

    if (attribname == l_menu_item) {

        //Read the menu item struct here...
        MenuData* l_menu_item = new MenuData;

        chil_name = child->GetName();
        CXMLTag::NV_POS pos = child->GetFirstAttribute(attribname, attribvalue);             // get the first attribute
        FillMenuItem(l_menu_item, chil_name, attribvalue);

        const int child_count = child->GetChildPSCount();

        for (int i = 0 ; i < child_count ; i++) {
            // get the child tag
            CXMLTag* sub_child = (CXMLTag*) child->GetChildPS(i);
            sub_child_name = sub_child->GetName();

            if (child) {
                attribname = _T("");
                attribvalue = _T("");
                CXMLTag::NV_POS pos = sub_child->GetFirstAttribute(attribname, attribvalue);             // get the first attribute
                CXMLTag::NV_POS validate = NULL;

                FillMenuItem(l_menu_item, sub_child_name, attribvalue);

                while (pos != validate) {

                    // set the tag information
                    //SetTagInformation(nodecnt, tag, *child, attribname, attribvalue);

                    // get the next attribute
                    pos = sub_child->GetNextAttribute(pos, attribname, attribvalue);
                }
            }
        }

        l_popup->m_menu_item = l_menu_item ;

        return TRUE ;
    }

    return FALSE;
}


