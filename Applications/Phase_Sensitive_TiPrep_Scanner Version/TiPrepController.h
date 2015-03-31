// TiPrepController.h: interface for the CTiPrepController class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREPCONTROLLER_H__1FA21A61_4D28_4549_904D_EDBCE9CC5C4A__INCLUDED_)
#define AFX_TIPREPCONTROLLER_H__1FA21A61_4D28_4549_904D_EDBCE9CC5C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PolarityCorrection.h"
#include "AddOffset.h"
#include "IRSListner.h"
#include "Reconstruction.h"
#include "PhaseCorrection.h"
#include "ApplyTi.h"
class CTiProlimUpdater;

class CTiPrepController
{
private:
    //private constructors and overloaded assignment operator for singleton class
    CTiPrepController();
    CTiPrepController(CTiPrepController& f_controller);
    CTiPrepController& operator = (CTiPrepController& f_controller);
    int CopyObject(CTiPrepController& f_controller);

private://Class Variables
	CString m_csStudyLoid;
    static CTiPrepController* m_TiController;
    CAddOffset * m_pAddOffset;
    CPolarityCorrection *m_pPolarityCorr;
    CReconstruction *m_cReconObj;
    CPhaseCorrection * m_phasecorr;

    int m_nDataForm;
    int m_nSingleButtonPhaseCorr;
    int m_nProtocolnoPC;
    int m_nProcessNoPC;

    CString m_csStudyFileName;

	CTiProlimUpdater *m_tiupdater; //AN
    CApplyTi *m_ApplyTiDlg;
public://destructor
    virtual ~CTiPrepController();

public://implementation
//AN scanner    
    int GetApplyTiLicense(BOOL &f_nLicense);   
    void RefreshApplyTiGUI();
	int UpdateTi(bool flag);
	void SendTiPrepCloseToPQM();
	int OnApplyTI(int prot,float ti);
	int GetPagerInfo();
	int isConnectedToPQM;
//	CString m_csStudyLoid;
	int ExtractPagerInfo(CString &csParam);
	//AN scanner
    static CTiPrepController* GetInstance();
    static void DeleteInstance();
    
    int SetImageLOIDForPolarityCorrection(long f_no_of_images_selected,
        /*In*/vector <CString> & f_image_loid);
        int PolarityCorrectionCalculation();
    
    int GetROIStatusForINROI(int &f_roi_status_for_inroi);  
    int AddOffsetCalculation(CString & f_offset_value, int f_add_offset_check_box_status,int *f_actual_expected_offset_value); 
    
    //Add Offset Functionality
    int SetImageLOIDForAddOffset(long f_no_of_images_selected, /*In*/vector <CString> & f_image_loid);
    int GetSelectedImageLOIDForAddOffset(vector <CString> &f_image_loid);
    int GetSelectedProtocolForAddOffset(CString & f_csProtName);
    void Close_AddOffset();

    CString GetFirstPolarityLOID() {
        if(m_pPolarityCorr) {
            return m_pPolarityCorr->GetFirstLOID();    
        } else {
            return _T("");
        }
    }

    CString GetFirstOffsetLOID() {
        if(m_pAddOffset) {
            return m_pAddOffset->GetFirstLOID();    
        } else {
            return _T("");
        }
    }

    int GetNumImagesForPolarityCorr() {
        if(m_pPolarityCorr) {
            return m_pPolarityCorr->GetNumSelectedImgs();    
        } else {
            return 0;
        }
    }

    int GetNumImagesForAddOffset() {
        if(m_pAddOffset) {
            return m_pAddOffset->GetNumSelectedImgs();    
        } else {
            return 0;
        }
    }

/*    int getSingleButtonPhaseCorr() const { 
        return m_nSingleButtonPhaseCorr; 
    }
    void setSingleButtonPhaseCorr(int val) { 
        m_nSingleButtonPhaseCorr = val; 
    }   */

    CString getStudyFileName() const { 
        return m_csStudyFileName; 
    }

/*    void setStudyFileName(CString val) { 
        m_csStudyFileName = val; 
    }*/

    int getProtocolnoPC() const { 
        return m_nProtocolnoPC; 
    }

/*    void setProtocolnoPC(int val) { 
        m_nProtocolnoPC = val; 
    }*/

    int doRawChkAndRecon(int f_nDataForm);
    int getReconstuctionImgs(vector<CString> &f_csloids/*, int numImgs*/);
    void SetPCLOIDs(vector<CString> &f_csloids, int numImgs, int manual_kind = MAGNITUDE);
    int SeriesPhaseCorrection();
    int AutoPhaseValidations();
    int GetSelImgInfoFromIsel(int & f_MaxCounter, int f_nEvent/*, CString f_csSeriesName*/);
    int ValidateManualSeries(CString& img_data_form, vector<CString> &f_loids);
    int ValidateSelectedSeries();
    int setDataFormPhaseCorrect(CString &f_csDataForm);
    int rawCheckRecon(CString &f_csStudyFileName,int &f_nProtocolNo);
    int validateSelectionSingleButton(int l_protocol_no,/*in*/CString & f_csStudyFileName);
    int CreateSeries(vector<CString> &imgLOIDs, vector<CString > &outLOIDs); //AN
    int InitializeIRS();
    int GetRawCheckStatusFromFile(int &f_nStatus);
    int DeleteRawCheckStatusFile();
    void DisplayApplyTiDlg();
};

#endif // !defined(AFX_TIPREPCONTROLLER_H__1FA21A61_4D28_4549_904D_EDBCE9CC5C4A__INCLUDED_)