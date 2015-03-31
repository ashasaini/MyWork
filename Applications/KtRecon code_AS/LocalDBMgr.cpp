// LocalDBMgr.cpp: implementation of the CLocalDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ktrecon.h"
#include "LocalDBMgr.h"
#include "KtReconErrors.h"
#include "Winsock.h"
#include "CommonDefinitions.h"
#include "KtReconLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalDBMgr::CLocalDBMgr()
{

}

CLocalDBMgr::~CLocalDBMgr()
{

}

int CLocalDBMgr::GetProtocolNameFromImageLevelDB( CString f_ImageLoid, CString &f_csProtocolName,CString &f_csStudyName,CString &f_csCompletepath )
{    
    int getstudy_status = E_NO_ERROR;
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        CString l_csstatus ;
        
        if(status != 0) {
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return E_SOCKET_ERR;
            // WSACleanup();
        }
        
        CDBSAVRNode *node =   NULL;
        CDBSAVR *leaf = NULL;
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(KTRECON_IMG_PATH);
        if(NULL == node || NULL == leaf){
            return E_MEM_ALLOC;
        }else{
            node->addLeaf(leaf);
        }
        int sz = 0;
        DBSAFactory factory;
        int ret = -1;
        CDBSATagErrorList *error_list = NULL;
        CDBSAImage *out_node = NULL;  //Image level
        
        ret = factory.DBSA_LoadImage(&out_node, &error_list, All, (LPCTSTR)f_ImageLoid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            CDBSAVRLeaf* series_num_leaf = NULL;
            long tag = KTRECON_IMG_PATH;
            series_num_leaf = out_node->FindLeaf(tag);
            void *load_leaf_val = NULL;
            
            if(series_num_leaf) {
                
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                load_leaf_val = (void*)malloc(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                ///Run40042/Run40042.-1959:/1.3/7.1/8.0/9.0
                CString l_csTempStr(_T(""));
                l_csTempStr.Format(_T("%s"), *l_value);
                f_csCompletepath.Format(_T("%s"), *l_value);//to get path till image level
                int cnt = l_csTempStr.ReverseFind(_T('/'));
                l_csTempStr = l_csTempStr.Left(cnt);
                cnt = l_csTempStr.ReverseFind(_T('/'));
                l_csTempStr = l_csTempStr.Left(cnt);
                f_csStudyName = l_csTempStr;
                cnt = l_csTempStr.ReverseFind(_T('/'));
                f_csProtocolName = l_csTempStr.Left(cnt);
                free(load_leaf_val); 
                load_leaf_val = NULL;
            } else {
                CString l_csErrMsg(_T("0x00291052 node is not found."));
                CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
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
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        l_csStrFormatted += szCause;
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    }
    
    return getstudy_status;
}
int CLocalDBMgr::GetSeriesNumberFromDB(CString &f_csSriesLoid, long *f_isSeriesNumString)
{
    int ret = -1;
    CDBSAVRNode *node =   NULL;
    CDBSASeries *out_node = NULL;            //series level
    CDBSATagErrorList *error_list = NULL;
    CDBSAVR *leaf = NULL;
    DBSAFactory *factory = NULL;
    
    try {
        if(NULL == f_isSeriesNumString) {
            CString l_csstatus = _T("");
            l_csstatus.Format(_T("f_isSeriesNumString is %x"), f_isSeriesNumString);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_ERROR);
        }
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus = _T("");
            l_csstatus.Format(_T("WSAStartup status %d"), status);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_SOCKET_ERR);
        }
        
        factory = new DBSAFactory();
        
        if(NULL == factory) {
            CString l_csstatus = _T("");
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_MEM_ALLOC);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadSeries(&out_node, &error_list, All, (LPCTSTR)f_csSriesLoid);
        }
        
        if(ret == MRC_DBSA_SUCCESS && out_node != NULL) {
            ret = out_node->DBSA_GetValue(&leaf, KTRECON_SERIES_NUMBER, 1, &error_list);
            
            if(ret == MRC_DBSA_SUCCESS && leaf != NULL) {
                ret = leaf->GetValue(f_isSeriesNumString, 1);
                
                if(ret != MRC_DBSA_SUCCESS) {
                    CString l_csstatus = _T("");
                    l_csstatus.Format(_T("ret val from leaf is %d , 0x00200011 not found"), ret);
					CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
                    return(E_DB_ERROR);
                }
            } else {
                CString l_csstatus = _T("");
                l_csstatus.Format(_T("ret val from out_node is %d , 0x00200011 tag not found"), ret);
				CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
                return(E_DB_ERROR);
            }
        } else {
            CString l_csstatus = _T("");
            l_csstatus.Format(_T("ret val from factory is %d  DBSA_LoadSeries failed"), ret);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_DB_ERROR);
        }        
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetSeriesNumberFromDB");
        l_csStrFormatted += szCause;
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in GetSeriesNumberFromDB");
        l_csStrFormatted += szCause;
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in GetSeriesNumberFromDB");
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}



