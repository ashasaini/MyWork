/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREPVIEW_H__E7B82CE0_894A_4575_A8A6_CE01FAE9F525__INCLUDED_)
#define AFX_TIPREPVIEW_H__E7B82CE0_894A_4575_A8A6_CE01FAE9F525__INCLUDED_

#include "DPSComManager.h"
#include "TiPrepDoc.h"
#include "ROIHandler.h"
#include "LocalDBMgr.h"
#include "Calc_equn1.h"
#include "Calc_equn2.h"
#include "DisplayData.h"
#include "PolarityCorrectionDlg.h" 
#include "PolarityCorrection.h" 
#include "AddOffset.h"
#include "Reconstruction.h"
#include "T1Map.h"  
#include <vector>
#include "ExtraPointsGen.h"
class CTiPrepController;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TiPrepDlg.h"
class CProcessExec;
class CSocketMgr; 
class CPhaseCorrection; 
class CTiPrepView : public CFormView
{
protected: // create from serialization only
    CTiPrepView();
    DECLARE_DYNCREATE(CTiPrepView)

public:
    //{{AFX_DATA(CTiPrepView)
    enum { IDD = IDD_TIPREP_FORM };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Attributes
public:
    void AddZOrderLayer(); 
    CTiPrepDoc* GetDocument();
    static CTiPrepView * GetView();
    CTiPrepView(CTiPrepView& f_view);
    CTiPrepView& operator = (CTiPrepView& f_view);
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTiPrepView)
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnInitialUpdate(); // called first time after construct
    //}}AFX_VIRTUAL

// Implementation
public:
    int GetHeightWidthImageFromDB(unsigned short &,unsigned short&);
    void DisplayErr(int &f_nmsg_code, int dlgcode = 0); 
    int CreateMapFiles(double*, double*);
    int GetStudyNameFromDB(CString &f_csStudyName);
    bool IsT1MapFlagSet() {
        return IsT1MapSelected;   
    }
    int GetSelectedIndex();
    int UpdateImageBuffer(CString &f_csimgLoid, void * imgBuff, unsigned int imgSz); 
    HWND GetChildHWND() {
        return child_app;   
    }
    void DisplaySeriesNumber(CString &imgLOID,  int f_nSelDlg = 0); 
    void DisplaySeriesNumber(); 
    int OnROICopy(CString &f_csSeriesLoid);
    int GetLocalDBProtocolNameAtSeriesLevel(CString &f_csSeriesLoid, CString &f_csProtocolName);
