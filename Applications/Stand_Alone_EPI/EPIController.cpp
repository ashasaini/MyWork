// EPIController.cpp: implementation of the CEPIController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EPI.h"
#include "IRSListner.h"
#include "EPIController.h"
#include "MainFrm.h"
#include "FormImageSelect.h"
#include "FormImageViewer.h"
#include "PageEPI.h"
#include "kspaceSeparator.h"
#include "ReconProcess.h"
#include "FileReader.h"
#include "EPIErrors.h"
#include "CommonDefinations.h"
#include <Winsock2.h>
#include "tami/libWNP/WNP_types.h"
#include <tami/libmathServer/msGetConfig.h>
#include "ReconRawCheck.h"
#include "ScrPadHandler.h"
#include <Shlwapi.h>
#include "EvenOddSeparator.h"
#include "PhaseCorrectionStrategy.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CEPIView* g_view;

CEPIController* CEPIController::m_EPIController = NULL;
CEPIController::CEPIController(): m_vcfHandle(NULL),m_vfcWriter(NULL),
m_nKsT1T2MergeProtIndex(-1),m_nProtIndex(-1),m_nSeparateKT1T2Flag(0)
{
}

CEPIController::~CEPIController()
{
    if(m_vcfHandle) {
        m_vcfHandle->CloseDBFile();
        delete m_vcfHandle;
        m_vcfHandle = NULL;
    }
	if(m_vfcWriter){
		delete m_vfcWriter;
		m_vfcWriter = NULL;
	}
}

CEPIController* CEPIController::GetInstance()
{
    if(NULL == m_EPIController) {
        m_EPIController = new CEPIController();

        if(NULL == m_EPIController) {
            CString l_cserr = _T("Memory allocation failed for CEPIController.");
        }
    }

    return m_EPIController;
}
void CEPIController::DeleteInstance()
{
    if(m_EPIController) {
        delete m_EPIController;
        m_EPIController = NULL;
    }
}

void CEPIController::setStudyFileName(const CString &f_csStudyFileName)
{
    if(m_vcfHandle == NULL) {
        m_vcfHandle = new CVFCReader();
    }
    if (m_vfcWriter == NULL)
    {
        m_vfcWriter = new CVFCWriter();
    }
    m_vcfHandle->StudyFileName(f_csStudyFileName);
    m_vfcWriter->StudyFileName(f_csStudyFileName);
    m_imageWrite.setStudyName(f_csStudyFileName);
}

int CEPIController::loadStudy(int f_nmergeDataFlag)
{
    try {
        int l_nStatus = loadImageStudyFile();

        if(E_NO_ERROR != l_nStatus) {
            CFormStatus *pWnd = getImageInfoInstance();
            CString l_csError(ERRORSTRING_EPI[l_nStatus]);
            pWnd->setErrorMessage(l_csError);
			if (EPI_NO_RAW_DATA!= l_nStatus) {
				CPageEPI::GetPageOneInstance()->ClearGUI();
				CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
				CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
				CFormImageSelect *pWnd2 = pChild->GetEPIFormView();
				pWnd2->ClearGUIForm();
				return E_ERROR;
			}
            
        }
        m_nDisplayFlag = 1;
        displayImagePath(0,1);
        displayProtcol(f_nmergeDataFlag);
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();
        pWnd->ClearSelectStepComboBox();
        CString l_csTemp("StudyFile Data");
        pWnd->addStep(l_csTemp);

//----------------------------------TEST SCRATCH PAD CREATION----------------------------------------        
//         CScrPadHandler l_SPHandler;
//         l_SPHandler.CreateScrPad("C:\\ScratchMe.tmp","D:\\StudyFiles\\Run10911.-1001\\Run10911.-1001",0);
//----------------------------------TEST SCRATCH PAD CREATION----------------------------------------

    } catch(...) {
        return E_ERROR;
    }
    m_nDisplayFlag = 0;
    return E_NO_ERROR;
}

int CEPIController::getChannelCount(int &f_nProtocolNo, int &f_nTotalChannelNo)
{
    try {
        int l_nStatus = m_vcfHandle->getNumberofChannel(f_nProtocolNo, f_nTotalChannelNo);

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("failed to get No of channel."), __LINE__, __FILE__);
            CFormStatus *pWnd = getImageInfoInstance();
            CString l_csTemp(ERRORSTRING_EPI[l_nStatus]);
            pWnd->setErrorMessage(l_csTemp);
            return l_nStatus;
        }

//  displayImagePath(f_nProtocolNo);
    } catch(...) {
        return E_ERROR;
    }

    return 0;
}

int CEPIController::dumpData(CString &f_csPath, const int &f_nDataform, int &f_nChannelSelection)
{
    double *l_dImageRawData = NULL;

    try {
        int l_nFileSize = 0;

        if(NULL != m_vcfHandle) {
            int l_nStatus = m_vcfHandle->dumpRawdata(f_csPath, f_nDataform, l_dImageRawData,
                            m_csImageFileName, l_nFileSize,
                            f_nChannelSelection);

            if(E_NO_ERROR != l_nStatus) {
                MEMFREE(l_dImageRawData);

                CEPILogHelper::WriteToErrorLog(_T("Failed to read raw data."), __LINE__, __FILE__);
                return l_nStatus;
            }

            if(NULL == l_dImageRawData) {
                CEPILogHelper::WriteToErrorLog(_T("Failed to read raw data."), __LINE__, __FILE__);
                return E_ERROR;
            }
        } else {
            return E_ERROR;
        }

        int l_nCount = m_csImageFileName.ReverseFind('_');
        int l_nTotalLen = m_csImageFileName.GetLength();
        CString l_csTemp = m_csImageFileName.Right(l_nTotalLen - (l_nCount + 1));
        l_nTotalLen = l_csTemp.GetLength();
        l_nCount = l_csTemp.Find('x');
        CString l_csWidth = l_csTemp.Left(l_nCount);
        CString l_csHeight = l_csTemp.Right(l_nTotalLen - (l_nCount + 1));
        char buff[32];
        sprintf(buff, "%s", l_csWidth);
        int l_nWidth = atoi(buff);
        sprintf(buff, "%s", l_csHeight);
        int l_nHeight = atoi(buff);

        int l_nBuffCount = (l_nFileSize / sizeof(double));

        InvertBuffer((void*&)l_dImageRawData,l_nBuffCount,VFT_FLT64);
        displayImage(l_dImageRawData, l_nHeight, l_nWidth);

        MEMFREE(l_dImageRawData);

    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in dumpData()."), __LINE__, __FILE__);
        MEMFREE(l_dImageRawData);
        return(E_ERROR);
    }

    return E_NO_ERROR;
}
int CEPIController::displayImage(double *&f_dImageRawData, int &f_nHeight, int f_nWidth)
{
    try {
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        // Get the active MDI child window.
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageViewer *pWnd = pChild->GetFormRightView();
        int l_nStatus = pWnd->displayImageStudyRaw(f_nWidth, f_nHeight, f_dImageRawData);

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("failed to display Raw data image."), __LINE__, __FILE__);
            return E_ERROR;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CEPIController::seperateTemplateAndKspace(int &f_nProtocolNo)
{
    void *l_cRawData = NULL;
    void *l_cTemplateDataBuffer1 = NULL;
    void *l_cTemplateDataBuffer2 = NULL;
    void *l_cKspaceDataBuffer = NULL;
    DUMPDATATYPE *l_dDumpDataK = NULL;
    DUMPDATATYPE *l_dDumpDataT1 = NULL;
    DUMPDATATYPE *l_dDumpDataT2  = NULL;
    long NoOfPoints =0;


    try {
        //int l_ProtIdx = f_nProtocolNo;
        int l_nSubProtNo;
        int l_nSubGrpNo;
        int l_nSliceNo;
        int l_nSliceImgNo;
        VfPathElem_t l_pathP[5] = { SVN_ST_PROTOCOL,   0, /* This path order should be maintained */
                                    SVN_ST_SUBPROTOCOL, 0,
                                    SVN_ST_SLICEGROUP, 0,
                                    SVN_ST_SLICE,      0,
                                    SVN_ST_IMAGE,      0
                                  };
        int l_nDataHeight = 0;
        int l_nDataWidth = 0;
        int l_nDataTypeSize = 0;
        int l_nNumRepetition = 0;
        CString l_csFilePath;

        int l_nStatus = m_vcfHandle->protocolCheckEPI(f_nProtocolNo);
        if(E_NO_ERROR != l_nStatus) {
            displayError(EPI_PROT_CHK);
            return E_ERROR;
        }
        m_nProtIndex = f_nProtocolNo;
        l_nStatus = m_vcfHandle->protTraverse(f_nProtocolNo,//IP
                                              l_nSubProtNo,//OP
                                              l_nSubGrpNo,//OP
                                              l_nSliceNo,//OP
                                              l_nSliceImgNo);//OP

        if(E_NO_ERROR != l_nStatus) {
            CString l_csError;
            l_csError.Format("failed to get subNodes for protocol %d.", f_nProtocolNo);
            CEPILogHelper::WriteToErrorLog(l_csError, __LINE__, __FILE__);
            return l_nStatus;
        }

        l_pathP[0].index = f_nProtocolNo;

        for(int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo; l_nSubProtCnt++) {
            l_pathP[1].index = l_nSubProtCnt;

            for(int l_nSubGrpCnt = 0; l_nSubGrpCnt < l_nSubGrpNo; l_nSubGrpCnt++) {
                l_pathP[2].index = l_nSubGrpCnt;

                for(int l_nSliceNoCnt = 0; l_nSliceNoCnt < l_nSliceNo; l_nSliceNoCnt++) {
                    l_pathP[3].index = l_nSliceNoCnt;

                    for(int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo; l_nSliceImgNoCnt++) {
                        int l_nMultiplier = 1;
                        l_pathP[4].index = l_nSliceImgNoCnt;
                        int l_nTotalChannel = m_vcfHandle->getTotalChannel();
                        m_imageWrite.createFolders(f_nProtocolNo,
                                                   l_nSubProtCnt,
                                                   l_nSubGrpCnt,
                                                   l_nSliceNoCnt,
                                                   l_nSliceImgNoCnt,
                                                   l_csFilePath);
                        l_csFilePath = l_csFilePath + _T("/Sep_K_T1_T2/");
                        l_nStatus = CreateDirectory(l_csFilePath, NULL);
                        l_nStatus = deleteFiles(l_csFilePath);

                        if(E_NO_ERROR != l_nStatus) {
                            CEPILogHelper::WriteToErrorLog(_T("Unable to delete files."));
                        }

                        m_csSeparateKT1T2DirPath = l_csFilePath;
                        long l_nImgRawDataSize = 0;

                        for(int l_nChannelNo = 0; l_nChannelNo < l_nTotalChannel; l_nChannelNo++) {
                            int temp = l_nChannelNo;
                            int l_nDataType = 0;
                            //------------------------------------------------------------------GET RAW DATA CHANNELWISE---------------------------------
                            l_nStatus = m_vcfHandle->getRawData(l_pathP, l_nDataHeight,
                                                                l_nDataWidth, l_nMultiplier,
                                                                temp, l_nDataTypeSize,
                                                                l_nNumRepetition, l_cRawData,
                                                                l_nImgRawDataSize, l_nDataType);

                            if(E_NO_ERROR != l_nStatus || l_nImgRawDataSize <=0 || l_nDataHeight <=0 || l_nDataWidth <=0  || l_nDataTypeSize <=0) {

                                CEPILogHelper::WriteToErrorLog(_T("Failed to get raw data from studyfile."), __LINE__, __FILE__);
                                CFormStatus *pWnd = getImageInfoInstance();
                                CString l_csErrorMsg(ERRORSTRING_EPI[l_nStatus]);
                                pWnd->setErrorMessage(l_csErrorMsg);
                                throw E_ERROR;
                            }
                            long memRequiredForImage = l_nImgRawDataSize / 3;
                            //------------------------------------------------------------------KSPACE MALLOC----------------------------------------------
                            l_cKspaceDataBuffer = (void*)malloc(memRequiredForImage);
                            if(NULL == l_cKspaceDataBuffer) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for KspaceData data.(for Separation)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }
                            ZeroMemory(l_cKspaceDataBuffer,memRequiredForImage);
                            //------------------------------------------------------------------T1 MALLOC---------------------------------------------------
                            l_cTemplateDataBuffer1 = (void*)malloc(memRequiredForImage);
                            if(NULL == l_cTemplateDataBuffer1) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for Template1 data.(for Separation)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }
                            ZeroMemory(l_cTemplateDataBuffer1,memRequiredForImage);
                            //------------------------------------------------------------------T2 MALLOC----------------------------------------------------
                            l_cTemplateDataBuffer2 = (void*)malloc(memRequiredForImage);
                            if(NULL == l_cTemplateDataBuffer2) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for Template2 data.(for Separation)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }                            
                            ZeroMemory(l_cTemplateDataBuffer2,memRequiredForImage);
                            //------------------------------------------------------------------SEPARATION----------------------------------------------------
                            CkspaceSeparator l_dataSaparate;
                            int l_nFlag = 0;
                            l_nStatus = l_dataSaparate.separateDataTemplateAndKspace((char*)l_cRawData,
                                        l_cKspaceDataBuffer,
                                        l_cTemplateDataBuffer1,
                                        l_cTemplateDataBuffer2,
                                        l_nDataWidth,
                                        l_nMultiplier,
                                        l_nDataHeight, l_nFlag,
                                        l_nNumRepetition,l_nDataTypeSize);
                            if(E_NO_ERROR != l_nStatus) {
                                CEPILogHelper::WriteToErrorLog(_T("Failed to Separate raw data from studyfile."), __LINE__, __FILE__);
                                CFormStatus *pWnd = getImageInfoInstance();
                                CString l_csErrorMsg(ERRORSTRING_EPI[l_nStatus]);
                                pWnd->setErrorMessage(l_csErrorMsg);
                                throw E_ERROR;
                            }
                            //------------------------------------------------------------------DUMPDATATYPE Size Calculate------------------------------------
                            NoOfPoints = (long) (memRequiredForImage/l_nDataTypeSize);
                            long l_nFloatWriteSize = 0;
                            l_nFloatWriteSize = (long)(NoOfPoints*sizeof(DUMPDATATYPE));
                            //------------------------------------------------------------------K-Space data writing----------------------------------------------------
                            m_csImageFileName.Format(_T(""));
                            m_csImageFileName.Format("Kspace_Channel_%d_Dim_%dx%d", l_nChannelNo, l_nDataWidth, (l_nDataHeight / 3));
                            CString l_csImagePathK = l_csFilePath + m_csImageFileName;
                            l_dDumpDataK = (DUMPDATATYPE*)malloc(l_nFloatWriteSize);                            
                            if(NULL == l_dDumpDataK) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for KspaceData data.(for float conversion)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }
                            ZeroMemory(l_dDumpDataK,l_nFloatWriteSize);
                            m_vcfHandle->vftoDUMPDATATYPE((void*)l_cKspaceDataBuffer, (void*)l_dDumpDataK, l_nDataType, NoOfPoints);
                            InvertBuffer((void*&)l_dDumpDataK,NoOfPoints,VFT_FLT64);
                            m_imageWrite.WriteToFile(l_csImagePathK, l_dDumpDataK, NoOfPoints);
                            //------------------------------------------------------------------Template1 data writing----------------------------------------------------
                            m_csImageFileName.Format("");
                            m_csImageFileName.Format("T1_Channel_%d_Dim_%dx%d", l_nChannelNo, l_nDataWidth, (l_nDataHeight / 3));
                            CString l_csImagePathT1 = l_csFilePath + m_csImageFileName;
                            l_dDumpDataT1 = (DUMPDATATYPE*)malloc(l_nFloatWriteSize); 
                            if(NULL == l_dDumpDataT1) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for Template1 data.(for float conversion)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }
                            ZeroMemory(l_dDumpDataT1,l_nFloatWriteSize);
                            m_vcfHandle->vftoDUMPDATATYPE((void*)l_cTemplateDataBuffer1, (void*)l_dDumpDataT1, l_nDataType, NoOfPoints);
                            InvertBuffer((void*&)l_dDumpDataT1,NoOfPoints,VFT_FLT64);
                            m_imageWrite.WriteToFile(l_csImagePathT1, l_dDumpDataT1, NoOfPoints);
                            //------------------------------------------------------------------Template2 data writing----------------------------------------------------
                            m_csImageFileName.Format("");
                            m_csImageFileName.Format("T2_Channel_%d_Dim_%dx%d", l_nChannelNo, l_nDataWidth, (l_nDataHeight / 3));
                            CString l_csImagePath = l_csFilePath + m_csImageFileName;
                            l_dDumpDataT2 = (DUMPDATATYPE*)malloc(l_nFloatWriteSize);
                            if(NULL == l_dDumpDataT2) {
                                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for Template2 data.(for float conversion)."), __LINE__, __FILE__);
                                throw E_ERROR;
                            }
                            ZeroMemory(l_dDumpDataT2,l_nFloatWriteSize);
                            m_vcfHandle->vftoDUMPDATATYPE((void*)l_cTemplateDataBuffer2, (void*)l_dDumpDataT2, l_nDataType, NoOfPoints);
                            InvertBuffer((void*&)l_dDumpDataT2,NoOfPoints,VFT_FLT64);
                            m_imageWrite.WriteToFile(l_csImagePath, l_dDumpDataT2, NoOfPoints);
                            //------------------------------------------------------------------CleanUP For Next Loop----------------------------------------------------
                            MEMFREE(l_dDumpDataK);
                            MEMFREE(l_dDumpDataT2);
                            MEMFREE(l_dDumpDataT1);
                            MEMFREE(l_cRawData);
                            MEMFREE(l_cKspaceDataBuffer);
                            MEMFREE(l_cTemplateDataBuffer1);
                            MEMFREE(l_cTemplateDataBuffer2);
                        }
                    }
                }
            }
        }
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();
        CString l_csTemp("Separate Data");
        pWnd->addStep(l_csTemp);
        pWnd->getFileList(m_csSeparateKT1T2DirPath);
        m_nSeparateKT1T2Flag = 1;
    } catch(...) {
        MEMFREE(l_dDumpDataK);
        MEMFREE(l_dDumpDataT2);
        MEMFREE(l_dDumpDataT1);
        MEMFREE(l_cRawData);
        MEMFREE(l_cKspaceDataBuffer);
        MEMFREE(l_cTemplateDataBuffer1);
        MEMFREE(l_cTemplateDataBuffer2);
        return E_ERROR;
    }
    return E_NO_ERROR;
}

