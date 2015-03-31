// LocalDBMgr.cpp: CLocalDBMgr クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "LocalDBMgr.h"
#include "CommonDefinitions.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"//sb
#include "atlconv.h"
#include <vector>
#include <afxsock.h>


#include <CreateSeriesNumber\CreateSeriesNumber.h>
using namespace TMRData;

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CLocalDBMgr
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CLocalDBMgr::CLocalDBMgr()
{
    /*TIPREP_TRACE(CLocalDBMgr::CLocalDBMgr);*/
}
//****************************Method Header***************************
//Method Name   : CLocalDBMgr
//Purpose       : Destructor
//********************************************************************
CLocalDBMgr::~CLocalDBMgr()
{
    /*TIPREP_TRACE(CLocalDBMgr::~CLocalDBMgr);*/
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       getImgDataFromDB
* Description:  This function is used to get data from MPlus
*               Data base.
* Parameter:    CString f_ImageLoid
*               LOID for given image.
* Parameter:    void * * f_ImgData
*               Out parameter to get image data.
* Parameter:    int * size
*               Out parameter which holds size of a data.
* Parameter:    float * f_sScale
*               Out parameter which holds scale value for image.
* Parameter:    int * f_nTime_stamp
* Parameter:    long & f_nlData_Type
* Returns:      int
*************************************************************/
int CLocalDBMgr::getImgDataFromDB(CString &f_ImageLoid,
                                  void **f_ImgData,
                                  int *size,
                                  float *f_sScale,
                                  int *f_nTime_stamp,
                                  unsigned short *f_nlData_Type,int &f_nImgNo) //sb
{
    TIPREP_TRACE(CLocalDBMgr::getImgDataFromDB,MRTraceMarker9);
    
    status_t getstudy_status = E_NO_ERROR;
    
    //CDBSAVRNode *node = NULL;
    
    
    try {
	 	CDBSAVRNode *node = NULL;
        node =   new CDBSAVRNode();
        CDBSAVR *leaf = NULL;
        leaf = new CDBSAVRIS(TIPREP_IMG_TAG);
        CDBSAVR *leaf_scalefact = NULL;
        leaf_scalefact = new CDBSAVRIS(TIPREP_IMG_SCALE);
        CDBSAVR *leaf_inversion_time = NULL;
        leaf_inversion_time = new CDBSAVRIS(TIPREP_IMG_INVERSION_TIME);
        CDBSAVR *l_image_bit_leaf = NULL; 
        l_image_bit_leaf = new CDBSAVRUS(TIPREP_IMAGE_BITS);
        
        if(NULL == node || NULL == leaf || NULL == leaf_scalefact
            || NULL == leaf_inversion_time || NULL == l_image_bit_leaf){
            return E_SOCKET_ERR;
        } else{        
            node->addLeaf(leaf);
            node->addLeaf(leaf_scalefact);
            node->addLeaf(leaf_inversion_time);
            node->addLeaf(l_image_bit_leaf);
        }
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        CDBSATagErrorList *error_list = NULL;
        
        if(status != 0) {
            // WSACleanup();
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
        }
        
        int ret = -1;
        CDBSAImage *out_node = NULL;
        DBSAFactory factory;
        
        ret = factory.DBSA_LoadImage(&out_node, &error_list, All, (LPCTSTR)f_ImageLoid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            int sz = 0;
            CDBSAVRLeaf* series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_IMG_TAG);
            
            if(series_num_leaf) {//NULL check before using pointer
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                *size = sz;
                *f_ImgData = /*(void*) malloc(sz)*/operator new(sz);
                ((CDBSAVR*)series_num_leaf)->GetValue(*f_ImgData, sz);
            } else {
                CString l_csErrMsg(_T("0x7FE00010 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                getstudy_status = E_IMG_DATA_FAIL;
            }
            
            series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_IMG_SCALE);
            
            if(series_num_leaf) {
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                ((CDBSAVR*)series_num_leaf)->GetValue(f_sScale, sz);
            } else {
                CString l_csErrMsg(_T("0xE4011008 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                getstudy_status = E_IMG_SCALE_FAIL;
            }
            
            series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_IMG_INVERSION_TIME);
            
            if(series_num_leaf) {
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                void *load_leaf_val = NULL;
                load_leaf_val = (void*)/*malloc(sz)*/operator new(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                CString l_cTI = _T("");
                l_cTI.Format(_T("%s"), *l_value);
                char * l_cTI1 = new char[64];
                sprintf(l_cTI1, "%S", l_cTI);
                *f_nTime_stamp = atoi(l_cTI1);
                delete [] l_cTI1;  
                l_cTI1 = NULL;
                operator delete(load_leaf_val); 
                load_leaf_val = NULL; 
            } else {
                CString l_csErrMsg(_T("0x00180082 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
			
                int l_nStatus = (CTiPrepView::GetView())->getVfcTimeStamp(f_ImageLoid,f_nImgNo,f_nTime_stamp);
                CString l_csMsg(_T("0x00180082 node is not found VFC Reading for ti : %d."),*f_nTime_stamp);
                CTiPrepLogHelper::WriteToErrorLog(l_csMsg, __LINE__, __FILE__);
                if (E_NO_ERROR != l_nStatus)
                {
                    getstudy_status = E_IMG_TIME_STAMP_FAIL;
                }         
				 
            }
            
            series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_IMAGE_BITS);
            
            if(series_num_leaf) {
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                ((CDBSAVR*)series_num_leaf)->GetValue(f_nlData_Type, sz);
            } else {
                CString l_csErrMsg = (_T("0x00280100 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                getstudy_status = E_IMG_TAG_FAIL;
            }
            
        } else {
            CString l_csErrMsg;
            l_csErrMsg.Format(_T("Fail to load image.DBSA_LoadImage Error Code:%d"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            getstudy_status = E_ERR_IMG_LOAD_FAIL;
        }
        
        //Release Memory
        DBSA_FreeImage(out_node);
        
        //  DBSA_FreeNode(node);
        if(NULL != node) {
            delete node;
            node = NULL;
        }
        
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
        }
        
        WSACleanup();
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured in getImgDataFromDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        getstudy_status = E_ERR_EXCEPTION;
    }
    
    return getstudy_status;
}

/*************************************************************
* Method:       GetProtocolNameFromImageLevelDB
* Description:  This function is used
* Parameter:    CString f_ImageLoid
* Parameter:    CString & f_csProtocolName
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetProtocolNameFromImageLevelDB(CString &f_ImageLoid, 
                                                 CString &f_csProtocolName,
                                                 CString &f_csStudyName,CString &f_csCompletepath)//sb
{
    TIPREP_TRACE(CLocalDBMgr::GetProtocolNameFromImageLevelDB,MRTraceMarker3);
    
    status_t getstudy_status = E_NO_ERROR;
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
            // WSACleanup();
        }
        
        CDBSAVRNode *node =   NULL;
        CDBSAVR *leaf = NULL;
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_IMG_PATH);
        if(NULL == node || NULL == leaf){
            return E_MEM_ALLOC;
        }else{
            node->addLeaf(leaf);
        }
        DBSAFactory factory;
        int ret = -1;
        CDBSATagErrorList *error_list = NULL;
        CDBSAImage *out_node = NULL;  //Image level
        
        ret = factory.DBSA_LoadImage(&out_node, &error_list, All, (LPCTSTR)f_ImageLoid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            CDBSAVRLeaf* series_num_leaf = NULL;
            long tag = TIPREP_IMG_PATH;
            series_num_leaf = out_node->FindLeaf(tag);
            if(NULL == out_node) {
                CString l_csstatus ;
                l_csstatus.Format(_T("out_node NULL %d"), out_node);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                return E_SOCKET_ERR;
        }
            
            if(series_num_leaf) {
                int sz = 0;
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                void *load_leaf_val = NULL;
                load_leaf_val = (void*)/*malloc*/operator new(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                ///Run40042/Run40042.-1959:/1.3/7.1/8.0/9.0
                CString l_csTempStr = EMPTY_STRING;
                l_csTempStr.Format(_T("%s"), *l_value);
                f_csCompletepath.Format(_T("%s"), *l_value);//to get path till image level
                int cnt = l_csTempStr.ReverseFind(_T('/'));
                l_csTempStr = l_csTempStr.Left(cnt);
                cnt = l_csTempStr.ReverseFind(_T('/'));
                l_csTempStr = l_csTempStr.Left(cnt);
                f_csStudyName = l_csTempStr;
                cnt = l_csTempStr.ReverseFind(_T('/'));
                f_csProtocolName = l_csTempStr.Left(cnt);
                /*free*/operator delete(load_leaf_val); 
                load_leaf_val = NULL;
            } else {
                CString l_csErrMsg(_T("0x00291052 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                getstudy_status = E_IMG_TAG_PROT_FAIL;
            }
            
            if(out_node) {
                DBSA_FreeImage(out_node);
            }
            
            if(error_list) {
                DBSA_FreeErrTagList(&error_list);
            }
        }
        
        if(NULL != node) {
            delete node;
            node = NULL;
        }
        WSACleanup();
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        getstudy_status = E_ERR_EXCEPTION;
    }
    
    return getstudy_status;
}

/*************************************************************
* Method:       GetParentLOID
* Description:  This function is used to return Parent LOID
*               from child LOID
* Parameter:    LPCTSTR child_node_loid
* Parameter:    LOID_TYPE child_node_loid_type
* Returns:      LPCTSTR
*************************************************************/
int CLocalDBMgr::GetParentLOID(  
                               LPCTSTR child_node_loid,
                               LOID_TYPE child_node_loid_type,
                               CString &l_csParentLoid)
{
    TIPREP_TRACE(CLocalDBMgr::GetParentLOID,MRTraceMarker3);
    
    CString lcsloid(_T(""));
    lcsloid = child_node_loid;
    
    if(!child_node_loid || lcsloid.IsEmpty()) {
        CString l_csmsg(_T("Child LOID is empty"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERROR_NULL_FUNCPARAMS;
    }
    
    WSADATA wsaData;
    int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
    
    if(status != 0) {
        CString l_csstatus ;
        l_csstatus.Format(_T("WSAStartup status %d"), status);
        CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
        return E_SOCKET_ERR;
    }
    
    //LPCTSTR parent_loid = NULL;
    int ret_status = 0;
    
    try {
        int ret = 0;
        CDBSATagErrorList *error_list = NULL;
        switch(child_node_loid_type) {
        case LOID_TYPE_STUDY: {
            try {    
                DBSAFactory factory_Study;
                CDBSAPatient *patient_obj = NULL;            
                ret = factory_Study.DBSA_CreatePatient(&patient_obj, &error_list, 
                    All, NULL, child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && patient_obj) {
                    LPCTSTR temp_loid = NULL;
                    temp_loid = patient_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        //int len = _tcslen(temp_loid);
                        l_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreePatient(patient_obj);
                    patient_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
                CTiPrepLogHelper::WriteToErrorLog(message, __LINE__, __FILE__);
                ret_status = E_DB_ERROR;
            }
                              }
            break;
        case LOID_TYPE_SERIES: {
            try {
                CDBSAStudy *study_obj = NULL;
                DBSAFactory factory_Series;
                ret = factory_Series.DBSA_CreateStudy(&study_obj, &error_list, 
                    All, NULL, NULL, child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && study_obj) {
                    LPCTSTR temp_loid = NULL;
                    temp_loid = study_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        l_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreeStudy(study_obj);
                    study_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
                CTiPrepLogHelper::WriteToErrorLog(message, __LINE__, __FILE__);
                ret_status = E_DB_ERROR;
                //log error
            }
                               }
            break;
        case LOID_TYPE_IMAGE: {            
            try {
                CDBSASeries *series_obj = NULL;
                DBSAFactory factory_Image;
                ret = factory_Image.DBSA_CreateSeries(&series_obj, &error_list, 
                    All, NULL, NULL, child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && series_obj) {
                    LPCTSTR temp_loid = NULL;
                    temp_loid = series_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        l_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreeSeries(series_obj);
                    series_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
                CTiPrepLogHelper::WriteToErrorLog(message, __LINE__, __FILE__);
                ret_status = E_DB_ERROR;
            }
                              }
            break;
        default :
            break;
        }
        
        if(NULL != error_list) {
            DBSA_FreeErrTagList(&error_list);
            error_list = NULL;
        }
        
        if(ret != MRC_DBSA_SUCCESS) {
            CString message = _T("");
            message.Format(_T("Exception occured in GetParentLOID()"));
            CTiPrepLogHelper::WriteToErrorLog(message, __LINE__, __FILE__);
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("exception occured in GetParentLOID()"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        ret_status = E_ERR_EXCEPTION;
    }
    
    
    return ret_status;
}
/*************************************************************
* Method:       
* Description:  This function is used
* Parameter:    CString f_csStudyLoid
* Parameter:    int * f_nseries_count
* Parameter:    vector<CString> & f_csAcqVec
* Parameter:    LPCTSTR * * f_SeriesLOIDArray
* Returns:      int
*************************************************************/
int CLocalDBMgr::getStudyPathFromDB(CString &f_csStudyLoid,
                                    CString &f_StyudyPath)
{
    TIPREP_TRACE(CLocalDBMgr::getStudyPathFromDB,MRTraceMarker3);
    int l_nStatus = 0;
    CDBSAStudy *out_node = NULL;            //study level
    CDBSATagErrorList *error_list = NULL;
    DBSAFactory *factory = NULL;
    CDBSAVR *leaf = NULL;
    CDBSAVRNode *node =   NULL;
    
    try {
        int ret = -1;
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_STUDY_PATH);
        node->addLeaf(leaf);

        factory = new DBSAFactory();//Factory object can be local
        
        if(NULL == factory) {
            CString l_csstatus ;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_MEM_ALLOC;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadStudy(&out_node, &error_list, All, (LPCTSTR)f_csStudyLoid, node);
        }
        
        if(ret == MRC_DBSA_SUCCESS && NULL != out_node) {
            
            CDBSAVRLeaf* series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_STUDY_PATH);
            
            if(series_num_leaf) {
                int sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                void *load_leaf_val = NULL;
                load_leaf_val = (void*)/*malloc*/operator new(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                f_StyudyPath.Format(_T("%s"), *l_value);
                if(load_leaf_val){
                    operator delete(load_leaf_val);
                }
            }else{
                CString l_csstatus = EMPTY_STRING ;
                l_csstatus.Format(_T("FindLeaf failed %d"), ret);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
                l_nStatus = E_DB_ERROR;
                return(l_nStatus);
            }

        } else {
            CString l_csstatus = EMPTY_STRING ;
            l_csstatus.Format(_T("ret val from factory is %d LoadStudy failed"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_DB_ERROR;
            return(l_nStatus);
        }
        
        this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getStudyPathFromDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getStudyPathFromDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getStudyPathFromDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    }
    
    return(l_nStatus);
}

/*************************************************************
* Method:       getProtAcqOrderFromDB
* Description:  This function is used
* Parameter:    CString f_csStudyLoid
* Parameter:    int * f_nseries_count
* Parameter:    vector<CString> & f_csAcqVec
* Parameter:    LPCTSTR * * f_SeriesLOIDArray
* Returns:      int
*************************************************************/
int CLocalDBMgr::getProtAcqOrderFromDB(CString &f_csStudyLoid,
                                       int *f_nseries_count,
                                       vector<CString> &f_csAcqVec,
                                       LPCTSTR **f_SeriesLOIDArray)
{
    TIPREP_TRACE(CLocalDBMgr::getProtAcqOrderFromDB,MRTraceMarker3);
    int l_nStatus = 0;
    CDBSAStudy *out_node = NULL;            //study level
    CDBSATagErrorList *error_list = NULL;
    DBSAFactory *factory = NULL;
    
    try {
        int ret = -1;
        if(NULL == f_SeriesLOIDArray) {
            CString l_csstatus ;
            l_csstatus.Format(_T("f_SeriesLOIDArray  is %x"), f_SeriesLOIDArray);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        
        factory = new DBSAFactory();//Factory object can be local
        
        if(NULL == factory) {
            CString l_csstatus ;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_MEM_ALLOC;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadStudy(&out_node, &error_list, All, (LPCTSTR)f_csStudyLoid, NULL);
        }
        
        if(ret == MRC_DBSA_SUCCESS && NULL != out_node) {
            WORD* l_acqorder = NULL;
            int l_acqsize  = out_node->DBSA_GetAcqOrder(&l_acqorder, f_SeriesLOIDArray);
            (*f_nseries_count) = l_acqsize;
            l_nStatus = ConvertAcqOrderToString(l_acqorder, l_acqsize, f_csAcqVec);
            
            if(NULL != l_acqorder) {
                DBSA_FreeLocalAcqOrder(l_acqorder);
            }
        } else {
            CString l_csstatus = EMPTY_STRING ;
            l_csstatus.Format(_T("ret val from factory is %d LoadStudy failed"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_DB_ERROR;
            return(l_nStatus);
        }
        
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    }
    
    return(l_nStatus);
}

/*************************************************************
* Method:       CleanUpLocalDBMgr
* Description:  This function is used
* Parameter:    CDBSAVR * leaf
* Parameter:    CDBSAVRNode * node
* Parameter:    CDBSAStudy * out_node_study
* Parameter:    CDBSASeries * out_node_series
* Parameter:    CDBSAImage * out_node_image
* Parameter:    DBSAFactory * factory
* Parameter:    CDBSATagErrorList * error_list
* Returns:      void
*************************************************************/
void CLocalDBMgr::CleanUpLocalDBMgr(CDBSAVR *leaf,
                                    CDBSAVRNode *node,
                                    CDBSAStudy *out_node_study,
                                    CDBSASeries *out_node_series,
                                    CDBSAImage *out_node_image,
                                    DBSAFactory *factory,
                                    CDBSATagErrorList *error_list)
{
    TIPREP_TRACE(CLocalDBMgr::CleanUpLocalDBMgr,MRTraceMarker3);
    
    //  WSACleanup();//to remove WinSock DLL from process space and its Traces
    try {
        if(NULL != factory) {
            delete factory;
            factory = NULL;
        }
        
        if(NULL != node) {
            delete node;
            node = NULL;
        }
        
        if(NULL != out_node_study) {
            DBSA_FreeStudy(out_node_study);
            out_node_study = NULL;
        }
        
        if(NULL != out_node_series) {
            DBSA_FreeSeries(out_node_series);
            out_node_series = NULL;
        }
        
        if(NULL != out_node_image) {
            DBSA_FreeImage(out_node_image);
            out_node_image = NULL;
        }
        
        if(NULL != error_list) {
            DBSA_FreeErrTagList(&error_list);
            error_list = NULL;
        }
        
        WSACleanup(); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in CleanUpLocalDBMgr");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in CleanUpLocalDBMgr");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in CleanUpLocalDBMgr");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       ConvertAcqOrderToString
* Description:  This function is used to return the acqorder
*               in string format
* Parameter:    const WORD * f_acqorder
* Parameter:    const int f_acqordersize
* Parameter:    vector<CString> & f_csAcqVec
* Returns:      int
*************************************************************/
int CLocalDBMgr::ConvertAcqOrderToString(
                                         const WORD* f_acqorder,
                                         const int f_acqordersize,
                                         /*out*/vector<CString> &f_csAcqVec)
{
    TIPREP_TRACE(CLocalDBMgr::ConvertAcqOrderToString,MRTraceMarker3);
    
    TCHAR l_tmpbuff[MAX_BUFFER] = {0};
    int l_loc = 0;
    
    if(NULL == f_acqorder) {
        CString l_csstatus ;
        l_csstatus.Format(_T("f_acqorder is %d "), f_acqorder);
        CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    // for writing l_acqorder in log
    while(l_loc < f_acqordersize) {
        CString l_buf(_T(""));
        _ltow(f_acqorder[l_loc], l_tmpbuff, NUM_10);
        l_buf = l_tmpbuff;
        //         l_buf += _T(",");
        f_csAcqVec.push_back(l_buf);
        l_loc++;
    }
    
    return E_NO_ERROR ;
}

/*************************************************************
* Method:       GetSeriesNumberFromDB
* Description:  This function is used
* Parameter:    CString f_csSriesLoid
* Parameter:    long * f_isSeriesNumString
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetSeriesNumberFromDB(CString &f_csSriesLoid, long *f_isSeriesNumString)
{
    TIPREP_TRACE(CLocalDBMgr::GetSeriesNumberFromDB,MRTraceMarker3);
    int l_nStatus = 0;
    int ret = -1;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL;
    
    try {
        if(NULL == f_isSeriesNumString) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("f_isSeriesNumString is %x"), f_isSeriesNumString);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            // ret = factory->DBSA_LoadSeries(&out_node, &error_list,All, (LPCTSTR)f_csSriesLoid, node);//ALL-> PQM mode
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSriesLoid);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            ret = out_node->DBSA_GetValue(&leaf, TIPREP_SERIES_NUMBER, 1, &error_list);
            
            if(ret == MRC_DBSA_SUCCESS && leaf != NULL) {
                ret = leaf->GetValue(f_isSeriesNumString, 1);
                
                if(ret != MRC_DBSA_SUCCESS) {
                    CString l_csstatus = EMPTY_STRING;
                    l_csstatus.Format(_T("ret val from leaf is %d , 0x00200011 not found"), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                    this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                    l_nStatus = E_SERIES_NO_FAIL;
                    return(l_nStatus);
                }
            } else {
                CString l_csstatus = EMPTY_STRING;
                l_csstatus.Format(_T("ret val from out_node is %d , 0x00200011 tag not found"), ret);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                l_nStatus = E_DB_ERROR;
                return(l_nStatus);
            }
        } else {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("ret val from factory is %d  DBSA_LoadSeries failed"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_DB_ERROR;
            return(l_nStatus);
        }
        
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    }
    
    return(l_nStatus);
}
/*************************************************************
* Method:       GetAcqStatusFromDB
* Description:  This function is used
* Parameter:    CString f_csSriesLoid
* Parameter:    long * f_isSeriesNumString
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetAcqStatusFromDB(CString &f_csSriesLoid, long *f_AcqStatus)
{
    TIPREP_TRACE(CLocalDBMgr::GetSeriesNumberFromDB,MRTraceMarker3);
    int l_nStatus = 0;
    int ret = -1;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL;
    
    try {
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            // ret = factory->DBSA_LoadSeries(&out_node, &error_list,All, (LPCTSTR)f_csSriesLoid, node);//ALL-> PQM mode
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSriesLoid);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            ret = out_node->DBSA_GetValue(&leaf, TIPREP_SERIES_ACQ_STATUS, 1, &error_list);//DCM_MR_EXE_PROTOCOL_ACQ_STATUS
            
            if(ret == MRC_DBSA_SUCCESS && leaf != NULL) {

                ret = leaf->GetValue(f_AcqStatus,leaf->GetSize());
                
                if(ret != MRC_DBSA_SUCCESS) {
                    CString l_csstatus = EMPTY_STRING;
                    l_csstatus.Format(_T("ret val from leaf is %d , TIPREP_SERIES_ACQ_STATUS not found"), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                    this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                    l_nStatus = E_SERIES_NO_FAIL;
                    return(l_nStatus);
                }
            } else {
                CString l_csstatus = EMPTY_STRING;
                l_csstatus.Format(_T("ret val from out_node is %d , TIPREP_SERIES_ACQ_STATUS tag not found"), ret);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                l_nStatus = E_DB_ERROR;
                return(l_nStatus);
            }
        } else {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("ret val from factory is %d  DBSA_LoadSeries failed"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_DB_ERROR;
            return(l_nStatus);
        }
        
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    }
    
    return(l_nStatus);
}

//+priyanka-Mpower 051
/*************************************************************
* Method:       TagReadingForImageData
* Description:  This function is used
* Parameter:    CString f_image_loid
* Parameter:    int * f_size
* Parameter:    unsigned short * f_image_width
* Parameter:    unsigned short * f_image_height
* Parameter:    unsigned short * f_image_bit
* Parameter:    void * * f_img_data
* Returns:      int
*************************************************************/
int CLocalDBMgr::TagReadingForImageData(CString &f_image_loid,
                                        int *f_size,
                                        unsigned short *f_image_width,
                                        unsigned short *f_image_height,
                                        unsigned short *f_image_bit,
                                        void **f_img_data,
                                        float *f_img_scale_factor)
{
    TIPREP_TRACE(CLocalDBMgr::TagReadingForImageData,MRTraceMarker3);
    
    CDBSAVRNode *l_node = NULL;
    CDBSAVR *l_image_width_leaf = NULL;
    CDBSAVR *l_image_height_leaf = NULL;
    CDBSAVR *l_image_bit_leaf = NULL;
    CDBSAVR *l_image_data_leaf = NULL;
    CDBSAVR *l_image_scale_factor = NULL;
    CString l_error_display(_T(""));
    unsigned int l_data_size = 0;
    status_t l_leaf_reading_status = E_NO_ERROR;
    
    try{
        l_image_width_leaf = new CDBSAVRUS(TIPREP_IMAGE_WIDTH);
        l_image_height_leaf = new CDBSAVRUS(TIPREP_IMAGE_HEIGHT);
        l_image_bit_leaf = new CDBSAVRUS(TIPREP_IMAGE_BITS);
        l_image_data_leaf = new CDBSAVRIS(TIPREP_IMG_TAG);
        l_image_scale_factor = new CDBSAVRIS(TIPREP_IMG_SCALE);
        l_node =   new CDBSAVRNode();
        
        l_node->addLeaf(l_image_width_leaf);
        l_node->addLeaf(l_image_height_leaf);
        l_node->addLeaf(l_image_bit_leaf);
        l_node->addLeaf(l_image_data_leaf);
        l_node->addLeaf(l_image_scale_factor);
        CDBSAVRLeaf* l_main_image_leaf = NULL;
        
        DBSAFactory factory;
        CDBSAImage *l_out_node = NULL;
        CDBSATagErrorList *l_error_list = NULL;
        int l_result_of_loading = -1;
        
        l_result_of_loading = factory.DBSA_LoadImage(&l_out_node, 
            &l_error_list, All,(LPCTSTR)f_image_loid, l_node);
        
        if(l_result_of_loading == MRC_DBSA_SUCCESS && l_out_node) {
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMAGE_WIDTH);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                *f_size = l_data_size;
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_image_width, l_data_size);
            } else {
                l_error_display = (_T("0x00280010 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            
            l_main_image_leaf = NULL;
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMAGE_HEIGHT);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                *f_size = l_data_size;
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_image_height, l_data_size);
            } else {
                l_error_display = (_T("0x00280011 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            
            l_main_image_leaf = NULL;
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMAGE_BITS);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                *f_size = l_data_size;
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_image_bit, l_data_size);
            } else {
                l_error_display = (_T("0x00280100 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            
            l_main_image_leaf = NULL;
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMG_TAG);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                *f_size = l_data_size;
                *f_img_data = (void*) /*malloc*/operator new(l_data_size);
                ((CDBSAVR*)l_main_image_leaf)->GetValue(*f_img_data, l_data_size);
            } else {
                l_error_display = (_T("0x7FE00010 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_DATA_FAIL;
            }
            //+priyanka-add-offsets
            l_main_image_leaf = NULL;
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMG_SCALE);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_img_scale_factor, l_data_size);
            } else {
                l_error_display = (_T("0xE4011008 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            
        }
        
        //Release Memory
        if(l_out_node) {
            DBSA_FreeImage(l_out_node);
        }
        
        if(l_error_list) {
            DBSA_FreeErrTagList(&l_error_list);   
        }
        
        if(NULL != l_node) {
            delete l_node;
            l_node = NULL;
        }
        
    
    } 
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception in TagReadingForHeightWidthImage"), __LINE__, __FILE__);
        l_leaf_reading_status = E_ERROR;
    }

    return l_leaf_reading_status;
}

/*************************************************************
* Method:    	TagReadingForHeightWidthImage
* Description: 	This function is used 
* Parameter: 	CString & f_image_loid :
* Parameter: 	unsigned short * f_image_width :
* Parameter: 	unsigned short * f_image_height :
* Returns:   	int
*************************************************************/
int CLocalDBMgr::TagReadingForHeightWidthImage(CString &f_image_loid,                                      
                                               unsigned short *f_image_width,
                                               unsigned short *f_image_height
                                               )
{
    TIPREP_TRACE(CLocalDBMgr::TagReadingForHeightWidthImage,MRTraceMarker3);
    CString l_error_display_string(_T(""));
    DBSAFactory factory;
    CDBSAImage *l_out_node = NULL;
    CDBSATagErrorList *l_error_list = NULL;
    CDBSAVR *l_image_width_leaf = NULL;
    CDBSAVR *l_image_height_leaf = NULL;
    int l_result_of_loading = -1;
    CString l_error_display(_T(""));
    unsigned int l_data_size = 0;
    status_t l_leaf_reading_status = E_NO_ERROR;

    try{
        CDBSAVRNode *l_node =   new CDBSAVRNode();
        l_image_width_leaf = new CDBSAVRUS(TIPREP_IMAGE_WIDTH);
        l_image_height_leaf = new CDBSAVRUS(TIPREP_IMAGE_HEIGHT);
        l_node->addLeaf(l_image_width_leaf);
        l_node->addLeaf(l_image_height_leaf);
        CDBSAVRLeaf* l_main_image_leaf = NULL;
        l_result_of_loading = factory.DBSA_LoadImage(&l_out_node, 
            &l_error_list, All, 
            (LPCTSTR)f_image_loid, l_node);
        if(l_result_of_loading == MRC_DBSA_SUCCESS && l_out_node) {
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMAGE_WIDTH);
            if(NULL == l_out_node) {
                CString l_csstatus ;
                l_csstatus.Format(_T("l_out_node NULL %d"), l_out_node);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                return E_SOCKET_ERR;
        }
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_image_width, l_data_size);
            } else {
                l_error_display = (_T("0x00280010 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            l_main_image_leaf = NULL;
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_IMAGE_HEIGHT);
            if(NULL == l_out_node) {
                CString l_csstatus ;
                l_csstatus.Format(_T("l_out_node NULL %d"), l_out_node);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                return E_SOCKET_ERR;
        }
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_image_height, l_data_size);
            } else {
                l_error_display = (_T("0x00280011 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
            if(l_out_node) {
                DBSA_FreeImage(l_out_node);
            }
            if(l_error_list) {
                DBSA_FreeErrTagList(&l_error_list);    
            }
            if(NULL != l_node) {
                delete l_node;
                l_node = NULL;
            }
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception in TagReadingForHeightWidthImage"), __LINE__, __FILE__);
        l_leaf_reading_status = E_ERROR;
    }

    return l_leaf_reading_status;
}

int CLocalDBMgr::GetProtocolIndexFromSeriesLevelDB(CString &f_csSeriesLoid,int &f_nProtcolIndex)
{
    TIPREP_TRACE(CLocalDBMgr::GetProtocolIndexFromSeriesLevelDB,MRTraceMarker3);
    int l_nStatus = 0;
    int ret = -1;
    long tag = TIPREP_PROTOCOL_PATH;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL; //Need not make pointer
    int sz = 0;
    
    try {
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_PROTOCOL_PATH);
        node->addLeaf(leaf);
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSeriesLoid, node);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            
                CDBSAVRLeaf* series_num_leaf = NULL;
                series_num_leaf = out_node->FindLeaf(tag);
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                
                if(series_num_leaf) {
                    sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                    void *load_leaf_val = NULL;
                    load_leaf_val = (void*)/*malloc*/operator new(sz);
                    LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                    ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                    CString l_csTempStr(_T(""));
                    l_csTempStr.Format(_T("%s"), *l_value);
                    //int cnt = l_csTempStr.GetLength() - 1;
                    CString l_csprotindex = l_csTempStr.Right(1);
                    char buf[16];
                    sprintf(buf,"%S",l_csprotindex);
                    f_nProtcolIndex = atoi(buf);
                    if(NULL != load_leaf_val) {
                        /*free*/operator delete(load_leaf_val);
                        load_leaf_val = NULL;
                    }
                } else {
                    CString l_csstatus(_T("0x00291051 node is not found."));
                    l_csstatus.Format(_T("ret val from out_node is %d. TIPREP_PROTOCOL_PATH node is not found."), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                    this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                    l_nStatus = E_ERROR;
                    return(l_nStatus);
                }
            
        } else {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("ret val from DBSA_LoadSeries is %d "), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_IMG_TAG_PROT_FAIL;
            return(l_nStatus);
        }
        
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolIndexFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolIndexFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in GetProtocolIndexFromSeriesLevelDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    }
    
    return(l_nStatus);
}

/*************************************************************
* Method:       
* Description:  This function is used
* Parameter:    CString f_csSeriesLoid
* Parameter:    CString & f_csProtocolName
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetProtocolSequenceFromSeriesLevelDB(CString &f_csSeriesLoid,                                                   
                                                  CString &f_csProtocolSequence)
{
    TIPREP_TRACE(CLocalDBMgr::GetProtocolSequenceFromSeriesLevelDB,MRTraceMarker3);
    int l_nStatus = 0;
    int ret = -1;
    long tag = TIPREP_PLN_TEMPLATE_SEQUENCE;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL; //Need not make pointer
    int sz = 0;
    
    try {
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_PLN_TEMPLATE_SEQUENCE);
        node->addLeaf(leaf);
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSeriesLoid, node);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            
                CDBSAVRLeaf* series_num_leaf = NULL;
                series_num_leaf = out_node->FindLeaf(tag);
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                
                if(series_num_leaf) {
                    sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                    void *load_leaf_val = NULL;
                    load_leaf_val = (void*)/*malloc*/operator new(sz);
                    LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                    ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);

                    f_csProtocolSequence.Format(_T("%s"), *l_value);  
                    
                    if(NULL != load_leaf_val) {
                        /*free*/operator delete(load_leaf_val);
                        load_leaf_val = NULL;
                    }
                } else {
                    CString l_csstatus(_T("TIPREP_PLN_TEMPLATE_SEQUENCE node is not found."));
                    l_csstatus.Format(_T("ret val from out_node is %d. 0x00291051 node is not found."), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                    this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                    l_nStatus = E_ERROR;
                    return(l_nStatus);
                }
        } else {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("ret val from DBSA_LoadSeries is %d "), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_IMG_TAG_PROT_FAIL;
            return(l_nStatus);
        }
        
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolSequenceFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolSequenceFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in GetProtocolSequenceFromSeriesLevelDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    }
    
    return(l_nStatus);
}

/*************************************************************
* Method:       GetProtocolNameFromSeriesLevelDB
* Description:  This function is used
* Parameter:    CString f_csSeriesLoid
* Parameter:    CString & f_csProtocolName
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetProtocolNameFromSeriesLevelDB(CString &f_csSeriesLoid, 
                                                  CString &f_csProtocolName, 
                                                  CString &f_csProcessName)
{
    TIPREP_TRACE(CLocalDBMgr::GetProtocolNameFromSeriesLevelDB,MRTraceMarker3);
    int l_nStatus = 0;
    int ret = -1;
    long tag = TIPREP_SERIES_PATH;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL; //Need not make pointer
    int sz = 0;
    
    try {
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_SERIES_PATH);
        node->addLeaf(leaf);
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_ERROR;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSeriesLoid, node);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            ret = out_node->DBSA_GetValue(&leaf, TIPREP_SERIES_NUMBER, 1, &error_list);
            
            if(ret == MRC_DBSA_SUCCESS && leaf != NULL) {
                CDBSAVRLeaf* series_num_leaf = NULL;
                series_num_leaf = out_node->FindLeaf(tag);
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                
                if(series_num_leaf) {
                    sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                    void *load_leaf_val = NULL;
                    load_leaf_val = (void*)/*malloc*/operator new(sz);
                    LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                    ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                    CString l_csTempStr(_T(""));
                    l_csTempStr.Format(_T("%s"), *l_value);
                    f_csProcessName = l_csTempStr;
                    int cnt = l_csTempStr.ReverseFind(_T('/'));
                    f_csProtocolName = l_csTempStr.Left(cnt);
                    
                    if(NULL != load_leaf_val) {
                        /*free*/operator delete(load_leaf_val);
                        load_leaf_val = NULL;
                    }
                } else {
                    CString l_csstatus(_T("0x00291051 node is not found."));
                    l_csstatus.Format(_T("ret val from out_node is %d. 0x00291051 node is not found."), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                    this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                    l_nStatus = E_ERROR;
                    return(l_nStatus);
                }
            } else {
                CString l_csstatus = EMPTY_STRING;
                l_csstatus.Format(_T("ret val from DBSA_GetValue is %d "), ret);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
                this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
                l_nStatus = E_ERROR;
                return(l_nStatus);
            }
        } else {
            CString l_csstatus = EMPTY_STRING;
            l_csstatus.Format(_T("ret val from DBSA_LoadSeries is %d "), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
            l_nStatus = E_IMG_TAG_PROT_FAIL;
            return(l_nStatus);
        }
        
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolNameFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetProtocolNameFromSeriesLevelDB");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in GetProtocolNameFromSeriesLevelDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        CleanUpLocalDBMgr(leaf, node, NULL, out_node, NULL, factory, error_list);
        l_nStatus = E_ERROR;
    }
    
    return(l_nStatus);
}

/*************************************************************
* Method:       CopySeries
* Description:  This function is used
* Parameter:    LPTSTR * out_series
* Parameter:    LPCTSTR parent_study_loid
* Parameter:    LPCTSTR source_series_loid
* Returns:      int
*************************************************************/
int CLocalDBMgr::CopySeries(CString & fcsout_series, 
                            LPCTSTR parent_study_loid, 
                            LPCTSTR source_series_loid)
{
    TIPREP_TRACE(CLocalDBMgr::CopySeries,MRTraceMarker3);
    //int ret = -1;
    int sz = _ttoi(parent_study_loid);
    LPTSTR out_series = NULL;
    
    try {
        out_series = new TCHAR[sz];
        
        if(NULL == out_series) {
            CString l_csmsg(_T("CopySeries memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        int series_number = 0;
        
        if(BeginCreateSeriesNumber() >= 0) {
            series_number =  CopyCorrespondingSeries(source_series_loid, out_series, 56);
            EndCreateSeriesNumber();
        }
        
        if(out_series) {
            fcsout_series = out_series;
            delete [] out_series;
            out_series = NULL;
        } 
        
        if(series_number == -1) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Series creation (CreateSeriesNumber) dll failed to create"), __LINE__, __FILE__);
            return E_IMG_SERIES;
        }
    } catch(...) {
        CString l_csmsg(_T("CopySeries Exception"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        
        if(out_series) {
            delete [] out_series;
            out_series = NULL;
        } 
        
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

//************************************
// Method:    CopyImage
// FullName:  CLocalDBMgr::CopyImage
// Access:    private
// Returns:   int
// Qualifier:
// Parameter: CString & f_csout_img_loid
// Parameter: LPCTSTR parent_series_loid
// Parameter: LPCTSTR source_image_loid
//************************************
int CLocalDBMgr::CopyImage(CString & f_csout_img_loid, 
                           LPCTSTR parent_series_loid, 
                           LPCTSTR source_image_loid)
{
    TIPREP_TRACE(CopyImage,MRTraceMarker3);
    int ret = -1;
    DBSAFactory factory;
    CDBSATagErrorList *error_list = NULL;
    LPTSTR out_img_loid = NULL;
    CDBSAVRNode *node =   NULL;
    
    try {
        ret = factory.DBSA_CopyImage(&out_img_loid, &error_list, 
            parent_series_loid, source_image_loid, node);
        
        if(ret != MRC_DBSA_SUCCESS) {
            CString l_csmsg(_T("DBSA_CopyImage failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            if (error_list) {
                DBSA_FreeErrTagList(&error_list);
            }
            if (node) {
                DBSA_FreeNode(node);
            }
            return E_ERROR; 
        }
        
        f_csout_img_loid = out_img_loid;
        if (error_list) {
            DBSA_FreeErrTagList(&error_list);
        }
        if (node) {
            DBSA_FreeNode(node); 
        } 
    } catch(...) {
        CString l_csmsg(_T("CopyImage Exception"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       UpdateImageBuffer
* Description:  This function is used
* Parameter:    CString f_csimgLoid
* Parameter:    void * imgBuff
* Parameter:    unsigned int imgSz
* Returns:      int
*************************************************************/
int CLocalDBMgr::UpdateImageBuffer(CString &f_csimgLoid, void * imgBuff, unsigned int imgSz)
{
    TIPREP_TRACE(CLocalDBMgr::UpdateImageBuffer,MRTraceMarker3);
    CDBSAVRNode *node =   new CDBSAVRNode();
    CDBSAVR *leaf = NULL;
    WSADATA wsaData;
    DBSAFactory factory;
    CDBSATagErrorList *error_list = NULL;
    int status = 0;
    
    if(NULL == imgBuff || imgSz <= 0) {
        CString l_csstatus ;
        l_csstatus.Format(_T("UpdateImageBuffer() Paramters NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    try {
        status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
        }
        
        leaf = new CDBSAVROB(TIPREP_IMG_TAG);
        leaf->SetValue(imgBuff, imgSz);
        node->addLeaf(leaf);
        int ret = 0;
        ret = factory.DBSA_WriteImage(&error_list, f_csimgLoid, node);
        
        if(ret != MRC_DBSA_SUCCESS) {
            CString lcserrmsg(_T(""));
            lcserrmsg.Format(_T("DBSA_WriteImage status :%d"), ret);
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            return E_DB_UPDATE_ERROR;
        }
        
        WSACleanup();
        
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
        }
        
        if(node) {
            DBSA_FreeNode(node);
            node = NULL;
        }
    } catch(...) {
        CString l_csmsg(_T("UpdateImageBuffer Exception"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
        }
        
        if(node) {
             DBSA_FreeNode(node);
            node = NULL;
        }
        
        return E_ERROR;
    }
    
    return 0;
}

/*************************************************************
* Method:       GetImgBuffer
* Description:  This function is used
* Parameter:    CString f_ImageLoid
* Parameter:    void * * imgBuff
* Parameter:    unsigned int * imgSz
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetImgBuffer(CString &f_ImageLoid, void ** imgBuff, 
                              unsigned int *imgSz)
{
    TIPREP_TRACE(CLocalDBMgr::GetImgBuffer,MRTraceMarker3);
    status_t getstudy_status = E_NO_ERROR;
    int ret = -1;
    DBSAFactory factory;
    CDBSAImage *out_node = NULL;
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    CDBSAVR *leaf_scalefact = NULL;
    CDBSAVR *leaf_inversion_time = NULL;
    CDBSAVRNode *node =   NULL;
    
    try {
        if(NULL == imgBuff) {
            CString l_csmsg(_T("GetImgBuffer() memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            CleanUpLocalDBMgr(NULL, node, NULL, NULL, out_node, NULL, error_list);
            return E_ERROR;
        }
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(TIPREP_IMG_TAG);
        leaf_scalefact = new CDBSAVRIS(TIPREP_IMG_SCALE);
        leaf_inversion_time = new CDBSAVRIS(TIPREP_IMG_INVERSION_TIME);
        node->addLeaf(leaf);
        node->addLeaf(leaf_scalefact);
        node->addLeaf(leaf_inversion_time);
        int sz = 0;
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        CString l_csstatus ;
        
        if(status != 0) {
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
        }
        
        ret = factory.DBSA_LoadImage(&out_node, &error_list, All, (LPCTSTR)f_ImageLoid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            CDBSAVRLeaf* series_num_leaf = NULL;
            series_num_leaf = out_node->FindLeaf(TIPREP_IMG_TAG);
            
            if(NULL == series_num_leaf) {
                CString l_csmsg(_T("GetImgBuffer() Unable to find tag Image Data"));
                CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                return E_ERR_IMG_LOAD_FAIL;
            }
            
            sz = ((CDBSAVR*)series_num_leaf)->GetSize();
            *imgSz = sz;
            *imgBuff = (void*) /*malloc*/ operator new(sz);
            if(NULL == *imgBuff) {
                CString l_csmsg(_T("GetImgBuffer() memory allocation failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                CleanUpLocalDBMgr(NULL, node, NULL, NULL, out_node, NULL, error_list);
                return E_ERROR;
            }
            
            if(series_num_leaf) {
                ((CDBSAVR*)series_num_leaf)->GetValue(*imgBuff, sz);
            } else {
                CString l_csErrMsg(_T("0x7FE00010 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);                
//                 if(imgBuff) {
//                     free(imgBuff);
//                 }                
//                 imgBuff = NULL;
                *imgSz = 0;
                CleanUpLocalDBMgr(NULL, node, NULL, NULL, out_node, NULL, error_list);
                return E_ERR_IMG_LOAD_FAIL;
            }
        } else {
            CString l_csmsg(_T("GetImgBuffer() Unable to get image data from DB "));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            getstudy_status =  E_ERR_IMG_LOAD_FAIL;
        }
//         if(imgBuff) {
//             free(imgBuff);
//         }        
//         imgBuff = NULL;
        CleanUpLocalDBMgr(NULL, node, NULL, NULL, out_node, NULL, error_list);
    } catch(...) {
        CString l_csmsg(_T("GetImgBuffer() Unable to get image data from DB "));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        
//         if(imgBuff) {
//             free(imgBuff);
//         }        
//         imgBuff = NULL;
        *imgSz = 0;
        CleanUpLocalDBMgr(NULL, node, NULL, NULL, out_node, NULL, error_list);
        return E_ERR_EXCEPTION;
    }    
    return getstudy_status;
}

/*************************************************************
* Method:       UpdateAcqOrder
* Description:  This function is used
* Parameter:    CString & SeriesLoid
* Returns:      int
*************************************************************/
int CLocalDBMgr::UpdateAcqOrder(CString & SeriesLoid)
{
    TIPREP_TRACE(CLocalDBMgr::UpdateAcqOrder,MRTraceMarker3);
    DBSAFactory factory;
    CDBSATagErrorList *error_list = NULL;
    int ret = 0;
    CDBSAStudy *study_obj = NULL;
    CDBSASeries * series = NULL;
    WORD *acq_order = NULL;
//     int acq_order_size = 0;
    WORD *arr_acq_order = NULL;
    WSADATA wsaData;
    int status = 0;
    
    try {
        status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
        }
        
        int sz = 0;
        ret = factory.DBSA_CreateStudy(&study_obj, &error_list, All, NULL, NULL, SeriesLoid);
        
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
            error_list = NULL;
        }
        
        if(ret == MRC_DBSA_SUCCESS && study_obj) {
            int acq_order_size = study_obj->DBSA_GetAcqOrder(&acq_order);
            
            if(acq_order_size > 0) {
                arr_acq_order = new WORD[acq_order_size+1];
                
                if(NULL == arr_acq_order) {
                    CString l_csmsg(_T("UpdateAcqOrder memory allocation failed"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                    
                    if(acq_order) {
                        delete []acq_order;
                        acq_order = NULL;
                    }
                    
                    CleanUpLocalDBMgr(NULL, NULL, study_obj, NULL, NULL, NULL, error_list);
                    return E_MEM_ALLOC;
                }
                
                for(sz = 0; sz < acq_order_size; sz++) {
                    arr_acq_order[sz] = acq_order[sz];
                }
            } else {
                CString l_csmsg(_T("UpdateAcqOrder DBSA_GetAcqOrder() acq order size in invalid"));
                CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                CleanUpLocalDBMgr(NULL, NULL, study_obj, NULL, NULL, NULL, error_list);
                return E_DB_ERROR;
            }
            
            ret = factory.DBSA_CreateSeries(&series, &error_list, All, NULL, NULL, SeriesLoid);
            
            if(error_list) {
                DBSA_FreeErrTagList(&error_list);
                error_list = NULL;
            }
            
            if(ret == MRC_DBSA_SUCCESS && series) {
                WORD series_index = 0;
                series_index = series->DBSA_GetIndex();
                arr_acq_order[sz++] = series_index;
                study_obj->DBSA_AcqOrderUpdate(arr_acq_order, sz);
                ret = study_obj->DBSA_Put(&error_list);
                
                if(ret != MRC_DBSA_SUCCESS) {
                    CString l_csmsg ;
                    l_csmsg.Format(_T("UpdateAcqOrder DBSA_Put() failed with status (%d)"), ret);
                    CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                    
                    if(arr_acq_order) {
                        delete[] arr_acq_order;
                        arr_acq_order = NULL;
                    }
                    
                    if(acq_order) {
                        delete []acq_order;
                        acq_order = NULL;
                    }
                    
                    CleanUpLocalDBMgr(NULL, NULL, study_obj, series, NULL, NULL, error_list);
                    return E_DB_UPDATE_ERROR;
                } else {
                    CString msg;
                    msg.Format(_T("DBSA_CreateSeries failed to get series node to update"));
                    CTiPrepLogHelper::WriteToErrorLog(msg,__LINE__,__FILE__);
                    status = E_DB_ERROR;
                }
            }
        }
        
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
            error_list = NULL;
        }
        
        if(arr_acq_order) {
            delete[] arr_acq_order;
            arr_acq_order = NULL;
        }
        
        if(series) {
            DBSA_FreeSeries(series);
            series = NULL;
        }
        
        if(study_obj) {
            DBSA_FreeStudy(study_obj);
        }
        
        if(acq_order) {
            delete []acq_order;
            acq_order = NULL;
        }
        
        WSACleanup();
    } catch(...) {
        CString l_csmsg(_T("Exception occured in UpdateAcqOrder"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }

    return 0;
}

/*************************************************************
* Method:       CreateSeriesCopy
* Description:  This function is used
* Parameter:    vector<CString> & f_inputImgLOIDs
* Parameter:    vector<CString> & f_outImgLOIDs
* Returns:      int
*************************************************************/
int CLocalDBMgr::CreateSeriesCopy(/*in*/vector<CString> &f_inputImgLOIDs,
                                  /*out*/vector<CString> &f_outImgLOIDs, CString &f_csseries)
{
    TIPREP_TRACE(CLocalDBMgr::CreateSeriesCopy,MRTraceMarker3);
    CString l_csnewseries_loid(_T(""));
    unsigned int i = 0;
    
    if(f_inputImgLOIDs.size() <= 0) {
        CString lcsmsg(_T("Input series total images is 0 in CreateSeriesCopy()"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    WSADATA wsaData;
    
    try {
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_SOCKET_ERR;
        }
        
        CString series_loid(_T(""));
        GetParentLOID(f_inputImgLOIDs.at(0), LOID_TYPE_IMAGE, series_loid);
        CString  study_loid(_T(""));
        GetParentLOID(series_loid, LOID_TYPE_SERIES, study_loid);
        
        if(series_loid.CompareNoCase(_T("")) == 0 || study_loid.CompareNoCase(_T("")) == 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("unable to get parent LOID, Cannot create series"));
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(E_NO_ERROR != CopySeries(l_csnewseries_loid, study_loid, series_loid)) {
            WSACleanup();
            return E_IMG_SERIES;
        }
        f_csseries = l_csnewseries_loid;
        
//         if(E_NO_ERROR != UpdateAcqOrder(l_csnewseries_loid)) {
//         }
        
        CString l_csimgLOID(_T(""));
        f_outImgLOIDs.clear();
        
        for(i = 0 ; i < f_inputImgLOIDs.size(); i++) {
            l_csimgLOID = (_T(""));
            
            if(E_NO_ERROR != CopyImage(l_csimgLOID, l_csnewseries_loid, f_inputImgLOIDs.at(i))) {
                f_outImgLOIDs.clear();
                WSACleanup();
                return E_IMG_SERIES;
            }
            
            f_outImgLOIDs.push_back(l_csimgLOID);
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in CreateSeriesCopy()"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       GetNumImagesInSeries
* Description:  This function is used
* Parameter:    CString & series_loid
* Parameter:    int & f_nnumImages
* Returns:      int
*************************************************************/
int CLocalDBMgr::GetNumImagesInSeries(CString &series_loid, int &f_nnumImages)
{
    TIPREP_TRACE(CLocalDBMgr::GetNumImagesInSeries,MRTraceMarker3);
    status_t getstudy_status = E_NO_ERROR;
    int ret = -1;
    DBSAFactory factory;
    CDBSASeries *out_node = NULL;                //Image level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        CDBSAVRNode *node =   new CDBSAVRNode();
        leaf = new CDBSAVRSL(0x001910b4);
        long num_pln_repetions = 0;
        node->addLeaf(leaf);
        ret = factory.DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)series_loid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            CDBSAVR* series_num_leaf = NULL;
            out_node->DBSA_GetValue(&series_num_leaf, 0x001910b4, 1, &error_list);
            
            if(series_num_leaf) {
                ((CDBSAVR*)series_num_leaf)->GetValue(num_pln_repetions);
                f_nnumImages = num_pln_repetions;
            } else {
                CString l_csErrMsg(_T("0x7FE00010 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                getstudy_status = E_ERROR;
            }
        } else {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetNumImagesInSeries error"), __LINE__, __FILE__);
        }
        
        DBSA_FreeErrTagList(&error_list);
        DBSA_FreeSeries(out_node);
        
        if(node) {
            delete node;
            node = NULL;
        }
        
        WSACleanup();
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred with this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
        //log error
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in GetNumImagesInSeries"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

//************************************Method Header**************************************

//Method Name:  TagReadingForGatingPeriod

//Author:  iGATE/Priyanka

//Purpose:

//***************************************************************************************
int CLocalDBMgr::TagReadingForGatingPeriod(CString &f_image_loid,
                                           int *f_size,
                                           float f_gating_period)
{
    TIPREP_TRACE(CPolarityCorrection::TagReadingForGatingPeriod,MRTraceMarker3);
    CString l_error_display_string(_T(""));
    DBSAFactory factory;
    CDBSAImage *l_out_node = NULL;
    CDBSATagErrorList *l_error_list = NULL;
    CDBSAVR *l_gating_period_leaf = NULL;
    int l_result_of_loading = -1;
    CString l_error_display(_T(""));
    unsigned int l_data_size = 0;
    status_t l_leaf_reading_status = E_NO_ERROR;
    CDBSAVRNode *l_node = NULL;
    WSADATA wsaData;
    
    try  {
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_node =   new CDBSAVRNode();
        l_gating_period_leaf = new CDBSAVRUS(TIPREP_GATING_PERIOD); 
        l_node->addLeaf(l_gating_period_leaf);
        CDBSAVRLeaf* l_main_image_leaf = NULL;
        l_result_of_loading = factory.DBSA_LoadImage(&l_out_node, &l_error_list, All, (LPCTSTR)f_image_loid, l_node);
        
        if(l_result_of_loading == MRC_DBSA_SUCCESS && l_out_node) {
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_GATING_PERIOD);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                *f_size = l_data_size;
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_gating_period, l_data_size);
            } else {
                l_error_display = (_T("0x001911FB leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
        } else {
            CString lcsmsg ;
            lcsmsg.Format(_T("DBSA_LoadImage failed with ret status %d"),l_result_of_loading);
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg,__LINE__,__FILE__);
            l_leaf_reading_status = E_DB_ERROR;
        }
        
        //Release Memory
        if(l_out_node) {
            DBSA_FreeImage(l_out_node);
        }
        
        if(NULL != l_node) {
            delete l_node;
            l_node = NULL;
        }
        
        if(l_error_list) {
            DBSA_FreeErrTagList(&l_error_list);
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in TagReadingForGatingPeriod"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        
        if(l_out_node) {
            DBSA_FreeImage(l_out_node);
        }
        
        if(NULL != l_node) {
            delete l_node;
            l_node = NULL;
        }
        
        if(l_error_list) {
            DBSA_FreeErrTagList(&l_error_list);
        }
        
        WSACleanup();
        return E_ERR_EXCEPTION;
    }
    
    return l_leaf_reading_status;
}

//************************************Method Header**************************************

//Method Name:  TagReadingForMaxPixelValueOfImage

//Author:  iGATE/Priyanka

//Purpose:

//***************************************************************************************
int CLocalDBMgr::TagReadingForMaxPixelValueOfImage(CString &f_image_loid,
                                           signed short *f_max_pixel_value_of_img_from_tag)
{
    TIPREP_TRACE(CPolarityCorrection::TagReadingForMaxPixelValueOfImage,MRTraceMarker3);
    CString l_error_display_string(_T(""));
    DBSAFactory factory;
    CDBSAImage *l_out_node = NULL;
    CDBSATagErrorList *l_error_list = NULL;
    CDBSAVR *l_max_pixel_value_of_img_leaf = NULL; 
    int l_result_of_loading = -1;
    CString l_error_display(_T(""));
    unsigned int l_data_size = 0;
    status_t l_leaf_reading_status = E_NO_ERROR;
    CDBSAVRNode *l_node = NULL;
    WSADATA wsaData;
    
    try  {
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_node =   new CDBSAVRNode();
        if(l_node == NULL) {
            CString l_csstatus ;
            l_csstatus.Format(_T("l_node NULL %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return E_ERROR;
        }
        l_max_pixel_value_of_img_leaf = new CDBSAVRSS(TIPREP_MAX_IMG_PIXEL_VALUE); 
        l_node->addLeaf(l_max_pixel_value_of_img_leaf);
        CDBSAVRLeaf* l_main_image_leaf = NULL;
        l_result_of_loading = factory.DBSA_LoadImage(&l_out_node, &l_error_list, All, (LPCTSTR)f_image_loid, l_node);
        
        if(l_result_of_loading == MRC_DBSA_SUCCESS && l_out_node) {
            l_main_image_leaf = l_out_node->FindLeaf(TIPREP_MAX_IMG_PIXEL_VALUE);
            
            if(l_main_image_leaf) {
                l_data_size = ((CDBSAVR*)l_main_image_leaf)->GetSize();
                ((CDBSAVR*)l_main_image_leaf)->GetValue(f_max_pixel_value_of_img_from_tag, l_data_size);
            } else {
                l_error_display = (_T("0x00280107 leaf not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                l_leaf_reading_status = E_IMG_TAG_FAIL;
            }
        } else {
            CString lcsmsg ;
            lcsmsg.Format(_T("DBSA_LoadImage failed with ret status %d"),l_result_of_loading);
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg,__LINE__,__FILE__);
            l_leaf_reading_status = E_DB_ERROR;
        }
        
        //Release Memory
        if(l_out_node) {
            DBSA_FreeImage(l_out_node);
        }
        
        if(NULL != l_node) {
            delete l_node;
            l_node = NULL;
        }
        
        if(l_error_list) {
            DBSA_FreeErrTagList(&l_error_list);
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in TagReadingForMaxPixelValueOfImage"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        
        if(l_out_node) {
            DBSA_FreeImage(l_out_node);
        }
        
        if(NULL != l_node) {
            delete l_node;
            l_node = NULL;
        }
        
        if(l_error_list) {
            DBSA_FreeErrTagList(&l_error_list);
        }
        
        WSACleanup();
        return E_ERR_EXCEPTION;
    }
    
    return l_leaf_reading_status;
}

/*************************************************************
* Method:    	GetChildNodeLOIDs
* Description: 	This function is used 
* Parameter: 	CString & f_csloid :
* Parameter: 	int & f_nnumNodes :
* Parameter: 	LOID_TYPE db_level :
* Returns:   	int
*************************************************************/
int CLocalDBMgr::GetChildNodeLOIDs(CString & f_csloid,
                                   int &f_nnumNodes, 
                                   LOID_TYPE db_level)
{
    TIPREP_TRACE(CLocalDBMgr::GetChildNodeLOIDs,MRTraceMarker3);
    MpDbsBaseObjectInfo *ObjInfo = NULL;
    LPWSTR* lpwszOIDsList = 0;
    UINT uChildCount = 0;
    WSADATA wsaData;
    f_nnumNodes = 0;
    int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
    
    if(status != 0) {
        CString l_csstatus ;
        l_csstatus.Format(_T("WSAStartup status %d"), status);
        CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
        return E_SOCKET_ERR;
    }
    
    try {
        switch(db_level) {
        case LOID_TYPE_SERIES:
            ObjInfo = new MpDbsSeriesInfo();
            ObjInfo->AddLoid(f_csloid);
            ObjInfo->SetCollectLoidStatus(f_csloid);
            
            if(MP_DBS_SUCCESS != ObjInfo->Lock()) {
                //MP_EDB_HANDLE_ERROR( "Failed to lock database object." );
                delete ObjInfo;
                return E_DB_ERROR;
            }
            
            if(MP_DBS_SUCCESS != ObjInfo->GetInfoEx(_T("Local"))) {
                //MP_EDB_HANDLE_ERROR( "Failed to read child object OIDs from database." );
                delete ObjInfo;
                return E_DB_ERROR;
            }
            
            if(MP_DBS_SUCCESS != ObjInfo->Unlock(MP_DBS_ROLLBACK)) {
                //MP_EDB_HANDLE_ERROR( "Failed to unlock database object." );
                delete ObjInfo;
                return E_DB_ERROR;
            }
            
            ObjInfo->GetCollectedLoids(f_csloid, lpwszOIDsList, uChildCount);
            f_nnumNodes = uChildCount;//ObjInfo->GetLoidListCount();
            break;
        case LOID_TYPE_STUDY:
            break;
        }
        
        if(ObjInfo) {
            delete ObjInfo;
            ObjInfo = NULL;
        }
        
        if(lpwszOIDsList) {
            delete [] lpwszOIDsList;
            lpwszOIDsList = NULL;
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in GetChildNodeLOIDs()"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	UpdateVFPathInDB
* Description: 	This function is used 
* Parameter: 	CString & csLOID :
* Parameter: 	LPCTSTR vfpath :
* Parameter: 	LOID_TYPE ltype :
* Returns:   	int
*************************************************************/
int CLocalDBMgr::UpdateVFPathInDB(CString &csLOID, 
                                  LPCTSTR vfpath, 
                                  LOID_TYPE ltype)
{
    TIPREP_TRACE(CLocalDBMgr::UpdateVFPathInDB,MRTraceMarker3);
//     LPCTSTR parent_loid = NULL;
//     LPCTSTR temp_loid = NULL;
//     int ret = 0;
//     CDBSASeries *study_obj = NULL;
//     CDBSASeries * series = NULL;
//     WORD *acq_order = NULL;
//     int acq_order_size = 0;
    int err_code = 0;
//     WORD *arr_acq_order = NULL;
    
    try{
        
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return E_ERROR;
        }
        if (NULL == vfpath )
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("UpdateVFPathInDB() function parametrs NULL"),__LINE__,__FILE__);
            return E_ERROR;
        }
        CDBSAVR *leaf = NULL;
        DBSAFactory factory;
        CDBSATagErrorList *error_list = NULL;
        int ret = 0;
        CDBSAVRNode * node = new CDBSAVRNode();
        switch(ltype)
        {
        case LOID_TYPE_IMAGE:
            leaf = new CDBSAVRLO(TIPREP_IMG_PATH);
            if (leaf == NULL)
            {
                CTiPrepLogHelper::WriteToErrorLog(_T("Memory allocation failed UpdateVFPathInDB()"),__LINE__,__FILE__);
                err_code = E_MEM_ALLOC;
                break;
            }
            node->addLeaf(leaf);
            ret = leaf->SetValue(&vfpath,1);
            if (ret != MRC_DBSA_SUCCESS)
            {
                CString msg;
                msg.Format(_T("Tag SetValue() fail status %d"),ret);
                CTiPrepLogHelper::WriteToErrorLog(msg,__LINE__,__FILE__);
                err_code  = E_DB_UPDATE_ERROR;
                break;
            }
            ret = factory.DBSA_WriteImage(&error_list,csLOID,node);
            if (ret != MRC_DBSA_SUCCESS)
            {
                CString msg;
                msg.Format(_T("DBSA_WriteImage() fail status %d"),ret);
                CTiPrepLogHelper::WriteToErrorLog(msg,__LINE__,__FILE__);
                err_code = E_DB_UPDATE_ERROR;
                break;
            }
            break;
        case LOID_TYPE_SERIES:
            leaf = new CDBSAVRLO(TIPREP_SERIES_PATH);
            
            if (leaf == NULL)
            {
                //log error
                err_code = E_MEM_ALLOC;
                break;
            }
            node->addLeaf(leaf);
            
            ret = leaf->SetValue(/*&SetStr*/&vfpath,1);
            if (ret != MRC_DBSA_SUCCESS)
            {
                CString msg;
                msg.Format(_T("Tag SetValue() fail status %d"),ret);
                CTiPrepLogHelper::WriteToErrorLog(msg,__LINE__,__FILE__);
                
                err_code  = E_DB_UPDATE_ERROR;
                break;
            }
            ret = factory.DBSA_WriteSeries(&error_list,csLOID,node );
            if (ret != MRC_DBSA_SUCCESS)
            {
                CString msg;
                msg.Format(_T("DBSA_WriteSeries() fail status %d"),ret);
                CTiPrepLogHelper::WriteToErrorLog(msg,__LINE__,__FILE__);
                
                err_code = E_DB_UPDATE_ERROR;
                break;
            }
            break;
        default:
            CTiPrepLogHelper::WriteToErrorLog(_T("LOID TYPE is invalid"),__LINE__,__FILE__);
            err_code = E_ERROR;
            break;
        }
//         int sz = 0;
        
        if(error_list) {                         
            DBSA_FreeErrTagList(&error_list);
            error_list = NULL;
        }
        if(NULL != node) {
            delete node;
            node = NULL;
        }
        
    }
    catch(...)
    {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in UpdateVFPathInDB()"),__LINE__,__FILE__);
        return E_ERR_EXCEPTION;
        
    }
    
    return err_code;
}

void CLocalDBMgr::InitTest()
{
    TIPREP_TRACE(CLocalDBMgr::InitTest,MRTraceMarker3);
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);            
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return ;
        }
        
        CDBSAVRNode *node =   NULL;
//         CDBSAVR *leaf = NULL;
        
        node = new CDBSAVRNode();
        
        if(NULL == node ){
            WSACleanup();
            return ;
        }
        
        CDBSATagErrorList *error_list = NULL;
        CDBSAImage *out_node = NULL;  //Image level    
        
        DBSAFactory factory;
        CString l_loid (_T("12"));
        int ret = factory.DBSA_LoadImage(&out_node, &error_list, All, (LPCTSTR)l_loid, node);
        
        CString lcsmsg ;
        lcsmsg.Format(_T("DBSA_LoadImage failed with ret status %d"),ret);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, lcsmsg,
            _T("LocalDBMgr::InitTest"));
        //Release Memory
        DBSA_FreeImage(out_node);
        
        //  DBSA_FreeNode(node);
        if(NULL != node) {
            delete node;
            node = NULL;
        }
        if(error_list) {
            DBSA_FreeErrTagList(&error_list);
        }
        WSACleanup();
    } catch(...) {        
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception Occured in CLocalDBMgr::InitTest() "), __LINE__, __FILE__);
    }
}

//AN scanner
int CLocalDBMgr::GetStudyNameFromStudyLoid(CString &loid, CString &studyname)
{
//TIPREP_TRACE(CLocalDBMgr::getProtAcqOrderForStudy);
    int l_nStatus = 0;
    CDBSAStudy *out_node = NULL;            //study level
    CDBSATagErrorList *error_list = NULL;
    DBSAFactory *factory = NULL;
    
    try {
        int ret = -1;
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
           // this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_SOCKET_ERR;
            return(l_nStatus);
        }
        CDBSAVRNode *node =   NULL;
        CDBSAVR *leaf = NULL;
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(0x00291050);
        if(NULL == node || NULL == leaf){
            return E_MEM_ALLOC;
        }else{
            node->addLeaf(leaf);
        }
        factory = new DBSAFactory();//Factory object can be local
        
        if(NULL == factory) {
            CString l_csstatus ;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
           // this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_MEM_ALLOC;
            return(l_nStatus);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadStudy(&out_node, &error_list, All, (LPCTSTR)loid, NULL);
        }
        
        if(ret == MRC_DBSA_SUCCESS && NULL != out_node) {
              CDBSAVRLeaf* series_num_leaf = NULL;
            long tag = 0x00291050;
            series_num_leaf = out_node->FindLeaf(tag);
            if(NULL == out_node) {
                CString l_csstatus ;
                l_csstatus.Format(_T("out_node NULL %d"), out_node);
                CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
				this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
                return E_SOCKET_ERR;
        }
            
            if(series_num_leaf) {
                int sz = 0;
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                void *load_leaf_val = NULL;
                load_leaf_val = (void*)/*malloc*/operator new(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                ///Run40042/Run40042.-1959:/1.3/7.1/8.0/9.0
                CString l_csTempStr = EMPTY_STRING;
                l_csTempStr.Format(_T("%s"), *l_value);
				studyname = l_csTempStr;
                /*free*/operator delete(load_leaf_val); 
                load_leaf_val = NULL;
            } else {
                CString l_csErrMsg(_T("0x00291052 node is not found."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_IMG_TAG_PROT_FAIL;
            }
            
        } else {
            CString l_csstatus = EMPTY_STRING ;
            l_csstatus.Format(_T("ret val from factory is %d LoadStudy failed"), ret);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
            l_nStatus = E_DB_ERROR;
            return(l_nStatus);
        }
        
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list); 
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        this->CleanUpLocalDBMgr(NULL, NULL, out_node, NULL, NULL, factory, error_list);
        l_nStatus = E_ERR_EXCEPTION;
    }
    
    return(l_nStatus);
}

//AN scanner
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
