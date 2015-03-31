// EPIFormView.cpp : implementation file
//

#include "stdafx.h"
#include "EPI.h"
#include "FormImageSelect.h"
#include "FormImageViewer.h"
#include "MainFrm.h"
#include "EPIController.h"
#include "CommonDefinations.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormImageSelect

IMPLEMENT_DYNCREATE(CFormImageSelect, CFormView)

CFormImageSelect::CFormImageSelect()
    : CFormView(CFormImageSelect::IDD)
{
    //{{AFX_DATA_INIT(CFormImageSelect)
    //}}AFX_DATA_INIT
}

CFormImageSelect::~CFormImageSelect()
{
}

void CFormImageSelect::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFormImageSelect)
	DDX_Control(pDX, IDC_EDIT_EPI_SPIN, m_editSpin);
    DDX_Control(pDX, IDC_LIST_IMAGE_DATA_PROTOCOL_LIST, m_ctrlImageDataList);
    DDX_Control(pDX, IDC_COMBO_SELECT_STEP, m_ctrlComboStepSelection);
    DDX_Control(pDX, IDC_COMBO_EPI_DATA_FORM, m_ctrlDataForm);
    DDX_Control(pDX, IDC_SPIN_CHANNEL, m_ctrlSpin);
    DDX_Control(pDX, IDC_LIST_RAW_DATA_PROTOCOL_LIST, m_ctrlRawDataList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImageSelect, CFormView)
    //{{AFX_MSG_MAP(CFormImageSelect)
    ON_CBN_SELCHANGE(IDC_COMBO_EPI_DATA_FORM, OnSelchangeComboEpiDataForm)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CHANNEL, OnDeltaposSpinChannel)
    ON_CBN_SELCHANGE(IDC_COMBO_SELECT_STEP, OnSelchangeComboSelectStep)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RAW_DATA_PROTOCOL_LIST, OnClickListRawDataProtocolList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMAGE_DATA_PROTOCOL_LIST, OnClickListImageDataProtocolList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RAW_DATA_PROTOCOL_LIST, OnItemchangedListRawDataProtocolList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IMAGE_DATA_PROTOCOL_LIST, OnItemchangedListImageDataProtocolList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImageSelect diagnostics

#ifdef _DEBUG
void CFormImageSelect::AssertValid() const
{
    CFormView::AssertValid();
}

void CFormImageSelect::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormImageSelect message handlers