int CEPIController::displayProtcol(int f_nmergeDataFlag)
{
    try {
        protInfo_t *l_protData = NULL;
        int l_nTotalProt = 0;
        m_vcfHandle->getProtocolInfo(l_protData, l_nTotalProt);
        (CPageEPI::GetPageOneInstance())->setListCtrlData(l_protData, l_nTotalProt,f_nmergeDataFlag);

        if(NULL != l_protData) {
            delete []l_protData;
            l_protData = NULL;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CEPIController::displayImagePath(int f_nProtIdx,int f_nFlag)
{
    try {
        int l_nTotalChannel = 0;
        m_nProtIndex = f_nProtIdx;
        int l_nStatus = getChannelCount(f_nProtIdx, l_nTotalChannel);

        if(E_NO_ERROR != l_nStatus) {
            return l_nStatus;
        }

        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();
        l_nStatus = pWnd->setListCtrlData(m_vcfHandle->getData(),
                                          m_vcfHandle->getTotalRawNodes(),
                                          f_nProtIdx, l_nTotalChannel);

        if (1 != m_nDisplayFlag)
        {
            if(E_NO_ERROR != l_nStatus) {
                CFormStatus *pInfoView = getImageInfoInstance();
                CString l_csError(ERRORSTRING_EPI[l_nStatus]);
                pInfoView->setErrorMessage(l_csError);
            }

        }
        if (1 == f_nFlag)
        {
            CFormImageSelect *pWndEPIFormView = pChild->GetEPIFormView();
            pWndEPIFormView->ClearSelectStepComboBox();
            CString l_csTemp("StudyFile Data");
            pWndEPIFormView->addStep(l_csTemp);
        }
        m_nSeparateEvenOddFlag = 0;
        m_nSeparateKT1T2Flag = 0;
        m_nPhaseCorrectionFlag = 0;
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CEPIController::loadImageStudyFile()
{
    try {
        int l_nStatus = E_NO_ERROR;
        l_nStatus = m_vcfHandle->CloseDBFile();

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("Unable to close previous study file."), __LINE__, __FILE__);
            return l_nStatus;
        }

        l_nStatus = m_vcfHandle->OpenDBFile();

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("VfOpen error."), __LINE__, __FILE__);
            return l_nStatus;
        }

        l_nStatus = m_vcfHandle->Disp_node_count();

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("Error occurred in vfc reading."), __LINE__, __FILE__);
            return l_nStatus;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CEPIController::getStudyImagePath()
{
    displayImagePath(m_nProtIndex,0);
    return E_NO_ERROR;
}

CString CEPIController::getSeparateKT1T2DirPath() const
{
    return m_csSeparateKT1T2DirPath;
}

int CEPIController::readFileDisplayImage(CString &f_csFileName, int &f_nDataForm,int f_nFlag)
{
    double * l_dImageBuff = NULL;
    double *l_dDataDumpBuff = NULL;

    try {
        int l_nCount = f_csFileName.ReverseFind('_');
        int l_nTotalLen = f_csFileName.GetLength();
        CString l_csTemp = f_csFileName.Right(l_nTotalLen - (l_nCount + 1));
        l_nTotalLen = l_csTemp.GetLength();
        l_nCount = l_csTemp.Find('x');
        CString l_csWidth = l_csTemp.Left(l_nCount);
        CString l_csHeight = l_csTemp.Right(l_nTotalLen - (l_nCount + 1));
        char buff[32];
        sprintf(buff, "%s", l_csWidth);
        int l_nWidth = atoi(buff);
        sprintf(buff, "%s", l_csHeight);
        int l_nHeight = atoi(buff);
		CString l_csFullFilePath;
        if (SEPARATE_EVEN_ODD == f_nFlag)
        {
            l_csFullFilePath = m_csSeparateKEvenOddDirPath + f_csFileName;
		}
		if(SEPARATE_KT1T2 == f_nFlag)
		{
          l_csFullFilePath = m_csSeparateKT1T2DirPath + f_csFileName;
		}
		if (FOLDER_PATH == f_nFlag)
		{
		  l_csFullFilePath = f_csFileName;
		}
        if (PHASE_CORRECTION == f_nFlag)
        {
            l_csFullFilePath = m_csPhaseCorrectionDirPath + f_csFileName;
        }

        CFileReader l_filreader;
        CString cs(l_csFullFilePath);
        CString l_csImagePath;
        int newImageSize = l_nHeight * l_nWidth * 2/** m_nKT1T2ImgSize*/;
        int l_nStatus = l_filreader.ReadImage(cs, newImageSize, l_dImageBuff);

        if(0 != l_nStatus) {
            MEMFREE(l_dImageBuff);
            return 1;
        }

        switch(f_nDataForm) {
            case MAGNITUDE:
                m_vcfHandle->MakeMagnitudeData(l_dImageBuff, newImageSize, l_dDataDumpBuff);
                break;
            case PHASE:
                m_vcfHandle->MakePhaseData(l_dImageBuff, newImageSize, l_dDataDumpBuff);
                break;
            case REAL:
                m_vcfHandle->MakeRealData(l_dImageBuff, newImageSize, l_dDataDumpBuff);
                break;
            case IMAGINARY:
                m_vcfHandle->MakeImaginaryData(l_dImageBuff, newImageSize, l_dDataDumpBuff);
                break;
            case NOFORM:
                break;
        }

        displayImage(l_dDataDumpBuff, l_nHeight, l_nWidth);

        DELARR(l_dDataDumpBuff);
        MEMFREE(l_dImageBuff);
    } catch(...) {
        DELARR(l_dDataDumpBuff);
        MEMFREE(l_dImageBuff);
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CEPIController::deleteFiles(CString &f_csFolderPath)
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    int l_nCnt_i = 0;
    CString l_csDirPath = f_csFolderPath + "*.*";
    CString l_csCompletePath("");
    hFind = FindFirstFile(l_csDirPath, &data);

    if(FALSE == SetFileAttributes(f_csFolderPath, FILE_ATTRIBUTE_NORMAL)) {
        CEPILogHelper::WriteToErrorLog(_T("deleteFiles(): SetFileAttributes Failed"), __LINE__);
    }

    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(strcmp(data.cFileName, ".") != 0
                    && strcmp(data.cFileName, "..") != 0) {
                printf("%s\n", data.cFileName);
                CString strItem(_T(""));
                strItem.Format(_T("%s"), data.cFileName);
                l_nCnt_i++;
                l_csCompletePath = f_csFolderPath + strItem;

                if(FALSE == SetFileAttributes(l_csCompletePath, FILE_ATTRIBUTE_NORMAL)) {
                    CEPILogHelper::WriteToErrorLog(_T("deleteFiles(): SetFileAttributes Failed"), __LINE__);
                }

                int l_nStatus = remove(l_csCompletePath);

                if(E_NO_ERROR != l_nStatus) {
                    //error
                }
            }
        } while(FindNextFile(hFind, &data));

        FindClose(hFind);
    }

    return 0;
}

CFormStatus * CEPIController::getImageInfoInstance()
{
    CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
    CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
    return pChild->getErrorInfoFormView();
}

int CEPIController::displayError(int f_nErrorCode)
{
    CString l_csErrorMsg(ERRORSTRING_EPI[f_nErrorCode]);
    CFormStatus *pwnd = getImageInfoInstance();
    pwnd->setErrorMessage(l_csErrorMsg);
    return E_NO_ERROR;
}

int CEPIController::dumpImageData(CString &f_csPath)
{
    try {
        double *l_dImageProcessedData = NULL;
        int l_nHeight = 0;
        int l_nWidth = 0;
        int l_nFileSize = 0;
        int l_nStatus = E_NO_ERROR;

        if(NULL != m_vcfHandle) {
            l_nStatus = m_vcfHandle->DumpProcessedData(f_csPath, l_nHeight, l_dImageProcessedData,
                        m_csImageFileName, l_nFileSize,
                        l_nWidth);

            if(E_NO_ERROR != l_nStatus) {
                if(l_dImageProcessedData) {
                    free(l_dImageProcessedData);
                    l_dImageProcessedData = NULL;
                }

                CEPILogHelper::WriteToErrorLog(_T("Failed to read Processed data."), __LINE__, __FILE__);
                return l_nStatus;
            }

            if(NULL == l_dImageProcessedData) {
                CEPILogHelper::WriteToErrorLog(_T("Failed to read Processed data."), __LINE__, __FILE__);
                return E_ERROR;
            }
        } else {
            return E_ERROR;
        }

        int l_nCount = m_csImageFileName.ReverseFind('_');
        int l_nTotalLen = m_csImageFileName.GetLength();
        CString l_csTemp = m_csImageFileName.Right(l_nTotalLen - (l_nCount + 1));
        l_nTotalLen = l_csTemp.GetLength();
        l_nCount = l_csTemp.Find('x');
        CString l_csWidth = l_csTemp.Left(l_nCount);
        CString l_csHeight = l_csTemp.Right(l_nTotalLen - (l_nCount + 1));
        l_nStatus = displayImage(l_dImageProcessedData, l_nHeight, l_nWidth);

        if(E_NO_ERROR != l_nStatus) {
            return(E_ERROR);
        }

        if(l_dImageProcessedData) {
            free(l_dImageProcessedData);
            l_dImageProcessedData = NULL;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
int CEPIController::saveOriginalRawData()
{
    try {
       int l_nStatus = E_NO_ERROR;
       CString l_csImagePath(_T(""));

        //check the selection of protocol        
        int rowSelected = CPageEPI::GetPageOneInstance()->m_RawDataList.GetSelectionMark();

        if(rowSelected < 0) {
            l_nStatus = EPI_SELECT_PROTOCOL;
            displayError(l_nStatus);
            return(E_ERROR);
        }

        //check selection of step 
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();

        int l_nSelPos = pWnd->m_ctrlComboStepSelection.GetCurSel();
        CString l_csSelStep;
        int l_nBuffCnt = pWnd->m_ctrlComboStepSelection.GetLBTextLen(l_nSelPos);
        pWnd->m_ctrlComboStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
        l_csSelStep.ReleaseBuffer();
        
        if(l_csSelStep.Compare("StudyFile Data") != 0) {
            l_nStatus = EPI_SELECT_STUDYFILEDATA_STEP;
            displayError(l_nStatus);
            return(E_ERROR);           
        }



        //check selection of raw data
        rowSelected = pWnd->m_ctrlRawDataList.GetSelectionMark();      
     
        if(rowSelected < 0) {
            l_nStatus = EPI_SELECT_RAW_DATA;
            displayError(l_nStatus);
            return(E_ERROR);
        }

        if(rowSelected > -1) {
            l_csImagePath = pWnd->m_ctrlRawDataList.GetItemText(rowSelected, 0);
        }

        if(l_csImagePath.IsEmpty() == TRUE) {
            pWnd->m_ctrlRawDataList.EndWaitCursor();
            l_nStatus = EPI_SELECT_RAW_DATA;
            displayError(l_nStatus);
            return(E_ERROR);
        }

        l_nStatus = ReadAndDumpOriginalRawData(l_csImagePath);

        if(E_NO_ERROR != l_nStatus) {
            displayError(l_nStatus);
        }
    } catch(...) {
        return E_ERROR;
    }
    int l_nStatus = EPI_RAW_DATA_SAVED;
    displayError(l_nStatus);
    return E_NO_ERROR;
}
int CEPIController::ReadAndDumpOriginalRawData(CString &f_csImagePath)
{
    void *l_dOrgImageRawData = NULL;

    try {
        int l_nStatus = E_NO_ERROR;
        int l_nProtocolNo = 0;
        int l_nSubProtCnt = 0;
        int l_nSubGrpCnt = 0;
        int l_nSliceNoCnt = 0;
        int l_nSliceImgNoCnt = 0;
        CString l_csFolderPath(_T(""));
        //### get selected image path for folder creation
        l_nStatus = getSelectedRawDataPath(f_csImagePath,
                                           l_nProtocolNo,
                                           l_nSubProtCnt,
                                           l_nSubGrpCnt,
                                           l_nSliceNoCnt,
                                           l_nSliceImgNoCnt,
                                           l_csFolderPath);

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get path for folder creation."));
            return(E_ERROR);
        }

        //### create the folder structure
        l_nStatus = m_imageWrite.createFolders(l_nProtocolNo,
                                               l_nSubProtCnt,
                                               l_nSubGrpCnt,
                                               l_nSliceNoCnt,
                                               l_nSliceImgNoCnt,
                                               l_csFolderPath);

        if(E_NO_ERROR != l_nStatus) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to create folder structure."));
            return(E_ERROR);
        }

        l_csFolderPath = l_csFolderPath + "Org_Raw_Data/";

        if(PathFileExists(l_csFolderPath)) {
            l_nStatus = deleteFiles(l_csFolderPath);

            if(E_NO_ERROR != l_nStatus) {
                CEPILogHelper::WriteToErrorLog(_T("Unable to delete files."));
            }
        } else {
            l_nStatus = CreateDirectory(l_csFolderPath, NULL);

            if(0  == l_nStatus) {
                CEPILogHelper::WriteToErrorLog(_T("Failed to create directory for Raw Data."));
                return(E_ERROR);
            }
        }

        //### read data from vfc write data to file
        if(NULL != m_vcfHandle) {
            //### get total num of channels
            int l_nTotalNumChannels = 0;
            int l_nTotalImgPix = 0;
            l_nStatus = getChannelCount(l_nProtocolNo, l_nTotalNumChannels);

            if(E_NO_ERROR != l_nStatus) {
                CEPILogHelper::WriteToErrorLog(_T("Failed to get total number of channels."));
                // return(E_ERROR);
                l_nTotalNumChannels = 1;
            }

            int l_nDataHeight = 0;
            int l_nDataWidth = 0;

            //### read org data
            for(int l_nChannel = 0; l_nChannel < l_nTotalNumChannels; l_nChannel++) {
                MEMFREE(l_dOrgImageRawData);

                //get original raw data for nth channel
                int l_datatype = VFT_INT32;
                l_nStatus = getOriginalRawdata(f_csImagePath, l_dOrgImageRawData, l_nChannel, l_nDataWidth, l_nDataHeight,l_datatype);

                if(E_NO_ERROR != l_nStatus || NULL == l_dOrgImageRawData) {
                    MEMFREE(l_dOrgImageRawData);
                    CEPILogHelper::WriteToErrorLog(_T("Failed to read raw data."), __LINE__, __FILE__);
                    return(E_ERROR);
                }
                //write original raw data for nth channel to a file
                CString l_csImageFileName(_T(""));
                CString l_csFinalPath(_T(""));
                l_csImageFileName.Format("RawData_Channel_%d_Dim_%dx%d", l_nChannel, l_nDataWidth, l_nDataHeight);
                l_csFinalPath = l_csFolderPath + l_csImageFileName;
                l_nTotalImgPix = (l_nDataWidth * 2) * l_nDataHeight;
                InvertBuffer(l_dOrgImageRawData,l_nTotalImgPix,l_datatype);
                switch(l_datatype){
                case VFT_INT16:
                    l_nStatus = m_imageWrite.WriteToFile(l_csFinalPath, (int16_t*)l_dOrgImageRawData, l_nTotalImgPix);                    
                    if(E_NO_ERROR != l_nStatus) {
                        MEMFREE(l_dOrgImageRawData);
                        CEPILogHelper::WriteToErrorLog(_T("Failed to write raw data."), __LINE__, __FILE__);
                        return(E_ERROR);
                    }

                    break;                
                case VFT_FLT32:
                    l_nStatus = m_imageWrite.WriteToFile(l_csFinalPath, (flt32_t*)l_dOrgImageRawData, l_nTotalImgPix);                    
                    if(E_NO_ERROR != l_nStatus) {
                        MEMFREE(l_dOrgImageRawData);
                        CEPILogHelper::WriteToErrorLog(_T("Failed to write raw data."), __LINE__, __FILE__);
                        return(E_ERROR);
                    }

                    break;
                case VFT_INT32:
                default:
                    l_nStatus = m_imageWrite.WriteToFile(l_csFinalPath, (int32_t*)l_dOrgImageRawData, l_nTotalImgPix);                    
                    if(E_NO_ERROR != l_nStatus) {
                        MEMFREE(l_dOrgImageRawData);
                        CEPILogHelper::WriteToErrorLog(_T("Failed to write raw data."), __LINE__, __FILE__);
                        return(E_ERROR);
                    }
                    break;
                }                
            }
        } else {
            CEPILogHelper::WriteToErrorLog(_T("Member found NULL."), __LINE__, __FILE__);
            return(E_ERROR);
        }

        MEMFREE(l_dOrgImageRawData);
    } catch(...) {
        MEMFREE(l_dOrgImageRawData);
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in ReadAndDumpOriginalRawData."), __LINE__, __FILE__);
        return(E_ERROR);
    }

    return(E_NO_ERROR);
}

int CEPIController::getSelectedRawDataPath(CString &f_csImagePath, int &f_nProtocolNo, int &f_nSubProtCnt, int &f_nSubGrpCnt, int &f_nSliceNoCnt, int &f_nSliceImgNoCnt, CString &f_csFilePath)
{
    try {
        if(TRUE == f_csImagePath.IsEmpty()) {
            CEPILogHelper::WriteToErrorLog(_T("Empty Image Path for RawData"), __LINE__, __FILE__);
            return(E_ERROR);
        }

        CString l_csOrgStr(_T(""));
        CString l_csTemp(_T(""));
        CString l_csExtractedStr(_T(""));
        int l_nPos = 0;
        l_csOrgStr = f_csImagePath;
        f_csFilePath.Empty();
        l_csTemp.Empty();
        l_csTemp = (_T("C:/EPI_Data/"));
        f_csFilePath = f_csFilePath + l_csTemp; //append epi folder name
        l_csTemp = m_imageWrite.getStudyName();
        f_csFilePath = f_csFilePath + l_csTemp;//append study name
        f_csFilePath = f_csFilePath + _T("/");
        l_nPos = l_csOrgStr.ReverseFind('/');

        if(-1 != l_nPos) {
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                f_csFilePath = f_csFilePath + l_csExtractedStr;
            } else {
                return(E_ERROR);
            }
        } else {
            return(E_ERROR);
        }

        f_csFilePath = f_csFilePath + _T("/");
        //### get protocol no.
        l_csExtractedStr.Empty();
        l_csTemp.Empty();
        l_nPos = 0;
        l_nPos = l_csOrgStr.Find('/');

        if(-1 != l_nPos) {  //if found
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                int l_nSecIdxPos = 0;
                l_nSecIdxPos = l_csExtractedStr.ReverseFind('.');
                l_csTemp = l_csExtractedStr.Right(l_csExtractedStr.GetLength() - (l_nSecIdxPos + 1));

                if(TRUE != l_csTemp.IsEmpty()) {
                    char buff[32];
                    sprintf(buff, "%s", l_csTemp);
                    f_nProtocolNo = atoi(buff);
                } else {
                    return(E_ERROR);
                }
            } else { //if empty
                return(E_ERROR);
            }

            l_csOrgStr = l_csOrgStr.Right(l_csOrgStr.GetLength() - (l_nPos + 1));
        } else { //if not found
            return(E_ERROR);
        }

        //### get sub-protocol count
        l_nPos = 0;
        l_csExtractedStr.Empty();
        l_csTemp.Empty();
        l_nPos = l_csOrgStr.Find('/');

        if(-1 != l_nPos) {  //if found
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                int l_nSecIdxPos = 0;
                l_nSecIdxPos = l_csExtractedStr.ReverseFind('.');
                l_csTemp = l_csExtractedStr.Right(l_csExtractedStr.GetLength() - (l_nSecIdxPos + 1));

                if(TRUE != l_csTemp.IsEmpty()) {
                    char buff[32];
                    sprintf(buff, "%s", l_csTemp);
                    f_nSubProtCnt = atoi(buff);
                } else {
                    return(E_ERROR);
                }
            } else { //if empty
                return(E_ERROR);
            }

            l_csOrgStr = l_csOrgStr.Right(l_csOrgStr.GetLength() - (l_nPos + 1));
        } else { //if not found
            return(E_ERROR);
        }

        //### get sub group count
        l_nPos = 0;
        l_csExtractedStr.Empty();
        l_csTemp.Empty();
        l_nPos = l_csOrgStr.Find('/');

        if(-1 != l_nPos) {  //if found
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                int l_nSecIdxPos = 0;
                l_nSecIdxPos = l_csExtractedStr.ReverseFind('.');
                l_csTemp = l_csExtractedStr.Right(l_csExtractedStr.GetLength() - (l_nSecIdxPos + 1));

                if(TRUE != l_csTemp.IsEmpty()) {
                    char buff[32];
                    sprintf(buff, "%s", l_csTemp);
                    f_nSubGrpCnt = atoi(buff);
                } else {
                    return(E_ERROR);
                }
            } else { //if empty
                return(E_ERROR);
            }

            l_csOrgStr = l_csOrgStr.Right(l_csOrgStr.GetLength() - (l_nPos + 1));
        } else { //if not found
            return(E_ERROR);
        }

        //### get slice no.
        l_nPos = 0;
        l_csExtractedStr.Empty();
        l_csTemp.Empty();
        l_nPos = l_csOrgStr.Find('/');

        if(-1 != l_nPos) {  //if found
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                int l_nSecIdxPos = 0;
                l_nSecIdxPos = l_csExtractedStr.ReverseFind('.');
                l_csTemp = l_csExtractedStr.Right(l_csExtractedStr.GetLength() - (l_nSecIdxPos + 1));

                if(TRUE != l_csTemp.IsEmpty()) {
                    char buff[32];
                    sprintf(buff, "%s", l_csTemp);
                    f_nSliceNoCnt = atoi(buff);
                } else {
                    return(E_ERROR);
                }
            } else { //if empty
                return(E_ERROR);
            }

            l_csOrgStr = l_csOrgStr.Right(l_csOrgStr.GetLength() - (l_nPos + 1));
        } else { //if not found
            return(E_ERROR);
        }

        //### get slice image count
        l_nPos = 0;
        l_csExtractedStr.Empty();
        l_csTemp.Empty();
        l_nPos = l_csOrgStr.Find('/');

        if(-1 != l_nPos) {  //if found
            l_csExtractedStr = l_csOrgStr.Left(l_nPos);

            if(TRUE != l_csExtractedStr.IsEmpty()) { // if not empty
                int l_nSecIdxPos = 0;
                l_nSecIdxPos = l_csExtractedStr.ReverseFind('.');
                l_csTemp = l_csExtractedStr.Right(l_csExtractedStr.GetLength() - (l_nSecIdxPos + 1));

                if(TRUE != l_csTemp.IsEmpty()) {
                    char buff[32];
                    sprintf(buff, "%s", l_csTemp);
                    f_nSliceImgNoCnt = atoi(buff);
                } else {
                    return(E_ERROR);
                }
            } else { //if empty
                return(E_ERROR);
            }

            l_csOrgStr = l_csOrgStr.Right(l_csOrgStr.GetLength() - (l_nPos + 1));
        } else { //if not found
            return(E_ERROR);
        }
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in getSelectedRawDataPath."), __LINE__, __FILE__);
        return(E_ERROR);
    }

    return(E_NO_ERROR);
}