//    int GetLocalDBProtocolNameAtImageLevel(CString f_csImageLoid, CString & f_csProtocolName,CString &f_csStudyName);
//    int GetLocalDBImgData(CString f_csImageLoid, void **f_pImgData,
//                          int *f_pnsize, float *f_pfScale,
//                          int *f_pnTime_stamp, unsigned short *f_nlData_Type,int &f_nImgNo);//sb
    BOOL HideROI(int f_nROINo, int f_nChecked);
    int SetImageLOID(long f_no_of_images_selected,
                     /*In*/vector <CString> & f_image_loid);
    int GetImgdata(double **& f_pOrgData , double **& f_pInvData,
                   double **& f_pFitData, long & f_nTotalImgCnt);
    int GetDisplayVals(double ** f_pfTiVals, double ** f_pfErrVals,
                       double ** f_pfT1Vals, double ** f_pfTiRevVals,
                       double ** f_pfErrRevVals, double ** f_pfT1RevVals);
    int GetSelSeriesName(int f_nCheckStatus/*,CString & f_csSeriesName*/); 
    int GetNoiseOffsetVal(double & f_nNoiseOffsetVal);
    int RoiCopyHandler(vector<CString> & f_csSeriesLoidVector, vector<long> & f_nSeriesNumberVector,
                       vector<long> & f_nProtocolNumberVector, CString & f_source_series,
                       int & f_nTotalScannedProt, int & f_nTotalSeriesCount);
    int TiCopyHandler( 
                    std::vector<int> & f_nTiValueVector,
                    std::vector<CString> & f_csProtSequenceVector,
                    std::vector<int> & f_nProtIndexVector,
                    std::vector<long> & f_nProtocolNumberVector, 
                    int & f_nTotalScannedProt, 
                    int & f_nTotalSeriesCount,
                    CString &f_csStudyPath);

    int OnExit();
    int OnReselect();
    int OnEquationThree(int f_nEquFlag);
    int OnEquationTwo(int f_nEquFlag);
    int OnEquationOne(int f_nEquFlag);
    int OnApply(int f_nEqnSelectFlag);
    int OnNoiseOffsetCorrection(BOOL *f_roi, BOOL *f_DisplayOption, 
        BOOL *f_RevMin, int f_nEqnSelectFlag); 
    bool IsWaitIsel();
    bool IsWaitCursor();
    void SetWaitCursor(bool flag);
    virtual ~CTiPrepView();
    void SetIselEvent(int state) {
        m_event_img_select = state;
    } 

    int GetISelEvent() {
        return m_event_img_select;   
    }
    BOOL RegisterWindowZOrder(HWND hWnd_i);
    BOOL UnRegisterWindowZOrder(HWND hWnd_i); 
    BOOL ChangeWindowZOrder(HWND hWnd_i);
    BOOL RequestShowISelector(BOOL bShow);
    BOOL RegisterISelector();
    BOOL GetSelectedImages();
    BOOL SetStateProcessTools(BSTR f_custom);
    BOOL SetStateROITools(BSTR f_custom);
    CROIHandler* GetROIInstance();
    CLocalDBMgr* GetLocalDBMgrInstance();
    CTiPrepDlg* GetCTiPrepDlgInstance();
    CDisplayData* m_DisplayData;
    CT1Map* m_T1Map;    

    int OnSetMap(byte * roi_map, int height, int width);
    int getSelectedEqn(int &f_nEqnFlag);
    int OnEquationFive(int f_nEquFlag);
    int OnEquationFour(int f_nEquFlag);
    int SendMsg(char * f_msg);
    int OnReceive(char * f_msg, unsigned int f_nlength);
    int InvokeT1Map();
    int GetSelectedSeriesImages(CString &series_loid);
    void RefreshIsel();
    int onReconDone();
    int vfcDataGetGDCKind(CString f_csImgLoid);
    int vfcDataGetDataForm(CString &f_csDataType, vector<CString> &f_img_loid); //Shweta
    int vfcDataGetDataType(CString f_csImgLoid);
    int getNumImgSelectedISel(long &f_nSelImgNo);
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    void SetT1MapFlag(bool flg);
    bool GetT1MapFlag();
    int SavePreviousROIstate();
    bool RefreshGraphAndDataOnGui();
    int GetRepeatationTimeFromTag();
    void ClearRepeatationTimeTextBox();
    bool GetRoiDataAndDisplayROI();
    bool SaveRoiDataToFile(BOOL f_ROI[]);   
    int getDataForm(vector<CString> &f_csloids, int numImgs);
    //int getReconstuctionImgs(vector<CString> &f_csloids, int numImgs);
    int getInitialGuessFlag() const;
    void setInitialGuessFlag(const int &val);
    int performCurveFitForInitialGuessValues(const double &f_dAfactor, const double &f_dBfactor,
                              const double &f_dMfactor, const double &f_dNfactor,int &f_nEqnFlag);
    int getInitialGuessValues(double &f_dAfactor,
                              double &f_dBfactor,
                              double &f_dMfactor,
                              double &f_dNfactor);
    void DeAllocateMemoryOnCheckT1Map(); //AS
    //sb
    int getVfcTimeStamp(CString &f_csImgLoid,int &f_nImgNo,int *f_nTime_stamp);//sb
    int GenerateTimeStamp(int img_no,  float &initTI,float &time_stamp);
    void enableOneButtonPC(BOOL &f_bFlag);
    int OnApply_Ti();
    int getVfcTI(CString& StudyPath,int &f_nProtIndex,int *f_nTI);
    double* GetCurrentROIsTIValues();

protected:
    LRESULT OnInitTiPrep(WPARAM wParam, LPARAM lParam);
    LRESULT OnCmdClose(WPARAM wParam, LPARAM lParam);
    LRESULT OnCmdShow(WPARAM wParam, LPARAM lParam);
    LRESULT OnCmdHide(WPARAM wParam, LPARAM lParam);
// Generated message map functions
protected:
    //{{AFX_MSG(CTiPrepView)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    afx_msg void OnCancelEditSrvr();
    afx_msg void OnCmdDrawROIComplete(WPARAM wParam, LPARAM lParam);   //CFW Handle function for ROI mouse draw complete
    afx_msg void OnCmdGetSelImg(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void ClearMapFiles();
    int InitSocket();
    BOOL IsValidForSelectedIndex(int f_nIndex);
    

private:

    HWND child_app;
    BOOL m_is_init_done;
    bool m_is_wait_request_isel;
    bool m_is_wait_cursor;
    int m_nEquFlag;
    int m_event_img_select;
    int m_max_counter;
    bool IsT1MapSelected;
    CProcessExec  *m_pexec;
    CSocketMgr * m_socketComm;
    CLineFitStrategy m_lineFitData;
    int m_nInitialGuessFlag;
    int m_nisApplyDone;
    int m_ExtraPtsINTDVD;
};

#ifndef _DEBUG  // debug version in TiPrepView.cpp
inline CTiPrepDoc* CTiPrepView::GetDocument()
{
    return (CTiPrepDoc*)m_pDocument;
}
#endif

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPREPVIEW_H__E7B82CE0_894A_4575_A8A6_CE01FAE9F525__INCLUDED_)
