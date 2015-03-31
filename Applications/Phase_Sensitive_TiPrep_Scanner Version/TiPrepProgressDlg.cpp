// TiPrepProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tiprep.h"
#include "TiPrepProgressDlg.h"
#include "TiPrepView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiPrepProgressDlg dialog


CTiPrepProgressDlg::CTiPrepProgressDlg(CWnd* pParent /*=NULL*/)
	: CPopupDlg(CTiPrepProgressDlg::IDD, pParent),m_ncurrentthreads(0),m_ntotalthreads(0),m_nProgressVal(0),m_threads(NULL)
{
	//{{AFX_DATA_INIT(CTiPrepProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTiPrepProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CPopupDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTiPrepProgressDlg)
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_ctrlProgress);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTiPrepProgressDlg, CPopupDlg)
	//{{AFX_MSG_MAP(CTiPrepProgressDlg)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiPrepProgressDlg message handlers

HBRUSH CTiPrepProgressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr = CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);
    
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH)m_list_brush;
    } else {
        //
    }
    
    return hbr;
}

void CTiPrepProgressDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTiPrepProgressDlg::SetTotalThreadsRunning(int n)
{
    m_ntotalthreads = n;
}

void CTiPrepProgressDlg::UpdateProgress(BOOL tstatus)
{
	m_CriticalSection.Lock();
    m_nProgressVal++;
	 //m_nProgressVal += 100;
    if (TRUE == tstatus){
        m_ncurrentthreads++;
    }
    if (m_ntotalthreads <= m_ncurrentthreads){ 
        OnOK();
    }
    m_ctrlProgress.SetValue(m_nProgressVal);
	m_CriticalSection.Unlock();
}

void CTiPrepProgressDlg::SetRange(int range)
{
	m_nrange = range;

}


BOOL CTiPrepProgressDlg::OnInitDialog() 
{
	CPopupDlg::OnInitDialog();

    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    SetRibbonHeight(0);

    m_ctrlProgress.SetFillColor( RGB(24,116,205));
    m_ctrlProgress.SetShowValue( true);    
    m_ctrlProgress.SetTextColor(RGB(255,255,255));
    m_ctrlProgress.SetRange(m_nrange);

    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CTiPrepProgressDlg::OnOK()
{
    try {
        CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
       // CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        return;
    }
    
    CPopupDlg::OnOK();
}

CTiPrepProgressDlg::~CTiPrepProgressDlg()
{
    m_list_brush.DeleteObject();
}

void CTiPrepProgressDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    if (m_ntotalthreads <= m_ncurrentthreads){ 
        CPopupDlg::OnClose();
    }else if (m_threads != NULL){
		DWORD result;
		int totalthreadsrunnig = 0;

		for (int i = 0; i < m_ntotalthreads; i++)
		{
			if (m_threads[i]) {
				result = WaitForSingleObject( m_threads[i], 0);
				if (result == WAIT_OBJECT_0) {
					totalthreadsrunnig++;
				}
			}
		}
		if (m_ntotalthreads <= totalthreadsrunnig){ 
        CPopupDlg::OnClose();
		 }
        return;
    } else {
		 CPopupDlg::OnClose();;
	}
}

void CTiPrepProgressDlg::SetThreadHandles(HANDLE *hnd_arr)
{
	m_threads = hnd_arr;

}