int CEPIController::getOriginalRawdata(CString &f_csImagePath, 
                                       void *&f_dOrgImageRawData, 
                                       int l_nChannel, 
                                       int &f_nWidth, int &f_nHeight,
                                       int &f_datatype)
{
    double *l_dbuff = NULL;

    try {
        UNREFERENCED_PARAMETER(f_csImagePath);
        int l_nTotImgPix = 0;
        int read_dim = 0;
        double Image_scale = 0.0;
        int l_nStatus = E_NO_ERROR;
        f_datatype = VFT_INT32;

        if(NULL != m_vcfHandle) {
            l_dbuff = m_vcfHandle->ReadRawData(l_nTotImgPix , read_dim, Image_scale, f_datatype, TRUE, l_nChannel);

            if(NULL == l_dbuff) {
                CEPILogHelper::WriteToErrorLog(_T("Raw data reading failed."), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        f_nWidth = read_dim;
        f_nHeight = l_nTotImgPix / (read_dim * 2);
        l_nStatus = ConvertToDataType(l_dbuff, f_dOrgImageRawData, l_nTotImgPix, f_datatype,true);

        if(E_NO_ERROR != l_nStatus || NULL == f_dOrgImageRawData) {
            CEPILogHelper::WriteToErrorLog(_T("ConvertToDataType returned error."), __LINE__, __FILE__);
            MEMFREE(l_dbuff);
            MEMFREE(f_dOrgImageRawData);
            return(E_ERROR);
        }
        MEMFREE(l_dbuff);
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occured in getOriginalRawdata()"), __LINE__, __FILE__);
        MEMFREE(f_dOrgImageRawData);
        MEMFREE(l_dbuff);
        return(E_ERROR);
    }

    return(E_NO_ERROR);
}

int CEPIController::ConvertToDataType(double *f_DblData, void *&f_nIntData, int f_TotalPixels, int f_nDataType,bool f_Scaleflag)
{
    try {
        MEMFREE(f_nIntData);
        if(NULL == f_DblData){
            throw E_ERROR;
        }
        long l_nMallocSize = 0;
        switch(f_nDataType) {
        case VFT_INT16:
            {            
                int16_t Scale = SHRT_MAX;
                if(false == f_Scaleflag){
                    Scale = 1;
                }
                l_nMallocSize = f_TotalPixels * sizeof(int16_t);
                f_nIntData  = malloc(l_nMallocSize);
                if(NULL == f_nIntData || l_nMallocSize <= 0) {
                    throw(E_ERROR);
                }
                int16_t *tmp = (int16_t*)f_nIntData;
                memset(f_nIntData, 0x00, l_nMallocSize);
                for(int j = 0; j < f_TotalPixels; j++) {
                    tmp[j] = (int16_t)(f_DblData[j]* Scale);
                }      
            }
            break;
        case VFT_INT32:
            {
                int32_t Scale = INT_MAX;
                if(false == f_Scaleflag){
                    Scale = 1;
                }

                l_nMallocSize = f_TotalPixels * sizeof(int32_t);
                f_nIntData  = malloc(l_nMallocSize);
                if(NULL == f_nIntData || l_nMallocSize <= 0) {
                    throw(E_ERROR);
                }
                int32_t *tmp = (int32_t*)f_nIntData;
                memset(f_nIntData, 0x00, l_nMallocSize);
                for(int j = 0; j < f_TotalPixels; j++) {
                    tmp[j] = (int32_t)(f_DblData[j]* Scale);
                }
            }
            break;
        case VFT_FLT32:
            {            
                flt32_t Scale = FLT_MAX;
                if(false == f_Scaleflag){
                    Scale = 1;
                }
                l_nMallocSize = f_TotalPixels * sizeof(flt32_t);
                f_nIntData  = malloc(l_nMallocSize);
                if(NULL == f_nIntData || l_nMallocSize <= 0) {
                    throw(E_ERROR);
                }
                memset(f_nIntData, 0x00, l_nMallocSize);
                flt32_t *tmp = (flt32_t*)f_nIntData;
                for(int j = 0; j < f_TotalPixels; j++) {
                    tmp[j] = (flt32_t)(f_DblData[j]* Scale);
                }
            }
            break;
        default:
            CEPILogHelper::WriteToErrorLog(_T("Unknown data type.Default case executed\n"), __LINE__, __FILE__);
            MEMFREE(f_nIntData);
            return (E_ERROR);
        }    
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception Occurred in ConvertToInt."), __LINE__, __FILE__);
        MEMFREE(f_nIntData);
        return(E_ERROR);
    }

    return(E_NO_ERROR);
}

int CEPIController::mergeTemplateAndKspace()
{
    double *l_cKspaceDataBuffer = NULL;
    double *l_cT1DataBuffer = NULL;
    double *l_cT2DataBuffer = NULL;
    double *l_dFinalBuffer = NULL;
    void *l_nVfdataBuff = NULL;

    try{
        if(NULL == m_vcfHandle || NULL == m_vfcWriter){
            throw E_ERROR;
        }
        CScrPadHandler l_ScrPadObj;
        CString l_csStudyFile = m_vcfHandle->StudyFileName();
        int l_nDstProt = -1;
        int l_nStatus = l_ScrPadObj.createNewProtocol(l_csStudyFile,m_nProtIndex,l_nDstProt);
        if (l_nStatus != E_NO_ERROR || l_nDstProt < 0){
            throw E_ERROR;
        }
        m_nKsT1T2MergeProtIndex = l_nDstProt;
        l_nStatus = m_vfcWriter->CloseDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;
        }
        l_nStatus = m_vfcWriter->OpenDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;            
        }
        int l_nSubProtNo = 0;
        int l_nSubGrpNo = 0;
        int l_nSliceNo= 0;
        int l_nSliceImgNo = 0;
        VfPathElem_t l_pathP[5] = { SVN_ST_PROTOCOL,   0, /* This path order should be maintained */
            SVN_ST_SUBPROTOCOL, 0,
            SVN_ST_SLICEGROUP, 0,
            SVN_ST_SLICE,      0,
            SVN_ST_IMAGE,      0
        };
        CString l_csKSFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csT1FileName(_T("T1_Channel_0_Dim_128x66"));
        CString l_csT2FileName(_T("T2_Channel_0_Dim_128x66"));
        CString l_csStep(_T("Separate Data"));
        int l_nHeight = 0,l_nWidth = 0;
        CFileReader l_filreader;

        l_nStatus = m_vcfHandle->protTraverse(  m_nProtIndex,
                                                l_nSubProtNo,
                                                l_nSubGrpNo,
                                                l_nSliceNo,
                                                l_nSliceImgNo);
        if (E_NO_ERROR != l_nStatus){
            throw E_ERROR;
        }
        int l_nTotalChannel = m_vcfHandle->getTotalChannel();

        l_pathP[0].index = l_nDstProt;
        for (int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo;l_nSubProtCnt++)
        {
            l_pathP[1].index = l_nSubProtCnt;
            for (int l_nSubGrpCnt = 0;l_nSubGrpCnt < l_nSubGrpNo;l_nSubGrpCnt++)
            {
                l_pathP[2].index = l_nSubGrpCnt;
                
                for(int l_nSliceNoCnt = 0;l_nSliceNoCnt < l_nSliceNo;l_nSliceNoCnt++)
                {
                    l_pathP[3].index = l_nSliceNoCnt;
                    
                    for (int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo;l_nSliceImgNoCnt++)
                    {
                        for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                        {
                            l_pathP[4].index = l_nSliceImgNoCnt;
                            l_csKSFileName.Format("");
                            l_csT1FileName.Format("");
                            l_csT2FileName.Format("");
                            getFileName(m_csSeparateKT1T2DirPath,l_csStep,
                                l_nChanCnt,l_csKSFileName,
                                l_csT1FileName,l_csT2FileName);
                            getHeightWidth(l_csKSFileName,l_nWidth,l_nHeight);
                            if(l_nHeight <=0 || l_nWidth <= 0){
                                throw E_ERROR;
                            }
                            l_csKSFileName = m_csSeparateKT1T2DirPath + "\\" + l_csKSFileName;
                            l_csT1FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT1FileName;
                            l_csT2FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT2FileName;

                            int newImagepixels = (l_nWidth* 2) * l_nHeight ; //2  = real + imaginary
                            l_nStatus = l_filreader.ReadImage(l_csKSFileName,newImagepixels,l_cKspaceDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            l_nStatus = l_filreader.ReadImage(l_csT1FileName,newImagepixels,l_cT1DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            
                            l_nStatus = l_filreader.ReadImage(l_csT2FileName,newImagepixels,l_cT2DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }


                            long FinalBuffSize = (newImagepixels * 3)* sizeof(double);
                            l_dFinalBuffer = (double*)malloc(FinalBuffSize);

                            if (l_cKspaceDataBuffer == NULL ||
                                l_cT1DataBuffer == NULL ||
                                l_cT2DataBuffer == NULL ||
                                NULL == l_dFinalBuffer)
                            {
                                throw EPI_ERROR_NULL_FUNCPARAMS;
                            }
                            ZeroMemory(l_dFinalBuffer, FinalBuffSize);



                            CkspaceSeparator l_objSeparate;

                            l_nStatus =l_objSeparate.mergeDataTemplateAndKspace(   l_dFinalBuffer,
                                                                        l_cKspaceDataBuffer,
                                                                        l_cT1DataBuffer,
                                                                        l_cT2DataBuffer,
                                                                        l_nWidth,
                                                                        l_nHeight*3);
                            if (l_nStatus != E_NO_ERROR)
                            {                                
                                throw E_ERROR;
                            }
                            int l_nRepetitionTime = 0;
                            int l_nFinalDataForm = 0;
                            int l_nFinalDataType = 0;
                            int l_nFinalDataHeight = 0;
                            int l_nFinalDataWidth = 0;
                            l_nStatus = m_vcfHandle->getProtocolData(l_pathP,
                                l_nRepetitionTime,
                                l_nFinalDataForm,
                                l_nFinalDataType,
                                l_nFinalDataHeight,
                                l_nFinalDataWidth);
                            if (l_nStatus != E_NO_ERROR || l_nFinalDataHeight <= 0 || l_nFinalDataWidth <= 0)
                            {                                
                                throw E_ERROR;
                            }
                            int FinalTotalPixels = l_nFinalDataWidth * 2 * l_nFinalDataHeight;
                            l_nStatus = ConvertToDataType(l_dFinalBuffer,l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType,false);
                            if(l_nStatus != 0 || NULL == l_nVfdataBuff){
                                throw E_ERROR;
                            }
                            InvertBuffer(l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType);
                            int memRequiredForImage = 0;
                            switch(l_nFinalDataType) {
                            case VFT_INT16:
                                memRequiredForImage = FinalTotalPixels*sizeof(int16_t);
                                break;
                            case VFT_INT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(int32_t);
                                break;
                            case VFT_FLT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(flt32_t);
                                break;
                            default:
                                throw (EPI_ERROR);
                            }
                            l_nStatus = m_vfcWriter->PutData(l_nVfdataBuff,
                                                              l_pathP,memRequiredForImage,
                                                              l_nFinalDataType,l_nChanCnt);
                            if (l_nStatus != E_NO_ERROR )
                            {                                
                                throw E_ERROR;
                            }
                            DELARR(l_cKspaceDataBuffer);
                            DELARR(l_cT1DataBuffer);
                            DELARR(l_cT2DataBuffer);
                            DELARR(l_dFinalBuffer);
                            MEMFREE(l_nVfdataBuff);
                        }
                    }
                }
            }
        }

    }catch(...){
        DELARR(l_cKspaceDataBuffer);
        DELARR(l_cT1DataBuffer);
        DELARR(l_cT2DataBuffer);
        DELARR(l_dFinalBuffer);
        MEMFREE(l_nVfdataBuff);
        return E_ERROR;
    }
    return E_NO_ERROR;    
}

int CEPIController::getHeightWidth(CString &f_csFileName,int &f_nWidth,int &f_nHeight)
{
    int l_nCount = f_csFileName.ReverseFind('_');
    int l_nTotalLen = f_csFileName.GetLength();
    CString l_csTemp = f_csFileName.Right(l_nTotalLen - (l_nCount + 1));
    l_nTotalLen = l_csTemp.GetLength();
    l_nCount = l_csTemp.Find('x');
    CString l_csWidth = l_csTemp.Left(l_nCount);
    CString l_csHeight = l_csTemp.Right(l_nTotalLen - (l_nCount +1));
    char buff[32];
    sprintf(buff,"%s",l_csWidth);
    f_nWidth = atoi(buff);
    sprintf(buff,"%s",l_csHeight);
    f_nHeight = atoi(buff);
    return E_NO_ERROR;
}
int CEPIController::getFileName(CString f_csFolderPath,CString f_csStep,
                                int f_nChannel,CString &f_csKSpaceFile,
                                CString &f_csT1File,CString &f_csT2File)
{
    /*get file names*/
    HANDLE hFind;
    WIN32_FIND_DATA data;
    int l_nCnt_i = 0;
    CString l_csKSpace;
    CString l_csT1;
    CString l_csT2;
    if (f_csStep.Compare("Separate Data") == 0){
        l_csKSpace.Format("Kspace_Channel_%d",f_nChannel);
        l_csT1.Format("T1_Channel_%d",f_nChannel);
        l_csT2.Format("T2_Channel_%d",f_nChannel);
        
    }
    if (f_csStep.Compare("Separate Even Odd") == 0){
        l_csKSpace.Format("KEven_Channel_%d",f_nChannel);
        l_csT1.Format("KOdd_Channel_%d",f_nChannel);     
    }

    CString l_csDirPath = f_csFolderPath + "*.*";
    hFind = FindFirstFile(l_csDirPath, &data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if(strcmp(data.cFileName, ".") != 0
                && strcmp(data.cFileName, "..") != 0)
            {
                printf("%s\n", data.cFileName);
                CString strItem(_T(""));
                strItem.Format(_T("%s"), data.cFileName);
                if (strItem.Find(l_csKSpace) != -1)
                {
                    f_csKSpaceFile = strItem;
                }
                if (strItem.Find(l_csT1) != -1)
                {
                    f_csT1File = strItem;
                }
                if (strItem.Find(l_csT2) != -1)
                {
                    f_csT2File = strItem;
                }
                l_nCnt_i++;
            }
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    /*end of get file name*/
    return E_NO_ERROR;
}

int CEPIController::PerformRawCheck(CString &f_csStudyFileName,int &f_nProtocolNo)
{
    /*EPI_TRACE(CEPIController::rawCheckRecon,MRTraceMarker3);*/
    try{  
        status_t sts = E_NO_ERROR;
        
        msGetConfig_t  msConfig;
        msGetConfig(&msConfig);
        
        //RawCheck -start
        CReconRawCheck l_CrawCheck;
        CString l_csEngineName(msConfig.engineHostName);
        CString l_csStatusFilePath(RAWCHECK_STATUS_FILEPATH);
        
        if(TRUE != l_CrawCheck.RawCheck(
            l_csEngineName, 
            f_csStudyFileName, 
            f_nProtocolNo,
            l_csStatusFilePath)){
            
            return(EPI_RAW_CHECK_FAIL);        
        }
        
        //RawCheck -end
        
        int l_nrawCheckStatus = E_NO_ERROR;
        sts = GetRawCheckStatusFromFile(l_nrawCheckStatus);
        if(E_NO_ERROR != sts){
            CString l_csRawCheckErr =_T("RawCheck status Reading Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            return(EPI_RAW_CHECK_FAIL);
        }else{
            //implies file reading was successful and l_nrawCheckStatus has rawCheck status read from file
            //which either E_NO_ERROR or E_ERROR
            return(l_nrawCheckStatus);
        }    
    }catch(...){
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in PerformRawCheck()"),__LINE__,__FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);    
}

int CEPIController::GetRawCheckStatusFromFile( int &f_nStatus )
{
    // reads the file written on redirection of rawCheck's output 
    /*EPI_TRACE(CEPIController::GetRawCheckStatusFromFile,MRTraceMarker3)*/
        FILE * fp = fopen(RAWCHECK_STATUS_FILEPATH,"r");
    
    try{            
        if (fp != NULL)
        {
            char buf[126];
            fgets(buf, sizeof(buf),fp);
            
            if(buf[0] == '0')
            {
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("Raw data is present"));
                CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                
                //implies rawCheck exit status was success
                //i.e raw data is present
                f_nStatus = E_NO_ERROR;
                
            } else if(buf[0] == '1'){
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("Raw data is not present"));
                CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                //implies rawCheck exit status was failure
                //raw data is not present
                f_nStatus = EPI_NO_RAW;
            } else {
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("Raw Check return invalid status. Study file may not be present at engine"));
                CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                l_csTempstr = _T("error message from rawcheck process: \n");
                do {
                    l_csTempstr += buf;
                    l_csTempstr += _T("\n");
                }
                while ( NULL != fgets(buf, sizeof(buf),fp));
                CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                f_nStatus = EPI_RAW_CHECK_FAIL;
            }
        }else{            
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("Error Opening rawCheckStatus file"));
            CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
            f_nStatus = EPI_RAW_CHECK_FAIL;
            return(E_ERROR);
        }   
        CString l_csTempstr = _T("");
        l_csTempstr.Format(_T("rawCheck Status read from file is : %d"),f_nStatus);
        CEPILogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
        fclose(fp);
        DeleteRawCheckStatusFile();
        return(E_NO_ERROR);
        
    }catch(...)
    {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in GetRawCheckStatusFromFile()"),__LINE__,__FILE__);
        f_nStatus = E_ERROR;        
        if(fp){
            fclose(fp);
        };        
        return(E_ERROR);
    }
}

int CEPIController::DeleteRawCheckStatusFile()
{
    try{
        // deletes the file written on redirection of rawCheck's output 
        /*EPI_TRACE(CTiPrepController::DeleteRawCheckStatusFile,MRTraceMarker3)*/
        
        CString l_csFilePath(_T(""));
        l_csFilePath.Format(_T("%S"),RAWCHECK_STATUS_FILEPATH);
        
        if(FALSE == SetFileAttributes(l_csFilePath, FILE_ATTRIBUTE_NORMAL)) {
            CEPILogHelper::WriteToErrorLog(_T("DeleteRawCheckStatusFile(): RAWCHECK_STATUS_FILEPATH SetFileAttributes Failed"), __LINE__);
        }
        
        if(!(DeleteFile(l_csFilePath))) { // failed to delete
            CString l_csErrMsg(_T(""));
            l_csErrMsg.Format(_T("Failed to unlink file : %S.File may not be updated"), RAWCHECK_STATUS_FILEPATH);
            CEPILogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
        } else { // succeeded
            CString l_csErrMsg(_T(""));
            l_csErrMsg.Format(_T("File %S deleted successfully"), RAWCHECK_STATUS_FILEPATH);
            CEPILogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
        }
    }catch(...){
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in DeleteRawCheckStatusFile()"),__LINE__,__FILE__);
        return(E_ERROR);
        
    }
    return(E_NO_ERROR);        
}
int CEPIController::performRecon(CString &f_csStudyFile,int f_nProtIdx)
{
    CIRSListner m_IrsComm;
    int l_nStatus = E_NO_ERROR;
    //------------ Check system is up or not--------------------
    BOOL l_nstaFlag = m_IrsComm.InitializeIRS();   
    if (TRUE != l_nstaFlag){
        CString l_cserrmsg = (_T("Failed to Initialize IRS"));
        l_nStatus = EPI_IRS_FAIL;
        displayError(l_nStatus);
        CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);  
        return E_ERROR;
    }
    //------------- Check selected protocol have raw data or not---------------
    l_nStatus = PerformRawCheck(f_csStudyFile,f_nProtIdx);
    if (l_nStatus != E_NO_ERROR)
    {
        l_nStatus = EPI_RAW_CHECK_FAIL;
        displayError(l_nStatus);
        return E_ERROR;
    }
    m_IrsComm.setReconCalled(1);
    //-------------- Call reconF for reconstruction-------------------
    CReconProcess l_RconProcessObj;
    l_nStatus = l_RconProcessObj.performReconstruction(f_csStudyFile,f_nProtIdx,0);
    if (E_NO_ERROR != l_nStatus)
    {
        displayError(l_nStatus);
        return E_ERROR;
    }
    return E_NO_ERROR;
}

int CEPIController::ReRecon(int  f_nReconProtIdx)
{
    CString l_csStudyFile = m_vcfHandle->StudyFileName();
    if (l_csStudyFile.Compare("") == 0)
    {
        displayError(EPI_SELECT_PROTOCOL);
        return E_ERROR;
    }
    if (-1 == f_nReconProtIdx)
    {
        displayError(EPI_SELECT_PROTOCOL);
        return E_ERROR;
    }

    int l_nStatus = performRecon(l_csStudyFile,f_nReconProtIdx);
    if (l_nStatus != E_NO_ERROR)
    {
        return E_ERROR;
    }
    return E_NO_ERROR;
}

int CEPIController::InvertBuffer( void* &f_InBuf,int f_totalPixels,int f_DataType )
{
    int ComplexDataPoints = f_totalPixels/2;
    switch(f_DataType) {
    case VFT_INT16:
        {
            
            i16complex_t *In  = (i16complex_t*)f_InBuf;
            i16complex_t tmp;
            for(int j = 0; j < ComplexDataPoints; j++) {
                tmp.real = In[j].real;
                tmp.imgn = In[j].imgn;
                In[j].real = In[ComplexDataPoints-j-1].real;
                In[j].imgn = In[ComplexDataPoints-j-1].imgn;
                In[ComplexDataPoints-j-1].real = tmp.real;
                In[ComplexDataPoints-j-1].imgn = tmp.imgn;
            }      
        }
        break;
    case VFT_INT32:
        {
            
            i32complex_t *In  = (i32complex_t*)f_InBuf;
            i32complex_t tmp;
            for(int j = 0; j < ComplexDataPoints; j++) {
                tmp.real = In[j].real;
                tmp.imgn = In[j].imgn;
                In[j].real = In[ComplexDataPoints-j-1].real;
                In[j].imgn = In[ComplexDataPoints-j-1].imgn;
                In[ComplexDataPoints-j-1].real = tmp.real;
                In[ComplexDataPoints-j-1].imgn = tmp.imgn;
            }      
        }
        break;
    case VFT_FLT32:
        {            
            
            f32complex_t *In  = (f32complex_t*)f_InBuf;
            f32complex_t tmp;
            for(int j = 0; j < f_totalPixels/2; j++) {
                tmp.real = In[j].real;
                tmp.imgn = In[j].imgn;
                In[j].real = In[ComplexDataPoints-j-1].real;
                In[j].imgn = In[ComplexDataPoints-j-1].imgn;
                In[ComplexDataPoints-j-1].real = tmp.real;
                In[ComplexDataPoints-j-1].imgn = tmp.imgn;
            }      
        }
        break;
    case VFT_FLT64:
        {            
            
            f64complex_t *In  = (f64complex_t*)f_InBuf;
            f64complex_t tmp;
            for(int j = 0; j < f_totalPixels/2; j++) {
                tmp.real = In[j].real;
                tmp.imgn = In[j].imgn;
                In[j].real = In[ComplexDataPoints-j-1].real;
                In[j].imgn = In[ComplexDataPoints-j-1].imgn;
                In[ComplexDataPoints-j-1].real = tmp.real;
                In[ComplexDataPoints-j-1].imgn = tmp.imgn;
            }      
        }
        break;

    default:
        return (E_ERROR);
        } 
    return E_NO_ERROR;
}

int CEPIController::mergeData(CString f_csSelStep,int & f_ndstProt)
{
    int l_nStatus = E_NO_ERROR;
    CString l_csStudyFile = m_vcfHandle->StudyFileName();
    if (l_csStudyFile.Compare("") == 0)
    {
        displayError(EPI_SELECT_PROTOCOL);
        return E_ERROR;
    }
    if (-1 == m_nProtIndex)
    {
        displayError(EPI_SELECT_PROTOCOL);
        return E_ERROR;
    }
    if(f_csSelStep.Compare("Separate Data") == 0) {
        l_nStatus = mergeTemplateAndKspace();
        if (E_NO_ERROR != l_nStatus)
        {
            displayError(EPI_MERGE_FAIL);
            return E_ERROR;
        }
    }

    if(f_csSelStep.Compare("Separate Even Odd") == 0) {
        l_nStatus = mergeEvenOddData();
        if (E_NO_ERROR != l_nStatus)
        {
            displayError(EPI_MERGE_FAIL);
            return E_ERROR;
        }
    }
    
    if(f_csSelStep.Compare("Phase Correction") == 0) {
        l_nStatus = mergePhaseCorrectedData();
        if (E_NO_ERROR != l_nStatus)
        {
            displayError(EPI_MERGE_FAIL);
            return E_ERROR;
        }
    }
    

    l_nStatus = loadStudy(1);
    if (E_NO_ERROR != l_nStatus)
    {
        return E_ERROR;
    }
	f_ndstProt = m_nKsT1T2MergeProtIndex;
    return E_NO_ERROR;
}

void CEPIController::SendMsgToView()
{
    ((CWnd*)(g_view))->PostMessage(WM_EPI_RELOAD,NULL,NULL);
}

int CEPIController::separateEvenOdd()
{
    double *l_cKspaceDataBuffer = NULL;
    double *l_cKspaceEvenDataBuffer = NULL;
    double *l_cKspaceOddDataBuffer = NULL;
	int l_nstatus = 0;
    try
    {
		if (m_vcfHandle) {
			l_nstatus = m_vcfHandle->rawDataCheck();
			if (l_nstatus != E_NO_ERROR)
			{
				displayError(l_nstatus);
				return E_ERROR;
			}
		}
        if (1 != m_nSeparateKT1T2Flag)
        {
            displayError(EPI_PERFORM_SEPARATE);
            return E_ERROR;
        }
        if (-1 == m_nProtIndex)
        {
            displayError(EPI_SELECT_PROTOCOL);
            return E_ERROR;
        }

        int l_nSubProtNo = 0;
        int l_nSubGrpNo = 0;
        int l_nSliceNo= 0;
        int l_nSliceImgNo = 0;
        CString l_csKSFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csT1FileName(_T("T1_Channel_0_Dim_128x66"));
        CString l_csT2FileName(_T("T2_Channel_0_Dim_128x66"));
        CString l_csStep(_T("Separate Data"));
        int l_nHeight = 0,l_nWidth = 0;
        CFileReader l_filreader;
        CString l_csFilePath;
        CEvenOddSeparator l_evenOddSeparate;
        int l_nStatus = m_vcfHandle->protTraverse(  m_nProtIndex,
            l_nSubProtNo,
            l_nSubGrpNo,
            l_nSliceNo,
            l_nSliceImgNo);
        if (E_NO_ERROR != l_nStatus){
            throw E_ERROR;
        }

        int l_nTotalChannel = m_vcfHandle->getTotalChannel();
        for (int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo;l_nSubProtCnt++)
        {
            for (int l_nSubGrpCnt = 0;l_nSubGrpCnt < l_nSubGrpNo;l_nSubGrpCnt++)
            {                
                for(int l_nSliceNoCnt = 0;l_nSliceNoCnt < l_nSliceNo;l_nSliceNoCnt++)
                {
                  
                    for (int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo;l_nSliceImgNoCnt++)
                    {
                        m_imageWrite.createFolders(m_nProtIndex,
                                                    l_nSubProtCnt,
                                                    l_nSubGrpCnt,
                                                    l_nSliceNoCnt,
                                                    l_nSliceImgNoCnt,
                                                    l_csFilePath);
                        l_csFilePath = l_csFilePath + _T("/Sep_K_Even_Odd/");
                        m_csSeparateKEvenOddDirPath = l_csFilePath;
                        l_nStatus = CreateDirectory(l_csFilePath, NULL);
                        l_nStatus = deleteFiles(l_csFilePath);

                        for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                        {
                            l_csKSFileName.Format("");
                            l_csT1FileName.Format("");
                            l_csT2FileName.Format("");
                            getFileName(m_csSeparateKT1T2DirPath,l_csStep,
                                l_nChanCnt,l_csKSFileName,
                                l_csT1FileName,l_csT2FileName);
                            getHeightWidth(l_csKSFileName,l_nWidth,l_nHeight);
                            if(l_nHeight <=0 || l_nWidth <= 0){
                                throw E_ERROR;
                            }
                            l_csKSFileName = m_csSeparateKT1T2DirPath + "\\" + l_csKSFileName;
                            int newImagepixels = (l_nWidth* 2) * l_nHeight ; //2  = real + imaginary
                            l_nStatus = l_filreader.ReadImage(l_csKSFileName,
                                                              newImagepixels,
                                                              l_cKspaceDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            int l_nImgPix = l_nWidth * (l_nHeight/2) * 2;
                            l_cKspaceEvenDataBuffer = new double[l_nImgPix];
                            l_cKspaceOddDataBuffer = new double[l_nImgPix];
                            if (NULL == l_cKspaceEvenDataBuffer ||
                                NULL == l_cKspaceOddDataBuffer)
                            {
                                DELARR(l_cKspaceDataBuffer);
                                DELARR(l_cKspaceEvenDataBuffer);
                                DELARR(l_cKspaceOddDataBuffer);
                                return E_ERROR;
                            }
                            l_evenOddSeparate.separateDataEvenOdd(l_cKspaceDataBuffer,
                                                                  l_cKspaceEvenDataBuffer,
                                                                  l_cKspaceOddDataBuffer,
                                                                  l_nWidth,l_nHeight);

                            int NoOfPoints = l_nWidth * (l_nHeight/2) * 2;
                            m_csImageFileName.Format("");
                            m_csImageFileName.Format("KEven_Channel_%d_Dim_%dx%d",l_nChanCnt,l_nWidth,(l_nHeight/2));
                            CString l_csImagePathKEven = l_csFilePath + m_csImageFileName;
                            m_imageWrite.WriteToFile(l_csImagePathKEven,
                                                     l_cKspaceEvenDataBuffer,
                                                     NoOfPoints);
                            m_csImageFileName.Format("");
                            m_csImageFileName.Format("KOdd_Channel_%d_Dim_%dx%d",l_nChanCnt,l_nWidth,(l_nHeight/2));
                            CString l_csImagePathKOdd = l_csFilePath + m_csImageFileName;
                            m_imageWrite.WriteToFile(l_csImagePathKOdd,
                                l_cKspaceEvenDataBuffer,
                                NoOfPoints);
                            DELARR(l_cKspaceDataBuffer);
                            DELARR(l_cKspaceEvenDataBuffer);
                            DELARR(l_cKspaceOddDataBuffer);
                        }
                    }
                }
            }
        }
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();
        CString l_csTemp("Separate Even Odd");
        pWnd->addStep(l_csTemp);
        pWnd->getFileList(m_csSeparateKEvenOddDirPath);
        m_nSeparateEvenOddFlag = 1;
    }
    catch (...)
    {
        DELARR(l_cKspaceDataBuffer);
        DELARR(l_cKspaceEvenDataBuffer);
        DELARR(l_cKspaceOddDataBuffer);
        return E_ERROR; 
    }
    return E_NO_ERROR;
}

CString CEPIController::getSeparateKEvenOddDirPath() const
{
    return m_csSeparateKEvenOddDirPath;
}

int CEPIController::mergeEvenOddData()
{
    double *l_cKspaceDataBuffer = NULL;
    double *l_cKspaceEvenDataBuffer = NULL;
    double *l_cKspaceOddDataBuffer = NULL;
    double *l_cT1DataBuffer = NULL;
    double *l_cT2DataBuffer = NULL;
    double *l_dFinalBuffer = NULL;
    void *l_nVfdataBuff = NULL;

    try{
        if(NULL == m_vcfHandle || NULL == m_vfcWriter){
            throw E_ERROR;
        }
        CScrPadHandler l_ScrPadObj;
        CString l_csStudyFile = m_vcfHandle->StudyFileName();
        int l_nDstProt = -1;
        int l_nStatus = l_ScrPadObj.createNewProtocol(l_csStudyFile,m_nProtIndex,l_nDstProt);
        if (l_nStatus != E_NO_ERROR || l_nDstProt < 0){
            throw E_ERROR;
        }
        m_nKsT1T2MergeProtIndex = l_nDstProt;
        l_nStatus = m_vfcWriter->CloseDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;
        }
        l_nStatus = m_vfcWriter->OpenDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;            
        }
        int l_nSubProtNo = 0;
        int l_nSubGrpNo = 0;
        int l_nSliceNo= 0;
        int l_nSliceImgNo = 0;
        VfPathElem_t l_pathP[5] = { SVN_ST_PROTOCOL,   0, /* This path order should be maintained */
            SVN_ST_SUBPROTOCOL, 0,
            SVN_ST_SLICEGROUP, 0,
            SVN_ST_SLICE,      0,
            SVN_ST_IMAGE,      0
        };
        CString l_csKSEvenFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csKSOddFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csKSFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csT1FileName(_T("T1_Channel_0_Dim_128x66"));
        CString l_csT2FileName(_T("T2_Channel_0_Dim_128x66"));
        CString l_csStep(_T("Separate Data"));
        int l_nHeight = 0,l_nWidth = 0;
        CFileReader l_filreader;

        l_nStatus = m_vcfHandle->protTraverse(  m_nProtIndex,
                                                l_nSubProtNo,
                                                l_nSubGrpNo,
                                                l_nSliceNo,
                                                l_nSliceImgNo);
        if (E_NO_ERROR != l_nStatus){
            throw E_ERROR;
        }
        int l_nTotalChannel = m_vcfHandle->getTotalChannel();
        CEvenOddSeparator l_evenOddSeparate;
        l_pathP[0].index = l_nDstProt;
        for (int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo;l_nSubProtCnt++)
        {
            l_pathP[1].index = l_nSubProtCnt;
            for (int l_nSubGrpCnt = 0;l_nSubGrpCnt < l_nSubGrpNo;l_nSubGrpCnt++)
            {
                l_pathP[2].index = l_nSubGrpCnt;
                
                for(int l_nSliceNoCnt = 0;l_nSliceNoCnt < l_nSliceNo;l_nSliceNoCnt++)
                {
                    l_pathP[3].index = l_nSliceNoCnt;
                    
                    for (int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo;l_nSliceImgNoCnt++)
                    {
                        for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                        {
                            l_pathP[4].index = l_nSliceImgNoCnt;
                            l_csKSFileName.Format("");
                            l_csT1FileName.Format("");
                            l_csT2FileName.Format("");
                            l_csKSEvenFileName.Format("");
                            l_csKSOddFileName.Format("");
                            CString l_csTemp;
                            getFileName(m_csSeparateKEvenOddDirPath,"Separate Even Odd",
                                l_nChanCnt,l_csKSEvenFileName,
                                l_csKSOddFileName,l_csTemp);
                            getFileName(m_csSeparateKT1T2DirPath,l_csStep,
                                l_nChanCnt,l_csKSFileName,
                                l_csT1FileName,l_csT2FileName);
                            int l_nHeightEO;
                            int l_nWidthEO;
                            getHeightWidth(l_csKSEvenFileName,l_nWidthEO,l_nHeightEO);
                            getHeightWidth(l_csKSFileName,l_nWidth,l_nHeight);
                            if(l_nHeight <=0 || l_nWidth <= 0 || l_nWidthEO <= 0 || l_nHeightEO <= 0){
                                throw E_ERROR;
                            }
                            l_csKSEvenFileName = m_csSeparateKEvenOddDirPath + "\\" + l_csKSEvenFileName;
                            l_csKSOddFileName = m_csSeparateKEvenOddDirPath + "\\" + l_csKSOddFileName;

                            l_csKSFileName = m_csSeparateKT1T2DirPath + "\\" + l_csKSFileName;
                            l_csT1FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT1FileName;
                            l_csT2FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT2FileName;
                            
                            int newImagepixels = (l_nWidthEO* 2) * l_nHeightEO ; //2  = real + imaginary

                            l_nStatus = l_filreader.ReadImage(l_csKSEvenFileName,newImagepixels,l_cKspaceEvenDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }

                            l_nStatus = l_filreader.ReadImage(l_csKSOddFileName,newImagepixels,l_cKspaceOddDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            int l_nImgPix = l_nWidth * l_nHeight * 2;
                            l_cKspaceDataBuffer = new double[l_nImgPix];
                            l_evenOddSeparate.mergeDataEvenOdd(l_cKspaceDataBuffer,
                                                             l_cKspaceEvenDataBuffer,
                                                             l_cKspaceOddDataBuffer,
                                                             l_nWidth,l_nHeight);



                            newImagepixels = (l_nWidth* 2) * l_nHeight ; //2  = real + imaginary
                            l_nStatus = l_filreader.ReadImage(l_csT1FileName,newImagepixels,l_cT1DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            
                            l_nStatus = l_filreader.ReadImage(l_csT2FileName,newImagepixels,l_cT2DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }


                            long FinalBuffSize = (newImagepixels * 3)* sizeof(double);
                            l_dFinalBuffer = (double*)malloc(FinalBuffSize);

                            if (l_cKspaceDataBuffer == NULL ||
                                l_cT1DataBuffer == NULL ||
                                l_cT2DataBuffer == NULL ||
                                NULL == l_dFinalBuffer)
                            {
                                throw EPI_ERROR_NULL_FUNCPARAMS;
                            }
                            ZeroMemory(l_dFinalBuffer, FinalBuffSize);



                            CkspaceSeparator l_objSeparate;

                            l_nStatus =l_objSeparate.mergeDataTemplateAndKspace(l_dFinalBuffer,
                                                                        l_cKspaceDataBuffer,
                                                                        l_cT1DataBuffer,
                                                                        l_cT2DataBuffer,
                                                                        l_nWidth,
                                                                        l_nHeight*3);
                            if (l_nStatus != E_NO_ERROR)
                            {                                
                                throw E_ERROR;
                            }
                            int l_nRepetitionTime = 0;
                            int l_nFinalDataForm = 0;
                            int l_nFinalDataType = 0;
                            int l_nFinalDataHeight = 0;
                            int l_nFinalDataWidth = 0;
                            l_nStatus = m_vcfHandle->getProtocolData(l_pathP,
                                l_nRepetitionTime,
                                l_nFinalDataForm,
                                l_nFinalDataType,
                                l_nFinalDataHeight,
                                l_nFinalDataWidth);
                            if (l_nStatus != E_NO_ERROR || l_nFinalDataHeight <= 0 || l_nFinalDataWidth <= 0)
                            {                                
                                throw E_ERROR;
                            }
                            int FinalTotalPixels = l_nFinalDataWidth * 2 * l_nFinalDataHeight;
                            l_nStatus = ConvertToDataType(l_dFinalBuffer,l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType,false);
                            if(l_nStatus != 0 || NULL == l_nVfdataBuff){
                                throw E_ERROR;
                            }
                            InvertBuffer(l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType);
                            int memRequiredForImage = 0;
                            switch(l_nFinalDataType) {
                            case VFT_INT16:
                                memRequiredForImage = FinalTotalPixels*sizeof(int16_t);
                                break;
                            case VFT_INT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(int32_t);
                                break;
                            case VFT_FLT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(flt32_t);
                                break;
                            default:
                                throw (EPI_ERROR);
                            }
                            l_nStatus = m_vfcWriter->PutData(l_nVfdataBuff,
                                                              l_pathP,memRequiredForImage,
                                                              l_nFinalDataType,l_nChanCnt);
                            if (l_nStatus != E_NO_ERROR )
                            {                                
                                throw E_ERROR;
                            }
                            DELARR(l_cKspaceDataBuffer);
                            DELARR(l_cT1DataBuffer);
                            DELARR(l_cT2DataBuffer);
                            DELARR(l_dFinalBuffer);
                            DELARR(l_cKspaceEvenDataBuffer);
                            DELARR(l_cKspaceOddDataBuffer);
                            MEMFREE(l_nVfdataBuff);
                        }
                    }
                }
            }
        }

    }catch(...){
        DELARR(l_cKspaceDataBuffer);
        DELARR(l_cT1DataBuffer);
        DELARR(l_cT2DataBuffer);
        DELARR(l_dFinalBuffer);
        DELARR(l_cKspaceEvenDataBuffer);
        DELARR(l_cKspaceOddDataBuffer);
        MEMFREE(l_nVfdataBuff);
        return E_ERROR;
    }
    return E_NO_ERROR;    
}

int CEPIController::correctPhase(int f_nPhaseMethodSelection,
                                 int f_nTemplateSelFlag)
{
    double *l_dKspaceEvenDataBuffer = NULL;
    double *l_dKspaceOddDataBuffer = NULL;
    double *l_dTempEvenDataBuffer = NULL;
    double *l_dTempOddDataBuffer = NULL;
    double *l_dTemplateDataBuffer = NULL;

    try{
        if (1 != m_nSeparateEvenOddFlag)
        {
            displayError(EPI_PERFORM_SEP_EVEN_ODD);
            return E_ERROR;
        }
        if (-1 == m_nProtIndex)
        {
            displayError(EPI_SELECT_PROTOCOL);
            return E_ERROR;
        }
        int l_nSubProtNo = 0;
        int l_nSubGrpNo = 0;
        int l_nSliceNo= 0;
        int l_nSliceImgNo = 0;
        CString l_csKSFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csT1FileName(_T("T1_Channel_0_Dim_128x66"));
        CString l_csT2FileName(_T("T2_Channel_0_Dim_128x66"));
        CString l_csKSEvenFileName("");
        CString l_csKSOddFileName("");
        CString l_csStep(_T("Separate Data"));
        int l_nHeight = 0,l_nWidth = 0;
        CFileReader l_filreader;
        CString l_csFilePath;
        CEvenOddSeparator l_evenOddSeparate;
        int l_nStatus = m_vcfHandle->protTraverse(m_nProtIndex,
                                                  l_nSubProtNo,
                                                  l_nSubGrpNo,
                                                  l_nSliceNo,
                                                  l_nSliceImgNo);
        if (E_NO_ERROR != l_nStatus){
            throw E_ERROR;
        }        
        int l_nTotalChannel = m_vcfHandle->getTotalChannel();
        for (int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo;l_nSubProtCnt++)
        {
            for (int l_nSubGrpCnt = 0;l_nSubGrpCnt < l_nSubGrpNo;l_nSubGrpCnt++)
            {                
                for(int l_nSliceNoCnt = 0;l_nSliceNoCnt < l_nSliceNo;l_nSliceNoCnt++)
                {
                    
                    for (int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo;l_nSliceImgNoCnt++)
                    {                        
                        for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                        {
                            createPhaseCorrectedDir(m_nProtIndex,
                                                    l_nSubProtCnt,
                                                    l_nSubGrpCnt,
                                                    l_nSliceNoCnt,
                                                    l_nSliceImgNoCnt,
                                                    f_nPhaseMethodSelection,
                                                    l_csFilePath);
                            for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                            {
                                l_csKSFileName.Format("");
                                l_csT1FileName.Format("");
                                l_csT2FileName.Format("");
                                l_csKSEvenFileName.Format("");
                                l_csKSOddFileName.Format("");
                                CString l_csTemp;
                                getFileName(m_csSeparateKEvenOddDirPath,"Separate Even Odd",
                                    l_nChanCnt,l_csKSEvenFileName,
                                    l_csKSOddFileName,l_csTemp);
                                getFileName(m_csSeparateKT1T2DirPath,l_csStep,
                                    l_nChanCnt,l_csKSFileName,
                                    l_csT1FileName,l_csT2FileName);
                                int l_nHeightEO;
                                int l_nWidthEO;
                                getHeightWidth(l_csKSEvenFileName,l_nWidthEO,l_nHeightEO);
                                getHeightWidth(l_csKSFileName,l_nWidth,l_nHeight);
                                if(l_nHeight <=0 || l_nWidth <= 0 || l_nWidthEO <= 0 || l_nHeightEO <= 0){
                                    throw E_ERROR;
                                }
                                l_csKSEvenFileName = m_csSeparateKEvenOddDirPath  + l_csKSEvenFileName;
                                l_csKSOddFileName = m_csSeparateKEvenOddDirPath  + l_csKSOddFileName;                                
                                l_csKSFileName = m_csSeparateKT1T2DirPath + l_csKSFileName;
                                l_csT1FileName = m_csSeparateKT1T2DirPath + l_csT1FileName;
                                l_csT2FileName = m_csSeparateKT1T2DirPath + l_csT2FileName;

                                int l_nTempImagepixels = (l_nWidth* 2) * l_nHeight ; //2  = real + imaginary
                                if (T1_SELECTION == f_nTemplateSelFlag)
                                {
                                    l_nStatus = l_filreader.ReadImage(l_csT1FileName,
                                                                      l_nTempImagepixels,
                                                                      l_dTemplateDataBuffer);
                                }
                                if (T2_SELECTION == f_nTemplateSelFlag)
                                {
                                    l_nStatus = l_filreader.ReadImage(l_csT2FileName,
                                                                      l_nTempImagepixels,
                                                                      l_dTemplateDataBuffer);                                    
                                }
                                if (E_NO_ERROR != l_nStatus)
                                {
                                    throw E_ERROR;
                                }
                                int l_nImgPix = l_nWidth * (l_nHeight/2) * 2;
                                l_dTempEvenDataBuffer = new double[l_nImgPix];
                                l_dTempOddDataBuffer = new double[l_nImgPix];
                                if (NULL == l_dTempEvenDataBuffer ||
                                    NULL == l_dTempOddDataBuffer)
                                {
                                    throw E_ERROR;
                                }
                                l_evenOddSeparate.separateDataEvenOdd(l_dTemplateDataBuffer,
                                                                      l_dTempEvenDataBuffer,
                                                                      l_dTempOddDataBuffer,
                                                                      l_nWidth,l_nHeight);
                                int l_nKsEOImagepixels = (l_nWidthEO* 2) * l_nHeightEO ; //2  = real + imaginary
                                l_nStatus = l_filreader.ReadImage(l_csKSEvenFileName,
                                                                 l_nKsEOImagepixels,
                                                                 l_dKspaceEvenDataBuffer);                                    
                                if (E_NO_ERROR != l_nStatus)
                                {
                                    throw E_ERROR;
                                }

                                l_nStatus = l_filreader.ReadImage(l_csKSOddFileName,
                                                                  l_nKsEOImagepixels,
                                                                  l_dKspaceOddDataBuffer);                                    
                                if (E_NO_ERROR != l_nStatus)
                                {
                                    throw E_ERROR;
                                }
                                CPhaseCorrectionStrategy l_phaseCorrectionObj;
                                l_nStatus = l_phaseCorrectionObj.performPhaseCorrection((f64complex_t*)l_dTempEvenDataBuffer,
                                                                            l_nHeightEO,l_nWidthEO,
                                                                            f_nPhaseMethodSelection,
                                                                            (f64complex_t*)l_dKspaceEvenDataBuffer);
                                if (E_NO_ERROR != l_nStatus)
                                {
                                    throw E_ERROR;
                                }
                                l_nStatus = l_phaseCorrectionObj.performPhaseCorrection((f64complex_t*)l_dTempOddDataBuffer,
                                                                                        l_nHeightEO,l_nWidthEO,
                                                                                        f_nPhaseMethodSelection,
                                                                                        (f64complex_t*)l_dKspaceOddDataBuffer);
                                if (E_NO_ERROR != l_nStatus)
                                {
                                    throw E_ERROR;
                                }
                                
                                int NoOfPoints = l_nWidth * (l_nHeight/2) * 2;
                                m_csImageFileName.Format("");
                                m_csImageFileName.Format("KEvenPC_Channel_%d_Dim_%dx%d",l_nChanCnt,l_nWidth,(l_nHeight/2));
                                CString l_csImagePathKEven = l_csFilePath + m_csImageFileName;
                                m_imageWrite.WriteToFile(l_csImagePathKEven,
                                    l_dKspaceEvenDataBuffer,
                                    NoOfPoints);
                                m_csImageFileName.Format("");
                                m_csImageFileName.Format("KOddPC_Channel_%d_Dim_%dx%d",l_nChanCnt,l_nWidth,(l_nHeight/2));
                                CString l_csImagePathKOdd = l_csFilePath + m_csImageFileName;
                                m_imageWrite.WriteToFile(l_csImagePathKOdd,
                                    l_dKspaceOddDataBuffer,
                                    NoOfPoints);

//                                 double *l_dDataDumpBuff = NULL;
//                                 int l_nSize = l_nWidthEO * l_nHeightEO * 2;
//                                 m_vcfHandle->MakeRealData(l_dKspaceEvenDataBuffer,l_nSize,l_dDataDumpBuff);
//                                 displayImage(l_dDataDumpBuff,l_nHeightEO,l_nWidthEO);

                                DELARR(l_dKspaceEvenDataBuffer);
                                DELARR(l_dKspaceOddDataBuffer);
                                DELARR(l_dTempEvenDataBuffer);
                                DELARR(l_dTempOddDataBuffer);
                                DELARR(l_dTemplateDataBuffer);

                            }
                        }
                    }
                }
            }
        }
        CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMainFrame *pChild = (CMainFrame*)pFrame->GetActiveFrame();
        CFormImageSelect *pWnd = pChild->GetEPIFormView();
        CString l_csTemp("Phase Correction");
        pWnd->addStep(l_csTemp);
        pWnd->getFileList(m_csPhaseCorrectionDirPath);
        m_nPhaseCorrectionFlag = 1;


    }catch(...){
        DELARR(l_dKspaceEvenDataBuffer);
        DELARR(l_dKspaceOddDataBuffer);
        DELARR(l_dTempEvenDataBuffer);
        DELARR(l_dTempOddDataBuffer);
        DELARR(l_dTemplateDataBuffer);
    }

    return E_NO_ERROR;
}
int CEPIController::createPhaseCorrectedDir(const int &f_PortIdx, 
                                            int &f_nSubProtNo, 
                                            int &f_nSubGrpNo, 
                                            int &f_nSliceNo, 
                                            int &f_nSliceImgNo,
                                            int &f_nPhaseSelectionFlag,
                                            CString &f_csDirPath)
{
    m_imageWrite.createFolders(f_PortIdx,
                               f_nSubProtNo,
                               f_nSubGrpNo,
                               f_nSliceNo,
                               f_nSliceImgNo,
                               f_csDirPath);
    f_csDirPath = f_csDirPath + _T("/Phase_Correction/");
    m_csPhaseCorrectionDirPath = f_csDirPath;
    int l_nStatus = CreateDirectory(f_csDirPath, NULL);
    switch (f_nPhaseSelectionFlag)
    {
    case TIME_SHIFT:
        f_csDirPath = f_csDirPath + _T("/Time_Shift/");
        m_csPhaseCorrectionDirPath = f_csDirPath;
        l_nStatus = CreateDirectory(f_csDirPath, NULL);
    	break;
    case TIME_SHIFT_LINE_FIT:
        f_csDirPath = f_csDirPath + _T("/Time_Shift_Line_Fit/");
        m_csPhaseCorrectionDirPath = f_csDirPath;
        l_nStatus = CreateDirectory(f_csDirPath, NULL);
        break;
    default:
        break;
    }
    l_nStatus = deleteFiles(f_csDirPath);

    return E_NO_ERROR;
}

CString CEPIController::getPhaseCorrectionDirPath() const
{
    return m_csPhaseCorrectionDirPath;
}

int CEPIController::mergePhaseCorrectedData()
{
    double *l_cKspaceDataBuffer = NULL;
    double *l_cKspaceEvenDataBuffer = NULL;
    double *l_cKspaceOddDataBuffer = NULL;
    double *l_cT1DataBuffer = NULL;
    double *l_cT2DataBuffer = NULL;
    double *l_dFinalBuffer = NULL;
    void *l_nVfdataBuff = NULL;

    try{
        if(NULL == m_vcfHandle || NULL == m_vfcWriter){
            throw E_ERROR;
        }
        CScrPadHandler l_ScrPadObj;
        CString l_csStudyFile = m_vcfHandle->StudyFileName();
        int l_nDstProt = -1;
        int l_nStatus = l_ScrPadObj.createNewProtocol(l_csStudyFile,m_nProtIndex,l_nDstProt);
        if (l_nStatus != E_NO_ERROR || l_nDstProt < 0){
            throw E_ERROR;
        }
        m_nKsT1T2MergeProtIndex = l_nDstProt;
        l_nStatus = m_vfcWriter->CloseDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;
        }
        l_nStatus = m_vfcWriter->OpenDBFile();
        if (l_nStatus != E_NO_ERROR){
            throw E_ERROR;            
        }
        int l_nSubProtNo = 0;
        int l_nSubGrpNo = 0;
        int l_nSliceNo= 0;
        int l_nSliceImgNo = 0;
        VfPathElem_t l_pathP[5] = { SVN_ST_PROTOCOL,   0, /* This path order should be maintained */
            SVN_ST_SUBPROTOCOL, 0,
            SVN_ST_SLICEGROUP, 0,
            SVN_ST_SLICE,      0,
            SVN_ST_IMAGE,      0
        };
        CString l_csKSEvenFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csKSOddFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csKSFileName(_T("Kspace_Channel_0_Dim_128x66"));
        CString l_csT1FileName(_T("T1_Channel_0_Dim_128x66"));
        CString l_csT2FileName(_T("T2_Channel_0_Dim_128x66"));
        CString l_csStep(_T("Separate Data"));
        int l_nHeight = 0,l_nWidth = 0;
        CFileReader l_filreader;

        l_nStatus = m_vcfHandle->protTraverse(  m_nProtIndex,
                                                l_nSubProtNo,
                                                l_nSubGrpNo,
                                                l_nSliceNo,
                                                l_nSliceImgNo);
        if (E_NO_ERROR != l_nStatus){
            throw E_ERROR;
        }
        int l_nTotalChannel = m_vcfHandle->getTotalChannel();
        CEvenOddSeparator l_evenOddSeparate;
        l_pathP[0].index = l_nDstProt;
        for (int l_nSubProtCnt = 0; l_nSubProtCnt < l_nSubProtNo;l_nSubProtCnt++)
        {
            l_pathP[1].index = l_nSubProtCnt;
            for (int l_nSubGrpCnt = 0;l_nSubGrpCnt < l_nSubGrpNo;l_nSubGrpCnt++)
            {
                l_pathP[2].index = l_nSubGrpCnt;
                
                for(int l_nSliceNoCnt = 0;l_nSliceNoCnt < l_nSliceNo;l_nSliceNoCnt++)
                {
                    l_pathP[3].index = l_nSliceNoCnt;
                    
                    for (int l_nSliceImgNoCnt = 0; l_nSliceImgNoCnt < l_nSliceImgNo;l_nSliceImgNoCnt++)
                    {
                        for (int l_nChanCnt = 0;l_nChanCnt < l_nTotalChannel;l_nChanCnt++)
                        {
                            l_pathP[4].index = l_nSliceImgNoCnt;
                            l_csKSFileName.Format("");
                            l_csT1FileName.Format("");
                            l_csT2FileName.Format("");
                            l_csKSEvenFileName.Format("");
                            l_csKSOddFileName.Format("");
                            CString l_csTemp;
                            getFileName(m_csSeparateKEvenOddDirPath,"Phase Correction",
                                l_nChanCnt,l_csKSEvenFileName,
                                l_csKSOddFileName,l_csTemp);
                            getFileName(m_csSeparateKT1T2DirPath,l_csStep,
                                l_nChanCnt,l_csKSFileName,
                                l_csT1FileName,l_csT2FileName);
                            int l_nHeightEO;
                            int l_nWidthEO;
                            getHeightWidth(l_csKSEvenFileName,l_nWidthEO,l_nHeightEO);
                            getHeightWidth(l_csKSFileName,l_nWidth,l_nHeight);
                            if(l_nHeight <=0 || l_nWidth <= 0 || l_nWidthEO <= 0 || l_nHeightEO <= 0){
                                throw E_ERROR;
                            }
                            l_csKSEvenFileName = m_csSeparateKEvenOddDirPath + "\\" + l_csKSEvenFileName;
                            l_csKSOddFileName = m_csSeparateKEvenOddDirPath + "\\" + l_csKSOddFileName;

                            l_csKSFileName = m_csSeparateKT1T2DirPath + "\\" + l_csKSFileName;
                            l_csT1FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT1FileName;
                            l_csT2FileName = m_csSeparateKT1T2DirPath + "\\" + l_csT2FileName;
                            
                            int newImagepixels = (l_nWidthEO* 2) * l_nHeightEO ; //2  = real + imaginary

                            l_nStatus = l_filreader.ReadImage(l_csKSEvenFileName,newImagepixels,l_cKspaceEvenDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }

                            l_nStatus = l_filreader.ReadImage(l_csKSOddFileName,newImagepixels,l_cKspaceOddDataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            int l_nImgPix = l_nWidth * l_nHeight * 2;
                            l_cKspaceDataBuffer = new double[l_nImgPix];
                            l_evenOddSeparate.mergeDataEvenOdd(l_cKspaceDataBuffer,
                                                             l_cKspaceEvenDataBuffer,
                                                             l_cKspaceOddDataBuffer,
                                                             l_nWidth,l_nHeight);



                            newImagepixels = (l_nWidth* 2) * l_nHeight ; //2  = real + imaginary
                            l_nStatus = l_filreader.ReadImage(l_csT1FileName,newImagepixels,l_cT1DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }
                            
                            l_nStatus = l_filreader.ReadImage(l_csT2FileName,newImagepixels,l_cT2DataBuffer);
                            if (E_NO_ERROR != l_nStatus)
                            {
                                throw E_ERROR;
                            }


                            long FinalBuffSize = (newImagepixels * 3)* sizeof(double);
                            l_dFinalBuffer = (double*)malloc(FinalBuffSize);

                            if (l_cKspaceDataBuffer == NULL ||
                                l_cT1DataBuffer == NULL ||
                                l_cT2DataBuffer == NULL ||
                                NULL == l_dFinalBuffer)
                            {
                                throw EPI_ERROR_NULL_FUNCPARAMS;
                            }
                            ZeroMemory(l_dFinalBuffer, FinalBuffSize);



                            CkspaceSeparator l_objSeparate;

                            l_nStatus =l_objSeparate.mergeDataTemplateAndKspace(l_dFinalBuffer,
                                                                        l_cKspaceDataBuffer,
                                                                        l_cT1DataBuffer,
                                                                        l_cT2DataBuffer,
                                                                        l_nWidth,
                                                                        l_nHeight*3);
                            if (l_nStatus != E_NO_ERROR)
                            {                                
                                throw E_ERROR;
                            }
                            int l_nRepetitionTime = 0;
                            int l_nFinalDataForm = 0;
                            int l_nFinalDataType = 0;
                            int l_nFinalDataHeight = 0;
                            int l_nFinalDataWidth = 0;
                            l_nStatus = m_vcfHandle->getProtocolData(l_pathP,
                                l_nRepetitionTime,
                                l_nFinalDataForm,
                                l_nFinalDataType,
                                l_nFinalDataHeight,
                                l_nFinalDataWidth);
                            if (l_nStatus != E_NO_ERROR || l_nFinalDataHeight <= 0 || l_nFinalDataWidth <= 0)
                            {                                
                                throw E_ERROR;
                            }
                            int FinalTotalPixels = l_nFinalDataWidth * 2 * l_nFinalDataHeight;
                            l_nStatus = ConvertToDataType(l_dFinalBuffer,l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType,false);
                            if(l_nStatus != 0 || NULL == l_nVfdataBuff){
                                throw E_ERROR;
                            }
                            InvertBuffer(l_nVfdataBuff,FinalTotalPixels,l_nFinalDataType);
                            int memRequiredForImage = 0;
                            switch(l_nFinalDataType) {
                            case VFT_INT16:
                                memRequiredForImage = FinalTotalPixels*sizeof(int16_t);
                                break;
                            case VFT_INT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(int32_t);
                                break;
                            case VFT_FLT32:
                                memRequiredForImage = FinalTotalPixels*sizeof(flt32_t);
                                break;
                            default:
                                throw (EPI_ERROR);
                            }
                            l_nStatus = m_vfcWriter->PutData(l_nVfdataBuff,
                                                              l_pathP,memRequiredForImage,
                                                              l_nFinalDataType,l_nChanCnt);
                            if (l_nStatus != E_NO_ERROR )
                            {                                
                                throw E_ERROR;
                            }
                            DELARR(l_cKspaceDataBuffer);
                            DELARR(l_cT1DataBuffer);
                            DELARR(l_cT2DataBuffer);
                            DELARR(l_dFinalBuffer);
                            DELARR(l_cKspaceEvenDataBuffer);
                            DELARR(l_cKspaceOddDataBuffer);
                            MEMFREE(l_nVfdataBuff);
                        }
                    }
                }
            }
        }

    }catch(...){
        DELARR(l_cKspaceDataBuffer);
        DELARR(l_cT1DataBuffer);
        DELARR(l_cT2DataBuffer);
        DELARR(l_dFinalBuffer);
        DELARR(l_cKspaceEvenDataBuffer);
        DELARR(l_cKspaceOddDataBuffer);
        MEMFREE(l_nVfdataBuff);
        return E_ERROR;
    }
    return E_NO_ERROR;    
}
