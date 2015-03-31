// EPIController.h: interface for the CEPIController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPICONTROLLER_H__A27DF9D9_4AA2_4BD5_A1EB_32AFEF38C49D__INCLUDED_)
#define AFX_EPICONTROLLER_H__A27DF9D9_4AA2_4BD5_A1EB_32AFEF38C49D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VFChandler.h"
#include "VFCReader.h"
#include "ImageWriter.h"
#include "FormStatus.h"
#include "EPILogHelper.h"
#include "VFCWriter.h"

class CEPIController
{
public:
    CEPIController();
    virtual ~CEPIController();
private:
    static CEPIController* m_EPIController;
    CVFCReader *m_vcfHandle;
    CString m_csImageFileName;
    CImageWriter m_imageWrite;
    CString m_csSeparateKT1T2DirPath;
    int m_nKT1T2ImgSize;
    int m_nProtIndex;
    int m_nKsT1T2MergeProtIndex;
    CVFCWriter *m_vfcWriter;
    CString m_csSeparateKEvenOddDirPath;
    int m_nSeparateKT1T2Flag;
    int m_nSeparateEvenOddFlag;
    int m_nPhaseCorrectionFlag;
    CString m_csPhaseCorrectionDirPath;
    int m_nDisplayFlag;


public:
	void SendMsgToView();
    static CEPIController* GetInstance();
    static void DeleteInstance();
    void setStudyFileName(const CString &f_csStudyFileName);
    int loadStudy(int f_nmergeDataFlag);
    int getChannelCount(int &f_nProtocolNo, int &f_nTotalChannelNo);
    int dumpData(CString &l_csPath, const int &f_nDataform, int &f_nChannelSelection);
    int displayImage(double *&f_dImageRawData, int &f_nHeight, int f_nWidth);
    int seperateTemplateAndKspace(int &f_nProtocolNo);
    int getStudyImagePath();
    CString getSeparateKT1T2DirPath() const;// { return m_csSeparateKT1T2DirPath; }
    int readFileDisplayImage(CString &f_csFileName, int &f_nDataForm ,int);
    int displayImagePath(int f_nProtIdx,int f_nFlag);
    int displayError(int f_nErrorCode);
    int dumpImageData(CString &f_csPath);
    int saveOriginalRawData();
    int mergeTemplateAndKspace();
    int ReRecon(int f_nReconProtIdx);
    int mergeData(CString f_csSelStep, int & f_ndstProt);
    int separateEvenOdd();
    CString getSeparateKEvenOddDirPath() const;
    int correctPhase(int f_nPhaseMethodSelection,int f_nTemplateSelFlag);
    CString getPhaseCorrectionDirPath() const;

private:
    int displayProtcol(int f_nmergeDataFlag);
    int loadImageStudyFile();
    int deleteFiles(CString &f_csFolderPath);
    CFormStatus * getImageInfoInstance();
    int ReadAndDumpOriginalRawData(CString &f_csImagePath);
    int getSelectedRawDataPath(CString &f_csImagePath, int &f_nProtocolNo, int &f_nSubProtCnt, int &f_nSubGrpCnt, int &f_nSliceNoCnt, int &f_nSliceImgNoCnt, CString &f_csFilePath);
    int getOriginalRawdata(CString &f_csImagePath, void *&f_dOrgImageRawData, int l_nChannel, int &f_nWidth, int &f_nHeight,int &f_datatype);
    int ConvertToDataType(double *f_DblData, void *&f_nIntData, int f_TotalPixels, int f_nDataType,bool f_Scaleflag);
    int getHeightWidth(CString &f_csFileName,int &f_nWidth,int &f_nHeight);
    int getFileName(CString f_csFolderPath,CString f_csStep, int f_nChannel,CString &f_csKSpaceFile, CString &f_csT1File,CString &f_csT2File);
    int PerformRawCheck(CString &f_csStudyFileName,int &f_nProtocolNo);
    int GetRawCheckStatusFromFile( int &f_nStatus );
    int DeleteRawCheckStatusFile();
    int performRecon(CString &f_csStudyFile,int f_nProtIdx);
    int InvertBuffer(void* &f_InBuf,int f_totalPixels,int f_DataType);
    int mergeEvenOddData();
    int createPhaseCorrectedDir(const int &f_PortIdx, 
                                int &f_nSubProtNo, 
                                int &f_nSubGrpNo, 
                                int &f_nSliceNo, 
                                int &f_nSliceImgNo, 
                                int &f_nPhaseSelectionFlag, 
                                CString &f_csDirPath);
    int mergePhaseCorrectedData();
};

#endif // !defined(AFX_EPICONTROLLER_H__A27DF9D9_4AA2_4BD5_A1EB_32AFEF38C49D__INCLUDED_)