void CFormImageSelect::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    //insert columns in list control
    m_ctrlRawDataList.DeleteAllItems();
    m_ctrlImageDataList.DeleteAllItems();
    int width = 350;
    m_ctrlRawDataList.InsertColumn(0, _T("Raw Data"), LVCFMT_LEFT, width);
    m_ctrlImageDataList.InsertColumn(0, _T("Image Data"), LVCFMT_LEFT, width);
    m_ctrlRawDataList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_ctrlImageDataList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_ctrlSpin.SetRange(0, 0);
    m_ctrlDataForm.SetCurSel(2);
    m_editSpin.SetReadOnly(TRUE);
    // TODO: Add your specialized code here and/or call the base class
}
int CFormImageSelect::setListCtrlData(char **f_cListData, int f_nTotalRawNodes,
                                      int f_nProtIdx, int f_nChannelCnt)
{
    try {
        //Fill the listcontrol here
        BOOL l_bRawChk = FALSE;

        if(f_cListData && f_nTotalRawNodes > 0) {
            m_ctrlRawDataList.DeleteAllItems();
            m_ctrlImageDataList.DeleteAllItems();
            int l_nCnt = 0;

            for(int l_nCnt_i = 0; l_nCnt_i < f_nTotalRawNodes; l_nCnt_i++) {
                CString strItem(_T(""));

                if(NULL != f_cListData[l_nCnt_i]) {
                    strItem.Format(_T("%s"), f_cListData[l_nCnt_i]);
                }

                int l_nLength = strItem.GetLength();
                int l_nCount = strItem.Find('/');
                CString l_csTemp = strItem.Left(l_nCount);
                l_nCount = l_csTemp.ReverseFind('.');
                l_nLength = l_csTemp.GetLength();
                l_csTemp = l_csTemp.Right(l_nLength - (l_nCount + 1));
                char buff[32];
                sprintf(buff, "%s", l_csTemp);
                int l_nProtocolNo = atoi(buff);

                if(f_nProtIdx == l_nProtocolNo) {
                    l_nCnt++;
                    LVITEM lv;
                    lv.iItem = l_nCnt;
                    lv.iSubItem = 0;//as only one column present
                    lv.pszText = strItem.GetBuffer(strItem.GetLength());
                    lv.mask = LVIF_TEXT;

                    if(FALSE == IsProcessed(strItem)) {
                        m_ctrlRawDataList.InsertItem(&lv);
                        l_bRawChk = TRUE;
                    } else {
                        m_ctrlImageDataList.InsertItem(&lv);
                    }
                }
            }
        }

        if(TRUE != l_bRawChk) {
            CEPILogHelper::WriteToErrorLog(_T("Selected Protocol does not have Raw data."), __LINE__, __FILE__);
            return EPI_PROT_RAW_DATA;
        }

        f_nChannelCnt--;
        m_ctrlSpin.SetRange(0, (short)f_nChannelCnt);
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
BOOL CFormImageSelect::IsProcessed(CString &cs)
{
    int cnt = 0;
    cnt = cs.Find("9.");

    if(cnt < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}


int CFormImageSelect::DisplayRawData()
{
    try {
        int l_nDataForm = m_ctrlDataForm.GetCurSel();
        int l_nChannelSelection = m_ctrlSpin.GetPos();
        int rowRaw = m_ctrlRawDataList.GetSelectionMark();
        CString s1(_T(""));

        if(rowRaw < 0) {
            return E_ERROR;
        }

        if(rowRaw > -1) {
            s1 = m_ctrlRawDataList.GetItemText(rowRaw, 0);
        }

        if(s1.IsEmpty() == TRUE) {
            m_ctrlRawDataList.EndWaitCursor();
            return E_ERROR;
        }

        m_csPathSelection = s1;
        int l_nStatus = (CEPIController::GetInstance())->dumpData(m_csPathSelection, l_nDataForm, l_nChannelSelection);

        if(E_NO_ERROR != l_nStatus) {
            (CEPIController::GetInstance())->displayError(l_nStatus);
            //AfxMessageBox("Error");
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
void CFormImageSelect::OnSelchangeComboEpiDataForm()
{
    // TODO: Add your control notification handler code here
    CString l_csSelStep;
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);

    if(l_nSelPos < 0 || l_nBuffCnt < 0) {
        return;
    }

    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();

    if(l_csSelStep.Compare("StudyFile Data") == 0) {
        DisplayRawData();
    }

    if(l_csSelStep.Compare("Separate Data") == 0) {
        DisplayKT1T2Data();
    }
    if(l_csSelStep.Compare("Separate Even Odd") == 0) {
        DisplayKEvenOddData();
    }
    if(l_csSelStep.Compare("Phase Correction") == 0) {
        DisplayPhaseCorrectedData();
    }


}

void CFormImageSelect::OnDeltaposSpinChannel(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UNREFERENCED_PARAMETER(pNMUpDown);
    // TODO: Add your control notification handler code here
    CString l_csSelStep(_T(""));
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);

    if(l_nSelPos < 0 || l_nBuffCnt < 0) {
        return;
    }

    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();

    if(l_csSelStep.Compare("StudyFile Data") == 0) {
        DisplayRawData();
    }

    if(l_csSelStep.Compare("Separate Data") == 0) {
        DisplayKT1T2Data();
    }
    if(l_csSelStep.Compare("Separate Even Odd") == 0) {
        DisplayKEvenOddData();
    }
    if(l_csSelStep.Compare("Phase Correction") == 0) {
        DisplayPhaseCorrectedData();
    }


    *pResult = 0;
}
int CFormImageSelect::getFileList(CString &f_csDirPath)
{
    bool found = false;
    HANDLE hFind;
    WIN32_FIND_DATA data;
    int l_nCnt_i = 0;
    m_ctrlRawDataList.DeleteAllItems();
    CString l_csDirPath = f_csDirPath + "*.*";
    hFind = FindFirstFile(l_csDirPath, &data);
    int numchannels = 0;
    CStringArray csarr;
    CString csitem, strItem, csimgpart;
    m_ctrlSpin.SetRange(0, 0);
    CString l_csSelStep(_T(""));
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    
    if(l_nSelPos < 0 || l_nBuffCnt < 0) {
        return E_ERROR;
    }

    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();


    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            found = false;

            if(strcmp(data.cFileName, ".") != 0
                    && strcmp(data.cFileName, "..") != 0) {
                printf("%s\n", data.cFileName);
                strItem.Format(_T("%s"), data.cFileName);
                int index = strItem.Find(_T("_Channel_"));
                csitem = strItem.Left(index);
                csitem += strItem.Right(strItem.GetLength() - strItem.Find(_T("_Dim")));

                if(csarr.GetSize() == 0) {
                } else {
                    for(int i = 0; i < csarr.GetSize(); i++) {
                        if(csarr.GetAt(i).CompareNoCase(csitem) == 0) {
                            found = true;
                            numchannels++;
                        }
                    }
                }

                if(!found) {
                    LVITEM lv;
                    lv.iItem = l_nCnt_i;
                    lv.iSubItem = 0;//as only one column present
                    lv.pszText = csitem.GetBuffer(csitem.GetLength());
                    lv.mask = LVIF_TEXT;
                    m_ctrlRawDataList.InsertItem(&lv);
                    csarr.Add(csitem);
                    numchannels++;
                }

                l_nCnt_i++;
            }
        } while(FindNextFile(hFind, &data));

        m_ctrlRawDataList.SetSelectionMark(0);
        if(l_csSelStep.Compare("Separate Even Odd") == 0) {
            numchannels /= 2;
        }
        if(l_csSelStep.Compare("Separate Data") == 0) {
            numchannels /= 3;
        }        
        m_ctrlSpin.SetRange(0, short(numchannels - 1));
        FindClose(hFind);
    }

    return 0;
}


void CFormImageSelect::addStep(CString &f_csStep)
{
    int nDex = 0;
    nDex = m_ctrlComboStepSelection.FindStringExact(nDex, f_csStep);

    if(CB_ERR == nDex) {
        m_ctrlComboStepSelection.AddString(f_csStep);
    }
    m_ctrlComboStepSelection.SelectString(0,f_csStep);
//     m_ctrlComboStepSelection.SetCurSel(0);
}

void CFormImageSelect::OnSelchangeComboSelectStep()
{
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();

    if(l_csSelStep.Compare("StudyFile Data") == 0) {
        int l_nStatus = (CEPIController::GetInstance())->getStudyImagePath();

        if(E_NO_ERROR != l_nStatus) {
            //Error;
            return;
        }
    }

    if(l_csSelStep.Compare("Separate Data") == 0) {
        CString l_csFilePath = (CEPIController::GetInstance())->getSeparateKT1T2DirPath();
        getFileList(l_csFilePath);
    }
    if(l_csSelStep.Compare("Separate Even Odd") == 0) {        
        CString l_csFilePath = (CEPIController::GetInstance())->getSeparateKEvenOddDirPath();
        getFileList(l_csFilePath);
    }
    if(l_csSelStep.Compare("Phase Correction") == 0) {        
        CString l_csFilePath = (CEPIController::GetInstance())->getPhaseCorrectionDirPath();
        getFileList(l_csFilePath);
    }

    

    //
}

int CFormImageSelect::DisplayKT1T2Data()
{
    try {
        int l_nDataForm = m_ctrlDataForm.GetCurSel();

        //int l_nChannelSelection = m_ctrlSpin.GetPos();
        if(m_csPathSelection.Compare("") == 0) {
            m_csPathSelection = m_ctrlRawDataList.GetItemText(0, 0);
        }

        CString imgname = m_csPathSelection.Left(m_csPathSelection.Find(_T("_")));
        CString dimension = m_csPathSelection.Right(m_csPathSelection.GetLength() - m_csPathSelection.Find(_T("_Dim")));
        CString completefilename;
        int channelno = 0;
        channelno = m_ctrlSpin.GetPos();
        completefilename.Format(_T("%s_Channel_%d%s"), imgname, channelno, dimension);
        int l_nStatus = (CEPIController::GetInstance())->readFileDisplayImage(completefilename, 
                                                                              l_nDataForm ,
                                                                              SEPARATE_KT1T2);

        if(E_NO_ERROR != l_nStatus) {
            //Error.
            return E_ERROR;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}


int CFormImageSelect::ClearGUIForm()
{
    m_ctrlRawDataList.DeleteAllItems();
    return E_NO_ERROR;
}

void CFormImageSelect::ClearSelectStepComboBox()
{
    m_ctrlComboStepSelection.ResetContent();
    ASSERT(m_ctrlComboStepSelection.GetCount() == 0);
}

int CFormImageSelect::DisplayImageData()
{
    int rowRaw = m_ctrlImageDataList.GetSelectionMark();
    CString s1(_T(""));

    if(rowRaw < 0) {
        return E_ERROR;
    }

    if(rowRaw > -1) {
        s1 = m_ctrlImageDataList.GetItemText(rowRaw, 0);
    }

    if(s1.IsEmpty() == TRUE) {
        m_ctrlImageDataList.EndWaitCursor();
        return E_ERROR;
    }

    m_csPathSelection = s1;
    int l_nStatus = (CEPIController::GetInstance())->dumpImageData(m_csPathSelection/*,l_nDataForm,l_nChannelSelection*/);

    if(E_NO_ERROR != l_nStatus) {
        (CEPIController::GetInstance())->displayError(l_nStatus);
    }

    return E_NO_ERROR;
}

void CFormImageSelect::OnClickListRawDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult) 
{

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here
    //  int l_nProtocolNo = 0;
    CString s1(_T(""));
    int rowRaw = 0;
    m_ctrlComboStepSelection.EnableWindow(TRUE);
    m_ctrlSpin.EnableWindow(TRUE);
    m_ctrlDataForm.EnableWindow(TRUE);
    m_editSpin.EnableWindow(TRUE);
    
   // if((pNMListView->uChanged & LVIF_STATE)
    //    && (pNMListView->uNewState & LVNI_SELECTED)) {
        rowRaw = pNMListView->iItem;//m_ctrlRawDataList.GetSelectionMark();
        
        if(rowRaw < 0) {
            return;
        }
        
        if(rowRaw > -1) {
            s1 = m_ctrlRawDataList.GetItemText(rowRaw, 0);
        }
        
        if(s1.IsEmpty() == TRUE) {
            m_ctrlRawDataList.EndWaitCursor();
            return;
        }
        
        m_csPathSelection = s1;
   // }
    
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();
    
    if(l_csSelStep.Compare("StudyFile Data") == 0) {
        m_ctrlRawDataList.SetSelectionMark(rowRaw);
        DisplayRawData();
    }
    
    if(l_csSelStep.Compare("Separate Data") == 0) {
        DisplayKT1T2Data();
    }
    if(l_csSelStep.Compare("Separate Even Odd") == 0) {
        DisplayKEvenOddData();
    }    
    if(l_csSelStep.Compare("Phase Correction") == 0) {
        DisplayPhaseCorrectedData();
    }

    
    *pResult = 0;
}

void CFormImageSelect::OnClickListImageDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CString s1(_T(""));
    int rowRaw = 0;
    
//     if((pNMListView->uChanged & LVIF_STATE)
//         && (pNMListView->uNewState & LVNI_SELECTED)) {
        rowRaw = pNMListView->iItem;//m_ctrlRawDataList.GetSelectionMark();
        
        if(rowRaw < 0) {
            return;
        }
        
        if(rowRaw > -1) {
            s1 = m_ctrlImageDataList.GetItemText(rowRaw, 0);
        }
        
        if(s1.IsEmpty() == TRUE) {
            m_ctrlImageDataList.EndWaitCursor();
            return;
        }
        
        m_csPathSelection = s1;
//     }
    
    int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();
    
    //     if(l_csSelStep.Compare("StudyFile Data") == 0) {
    m_ctrlImageDataList.SetSelectionMark(rowRaw);
    m_ctrlComboStepSelection.EnableWindow(FALSE);
    m_ctrlSpin.EnableWindow(FALSE);
    m_ctrlDataForm.EnableWindow(FALSE);
    m_editSpin.EnableWindow(FALSE);
    DisplayImageData();
    //     }
    
    *pResult = 0;
}
void CFormImageSelect::OnItemchangedListRawDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString s1(_T(""));
    int rowRaw = 0;
	if((pNMListView->uChanged & LVIF_STATE)
		&& (pNMListView->uNewState & LVNI_SELECTED)) {
        rowRaw = pNMListView->iItem;
		m_ctrlRawDataList.GetSelectionMark();
        if(rowRaw < 0) {
            return;
        }
        if(rowRaw > -1) {
            s1 = m_ctrlImageDataList.GetItemText(rowRaw, 0);
        }
        if(s1.IsEmpty() == TRUE) {
            m_ctrlImageDataList.EndWaitCursor();
            return;
        }
        m_csPathSelection = s1;
     }
	int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();
    if(l_csSelStep.Compare("StudyFile Data") == 0) {
        m_ctrlRawDataList.SetSelectionMark(rowRaw);
        DisplayRawData();
    }
    if(l_csSelStep.Compare("Separate Data") == 0) {
        DisplayKT1T2Data();
    }
	*pResult = 0;
}
void CFormImageSelect::OnItemchangedListImageDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString s1(_T(""));
    int rowRaw = 0;
	if((pNMListView->uChanged & LVIF_STATE)
		&& (pNMListView->uNewState & LVNI_SELECTED)) {
        rowRaw = pNMListView->iItem;
		m_ctrlRawDataList.GetSelectionMark();
        if(rowRaw < 0) {
            return;
        }
        if(rowRaw > -1) {
            s1 = m_ctrlImageDataList.GetItemText(rowRaw, 0);
        }
        if(s1.IsEmpty() == TRUE) {
            m_ctrlImageDataList.EndWaitCursor();
            return;
        }
        m_csPathSelection = s1;
     }
	int l_nSelPos = m_ctrlComboStepSelection.GetCurSel();
    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();
    m_ctrlImageDataList.SetSelectionMark(rowRaw);
    m_ctrlComboStepSelection.EnableWindow(FALSE);
    m_ctrlSpin.EnableWindow(FALSE);
    m_ctrlDataForm.EnableWindow(FALSE);
    m_editSpin.EnableWindow(FALSE);
    DisplayImageData();
	*pResult = 0;
}

