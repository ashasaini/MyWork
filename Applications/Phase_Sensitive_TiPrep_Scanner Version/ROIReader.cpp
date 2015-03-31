// ROIInfo.cpp: implementation of the CROIInfo class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ROIReader.h"
#include "TiPrepLogHelper.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CROIReader
//Purpose       : Constructor
//********************************************************************
CROIReader::CROIReader()
{
    //    
}
//****************************Method Header***************************
//Method Name   : CROIReader
//Purpose       : Destructor
//********************************************************************
CROIReader::~CROIReader()
{
    //
}


/*************************************************************
* Method:       ReadROIInfo
* Description:  This function is used
* Parameter:    CString f_csfileName
* Parameter:    CString f_csProtocolstr
* Parameter:    vector<CCircleROIParams> & f_vRoiVector
* Returns:      BOOL
*************************************************************/
BOOL CROIReader::ReadROIInfo(CString &f_csfileName, CString &f_csProtocolstr, vector<CCircleROIParams> &f_vRoiVector)
{
    char* l_cpFileName = NULL;

    try {
        char l_cbuffer[100];
        int l_nReadCnt = 0;
        int l_nRoicnt = 0;
        CCircleROIParams l_crROIparams;
        float l_fTempval;
        BOOL l_bStatusProtoFind = FALSE;
        int l_nCount = 1;
        l_cpFileName = new char[64];
        wcstombs(l_cpFileName, (LPCTSTR) f_csfileName, 64);
        fstream l_ifReader(l_cpFileName, ios::in /*| ios::out*/); 

        if(l_ifReader.fail()) {
            CString l_cserrmsg = _T("Failed to read ROI file : ") + f_csfileName;
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);

            if(NULL != l_cpFileName) {
                delete [] l_cpFileName;
                l_cpFileName = NULL;
            }

            return E_ERROR;
        }

        while(!l_ifReader.eof()) {
            l_ifReader.getline(l_cbuffer, 100);
            CString l_csTempStr(l_cbuffer);

            if((l_csTempStr.Find(_T(";"))) > -1) {
                break;
            }

            int l_nPos = l_csTempStr.Find(_T("="));
            CString l_csValue = l_csTempStr.Right(l_csTempStr.GetLength() - (l_nPos + 1));
            CString l_csKey = l_csTempStr.Left(l_nPos);

            if(l_csValue.CompareNoCase(f_csProtocolstr) == 0) {
                l_bStatusProtoFind = TRUE;
                continue;
            }

            if(l_bStatusProtoFind) {
                l_nCount++;

                if(l_csKey.CompareNoCase(_T("ROI_Number")) == 0) {
                    continue;
                }

                if((l_nReadCnt != 0) && (l_nReadCnt % 3) == 0) { 
                    l_nRoicnt++;
                    f_vRoiVector.push_back(l_crROIparams);
                }

                char l_cpBuff[64];
                wcstombs(l_cpBuff, (LPCTSTR) l_csValue, 64);
                l_fTempval = (float)atof(l_cpBuff);

                if(l_csKey.CompareNoCase(_T("Radius")) == 0) {
                    l_fTempval = static_cast<float>(fabs(l_fTempval)); 
                    l_crROIparams.SetRadius(l_fTempval);
                    l_nReadCnt++;
                }

                if(l_csKey.CompareNoCase(_T("X-Center")) == 0) {
                    l_crROIparams.SetCenter_X(l_fTempval);
                    l_nReadCnt++;
                }

                if(l_csKey.CompareNoCase(_T("Y-Center")) == 0) {
                    l_crROIparams.SetCenter_Y(l_fTempval);
                    l_nReadCnt++;
                }
            }

            if(ROI_INFO_TOTAL_LINE_COUNT_CIRCLE == l_nCount) { 
                f_vRoiVector.push_back(l_crROIparams);
                break;
            }
        }

        if(l_bStatusProtoFind && l_nReadCnt != 15) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get ROI information from file. File Format error "), __LINE__, __FILE__);
            f_vRoiVector.clear();
            l_ifReader.close();

            if(NULL != l_cpFileName) {
                delete [] l_cpFileName; 
                l_cpFileName = NULL;
            }

            return E_ERROR;
        }

        if(!l_bStatusProtoFind) {
            //
        }

        l_ifReader.close();

        if(NULL != l_cpFileName) {
            delete [] l_cpFileName; 
            l_cpFileName = NULL;
        }
    } catch(CMemoryException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
        return E_ERR_EXCEPTION;
    } catch(CFileException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("File exception of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
        return E_ERR_EXCEPTION;
    } catch(CException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
        return E_ERR_EXCEPTION;
    }catch(...){
        CString l_csStrFormatted;
        l_csStrFormatted = _T("Exception because of this error: ");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
    }

    return E_NO_ERROR;
}

