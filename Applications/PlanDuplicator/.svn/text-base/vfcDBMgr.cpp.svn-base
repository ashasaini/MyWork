// CreateFileName.cpp: implementation of the CCreateFileName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vfcDBMgr.h"
#include "common.h"
#include "atlconv.h"
#include "PDLogHelper.h"
#include "PDTrace.h"
#include "tami\libvfToUi\vfToUiDef.h"
#include "tami\common\bitfields.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CvfcDBMgr
* Description: 	Constructor
* Returns:   	
*************************************************************/
CvfcDBMgr::CvfcDBMgr():m_procInfo(NULL),m_study_name(_T("")),m_vfd(NULL)
{
    PD_TRACE(CvfcDBMgr::CvfcDBMgr);
	memset(m_path,0,IMG_PATH_LEVEL*sizeof(VfPathElem_t));       //AS
//	memset(m_path,0,sizeof(m_path));		//Mpower 040/2012-08-09/AS/Commented
}

/*************************************************************
* Method:    	~CvfcDBMgr
* Description: 	Destructor 
* Returns:   	
*************************************************************/
CvfcDBMgr::~CvfcDBMgr()
{
    PD_TRACE(CvfcDBMgr::~CvfcDBMgr);
    
    if (NULL != m_procInfo){
        delete [] m_procInfo;
        m_procInfo = NULL;
    }
}
/*************************************************************
* Method:       CvfcDBMgr copy constructor
* Description:  This function is used 
* Parameter:    CvfcDBMgr & f_vfcmgr :
* Returns:      
*************************************************************/
CvfcDBMgr::CvfcDBMgr(CvfcDBMgr& f_vfcmgr)
{
    PD_TRACE(CvfcDBMgr::CvfcDBMgr);
    try
    {
		memcpy(m_path,f_vfcmgr.m_path,IMG_PATH_LEVEL*sizeof(VfPathElem_t));//Mpower 040/2012-08-09/AS/Modified /sizeof

        m_study_name = f_vfcmgr.m_study_name;
        memcpy(m_path,f_vfcmgr.m_path,IMG_PATH_LEVEL*sizeof(VfPathElem_t));//Mpower 040/2012-08-09/AS/Modified /sizeof
        m_vfd = NULL;
        if (NULL != f_vfcmgr.m_vfd)
        {
            m_vfd = f_vfcmgr.m_vfd; // study file handle should be same for same study, as it cannot be opened multiple times simultaneously
        }
        if (NULL != m_procInfo){    //Mpower 040/2012-08-09/AS/Added
            delete [] m_procInfo;
        m_procInfo = NULL;
        }
        m_procInfo = new TotalProcessInfo();
        if (NULL == m_procInfo)
        {
            throw new CMemoryException();
        }
        m_procInfo->totalProcessNodes = f_vfcmgr.m_procInfo->totalProcessNodes;
        m_procInfo->processInfo = NULL;
        if (0 < m_procInfo->totalProcessNodes)
        {
            
            m_procInfo->processInfo = new ProcessInfo[m_procInfo->totalProcessNodes];
            if (NULL == m_procInfo->processInfo)
            {
                throw new CMemoryException();
            }
            int l_nprocessnd=0;
            for (l_nprocessnd = 0; l_nprocessnd < m_procInfo->totalProcessNodes; l_nprocessnd++)
            {
                m_procInfo->processInfo[l_nprocessnd].totalEchoNodes =  f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].totalEchoNodes;
                m_procInfo->processInfo[l_nprocessnd].isGDCKind =  f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].isGDCKind;
                m_procInfo->processInfo[l_nprocessnd].echoInfo =  NULL;
                if (0 >= m_procInfo->processInfo[l_nprocessnd].totalEchoNodes)
                {
                    continue;
                } 
                m_procInfo->processInfo[l_nprocessnd].echoInfo = new EchoInfo[m_procInfo->processInfo[l_nprocessnd].totalEchoNodes];
                if (NULL == m_procInfo->processInfo[l_nprocessnd].echoInfo)
                {
                    throw new CMemoryException();
                }
                int l_nechond = 0;
                for (l_nechond = 0; l_nechond < m_procInfo->processInfo[l_nprocessnd].totalEchoNodes; l_nechond++)
                {
                    m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].index = f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].index;
                    m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].totalImages = f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].totalImages;
                    
                }   
            }
        }
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
/*************************************************************
* Method:       operator= overloaded function
* Description:  This function is used 
* Parameter:    CvfcDBMgr & f_vfcmgr :
* Returns:      CvfcDBMgr&
*************************************************************/
CvfcDBMgr& CvfcDBMgr::operator = (CvfcDBMgr& f_vfcmgr)
{//+Patni-VP/2012Aug06/Review/in overloaded assignment operator free memory before allocating new memory
    PD_TRACE(CvfcDBMgr::operator=);
    if (this == &f_vfcmgr)
        return(*this);
    try
    {   
        m_study_name = f_vfcmgr.m_study_name;
        memcpy(m_path,f_vfcmgr.m_path,IMG_PATH_LEVEL*sizeof(VfPathElem_t)); //AS NOTE THIS
        m_vfd = NULL;
        if (NULL != f_vfcmgr.m_vfd)
        {
            m_vfd = f_vfcmgr.m_vfd; // study file handle should be same for same study, as it cannot be opened multiple times simultaneously
        }
        if (NULL != m_procInfo){    //Mpower 040/2012-08-09/AS/Added
            delete [] m_procInfo;
        m_procInfo = NULL;
        }
        m_procInfo = new TotalProcessInfo();
        if (NULL == m_procInfo)
        {
            throw new CMemoryException();
        }
        m_procInfo->totalProcessNodes = f_vfcmgr.m_procInfo->totalProcessNodes;
        m_procInfo->processInfo = NULL;
        if (0 >= m_procInfo->totalProcessNodes)
        {
            return(*this);
        } 
        m_procInfo->processInfo = new ProcessInfo[m_procInfo->totalProcessNodes];
        if (NULL == m_procInfo->processInfo)
        {
            throw new CMemoryException();
        }
        int l_nprocessnd = 0;
        for (l_nprocessnd = 0; l_nprocessnd < m_procInfo->totalProcessNodes; l_nprocessnd++)
        {
            m_procInfo->processInfo[l_nprocessnd].totalEchoNodes =  f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].totalEchoNodes;
            m_procInfo->processInfo[l_nprocessnd].isGDCKind =  f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].isGDCKind;
            m_procInfo->processInfo[l_nprocessnd].echoInfo =  NULL;
            if (0 >= m_procInfo->processInfo[l_nprocessnd].totalEchoNodes)
            {
                continue;
            } 
            m_procInfo->processInfo[l_nprocessnd].echoInfo = new EchoInfo[m_procInfo->processInfo[l_nprocessnd].totalEchoNodes];
            if (NULL == m_procInfo->processInfo[l_nprocessnd].echoInfo)
            {
                throw new CMemoryException();
            }
            int l_nechond = 0;
            for(l_nechond = 0; l_nechond < m_procInfo->processInfo[l_nprocessnd].totalEchoNodes; l_nechond++)
            {
                m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].index = f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].index;
                m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].totalImages = f_vfcmgr.m_procInfo->processInfo[l_nprocessnd].echoInfo[l_nechond].totalImages;
                
            }   
        }
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