int CFormImageSelect::DisplayKEvenOddData()
{
    try {
        int l_nDataForm = m_ctrlDataForm.GetCurSel();
        
        //int l_nChannelSelection = m_ctrlSpin.GetPos();
        if(m_csPathSelection.Compare("") == 0) {
            m_csPathSelection = m_ctrlRawDataList.GetItemText(0, 0);
        }
        
        CString imgname = m_csPathSelection.Left(m_csPathSelection.Find(_T("_")));
        CString dimension = m_csPathSelection.Right(m_csPathSelection.GetLength() - m_csPathSelection.Find(_T("_Dim")));
        CString completefilename;
        int channelno = 0;
        channelno = m_ctrlSpin.GetPos();
        completefilename.Format(_T("%s_Channel_%d%s"), imgname, channelno, dimension);
        int l_nStatus = (CEPIController::GetInstance())->readFileDisplayImage(completefilename,
                                                                              l_nDataForm ,
                                                                              SEPARATE_EVEN_ODD);
        
        if(E_NO_ERROR != l_nStatus) {
            //Error.
            return E_ERROR;
        }
    } catch(...) {
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

int CFormImageSelect::DisplayPhaseCorrectedData()
{
    try {
        int l_nDataForm = m_ctrlDataForm.GetCurSel();
        
        //int l_nChannelSelection = m_ctrlSpin.GetPos();
        if(m_csPathSelection.Compare("") == 0) {
            m_csPathSelection = m_ctrlRawDataList.GetItemText(0, 0);
        }
        
        CString imgname = m_csPathSelection.Left(m_csPathSelection.Find(_T("_")));
        CString dimension = m_csPathSelection.Right(m_csPathSelection.GetLength() - m_csPathSelection.Find(_T("_Dim")));
        CString completefilename;
        int channelno = 0;
        channelno = m_ctrlSpin.GetPos();
        completefilename.Format(_T("%s_Channel_%d%s"), imgname, channelno, dimension);
        int l_nStatus = (CEPIController::GetInstance())->readFileDisplayImage(completefilename,
                                                                              l_nDataForm ,
                                                                              PHASE_CORRECTION);
        
        if(E_NO_ERROR != l_nStatus) {
            //Error.
            return E_ERROR;
        }
    } catch(...) {
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}
