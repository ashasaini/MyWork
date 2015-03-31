// PDController.cpp: implementation of the CPDController class.
//MPower 007/AN/Added
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDController.h"
#include "vfcDBMgr.h"
#include "PDProcessExec.h"
#include "PDTrace.h"

#include "DPSComManager.h"
#include "PDDataFileReader.h"
#include "IniFileReader.h"
#include "PDLogHelper.h"
#include "PDDicomConverter.h"
#include <conio.h>
#include "PDCalculation.h"
#include "DcmController.h"
#include "PDErrorMsgDlg.h"
//#include "ScrPadMgr.h"
//#include "RelimitMgr.h"
#include "PlanUpdate.h"
#include "PlanUpdateMPR.h"
#include "PlanUpdateVOI.h"

extern "C" plUpdate();
extern "C" int Relimit(const char *studyFileName,
                       const int  sourceProtocolIndex,
                       const relimitUpdateParams_t *f_relimitUpdateParams);
//extern "C" DebugInit();

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*************************************************************
* Method:    	CPDController
* Description: 	This function is a constructor for initialising
*               CPDController  class variables.
* Returns:   	NONE
*************************************************************/
CPDController::CPDController() : m_pdexec(NULL),m_comm(NULL) ,
m_vfc_mgr(NULL) ,m_ini_reader(NULL), m_app_mode(SMART_VOI),m_nScannedProtIdx(0),  m_csstudy_file_name (_T("")), m_nProcIdx (0)
{
   // DebugInit();
    PD_TRACE(CPDController::CPDController);
}

CPDController::~CPDController()
{
    PD_TRACE(CPDController::~CPDController);
    Close();
}
	//Mpower 039//Jul-12/Added/AN/CPP Test
/*************************************************************
* Method:    	CPDController
* Description: 	This function is used 
* Parameter: 	CPDController & f_controller :
* Returns:   	
*************************************************************/
CPDController::CPDController(CPDController& f_controller)
{
    PD_TRACE(CPDController::CPDController);
    m_pdexec = NULL;
    if (NULL != f_controller.m_pdexec)
    {
        m_pdexec = new CPDProcessExec(*(f_controller.m_pdexec));
    }
    m_comm = NULL;
    if (NULL != f_controller.m_comm)
    {
        m_comm = CDPSComManager::GetInstance();
    }
    m_vfc_mgr = NULL;
    if (NULL != f_controller.m_vfc_mgr)
    {
        m_vfc_mgr = new CvfcDBMgr(*(f_controller.m_vfc_mgr));

    }
    m_ini_reader = NULL;
    if (NULL != f_controller.m_ini_reader)
    {
        m_ini_reader = new CIniFileReader(*(f_controller.m_ini_reader));
    }
    m_app_mode = f_controller.m_app_mode;
    m_nScannedProtIdx = f_controller.m_nScannedProtIdx;
    m_csstudy_file_name = f_controller.m_csstudy_file_name;
    SaveSlicePlaneData(m_csstudy_file_name);
}
//+Patni-VP/2012Aug06/Review/code common to copy constructor and assignment operator can go into a common function
CPDController& CPDController::operator = (CPDController& f_controller)
{
    PD_TRACE(CPDController::operator=);
    if (this == &f_controller)
        return *this;
    m_pdexec = NULL;
    if (NULL != f_controller.m_pdexec){
        m_pdexec = new CPDProcessExec(*(f_controller.m_pdexec));
    }
    m_comm = NULL;
    if (NULL != f_controller.m_comm){
        m_comm = CDPSComManager::GetInstance();
    }
    m_vfc_mgr = NULL;
    if (NULL != f_controller.m_vfc_mgr){
        m_vfc_mgr = new CvfcDBMgr(*(f_controller.m_vfc_mgr));
        
    }
    m_ini_reader = NULL;
    if (NULL != f_controller.m_ini_reader)
    {
        m_ini_reader = new CIniFileReader(*(f_controller.m_ini_reader));
    }
    m_app_mode = f_controller.m_app_mode;
    m_nScannedProtIdx = f_controller.m_nScannedProtIdx;
    return *this;
}

//****************************Method Header*************************************

//Method Name   :SendMessagePQM

//Author        :PATNI