int CLocalDBMgr::GetProtAcqOrderFromDB(CString &f_csStudyLoid,
                                       int *f_nseries_count,
                                       vector<CString> &f_csAcqVec,
                                       LPCTSTR **f_SeriesLOIDArray)
{
    int l_nStatus = 0;
    int ret = -1;
    CDBSAStudy *out_node = NULL;            //study level
    CDBSATagErrorList *error_list = NULL;
    DBSAFactory *factory = NULL;
    WORD* l_acqorder = NULL;
    
    try {
        if(NULL == f_SeriesLOIDArray) {
            CString l_csstatus ;
            l_csstatus.Format(_T("f_SeriesLOIDArray  is %x"), f_SeriesLOIDArray);
            CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_ERROR);
        }
        
        WSADATA wsaData;
        //To Load the WinSock DLL file into process space
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_SOCKET_ERR);
        }
        
        factory = new DBSAFactory();//Factory object can be local
        
        if(NULL == factory) {
            CString l_csstatus ;
            l_csstatus.Format(_T("DBSAFactory Object is %x"), factory);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_MEM_ALLOC);
        }
        
        if(out_node == NULL) {
            ret = factory->DBSA_LoadStudy(&out_node, &error_list, All, (LPCTSTR)f_csStudyLoid, NULL);
        }
        
        if(ret == MRC_DBSA_SUCCESS && NULL != out_node) {
            int l_acqsize  = out_node->DBSA_GetAcqOrder(&l_acqorder, f_SeriesLOIDArray);
            (*f_nseries_count) = l_acqsize;
            l_nStatus = ConvertAcqOrderToString(l_acqorder, l_acqsize, f_csAcqVec);
            
            if(NULL != l_acqorder) {
                DBSA_FreeLocalAcqOrder(l_acqorder);
            }
        } else {
            CString l_csstatus = _T("") ;
            l_csstatus.Format(_T("ret val from factory is %d LoadStudy failed"), ret);
			CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return(E_DB_ERROR);
        }
        
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
        l_csStrFormatted += szCause;
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occurred in getProtAcqOrderForStudy");
		CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

int CLocalDBMgr::ConvertAcqOrderToString(const WORD* f_acqorder,
                                         const int f_acqordersize,
                                         /*out*/vector<CString> &f_csAcqVec)
{
   
    TCHAR l_tmpbuff[MAX_BUFFER] = {0};
    int l_loc = 0;
    
    if(NULL == f_acqorder) {
        CString l_csstatus ;
        l_csstatus.Format(_T("f_acqorder is %d "), f_acqorder);
		CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
        return E_ERROR;
    }
    while(l_loc < f_acqordersize) {
        CString l_buf(_T(""));
        _ltow(f_acqorder[l_loc], l_tmpbuff, NUM_10);
        l_buf = l_tmpbuff;
        f_csAcqVec.push_back(l_buf);
        l_loc++;
    }
    
    return E_NO_ERROR ;
}