/*************************************************************
* Method:       WriteROIInfo
* Description:  This function is used
* Parameter:    CString f_csfileName
* Parameter:    CString f_csProtocolName
* Parameter:    vector<CCircleROIParams> & f_vRoiVector
* Returns:      BOOL
*************************************************************/
BOOL CROIReader::WriteROIInfo(CString &f_csfileName, CString &f_csProtocolName, vector<CCircleROIParams> &f_vRoiVector)
{
    try {
        int l_nCount = 0;
        int l_nWriteCnt = 0;
        int l_nRoicnt = 0;
        fstream l_fileWriter;
        BOOL l_bstatus = FALSE;
        BOOL l_bupdate_done = FALSE;
        //CString l_csMapValue;
        char* l_cpFileName = new char[64];
        CCircleROIParams l_crTempObj;
        wcstombs(l_cpFileName, (LPCTSTR) f_csfileName, 64);
        char destFldr[100] = {'\0'};
        strcpy(destFldr, "c:/MrMplus/Data/TiPrep");
        if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
            //folder present
        }else{
            CTiPrepLogHelper::WriteToErrorLog
            (_T("WriteROIInfo():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
            return(E_ERROR);
        }
        DWORD dwAttrs;
        dwAttrs = GetFileAttributes(f_csfileName);

        if(INVALID_FILE_ATTRIBUTES != dwAttrs) {
            l_fileWriter.open(l_cpFileName, fstream::in);
            if(l_fileWriter.fail()) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("Unable to open file for writing"), __LINE__, __FILE__);
                
                if(NULL != l_cpFileName) { 
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }
                
                return (E_ERROR);
            }
            fstream tempoutfile;
            char *l_cstempFilename = "c:/MrMplus/Data/TiPrep/TiPrep_temp_roiinfo.txt";
            tempoutfile.open(l_cstempFilename, fstream::out);

            if(tempoutfile.fail()) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("Unable to open file for writing"), __LINE__, __FILE__);

                if(NULL != l_cpFileName) {  
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }
                l_fileWriter.close();
                return (E_ERROR);
            }

            char l_cBuffer[100];

            while(l_fileWriter.getline(l_cBuffer, 100)) {
                CString l_csTempStr(l_cBuffer);
                int l_nPos = l_csTempStr.Find(_T("="));
                CString l_csValue = l_csTempStr.Right(l_csTempStr.GetLength() - (l_nPos + 1));
                CString l_csKey = l_csTempStr.Left(l_nPos);
                //int length = l_csTempStr.GetLength();

                if(l_csValue.CompareNoCase(f_csProtocolName) == 0) {
                    l_bstatus = TRUE;
                    l_bupdate_done = TRUE;
                    l_nCount++;
                    sprintf(l_cBuffer, "StudyFileName=%S\n", f_csProtocolName);
                    tempoutfile.write(l_cBuffer, strlen(l_cBuffer)); 
                    continue;
                }

                if(l_bstatus) {    
                    //Replace existing value with new values
                    l_crTempObj = f_vRoiVector.at(l_nRoicnt);

                    if(l_nWriteCnt == 3) {
                        l_nWriteCnt = 0;
                        l_nRoicnt++;
                    }

                    if(l_csKey.CompareNoCase(_T("ROI_Number")) == 0) {
                        l_nCount++;
                        tempoutfile.write(l_cBuffer, strlen(l_cBuffer)); 
                        tempoutfile << endl; 
                        continue;
                    }

                    if(l_csKey.CompareNoCase(_T("Radius")) == 0) {
                        l_csValue.Format(_T("Radius=%.8f\n"), f_vRoiVector.at(l_nRoicnt).GetRadius()); 
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength()); 
                        l_nWriteCnt++;
                        l_nCount++;
                    }

                    if(l_csKey.CompareNoCase(_T("X-Center")) == 0) {
                        l_csValue.Format(_T("X-Center=%.8f\n"), f_vRoiVector.at(l_nRoicnt).GetCenter_X());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength()); 
                        l_nWriteCnt++;
                        l_nCount++;
                    }

                    if(l_csKey.CompareNoCase(_T("Y-Center")) == 0) {
                        l_csValue.Format(_T("Y-Center=%8f\n"), f_vRoiVector.at(l_nRoicnt).GetCenter_Y());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength()); 
                        l_nWriteCnt++;
                        l_nCount++;
                    }
                } else { //Copy data of Other saved protocols
                    tempoutfile.write(l_cBuffer, strlen(l_cBuffer)); 
                    tempoutfile << endl;
                }

                if(l_nCount == 13) {
                    l_bstatus = FALSE; 
                }
            }

            //Delete old file and rename temporary file as new file
            l_fileWriter.close();
            tempoutfile.close();
            unlink(l_cpFileName);
            CString l_cstrtempFile = l_cstempFilename;
            BOOL status = MoveFile(l_cstrtempFile, f_csfileName);

            if(status == FALSE) {
                CString l_cserr;
                l_cserr.Format(_T("Error while moving File : %d"), GetLastError());
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
                l_cserr.Format(_T("Updating ROI file failed.To save ROI position, rename file %s to %s Manually."), l_cstrtempFile, f_csfileName);
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);

                if(NULL != l_cpFileName) {
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }

                return E_ERROR;
            } else {
                unlink("c:/MrMplus/Data/TiPrep/temp_roiinfo.txt");
            }
        }

        if(l_bupdate_done == FALSE) {
            CString l_csTempStr;
            ofstream obj;
            char l_cBuffer[100];
            obj.open(l_cpFileName, ios::app);
            sprintf(l_cBuffer, "StudyFileName=%S\n", f_csProtocolName);
            obj.write(l_cBuffer, strlen(l_cBuffer));

            for(int i = 0; i < MAX_LINE_ROI; i++) {     
                l_csTempStr.Format(_T("ROI_Number=%d\n"), i + 1);
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("Radius=%.8f\n"), f_vRoiVector.at(i).GetRadius());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("X-Center=%.8f\n"), f_vRoiVector.at(i).GetCenter_X());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("Y-Center=%.8f\n"), f_vRoiVector.at(i).GetCenter_Y());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
            }

            obj.close();
        }

        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return FALSE;
    } catch(CFileException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return FALSE;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return FALSE;
    } catch(...) { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred while Saving ROI in File "), __LINE__, __FILE__);
        return FALSE;
    }

    return TRUE;
}

