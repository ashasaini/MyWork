//TMSC-REDMINE-1671
//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
//TMSC-REDMINE-777
// SFTMenuHandler.h: interface for the CSFTMenuHandler class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-777
#ifndef __SM__SFT_MENU_HANDLER_H__
#define __SM__SFT_MENU_HANDLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MPlusUICommon/PopupMenuEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;

struct PopupMenu;
struct MenuData;
class CPQMView ;
class CPQMListCtrl ;

class PQMConfigMenu;


class CSFTMenuInterface : private MR::ACQ::SM::COMMON::NonCopyable<CSFTMenuInterface>
{

public :
    virtual bool CanEnableMenuItem(const MenuData* f_popup_menu)const = 0;
    virtual bool CanCheckMenuItem(const MenuData* f_popup_menu)const = 0;
};



typedef CList<PopupMenuEx*, PopupMenuEx*> ConfigPopupMenuItemList;
typedef CMap<int, int, void*, void*> PqmConfigMenuMap;

class PQMConfigMenu : private MR::ACQ::SM::COMMON::NonCopyable<PQMConfigMenu>
{
public:

    explicit PQMConfigMenu(CSFTMenuInterface* f_sftmenuinterface) ;
    ~PQMConfigMenu();

    void DetachMenu() ;
    void SetPqmConfigMenu(PopupMenu* pqm_confg_menu);

    const MenuData* GetPqmConfigMenuMap(const int f_index) ;
    void CreateSFTToolMenu(const UINT f_menu_id, const CString& f_menu_str, PopupMenuEx* f_main_menu);

private :

    ConfigPopupMenuItemList m_config_popupmenuitem_list;
    PqmConfigMenuMap m_pqmconfigmenu_map;
    PopupMenu* m_pqm_confg_menu ;
    CSFTMenuInterface* m_sftmenuinterface;

    bool CreateSFTToolMenu(PopupMenu* f_popup_menu, PopupMenuEx* f_sftmenu, PQMConfigMenu* f_config_menu, int& f_menu_command_start) const;
    void AppendPopUpMenu(PopupMenuEx* f_popupmenu) ;

    int GetCommandForMenuItem(const MenuData* f_popup_menu, const int f_command) const ;
    void SetPqmConfigMenuMap(const int f_index, MenuData* f_menudata);
};



class CSFTMenuCreator : public CSFTMenuInterface
{
public:
    explicit CSFTMenuCreator(CPQMView* f_main_view);
    virtual ~CSFTMenuCreator();

    void AttachSFTMenu(PopupMenuEx* f_menu);
    void AttachToolMenu(PopupMenuEx* f_menu);
    void DetachSFTToolmenu();

    void GenerateSFTMenuFromXMLData() ;
    void GenerateToolMenuFromXMLData() ;
    void SetToolMenuAvaibility(const bool f_flag);

protected :
    CPQMView* m_main_view ;
    PQMConfigMenu m_sft_menu ;
    PQMConfigMenu m_tool_menu ;
    bool m_load_toolmenu ;

    PopupMenu* GetSFTOrToolMenuFromXMLData(const bool f_sft_menu = true) const;
    void SetSFTOrToolMenu(PopupMenu* f_sft_tool_menu_xmldata, PQMConfigMenu& f_config_menu) const;

    virtual bool CanEnableMenuItem(const MenuData* f_popup_menu)const;
    virtual bool CanCheckMenuItem(const MenuData* f_popup_menu)const;

    bool IsRemoteDesktopActive()const;

    CString GetSFTParamValue(const CString& f_param)const;
    CPQMListCtrl* GetPQMListCtrl()const;
};




class CSFTMenuHandler : public CSFTMenuCreator
{
public:
    explicit CSFTMenuHandler(CPQMView* f_main_view);
    virtual ~CSFTMenuHandler();

    void HandleSFTMenuCommand(const UINT f_cmd_id) ;
    void SetSFTMenuProcessHandle(HANDLE hSFTMenuProcess);
    HANDLE GetSFTMenuProcessHandle()const;
    mutable CString m_SFtMenuName;
private :
    mutable HANDLE m_hSFTMenuProcess;

    CString PrepareCommmandStr(const CString& f_cmd) const;
    CString PrepareAppStr(CString f_apppath) const;

    void ExecuteSFTMenuDialog(const MenuData* f_popup_menu)const;
    void ExecuteSFTMenuCommand(const MenuData* f_popup_menu) const;
    void ExecuteCommandShell(CString f_app_path, CString f_command_line, const bool f_sync = true, const bool f_interact = true)const;
    void ExecutePQMConfigMenuCommand(const MenuData* l_popup_menu) const;

    CString GetCommandstr(const CString& f_format) const;
    CString GetEnvFor(CString f_env_name) const;
};



#endif // #ifndef __SM__SFT_MENU_HANDLER_H__
