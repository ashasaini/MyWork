//TMSC-Redmine-782
//TMSC-Redmine-780
//TMSC-REDMINE-777

// QueueDialog.cpp : implementation file
//

#include "stdafx.h"
#include "QueueDialog.h"
#include <PQM/resource.h>
#include "DailogInterfaces.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueueDialog dialog


CQueueDialog::CQueueDialog(CBaseInterfaceForQueueDialog* baseinterfaceforqueuedialog
                          ):	m_baseinterfaceforqueuedialog(baseinterfaceforqueuedialog),
    m_show_wait_cursor(false)
{
    //{{AFX_DATA_INIT(CQueueDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT


    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    for (int l_index = 0; l_index < g_max_queue_buttons ; l_index++) {
        m_queue_buttons[l_index] = NULL ;
    }

    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
}

CQueueDialog::~CQueueDialog(
)
{

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CQueueDialog::EnableDisableButton(
    const ProconMessageEnum button_type,
    const bool enable_disable
)const
{
    CWnd* window_to_enable_disable = NULL ;

    switch (button_type) {

            //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
        case PQM_PROCON_QUEUE_NEXT :
        case PQM_PROCON_QUEUE_EXIT :
        case PQM_PROCON_NEXT_COPY :

            //+Patni-KSS/2010June16/ADDED/IR-155
        case PQM_PROCON_CANCEL :

            //-Patni-KSS/2010June16/ADDED/IR-155
        case PQM_PROCON_RESET:

            window_to_enable_disable = m_queue_buttons[(int)button_type] ;
            //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    }

    if (window_to_enable_disable)
        window_to_enable_disable->EnableWindow(enable_disable) ;

    else {

        ASSERT(FALSE);		//check for supported buttons here..
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CQueueDialog::EnableDisableAllButtons(
    const bool enable /*= true*/
)const
{

    for (int button_index = 0; button_index < g_max_queue_buttons ; button_index++) {

        m_queue_buttons[button_index]->EnableWindow(enable);
    }
}


//+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CQueueDialog::IsQueueDialogButton(
    const POINT& f_pt
)
{
    CRect l_rect ;

    for (int l_button_index = 0; l_button_index < g_max_queue_buttons ; l_button_index++) {

        if (m_queue_buttons[l_button_index]) {

            m_queue_buttons[l_button_index]->GetWindowRect(&l_rect) ;

            if (l_rect.PtInRect(CPoint(f_pt.x, f_pt.y))) {
                return true ;
            }
        }
    }

    return false ;
}
//-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

//+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
//************************************Method Header************************************
// Method Name  : SetQueueButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CQueueDialog::SetQueueButton(
    const ProconMessageEnum f_button_type,
    CWnd* f_button
)
{
    if (f_button) {

        switch (f_button_type) {
            case PQM_PROCON_QUEUE_NEXT :
            case PQM_PROCON_QUEUE_EXIT :
            case PQM_PROCON_NEXT_COPY :

                //+Patni-KSS/2010June16/ADDED/IR-155
            case PQM_PROCON_CANCEL:

                //-Patni-KSS/2010June16/ADDED/IR-155
            case PQM_PROCON_RESET:

                m_queue_buttons[(int)f_button_type] = f_button;
        }
    }
}

//-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

//************************************Method Header************************************
// Method Name  : GetWaitCursor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CQueueDialog::GetWaitCursor()const
{
    return m_show_wait_cursor;
}

//-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90



//+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CQueueDialog::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor = f_show_wait_cursor;
}


