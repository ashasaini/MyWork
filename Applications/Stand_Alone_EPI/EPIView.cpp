// EPIView.cpp : implementation of the CEPIView class
//

#include "stdafx.h"
#include "EPI.h"

#include "EPIDoc.h"
#include "EPIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CEPIView* g_view = NULL;
/////////////////////////////////////////////////////////////////////////////
// CEPIView

IMPLEMENT_DYNCREATE(CEPIView, CView)

BEGIN_MESSAGE_MAP(CEPIView, CView)
    //{{AFX_MSG_MAP(CEPIView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_EPI_RELOAD, Oncmdreload)   

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPIView construction/destruction

CEPIView::CEPIView()
{
    // TODO: add construction code here
}

CEPIView::~CEPIView()
{
    if(m_pOptionsSheet) {
        delete m_pOptionsSheet;
        m_pOptionsSheet = NULL;
    }
}

BOOL CEPIView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEPIView drawing

void CEPIView::OnDraw(CDC* pDC)
{
    UNREFERENCED_PARAMETER(pDC);
    CEPIDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CEPIView printing

BOOL CEPIView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CEPIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CEPIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEPIView diagnostics

#ifdef _DEBUG
void CEPIView::AssertValid() const
{
    CView::AssertValid();
}

void CEPIView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CEPIDoc* CEPIView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEPIDoc)));
    return (CEPIDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEPIView message handlers


/*************************************************************
* Method:       OnCreate
* Description:  This handler is used to create Tab control
* Parameter:    LPCREATESTRUCT lpCreateStruct :
* Returns:      int
*************************************************************/
int CEPIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(CView::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    m_pOptionsSheet = new CSheetOptions();
    m_pOptionsSheet->Create(this, WS_SYSMENU | WS_CHILD | WS_VISIBLE, 0);
    CRect rSheet, rView;
    m_pOptionsSheet->GetWindowRect(rSheet);
    rView = rSheet;
    CalcWindowRect(rView);
    SetWindowPos(NULL, rView.left, rView.top,
                 rView.Width() + 5, rView.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
    m_pOptionsSheet->SetWindowPos(NULL, 0, 0, rSheet.Width(),
                                  rSheet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	g_view = this;
    return 0;
}

/*************************************************************
* Method:       OnSize
* Description:  This handler is used to fit tabbed pages inside Pane
* Parameter:    UINT nType :
* Parameter:    int cx : Width of Property Sheet
* Parameter:    int cy : Height of Property Sheet
* Returns:      void
*************************************************************/
void CEPIView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if(m_pOptionsSheet && cx && cy) {
        CRect r;
        GetClientRect(r);
        m_pOptionsSheet->MoveWindow(0, 0, cx, cy);
        CTabCtrl* pT = m_pOptionsSheet->GetTabControl();

        if(pT && pT->m_hWnd) {
            CRect rTabCtrl;
            pT->GetWindowRect(rTabCtrl);
            CPropertyPage* pP = m_pOptionsSheet->GetPage(0);

            if(pP && pP->m_hWnd) {
                CRect rPropPg;
                pP->GetWindowRect(rPropPg);
                int x1 = rPropPg.left - rTabCtrl.left;
                int y1 = rPropPg.top - rTabCtrl.top;
                //                int x2 = rTabCtrl.right - rPropPg.right;
                int y2 = rTabCtrl.bottom - rPropPg.bottom;
                pT->MoveWindow(0, 0, cx, cy);
                int nCnt = m_pOptionsSheet->GetPageCount();

                for(int i = 0; i < nCnt; i++) {
                    CPropertyPage* pP = m_pOptionsSheet->GetPage(i);

                    if(pP && pP->m_hWnd) {
                        pP->MoveWindow(x1, y1, cx - x1 - x1, cy - y1 - y2);
                    }
                }
            }
        }
    }
}
void CEPIView::Oncmdreload(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    (CEPIController::GetInstance())->loadStudy(0);
}
