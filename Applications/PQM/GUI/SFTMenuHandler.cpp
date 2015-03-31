//TMSC-REDMINE-1671
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777

// SFTMenuHandler.cpp: implementation of the CSFTMenuHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTMenuHandler.h"
#include <PQM/SFTMenuConstants.h>
#include "PQMView.h"
#include "PQMListCtrl.h"

using namespace MR::ACQ::SM::COMMON;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************Method Header************************************
// Method Name  : PQMConfigMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PQMConfigMenu::PQMConfigMenu(
    CSFTMenuInterface* f_sftmenuinterface
):	m_config_popupmenuitem_list(),
    m_pqmconfigmenu_map(),
    m_pqm_confg_menu(NULL),
    m_sftmenuinterface(f_sftmenuinterface)
{

}


//************************************Method Header************************************
// Method Name  : ~PQMConfigMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PQMConfigMenu::~PQMConfigMenu()
{
    DetachMenu();
}


//************************************Method Header************************************
// Method Name  : DetachMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void PQMConfigMenu::DetachMenu(
)
{
    UTIL::DeleteAndClearMFCList<PopupMenuEx>(&m_config_popupmenuitem_list);
}


//************************************Method Header************************************
// Method Name  : SetPqmConfigMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void PQMConfigMenu::SetPqmConfigMenu(
    PopupMenu* pqm_confg_menu
)
{
    m_pqm_confg_menu = pqm_confg_menu;
}


//************************************Method Header************************************
// Method Name  : GetPqmConfigMenuMap
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const MenuData* PQMConfigMenu::GetPqmConfigMenuMap(
    const int f_index
)
{
    return static_cast<MenuData*>(m_pqmconfigmenu_map[f_index]);
}


