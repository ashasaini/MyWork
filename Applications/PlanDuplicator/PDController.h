// PDController.h: interface for the CPDController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDCONTROLLER_H__CC676A4C_47A7_4EF2_A15E_6898287F4D42__INCLUDED_)
#define AFX_PDCONTROLLER_H__CC676A4C_47A7_4EF2_A15E_6898287F4D42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PDDataHandler.h"
#include "dataTypes.h"


class CDPSComManager;
class CIniFileReader;
class CPDProcessExec;
class CvfcDBMgr;
class CPDCoordinates;
class CPDController  
{
public:
    CPDController();
    virtual ~CPDController();
    CPDController(CPDController& f_controller);
    CPDController& operator = (CPDController& f_controller);
	int ClearCalcData(void);
    void SetAppMode(PDAppMode_t f_app_mode);
    int CalculateParams(const int f_nselected_row,const funcParams_t &f_fcparams);
    int Initialize(void);
    int SendMessagePQM(CString& prot_index_string);
    int SaveSlicePlaneData(const CString & f_sFilename);
    int GetProtocolsList(const CString & f_sFilename,p_data_t ** protocols,int *f_ntotal_prot );
    int InvokeSmartMPRVoi(void);
    CPDDataHandler * GetPDDataHandle() {return    &m_objPDData;}
    int Updatefields(const int f_ncurrent_row,const funcParams_t &fcParams, const int f_scout_selection);
    int Close(void);
    CIniFileReader * GetIniFileReader(void) const {return m_ini_reader;}
    int ConvertDicom(const int f_nprotidx,const CString& img_path,const char * f_csStudyfilename);
    void RemoveDirectoryRecursive(const CString& f_szPath);
    int GetSlicePlaneSelection(void) const ;
   // void SetSlicePlaneSelection(int f_nSlicePlaneSelection);
    int GetAttributeSelection(void) const;
    void SetAttributeSelection(int f_nAttributeSelection) ;
    int GetMPRorVOIMode(void) const;
    void SetScannedProtIdx(int f_nScannedProtIdx) { m_nScannedProtIdx = f_nScannedProtIdx; }

private:
    PDAppMode_t     m_app_mode;
    CPDDataHandler  m_objPDData;
    CDPSComManager  *m_comm;
    CPDProcessExec  *m_pdexec;
    CIniFileReader  *m_ini_reader;
    CvfcDBMgr       *m_vfc_mgr;
    CString         m_csstudy_file_name;
   // int             m_nSlicePlaneSelection;    
   // int             m_nAttributeSelection;
    int             m_nScannedProtIdx;   
    int             m_nProcIdx;    
};

#endif // !defined(AFX_PDCONTROLLER_H__CC676A4C_47A7_4EF2_A15E_6898287F4D42__INCLUDED_)