int CLocalDBMgr::GetParentLOID(LPCTSTR f_child_node_loid,
                               LOID_TYPE f_child_node_loid_type,
                               CString &f_csParentLoid)
{
    
    CString lcsloid(_T(""));
    lcsloid = f_child_node_loid;
    
    if(!f_child_node_loid || lcsloid.IsEmpty()) {
        CString l_csmsg(_T("Child LOID is empty"));
		CKtReconLogHelper::WriteToErrorLog(l_csmsg, __LINE__);
        return E_ERROR_NULL_FUNCPARAMS;
    }
    
    WSADATA wsaData;
    int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
    CString l_csstatus ;
    
    if(status != 0) {
        l_csstatus.Format(_T("WSAStartup status %d"), status);
        CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
        return E_SOCKET_ERR;
    }
    
    DBSAFactory factory;
    CDBSATagErrorList *error_list = NULL;
    LPCTSTR parent_loid = NULL;
    LPCTSTR temp_loid = NULL;
    int ret = 0;
    int ret_status = 0;
    
    try {
        switch(f_child_node_loid_type) {
        case LOID_TYPE_STUDY: {
            CDBSAPatient *patient_obj = NULL;
            
            try {
                ret = factory.DBSA_CreatePatient(&patient_obj, &error_list, 
                    All, NULL, f_child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && patient_obj) {
                    temp_loid = patient_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        int len = _tcslen(temp_loid);
                        f_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreePatient(patient_obj);
                    patient_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
				CKtReconLogHelper::WriteToErrorLog(message, __LINE__);
                ret_status = E_DB_ERROR;
            }
                              }
            break;
        case LOID_TYPE_SERIES: {
            CDBSAStudy *study_obj = NULL;
            
            try {
                ret = factory.DBSA_CreateStudy(&study_obj, &error_list, 
                    All, NULL, NULL, f_child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && study_obj) {
                    temp_loid = study_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        f_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreeStudy(study_obj);
                    study_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
                ret_status = E_DB_ERROR;
		        CKtReconLogHelper::WriteToErrorLog(message, __LINE__);
                return(E_ERR_EXCEPTION);
            }
                               }
            break;
        case LOID_TYPE_IMAGE: {
            CDBSASeries *series_obj = NULL;
            
            try {
                ret = factory.DBSA_CreateSeries(&series_obj, &error_list, 
                    All, NULL, NULL, f_child_node_loid);
                
                if(ret == MRC_DBSA_SUCCESS && series_obj) {
                    temp_loid = series_obj->DBSA_GetLOID();
                    
                    if(temp_loid) {
                        f_csParentLoid = temp_loid; 
                    }
                    
                    DBSA_FreeSeries(series_obj);
                    series_obj = NULL;
                }
            } catch(...) {
                CString message = _T("");
                message.Format(_T("Unable to get parent loid"));
				CKtReconLogHelper::WriteToErrorLog(message, __LINE__);
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
			CKtReconLogHelper::WriteToErrorLog(message, __LINE__);
        }
        
        WSACleanup();
    } catch(...) {
        CString lcsmsg(_T("exception occured in GetParentLOID()"));
		CKtReconLogHelper::WriteToErrorLog(lcsmsg, __LINE__);
        ret_status = E_ERR_EXCEPTION;
    }
    
    
    return ret_status;
}
/////////////////////////////////END OF FILE /////////////////////////////////////

int CLocalDBMgr::GetStudyFileNameFromStudyLOID(CString &studyloid, CString &Studyname)
{
int getstudy_status = E_NO_ERROR;
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);//To Load the WinSock DLL file into process space
        CString l_csstatus ;
        
        if(status != 0) {
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CKtReconLogHelper::WriteToErrorLog(l_csstatus, __LINE__);
            return E_SOCKET_ERR;
            // WSACleanup();
        }
        
        CDBSAVRNode *node =   NULL;
        CDBSAVR *leaf = NULL;
        
        node = new CDBSAVRNode();
        leaf = new CDBSAVRIS(KTRECON_STUDY_PATH);
        if(NULL == node || NULL == leaf){
            return E_MEM_ALLOC;
        }else{
            node->addLeaf(leaf);
        }
        int sz = 0;
        DBSAFactory factory;
        int ret = -1;
        CDBSATagErrorList *error_list = NULL;
        CDBSAStudy *out_node = NULL;  //Image level
        
        ret = factory.DBSA_LoadStudy(&out_node, &error_list, All, (LPCTSTR)studyloid, node);
        
        if(ret == MRC_DBSA_SUCCESS && out_node) {
            CDBSAVRLeaf* series_num_leaf = NULL;
            long tag = KTRECON_IMG_PATH;
            series_num_leaf = out_node->FindLeaf(tag);
            void *load_leaf_val = NULL;
            
            if(series_num_leaf) {
                
                sz = ((CDBSAVR*)series_num_leaf)->GetSize();
                load_leaf_val = (void*)malloc(sz);
                LPTSTR* l_value = (LPTSTR*)load_leaf_val;
                ((CDBSAVR*)series_num_leaf)->GetValue(l_value, sz);
                ///Run40042/Run40042.-1959:/1.3/7.1/8.0/9.0
                CString l_csTempStr(_T(""));
                l_csTempStr.Format(_T("%s"), *l_value);
//                f_csCompletepath.Format(_T("%s"), *l_value);//to get path till image level
               // int cnt = l_csTempStr.ReverseFind(_T('/'));
               // l_csTempStr = l_csTempStr.Left(cnt);
                //cnt = l_csTempStr.ReverseFind(_T('/'));
                //l_csTempStr = l_csTempStr.Left(cnt);
                //f_csStudyName = l_csTempStr;
                //cnt = l_csTempStr.ReverseFind(_T('/'));
                //f_csProtocolName = l_csTempStr.Left(cnt);
                free(load_leaf_val); 
                load_leaf_val = NULL;
            } else {
                CString l_csErrMsg(_T("0x00291052 node is not found."));
                CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
                getstudy_status = E_IMG_TAG_PROT_FAIL;
            }
            
            if(out_node) {
                DBSA_FreeStudy(out_node);
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
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        l_csStrFormatted += szCause;
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured in getProtocolNameFromDB");
        CKtReconLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__);
        getstudy_status = E_ERR_EXCEPTION;
    }
    
    return getstudy_status;
}
