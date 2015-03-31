// PDDicomConverter.cpp: implementation of the CPDDicomConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDDicomConverter.h"
#include "ImageInfo.h"
#include "PDTrace.h"

#include "atlconv.h"
#include "winsock2.h"
#include "LocalDBMgr.h"
#include "PDLogHelper.h"
#include "PDDicomFileWriter.h"
#include <io.h>

#include <WinNT.H>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////
//****************************Method Header************************************

//Method Name   : initGroup0002

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0002(Group0002T &f_group0002) {
    PD_TRACE(CPDDicomConverter::initGroup0002);
    int l_nsize=0;
    Group0008T l_group = {
        {0x00020010, 0, NULL,0x0002,0x0010,POSITION_LEVEL},   //VR Transfer_UID  VR-->UI node is image
    };
    
    memcpy(&f_group0002, &l_group, sizeof(l_group));
    this->m_numElementsInGroup02 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0008

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0008(Group0008T &f_group0008) {
    PD_TRACE(CPDDicomConverter::initGroup0008);
    int l_nsize=0;
    Group0008T l_group = {
        {0x00080060, 0, NULL,0x0008,0x0060,SERIES_LEVEL} //VR_AT, ID_MODALITY  VR-->CS node is series
    };
    
    memcpy(&f_group0008, &l_group, sizeof(l_group));
    this->m_numElementsInGroup08 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0010

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0010(Group0010T &f_group0010) 
{
    PD_TRACE(CPDDicomConverter::initGroup0010);
    int l_nsize=0;
    Group0010T l_group = {
        {0x00100010, 0, NULL,0x0010,0x0010,PATIENT_LEVEL},   //VR-->PN node is patient
        {0x00100020, 0, NULL,0x0010,0x0020,PATIENT_LEVEL}    //VR-->LO node is patient
    };
    
    memcpy(&f_group0010, &l_group, sizeof(l_group));
    this->m_numElementsInGroup10 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0018

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0018(Group0018T &f_group0018/*,int &f_numElementsInGroup18*/) 
{
    PD_TRACE(CPDDicomConverter::initGroup0018);
    int l_nsize=0;
    Group0018T l_group = {
        {0x00180050, 0, NULL,0x0018,0x0050,POSITION_LEVEL},  //VR-->DS node is image
        {0x00180088, 0, NULL,0x0018,0x0088,POSITION_LEVEL}   //VR-->DS node is image
        
    };
    
    memcpy(&f_group0018, &l_group, sizeof(l_group));
    this->m_numElementsInGroup18 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0020

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0020(Group0020T &f_group0020/*,int &f_numElementsInGroup20*/) 
{
    PD_TRACE(CPDDicomConverter::initGroup0020);
    int l_nsize=0;
    Group0020T l_group = {
        {0x0020000E, 0, NULL,0x0020,0x000E,SERIES_LEVEL}, //VR-->UI node is series
        {0x00200010, 0, NULL,0x0020,0x0010,STUDY_LEVEL}, //VR-->SH node is study
        {0x00200013, 0, NULL,0x0020,0x0013,POSITION_LEVEL}, //VR-->IS node is Image
        {0x00200032, 0, NULL,0x0020,0x0032,POSITION_LEVEL}, //VR-->DS node is image    
        {0x00200037, 0, NULL,0x0020,0x0037,POSITION_LEVEL}, //VR-->DS node is image
        {0x00201041, 0, NULL,0x0020,0x1041,POSITION_LEVEL}  //VR-->DS node is image
    };
    
    memcpy(&f_group0020, &l_group, sizeof(l_group));
    this->m_numElementsInGroup20 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0028

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0028(Group0028T &f_group0028/*,int &f_numElementsInGroup28*/) 
{
    PD_TRACE(CPDDicomConverter::initGroup0028);
    int l_nsize=0;
    Group0028T l_group = {
        {0x00280010, 0, NULL,0x0028,0x0010,POSITION_LEVEL}, //VR-->US node is image    
        {0x00280011, 0, NULL,0x0028,0x0011,POSITION_LEVEL}, //VR-->US node is image
        {0x00280030, 0, NULL,0x0028,0x0030,POSITION_LEVEL}, //VR-->DS node is image
        {0x00280100, 0, NULL,0x0028,0x0100,POSITION_LEVEL}, //VR-->US node is image
        {0x00280101, 0, NULL,0x0028,0x0101,POSITION_LEVEL}, //VR-->US node is image
        {0x00280103, 0, NULL,0x0028,0x0103,POSITION_LEVEL}, //VR-->US node is image
        {0x00280106, 0, NULL,0x0028,0x0106,POSITION_LEVEL}, //VR-->SS node is image
        {0x00280107, 0, NULL,0x0028,0x0107,POSITION_LEVEL}, //VR-->SS node is image
        {0x00280108, 0, NULL,0x0028,0x0108,SERIES_LEVEL},   //VR-->SS node is image
        {0x00280109, 0, NULL,0x0028,0x0109,SERIES_LEVEL},   //VR-->SS node is image
        //{0x00280120, 0, NULL,0x0028,0x0120,POSITION_LEVEL},   //could not find this tag in MPlusDB
        {0x00281050, 0, NULL,0x0028,0x1050,POSITION_LEVEL}, //VR-->DS node is image
        {0x00281051, 0, NULL,0x0028,0x1051,POSITION_LEVEL}, //VR-->DS node is image
        //{0x00281052, 0, NULL,0x0028,0x1052,POSITION_LEVEL},   //could not find this tag in MPlusDB
        //{0x00281053, 0, NULL,0x0028,0x1053,POSITION_LEVEL}    //could not find this tag in MPlusDB
          {0x00280002, 0, NULL,0x0028,0x0002,POSITION_LEVEL}, //VR-->US node is image //AN/Added
            {0x00280004, 0, NULL,0x0028,0x0004,POSITION_LEVEL}//, //VR-->CS node is image //AN/Added
        
    };
    
    memcpy(&f_group0028, &l_group, sizeof(l_group));
    this->m_numElementsInGroup28 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup0038

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup0038(Group0038T &f_group0038/*,int &f_numElementsInGroup38*/) 
{
    PD_TRACE(CPDDicomConverter::initGroup0038);
    int l_nsize=0;
    Group0038T l_group = {
        {0x00380010, 0, NULL,0x0038,0x0010}    //could not find this tag in MPlusDB
    };
    
    memcpy(&f_group0038, &l_group, sizeof(l_group));
    this->m_numElementsInGroup38 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}
//****************************Method Header************************************

//Method Name   : initGroup7FE0

//Author        :PATNI

//Purpose       : Initialize the elements of the group
//*****************************************************************************
int CPDDicomConverter::initGroup7FE0(Group7FE0T &f_group7FE0/*,int &f_numElementsInGroup7FE0*/) 
{
    PD_TRACE(CPDDicomConverter::initGroup7FE0);
    int l_nsize=0;
    Group7FE0T l_group = {
        {0x7FE00010, 0, NULL,0x7FE0,0x0010,POSITION_LEVEL}     // VR--> OB node is image
    };
    
    memcpy(&f_group7FE0, &l_group, sizeof(l_group));
    this->m_numElementsInGroup7FE0 = (sizeof(l_group)/sizeof(dataElementT));
    l_nsize=(sizeof(l_group)/sizeof(dataElementT));
    return(l_nsize);
}

//****************************Method Header************************************

//Method Name   : CPDDicomConverter

//Author        :PATNI

//Purpose       : Construction
//*****************************************************************************
CPDDicomConverter::CPDDicomConverter():
m_numElementsInGroup02(0),
m_numElementsInGroup08(0),
m_numElementsInGroup10(0),
m_numElementsInGroup20(0),
m_numElementsInGroup28(0),
m_numElementsInGroup38(0),
m_numElementsInGroup18(0),
m_numElementsInGroup7FE0(0),
m_dcmError_code (0),
m_dcm_writer(NULL)
{
    PD_TRACE(CPDDicomConverter::CPDDicomConverter);
}

//****************************Method Header************************************

//Method Name   : ~CPDDicomConverter

//Author        :PATNI

//Purpose       : Destructor
//*****************************************************************************
CPDDicomConverter::~CPDDicomConverter()
{
    PD_TRACE(CPDDicomConverter::~CPDDicomConverter);
    
    m_numElementsInGroup08 = 0;
    if (NULL != m_dcm_writer)
    {
        delete m_dcm_writer;
        m_dcm_writer = NULL;
    }
}
/*************************************************************
* Method:       CPDDicomConverter
* Description:  Copy constructor 
* Parameter:    CPDDicomConverter & f_dcm_converter :
* Returns:      
*************************************************************/
CPDDicomConverter::CPDDicomConverter(CPDDicomConverter& f_dcm_converter)
{
    PD_TRACE(CPDDicomConverter::CPDDicomConverter(CPDDicomConverter&));
    if (NULL != f_dcm_converter.m_dcm_writer)
    {
        m_dcm_writer = new CPDDicomFileWriter(*(f_dcm_converter.m_dcm_writer));
    }
    m_numElementsInGroup02 = f_dcm_converter.m_numElementsInGroup02;
    m_numElementsInGroup08 = f_dcm_converter.m_numElementsInGroup08;
    m_numElementsInGroup10 = f_dcm_converter.m_numElementsInGroup10;
    m_numElementsInGroup20 = f_dcm_converter.m_numElementsInGroup20;
    m_numElementsInGroup28 = f_dcm_converter.m_numElementsInGroup28;
    m_numElementsInGroup38 = f_dcm_converter.m_numElementsInGroup38;
    m_numElementsInGroup18 = f_dcm_converter.m_numElementsInGroup18;
    m_numElementsInGroup7FE0 = f_dcm_converter.m_numElementsInGroup7FE0;
    m_dcmError_code = f_dcm_converter.m_dcmError_code;
}
/*************************************************************
* Method:       CPDDicomConverter
* Description:   = operator overloading 
* Parameter:    CPDDicomConverter & f_dcm_converter :
* Returns:      
*************************************************************/
CPDDicomConverter& CPDDicomConverter::operator = (CPDDicomConverter& f_dcm_converter)
{
    PD_TRACE(CPDDicomConverter::operator=);
    if (this == &f_dcm_converter)
        return(*this);
    
    if (NULL != f_dcm_converter.m_dcm_writer)
    {
        m_dcm_writer = new CPDDicomFileWriter(*(f_dcm_converter.m_dcm_writer));
    }
    m_numElementsInGroup02 = f_dcm_converter.m_numElementsInGroup02;
    m_numElementsInGroup08 = f_dcm_converter.m_numElementsInGroup08;
    m_numElementsInGroup10 = f_dcm_converter.m_numElementsInGroup10;
    m_numElementsInGroup20 = f_dcm_converter.m_numElementsInGroup20;
    m_numElementsInGroup28 = f_dcm_converter.m_numElementsInGroup28;
    m_numElementsInGroup38 = f_dcm_converter.m_numElementsInGroup38;
    m_numElementsInGroup18 = f_dcm_converter.m_numElementsInGroup18;
    m_numElementsInGroup7FE0 = f_dcm_converter.m_numElementsInGroup7FE0;
    m_dcmError_code = f_dcm_converter.m_dcmError_code;
    m_dcmError_code = f_dcm_converter.m_dcmError_code;
    return(*this);
}
//****************************Method Header************************************

//Method Name   : freeMemoryOfGroup

//Author        :PATNI

//Purpose       : Frees the allocated memory of group elements
//*****************************************************************************
int CPDDicomConverter::freeMemoryOfGroup(dataElementT *f_groupElement,const int l_numElements)
{
    PD_TRACE(CPDDicomConverter::freeMemoryOfGroup);
    if (NULL == f_groupElement)
    {
        CString l_cserrmsg = _T("freeMemoryOfGroup parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    try
    {
        int noelement = 0;
        for(noelement = 0; noelement<l_numElements; noelement++) {
            if ((NULL != f_groupElement[noelement].value)) {
                delete []f_groupElement[noelement].value;
                f_groupElement[noelement].value = NULL;
            }
            if ((NULL != f_groupElement[noelement].VR)) {
                delete []f_groupElement[noelement].VR;
                f_groupElement[noelement].VR = NULL;
            }
        }
    }
    catch (...)
    {
        CString l_cserrmsg = _T("freeMemoryOfGroup :  Exception occured");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    return(E_NO_ERROR);
}

//****************************Method Header************************************

//Method Name   : GetDcmErrorcode

//Author        :PATNI

//Purpose       : Get the error code
//*****************************************************************************
int CPDDicomConverter::GetDcmErrorcode(void)
{
    PD_TRACE(CPDDicomConverter::GetDcmErrorcode);
    return(m_dcmError_code);
}
//****************************Method Header************************************

//Method Name   : WriteInDICOMFile

//Author        : PATNI

//Purpose       : Writes information of different groups in DICOM file
//*****************************************************************************
int CPDDicomConverter::WriteInDICOMFile(char *f_finalPath,/*CString& f_studyName*/CImageInfo * f_imginfo)
{
    PD_TRACE(CPDDicomConverter::WriteInDICOMFile);
    int l_nstatus = E_NO_ERROR;
    int l_nbldgrpstatus= E_NO_ERROR;
    Group0002T group0002;
    int numElementsInGroup02 = 0;
    Group0008T group0008;   
    int numElementsInGroup08 = 0;
    Group0010T group0010;
    int numElementsInGroup10 = 0;
    Group0020T group0020;
    int numElementsInGroup20 = 0;
    Group0028T group0028;
    int numElementsInGroup28 = 0;
    Group7FE0T group7FE0;
    int numElementsInGroup7FE0 = 0;
    Group0018T group0018;
    int numElementsInGroup18  = 0;
    try {
        
        if (NULL == m_dcm_writer) {
            m_dcm_writer = new CPDDicomFileWriter();    
        }
        
        if (NULL == m_dcm_writer) {
            CString l_cserr (_T("Dicom File writer is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            return(E_ERROR);
        }
        
        CLocalDBMgr  l_localDBMgr;
        unsigned int l_ntotal_data_length = 0;
        
        // Group 0002
        numElementsInGroup02 = initGroup0002(group0002/*,numElementsInGroup02*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0002),numElementsInGroup02,f_imginfo);
        if (E_NO_ERROR != l_nbldgrpstatus){
            freeMemoryOfGroup((dataElementT *)(&group0002),numElementsInGroup02);
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0002 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0002),numElementsInGroup02); //AN
        // Group 0008
        numElementsInGroup08 = initGroup0008(group0008/*,numElementsInGroup08*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0008),numElementsInGroup08,f_imginfo);
        if (E_NO_ERROR != l_nbldgrpstatus) {
            freeMemoryOfGroup((dataElementT *)(&group0008),numElementsInGroup08);           
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0008 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0008),numElementsInGroup08); //AN
        // Group 0010
        
        numElementsInGroup10 = initGroup0010(group0010/*,numElementsInGroup10*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0010),numElementsInGroup10,f_imginfo);  
        if (E_NO_ERROR != l_nbldgrpstatus){
            freeMemoryOfGroup((dataElementT *)(&group0010),numElementsInGroup10);
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0010 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0010),numElementsInGroup10); //AN
        
        // Group 0018
        numElementsInGroup18 = initGroup0018(group0018/*,numElementsInGroup18*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0018),numElementsInGroup18,f_imginfo);
        if (E_NO_ERROR != l_nbldgrpstatus){
            freeMemoryOfGroup((dataElementT *)(&group0018),numElementsInGroup18);
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0018 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0018),numElementsInGroup18); //AN
        
        // Group 0020
        
        numElementsInGroup20 = initGroup0020(group0020/*,numElementsInGroup20*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0020),numElementsInGroup20,f_imginfo); 
        if (l_nbldgrpstatus != E_NO_ERROR ) {
            freeMemoryOfGroup((dataElementT *)(&group0020),numElementsInGroup20);
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0020 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0020),numElementsInGroup20); //AN
        
        // Group 0028
        
        numElementsInGroup28 = initGroup0028(group0028/*,numElementsInGroup28*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group0028),numElementsInGroup28,f_imginfo);
        if (E_NO_ERROR != l_nbldgrpstatus){
            freeMemoryOfGroup((dataElementT *)(&group0028),numElementsInGroup28);
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group0028 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group0028),numElementsInGroup28); //AN
        
        // Group 0038
        /*Group0038T group0038;
        int numElementsInGroup38;
        numElementsInGroup38 = l_dicomConvert_obj.initGroup0038(group0038,numElementsInGroup38);
        l_bldgrpstatus = l_dicomConvert_obj.buildGroup((dataElementT *)(&group0038),numElementsInGroup38);
        l_dicomConvert_obj.freeMemoryOfGroup((dataElementT *)(&group0038),numElementsInGroup38);
        if (!l_bldgrpstatus){
        m_error_code = l_dicomConvert_obj.GetDcmErrorcode();
        return FALSE;
        }
        */
        // Group 7FE0
        
        numElementsInGroup7FE0 = initGroup7FE0(group7FE0/*,numElementsInGroup7FE0*/);
        l_nbldgrpstatus = l_localDBMgr.buildGroup((dataElementT *)(&group7FE0),numElementsInGroup7FE0,f_imginfo);    
        if (E_NO_ERROR != l_nbldgrpstatus){
            freeMemoryOfGroup((dataElementT *)(&group7FE0),numElementsInGroup7FE0); 
            m_dcmError_code = l_localDBMgr.GetErrorCode();
            CString l_csErrMsg(_T("BuildGroup: Group7FE0 Error in building group."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        l_ntotal_data_length += GetGroupDataSize((dataElementT *)(&group7FE0),numElementsInGroup7FE0); //AN
        
        l_nstatus = m_dcm_writer->AllocWorkBuff(l_ntotal_data_length);
        if(l_nstatus != E_NO_ERROR){
            m_dcmError_code = PD_MEMORY_ALLOCATION;
            CString l_csErrMsg(_T("BuildGroup: Memeory allocation error."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return (E_ERROR);
        }
        
        l_nstatus = m_dcm_writer->writeDICOMHeader();
        if(l_nstatus == E_NO_ERROR){
            
            
            // Group 0002
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0002),numElementsInGroup02);
            freeMemoryOfGroup((dataElementT *)(&group0002),numElementsInGroup02);
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 0002 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return E_ERROR;
            }
            // Group 0008
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0008),numElementsInGroup08);
            freeMemoryOfGroup((dataElementT *)(&group0008),numElementsInGroup08);           
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 0008 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return (E_ERROR);
            }
            // Group 0010
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0010),numElementsInGroup10);
            freeMemoryOfGroup((dataElementT *)(&group0010),numElementsInGroup10);
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 0010 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            
            // Group 0018
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0018),numElementsInGroup18);
            freeMemoryOfGroup((dataElementT *)(&group0018),numElementsInGroup18);
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_csErrMsg = _T("Group 0018 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
                return E_ERROR;
            }
            
            // Group 0020
            
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0020),numElementsInGroup20);
            freeMemoryOfGroup((dataElementT *)(&group0020),numElementsInGroup20);
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 0020 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            
            // Group 0028
            
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group0028),numElementsInGroup28);
            freeMemoryOfGroup((dataElementT *)(&group0028),numElementsInGroup28);
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 0028 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return E_ERROR;
            }
            // Group 7FE0
            l_nstatus = m_dcm_writer->WriteGroupToFile((dataElementT *)(&group7FE0),numElementsInGroup7FE0);    
            freeMemoryOfGroup((dataElementT *)(&group7FE0),numElementsInGroup7FE0); 
            if (E_NO_ERROR != l_nstatus)
            {
                m_dcmError_code = l_nstatus;
                CString l_cserrmsg = _T("Group 7FE0 Writing failed");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            
        } else {
            CString l_cserrmsg = _T("Function writeDICOMHeader() returned an error");			//Mpower 040/2012-08-24/AS/Added
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        int l_nfile_write_status = 0;
        l_nfile_write_status = m_dcm_writer->SetFilePath(f_finalPath);
        if (E_NO_ERROR != l_nfile_write_status)
        {
            m_dcmError_code = l_nfile_write_status;
            CString l_cserrmsg = _T("Function SetFilePath() returned an error");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        l_nfile_write_status = l_nfile_write_status = m_dcm_writer->writeToFile();
        if (E_NO_ERROR != l_nfile_write_status)
        {
            m_dcmError_code = l_nfile_write_status;
            return E_ERROR;
        }
        m_dcm_writer->freeworkbuf();
        m_numElementsInGroup08 = 0;
        
    }
    catch(exception e) 
    {
        CString l_csmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csmsg,__LINE__);
        return E_ERROR;
    }
    
    return(E_NO_ERROR);
}


// AN Added
unsigned int CPDDicomConverter::GetGroupDataSize(dataElementT * f_groupElement,int f_numElements )
{
    PD_TRACE(CPDDicomConverter::freeMemoryOfGroup);
    if (NULL == f_groupElement)
    {
        CString l_cserrmsg = _T("freeMemoryOfGroup parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    int noelement = 0;
    unsigned int l_ntotoal_size = 0;
    for(noelement = 0; noelement<f_numElements; noelement++) 
    {
        l_ntotoal_size += f_groupElement[noelement].length;  //Value length
        l_ntotoal_size += DICOM_VER_BUFF_SZ; //VR
        l_ntotoal_size += sizeof(f_groupElement[noelement].group); //Group
        l_ntotoal_size += sizeof(f_groupElement[noelement].element); //element
        l_ntotoal_size += sizeof(f_groupElement[noelement].length); //length size
        
    }
    return (l_ntotoal_size);
    
    
}