/*************************************************************
* Method:    	protTraverse
* Description: 	Traverse through the selected protocol and
*               get no of processed, echo and position nodes
* Parameter: 	const char * f_studyName :
* Parameter: 	const int & f_port_index :
* Parameter: 	vector<CString> & f_csFileName :
* Parameter: 	int & f_ntotalimg :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::protTraverse(const char *f_studyName,const int& f_port_index,vector<CString> &f_csFileName,int &f_ntotalimg)
{
    PD_TRACE(CvfcDBMgr::protTraverse);
	if (NULL == f_studyName)		//Mpower 040/2012-08-09/AS/Added
    {
        CString l_cserrmsg (_T("parameter f_studyName empty in protTraverse()"));//Mpower 040/2012-08-09/AS/Modified
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }

    int l_nTotalImg = 0;
    
    try{
        VfDBHandle_t            l_vfId=NULL;
        VfDBName_t              l_vfName;
        status_t                  l_status=E_NO_ERROR;
        int                     l_numSlices = 0;
        
        // This path order should be maintained 
        VfPathElem_t l_pathP[4] = { SVN_ST_PROTOCOL,   f_port_index, 
            SVN_ST_PROCESSED, 0,
            SVN_ST_ECHO, 0,
            SVN_ST_POSITION,      0
        };
        
        int                     l_ntotSubProtNodes = 0;
        int                     l_ntotSgNodes = 0;
        int                     l_nproc_index=0, l_nechoidx=0 ,l_nposidx=0 ;
        char                    l_tempstr[50]={0};
        CString                 l_csStudyLOID;
        CString                 l_csSeriesLOID;
        CString                 l_csImageLOID;
               
        CString l_studyName(f_studyName);
        int l_nIndex = l_studyName.ReverseFind('/');
        if (-1 == l_nIndex)             //Mpower 040/2012-08-09/AS/Added
        {
            CString l_cserrmsg = _T("ReverseFind() returned -1 (not found)");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
       
        int l_nTempIndexStr = ((l_studyName.GetLength())-(l_nIndex+1));
        CString l_fileNumber = l_studyName.Right(l_nTempIndexStr);  
        CString dcmfilename (_T(""));                
        
        SetStudyName(l_fileNumber); //Mpower 007/AN/Modified
        InitVfPath();
        l_vfName.filename = NULL;
        l_vfName.filename = new char[BUFFER_SIZE_128];
        if (NULL == l_vfName.filename)
        {
            CString l_cserrmsg = _T("protTraverse Memory allocation error");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        memcpy(l_vfName.filename,f_studyName,BUFFER_SIZE_128);
        //l_vfName.filename = (char *)f_studyName;
        l_status = ::VfOpen(VFFT_FILE, l_vfName, O_RDWR, &l_vfId);
        if (NULL != l_vfName.filename)
        {
            delete []l_vfName.filename;
            l_vfName.filename = NULL;
        }
        if (E_NO_ERROR != l_status)
        {
            m_error_code = PD_STUDY_OPEN;
            CString l_cserrmsg = ErrorList[PD_STUDY_OPEN];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        
        //  Get No of subprotocols.
        l_status = GetSubNodes(l_vfId, l_pathP, SVD_ST_PROTOCOL,&l_ntotSubProtNodes);
        if (E_NO_ERROR != l_status){
            CString l_cserrmsg = _T("GetSubNodes: Error.");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        
        if (0 == l_ntotSubProtNodes) {
            m_error_code = PD_NODE_DICOM_CREATION;
            CString l_cserrmsg = ErrorList[m_error_code];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }else{
            //dummy else
        }
        
        m_procInfo =  new TotalProcessInfo();
        m_procInfo->processInfo = new ProcessInfo[l_ntotSubProtNodes];
        m_procInfo->totalProcessNodes = l_ntotSubProtNodes;
        
        if (NULL == m_procInfo){
            m_error_code = PD_MEMORY_ALLOCATION;
            return(E_ERROR);
        }else{
            //dummy else
        }
        
        //    Get no of slice groups in each subprotocol.
        for(l_nproc_index = 0; l_nproc_index < l_ntotSubProtNodes; l_nproc_index++) {
            l_pathP[SVD_ST_PROCESSED -1].index = l_nproc_index;
            
            if (1 != Get_GDC_Image_Kind(l_vfId,l_pathP )) {
                m_procInfo->processInfo[l_nproc_index].isGDCKind = FALSE;
                continue;
            }else{
                //dummy else
            }
            
            m_procInfo->processInfo[l_nproc_index].isGDCKind = TRUE;
            l_status = GetSubNodes(l_vfId, l_pathP, SVD_ST_PROCESSED,&l_ntotSgNodes);
            if (E_NO_ERROR != l_status){
                CString l_cserrmsg = _T("GetSubNodes: Error.");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            m_procInfo->processInfo[l_nproc_index].totalEchoNodes = l_ntotSgNodes;
            
            if (0 == l_ntotSgNodes){
                m_error_code = PD_NODE_DICOM_CREATION;
                return(E_ERROR);
            }else{
                //dummy else
            }
            
            m_procInfo->processInfo[l_nproc_index].echoInfo = new EchoInfo[l_ntotSgNodes];
            
            if (NULL == m_procInfo->processInfo[l_nproc_index].echoInfo){
                m_error_code = PD_MEMORY_ALLOCATION;
                return(E_ERROR);
            }else{
                //dummy else
            }
            
            for(l_nechoidx = 0; l_nechoidx < l_ntotSgNodes; l_nechoidx++) {  
                l_pathP[SVD_ST_ECHO -1].index = l_nechoidx;
                l_status = GetSubNodes(l_vfId, l_pathP, SVD_ST_ECHO,&l_numSlices);            
                
                if (E_NO_ERROR != l_status){
                    CString l_cserrmsg = _T("GetSubNodes: Error.");
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return(E_ERROR);
                }
                m_procInfo->processInfo[l_nproc_index].echoInfo[l_nechoidx].index = l_nechoidx;
                m_procInfo->processInfo[l_nproc_index].echoInfo[l_nechoidx].totalImages = l_numSlices;
                
                if (0 == l_numSlices) {              
                    m_error_code = PD_NODE_DICOM_CREATION;
                    return(E_ERROR);
                } else{
                    //dummy else
                }
                
                l_nTotalImg =+ /*l_nTotalImg*/l_numSlices;
                
                for(l_nposidx = 0;l_nposidx < l_numSlices;l_nposidx++){
                    
                    sprintf(l_tempstr,"%d.%d.%d.%d.%d.%d.%d.%d",
                        SVN_ST_PROTOCOL,f_port_index,
                        SVN_ST_PROCESSED,l_nproc_index,SVN_ST_ECHO,
                        l_nechoidx,SVN_ST_POSITION,l_nposidx);
                    dcmfilename = l_fileNumber + l_tempstr + _T(".dcm");    
                    
                    GetLOID(SVD_ST_STUDY,l_csStudyLOID);
                    GetLOID(SVD_ST_PROCESSED,l_csSeriesLOID);
                    GetLOID(SVD_ST_POSITION,l_csImageLOID);
                    dcmfilename = dcmfilename + _T(" ") + l_csStudyLOID + _T(" ") + l_csSeriesLOID + _T(" ") + l_csImageLOID;
                    f_csFileName.push_back(dcmfilename);
                }
            }           
        }
        
        ::VfClose(l_vfId);
    }catch(...){
        m_error_code = PD_EXCEPTION;
        return(E_ERROR);
    }
    
    f_ntotalimg =  l_nTotalImg;
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	GetSubNodes
* Description: 	Get total no of subnodes
* Parameter: 	VfDBHandle_t f_srcVf :
* Parameter: 	VfPathElem_t f_pathP :
* Parameter: 	int f_curLevel :
* Parameter: 	int * f_ntotalsubnodes :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::GetSubNodes(VfDBHandle_t f_srcVf,
                           VfPathElem_t f_pathP[],
                           int f_curLevel,
                           int *f_ntotalsubnodes)
{
    PD_TRACE(CvfcDBMgr::GetSubNodes);
    int l_ntotalSubNodes = MAX_VF_BRANCHES;
    BITFLD_DECL(vfBranches, MAX_VF_BRANCHES);
    BITFLD_INIT(vfBranches, MAX_VF_BRANCHES, 0);
    
    try{
        
        if ((NULL == f_pathP) || (NULL == vfBranches) || (NULL == f_ntotalsubnodes)){
            CString l_cserr (_T("GetSubNodes parameter NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            return(E_ERROR); // -1 indicates error
        }
        int l_nactiveSubNodes = 0;
        status_t l_ret=E_NO_ERROR;
        l_ret = ::VfScanSubtree(f_srcVf, f_pathP, f_curLevel, 
            f_pathP[f_curLevel].subtree_name,vfBranches, &l_ntotalSubNodes,
            &l_nactiveSubNodes);
        
        if (E_NO_ERROR != l_ret){
            CString l_cserrmsg = _T("VfScanSubtree Error");
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR) ;
        }else{
            //dummy else
        }
    }
    catch(...){
        m_error_code = PD_EXCEPTION;
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    *f_ntotalsubnodes = l_ntotalSubNodes;
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	Get_GDC_Image_Kind
* Description: 	Get Get_GDC_Image_Kind of the image
* Parameter: 	VfDBHandle_t f_vfId :
* Parameter: 	VfPathElem_t f_pathP :
* Returns:   	int32_t
*************************************************************/
int32_t CvfcDBMgr::Get_GDC_Image_Kind(VfDBHandle_t f_vfId,VfPathElem_t f_pathP[])
{
    
    PD_TRACE(CvfcDBMgr::Get_GDC_Image_Kind);
    int32_t l_GDC_Img_kind= -1;     //Mpower 030/2012-06-26/AS/Modified/cpp change
    VfFieldSpec_t   l_vfReq[1]={0};
    
    try{
        int l_n = 0;
        VFF_SET_ARGS(l_vfReq[l_n], SVN_PRC_GDC_IMAGE_KIND,
            SVT_PRC_GDC_IMAGE_KIND, sizeof(l_GDC_Img_kind), &l_GDC_Img_kind, 0) ;
        l_n++;
        
        int  l_numget = 0;       //Mpower 030/2012-06-26/AS/Modified/cpp change
        if (E_NO_ERROR != VfGetFields(f_vfId, f_pathP, SVD_ST_PROCESSED, 
            l_vfReq, l_n, &l_numget))
        {
            l_GDC_Img_kind = -1;
        }else{
            //dummy else
        }
    }
    catch(...){
        return(E_ERROR);
    }
    
    return(l_GDC_Img_kind);
}

/*************************************************************
* Method:    	CheckIsProtocolScanned
* Description: 	checks the given protocol is scanned or unscanned
* Parameter: 	int f_nprotIndx :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::CheckIsProtocolScanned(int f_nprotIndx,int &f_prot_type)  //Mpower 030/2012-06-26/AS/Modified/cpp change
{
    PD_TRACE(CvfcDBMgr::CheckIsProtocolScanned);
    VfPathElem_t    l_NodePath[PROT_PATH_LEVEL]={0};
    VfFieldSpec_t   l_VfReq[1]={0};
    int         l_nacq_status = 0;
    int             l_nnum_get = 0;
    int  l_nn = 0;
    int l_nsts = 0;
    
    l_NodePath[0].subtree_name = SVN_ST_PROTOCOL;
    l_NodePath[0].index = f_nprotIndx;    
    l_nn = 0 ;
    VFF_SET_ARGS(l_VfReq[l_nn],SVN_EXE_PROTOCOL_ACQ_STATUS,
        SVT_EXE_PROTOCOL_ACQ_STATUS,
        sizeof(l_nacq_status), &l_nacq_status, 0); 
      
    l_nn++;    
    l_nsts = ::VfGetFields(m_vfd, l_NodePath,SVD_ST_PROTOCOL, l_VfReq, l_nn, &l_nnum_get);
    
    if ( E_NO_ERROR != l_nsts ){
        fprintf(stdout, "VfGetFields error (0x%08x)\n", l_nsts);
        fflush(stdout);
        return(E_ERROR);
    }
    if(VFC_ACQ_STATUS_SUCCESS == l_nacq_status){
        f_prot_type = SCANNED;
    } else if(VFC_ACQ_STATUS_WAIT == f_prot_type){
        f_prot_type = UNSCANNED;
    }else{
        f_prot_type = SCAN_FAIL;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	OpenDBFile
* Description: 	checks the given protocol is scanned or unscanned
* Parameter: 	const CString & f_sfileName :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::OpenDBFile(const CString &f_sfileName)
{
     PD_TRACE(CvfcDBMgr::OpenDBFile);
    //Mpower 040/2012-08-09/AS/Added
     if(TRUE == f_sfileName.IsEmpty()){
             CString l_csErrMsg = _T("Parameter f_sfileName empty in OpenDBFile()");
             CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
             return(E_ERROR);
         }  
     //Mpower 040/2012-08-09/AS/Ends
   
    VfDBName_t l_name;
    int l_nstatus = 0;
    char* l_cpStudyfilename = NULL;
    CString l_csErrMsg ;
    try
    {
        l_cpStudyfilename = new char[BUFFER_SIZE_128];
        if (NULL == l_cpStudyfilename){
            CString l_csErrMsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        sprintf(l_cpStudyfilename,"%S",f_sfileName);
        l_name.filename = l_cpStudyfilename;
        l_nstatus = ::VfOpen(VFFT_FILE,l_name,O_RDONLY,&m_vfd);
        
        if( NULL != l_cpStudyfilename ) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;
            if( E_NO_ERROR != l_nstatus ) {    
                
                l_csErrMsg.Format(_T("vfopen error"));
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERROR_LOGONLY);
                
                return(E_ERROR);
            }
        }
    }
    catch (CMemoryException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        if( NULL != l_cpStudyfilename ) {
            delete []l_cpStudyfilename; //Mpower 030/2012-06-26/AS/Modified/cpp change
            l_cpStudyfilename = NULL;
            return(E_ERROR);
            
        }
    }
    catch (CException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        if( NULL != l_cpStudyfilename ) {
            delete []l_cpStudyfilename; //Mpower 030/2012-06-26/AS/Modified/cpp change
            l_cpStudyfilename = NULL;
            return(E_ERROR);
            
        }
    }
    catch (...)
    {
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        if( NULL != l_cpStudyfilename ) {
            delete []l_cpStudyfilename; //Mpower 030/2012-06-26/AS/Modified/cpp change
            l_cpStudyfilename = NULL;
            return(E_ERROR);
        }
    }
    
    return(E_NO_ERROR);
}

/**************************************************************
* Method:    	getProtAcqOrder
* Description: 	This function reads order of protocol present
*               in given study file
*               and stores in the form of array
* Parameter: 	int * f_pnarray :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::getProtAcqOrder(int *f_pnarray)     //Mpower 030/2012-06-26/AS/Modified/cpp change
{
    PD_TRACE(CvfcDBMgr::getProtAcqOrder);
    status_t      l_nstatus = 0;
    VfFieldSpec_t l_fieldSpec[1]={0};
    int           l_nnumDone = 0;
    int           l_narq_order[BUFFER_SIZE_256] ={0};
    int           l_nacqOrderSize =0;   
    if (NULL == f_pnarray)
    {
        CString l_cserrmsg (_T("getProtAcqOrder() out parameter NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_nacqOrderSize);
    }
    VFF_SET_ARGS(l_fieldSpec[0],SVN_PLN_PROT_ACQ_ORDER,VFT_INT32,
        sizeof(l_narq_order),l_narq_order,VFO_VARIABLE_LEN);    
    l_nstatus = ::VfGetFields(m_vfd,NULL,SVD_ST_STUDY,l_fieldSpec,1,&l_nnumDone);
    
    if ((0 != l_nstatus)||(0 >= l_nnumDone) ) {
        CString l_cserrmsg = _T("getProtAcqOrder() VfGetFields error");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_nacqOrderSize);
    }
    
    l_nacqOrderSize = 0;
    l_nacqOrderSize = l_fieldSpec[0].size/sizeof(int);
    memcpy(f_pnarray,l_narq_order,l_fieldSpec[0].size);    
    return(l_nacqOrderSize);    
}

/*************************************************************
* Method:    	GetLOID
* Description: 	Get the LOID at specified path
* Parameter: 	int f_node_depth :
* Parameter: 	CString & f_study_loid :
* Returns:   	status_t
*************************************************************/
status_t CvfcDBMgr::GetLOID(int f_node_depth,
                            CString &f_study_loid)
{
    PD_TRACE(CvfcDBMgr::GetLOID);
    VfDBName_t          l_vf_name;
    VfDBHandle_t        l_vfstudy_handle=NULL;
    VfFieldSpec_t       l_fieldspec[2]={0};
    status_t            l_status = E_NO_ERROR;
    int                 l_no = 0;   
    int                 l_num_done=0;
    int                 l_nsize=0;
    
    if (TRUE == m_study_name.IsEmpty()) {
        CString l_csErrMsg = "GetLOID() Study file name is empty";
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return E_ERROR;
    }
    
    try{
        //Open varfield database 
        CString l_sites = _wgetenv(SITE);
        CString l_study_path = l_sites + _T("/") + STUDY_PATH + _T("/") + m_study_name;
        USES_CONVERSION;
        l_vf_name.filename  = W2A(l_study_path);
        l_status = ::VfOpen( VFFT_FILE, l_vf_name, O_RDWR, &l_vfstudy_handle );
        
        if_error(l_status) {
            m_error_code = PD_STUDY_OPEN;
            return(E_ERROR);
        }
        
        //Fill field specifications
        VFF_SET_ARGS(l_fieldspec[l_no], SVN_PLN_LDB_MAPPED_ID, 
            SVT_PLN_LDB_MAPPED_ID,0, NULL, VFO_VARIABLE_LEN);
        l_no++;
        
        //Get data from varfield database
        l_status = ::VfGetFields(l_vfstudy_handle, m_path, f_node_depth,
            l_fieldspec, l_no, (int*)&l_num_done);
        
        if_error(l_status) {
            ::VfClose( l_vfstudy_handle );
            m_error_code = PD_STUDY_OPEN;
            CString l_csErrMsg = ErrorList[PD_STUDY_OPEN];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        } else if ( l_num_done != l_no ) {
            ::VfClose( l_vfstudy_handle );
            m_error_code = PD_STUDY_OPEN;
            CString l_csErrMsg = ErrorList[PD_STUDY_OPEN];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }else{
            //dummy else
        }
        
        l_nsize = l_fieldspec[0].size ;
        l_no = 0;
        char *l_cbuff = NULL;
        l_cbuff = new char[l_nsize];
        
        if (NULL == l_cbuff){
            m_error_code = PD_MEMORY_ALLOCATION;
            CString l_csErrMsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            return(E_ERROR);
        }
        
        VFF_SET_ARGS(l_fieldspec[l_no], SVN_PLN_LDB_MAPPED_ID, 
            SVT_PLN_LDB_MAPPED_ID,l_nsize, l_cbuff, VFO_VARIABLE_LEN); 
        
        l_no++;
        l_status = ::VfGetFields(l_vfstudy_handle, m_path, f_node_depth,
            l_fieldspec, l_no, (int*)&l_num_done);
        
        if_error(l_status) {
            m_error_code = PD_STUDY_OPEN;
            l_status = E_ERROR;
        } 
        else if ( l_num_done != l_no ) {
            m_error_code = PD_STUDY_OPEN;
            l_status = E_ERROR;
        }
        else {
            f_study_loid = l_cbuff;
        }
        
        //Close varfield database 
        ::VfClose( l_vfstudy_handle );
        
        if ( NULL != l_cbuff ) {
            delete[] l_cbuff;
            l_cbuff = NULL;
        }
    }catch(...){
        m_error_code = PD_EXCEPTION;
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        l_csErrMsg += _T(" in GetLOID()");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
//         if ( NULL != l_cbuff ) {
//             delete[] l_cbuff;
//             l_cbuff = NULL;
//         }
        return(E_ERROR);
    }
    
    return(l_status);
}

/*************************************************************
* Method:    	SetStudyName
* Description: 	Set the study file name
* Parameter: 	const CString & f_study_name :
* Returns:   	void
*************************************************************/
void CvfcDBMgr::SetStudyName(const CString& f_study_name)
{
    PD_TRACE(CvfcDBMgr::SetStudyName);
    m_study_name = f_study_name;
}

/*************************************************************
* Method:    	InitVfPath
* Description: 	Set the study file name
* Returns:   	void
*************************************************************/
void CvfcDBMgr::InitVfPath()
{
    PD_TRACE(CvfcDBMgr::InitVfPath);
    memset(m_path,0,IMG_PATH_LEVEL*sizeof(VfPathElem_t));   //Mpower 040/2012-08-09/AS/Modified/sizeof value changed
    int l_ni = 0;
    for(l_ni = 0;l_ni < 4;l_ni++){
        m_path[l_ni].index = 0;
        switch(l_ni){
        case 0:
            m_path[l_ni].subtree_name = SVN_ST_PROTOCOL;
            break;
        case 1:
            m_path[l_ni].subtree_name = SVN_ST_PROCESSED;
            break;
        case 2:
            m_path[l_ni].subtree_name = SVN_ST_ECHO;
            break;
        case 3:
            m_path[l_ni].subtree_name = SVN_ST_POSITION;
            break;
        default:
            break;
        }
    }
}

/*************************************************************
* Method:    	CloseDBFile
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CvfcDBMgr::CloseDBFile() 
{
    PD_TRACE(CvfcDBMgr::CloseDBFile);
    
    try
    {
        ::VfClose(m_vfd);
    }
    
    catch (...)
    {
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetSliceThickness
* Description: 	returns slice thickness from varfield study file 
* Parameter: 	float * f_fslice_thickness :
* Parameter: 	const int & f_nprotindex :
* Returns:   	int
*************************************************************/
int  CvfcDBMgr::GetSliceThickness(float *f_fslice_thickness,const int &f_nprotindex)
{
    PD_TRACE(CvfcDBMgr::GetSliceThickness);
    int          plStatus=E_NO_ERROR;
    VfPathElem_t      pathSG[VG_PATH_LEVEL]={0};
    
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
    int n=0; 
    int num =0;
    
    
    if (NULL == f_fslice_thickness)
    {
        CString l_cserrmsg = _T("GetSliceThickness: Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }else{
        //dummy else
    }
    
    pathSG[0].subtree_name = SVN_ST_PROTOCOL;
    pathSG[0].index    = f_nprotindex;
    
    pathSG[1].subtree_name = SVN_ST_SUBPROTOCOL;
    pathSG[1].index    = 0;
    
    pathSG[2].subtree_name = SVN_ST_SLICEGROUP;
    pathSG[2].index    = 0;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_THICKNESS, SVT_PLN_THICKNESS,
        sizeof(flt32_t), f_fslice_thickness, 0);
    n++ ;    
    plStatus = ::VfGetFields(m_vfd,pathSG,SVD_ST_SLICEGROUP,vfReq,
        n,(int *)&num) ;
    
    if_error(plStatus){
        //trace errro
        return(plStatus);
    }
    
    return(E_NO_ERROR);          
}

/*************************************************************
* Method:    	GetSliceGap
* Description: 	returns slice gap from varfield study file
* Parameter: 	float * f_fslice_gap :
* Parameter: 	const int & f_nprotindex :
* Returns:   	status_t
*************************************************************/
status_t  CvfcDBMgr::GetSliceGap(float *f_fslice_gap,const int &f_nprotindex)
{
    PD_TRACE(CvfcDBMgr::GetSliceGap);
    status_t            plStatus=E_NO_ERROR;
    
    VfPathElem_t      pathSG[VG_PATH_LEVEL]={0};
    
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
    int n=0; 
    int num =0;
    
    
    if (NULL == f_fslice_gap){
        CString l_cserrmsg = _T("GetSliceGap: Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }else{
        //dummy else
    }
    
    pathSG[0].subtree_name = SVN_ST_PROTOCOL;
    pathSG[0].index    = f_nprotindex;
    
    pathSG[1].subtree_name = SVN_ST_SUBPROTOCOL;
    pathSG[1].index    = 0;
    
    pathSG[2].subtree_name = SVN_ST_SLICEGROUP;
    pathSG[2].index    = 0;
    
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SLICE_GAP, SVT_PLN_SLICE_GAP,
        sizeof(float), f_fslice_gap, 0);
    n++ ;    
    plStatus = ::VfGetFields(m_vfd,pathSG,SVD_ST_SLICEGROUP,vfReq,n,
        (int *)&num) ;
    
    if_error(plStatus){
        //trace errro
        return(plStatus);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	GetSliceNum
* Description: 	returns number of slices from varfield study file
* Parameter: 	int * f_nslice_num :
* Parameter: 	const int & f_nprotindex :
* Returns:   	status_t
*************************************************************/
status_t  CvfcDBMgr::GetSliceNum(int *f_nslice_num,const int &f_nprotindex)
{
    PD_TRACE(CvfcDBMgr::GetSliceNum);
    status_t            plStatus=E_NO_ERROR;
    VfPathElem_t      pathSG[VG_PATH_LEVEL]={0};
    
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
    int n=0; 
    int num =0;
    
    
    if (NULL == f_nslice_num){
        CString l_cserrmsg = _T("GetSliceNum: Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }else{
        //dummy else
    }
    
    pathSG[0].subtree_name = SVN_ST_PROTOCOL;
    pathSG[0].index    = f_nprotindex;
    
    pathSG[1].subtree_name = SVN_ST_SUBPROTOCOL;
    pathSG[1].index    = 0;
    
    pathSG[2].subtree_name = SVN_ST_SLICEGROUP;
    pathSG[2].index    = 0;
    
    
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SLICES_IN_SG, SVT_PLN_SLICES_IN_SG,
        sizeof(int), f_nslice_num, 0);
    n++ ;    
    plStatus = ::VfGetFields(m_vfd,pathSG,SVD_ST_SLICEGROUP,vfReq,
        n,(int *)&num) ;
    
    if_error(plStatus){
        //trace errro
        return(plStatus);
    }
    
    return(E_NO_ERROR);
}

//****************************Method Header************************************
//Method Name   : GetPLNHold

//Author        :PATNI

//Purpose       : returns PLNHold value from varfield study file
//*****************************************************************************
// int CvfcDBMgr::GetPLNHold(int *f_nplnhold,const int &f_nprotindex)
// {
//     PD_TRACE(CvfcDBMgr::GetPLNHold);
//     int            plStatus=E_NO_ERROR;
//     
//     VfPathElem_t      pathSG[VG_PATH_LEVEL]={0};
//     
//     VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
//     int n=0; 
//     int num =0;
//     
//     
//     if (NULL == f_nplnhold){
//         CString l_cserrmsg = _T("GetPLNHold: Parameter NULL");
//         CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
//         return(E_ERROR);
//     }else{
//         //dummy else
//     }
//     
//     pathSG[0].subtree_name = SVN_ST_PROTOCOL;
//     pathSG[0].index    = f_nprotindex;
//     
//     pathSG[1].subtree_name = SVN_ST_SUBPROTOCOL;
//     pathSG[1].index    = 0;
//     
//     pathSG[2].subtree_name = SVN_ST_SLICEGROUP;
//     pathSG[2].index    = 0;
//     
//     VFF_SET_ARGS(vfReq[n], SVN_PLN_HOLD, SVT_PLN_HOLD,sizeof(int),
//         f_nplnhold, 0);
//     n++ ;    
//     plStatus = ::VfGetFields(m_vfd,pathSG,SVD_ST_PROTOCOL,vfReq,
//         n,(int *)&num) ;
//     
//     if_error(plStatus){
//         //trace errro
//         return(plStatus);
//     }
//     
//     return(E_NO_ERROR);
// }

/*************************************************************
* Method:    	GetFOV
* Description: 	returns FOV values from varfield study file
* Parameter: 	f32vec2_t * f_fov :
* Parameter: 	const int & f_nprotindex :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::GetFOV(f32vec2_t *f_fov,const int &f_nprotindex)
{
    PD_TRACE(CvfcDBMgr::GetFOV);
    status_t            plStatus=E_NO_ERROR;
    
    VfPathElem_t      pathSG[VG_PATH_LEVEL]={0};
    
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
    int n=0; 
    int num =0;
    
    
    if (NULL == f_fov){
        CString l_cserrmsg = _T("GetFOV: Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }else{
        //dummy else
    }
    
    pathSG[0].subtree_name = SVN_ST_PROTOCOL;
    pathSG[0].index    = f_nprotindex;
    
    pathSG[1].subtree_name = SVN_ST_SUBPROTOCOL;
    pathSG[1].index    = 0;
    
    pathSG[2].subtree_name = SVN_ST_SLICEGROUP;
    pathSG[2].index    = 0;
    
    VFF_SET_ARGS(vfReq[n], SVN_PLN_FOV, SVT_PLN_FOV,
        sizeof(f32vec2_t), f_fov, 0);
    n++ ;    
    plStatus = ::VfGetFields(m_vfd,pathSG,SVD_ST_PROTOCOL,
        vfReq,n,(int *)&num) ;
    
    if_error(plStatus){
        //trace errro
        return(E_ERROR);
    }
    
    return(E_NO_ERROR);
}
/*************************************************************
* Method:       TraverseProt
* Description:  This function is used to traverse the study file and get protocol information
* Parameter:    char * f_scartchpad_name :
* Returns:      int
*************************************************************/
int CvfcDBMgr::TraverseProt(const char * f_scartchpad_name)
{
    PD_TRACE(CvfcDBMgr::TraverseProt);
    VfPathElem_t    l_pathVG[SERIES_PATH_LEVEL] = {0};
    status_t        l_plstatus = 0;
    VfDBHandle_t    l_vfhandle = NULL;
    VfDBName_t      l_vfname;
    int             l_ntotalVGNodes = 0;
    int             l_nactiveVGNodes = 0;
    BITFLD_DECL(mask,MAX_BITFLD);
    
    BITFLD_INIT(mask, MAX_BITFLD, 0);
    try
    {
        if (NULL == f_scartchpad_name)
        {
            CString l_cserr = _T("TraverseProt() Study file name is empty");
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            return(E_ERROR);
        }
        
        l_vfname.filename = (char *)f_scartchpad_name;
        
        l_pathVG[0].subtree_name = SVN_ST_PROTOCOL;
        l_pathVG[0].index = 0;
        
        l_pathVG[1].subtree_name = SVN_ST_VISUALGROUP;
        l_pathVG[1].index = 0;
        
        if_error(VfOpen(VFFT_FILE,l_vfname,O_RDONLY,&l_vfhandle))
        {
            CString l_cserrmsg (_T("E : *** Function VfOpen() returned an error.\n"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        
        /*if_error(VfScanSubtree(l_vfhandle,l_pathVG,SVD_ST_PROTOCOL,SVN_ST_VISUALGROUP,mask,&l_ntotalVGNodes,&l_nactiveVGNodes))
        {
        //printf
        return E_ERROR;
    }*/
    }
    
    catch (...)
    {
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
    
    return E_NO_ERROR;
}
/*************************************************************
* Method:       GetScannedCenterSliceNumber
* Description:  This function is used 
* Parameter:    int f_nprotindex :
* Parameter:    int & f_nProcIdx :
* Returns:      int
*************************************************************/
int CvfcDBMgr::GetScannedCenterSliceNumber(const int &f_nprotindex,int &f_nProcIdx)
{
    PD_TRACE(CvfcDBMgr::GetScannedCenterSliceNumber);
    int            plStatus=E_NO_ERROR;
    
    VfPathElem_t      pathVG[SERIES_PATH_LEVEL]={0};
    
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8]={0};
    int n=0; 
    int num =0;
    int l_nSliceNumber = 0;
    
    
    pathVG[0].subtree_name = SVN_ST_PROTOCOL;
    pathVG[0].index    = f_nprotindex;
    
    pathVG[1].subtree_name = SVN_ST_VISUALGROUP;
    pathVG[1].index    = 0;
    
    n = 0;
    
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SLICES_IN_VG, SVT_PLN_SLICES_IN_VG,sizeof(int), &l_nSliceNumber, 0);
    n++ ;    
    plStatus = ::VfGetFields(m_vfd,pathVG,SVD_ST_VISUALGROUP,vfReq,n,(int *)&num) ;
    
    if_error(plStatus){
        //trace errro
        return(plStatus);
    }
    if(l_nSliceNumber % 2){
        l_nSliceNumber = (int)((l_nSliceNumber-1)/2);
    }else{
        l_nSliceNumber = (int)(l_nSliceNumber/2);
    }
    protTraverseGDC(f_nprotindex,f_nProcIdx);
    return(l_nSliceNumber);
}

/*************************************************************
* Method:       GetStudyName
* Description:  This function is used to return name of study file
* Returns:      CString
*************************************************************/
// CString CvfcDBMgr::GetStudyName()
// {
//     return(m_study_name);
// }

/*************************************************************
* Method:       protTraverseGDC
* Description:  This function is used to get processed node index of GDC kind series
* Parameter:    const int & f_ScannedPortIdx :
* Parameter:    int & f_processIdx :
* Returns:      int
*************************************************************/
int CvfcDBMgr::protTraverseGDC(const int &f_ScannedPortIdx,int &f_processIdx)
{   
    int32_t                 numSlices = 0;
    int32_t                 numImages = 0;
    VfPathElem_t            pathP[SERIES_PATH_LEVEL];
    int                     totSubProtNodes = 0;
    int                     totSgNodes = 0;
    int                     i;
    int                     numDone = 0;
    int                     retVal = 0;
    int                     firstTime = 1;
    int                     phaseCorrection = 0;
    int                     Speeder = 0;
    status_t plStatus = E_NO_ERROR;
    
    int idx = 0, n=0, numget = 0;       //Mpower 030/2012-06-26/AS/Modified/cpp change
    int32_t l_GDC_Img_kind= -1;     //Mpower 030/2012-06-26/AS/Modified/cpp change
    VfFieldSpec_t   vfReq[BUFFER_SIZE_8];
    status_t   status = E_NO_ERROR;
    
    pathP[0].subtree_name = SVN_ST_PROTOCOL;
    pathP[0].index = f_ScannedPortIdx;
    pathP[1].subtree_name = SVN_ST_PROCESSED;
    pathP[1].index = 0;
    
    //    Get No of subprotocols.
    plStatus = GetSubNodes(m_vfd, pathP, SVD_ST_PROTOCOL,&totSubProtNodes);
    if(0 == totSubProtNodes){
        CString l_cserrmsg (_T("NO totSubProtNodes SVD_ST_PROTOCOL \n"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERROR_LOGONLY);
        return(totSubProtNodes);
    }
    
    //    Get no of slice groups in each subprotocol.
    for(i = 0; i < totSubProtNodes; i++)
    {
        pathP[SVD_ST_PROCESSED -1].index = i;
        n = 0;
        VFF_SET_ARGS(vfReq[n], SVN_PRC_GDC_IMAGE_KIND,
            SVT_PRC_GDC_IMAGE_KIND, sizeof(l_GDC_Img_kind), &l_GDC_Img_kind, 0) ;
        n++;
        if(E_NO_ERROR != VfGetFields(m_vfd, pathP, SVD_ST_PROCESSED, vfReq, n, &numget))
        {
            l_GDC_Img_kind = -1;
            //printf("Unable to get GDC kind.\n",i);
            CString l_cserrmsg (_T("Unable to get GDC kind.\n"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERROR_LOGONLY);
        }
        if(1 == l_GDC_Img_kind){
            f_processIdx = i;
            printf("process index in protTraverseGDC =[%d]\n",i);            
            break;
        }
        
    } 
    printf("process index in protTraverseGDC =[%d]\n",f_processIdx);
    return(E_NO_ERROR);
}
////////////////////////    End Of File     /////////////////////////////////
