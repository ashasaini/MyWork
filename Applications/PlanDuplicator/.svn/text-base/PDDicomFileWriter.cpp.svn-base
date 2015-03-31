// PDDicomFileWriter.cpp: implementation of the CPDDicomFileWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "PDDicomFileWriter.h"
#include "dataTypes.h"
#include "PDErrorMsgDlg.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
#include <io.h>
#include <fstream>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DICOM_DATA_SZ_2 2
#define DICOM_DATA_SZ_4 4
const int DICM_STR_SZ = 4;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CPDDicomFileWriter
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CPDDicomFileWriter::CPDDicomFileWriter() : m_filePath(NULL),
m_cpWorkbuf(NULL),m_ndataSize(0)
{
    PD_TRACE(CPDDicomFileWriter::CPDDicomFileWriter);
    try
    {       
        //AN Commented
        /*m_cpWorkbuf = new char[3*BUFFER_SIZE_1024*BUFFER_SIZE_1024];
        
          if(NULL == m_cpWorkbuf)
          {
          CString l_cserrmsg = _T("Can not allocate memory for workbuff.");
          CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
          return;
          }
          else
        {*/
        m_filePath = new char[BUFFER_SIZE_160/*size +1*/];
        //dummy
        // }
        if (NULL == m_filePath)
        {
            CString l_cserrmsg = _T("Can not allocate memory for filepath.");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return;
            
        }
    }
    catch(...){
        //
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }
}
//////////////////////////////////////////////////////////////////////
// Copy Construction
//////////////////////////////////////////////////////////////////////

CPDDicomFileWriter::~CPDDicomFileWriter()
{
    PD_TRACE(CPDDicomFileWriter::~CPDDicomFileWriter);
    
    if(NULL != m_cpWorkbuf){
        delete []m_cpWorkbuf;
        m_cpWorkbuf = NULL;
    } 
    
    if(NULL != m_filePath){
        delete []m_filePath;
        m_filePath = NULL;
    }
    
    m_ndataSize = 0;
}

/*************************************************************
* Method:    	CPDDicomFileWriter
* Description: 	This function is used 
* Parameter: 	CPDDicomFileWriter & f_dcm_file_writer :
* Returns:   	
*************************************************************/
CPDDicomFileWriter::CPDDicomFileWriter(CPDDicomFileWriter& f_dcm_file_writer)
{
    PD_TRACE(CPDDicomFileWriter::CPDDicomFileWriter);
    try
    {   
        m_ndataSize = f_dcm_file_writer.m_ndataSize;
        m_cpWorkbuf = NULL;
        m_filePath = NULL;
        m_cpWorkbuf = new char[1024 *1024 * 3];
        if (NULL == m_cpWorkbuf)
        {
            throw new CMemoryException();
        }
        memcpy(m_cpWorkbuf,f_dcm_file_writer.m_cpWorkbuf,(1024 *1024 * 3));
        m_filePath = new char[BUFFER_SIZE_160];
        if (NULL == m_filePath)
        {
            throw new CMemoryException();
        }
        memcpy(m_filePath,f_dcm_file_writer.m_filePath,BUFFER_SIZE_160);
    }
    catch (CMemoryException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);      
    }
    catch (CException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }
    catch (...)
    {
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }
}

CPDDicomFileWriter& CPDDicomFileWriter::operator = (CPDDicomFileWriter& f_dcm_file_writer)
{
    PD_TRACE(CPDDicomFileWriter::operator=);
    
    if (this == &f_dcm_file_writer)
    {
        return(*this);
    }
    try
    {   
        m_ndataSize = f_dcm_file_writer.m_ndataSize;
        m_cpWorkbuf = NULL;
        m_filePath = NULL;
        m_cpWorkbuf = new char[BUFFER_SIZE_1024 *BUFFER_SIZE_1024 * 3];
        if (NULL == m_cpWorkbuf)
        {
            throw new CMemoryException();
        }
        memcpy(m_cpWorkbuf,f_dcm_file_writer.m_cpWorkbuf,(1024 *1024 * 3));
        m_filePath = new char[BUFFER_SIZE_160];
        if (NULL == m_filePath)
        {
            throw new CMemoryException();
        }
        memcpy(m_filePath,f_dcm_file_writer.m_filePath,BUFFER_SIZE_160);
    }
    catch (CMemoryException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);      
        return(*this);
    }
    catch (CException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(*this);
    }
    catch (...)
    {
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(*this);
    }
    return(*this);
    
}