/*************************************************************
* Method:       ReadROIInfo
* Description:  This function is used
* Parameter:    CString f_csfileName
* Parameter:    CString f_csProtocolstr
* Parameter:    vector<CCircleROIParams> & f_vRoiVector
* Returns:      BOOL
*************************************************************/
BOOL CROIReader::ReadROIInfo(CString &f_csfileName, CString &f_csProtocolstr, 
                             vector<CEllipticalROIParams> &f_vRoiVector)
{
    try {
        char l_cbuffer[100];
        int l_nReadCnt = 0;
        int l_nRoicnt = 0;
        CEllipticalROIParams l_crROIparams;
        float l_fTempval;
        BOOL l_bStatusProtoFind = FALSE;
        int l_nCount = 1;
        char* l_cpFileName = NULL;
        l_cpFileName = new char[64];
        wcstombs(l_cpFileName, (LPCTSTR) f_csfileName, 64);
        fstream l_ifReader(l_cpFileName, ios::in);

        if(l_ifReader.fail()) {
            CString l_cserrmsg = _T("Failed to read ROI file : ") + f_csfileName;
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);

            if(NULL != l_cpFileName) {  
                delete [] l_cpFileName;
                l_cpFileName = NULL;
            }

            return E_ERROR;
        }
        l_nRoicnt = 0;
        while(!l_ifReader.eof()) {
            l_ifReader.getline(l_cbuffer, 100);
            CString l_csTempStr(l_cbuffer);

            if((l_csTempStr.Find(_T(";"))) > -1) {
                break;
            }

            int l_nPos = l_csTempStr.Find(_T("="));
            CString l_csValue = l_csTempStr.Right(l_csTempStr.GetLength() - (l_nPos + 1));
            CString l_csKey = l_csTempStr.Left(l_nPos);

            if(l_csValue.CompareNoCase(f_csProtocolstr) == 0) {
                l_bStatusProtoFind = TRUE;
                continue;
            }

            if(l_bStatusProtoFind) {
                l_nCount++;


                if(l_csKey.CompareNoCase(_T("ROI_Number")) == 0) {
                    continue;
                }               
                if((l_nReadCnt != 0) && (l_nReadCnt % 4) == 0) {
                    l_nRoicnt++;
                    f_vRoiVector.push_back(l_crROIparams);
                }               

                char l_cpBuff[64];
                wcstombs(l_cpBuff, (LPCTSTR) l_csValue, 64);
                l_fTempval = (float)atof(l_cpBuff);

                if(l_csKey.CompareNoCase(_T("Height")) == 0) {
                    l_fTempval = static_cast<float>(fabs(l_fTempval));
                    l_crROIparams.SetHeight(l_fTempval);
                    l_nReadCnt++;
                }

                if(l_csKey.CompareNoCase(_T("Width")) == 0) {
                    l_fTempval = static_cast<float>(fabs(l_fTempval));
                    l_crROIparams.SetWidth(l_fTempval);
                    l_nReadCnt++;
                }

                if(l_csKey.CompareNoCase(_T("X-Center")) == 0) {
                    l_crROIparams.SetCenter_X(l_fTempval);
                    l_nReadCnt++;
                }

                if(l_csKey.CompareNoCase(_T("Y-Center")) == 0) {
                    l_crROIparams.SetCenter_Y(l_fTempval);
                    l_nReadCnt++;
                }

            }

            if(ROI_INFO_TOTAL_LINE_COUNT == l_nCount) {
                f_vRoiVector.push_back(l_crROIparams);
                break;
            }
        }

        if(l_bStatusProtoFind && l_nReadCnt != 20) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get ROI information from file. File Format error ellipse"), __LINE__, __FILE__);
            f_vRoiVector.clear();

            if(NULL != l_cpFileName) {
                delete [] l_cpFileName;
                l_cpFileName = NULL;
            }

            l_ifReader.close();
            return E_ERROR;
        }

        if(!l_bStatusProtoFind) {
        }

        l_ifReader.close();

        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;
            l_cpFileName = NULL;
        }
    } catch(CMemoryException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(CFileException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("File exception of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(CException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }

    return TRUE;
}

/*************************************************************
* Method:       WriteROIInfo
* Description:  This function is used
* Parameter:    CString f_csfileName
* Parameter:    CString f_csProtocolName
* Parameter:    vector<CCircleROIParams> & f_vRoiVector
* Returns:      BOOL
*************************************************************/
BOOL CROIReader::WriteROIInfo(CString &f_csfileName, CString &f_csProtocolName, 
                              vector<CEllipticalROIParams> &f_vRoiVector)
{
    try {
        int l_nCount = 0;
        int l_nWriteCnt = 0;
        int l_nRoicnt = 0;
        fstream l_fileWriter;
        BOOL l_bstatus = FALSE;
        BOOL l_bupdate_done = FALSE;
        //CString l_csMapValue;
        char* l_cpFileName = new char[64];
        CEllipticalROIParams l_crTempObj;
        wcstombs(l_cpFileName, (LPCTSTR) f_csfileName, 64);
        char destFldr[100] = {'\0'};
        strcpy(destFldr, "c:/MrMplus/Data/TiPrep");
        if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
            //folder present
        }else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("WriteROIInfo():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
            return(E_ERROR);
        }
        DWORD dwAttrs;
        dwAttrs = GetFileAttributes(f_csfileName);

        if(INVALID_FILE_ATTRIBUTES != dwAttrs) {
            l_fileWriter.open(l_cpFileName, fstream::in);
            if(l_fileWriter.fail()) {
                CTiPrepLogHelper::WriteToErrorLog(_T("Unable to open file for writing"), __LINE__, __FILE__);
                
                if(NULL != l_cpFileName) {  
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }
                
                return (E_ERROR);
            }
            fstream tempoutfile;
            char *l_cstempFilename = "c:/MrMplus/Data/TiPrep/TiPrep_temp_roiinfo.txt";
            tempoutfile.open(l_cstempFilename, fstream::out);

            if(tempoutfile.fail()) {
                CTiPrepLogHelper::WriteToErrorLog(_T("Unable to open file for writing"), __LINE__, __FILE__);

                if(NULL != l_cpFileName) {
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }
                l_fileWriter.close();
                return (E_ERROR);
            }

            char l_cBuffer[100];

            l_nRoicnt = 0;
            while(l_fileWriter.getline(l_cBuffer, 100)) {

                CString l_csTempStr(l_cBuffer);
                int l_nPos = l_csTempStr.Find(_T("="));
                CString l_csValue = l_csTempStr.Right(l_csTempStr.GetLength() - (l_nPos + 1));
                CString l_csKey = l_csTempStr.Left(l_nPos);
                //int length = l_csTempStr.GetLength();

                if(l_csValue.CompareNoCase(f_csProtocolName) == 0) {
                    l_bstatus = TRUE;
                    l_bupdate_done = TRUE;
                    l_nCount++;
                    sprintf(l_cBuffer, "StudyFileName=%S\n", f_csProtocolName);
                    tempoutfile.write(l_cBuffer, strlen(l_cBuffer)); 
                    continue;
                }

                if(l_bstatus) {          //Replace existing value with new values
                    l_crTempObj = f_vRoiVector.at(l_nRoicnt);

                    if(l_nWriteCnt == 4) {
                        l_nWriteCnt = 0;
                        l_nRoicnt++;
                    }

                    if(l_csKey.CompareNoCase(_T("ROI_Number")) == 0) {
                        l_nCount++;
                        tempoutfile.write(l_cBuffer, strlen(l_cBuffer));
                        tempoutfile << endl;
                        continue;
                    }

                    if(l_csKey.CompareNoCase(_T("Height")) == 0) {
                        l_csValue.Format(_T("Height=%.8f\n"), f_vRoiVector.at(l_nRoicnt).GetHeight());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength());
                        l_nWriteCnt++;
                        l_nCount++;
                    }

                    if(l_csKey.CompareNoCase(_T("Width")) == 0) {
                        l_csValue.Format(_T("Width=%.8f\n"), f_vRoiVector.at(l_nRoicnt).GetWidth());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength());
                        l_nWriteCnt++;
                        l_nCount++;
                    }

                    if(l_csKey.CompareNoCase(_T("X-Center")) == 0) {
                        l_csValue.Format(_T("X-Center=%.8f\n"), f_vRoiVector.at(l_nRoicnt).GetCenter_X());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength());
                        l_nWriteCnt++;
                        l_nCount++;
                    }

                    if(l_csKey.CompareNoCase(_T("Y-Center")) == 0) {
                        l_csValue.Format(_T("Y-Center=%8f\n"), f_vRoiVector.at(l_nRoicnt).GetCenter_Y());
                        wcstombs(l_cBuffer, (LPCTSTR) l_csValue, 100);
                        tempoutfile.write(l_cBuffer, l_csValue.GetLength());
                        l_nWriteCnt++;
                        l_nCount++;
                    }
                } else { //Copy data of Other saved protocols
                    tempoutfile.write(l_cBuffer, strlen(l_cBuffer));
                    tempoutfile << endl;
                    //break;//KJ-JULY
                }

                if(ROI_INFO_TOTAL_LINE_COUNT == l_nCount) {
                    l_bstatus = FALSE;
                }
            }

            //Delete old file and rename temporary file as new file
            l_fileWriter.close();
            tempoutfile.close();
            unlink(l_cpFileName);
            CString l_cstrtempFile = l_cstempFilename;
            BOOL status = MoveFile(l_cstrtempFile, f_csfileName);

            if(status == FALSE) {
                CString l_cserr;
                l_cserr.Format(_T("Error while moving File : %d"), GetLastError());
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
                l_cserr.Format(_T("Updating ROI file failed.To save ROI position, rename file %s to %s Manually."), l_cstrtempFile, f_csfileName);
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);

                if(NULL != l_cpFileName) {
                    delete [] l_cpFileName;
                    l_cpFileName = NULL;
                }

                return E_ERROR;
            } else {
                unlink("c:/MrMplus/Data/TiPrep/temp_roiinfo.txt");
            }
        }

        if(l_bupdate_done == FALSE) {
            CString l_csTempStr;
            ofstream obj;
            char l_cBuffer[100];
            obj.open(l_cpFileName, ios::app);
            sprintf(l_cBuffer, "StudyFileName=%S\n", f_csProtocolName);
            obj.write(l_cBuffer, strlen(l_cBuffer));

            for(int i = 0; i < 5; i++) {
                l_csTempStr.Format(_T("ROI_Number=%d\n"), i + 1);
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("Height=%.8f\n"), f_vRoiVector.at(i).GetHeight());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("Width=%.8f\n"), f_vRoiVector.at(i).GetWidth());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("X-Center=%.8f\n"), f_vRoiVector.at(i).GetCenter_X());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
                l_csTempStr.Format(_T("Y-Center=%.8f\n"), f_vRoiVector.at(i).GetCenter_Y());
                wcstombs(l_cBuffer, (LPCTSTR) l_csTempStr, 100);
                obj.write(l_cBuffer, l_csTempStr.GetLength());
            }

            obj.close();
        }

        if(NULL != l_cpFileName) {
            delete [] l_cpFileName;  
            l_cpFileName = NULL;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(CFileException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred while Saving ROI in File "), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////