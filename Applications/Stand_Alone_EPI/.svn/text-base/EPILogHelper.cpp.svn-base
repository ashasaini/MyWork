// T1MapLogHelper.cpp: implementation of the T1MapLogHelper class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "EPILogHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEPILogHelper* CEPILogHelper::m_EPILogHelper = NULL;
FILE* CEPILogHelper::m_pEPILogFile = NULL;
std::ofstream CEPILogHelper::m_pEPIErrorLogFile;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:       CEPILogHelper
* Description:  This function is used
* Returns:
*************************************************************/
CEPILogHelper::CEPILogHelper()
{
    //
}

/*************************************************************
* Method:       ~CEPILogHelper
* Description:  This function is used
* Returns:
*************************************************************/
CEPILogHelper::~CEPILogHelper()
{
//     if(NULL != m_T1MapLogHelper) {
//         delete m_T1MapLogHelper;
//         m_T1MapLogHelper = NULL;
//     }
//  m_pEPIErrorLogFile.close();
}

/*************************************************************
* Method:       CreateInstance
* Description:  This function is used
* Returns:      bool
*************************************************************/
bool CEPILogHelper::CreateInstance()
{
    try {
        if(NULL == m_EPILogHelper) {
            m_EPILogHelper = new CEPILogHelper();

            if(!m_EPILogHelper) {
                return false;
            } else {
//                 char config_path[100];// = NULL;
//                 //config_path = new char[100];
//                 //memset(config_path,0x0,sizeof(char)*100);
//                 //config_path = getenv("MRMP_LOGDIR");
//                 sprintf(config_path,"%s","C:\\EPI_Data\\Error\\EPIErrorLog.txt");
//                 if(NULL == m_pEPILogFile){
//                     m_pEPILogFile = fopen(config_path,"w");
//
//                     if(NULL == m_pEPILogFile){
//                         return false;
//                     }
//                  fprintf(m_pEPILogFile,"Error log file");
//                 }
//                   //  config_path = NULL;
                CString l_csFilePath("C:/EPI_Data/");;
                int l_nStatus = CreateDirectory(l_csFilePath, NULL);
                l_csFilePath = l_csFilePath + _T("/Error/");
                l_nStatus = CreateDirectory(l_csFilePath, NULL);
                m_pEPIErrorLogFile.open(_T("C:\\EPI_Data\\Error\\EPIErrorLog.txt"));
                m_pEPIErrorLogFile << _T("Writing this to a file.\n");
            }
        }
    } catch(...) {
        return false;
    }

    return true;
}

/*************************************************************
* Method:       DeleteInstance
* Description:  This function is used
* Returns:      void
*************************************************************/
void CEPILogHelper::DeleteInstance()
{
    if(NULL != m_EPILogHelper) {
        delete m_EPILogHelper;
        m_EPILogHelper = NULL;
    }
}

/*************************************************************
* Method:       WriteToErrorLog
* Description:  This function is used
* Parameter:    CString f_smsg :
* Parameter:    int f_Line_No :
* Parameter:    CString f_csFile_Name :
* Returns:      void
*************************************************************/
void CEPILogHelper::WriteToErrorLog(CString f_smsg, int f_Line_No, CString f_csFile_Name)
{
//     if(NULL != m_pEPILogFile){
//      char *buffer = NULL;
//      CString l_csMsg;
//      l_csMsg.Format("ErrMsg:%s\nFILE:%s\tLine:%d\n\n",f_smsg,f_csFile_Name,f_Line_No);
//      int l_nlen = l_csMsg.GetLength();
//      buffer = new char[l_nlen];
//      if (NULL == buffer)
//      {
//          //error
//          return;
//      }
//      FILE * pFile;
//      pFile = fopen(_T("c:/myfile.txt"),"w");
//      sprintf(buffer,"%s",l_csMsg);
//      fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
    //fprintf(m_pEPILogFile,"ErrMsg:%s\nFILE:%s\tLine:%d\n\n",f_smsg,f_csFile_Name,f_Line_No);
    m_pEPIErrorLogFile << "ErrMsg:" << f_smsg << "\nFILE:" << f_csFile_Name << "\tLine:" << f_Line_No << std::endl << std::endl;
//     }
}

/*************************************************************
* Method:       WriteExceptionToLog
* Description:  This function is used to log me
* Parameter:    int f_Line_No :
* Parameter:    CString f_csFile_Name :
* Returns:      void
*************************************************************/
void CEPILogHelper::WriteExceptionToLog(int f_Line_No,
                                        CString f_csFile_Name)
{
    CString l_csexceptionmsg = _T("Exception occured in EPI");

    if(NULL != m_pEPILogFile) {
        fprintf(m_pEPILogFile, "ErrMsg:%S\nFILE:%S\tLine:%d\n\n", l_csexceptionmsg, f_csFile_Name, f_Line_No);
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////