//****************************Method Header************************************

//Method Name   : SetFilePath

//Author        :PATNI

//Purpose       : Set the study file path
//*****************************************************************************
int CPDDicomFileWriter::SetFilePath(char *f_filePath)
{
    PD_TRACE(CPDDicomFileWriter::SetFilePath);
    int l_ndcmError_code = E_NO_ERROR;
    
    try{
        if (NULL == m_filePath)
            //int size = strlen(f_filePath);
        {
            m_filePath = new char[BUFFER_SIZE_160/*size +1*/];
        }
        
        if(NULL == m_filePath){
            l_ndcmError_code = PD_MEMORY_ALLOCATION;
            CString l_cserrmsg = ErrorList[l_ndcmError_code];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(l_ndcmError_code);
        } else{
            
            memset(m_filePath,0,BUFFER_SIZE_160);
        }
        
        strcpy(m_filePath,f_filePath);
    }catch(...){
        l_ndcmError_code = PD_EXCEPTION;
        return(l_ndcmError_code);
    }
    
    return(E_NO_ERROR);
}

//****************************Method Header************************************

//Method Name   : freeworkbuf

//Author        :PATNI

//Purpose       : Free the m_cpWorkbuf
//*****************************************************************************
int CPDDicomFileWriter::freeworkbuf(void)
{
    PD_TRACE(CPDDicomFileWriter::freeworkbuf);
    
    /* if (m_ndataSize != 0){
    memset(m_cpWorkbuf,0,m_ndataSize);
    }else{
    //dummy
}*/
    try
    {
        delete [] m_cpWorkbuf;
        m_cpWorkbuf = NULL;
    }
    catch (...)
    {
        //	l_ndcmError_code = PD_EXCEPTION;
        CString l_cserrmsg = _T("PDDicomFileWriter::freeworkbuf Exception");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        
        return(PD_EXCEPTION);
    }
    m_ndataSize = 0;
    if (NULL != m_filePath)
    {
        memset(m_filePath,0,BUFFER_SIZE_160);
    }
    return (E_NO_ERROR);
}

//****************************Method Header************************************

//Method Name   : WriteMetaElementsGroup

//Author        :PATNI

