//Redmine-783
//Redmine-782
//TMSC-Redmine-780
//Redmine-775
//Redmine-777
#ifndef __SM_QUEUE_DIALOG_H__
#define __SM_QUEUE_DIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QueueDialog.h : header file
//
#include <libStudyManager/NonCopyable.h>

/////////////////////////////////////////////////////////////////////////////
// CQueueDialog dialog


class CBaseInterfaceForQueueDialog;



const int g_max_queue_buttons = 5;

class CQueueDialog : private MR::ACQ::SM::COMMON::NonCopyable<CQueueDialog>

{


    // Construction
public:
    explicit CQueueDialog(CBaseInterfaceForQueueDialog* baseinterfaceforqueuedialog);   // standard constructor
    ~CQueueDialog() ;

    void EnableDisableButton(const ProconMessageEnum button_type, const bool enable_disable)const ;
    void EnableDisableAllButtons(const bool enable = true)const;
    bool IsQueueDialogButton(const POINT& f_pt);

    void SetQueueButton(const ProconMessageEnum f_button_type, CWnd* f_button);

    bool GetWaitCursor()const;
    void SetWaitCursor(const bool f_show_wait_cursor);

private:
    CBaseInterfaceForQueueDialog* m_baseinterfaceforqueuedialog ;
    bool m_show_wait_cursor;
    CWnd* m_queue_buttons[g_max_queue_buttons];


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_QUEUE_DIALOG_H__