//************************************Method Header************************************
// Method Name  : CreateSFTToolMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void PQMConfigMenu::CreateSFTToolMenu(
    const UINT f_menu_id,
    const CString& f_menu_str,
    PopupMenuEx* f_main_menu
)
{

    LPCTSTR FUNC_NAME = _T("PQMConfigMenu::CreateSFTToolMenu");

    if (f_main_menu) {

        PopupMenuEx* l_sft_context_menu = new PopupMenuEx;
        m_config_popupmenuitem_list.AddTail(l_sft_context_menu);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Inserting SFT Menu Items..."));

        int l_start_menu = f_menu_id;

        if (CreateSFTToolMenu(m_pqm_confg_menu, l_sft_context_menu , this, l_start_menu)) {
            f_main_menu->InsertMenu(-1, MF_POPUP, (UINT) l_sft_context_menu->m_hMenu, f_menu_str) ;

        } else {
            DetachMenu();
        }
    }
}


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : CreateSFTMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool PQMConfigMenu::CreateSFTToolMenu(
    PopupMenu* f_popup_menu,
    PopupMenuEx* f_sftmenu,
    PQMConfigMenu* f_config_menu,
    int& f_menu_command_start
) const
{
    LPCTSTR FUNC_NAME = _T("PQMConfigMenu::CreateSFTToolMenu");

    //+Code Review
    if ((NULL == f_popup_menu) || (NULL == f_sftmenu)) {
        return false;
    }

    //-Code Review

    POSITION pos = f_popup_menu->m_menu_list.GetHeadPosition();

    if (NULL == pos)
        return false;

    f_sftmenu->CreatePopupMenu();

    CString l_log_str = _T("");
    PopupMenu* l_next_menu = NULL;
    int l_command = f_menu_command_start ;

    while (pos) {
        l_next_menu = f_popup_menu->m_menu_list.GetNext(pos);

        if (l_next_menu->m_menu_item) {
            //its a menu item..
            l_command = GetCommandForMenuItem(l_next_menu->m_menu_item, f_menu_command_start);

            if (l_command != -1) {
                f_sftmenu->InsertMenu(-1, MF_BYPOSITION, l_command, l_next_menu->m_menu_item->m_menu_item_name) ;

                l_log_str.Format(_T("Menu ID : %d"), l_command);
                l_log_str = l_log_str + l_next_menu->m_menu_item->m_menu_item_name;
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

                if (l_command) {
                    f_menu_command_start++;
                }

                //Check for menu enable/disable
                if (!m_sftmenuinterface->CanEnableMenuItem(l_next_menu->m_menu_item)) {
                    f_sftmenu->EnableMenuItem(l_command, MF_BYCOMMAND | MF_GRAYED);
                }

                if (m_sftmenuinterface->CanCheckMenuItem(l_next_menu->m_menu_item)) {
                    f_sftmenu->CheckMenuItem(l_command, MF_BYCOMMAND | MF_CHECKED);
                }

                f_config_menu->SetPqmConfigMenuMap(l_command , l_next_menu->m_menu_item) ;
                l_command++ ;
            }

        } else {

            //its s popup menu again
            PopupMenuEx* l_new_sub_menu = new PopupMenuEx ;
            f_config_menu->AppendPopUpMenu(l_new_sub_menu);

            CreateSFTToolMenu(l_next_menu, l_new_sub_menu, f_config_menu, f_menu_command_start);
            f_sftmenu->AppendMenu(MF_POPUP, (UINT) l_new_sub_menu->m_hMenu, l_next_menu->m_popup_menu_string) ;
        }
    }

    return true;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//************************************Method Header************************************
// Method Name  : GetConfigPopupMenuItemList
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void PQMConfigMenu::AppendPopUpMenu(
    PopupMenuEx* f_popupmenu
)
{
    m_config_popupmenuitem_list.AddTail(f_popupmenu);
}


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GetCommandForMenuItem
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int PQMConfigMenu::GetCommandForMenuItem(
    const MenuData* f_popup_menu,
    const int f_command
)const
{
    if (!f_popup_menu) {
        return -1;
    }

    if (f_popup_menu->m_menu_item_name.IsEmpty()) {
        return -1;
    }

    if (f_popup_menu->m_menu_item_name.CompareNoCase(g_sft_menu_seperator) == 0) {
        return 0;
    }

    return (f_command + 1) ;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu




//************************************Method Header************************************
// Method Name  : SetPqmConfigMenuMap
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void PQMConfigMenu::SetPqmConfigMenuMap(
    const int f_index,
    MenuData* f_menudata
)
{
    m_pqmconfigmenu_map[f_index] = f_menudata ;
}



//************************************Method Header************************************
// Method Name  : CSFTMenuCreator
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CSFTMenuCreator::CSFTMenuCreator(CPQMView* f_main_view
                                ):	m_main_view(f_main_view),
    m_sft_menu(static_cast<CSFTMenuInterface*>(this)),
    m_tool_menu(static_cast<CSFTMenuInterface*>(this)),
    m_load_toolmenu(false)
{

}


//************************************Method Header************************************
// Method Name  : ~CSFTMenuCreator
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CSFTMenuCreator::~CSFTMenuCreator()
{

    DetachSFTToolmenu();
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : AttachSFTMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::AttachSFTMenu(
    PopupMenuEx* f_menu
)
{
    m_sft_menu.CreateSFTToolMenu(ID_SFT_MENU_MIN, g_SFT_Menu_str, f_menu);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : AttachToolMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::AttachToolMenu(
    PopupMenuEx* f_menu
)
{
    LPCTSTR FUNC_NAME = _T("CSFTMenuCreator::AttachToolMenu");

    if (!m_load_toolmenu) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_load_toolmenu is false..."));
        return;
    }

    m_tool_menu.CreateSFTToolMenu(ID_TOOL_MENU_MIN, g_Tool_Menu_str, f_menu);

}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : DetachSFTToolmenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::DetachSFTToolmenu(
)
{
    m_sft_menu.DetachMenu();
    m_tool_menu.DetachMenu();
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GenerateSFTMenuFromXMLData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::GenerateSFTMenuFromXMLData(
)
{
    SetSFTOrToolMenu(GetSFTOrToolMenuFromXMLData(true), m_sft_menu);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GenerateToolMenuFromXMLData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::GenerateToolMenuFromXMLData(
)
{
    if (!m_load_toolmenu) {
        return;
    }

    SetSFTOrToolMenu(GetSFTOrToolMenuFromXMLData(false), m_tool_menu);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//************************************Method Header************************************
// Method Name  : SetToolMenuAvaibility
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::SetToolMenuAvaibility(
    const bool f_flag
)
{
    m_load_toolmenu = f_flag ;
}


//************************************Method Header************************************
// Method Name  : GetSFTOrToolMenuFromXMLData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PopupMenu* CSFTMenuCreator::GetSFTOrToolMenuFromXMLData(
    const bool f_sft_menu
) const
{

    VARIANT* pData = new VARIANT;

    if (f_sft_menu) {
        m_main_view->GetAcquisition()->GetSFTMenuXMLData(&pData);

    } else {
        m_main_view->GetAcquisition()->GetToolMenuXMLData(&pData);
    }

    PopupMenu* l_sft_tool_menu = (PopupMenu*)pData->byref;

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }

    return l_sft_tool_menu;
}


//************************************Method Header************************************
// Method Name  : SetSFTOrToolMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuCreator::SetSFTOrToolMenu(
    PopupMenu* f_sft_tool_menu_xmldata,
    PQMConfigMenu& f_config_menu
) const
{

    if (NULL == f_sft_tool_menu_xmldata) {
        return;
    }

    POSITION pos = f_sft_tool_menu_xmldata->m_menu_list.GetHeadPosition();

    if (pos) {
        f_config_menu.SetPqmConfigMenu(f_sft_tool_menu_xmldata->m_menu_list.GetNext(pos));
    }
}






//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : CanEnableMenuItem
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CSFTMenuCreator::CanEnableMenuItem(
    const MenuData* f_popup_menu
)const
{
    if (!f_popup_menu) {
        return false;
    }

    if (f_popup_menu->RemoteConsole.CompareNoCase(_T("1")) == 0) {
        if (IsRemoteDesktopActive()) {
            return false;
        }
    }

    if (f_popup_menu->m_menu_item_name.CompareNoCase(g_sft_manual_prescan) == 0)
        return m_main_view->CanPerfornManualPrescan();

    if (m_main_view->IsAMBInvoked())
        return false;

    return true ;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : CanCheckMenuItem
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CSFTMenuCreator::CanCheckMenuItem(
    const MenuData* f_popup_menu
)const
{
    if (!f_popup_menu) {
        return false;
    }

    if (f_popup_menu->m_menu_item_name.CompareNoCase(g_sft_cds_debug) == 0) {
        return m_main_view->IsCDSDebugON();
    }

    if (m_main_view->IsAMBInvoked())
        return false;

    return false;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : IsRemoteDesktopActive
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CSFTMenuCreator::IsRemoteDesktopActive(
)const
{

    //return true ;
    return (S_OK == m_main_view->GetAcquisition()->RemoteCheckActiveFlag()) ;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GetSFTParamValue
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSFTMenuCreator::GetSFTParamValue(
    const CString& f_param
)const
{

    CString l_param_val = f_param;

    VARIANT* pData = new VARIANT;
    pData->byref = &l_param_val;

    const int l_selected_protocol = m_main_view->GetAcqOderForGivenProtocolIndex(GetPQMListCtrl()->GetFirstSelectedItem());
    m_main_view->GetAcquisition()->GetSFTParamValue(&pData, l_selected_protocol);

    DEL_PTR(pData)

    return l_param_val;

}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//************************************Method Header************************************
// Method Name  : GetPQMListCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMListCtrl* CSFTMenuCreator::GetPQMListCtrl(
)const
{
    if (m_main_view) {
        return &(m_main_view->GetPQMListCtrl());
    }

    return NULL ;
}




//************************************Method Header************************************
// Method Name  : CSFTMenuHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CSFTMenuHandler::CSFTMenuHandler(
    CPQMView* f_main_view
):	CSFTMenuCreator(f_main_view),
    m_hSFTMenuProcess(NULL)
{

}


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : ~CSFTMenuHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CSFTMenuHandler::~CSFTMenuHandler()
{

}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : HandleSFTMenuCommand
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuHandler::HandleSFTMenuCommand(
    const UINT f_cmd_id
)
{
    const MenuData* l_popup_menu = NULL ;

    if (f_cmd_id >= ID_SFT_MENU_MIN && f_cmd_id <= ID_SFT_MENU_MAX) {
        l_popup_menu = m_sft_menu.GetPqmConfigMenuMap(f_cmd_id) ;

    } else if (f_cmd_id >= ID_TOOL_MENU_MIN && f_cmd_id <= ID_TOOL_MENU_MAX) {
        l_popup_menu = m_tool_menu.GetPqmConfigMenuMap(f_cmd_id) ;
    }

    ExecutePQMConfigMenuCommand(l_popup_menu);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//************************************Method Header************************************
// Method Name  : SetSFTMenuProcessHandle
// Author       : iGate/ SP
// Purpose      : Sets the handle of SFTMenu Processes  window
//***********************************************************************************
void CSFTMenuHandler::SetSFTMenuProcessHandle(HANDLE hSFTMenuProcess)
{
    m_hSFTMenuProcess = hSFTMenuProcess;
}

//************************************Method Header************************************
// Method Name  : GetSFTMenuProcessHandle
// Author       : iGate/ SP
// Purpose      : Returns the handle of SFTMenu Processes  window
//***********************************************************************************
HANDLE CSFTMenuHandler::GetSFTMenuProcessHandle()const
{
    return m_hSFTMenuProcess;
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : PrepareCommmandStr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSFTMenuHandler::PrepareCommmandStr(
    const CString& f_cmd
) const
{
    CString str_to_return = f_cmd;

    //$SD : <Sequence Directory>
    //$SF : -study <Study Filename>
    //$sf : <Study Filename>
    //$PI : -protocol <Protocol Index>
    //$pi : <Protocol Index>
    //$SP : -scratchPad <Scratchpad>
    //$sp : <Scratchpad>
    //$PL : <Protocol List, separated by comma (1, 2, 3)>
    //$pl : <Protocol List, separated by space (1 2 3)>

    const int l_total_formats = 9;
    CString l_array[l_total_formats] = { g_SD,
                                         g_SF,
                                         g_sf,
                                         g_PI,
                                         g_pi,
                                         g_SP,
                                         g_sp,
                                         g_PL,
                                         g_pl,
                                       };

    for (int l_index = 0; l_index < l_total_formats; l_index++) {

        if (str_to_return.Find(l_array[l_index]) != -1) {

            str_to_return.Replace(l_array[l_index], GetCommandstr(l_array[l_index]));
        }
    }

    return str_to_return;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu



//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : PrepareAppStr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSFTMenuHandler::PrepareAppStr(
    CString f_apppath
) const
{
    int l_start_index = 0;
    int l_end_index = 0;
    CString l_env_str = _T("");
    CString l_str_to_replace = _T("");

    while ((l_start_index = f_apppath.Find(g_percentage_chr, l_start_index)) != -1) {

        if ((l_end_index = f_apppath.Find(g_percentage_chr, l_start_index + 1)) != -1) {

            l_str_to_replace = f_apppath.Mid(l_start_index + 1, l_end_index - l_start_index - 1);
            l_env_str = GetEnvFor(l_str_to_replace);
            l_str_to_replace = g_percentage_str + l_str_to_replace + g_percentage_str ;

            f_apppath.Replace(l_str_to_replace, l_env_str);

            l_start_index = 0;

        } else {
            break ;
        }
    }


    return f_apppath;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu



//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : ExecuteSFTMenuDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuHandler::ExecuteSFTMenuDialog(
    const MenuData* f_popup_menu
)const
{
    if (!f_popup_menu) {
        return ;
    }

    if (f_popup_menu->m_menu_item_name.Compare(g_sft_cds_debug) == 0) {
        //invoke respective functions
        m_main_view->OnOffCDSDebug();

    } else if (f_popup_menu->m_menu_item_name.Compare(g_sft_manual_prescan) == 0) {
        //invoke respective functions

        //m_main_view->OnClickMenuitemManualPrescan();
        m_main_view->PostMessage(WM_OPEN_MANUAL_PRESCAN, 0, 0);
    }
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu



//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : ExecuteSFTMenuCommand
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuHandler::ExecuteSFTMenuCommand(
    const MenuData* f_popup_menu
) const
{
    if (!f_popup_menu) {
        return ;
    }

    CString l_command_line =  f_popup_menu->m_cmd + _T(" ") + PrepareCommmandStr(f_popup_menu->m_param) ;
    CString l_app_path = PrepareAppStr(f_popup_menu->m_cmd);

    const bool l_sync = (f_popup_menu->m_sync.CompareNoCase(_T("1")) == 0);

    //+Patni-RAJ/2010AUG23/Added + Modified/V1.30/IR-152-Part4
    const bool l_interact = (f_popup_menu->m_interact.CompareNoCase(_T("1")) == 0);
    ExecuteCommandShell(l_app_path, l_command_line, l_sync, l_interact);
    //-Patni-RAJ/2010AUG23/Added + Modified/V1.30/IR-152-Part4
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : ExecuteCommandShell
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuHandler::ExecuteCommandShell(
    CString f_app_path,
    CString f_command_line,
    const bool f_sync, /*= true*/
    const bool f_interact /*= true*/     //Patni-RAJ/2010AUG23/Added/V1.30/IR-152-Part4
)const
{

    LPCTSTR FUNC_NAME = _T("CSFTMenuHandler::ExecuteCommandShell");

    //+Patni-RAJ+AJS/2010AUG23/Modified/V1.30/IR-152-CodeReviewComment

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_app_path);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_command_line);

    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    f_command_line = f_command_line.IsEmpty() ? f_app_path : f_command_line ;

    PROCESS_INFORMATION pi;

    //+Patni-RAJ/2010AUG23/Modified/V1.30/IR-152-Part4
    //if (!CreateProcess(f_app_path.IsEmpty() ? NULL : f_app_path.GetBuffer(f_app_path.GetLength()), f_command_line.IsEmpty() ? NULL : f_command_line.GetBuffer(f_command_line.GetLength()), NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
    if (!CreateProcess(f_app_path.IsEmpty() ? NULL : f_app_path.GetBuffer(f_app_path.GetLength()),
                       f_command_line.IsEmpty() ? NULL : f_command_line.GetBuffer(f_command_line.GetLength()),
                       NULL, NULL, false, (f_interact ? 0 : CREATE_NO_WINDOW),
                       NULL, NULL, &si, &pi)) {

        //-Patni-RAJ/2010AUG23/Modified/V1.30/IR-152-Part4


        CString l_log_ms;
        l_log_ms.Format(_T("Could not run the program(%s) %d = GetLastError()"), f_app_path, GetLastError());

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_ms);

    } else {

        //+Patni-RAJ+AJS/2010AUG23/Modified/V1.30/IR-152-CodeReviewComment
        if (f_sync) {

            //+Patni-Hemant/2010Aug05/Modified/IR-152_ITT_DefectFix
            m_main_view->SetWaitCursor(true);
            m_main_view->SetWaitCursorForModelessDlg(true);

            m_SFtMenuName = f_app_path.Right(f_app_path.GetLength() - f_app_path.ReverseFind(_T('\\')) - 1);

            m_hSFTMenuProcess = pi.hProcess ;

            DWORD l_result = WaitForSingleObject(pi.hProcess, INFINITE);

            if (l_result == 0) {
                m_hSFTMenuProcess = NULL;
            }

            m_main_view->SetWaitCursor(false);
            m_main_view->SetWaitCursorForModelessDlg(false);
            //-Patni-Hemant/2010Aug05/Modified/IR-152_ITT_DefectFix

            switch (l_result) {

                case WAIT_ABANDONED :

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("WaitForSingleObject(WAIT_ABANDONED)"));
                    break ;

                case WAIT_OBJECT_0 :

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("WaitForSingleObject(WAIT_OBJECT_0)"));
                    break ;

                case WAIT_TIMEOUT:

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("WaitForSingleObject(WAIT_TIMEOUT)"));
                    break ;

                case WAIT_FAILED:

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("WaitForSingleObject(WAIT_FAILED)"));
                    break ;
            }
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        //-Patni-RAJ+AJS/2010AUG23/Modified/V1.30/IR-152-CodeReviewComment
    }

    f_command_line.ReleaseBuffer();

    //Patni-RAJ+AJS/2010AUG23/Added/V1.30/IR-152-CodeReviewComment
    f_app_path.ReleaseBuffer();
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : ExecutePQMConfigMenuCommand
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSFTMenuHandler::ExecutePQMConfigMenuCommand(
    const MenuData* f_popup_menu
) const
{
    if (!f_popup_menu) {
        ASSERT(FALSE);
        return ;
    }

    if (f_popup_menu->m_cmd.IsEmpty()) {
        //execute dialog invocation
        ExecuteSFTMenuDialog(f_popup_menu);

    } else {
        //execute command
        ExecuteSFTMenuCommand(f_popup_menu);
    }
}
//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu




//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GetCommandstr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSFTMenuHandler::GetCommandstr(
    const CString& f_format
) const
{
    //$SD : <Sequence Directory>
    //$SF : -study <Study Filename>
    //$sf : <Study Filename>
    //$PI : -protocol <Protocol Index>
    //$pi : <Protocol Index>
    //$SP : -scratchPad <Scratchpad>
    //$sp : <Scratchpad>
    //$PL : <Protocol List, separated by comma (1, 2, 3)>
    //$pl : <Protocol List, separated by space (1 2 3)>

    CString l_str_to_return = _T("");

    if (f_format.Compare(g_SD) == 0) {

        l_str_to_return = GetSFTParamValue(g_SD);

    } else if (f_format.Compare(g_SF) == 0) {

        l_str_to_return = GetSFTParamValue(g_SF);

    } else if (f_format.Compare(g_sf) == 0) {

        l_str_to_return = GetSFTParamValue(g_sf);

    } else if (f_format.Compare(g_PI) == 0) {

        CString l_acq_order_str ;
        l_acq_order_str.Format(_T("%d"), m_main_view->GetAcqOderForGivenProtocolIndex(GetPQMListCtrl()->GetFirstSelectedItem()));

        //Patni-Hemant/08Sep2010/Modified/IR-152 Reopen
        l_str_to_return = g_protocol_str + _T(" ") + l_acq_order_str ;

    } else if (f_format.Compare(g_pi) == 0) {
        CString l_acq_order_str ;
        l_acq_order_str.Format(_T("%d"), m_main_view->GetAcqOderForGivenProtocolIndex(GetPQMListCtrl()->GetFirstSelectedItem()));
        l_str_to_return = l_acq_order_str ;

    } else if (f_format.Compare(g_SP) == 0) {

        l_str_to_return = GetSFTParamValue(g_SP);

    } else if (f_format.Compare(g_sp) == 0) {

        l_str_to_return = GetSFTParamValue(g_sp);

    } else if (f_format.Compare(g_PL) == 0) {

        int* selected_items = NULL;
        const int count = m_main_view->GetPQMListCtrl().GetselectedItems(&selected_items);
        CString l_acq_order_str = _T("");

        if (selected_items && count > 0) {

            for (int i = 0; i < count; i++) {

                l_acq_order_str.Format(_T("%d"), m_main_view->GetAcqOderForGivenProtocolIndex(selected_items [i]));

                if (l_str_to_return.IsEmpty()) {
                    l_str_to_return = l_acq_order_str ;

                } else {

                    //Patni-Hemant/2010July21/Modified/TMSC-REDMINE-501
                    l_str_to_return = l_str_to_return + _T(",") + l_acq_order_str ;
                }
            }
        }

    } else if (f_format.Compare(g_pl) == 0) {

        int* selected_items = NULL;
        int count = GetPQMListCtrl()->GetselectedItems(&selected_items);

        CString l_acq_order_str = _T("") ;

        if (selected_items && count > 0) {

            for (int i = 0; i < count; i++) {

                l_acq_order_str.Format(_T("%d"), m_main_view->GetAcqOderForGivenProtocolIndex(selected_items [i]));

                if (l_str_to_return.IsEmpty()) {
                    l_str_to_return = l_acq_order_str ;

                } else {
                    l_str_to_return = l_str_to_return + _T(" ") + l_acq_order_str ;
                }
            }
        }
    }


    return l_str_to_return;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu



//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//************************************Method Header************************************
// Method Name  : GetEnvFor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSFTMenuHandler::GetEnvFor(
    CString f_env_name
) const
{
    if (f_env_name.IsEmpty())
        return _T("");

    f_env_name.Remove(g_percentage_chr);

    TCHAR l_env_name_to_return[100] = {0};

    GetEnvironmentVariable(f_env_name, l_env_name_to_return, MAX_PATH);

    return l_env_name_to_return ;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
