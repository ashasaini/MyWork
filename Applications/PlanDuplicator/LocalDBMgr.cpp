// LocalDBMgr.cpp: implementation of the CLocalDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalDBMgr.h"
#include <DBSALib/DBSALib.h>
#include <DBSALib/DBSAVR.h>
#include <DBSALib/DBSA.h>
#include "dataTypes.h"
#include "Common.h"
#include "vfcDBMgr.h"
#include "PDLogHelper.h"
#include "ImageInfo.h"
#include "PDTrace.h"

const int MR_DBS_TIMEOUT = 1000;
const int MAX_DB_OBJ_NUM = 4;
#define MR_DBS_LO                     _T( "LO" )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalDBMgr::CLocalDBMgr()
{
    PD_TRACE(CLocalDBMgr::CLocalDBMgr);
}

CLocalDBMgr::~CLocalDBMgr()
{
    PD_TRACE(CLocalDBMgr::~CLocalDBMgr);
}
//****************************Method Header************************************

//Method Name   : GetTagValue

//Author        :PATNI

//Purpose       : Get the tag value
//*****************************************************************************
int CLocalDBMgr::GetTagValue(dataElementT *f_grouptag,
                             CDBSAVR* f_leaf,
                             const int f_nsize)
{
    PD_TRACE(CLocalDBMgr::GetTagValue);
    if (NULL == f_leaf)
    {
        CString l_cserrmsg (_T("GetTagValue NULL parameter"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        m_error_code = PD_PARAMETER_INVALID;
        return(E_ERROR);
    }
    static int l_nimgno =0;
    unsigned int l_nVR_type = f_leaf->GetVRType();
    void *f_load_leaf_val = NULL;
    bool l_OB_flag = FALSE;
    
    try{
        char l_cptr[DICOM_VER_BUFF_SZ] = {0};       // new char[DICOM_VER_BUFF_SZ]; 
        LPTSTR* l_value = NULL;
        char *l_cpC = new char[BUFFER_SIZE_256];
        
        if ( NULL == l_cpC ){
            m_error_code = PD_MEMORY_ALLOCATION;
            CString l_cserrmsg (_T("Can not allocate memory to buffer."));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }else{
            //dummy
        }
        
        size_t l_val_len =0;
        
        switch (l_nVR_type)
        {
        case DBSA_DCM_VR_CS: //VR--> CS
            {               
                int l_nmax_vr_size = BUFFER_SIZE_16;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if ( NULL == f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                     }
                    return(E_ERROR);
                }
                
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                l_value = (LPTSTR*)f_load_leaf_val;
                
                if ( NULL == l_value ){
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                    return(E_ERROR);
                }
                
                f_leaf->GetValue(l_value,f_nsize);
                wcstombs(l_cpC,*l_value, BUFFER_SIZE_128);      //AS was 100
                //VR start
                sprintf(l_cptr,"%s","CS");
                //VR End
                
                l_val_len = strlen(l_cpC);
                break;
            }
        case DBSA_DCM_VR_UI://UI
            {
                int l_nmax_vr_size = BUFFER_SIZE_64;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if(NULL == f_load_leaf_val) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                     }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                
                CString l_row_no;
                
                l_value = (LPTSTR*)f_load_leaf_val;
                
                if (NULL == l_value){
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
        }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                f_leaf->GetValue(l_value,f_nsize);
                l_row_no.Format(_T("%s"), *l_value);            
                wcstombs(l_cpC,(LPCTSTR) l_row_no, BUFFER_SIZE_128);        //AS was 100
                //VR start
                sprintf(l_cptr,"%s","UI");
                //VR End
                l_val_len = strlen(l_cpC);
                break;
            }
        case DBSA_DCM_VR_DS: //VR--> DS
            {
                int l_nmax_vr_size = BUFFER_SIZE_16;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if ( NULL ==f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                       }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                CString l_row_no;   
                char *l_strapp = new char[BUFFER_SIZE_64];
                
                if ( NULL == l_strapp ){
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                   }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                l_value = (LPTSTR*)f_load_leaf_val;
                
                if ( NULL == l_value ){
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                     }
                    if ( NULL != l_strapp ){
                        delete []l_strapp;
                        l_strapp = NULL;
                    }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                f_leaf->GetValue(l_value,f_nsize);
                
                wcstombs(l_cpC,*l_value, BUFFER_SIZE_64);
                l_value++;
                int l_ni = 0;
                for(l_ni=1;l_ni<f_nsize ;l_ni++){   
                    wcstombs(l_strapp,*l_value, BUFFER_SIZE_64);
                    l_value++;
                    strcat(l_cpC,"\\");
                    strcat(l_cpC,l_strapp); 
                }
                
                if ( 1 < f_nsize ){
                    strcat(l_cpC," ");
                }else{
                    //dummy
                }
                
                //VR start
                sprintf(l_cptr,"%s","DS");
                l_val_len = strlen(l_cpC);
                
                if ( NULL != l_strapp ){
                    delete []l_strapp;
                    l_strapp = NULL;
                }else{
                    //dummy
                }   
                
                break;
            }
        case DBSA_DCM_VR_LO: //VR--> LO
            {
                int l_nmax_vr_size = BUFFER_SIZE_64;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if ( NULL ==f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                     }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                CString l_row_no;
                l_value = (LPTSTR*)f_load_leaf_val;
                
                if ( NULL == l_value ){
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                     }
                    return(E_ERROR);
                }else{
                    //dummy
                }
                
                f_leaf->GetValue(l_value,f_nsize);
                l_row_no.Format(_T("%s"), *l_value);
                wcstombs(l_cpC,(LPCTSTR) l_row_no, BUFFER_SIZE_128);        //AS was 100
                //VR start
                sprintf(l_cptr,"%s","LO");
                //VR End
                l_val_len = strlen(l_cpC);
                break;
            }
        case DBSA_DCM_VR_SH://SH
            {
                int l_nmax_vr_size = BUFFER_SIZE_16;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if ( NULL ==f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                      }
                    return(E_ERROR);
                } 
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                
                CString l_row_no;
                l_value = (LPTSTR*)f_load_leaf_val;
                
                if ( NULL == l_value ){
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                    return(E_ERROR);
                }
                
                f_leaf->GetValue(l_value,f_nsize);
                l_row_no.Format(_T("%s"), *l_value);            
                wcstombs(l_cpC,(LPCTSTR) l_row_no, BUFFER_SIZE_128);        //AS was 100
                //VR start
                sprintf(l_cptr,"%s","SH");
                //VR End
                l_val_len = strlen(l_cpC);
                break;
            }
        case DBSA_DCM_VR_OB: //VR--> OB
            {
                f_load_leaf_val = (void*) malloc (f_nsize);
                l_OB_flag = TRUE;
                
                if ( NULL == f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                    return(E_ERROR);
                }
                
                memset(f_load_leaf_val,NULL,f_nsize);
                double *l_dpvalue = NULL;
                char l_cImgFileName[BUFFER_SIZE_64]={0}; 
                f_leaf->GetValue(f_load_leaf_val,f_nsize);
                l_dpvalue = (double *)f_load_leaf_val;
                
                if ( NULL == l_dpvalue ) {
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                    return(E_ERROR);
                }
                sprintf(l_cImgFileName,"%s.%d","ImgData",l_nimgno);
                l_nimgno++;
#ifdef _DEBUG
                FILE *fp = NULL;
                fp = fopen(l_cImgFileName,"wb+");
                
                if(NULL != fp){
                    fwrite(f_load_leaf_val,1,f_nsize,fp);
                    fclose(fp);
                    fp = NULL;
                }
#endif
                //VR start
                sprintf(l_cptr,"%s","OB");
                //Mpower 007/AN/Added
                f_grouptag->value = new char[f_nsize];
                f_grouptag->length = f_nsize;
                memcpy(f_grouptag->value,f_load_leaf_val,f_nsize);
                //VR End    
                l_val_len = f_nsize ;//+ 2;
                l_val_len += DICOM_VER_BUFF_SZ;
                break;
            }
        case DBSA_DCM_VR_PN: //VR--> PN
            {
                NameString_t l_nameStr;
                char *l_cpC_tmp = new char[BUFFER_SIZE_64];
                
                if (NULL == l_cpC_tmp){
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                  /*  if(NULL != f_grouptag->value){
                        delete []f_grouptag->value;
                        f_grouptag->value = NULL;
                    }*/
                    return(E_ERROR);
                }
                
                f_leaf->GetValue(l_nameStr);
                
                if(NULL != l_nameStr.familyname){
                    wcstombs(l_cpC,l_nameStr.familyname, BUFFER_SIZE_128);      //AS was 100
                }
                
                if(NULL != l_nameStr.givenname){
                    wcstombs(l_cpC_tmp,l_nameStr.givenname, BUFFER_SIZE_128);   //AS was 100
                    strcat(l_cpC,"^");
                    strcat(l_cpC,l_cpC_tmp);
                }
                
                if (NULL != l_nameStr.middlename){
                    wcstombs(l_cpC_tmp,l_nameStr.middlename, BUFFER_SIZE_128);  //AS was 100
                    strcat(l_cpC,"^");
                    strcat(l_cpC,l_cpC_tmp);
                }
                
                if (NULL != l_nameStr.nameprefix){
                    wcstombs(l_cpC_tmp,l_nameStr.nameprefix, BUFFER_SIZE_128);  //AS was 100
                    strcat(l_cpC,"^");
                    strcat(l_cpC,l_cpC_tmp);
                }
                
                if (NULL != l_nameStr.namesuffix){
                    wcstombs(l_cpC_tmp,l_nameStr.namesuffix, BUFFER_SIZE_128);     //AS was 100
                    strcat(l_cpC,"^");
                    strcat(l_cpC,l_cpC_tmp);
                }
                
                strcat(l_cpC," ");
                
                //VR start
                sprintf(l_cptr,"%s","PN");
                //VR End
                
                l_val_len = strlen(l_cpC);
                
                if (NULL != l_cpC_tmp){
                    delete []l_cpC_tmp;
                    l_cpC_tmp = NULL;
                }
                
                break;
            }
        case DBSA_DCM_VR_US: //VR --> US
            {
                int l_nmax_vr_size = DICOM_VER_BUFF_SZ;
                f_load_leaf_val = (void*) malloc (l_nmax_vr_size*f_nsize);
                
                if ( NULL ==f_load_leaf_val ) {
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                      }
                 /*   if(NULL != f_grouptag->value){
                        delete []f_grouptag->value;
                        f_grouptag->value = NULL;
                    }*/
                    return(E_ERROR);
                }
                
                memset(f_load_leaf_val,NULL,l_nmax_vr_size*f_nsize);
                unsigned short *l_value = NULL;
                l_value = (unsigned short *)f_load_leaf_val;
                
                if ( NULL == l_value ) {
                    m_error_code = PD_PARAMETER_INVALID;
                    CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                /*    if(NULL != f_grouptag->value){
                        delete []f_grouptag->value;
                        f_grouptag->value = NULL;
                    }*/
                    return(E_ERROR);
                }
                
                f_leaf->GetValue(*l_value);
                
                //VR start
                sprintf(l_cptr,"%s","US");
                //VR End    
                
                l_val_len = sizeof(l_val_len);
                memcpy(l_cpC,l_value,sizeof(unsigned short));
                break; 
            }
        case DBSA_DCM_VR_SS: //VR --> SS
            {
                short l_value = 0;
                f_leaf->GetValue(&l_value,f_nsize);
                
                //VR start
                sprintf(l_cptr,"%s","SS");
                //VR End
                
                l_val_len = sizeof(l_value);
                memcpy(l_cpC,&l_value,sizeof(l_value));
                break; 
            }
        case DBSA_DCM_VR_IS: //VR --> IS
            {
                long *l_IS_value = new long;
                
                if ( NULL == l_IS_value ){
                    m_error_code = PD_MEMORY_ALLOCATION;
                    CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    if ( NULL != l_cpC ){
                        delete []l_cpC;
                        l_cpC = NULL;
                    }
                  /*  if(NULL != f_grouptag->value){
                        delete []f_grouptag->value;
                        f_grouptag->value = NULL;
                    }*/
                    return(E_ERROR);
                }
                
                f_leaf->GetValue(l_IS_value,f_nsize);
                sprintf(l_cpC,"%ld",*l_IS_value);
                //VR start
                sprintf(l_cptr,"%s","IS");
                //VR End
                l_val_len = strlen(l_cpC);
                
                if ( NULL != l_IS_value ){
                    delete l_IS_value;
                    l_IS_value = NULL;
                }
                
                break;
            }
        default:
            {
                break;
            }
        }
        
        if ( FALSE == l_OB_flag) {
          
            if(NULL != f_grouptag->value){
                delete []f_grouptag->value;
                f_grouptag->value = NULL;
            }
          
            f_grouptag->value = new char[l_val_len];
            if (NULL == f_grouptag->value)
            {
                CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                if ( NULL != l_cpC ){
                    delete []l_cpC;
                    l_cpC = NULL;
                 }
               /* if(NULL != f_grouptag->value){
                    delete []f_grouptag->value;
                    f_grouptag->value = NULL;
                }*/
                return(E_ERROR);
            }
            f_grouptag->length = l_val_len;
            memcpy(f_grouptag->value,l_cpC,l_val_len);
        }
        f_grouptag->VR = NULL;
        f_grouptag->VR = new char[DICOM_VER_BUFF_SZ];
        strcpy(f_grouptag->VR,l_cptr);
        
        if ( NULL != l_cpC ){
            delete []l_cpC;
            l_cpC = NULL;
        }
       /* if(NULL != f_grouptag->value){
            delete []f_grouptag->value;
            f_grouptag->value = NULL;
        }*/
        
        //         if ( NULL != l_cptr ){
        //           delete []l_cptr;
        //             l_cptr = NULL;
        //         }
     
        
        if ( NULL != f_load_leaf_val ){
            free(f_load_leaf_val);//to be modified
            f_load_leaf_val = NULL;
        }   
    }
    catch(...){
        m_error_code = PD_EXCEPTION;
        return(E_ERROR);
    }
    
    return(E_NO_ERROR);
}
//****************************Method Header************************************
/**
* Set attribute tag and type for get
* @param     [in]eDBLevel: the level of DB object
* @param     [in]csLoid: the new created image loid
* @param     [in]stTagInfo: the tag info struct
* @return    S_OK if successful
* @return    others if failed
* @since     1.00
*/
//*****************************************************************************
HRESULT  CLocalDBMgr::SetAttrForGet(  MPlus::DBServer::OBJECT_LEVEL_e eDBLevel,
                                    const CString& csLoid,
                                    const std::vector<TAGINFO_t>& vTagInfo,
                                    MpDbsBaseObjectInfo* pDBObj)
{
    PD_TRACE(CLocalDBMgr::SetAttrForGet);
    
    if (TRUE == csLoid.IsEmpty())
    {
        CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    if (NULL == pDBObj)
    {
        CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    if ( ( TYPE_STUDY_LEVEL != eDBLevel) &&
        ( TYPE_SERIES_LEVEL != eDBLevel ) &&
        ( TYPE_IMAGE_LEVEL != eDBLevel ) &&
        ( TYPE_PATIENT_LEVEL != eDBLevel ))
    {
        CString l_cserrmsg = _T("SetAttrForGet() : Error");			//Mpower 040/2012-08-24/AS/Added
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(static_cast<HRESULT>( E_ERROR)); 
    }
    
    try
    {
        // according loid and attr to get different object info
        if ( NULL == pDBObj ){
            return(static_cast<HRESULT>( 1 ));
        }
        pDBObj->AddLoid( static_cast<LPCTSTR>( csLoid ));
        
        if (vTagInfo.size() <= 0){
            
            return(E_ERROR);
        }
        
        // set all the attrb
        bool l_bstatus = true;
        for ( unsigned int nIndex = 0u; nIndex < vTagInfo.size(); nIndex++ )
        {
            TAGINFO_t stTagInfo = vTagInfo[nIndex];
            l_bstatus = pDBObj->AddAttrTag( csLoid,
                                stTagInfo.m_szDICOMTag,
                                DBS_GET, MP_DBS_UNSET_FLAG);
            if(false == l_bstatus)
            {
                pDBObj->RemoveAllAttrTag( csLoid );
                return(static_cast<HRESULT>( E_ERROR ));
            }
            
        }
    }
    catch ( ... )
    {
        CString l_cserrmsg = _T("SetAttrForGet() : Error");			//Mpower 040/2012-08-24/AS/Added
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(static_cast<HRESULT>( E_ERROR )); 
    }
    
    return(S_OK);
}

//****************************Method Header************************************
/**
* Set attribute tag and type for get
* @param     [in]eDBLevel: the level of DB object
* @param     [in]csLoid: the new created image loid
* @param     [in]stTagInfo: the tag info struct
* @return    S_OK if successful
* @return    others if failed
* @since     1.00
*/
//*****************************************************************************
HRESULT  CLocalDBMgr::SetAttrForGet(  MPlus::DBServer::OBJECT_LEVEL_e eDBLevel,
                                    const CString& csLoid,
                                    const TAGINFO_t& tTagInfo,
                                    MpDbsBaseObjectInfo* pDBObj)
{
    PD_TRACE(CLocalDBMgr::SetAttrForGet);
    
    HRESULT hr = S_OK;
    
    try
    {
        std::vector<TAGINFO_t> vTagInfo;
        vTagInfo.push_back(tTagInfo);
        
        hr = SetAttrForGet(eDBLevel, csLoid, vTagInfo,pDBObj);
    }
    catch ( ... )
    {
        CString l_cserrmsg = _T("SetAttrForGet() : Error");			//Mpower 040/2012-08-24/AS/Added
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(static_cast<HRESULT>( E_ERROR )); 
    }
    
    return(hr);
}
//****************************Method Header************************************

//Method Name   : SetMpDbsCommit

//Author        :PATNI

//Purpose       : Get the patient LOID
//*****************************************************************************
int CLocalDBMgr::SetMpDbsCommit(const CString& f_loid_stdLevel, CString& f_loid_patLevel)
{
    PD_TRACE(CLocalDBMgr::SetMpDbsCommit);
    HRESULT l_hResult = E_FAIL; 
    MpDbsIFClassFactory *l_factory = MpDbsIFClassFactory::GetInstance();
    
    if( NULL != l_factory )
    {
        MpDbsBaseObjectInfo *l_pObj = l_factory->CreateInfoObject(TYPE_PATIENT_LEVEL);
        LPCTSTR l_loid_imgLevel = (LPCTSTR)f_loid_stdLevel;
        const TAGINFO_t l_stParentsID = { DCM_ST_PATIENT_ID, MR_DBS_LO };
        l_hResult = SetAttrForGet( TYPE_PATIENT_LEVEL, l_loid_imgLevel, l_stParentsID,l_pObj); //AN
        if (S_OK != l_hResult)
        {
            CString l_cserrmsg (_T("Function SetAttrForGet() returned an error"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
        const CString l_csLocalDB = _T("Local");
        bool l_bstatus = true;
        l_bstatus = l_factory->Initialise( l_csLocalDB );
        if(false == l_bstatus){
            // get from DB object info
            int l_nLockType = MP_DBS_READ;
            int l_nTimeOut = MR_DBS_TIMEOUT;
            int l_nCount = 0;
            for ( l_nCount = 0; l_nCount < MAX_DB_OBJ_NUM; l_nCount++ ){
                MpDbsBaseObjectInfo* l_pDBObj = l_pObj;
                
                if ( NULL == l_pDBObj ){
                    continue;
                }
                
                // Lock
                if ( MP_DBS_SUCCESS != l_pDBObj->Lock( 
                    l_nLockType, l_nTimeOut, l_csLocalDB)){
                    CString l_cserrmsg (_T("Failed to obtain lock on DB object"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return(E_ERROR);
                }
            }
        }
        // Get info
        int l_nCommitType = MP_DBS_COMMIT;
        
        try
        {   int l_nCount = 0;
            for ( l_nCount = 0; l_nCount < MAX_DB_OBJ_NUM; l_nCount++ )
            {
                MpDbsBaseObjectInfo* l_pDBObj = l_pObj;
                
                if ( NULL == l_pDBObj ){
                    continue;
                }                   
                // get
                if ( MP_DBS_SUCCESS != l_pDBObj->GetInfo( l_csLocalDB )){
                    //  l_hResult = static_cast<HRESULT>( 23 );
                    CString l_cserrmsg (_T("Failed to get DB info"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    l_nCommitType = MP_DBS_ROLLBACK;
                }
            }
        }
        catch ( ... )
        {
            //             l_hResult = static_cast<HRESULT>( 12 );
            CString l_cserrmsg (_T("Exception occured in SetMpDbsCommit"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            
            l_nCommitType = MP_DBS_ROLLBACK;
        }           
        // end transaction
        if ( MP_DBS_SUCCESS != MpDbsIFClassFactory::Destroy( l_nCommitType ))
        {
            int l_nCount = 0;
            for ( l_nCount = 0; l_nCount < MAX_DB_OBJ_NUM; l_nCount++ ){
                MpDbsBaseObjectInfo* l_pDBObj = l_pObj;
                
                if ( NULL == l_pDBObj ){
                    continue;
                }
                
                l_hResult = static_cast<HRESULT>(l_pDBObj->Unlock( 
                    l_nCommitType, l_csLocalDB));
            }
        }
        
        f_loid_patLevel = l_pObj->GetInterfaceLevelLOID(l_loid_imgLevel);
        return(E_NO_ERROR);
    }       
    return(E_ERROR);
}
//****************************Method Header********************************************

//Method Name   : buildGroup

//Author        :PATNI

//Purpose       : Build the group, get leafs and write group and elements in DICOm file 
//*************************************************************************************
int CLocalDBMgr::buildGroup(dataElementT *f_group,
                            const int f_numElementsInGroup,
                            CImageInfo * f_imginfo
                            ) {
    PD_TRACE(CLocalDBMgr::buildGroup);    
    if ((NULL == f_imginfo) || (NULL == f_group)) 
    {
        CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    try{
        CDBSAVR **l_leaves = NULL;
        int l_npatientLevelTag =0;
        int l_nstudyLevelTag =0;
        int l_nseriesLevelTag = 0;
        int l_nimageLevelTag = 0;
        int l_nsz = 0;
        int l_nstatus=E_NO_ERROR;
        
        CString l_loid_patLevel = EMPTY_STRING;
        CString l_loid_stdLevel = EMPTY_STRING;
        CString l_loid_serLevel = EMPTY_STRING;
        CString l_loid_imgLevel = EMPTY_STRING;
        
        //         CDBSAVRNode *l_node_patient =   new CDBSAVRNode();
        //         CDBSAVRNode *l_node_study =   new CDBSAVRNode();
        //         CDBSAVRNode *l_node_series =   new CDBSAVRNode();
        //         CDBSAVRNode *l_node_img =   new CDBSAVRNode();
        //         
        l_leaves = new CDBSAVR *[f_numElementsInGroup];
        
        if (NULL == l_leaves) {
            m_error_code = PD_MEMORY_ALLOCATION;
            CString l_cserrmsg = ErrorList[m_error_code];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(m_error_code);
        }
        // leaves is a double pointer so we need to assign 
        // like this ..in C for ex ... leaves = (CDBSAVR**)malloc(size);
        int l_nlfs = 0;
        for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++){
            l_leaves[l_nlfs] = new CDBSAVRIS(f_group[l_nlfs].nemacode);
            
            if (NULL == l_leaves[l_nlfs]){
                m_error_code = PD_MEMORY_ALLOCATION;
                CString l_cserrmsg = ErrorList[m_error_code];
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(m_error_code);
            }
            
            switch(f_group[l_nlfs].level) {
                
            case PATIENT_LEVEL:
                //                 if (NULL == l_node_patient){
                //                     m_error_code = PD_MEMORY_ALLOCATION;
                //                     CPDLogHelper::WriteToErrorLog(ErrorList[PD_MEMORY_ALLOCATION],__LINE__);
                //                     return(m_error_code);
                //                 }
                // 
                //                 l_node_patient->addLeaf(l_leaves[l_nlfs]);
                l_npatientLevelTag ++;
                break;
            case STUDY_LEVEL:
                //                 if (NULL == l_node_study){
                //                     m_error_code = PD_MEMORY_ALLOCATION;
                //                     CPDLogHelper::WriteToErrorLog(ErrorList[PD_MEMORY_ALLOCATION],__LINE__);
                //                     return(m_error_code);
                //                 }
                // 
                //                 l_node_study->addLeaf(l_leaves[l_nlfs]);
                l_nstudyLevelTag ++;
                break;
            case SERIES_LEVEL:
                //                 if (NULL == l_node_series){
                //                     m_error_code = PD_MEMORY_ALLOCATION;
                //                     CPDLogHelper::WriteToErrorLog(ErrorList[PD_MEMORY_ALLOCATION],__LINE__);
                //                     return m_error_code;
                //                 }
                // 
                //                 l_node_series->addLeaf(l_leaves[l_nlfs]);
                l_nseriesLevelTag++;
                break;
            case POSITION_LEVEL:
                //                 if (NULL == l_node_img){
                //                     m_error_code = PD_MEMORY_ALLOCATION;
                //                     CPDLogHelper::WriteToErrorLog(ErrorList[PD_MEMORY_ALLOCATION],__LINE__);
                //                     return(m_error_code);
                //                 }
                // 
                //                 l_node_img->addLeaf(l_leaves[l_nlfs]);
                l_nimageLevelTag++;
                break;
            default: 
                break;
            }
        }
        
        CDBSAPatient   *l_out_node_patLevel = NULL;
        CDBSAStudy     *l_out_node_stdLevel = NULL;
        CDBSASeries    *l_out_node_serLevel = NULL;
        CDBSAImage     *l_out_node_imgLevel = NULL;
        int l_nret = -1;
        DBSAFactory l_factory;
        CDBSAVRNode *l_node =   new CDBSAVRNode();
        
        if (NULL == l_node){
            m_error_code = PD_MEMORY_ALLOCATION;
            
            if (NULL != l_leaves){
                for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                {
                    delete l_leaves[l_nlfs];
                }
                delete l_leaves;
                l_leaves = NULL;
            }
            return(m_error_code);
        }
        
        CDBSATagErrorList *l_error_list_patLevel=NULL;
        CDBSATagErrorList *l_error_list_stdLevel=NULL;
        CDBSATagErrorList *l_error_list_serLevel=NULL;
        CDBSATagErrorList *l_error_list_imgLevel=NULL;
        HRESULT l_hresult = E_FAIL;
        
        if (0 != l_npatientLevelTag) {
            status_t l_status= E_NO_ERROR;
            l_loid_stdLevel = f_imginfo->getStudyLOID();// SVD_ST_STUDY,l_loid_stdLevel);
            
            if (TRUE == l_loid_stdLevel.IsEmpty()){
                CString l_csErrMsg(_T("GetLOID: Patient level LOid ."));
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
                m_error_code = PD_MPLUSDB;
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                  }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(m_error_code);
            }
            l_status = SetMpDbsCommit(l_loid_stdLevel, l_loid_patLevel);
            if_error(l_status)
            {
                CString l_cserrmsg (_T("Function SetMpDbsCommit() returned an error"));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(E_ERROR);
            }
            l_nret = -1;
            l_nret = l_factory.DBSA_LoadPatient(&l_out_node_patLevel, &l_error_list_patLevel, All, (LPCTSTR)l_loid_patLevel, l_node); 
            
            if ((MRC_DBSA_SUCCESS != l_nret) || (NULL == l_out_node_patLevel)){
                m_error_code = PD_MPLUSDB;
                CString l_cserrmsg (_T("DBSA_LoadPatient: Error."));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(m_error_code);
            }
        }
        
        if (0 != l_nstudyLevelTag) {
            l_loid_stdLevel = f_imginfo->getStudyLOID();
            if(TRUE == l_loid_stdLevel.IsEmpty()){            //Mpower 040/2012-08-24/AS/Added
                CString l_cserrmsg (_T("Function getStudyLOID() returned NULL"));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            l_nret = -1;
            l_nret = l_factory.DBSA_LoadStudy(&l_out_node_stdLevel, &l_error_list_stdLevel, All, (LPCTSTR)l_loid_stdLevel, l_node); 
            if ((MRC_DBSA_SUCCESS != l_nret) || (NULL == l_out_node_stdLevel)){
                m_error_code = PD_MPLUSDB;
                CString l_cserrmsg (_T("DBSA_LoadStudy: Error."));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(m_error_code);
            }       
        }
        
        if (0 != l_nseriesLevelTag) {
            l_loid_serLevel = f_imginfo->getSeriesLOID();
            if(TRUE == l_loid_serLevel.IsEmpty()){                //Mpower 040/2012-08-24/AS/Added
                CString l_cserrmsg (_T("Function getStudyLOID() returned NULL"));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            l_nret = -1;
            l_nret = l_factory.DBSA_LoadSeries(&l_out_node_serLevel, &l_error_list_serLevel, All, (LPCTSTR)l_loid_serLevel, l_node); 
            if ((MRC_DBSA_SUCCESS != l_nret) || (NULL == l_out_node_serLevel)){
                m_error_code = PD_MPLUSDB;
                CString l_cserrmsg (_T("DBSA_LoadSeries: Error."));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(m_error_code);
            }
        }
        
        if (0 != l_nimageLevelTag) {
            l_loid_imgLevel = f_imginfo->getImageLOID();
            if(TRUE == l_loid_imgLevel.IsEmpty()){                //Mpower 040/2012-08-24/AS/Added
                CString l_cserrmsg (_T("Function getStudyLOID() returned NULL"));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
            l_nret = -1;
            l_nret = l_factory.DBSA_LoadImage(&l_out_node_imgLevel, &l_error_list_imgLevel, All, (LPCTSTR)l_loid_imgLevel, l_node); 
            if ((MRC_DBSA_SUCCESS != l_nret) || (NULL == l_out_node_imgLevel)){
                m_error_code = PD_MPLUSDB;
                CString l_cserrmsg (_T("DBSA_LoadImage: Error."));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                if (NULL != l_leaves){
                    for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                    {
                        delete l_leaves[l_nlfs];
                    }
                    delete l_leaves;
                    l_leaves = NULL;
                }
                
                if (NULL != l_node){
                    delete l_node;
                    l_node = NULL;
                }
                return(m_error_code);
            }       
        }
        
        CDBSAVRLeaf* l_tag_leaf = NULL;
        
        for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++){
            switch(f_group[l_nlfs].level) {
            case PATIENT_LEVEL:
                l_tag_leaf = l_out_node_patLevel->FindLeaf(f_group[l_nlfs].nemacode); //Aakash
                break;
            case STUDY_LEVEL:
                l_tag_leaf = l_out_node_stdLevel->FindLeaf(f_group[l_nlfs].nemacode);
                break;
            case SERIES_LEVEL:
                if(NULL != l_out_node_serLevel){
                    l_tag_leaf = l_out_node_serLevel->FindLeaf(f_group[l_nlfs].nemacode);
                }else{
                    CString l_cserrmsg (_T("Error!! Series level leaf"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                }
                
                break;
            case POSITION_LEVEL:
                l_tag_leaf = l_out_node_imgLevel->FindLeaf(f_group[l_nlfs].nemacode);
                break;
            default: break;
            }
            
            if (NULL != l_tag_leaf){
                l_nsz = ((CDBSAVR*)l_tag_leaf)->GetSize();
                f_group[l_nlfs].length =  l_nsz;
                
                if(META_TAG != f_group[l_nlfs].nemacode){
                    l_nstatus = GetTagValue(&f_group[l_nlfs],(CDBSAVR*)l_tag_leaf,l_nsz);
                }
                
                if ( E_NO_ERROR != l_nstatus){
                    //log error
                    if (NULL != l_leaves){
                        for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
                        {
                            delete l_leaves[l_nlfs];
                        }
                        delete l_leaves;
                        l_leaves = NULL;
                    }
                    
                    if (NULL != l_node){
                        delete l_node;
                        l_node = NULL;
                }
                    CString l_cserrmsg (_T("l_node found NULL"));			//Mpower 040/2012-08-24/AS/Added
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return(E_ERROR);
                }
            }else{
                CString l_cserrmsg;
                l_cserrmsg.Format(_T("l_tag_leaf found NULL : (%x,%x)"),f_group[l_nlfs].group,f_group[l_nlfs].element);			//Mpower 040/2012-08-24/AS/Added
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(E_ERROR);
            }
        }   
        DBSA_FreePatient(l_out_node_patLevel);  
        DBSA_FreeStudy(l_out_node_stdLevel);
        DBSA_FreeSeries(l_out_node_serLevel);
        DBSA_FreeImage(l_out_node_imgLevel);    
        
        //         DBSA_FreeNode(l_node_patient); 
        //         DBSA_FreeNode(l_node_study);
        //         DBSA_FreeNode(l_node_series);
        //         DBSA_FreeNode(l_node_img);
        
        DBSA_FreeErrTagList(&l_error_list_patLevel);
        DBSA_FreeErrTagList(&l_error_list_stdLevel);
        DBSA_FreeErrTagList(&l_error_list_serLevel);
        DBSA_FreeErrTagList(&l_error_list_imgLevel);
        
            if (NULL != l_leaves){
            for (l_nlfs = 0;l_nlfs < f_numElementsInGroup;l_nlfs++)
            {
                delete l_leaves[l_nlfs];
            }
            delete l_leaves;
            l_leaves = NULL;
            }
        
        if (NULL != l_node){
            delete l_node;
            l_node = NULL;
        }
    }catch(...){
        m_error_code = PD_EXCEPTION;
        CString l_cserrmsg = ErrorList[PD_EXCEPTION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(m_error_code);
    }
    
    return(E_NO_ERROR);
}

////////////////////////    End Of File     /////////////////////////////////