//Purpose       :Sending Message to PQM using DPS
//******************************************************************************
int CPDController::SendMessagePQM(CString& prot_index_string) 
{
    PD_TRACE(CPDController::SendMessagePQM);
    CString l_target = CFW_BC::TARGET_CHANNEL;
    int l_status = E_NO_ERROR;
    m_comm = CDPSComManager::GetInstance();
    
    if (NULL != m_comm)
    {
        l_status = m_comm->Connect(l_target);
        if (E_NO_ERROR != l_status) {
            CString l_csErrMsg(_T("SendMessagePQM(): Not connected successfully."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
            return(E_ERROR);
        }
                
        IMplusMessage* pMsg = NEW_MSG();
        
        if(NULL == pMsg) {
            l_status=m_comm->Disconnect();

            if(E_NO_ERROR != l_status ){
                CString l_csErrMsg(_T("SendMessagePQM(): Not disconnected successfully."));
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
                return(E_ERROR);
           }
        }
        
        pMsg->SetSender(_bstr_t(CFW_BC::CHANNEL));
        pMsg->SetCommand(_bstr_t(CFW_BC::CMD));
        pMsg->SetParameter(_bstr_t(prot_index_string));   
        l_status = m_comm->SendMsg(pMsg);
        if(E_NO_ERROR == l_status) {
            l_status = E_NO_ERROR;
        }else {
            CString l_csErrMsg(_T("SendMsg: Message not sent successfully."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        }
        
        DEL_MSG(pMsg);
        l_status=m_comm->Disconnect();

        if(E_NO_ERROR != l_status ){
            CString l_csErrMsg(_T("Disconnect: Not disconnected successfully."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        }
    }
    else
    {   CString l_cserrmsg = _T("CPDController::SendMessagePQM() DPSCommManager is NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }

    return(l_status);
}
//****************************Method Header************************************

//Method Name   :Initialize

//Author        :PATNI

//Purpose       : Reads the config file.
//*****************************************************************************

int CPDController::Initialize(void)
{
    PD_TRACE(CPDController::Initialize);
    int l_nstatus = E_ERROR ;

    if (NULL == m_ini_reader){
        m_ini_reader = new CIniFileReader();
    }
    if (NULL == m_ini_reader)
    {
        CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_nstatus = m_ini_reader->ReadIniParams();
    
    if(E_NO_ERROR == l_nstatus){
        return(E_NO_ERROR);
    }else{
        CString l_csErrMsg(_T("Failed to Read Values from Configuration file."));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
}
//****************************Method Header************************************

//Method Name   :SaveSlicePlaneData

//Author        :PATNI

//Purpose       : Read data from specified VarfieldDataFile and save in data class
//*****************************************************************************

int CPDController::SaveSlicePlaneData(const CString & f_sFilename)
{
    PD_TRACE(CPDController::SaveSlicePlaneData);        //Mpower 040/2012-08-09/AS/Trace and error log working
    char *l_cpFilename = NULL;
    int l_len =0;
    l_len = f_sFilename.GetLength();
    l_cpFilename = new char[l_len];                  //Mpower 040/2012-08-09/AS/Buffer size modified
    
    if(NULL == l_cpFilename)
    {
        CString l_cserrmsg (_T("SaveSlicePlaneData Memory allocation error"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    int l_nerror_code = 0;
    wcstombs(l_cpFilename,(LPCTSTR) f_sFilename, BUFFER_SIZE_128); 
    CPDDataFileReader l_objFileReader;
    DATAVECTOR l_vantagevector;
    DATAVECTOR l_dicomvector;
    int l_nstatus = l_objFileReader.ReadVarfieldDataFile(l_cpFilename,l_vantagevector,m_app_mode);

    if (E_NO_ERROR == l_nstatus){
        m_objPDData.SetVantageDataVector(l_vantagevector);
        m_objPDData.SetDicomDataVector(l_dicomvector); 
    }else{
        l_nstatus = l_objFileReader.getErrorCode();     
    }

    if ( NULL != l_cpFilename ){
        delete [] l_cpFilename; //Mpower 030/2012-06-26/AS/Modified/cpp change
        l_cpFilename = NULL;
    }

    return(l_nstatus);
}

//****************************Method Header************************************

//Method Name   :GetProtocolsList

//Author        :PATNI

//Purpose       : Read data from specified VarfieldDataFile and save in data class
//*****************************************************************************
int CPDController::GetProtocolsList(const CString & f_sfileName,p_data_t ** f_pprotocols,int *f_ntotal_prot ) 
{
    PD_TRACE(CPDController::GetProtocolsList);
    int l_ni = 0;
    int l_nj = 0;
    int l_nk = 0;
    int l_ntotal_prot = -1;
    int* l_nprotArray = new int[BUFFER_SIZE_128];
    if (NULL == l_nprotArray)
    {
        CString l_cserrmsg  = ErrorList[PD_MEMORY_ALLOCATION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        if ( NULL != l_nprotArray ){
            delete []l_nprotArray;
            l_nprotArray = NULL;
         }
        return(E_ERROR);
    }
    int status = E_NO_ERROR;
    CString l_csErrMsg_T("") ;
    if (NULL == f_pprotocols)
    {
        CString l_cserrmsg (_T("GetProtocolsList Parameter NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        if ( NULL != l_nprotArray ){
            delete []l_nprotArray;
            l_nprotArray = NULL;
         }
        return(E_ERROR);
    }
    try
    {
        if (NULL == m_vfc_mgr)
        {
            m_vfc_mgr = new CvfcDBMgr();
        }
        
        if (NULL == m_vfc_mgr)
        {
            CString l_csErrMsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
            if ( NULL != l_nprotArray ){
                delete []l_nprotArray;
                l_nprotArray = NULL;
            }
            return(E_ERROR);
        }
        
        status = m_vfc_mgr->OpenDBFile(f_sfileName);
        
        if (E_NO_ERROR != status) 
        {
            CString l_csErrMsg = ErrorList[PD_STUDY_OPEN];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERROR_LOGONLY); //AN
            if ( NULL != l_nprotArray ){
                delete []l_nprotArray;
                l_nprotArray = NULL;
             }
            return(status);
        }
        
        l_ntotal_prot = m_vfc_mgr->getProtAcqOrder(l_nprotArray);
        
        if (0 >= l_ntotal_prot)
        {
            //CString l_csErrMsg (_T("No protocols available in study file")); 
            CString l_csErrMsg = ErrorList[PD_NO_UNSCANNED_PROTOCOL];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
            if ( NULL != l_nprotArray ){
                delete []l_nprotArray;
                l_nprotArray = NULL;
            }
            return(E_ERROR);
        }
        
        *f_pprotocols = new p_data_t[l_ntotal_prot];
        if (NULL == f_pprotocols)
        {
            CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            if ( NULL != l_nprotArray ){
                delete []l_nprotArray;
                l_nprotArray = NULL;
            }
            return(E_ERROR);
        }
        p_data_t * tmp=  NULL;
        int l_nprot_type = 0 ;
        for(l_ni = 0;l_ni < l_ntotal_prot;l_ni++) {
            tmp = *f_pprotocols;
            tmp[l_nj].protocolNum = l_nprotArray[l_ni];
            status = m_vfc_mgr->CheckIsProtocolScanned(l_nprotArray[l_ni],l_nprot_type);
            
            if (E_NO_ERROR != status) {
                CString l_csErrMsg =_T("vfGetFields error");
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERROR_LOGONLY);
                if ( NULL != l_nprotArray ){
                    delete []l_nprotArray;
                    l_nprotArray = NULL;
                }
                return(E_ERROR);
            }
            
            if (VFC_ACQ_STATUS_WAIT == l_nprot_type) {
                tmp[l_nj].isScanned   = UNSCANNED;
            }   else if (VFC_ACQ_STATUS_SUCCESS == l_nprot_type) {
                tmp[l_nj].isScanned   = SCANNED;
            } else {
                tmp[l_nj].isScanned = SCAN_FAIL;
            }
            
            l_nj++;
        }
        m_vfc_mgr->CloseDBFile();
        *f_ntotal_prot= l_ntotal_prot;
        return(E_NO_ERROR);
    }
    catch (CMemoryException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = _T("Memory Exception occured");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
        
    }
    catch (CFileException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = _T("File exception occured");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
    catch (CException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
        
    }
    catch (...)
    {
        //UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
        
    }
    return(E_ERROR);        
}
//****************************Method Header************************************

//Method Name   :Updatefields

//Author        :PATNI

//Purpose       : Read data from specified VarfieldDataFile and save in data class
//*****************************************************************************
int CPDController::Updatefields(const int f_ncurrent_row,
                                const funcParams_t  &fcParams,
                                const int f_scout_selection)
{
    PD_TRACE(CPDController::Updatefields);
    CPDCoordinates * l_pd_coordinate = NULL;
    l_pd_coordinate = m_objPDData.GetVantageData(f_ncurrent_row);
    if (NULL == l_pd_coordinate)
    {
        CString l_csErrMsg = _T("PD data is NULL");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
#if 0 
    char l_csseq_gen_name [BUFFER_SIZE_128]= {"\0"};
    VfPathElem_t l_pathVG[SERIES_PATH_LEVEL];
    
    l_pathVG[0].subtree_name = SVN_ST_PROTOCOL;
    l_pathVG[0].index = 0;
    
    l_pathVG[1].subtree_name = SVN_ST_VISUALGROUP;
    l_pathVG[1].index = 0;
    
    
    SPHND l_sp_handle = CreateScrPadInstance();
    //1.create ScratchPad 
    if (NULL == l_sp_handle)
    {
        CPDErrorMsgDlg l_errmsg_dlg;
        l_errmsg_dlg.SetErrorMessage("ScrPad Handle Null");
        l_errmsg_dlg.DoModal();
        return E_ERROR;
    }
    char *ptr = new char [BUFFER_SIZE_128] ;
    if(NULL ! = ptr){             //Mpower 040/2012-08-09/AS/Added
    strcpy(ptr,"/usr/tmp/PDTOOL.tmp_file");
    }else{
        return (E_ERROR);
    }
    int status = l_sp_handle->CreateScrPad(fcParams.studyfilename,fcParams.sel_protocol_index_value[0],ptr);
    if (E_NO_ERROR != status)   //Mpower 040/2012-08-09/AS/Added
    {
        l_sp_handle->ReleaseHandle();
        l_sp_handle = NULL;
        if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
            delete[] ptr;
            ptr = NULL;
        }
        return(E_ERROR);
    }

    //2.get Sequence-name
    if_error(l_sp_handle->GetSeqName(l_csseq_gen_name))
    {
        l_sp_handle->ReleaseHandle();
        l_sp_handle = NULL;
        if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
            delete[] ptr;
            ptr = NULL;
        }
        return(E_ERROR);
    }

//---------------------------------------------------

    RELIMHANDLE l_relim_handle= CreateRelimInstance();
    //3.StartRelim
    
    if_error(l_relim_handle->StartRelim(ptr,l_csseq_gen_name,fcParams.studyfilename ))
    {
        l_relim_handle->ReleaseHandle();
        l_relim_handle = NULL;
        if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
            delete[] ptr;
            ptr = NULL;
        }
        return(E_ERROR);
    }

    PLUPDATEHANDLE l_pdupdate = NULL;
    if (SMART_MPR == m_app_mode){
        if(NULL == l_pdupdate){				//Mpower 040/2012-08-09/AS/Added
        l_pdupdate = new CPlanUpdateMPR ();
        }
    }else{		
        if(NULL == l_pdupdate){				//Mpower 040/2012-08-09/AS/Added
        l_pdupdate = new CPlanUpdateVOI ();
        }
    }
    int l_ntotal_vg_count = m_vfc_mgr->TraverseProt(ptr);

    if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
        delete[] ptr;
        ptr = NULL;
    }
    int l_ncntVg = 0 ;
   
    if(NULL != l_pdupdate){     //Mpower 040/2012-08-09/AS/Added
     //  for(l_ncntVg = 0 ; l_ncntVg < 1;l_ncntVg++)    //Mpower 040/2012-08-09/AS/Commented
   //   {
        
            //Scan subtree to calculate VG nodes...function to be made
        
            //4.Update data
            //int l_nscanned_centerSlice = m_vfc_mgr->GetScannedCenterSliceNumber(0);//pass last scanned protocol.
         //        l_pathVG[1].index = l_ncntVg;
         //+Patni-VP/2012Aug06/Review/refactor code to move common code below all 'if' blocks

            if_error(l_pdupdate->UpdateLocator(m_vfc_mgr->GetScannedCenterSliceNumber(0)))
            {
                l_relim_handle->TerminateRelim();
                l_relim_handle->ReleaseHandle();
                l_relim_handle = NULL;           
                l_sp_handle->ReleaseHandle();
                l_sp_handle = NULL;
                if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
                    delete[] ptr;
                    ptr = NULL;
                }
                if (NULL != l_pdupdate)
                {
                    delete l_pdupdate;
                    l_pdupdate = NULL;
                }
                return(E_ERROR);
            }

            if_error (l_pdupdate->UpdatePlanStatus())
            {
                l_relim_handle->TerminateRelim();
                l_relim_handle->ReleaseHandle();
                l_relim_handle = NULL;           
                l_sp_handle->ReleaseHandle();
                l_sp_handle = NULL;
                if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
                    delete[] ptr;
                    ptr = NULL;
                }
                if (NULL != l_pdupdate)
                {
                    delete l_pdupdate;
                    l_pdupdate = NULL;
                }
                return(E_ERROR);
            }
            if_error(l_pdupdate->UpdateVector(l_pd_coordinate, l_pathVG,fcParams.selectedParam/*m_nSlicePlaneSelection*/))
            {
                l_relim_handle->TerminateRelim();
                l_relim_handle->ReleaseHandle();
                l_relim_handle = NULL;           
                l_sp_handle->ReleaseHandle();
                l_sp_handle = NULL;
                if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
                    delete[] ptr;
                    ptr = NULL;
                }
                if (NULL != l_pdupdate)
                {
                    delete l_pdupdate;
                    l_pdupdate = NULL;
                }
                return(E_ERROR);
            }
            if_error(l_pdupdate->UpdateSliceAttributes(l_pd_coordinate,m_nAttributeSelection,fcParams.selectedParam/*m_nSlicePlaneSelection*/))
            {
                l_relim_handle->TerminateRelim();
                l_relim_handle->ReleaseHandle();
                l_relim_handle = NULL;           
                l_sp_handle->ReleaseHandle();
                l_sp_handle = NULL;
                if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
                    delete[] ptr;
                    ptr = NULL;
                }
                if (NULL != l_pdupdate)
                {
                    delete l_pdupdate;
                    l_pdupdate = NULL;
                }
                return(E_ERROR);
            }
    
    //   }
    }else{              //Mpower 040/2012-08-09/AS/Added
        CString l_cserr (_T("CPDController::l_pdupdate Memory allocation error"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        if(NULL != ptr){					//Mpower 040/2012-08-09/AS/Added
            delete[] ptr;
            ptr = NULL;
        }
        return(E_ERROR);
    }
   if (NULL != l_pdupdate)
   {
       delete l_pdupdate;
   }
  
   //5.Execute Re limit
    l_relim_handle->ExecRelim();
    //6.Save changes onto the scratch pad
    //7.Terminate Re limit
    l_relim_handle->TerminateRelim();
    //Release memory after terminate
    //8.Release handle Re limit
    l_relim_handle->ReleaseHandle();
    l_relim_handle = NULL;

    //9.Copy Scratch pad to Study file
    l_sp_handle->DeleteScrPad();

    //10.Release handle of scratch pad
    l_sp_handle->ReleaseHandle();
    l_sp_handle = NULL;

#endif
    if (TRUE == l_pd_coordinate->getPDPlan().IsEmpty()){
        CString l_cserr (_T("PD SlicePlane NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }
   
    
    try
    {   
        int l_nSliceNum = 0;        //Mpower 040/2012-08-09/AS/Modified/variables taken inside try block
        float l_fSliceGap = 0;
        float l_fscliceThck = 0;
        f32vec2_t l_fFov = {0};
     
//     l_pd_coordinate->GetSliceAttributes(SLICE_NUM,(char *)&l_nSliceNum);
//     l_pd_coordinate->GetSliceAttributes(SLICE_GAP,(char *)&l_fSliceGap);
//     l_pd_coordinate->GetSliceAttributes(SLICE_THICKNESS,(char *)&l_fscliceThck);
//     l_pd_coordinate->GetSliceAttributes(FOV,(char *)&l_fFov);
    relimitUpdateParams_t l_relimitUpdateParams;
    if (SMART_MPR == m_app_mode)
    {
        l_relimitUpdateParams.v_offset = l_pd_coordinate->GetSliceVectorinfo(OFFSET_VECTOR);
        
        l_relimitUpdateParams.v_slice = l_pd_coordinate->GetSliceVectorinfo(SLICE_ORI_VECTOR);
        
        l_relimitUpdateParams.v_phase = l_pd_coordinate->GetSliceVectorinfo(PHASE_ORI_VECTOR);
    }else if (SMART_VOI == m_app_mode ){
        l_relimitUpdateParams.m_vec1 = l_pd_coordinate->GetSliceVectorinfo(COORD1);
        
        l_relimitUpdateParams.m_vec2 = l_pd_coordinate->GetSliceVectorinfo(COORD2);

        float l_fcouch_offset = 0;
        l_pd_coordinate->GetSliceAttributes(COUCH_OFFSET,(char *)&l_fcouch_offset);
      
        l_relimitUpdateParams.m_fcouchOffset = l_fcouch_offset;
    } else {
        CString l_cserr (_T("CPDController::Updatefields() Invalid option for MPR / VOI"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }     
     //Mpower 040/2012-08-09/AS/Modified/variables initialization done just before use
    l_pd_coordinate->GetSliceAttributes(SLICE_NUM,(char *)&l_nSliceNum);
    l_pd_coordinate->GetSliceAttributes(SLICE_GAP,(char *)&l_fSliceGap);
    l_pd_coordinate->GetSliceAttributes(SLICE_THICKNESS,(char *)&l_fscliceThck);
    l_pd_coordinate->GetSliceAttributes(FOV,(char *)&l_fFov);

    l_relimitUpdateParams.m_nSliceNum = l_nSliceNum;
    l_relimitUpdateParams.m_fSliceGap = l_fSliceGap;
    l_relimitUpdateParams.m_fscliceThck = l_fscliceThck;
    l_relimitUpdateParams.m_fov = l_fFov;
    l_relimitUpdateParams.m_nScannedProtIdx = m_nScannedProtIdx;
    l_relimitUpdateParams.m_nMpr_Voi_flag = m_app_mode;
    l_relimitUpdateParams.m_nflag_scout_selection = f_scout_selection;
    
    int l_nCenterSliceNum = m_vfc_mgr->GetScannedCenterSliceNumber(m_nScannedProtIdx,m_nProcIdx);
    l_relimitUpdateParams.m_nCenterSliceNum = l_nCenterSliceNum;
    l_relimitUpdateParams.m_nProcIdx = m_nProcIdx;
    //Mpower 040/2012-08-09/AS/Added
    int l_nstatus = Relimit(fcParams.studyfilename,fcParams.sel_protocol_index_value[0],&l_relimitUpdateParams);
    if (E_NO_ERROR != l_nstatus)        
    {
        CString l_cserrmsg (_T("UpdateFields:: Relimit error"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    }
    catch (CMemoryException& e)
    {
        UNREFERENCED_PARAMETER(e);
        CString l_cserr = _T("Memory related exception occured in UpdateFields()");
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }
    catch (...)
    {
        CString l_cserr = _T("exception occured in UpdateFields()");
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   :InvokeSmartMPRVoi

//Author        :PATNI

//Purpose       : Invokes the SmartVOI.exe or SmartMPR.exe depending on mode 
//                selected
//*****************************************************************************

int CPDController::InvokeSmartMPRVoi(void) {

    PD_TRACE(CPDController::InvokeSmartMPRVoi);

    if (NULL == m_pdexec) {    
        m_pdexec = new CPDProcessExec();
    }
    if (NULL == m_pdexec)
    {
        CString l_cserrmsg (_T("InvokeSmartMPRVoi Memory allocation error"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    m_pdexec->ExitProc();
   
    const char * l_csExe_name = NULL;

    if (SMART_MPR == m_app_mode ) {
        l_csExe_name = SMART_MPR_EXE;
    }   else {
        l_csExe_name = SMART_VOI_EXE;
    }
    //Mpower 040/2012-08-09/AS/Variables made more local
    bool l_flag = false;
    int l_nstatus = 0;

    l_nstatus = m_pdexec->GetProcessList(l_csExe_name,l_flag );

    if ( E_NO_ERROR != l_nstatus ){ 
        CString l_csErrMsg(_T("InvokeSmartMPR(): Error in getting process list ."));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);    //Mpower 040/2012-08-09/AS/Added
    }   

    if (true == l_flag){
        CString l_csErrMsg = ErrorList[PD_PROC_ERROR];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY); //AN
    }else{  
        CString l_csProcessNm(_T(""));  //Mpower 040/2012-08-09/AS/Modified/declared nearest to usage
        if (SMART_MPR == m_app_mode) { //Mpower 040/2012-08-09/AS/Modified
            l_csProcessNm = GetIniFileReader()->GetMPRExePath();

            if (TRUE == l_csProcessNm.IsEmpty()){
                l_csProcessNm = SMARTMPR_EXE_PATH;
            }
        } else {
            l_csProcessNm = GetIniFileReader()->GetVOIExePath();

            if (TRUE == l_csProcessNm.IsEmpty()){
                l_csProcessNm = SMARTVOI_EXE_PATH;
            }
        }
        
        if_error(m_pdexec->CreateProc(l_csProcessNm))
        {
            CString l_cserrmsg = _T("Create Process failed for application :");
            l_cserrmsg += l_csProcessNm;
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
    }

    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   :Close

//Author        :PATNI

//Purpose       : Memory cleanup of members
//*****************************************************************************
int CPDController::Close(void) {
    PD_TRACE(CPDController::Close);
   
    if ( NULL != m_comm ) {
        delete m_comm;
        m_comm = NULL;
    }

    if ( NULL != m_pdexec ) {
        m_pdexec->ExitProc();
        delete m_pdexec;
        m_pdexec = NULL;
    }
    
    if ( NULL != m_ini_reader ) {
        delete m_ini_reader;
        m_ini_reader = NULL;
    }

    if ( NULL !=m_vfc_mgr ) {
        delete m_vfc_mgr; 
        m_vfc_mgr = NULL;
    }
    
    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   : ConvertDicom

//Author        : PATNI

//Purpose       : Calls DcmController function to convert and create DICOM files
//Parameters    : const int f_nprotidx is the protocol index
//                const CString& img_path is  the path for image's dicom files
//                const char * f_csStudyfilename is the study file name
//*****************************************************************************
	//Mpower 039//Jul-12/Modified/AN/
int CPDController::ConvertDicom(const int f_nprotidx,const CString& img_path, const char * f_csStudyfilename){
    PD_TRACE(CPDController::ConvertDicom);

    if (NULL == f_csStudyfilename)
    {
        CString l_csErrMsg (_T("Study file name is NULL"));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
    CDcmController l_dcmController;
    int l_nstatus = l_dcmController.startDICOMConversion(f_csStudyfilename,f_nprotidx,img_path);
    return(l_nstatus);
}

/*************************************************************
* Method:    	CalculateParams
* Description: 	This function is used to calculate slice parameters
* Parameter: 	int & f_nselected_row :
* Parameter: 	funcParams_t & f_fcparams : information about parameters being updated
* Returns:   	int
*************************************************************/
int CPDController::CalculateParams(const int f_nselected_row,const funcParams_t &f_fcparams)
{
    PD_TRACE(CPDController::CalculateParams);

    //DATAVECTOR l_vantagevector;
    //Reset previous calculated data

    CString l_csfileName (f_fcparams.studyfilename);
    if (NULL == m_vfc_mgr)
    {
        m_vfc_mgr = new CvfcDBMgr();
        if (NULL == m_vfc_mgr)
        {
            CString l_cserrmsg (_T("VFC mgr NULL in CalculateParams"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
    }
      
    int status = m_vfc_mgr->OpenDBFile(l_csfileName);

    if ( E_ERROR == status ) {
        CString l_csErrMsg(_T("Failed to open database file."));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }

    int l_nprot_idx = f_fcparams.sel_protocol_index_value[0];
    CPDCalculation l_pd_calc(m_vfc_mgr);
    
     CPDCoordinates * l_pd_coordinate = NULL;   //Mpower 040/2012-08-09/AS/Modified
    
    l_pd_coordinate = m_objPDData.GetVantageData(f_nselected_row);
    if (NULL == l_pd_coordinate)
    {
        CString l_cserr (_T("CalculateParams : PD Data is NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }

    int l_nstatus = E_ERROR;            //Mpower 040/2012-08-09/AS/Added
    if (f_fcparams.isScoutSelected) {
        l_nstatus = l_pd_calc.CalculateCouchOffset(l_nprot_idx,l_pd_coordinate);

        if (l_nstatus != E_NO_ERROR) {
            CString l_csErrMsg(_T("Error in couch offset Calculation."));
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
            m_vfc_mgr->CloseDBFile();//vilas..query
            return(E_ERROR);
        }

        //calculate couch offset
        m_vfc_mgr->CloseDBFile();
        return(E_NO_ERROR);
    }
    //Mpower 040/2012-08-09/AS/Modified
    CString l_csErrMsg(_T(""));      
    switch (f_fcparams.selectedParam) {
    case SEL_SLICE_THICKNESS:
        l_nstatus = l_pd_calc.CalculateSliceThickness(f_fcparams.selectedAxis,l_nprot_idx,l_pd_coordinate);

        if (l_nstatus != E_NO_ERROR) {
            l_csErrMsg = (_T("Error in Slice Thickness Calculation."));
            //CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        }

        break;
    case SEL_SLICE_GAP:
        l_nstatus = l_pd_calc.CalculateSliceGap(f_fcparams.selectedAxis,l_nprot_idx,l_pd_coordinate);

        if (l_nstatus != E_NO_ERROR) {
            l_csErrMsg = (_T("Error in Slice Gap Calculation."));
            //CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        }

        break;
    case SEL_SLICE_NUM:
        l_nstatus = l_pd_calc.CalculateSliceNum(f_fcparams.selectedAxis,l_nprot_idx,l_pd_coordinate);

        if (l_nstatus != E_NO_ERROR) {
            l_csErrMsg = (_T("Error in Slice Number Calculation."));
            //CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        }

        break;
        
    default:
            l_csErrMsg = (_T("Unknown field Error"));
            //CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
      //      return(E_ERROR);
        break;
    }

    if (l_nstatus != E_NO_ERROR) {
        //CString l_csErrMsg(_T("Error in slice Calculation."));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
        m_vfc_mgr->CloseDBFile();
        return(E_ERROR);
    }
//Mpower 040/2012-08-09/AS/ends
    m_vfc_mgr->CloseDBFile();
    return(E_NO_ERROR);
}
//****************************Method Header************************************

//Method Name   :SetAppMode

//Author        :PATNI

//Purpose       : Sets the mode of PlanDuplicator to either SmartVoi or SmartMpr
//				  depending upon the radio button selection   //Mpower 040/2012-08-09/AS/Added
//*****************************************************************************

void CPDController::SetAppMode(PDAppMode_t f_app_mode)
{
    PD_TRACE(CPDController::SetAppMode);

    m_app_mode = f_app_mode;
}
//****************************Method Header************************************

//Method Name   : RemoveDirectoryRecursive

//Author        :PATNI

//Purpose       : Remove the directory at specified path
//*****************************************************************************

void CPDController::RemoveDirectoryRecursive(const CString& f_szPath)
{
    PD_TRACE(CPDController::RemoveDirectoryRecursive);

    // Removes the folder szPath after removing all its
    // contents including its sub-folders and their contents
    // The following block {..} limits the scope of ff
    CFileFind l_filefind;
    CString l_path = f_szPath;
    
    if(l_path.Right(1) != "\\"){
        l_path += "\\";
    }
    
    l_path += "*.*";
    BOOL res = l_filefind.FindFile(l_path);
  
    while(res){
        res = l_filefind.FindNextFile();

        if (l_filefind.IsDots()){
            continue;
        }
        
        if (l_filefind.IsDirectory()){
            l_path = l_filefind.GetFilePath();
            RemoveDirectoryRecursive(l_path);
            RemoveDirectory(l_path);
        }else{
            DeleteFile(l_filefind.GetFilePath());
        }
    }
   

		//Mpower 039//Jul-12/Added/AN/
    struct stat sts;
    struct stat sts2;
    char l_cspath[BUFFER_SIZE_128] = {0};
    int status = 0;
    
    CString l_csGP_Data_PD(_T(""));
    l_csGP_Data_PD = getenv(GP_DATA);
    if (l_csGP_Data_PD.IsEmpty())
    {
        l_csGP_Data_PD = "/gp/data";
    }
    l_csGP_Data_PD += PD_FOLDER;
    wcstombs(l_cspath,l_csGP_Data_PD,BUFFER_SIZE_128);
    status = stat(l_cspath , &sts);
    if (0 != status )//Mpower 040/2012-08-09/AS/Modified
    {
        CreateDirectory(l_csGP_Data_PD,NULL); 
    }
    memset(l_cspath,0,BUFFER_SIZE_128);
    wcstombs(l_cspath,f_szPath,BUFFER_SIZE_128);
    status = stat(l_cspath , &sts2);
    
    if (0 != status )//Mpower 040/2012-08-09/AS/Modified
    {
        CreateDirectory(f_szPath,NULL); 
    }
}
/*************************************************************
* Method:    	GetSlicePlaneSelection
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
// int CPDController::GetSlicePlaneSelection(void) const 
// { 
//     return m_nSlicePlaneSelection; 
// }
/*************************************************************
* Method:    	SetSlicePlaneSelection
* Description: 	This function is used 
* Parameter: 	int f_nSlicePlaneSelection :
* Returns:   	void
*************************************************************/
// void CPDController::SetSlicePlaneSelection(int f_nSlicePlaneSelection) 
// { 
//     m_nSlicePlaneSelection = f_nSlicePlaneSelection; 
// }
/*************************************************************
* Method:    	GetAttributeSelection
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
// int CPDController::GetAttributeSelection(void) const 
// { 
//     return m_nAttributeSelection; 
// }
/*************************************************************
* Method:    	SetAttributeSelection
* Description: 	This function is used 
* Parameter: 	int f_nAttributeSelection :
* Returns:   	void
*************************************************************/
// void CPDController::SetAttributeSelection(int f_nAttributeSelection) 
// { 
//     m_nAttributeSelection = f_nAttributeSelection; 
// }
/*************************************************************
* Method:    	GetMPRorVOIMode
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CPDController::GetMPRorVOIMode(void) const
{
    return m_app_mode; 
}
	//Mpower 039//Jul-12/Added/AN
/*************************************************************
* Method:    	ClearCalcData
* Description: 	This function is used for clearing contents of MPR /VOI calculated data
* Returns:   	int
*************************************************************/
int CPDController::ClearCalcData(void)
{
    CPDCoordinates * l_pd_data = NULL;
    int i = 0;
    for (i = 0; i < m_objPDData.GetDataSize(); i++){
        l_pd_data = m_objPDData.GetVantageData(i);
        if (NULL != l_pd_data)
        {
           l_pd_data->SetSliceAttributes(ALL,NULL);
        }
        else
        {
            CString l_cserrmsg (_T("ClearCalcData : PD Data NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        }
    }
    return(E_NO_ERROR);
    
}

/////////////////////////////////   End of File     /////////////////////////////