//Purpose       : Write MetaElement group
//*****************************************************************************
int CPDDicomFileWriter::WriteMetaElementsGroup()
{
    PD_TRACE(CPDDicomFileWriter::WriteMetaElementsGroup);
    if (NULL == m_cpWorkbuf)
    {
        CString l_cserrmsg = _T("CPDDicomFileWriter::WriteMetaElementsGroup buffer NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(PD_PARAMETER_INVALID);
    }
    int l_ndcmError_code = E_NO_ERROR;
    char l_cVR_type[DICOM_VER_BUFF_SZ] = {0};
    char *l_cptr = NULL;
    char *l_cpC = new char[BUFFER_SIZE_256];
    int16_t group = 0x0002;
    int16_t element = 0x0010;
    
    try{
        if(NULL == l_cpC){
            l_ndcmError_code = PD_MEMORY_ALLOCATION;
            return(l_ndcmError_code);
        }
        
        short int l_nlength = 0;
        l_cptr = this->m_cpWorkbuf + this->m_ndataSize;
        memcpy(l_cptr,&group,sizeof(group));
        m_ndataSize += sizeof(group);
        l_cptr += sizeof(group);
        memcpy(l_cptr,&element,sizeof(element));
        m_ndataSize += sizeof(element);
        l_cptr += sizeof(element);
        
        strncpy(l_cVR_type,"UI",DICOM_VER_BUFF_SZ);//to be modified
        memcpy(l_cptr,l_cVR_type,DICOM_VER_BUFF_SZ);    
        l_cptr = l_cptr + DICOM_VER_BUFF_SZ;
        m_ndataSize = m_ndataSize + DICOM_VER_BUFF_SZ;
        
        l_nlength = (short)strlen(IMPLICIT_VR_LEN) + 1;
        memcpy(l_cptr,&l_nlength,DICOM_DATA_SZ_2);
        l_cptr = l_cptr + DICOM_DATA_SZ_2;
        m_ndataSize = m_ndataSize + DICOM_DATA_SZ_2;
        
        sprintf(l_cpC,"%s",IMPLICIT_VR_LEN);    
        strcat(l_cpC,"\0");
        
        memcpy(l_cptr,l_cpC,l_nlength);
        m_ndataSize = m_ndataSize + l_nlength;
        l_cptr = l_cptr + l_nlength;
        if(NULL != l_cpC){
            delete [] l_cpC;
            l_cpC = NULL;
        }
    }
    catch(...){
        l_ndcmError_code = PD_EXCEPTION;
        CString l_cserrmsg = _T("PDDicomFileWriter::WriteMetaElementsGroup Exception");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        if(NULL != l_cpC){
            delete [] l_cpC;
            l_cpC = NULL;
        }
        return(l_ndcmError_code);
    }
    
    return(E_NO_ERROR);
}
/****************************Method Header************************************

  //Method Name   : WriteGroupToFile
  
    //Author        :PATNI
    
      //Purpose       : Writes information of different groups in DICOM file
//*****************************************************************************/    
int CPDDicomFileWriter::WriteGroupToFile(dataElementT *f_grouptag, const int f_numElementsInGroup)
{
    PD_TRACE(CPDDicomFileWriter::WriteGroupToFile);
    if ((NULL == f_grouptag) || (NULL == m_cpWorkbuf))
    {
        CString l_cserrmsg = _T("CPDDicomFileWriter::WriteGroupToFile group tag NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(PD_PARAMETER_INVALID);
    }
    int l_nlfs = 0;
    int l_status = 0;
    int size = 0;
    char *l_cptr = NULL;   
    
    try {
        for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++){
            
            if(META_TAG != f_grouptag[l_nlfs].nemacode){
                //Write Group
                
                l_cptr = m_cpWorkbuf + m_ndataSize;
                memcpy(l_cptr,&f_grouptag[l_nlfs].group,sizeof(f_grouptag[l_nlfs].group));
                m_ndataSize += sizeof(f_grouptag[l_nlfs].group);
                
                //Write Element
                l_cptr = l_cptr + sizeof(f_grouptag[l_nlfs].group);
                memcpy(l_cptr,&f_grouptag[l_nlfs].element,sizeof(f_grouptag[l_nlfs].element));
                m_ndataSize += sizeof(f_grouptag[l_nlfs].element);
                
                //Write VR
                l_cptr = l_cptr + sizeof(f_grouptag[l_nlfs].element);
                memcpy(l_cptr,f_grouptag[l_nlfs].VR,DICOM_VER_BUFF_SZ);
                m_ndataSize += DICOM_VER_BUFF_SZ;
                l_cptr = l_cptr + DICOM_VER_BUFF_SZ;
                
                if (0 == strcmp(f_grouptag->VR,"OB")) {
                    size = DICOM_DATA_SZ_4; 
                    strcpy(l_cptr, "\0");
                    l_cptr++;
                    strcpy(l_cptr, "\0");
                    l_cptr++;
                    m_ndataSize += DICOM_DATA_SZ_2;         //AS for the null characters added twice 
                } else {
                    size = DICOM_DATA_SZ_2;
                }
                
                //Write Value Size
                memcpy(l_cptr,&f_grouptag[l_nlfs].length,size);
                m_ndataSize += size;
                
                //Write Value
                l_cptr = l_cptr + size;
                if ((NULL != f_grouptag[l_nlfs].value) && (0 < f_grouptag[l_nlfs].length))
                {
                   memcpy(l_cptr,f_grouptag[l_nlfs].value,f_grouptag[l_nlfs].length);
                   m_ndataSize += f_grouptag[l_nlfs].length;
                }
                
            }else {
                l_status = WriteMetaElementsGroup();
                
                if (l_status != E_NO_ERROR) {
                    CString l_cserrmsg = _T("WriteMetaElementsGroup: Error.");
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return PD_DICOM_FILE_CREATION;
                }
            }
        }
    } 
    catch(...) 
    {
        CString l_cserrmsg = _T("WriteGroupToFile: Exception.");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return PD_DICOM_FILE_CREATION;
    }
    
    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   : writeDICOMHeader

//Author        :PATNI

//Purpose       : Write DICOM file header
//*****************************************************************************
int CPDDicomFileWriter::writeDICOMHeader(void)
{
    PD_TRACE(CPDDicomFileWriter::writeDICOMHeader);
    char *l_cptr = NULL;
    int l_ndcmError_code = E_NO_ERROR;
    int l_nzerocnt=0;
    
    try
    {
        if(NULL == m_cpWorkbuf){
            l_ndcmError_code = PD_MEMORY_ALLOCATION;
            CString l_cserrmsg = _T("Can not point to workbuff.");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(l_ndcmError_code);
        } else {
            //dummy
        }
        
        for(l_nzerocnt=0;l_nzerocnt<NO_ZERO_BYTES;l_nzerocnt++){    
            strcpy(m_cpWorkbuf, "\0");
            m_cpWorkbuf++;
        }
        
        l_cptr = m_cpWorkbuf - NO_ZERO_BYTES;
        m_cpWorkbuf = m_cpWorkbuf - NO_ZERO_BYTES;
        l_cptr += NO_ZERO_BYTES;
        
        if(NULL == l_cptr){
            l_ndcmError_code = PD_MEMORY_ALLOCATION;
            CString l_cserrmsg = _T("Can not point to workbuff.");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(l_ndcmError_code);
        } else {
            //dummy
        }
        
        m_ndataSize = m_ndataSize + NO_ZERO_BYTES;
        sprintf(l_cptr,"%s","DICM");    
        l_cptr += DICM_STR_SZ;
        m_ndataSize += DICM_STR_SZ;    
    }
    catch(...)
    {
        l_ndcmError_code = PD_EXCEPTION;
        return(l_ndcmError_code);
    }
    
    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   : writeToFile

//Author        :PATNI

//Purpose       : Writes buf into DICOM file
//*****************************************************************************
int CPDDicomFileWriter::writeToFile(void)
{
    PD_TRACE(CPDDicomFileWriter::writeToFile);
    int l_ndcmError_code = E_NO_ERROR;
    
    try
    {
        fstream l_binary_file(m_filePath,ios::out|ios::binary);
        
        if(l_binary_file.fail()){
            l_ndcmError_code = PD_DICOM_FILE_CREATION;
            return(l_ndcmError_code);
        }
        
        l_binary_file.write(reinterpret_cast<char *>(m_cpWorkbuf),m_ndataSize);
        l_binary_file.close();
    }
    catch(...)
    {
        l_ndcmError_code = PD_EXCEPTION;
        CString l_cserrmsg = _T("Exception in writeToFile");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_ndcmError_code);
    }
    
    return(E_NO_ERROR);
}

//AN Added
int CPDDicomFileWriter::AllocWorkBuff(unsigned int f_nsize)
{
    try
    {   
        if(NULL != m_cpWorkbuf)
        {
            delete [] m_cpWorkbuf;
            m_cpWorkbuf = NULL;
        }
        
        m_cpWorkbuf = new char[f_nsize + 2 + NO_ZERO_BYTES + DICM_STR_SZ]; //extra NULL characters before OB group and header size
        
        if(NULL == m_cpWorkbuf)
        {
            CString l_cserrmsg = _T("Can not allocate memory for workbuff.");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return (PD_MEMORY_ALLOCATION);
        }
    }
    catch(...){
        //
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return (PD_MEMORY_ALLOCATION);
    }
    return (E_NO_ERROR);
}
////////////////////////    End Of File     /////////////////////////////////
