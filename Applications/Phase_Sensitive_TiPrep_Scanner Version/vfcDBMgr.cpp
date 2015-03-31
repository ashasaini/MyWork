// vfcDBMgr.cpp: implementation of the CvfcDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vfcDBMgr.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "tami/libvf/vf_errors.h"
#include "CommonDefinitions.h"
//#include "TiPrepErrors.h"
#include "VfcNodeCopy.h" 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const CString EXCEPTION_VFCDBMGR = L"Exception occurred in VFC DB manager";

CvfcDBMgr::CvfcDBMgr()
{
}

CvfcDBMgr::~CvfcDBMgr()
{
//     if(NULL != m_vfd) {
//         VfClose(m_vfd);
//         m_vfd = NULL;
//     }
}

CvfcDBMgr::CvfcDBMgr(CvfcDBMgr& f_vfcmgr)
{
    TIPREP_TRACE(CvfcDBMgr::CvfcDBMgr,MRTraceMarker9);

    try {
        //+vp..1-aug
//        memcpy(m_path, f_vfcmgr.m_path, IMG_PATH_LEVEL * sizeof(VfPathElem_t));
//        m_study_name = f_vfcmgr.m_study_name;
//        memcpy(m_path, f_vfcmgr.m_path, IMG_PATH_LEVEL * sizeof(VfPathElem_t));
//         m_vfd = NULL;
// 
//         if(NULL != f_vfcmgr.m_vfd) {
//             m_vfd = f_vfcmgr.m_vfd;
        
//         }//-vp..1-aug

    } catch(CMemoryException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    } catch(CException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    } catch(...) {
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    }
}
CvfcDBMgr& CvfcDBMgr::operator = (CvfcDBMgr& f_vfcmgr)
{
    TIPREP_TRACE(CvfcDBMgr::operator=,MRTraceMarker9);

    if(this == &f_vfcmgr) {
        return(*this);
    }

    try {
        //+vp..1-aug
        //        m_study_name = f_vfcmgr.m_study_name;
        //        memcpy(m_path, f_vfcmgr.m_path, IMG_PATH_LEVEL * sizeof(VfPathElem_t));
        //        m_vfd = NULL;
        
        //         if(NULL != f_vfcmgr.m_vfd) {
        //             m_vfd = f_vfcmgr.m_vfd; // study file handle should be same for same study, as it cannot be opened multiple times simultaneously
           
//    }//-vp..1-aug  
    } catch(CMemoryException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(*this);
    } catch(CException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(*this);
    } catch(...) {
        CString l_cserrmsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(*this);
    }

    return(*this);
}
int CvfcDBMgr::OpenDBFile(const CString &f_sfileName,VfDBHandle_t & f_vfd,bool moderdwrite )
{
    TIPREP_TRACE(CvfcDBMgr::OpenDBFile,MRTraceMarker9);

    if(TRUE == f_sfileName.IsEmpty()) {
        CString l_csErrMsg = _T("Parameter f_sfileName empty in OpenDBFile()");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return(E_ERROR);
    }

    VfDBName_t l_name;
    int l_nstatus = 0;
    char* l_cpStudyfilename = NULL;
    CString l_csErrMsg ;

    try {
//        m_study_name = f_sfileName;//self rev //[7/9/2013 Shweta]start//vp..1-aug
        l_cpStudyfilename = new char[128];

        if(NULL == l_cpStudyfilename) {
            CString l_csErrMsg = _T("Memory can not be allocated because of error");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return(E_ERROR);
        }

        sprintf(l_cpStudyfilename, "%S", f_sfileName);
        l_name.filename = l_cpStudyfilename;
        if (!moderdwrite)
        {
            l_nstatus = ::VfOpen(VFFT_FILE, l_name, O_RDONLY , &f_vfd); //AN Modified
            
        } else {
            l_nstatus = ::VfOpen(VFFT_FILE, l_name, O_RDWR , &f_vfd); //AN Modified
        }
        //Mpower_65/Jun-2013/VP..memory_Revw_ensure corresponding VfClose
        if(E_NO_ERROR != l_nstatus) {
            
            if(NULL != l_cpStudyfilename) {
                delete []l_cpStudyfilename;
                l_cpStudyfilename = NULL;            
            }
            if (E_FILE_NOT_FOUND == l_nstatus)
            {
                CTiPrepLogHelper::WriteToErrorLog(_T("Study files not found for VFC operations"), __LINE__, __FILE__);
                return E_VFC_FILE_NOT_FOUND;
			} else {
				l_csErrMsg.Format(_T("vfopen error"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__, ERROR_LOGONLY);
			}
            return(E_VF_OPEN);
        }
        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;            
        }
    } catch(CMemoryException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        CloseDBFile(f_vfd);

        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;
            return(E_ERR_EXCEPTION);
        }
    } catch(CException& e) {
        UNREFERENCED_PARAMETER(e);
        CString l_csErrMsg = _T("Exception occured.");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        CloseDBFile(f_vfd);

        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;
            return(E_ERR_EXCEPTION);
        }
    } catch(...) {
        CString l_csErrMsg = _T("Memory can not be allocated because of error");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        CloseDBFile(f_vfd);

        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;
            return(E_ERR_EXCEPTION);
        }
    }

    return(E_NO_ERROR);
}

int CvfcDBMgr::CloseDBFile(VfDBHandle_t & f_vfd)
{
    TIPREP_TRACE(CvfcDBMgr::CloseDBFile,MRTraceMarker9);

    try {
        ::VfClose(f_vfd);
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while closing vfDB");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }

    return E_NO_ERROR;
}

int CvfcDBMgr::getGDCImageKind(VfPathElem_t f_pathP[], int &f_nGDCImgkind,VfDBHandle_t & f_vfd)
{
    TIPREP_TRACE(CvfcDBMgr::getGDCImageKind,MRTraceMarker9);
    int l_GDC_Img_kind = -1;
    VfFieldSpec_t   l_vfReq[1] = {0};

    try {
        int l_n = 0;
        VFF_SET_ARGS(l_vfReq[l_n], SVN_PRC_GDC_IMAGE_KIND,
                     SVT_PRC_GDC_IMAGE_KIND, sizeof(l_GDC_Img_kind), &l_GDC_Img_kind, 0) ;
        l_n++;
        int  l_numget = 0;

        if(E_NO_ERROR != VfGetFields(f_vfd, f_pathP, SVD_ST_PROCESSED,
                                     l_vfReq, l_n, &l_numget)) {
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            f_nGDCImgkind = -1;
            return E_Vf_GET_ERR;
            //[7/9/2013 Shweta]end
        } else {
            //dummy else
        }
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while getting value from vfc");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }

    f_nGDCImgkind = l_GDC_Img_kind;
    return(E_NO_ERROR);
}

int CvfcDBMgr::_getImageDataForm(VfPathElem_t f_pathP[], int &f_nImgDataForm,VfDBHandle_t & f_vfd)
{
    TIPREP_TRACE(CvfcDBMgr::getGDCImageKind,MRTraceMarker9);
    int l_nImgDataForm = -1;
    VfFieldSpec_t   l_vfReq[5];

    try {
        int l_n = 0;
        VFF_SET_ARGS(l_vfReq[l_n], SVN_GEN_DATA_FORM,
                     SVT_GEN_DATA_FORM, sizeof(l_nImgDataForm), &l_nImgDataForm, 0) ;
        l_n++;
        int  l_numget = 0;
        int l_nStatus =  VfGetFields(f_vfd, f_pathP, SVD_ST_PROCESSED,
                                     l_vfReq, l_n, &l_numget);
        if(E_NO_ERROR != l_nStatus) {
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            l_nImgDataForm = -1;
            f_nImgDataForm = -1;
            return E_Vf_GET_ERR;
            //[7/9/2013 Shweta]end
        } 
        if(l_n != l_numget){
            l_nImgDataForm = -1;
            f_nImgDataForm = -1;
            return E_Vf_GET_ERR;
        }
        f_nImgDataForm = l_nImgDataForm;
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while getting value from vfc");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);

    }

    f_nImgDataForm = l_nImgDataForm;
    return E_NO_ERROR;
}

//+Mpower/20130618/PN/added
//************************************Method Header**************************************

//Method Name:  GetGatingPeriod

//Author:  iGATE/Priyanka

//Purpose:

//***************************************************************************************

int CvfcDBMgr::GetGatingPeriod(VfPathElem_t f_img_path[], float &f_gating_period,VfDBHandle_t & f_vfd)
{
    try{
        TIPREP_TRACE(CvfcDBMgr::GetGatingPeriod,MRTraceMarker9);
        int l_no_of_varfield_read = 0;
        int l_no_of_varfield_read_back = 0;
        status_t    l_tag_reading_status = E_NO_ERROR;
        VfFieldSpec_t   l_vfreq[5];
        VftGatingParam_t l_vfgateParam;
        
        if(NULL == f_img_path) {
            CTiPrepLogHelper::WriteToErrorLog(_T("image path  NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_no_of_varfield_read = 0;
        l_vfreq[l_no_of_varfield_read].status = E_NO_ERROR;
        l_vfgateParam.gatingPeriod = 0.0;
        VFF_SET_ARGS(l_vfreq[l_no_of_varfield_read], SVN_PLN_GATE_PARAM, SVT_PLN_GATE_PARAM,
            sizeof(l_vfgateParam), &l_vfgateParam, 0);
        l_no_of_varfield_read++;
        
        if(NULL == f_vfd) {
            CTiPrepLogHelper::WriteToErrorLog(_T("vfc handle  NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if_no_error(l_tag_reading_status = VfGetFields(f_vfd, f_img_path, SVD_ST_PROTOCOL,
            l_vfreq, l_no_of_varfield_read, &l_no_of_varfield_read_back)) {
        }
        else {
            l_tag_reading_status = E_Vf_GET_ERR;
        }
        
        if_no_error(l_tag_reading_status) {
        }
        f_gating_period = l_vfgateParam.gatingPeriod;
        return(l_tag_reading_status);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

//-Mpower/20130618/PN/added
//Shweta
int CvfcDBMgr::getImageDataType(const int f_protocol_index, const int f_process_index, 
                                int &f_nImgDataType, CString &f_csDataType,VfDBHandle_t & f_vfd)
{
    TIPREP_TRACE(CvfcDBMgr::getGDCImageKind,MRTraceMarker9);
    int l_nImgDataType = -1;
    VfFieldSpec_t   l_vfReq[5];
    VfPathElem_t f_pathP[4] = {SVN_ST_PROTOCOL,   f_protocol_index,
                               SVN_ST_PROCESSED, f_process_index,
                               SVN_ST_ECHO, 0,
                               SVN_ST_POSITION,      0
                              };

    try {
        int l_n = 0;
        VFF_SET_ARGS(l_vfReq[l_n], SVN_GEN_DATA_TYPE,
                     SVT_GEN_DATA_TYPE, sizeof(l_nImgDataType), &l_nImgDataType, 0) ;
        l_n++;
        int  l_numget = 0;

        if(E_NO_ERROR != VfGetFields(f_vfd, f_pathP, SVD_ST_PROCESSED,
                                     l_vfReq, l_n, &l_numget)) {
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            l_nImgDataType = -1;
            f_nImgDataType = -1;
            return E_Vf_GET_ERR;
        } else {
            //dummy else
        }
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while getting value from vfc");
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);

    }

    f_nImgDataType = l_nImgDataType;
    return E_NO_ERROR;
}

int CvfcDBMgr::protTraverse(const int &f_PortIdx,
                            int f_nSingleButtonFlag,
                            imgProcessData_t *f_sImagData,
                            int *f_nDataFlag,VfDBHandle_t & f_vfd)
{
    try{
        TIPREP_TRACE(CvfcDBMgr::protTraverse,MRTraceMarker9);
        status_t l_nstatus = E_NO_ERROR;
        int l_nNumSlices = 0;
        int  l_nDataForm = 0;
        int  l_numNode = -1;
        int  l_nImgsUpCnt = 0;
        int  l_nImgType = 0;
        int l_nret = 0;
        int l_nImgCount = 0;
        int l_ndataKind = 1;
        VfPathElem_t l_pathP[4] = { SVN_ST_PROTOCOL,   f_PortIdx,
            SVN_ST_PROCESSED, 0,
            SVN_ST_ECHO, 0,
            SVN_ST_POSITION,      0
        };
        VfPathElem_t    l_ImgPath[4] = {
            {SVN_ST_PROTOCOL,   0},
            {SVN_ST_PROCESSED,  0},
            {SVN_ST_ECHO,       0},
            {SVN_ST_POSITION,   0}
        };
        int l_nTotProcessNodes = 0;
        int l_nTotEchoNodes = 0;
        int l_nProcessCnt, l_nEchoCnt , l_nImgCnt;
        l_nstatus = GetSubNodes(l_pathP, SVD_ST_PROTOCOL, l_nTotProcessNodes,f_vfd);
        
        if(E_NO_ERROR != l_nstatus || 0 == l_nTotProcessNodes) {
            return(E_Vf_GET_ERR);
        }
        
        for(l_nProcessCnt = 0; l_nProcessCnt < l_nTotProcessNodes; l_nProcessCnt++) {
            l_ImgPath[0].index = f_PortIdx;
            l_ImgPath[1].index = l_nProcessCnt;
            l_ImgPath[2].index = 0;
            l_ImgPath[3].index = 0;
            l_nstatus = _getImageDataForm(l_ImgPath, l_nDataForm,f_vfd);
            
            if(E_NO_ERROR != l_nstatus) {
                CString l_csErrMsg = _T("Error occured while getting data form value from vfc");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return l_nstatus;
            }
            
            l_nret = GetNodeInfo(l_ImgPath, l_nImgType, l_nImgsUpCnt, l_numNode,f_vfd);
            
            if(l_nret != E_NO_ERROR) {
                CString l_csErrMsg = _T("Error occured while getting Node info");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return(l_nret);
            }
            
            if(1 == f_nSingleButtonFlag) {
                l_nstatus = GetVfDataKind(l_ImgPath, l_ndataKind,f_vfd);
                
                if(E_NO_ERROR != l_nstatus) {
                    CString l_csErrMsg = _T("Error occured while getting Data kind");
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    return l_nstatus;
                }
                
                if(VFC_PROC_RECON != l_ndataKind) {
                    printf("data kind : %d for path : [%d.%d/%d.%d]\n",
                        l_ndataKind,
                        l_ImgPath[0].subtree_name,
                        l_ImgPath[0].index,
                        l_ImgPath[1].subtree_name,
                        l_ImgPath[1].index);
                    continue;
                }
            }
            
            //set protocol info i.e DataForm,ImgCount,ImagetType,Number of nodes
            l_nstatus = SetProtocolInfo(l_nDataForm, l_numNode,
                l_nImgsUpCnt,
                l_nImgType,
                f_sImagData, f_nDataFlag);
            
            if(E_NO_ERROR != l_nstatus) {
                CString l_csErrMsg = _T("Error occured while setting Protocol info");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return l_nstatus;
            }
            
            l_nstatus = GetSubNodes(l_ImgPath, SVD_ST_PROCESSED, l_nTotEchoNodes,f_vfd);//SB added
            
            if(E_NO_ERROR != l_nstatus) {
                CString l_csErrMsg = _T("Error occured while getting sub node(Echo node)");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_Vf_GET_ERR;
            }
            if(0 == l_nTotEchoNodes) {
                printf("totEchoNodes is 0\n");
                return(E_ERROR);
            }
            
            l_nImgCount = 0;
            
            for(l_nEchoCnt = 0; l_nEchoCnt < l_nTotEchoNodes; l_nEchoCnt++) {
                l_ImgPath[SVD_ST_ECHO -1].index = l_nEchoCnt;//SB added
                l_nstatus = GetSubNodes(l_ImgPath, SVD_ST_ECHO, l_nNumSlices,f_vfd);//SB added
                if(E_NO_ERROR != l_nstatus) {
                    CString l_csErrMsg = _T("Error occured while getting Sub node(Image node)");
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    return E_Vf_GET_ERR;
                }
                if(0 == l_nNumSlices) {
                    printf("numSlices is 0\n");
                    return(E_ERROR);
                }
                
                for(l_nImgCnt = 0; l_nImgCnt < l_nNumSlices; l_nImgCnt++) {
                    l_ImgPath[SVD_ST_POSITION -1].index = l_nImgCnt;//SB added
                    l_nstatus = SetImagePath(l_nImgCount,
                        l_nDataForm,
                        f_PortIdx,
                        l_nProcessCnt,
                        l_nEchoCnt,
                        l_nImgCnt,
                        f_sImagData,
                        f_nDataFlag);
                    if(E_NO_ERROR != l_nstatus) {
                        printf("SetImagePath returns error \n");
                        CString l_csErrMsg = _T("Error occured in SetImagePath");
                        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                        return l_nstatus;
                    }
                    
                    //set image path for further calculations.
                    l_nImgCount++;
                }
            }
    }
    
    return(E_NO_ERROR);
}catch(...) {
    CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
    return E_ERROR;
}
}

int CvfcDBMgr::GetSubNodes(VfPathElem_t f_pathP[],
                           int f_curLevel,
                           int &f_ntotalsubnodes,VfDBHandle_t & f_vfd)
{
    TIPREP_TRACE(CvfcDBMgr::GetSubNodes,MRTraceMarker9);
    int l_ntotalSubNodes = 256;
    BITFLD_DECL(vfBranches, 256);
    BITFLD_INIT(vfBranches, 256, 0);

    try {
//         if ((NULL == f_pathP) || (NULL == vfBranches)/* || (NULL == f_ntotalsubnodes*/)){
//             CString l_cserr (_T("GetSubNodes parameter NULL"));
//             CTiPrepLogHelper::WriteToErrorLog(l_cserr,__LINE__,__FILE__);
//             return(E_ERROR); // -1 indicates error
//         }
        int l_nactiveSubNodes = 0;
        status_t l_ret = E_NO_ERROR;
        l_ret = ::VfScanSubtree(f_vfd, f_pathP, f_curLevel,
                                f_pathP[f_curLevel].subtree_name, vfBranches, &l_ntotalSubNodes,
                                &l_nactiveSubNodes);

        if(E_NO_ERROR != l_ret) {
            CString l_cserrmsg = _T("VfScanSubtree Error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            f_ntotalsubnodes = -1;//[7/9/2013 Shweta]start error case
            return(l_ret) ;
        } else {
            //dummy else
        }
    } catch(...) {
        CString l_cserrmsg("EXCEPTION occurred.");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        f_ntotalsubnodes = -1;//[7/9/2013 Shweta]start error case
        return(E_ERR_EXCEPTION);
    }

    f_ntotalsubnodes = l_ntotalSubNodes;
    return(E_NO_ERROR);
}

int CvfcDBMgr::GetNodeInfo(VfPathElem_t  *f_ImgPath,
                           int &f_nImgGDCType, int &f_nImgCnt, 
                           int &f_nNumNode,VfDBHandle_t & f_vfd)
{
    try{
        int l_nImages_up = 0;
        //int l_nDataform  = -1;
        //int l_nNumImgs = 0;
        //int l_nMagnitudeDataNode = -1;
        int l_nTotal_branches = 0, l_nTotalact = 0;
        int /*l_nImgCnt = 0, l_nIdx = 0,*/ l_nCnt = 0, l_nNumget = 0;
        int32_t l_nKind = 0;
        VfFieldSpec_t   l_vfReq[8];
        status_t    l_nstatus = E_NO_ERROR;
        BITFLD_DECL(active_branch_mask, 256);
        
        if(NULL == f_ImgPath) {
            return E_ERROR;
        }
        
        l_nTotal_branches = 256;
        BITFLD_INIT(active_branch_mask, 256, 0);
        if_error(VfScanSubtree(f_vfd,
            f_ImgPath,
            SVD_ST_PROTOCOL,
            SVN_ST_PROCESSED,
            active_branch_mask,
            &l_nTotal_branches,
            &l_nTotalact)) {
            return(E_ERROR);
        }
        l_nCnt = 0;
        VFF_SET_ARGS(l_vfReq[l_nCnt], SVN_GEN_NUM_IMAGES_UP,
            SVT_GEN_NUM_IMAGES_UP, sizeof(int), &l_nImages_up, 0);
        l_nCnt++;
        l_nstatus = VfGetFields(f_vfd, f_ImgPath, SVD_ST_PROCESSED, l_vfReq, l_nCnt, &l_nNumget);
        
        if(E_NO_ERROR == l_nstatus) {
            if(l_vfReq[0].status == E_NO_ERROR) {
                f_nImgCnt = l_nImages_up;
            }else{
                CString l_csErrMsg = _T("Error occured in VfGetFields");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_Vf_GET_ERR;
            }
        }else{
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        
        l_nCnt = 0;
        VFF_SET_ARGS(l_vfReq[l_nCnt], SVN_PRC_GDC_IMAGE_KIND,
            SVT_PRC_GDC_IMAGE_KIND, sizeof(l_nKind), &l_nKind, 0) ;
        l_nCnt++;
        l_nstatus = VfGetFields(f_vfd, f_ImgPath, SVD_ST_PROCESSED, l_vfReq, l_nCnt, &l_nNumget);
        
        if(E_NO_ERROR != l_nstatus) {
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            f_nImgGDCType = -1;
            f_nNumNode = -1;
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        
        f_nImgGDCType = l_nKind;
        f_nNumNode = l_nTotalact;
        return(l_nstatus);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}
int CvfcDBMgr::GetVfDataKind(VfPathElem_t *f_ImgPath, int &f_nDataKind,VfDBHandle_t & f_vfd)
{
    try{
        int l_nCnt = 0;
        int l_nNumget = 0;
        status_t    l_nstatus = E_NO_ERROR;
        VfFieldSpec_t   l_vfreq[5];
        
        if(NULL == f_ImgPath) {
            return E_ERROR;
        }
        
        l_nCnt = 0;
        l_vfreq[l_nCnt].status = E_NO_ERROR;
        VFF_SET_ARGS(l_vfreq[l_nCnt], SVN_PRC_DATA_KIND, SVT_PRC_DATA_KIND,
            sizeof(f_nDataKind), &f_nDataKind, 0);
        l_nCnt++;
        if_no_error(l_nstatus = VfGetFields(f_vfd, f_ImgPath, SVD_ST_PROCESSED,
            l_vfreq, l_nCnt, &l_nNumget)) {
        }
        else {
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            l_nstatus = E_ERROR;
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        
        if_no_error(l_nstatus) {
            //print
        }
        return(l_nstatus);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}
int CvfcDBMgr::GetVfMapID(VfPathElem_t *f_ImgPath, 
                          CString &f_csImageLoid,VfDBHandle_t & f_vfd)//SB added
{
    try{
        VfFieldSpec_t       l_fieldspec[2];
        status_t            l_nstatus = 0;
        int                 l_nCnt = 0;
        int                 l_nNum_done;
        int                 l_nSize;
        VFF_SET_ARGS(l_fieldspec[l_nCnt], SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID,
            0, NULL, VFO_VARIABLE_LEN);
        l_nCnt++;
        l_nstatus = VfGetFields(f_vfd, f_ImgPath, SVD_ST_POSITION,
            l_fieldspec, l_nCnt, (int*)&l_nNum_done);//SB added
        if_error(l_nstatus) {
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        else if(l_nNum_done != l_nCnt) {
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        
        l_nSize = l_fieldspec[0].size ;
        l_nCnt = 0;
        char *l_cTempBuff = NULL;
        //l_cTempBuff = (char*)malloc(sizeof(char) * l_nSize);//use new;
        l_cTempBuff = new char[l_nSize];
        memset(l_cTempBuff, 0, sizeof(char)*l_nSize);
        VFF_SET_ARGS(l_fieldspec[l_nCnt], SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID,
            l_nSize, l_cTempBuff, VFO_VARIABLE_LEN);
        l_nCnt++;
        l_nstatus = VfGetFields(f_vfd, f_ImgPath, SVD_ST_POSITION,
            l_fieldspec, l_nCnt, (int*)&l_nNum_done);//SB added
        if_error(l_nstatus) {
            l_nstatus = E_ERROR;
            //[7/9/2013 Shweta]start
            //returned error if VfGetFields fails
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_Vf_GET_ERR;
        }
        else if(l_nNum_done != l_nCnt) {
            CString l_csErrMsg = _T("Error occured in VfGetFields");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return E_Vf_GET_ERR;
        } else {
            f_csImageLoid = l_cTempBuff;
        }
        
        if(NULL != l_cTempBuff) {
            //Mpower_65/Jun-2013/VP..memory_Revw..use delete
            delete [] l_cTempBuff;//[7/9/2013 Shweta]start revw fix
            l_cTempBuff = NULL;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CvfcDBMgr::SetProtocolInfo(int  f_nData_Form,
                               int  f_nNumNode,
                               int  f_nImgsUpCnt,
                               int  f_nImgType,
                               imgProcessData_t *f_sImagData,
                               int *f_nDataFlag)
{
    try{
        int l_nStatus = E_NO_ERROR;
        TIPREP_TRACE(CvfcDBMgr::SetProtocolInfo,MRTraceMarker9);
        
        switch(f_nData_Form) {
        case VFC_FORM_REAL_ONLY:
            
            if(f_nDataFlag[0] == 0) {
                f_sImagData[0].dataform = f_nData_Form;
                f_sImagData[0].nImages = f_nImgsUpCnt;
                f_sImagData[0].imgGDCtype = f_nImgType;
                f_sImagData[0].numNode = f_nNumNode;
                l_nStatus = MemAllocImgData(0, f_sImagData);
            }
            
            break;
        case VFC_FORM_MAGNITUDE:
            
            if(f_nDataFlag[1] == 0) {
                f_sImagData[1].dataform = f_nData_Form;
                f_sImagData[1].nImages = f_nImgsUpCnt;
                f_sImagData[1].imgGDCtype = f_nImgType;
                f_sImagData[1].numNode = f_nNumNode;
                l_nStatus = MemAllocImgData(1, f_sImagData);
            }
            
            break;
        case VFC_FORM_IMGN_ONLY:
            
            if(f_nDataFlag[2] == 0) {
                f_sImagData[2].dataform = f_nData_Form;
                f_sImagData[2].nImages = f_nImgsUpCnt;
                f_sImagData[2].imgGDCtype = f_nImgType;
                f_sImagData[2].numNode = f_nNumNode;
                l_nStatus = MemAllocImgData(2, f_sImagData);
            }
            
            break;
        default:
            break;
        }
        
        return l_nStatus;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CvfcDBMgr::MemAllocImgData(int l_nIndex, imgProcessData_t *f_sImagData)
{
    try{
        if(0 > l_nIndex || 4 < l_nIndex)    {
            return E_ERROR;
        }
        
        if(NULL != f_sImagData[l_nIndex].convPolImgs) {
            free(f_sImagData[l_nIndex].convPolImgs);
            f_sImagData[l_nIndex].convPolImgs = NULL;
        }
        
        if(0 < f_sImagData[l_nIndex].nImages) {
            f_sImagData[l_nIndex].convPolImgs = (ImagNode_t *) malloc(sizeof(ImagNode_t) * f_sImagData[l_nIndex].nImages);
            if(NULL == f_sImagData[l_nIndex].convPolImgs) {
                CString l_csErrMsg = _T("memory allocation error \n");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                printf("memory allocation error \n");
                return E_MEM_ALLOC;
            }
            memset(f_sImagData[l_nIndex].convPolImgs, 0, sizeof(ImagNode_t) * f_sImagData[l_nIndex].nImages);//SB added
        } else {
            CString l_csErrMsg = _T("number of images is invalid. memory not allocated \n");
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            printf("number of images is invalid. memory not allocated \n");
            return E_ERROR;
        }
        
        return (E_NO_ERROR);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}
int CvfcDBMgr::SetImagePath(int imgcnt,
                            int Data_Form,
                            int protIndex,
                            int proceIndex,
                            int echoIndex,
                            int imgIndex,
                            imgProcessData_t *f_sImagData,
                            int *f_nDataFlag)
{
    try{
        //int i = 0;
        
        switch(Data_Form) {
        case VFC_FORM_REAL_ONLY:
            
            if(NULL == f_sImagData[0].convPolImgs) {
                CString l_csErrMsg = _T("f_sImagData[0] is NULL \n");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                printf("f_sImagData[0] is NULL \n");
                return E_ERROR_NULL_FUNCPARAMS;
            }
            
            if(f_nDataFlag[0] == 0) {
                //SB modified
                f_sImagData[0].convPolImgs[imgcnt].protocol[0] = SVN_ST_PROTOCOL;
                f_sImagData[0].convPolImgs[imgcnt].protocol[1] = protIndex;
                f_sImagData[0].convPolImgs[imgcnt].processed[0] = SVN_ST_PROCESSED;
                f_sImagData[0].convPolImgs[imgcnt].processed[1] = proceIndex;
                f_sImagData[0].convPolImgs[imgcnt].echo[0] = SVN_ST_ECHO;
                f_sImagData[0].convPolImgs[imgcnt].echo[1] = echoIndex;
                f_sImagData[0].convPolImgs[imgcnt].image[1] = SVN_ST_POSITION;
                f_sImagData[0].convPolImgs[imgcnt].image[1] = imgIndex;
                
                if(imgcnt ==  f_sImagData[0].nImages - 1) {
                    f_nDataFlag[0] = 1;
                }
            }
            
            break;
        case VFC_FORM_MAGNITUDE:
            
            if(NULL ==    f_sImagData[1].convPolImgs) {
                CString l_csErrMsg = _T("f_sImagData[0] is NULL \n");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                printf("imgData[1] is NULL \n");
                return E_ERROR_NULL_FUNCPARAMS;
            }
            
            if(f_nDataFlag[1] == 0) {
                //SB added
                f_sImagData[1].convPolImgs[imgcnt].protocol[0] = SVN_ST_PROTOCOL;
                f_sImagData[1].convPolImgs[imgcnt].protocol[1] = protIndex;
                f_sImagData[1].convPolImgs[imgcnt].processed[0] = SVN_ST_PROCESSED;
                f_sImagData[1].convPolImgs[imgcnt].processed[1] = proceIndex;
                f_sImagData[1].convPolImgs[imgcnt].echo[0] = SVN_ST_ECHO;
                f_sImagData[1].convPolImgs[imgcnt].echo[1] = echoIndex;
                f_sImagData[1].convPolImgs[imgcnt].image[1] = SVN_ST_POSITION;
                f_sImagData[1].convPolImgs[imgcnt].image[1] = imgIndex;
                
                //                 f_sImagData[1].convPolImgs[imgcnt].protocol[1] = protIndex;
                //                 f_sImagData[1].convPolImgs[imgcnt].processed[1] = proceIndex;
                //                 f_sImagData[1].convPolImgs[imgcnt].echo[1] = echoIndex;
                //                 f_sImagData[1].convPolImgs[imgcnt].image[1] = imgIndex;
                
                if(imgcnt ==  f_sImagData[1].nImages - 1) {
                    f_nDataFlag[1] = 1;
                }
            }
            
            break;
        case VFC_FORM_IMGN_ONLY:
            
            if(NULL ==    f_sImagData[2].convPolImgs) {
                CString l_csErrMsg = _T("f_sImagData[0] is NULL \n");
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                printf("imgData[2] is NULL \n");
                return E_ERROR_NULL_FUNCPARAMS;
            }
            
            if(f_nDataFlag[2] == 0) {
                //SB added
                f_sImagData[2].convPolImgs[imgcnt].protocol[0] = SVN_ST_PROTOCOL;
                f_sImagData[2].convPolImgs[imgcnt].protocol[1] = protIndex;
                f_sImagData[2].convPolImgs[imgcnt].processed[0] = SVN_ST_PROCESSED;
                f_sImagData[2].convPolImgs[imgcnt].processed[1] = proceIndex;
                f_sImagData[2].convPolImgs[imgcnt].echo[0] = SVN_ST_ECHO;
                f_sImagData[2].convPolImgs[imgcnt].echo[1] = echoIndex;
                f_sImagData[2].convPolImgs[imgcnt].image[1] = SVN_ST_POSITION;
                f_sImagData[2].convPolImgs[imgcnt].image[1] = imgIndex;
                
                //                 f_sImagData[2].convPolImgs[imgcnt].protocol[1] = protIndex;
                //                 f_sImagData[2].convPolImgs[imgcnt].processed[1] = proceIndex;
                //                 f_sImagData[2].convPolImgs[imgcnt].echo[1] = echoIndex;
                //                 f_sImagData[2].convPolImgs[imgcnt].image[1] = imgIndex;
                
                if(imgcnt ==  f_sImagData[2].nImages - 1) {
                    f_nDataFlag[2] = 1;
                }
            }
            
            break;
        default:
            break;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CvfcDBMgr::getGDCImageKind(const int f_PortIdx, const int f_procidx, 
                               int &f_nGDCImgkind,VfDBHandle_t & f_vfd)
{
    VfPathElem_t l_pathP[4] = { SVN_ST_PROTOCOL,   f_PortIdx,
                                SVN_ST_PROCESSED, f_procidx,
                                SVN_ST_ECHO, 0,
                                SVN_ST_POSITION,      0
                              };
    return getGDCImageKind(l_pathP, f_nGDCImgkind,f_vfd);
}

/*************************************************************
* Method:    	getImageDataForm
* Description: 	This function is used 
* Parameter: 	const int f_PortIdx :
* Parameter: 	const int f_procidx :
* Parameter: 	int & f_nImgDataForm :
* Parameter: 	CString & f_csDataType :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::getImageDataForm(const int f_PortIdx, const int f_procidx, int &f_nImgDataForm, 
                                CString &f_csDataType,VfDBHandle_t & f_vfd)
{
    try{
        VfPathElem_t l_pathP[4] = { SVN_ST_PROTOCOL,   f_PortIdx,
            SVN_ST_PROCESSED, f_procidx,
            SVN_ST_ECHO, 0,
            SVN_ST_POSITION,      0
        };
        int l_nStatus = _getImageDataForm(l_pathP, f_nImgDataForm,f_vfd);
        
        if(E_NO_ERROR != l_nStatus) {
            return l_nStatus;
        }
        
        switch(f_nImgDataForm) {
        case VFC_FORM_MAGNITUDE:
            f_csDataType.Format(_T("Magnitude"));
            break;
        case VFC_FORM_PHASE:
            f_csDataType.Format(_T("Phase"));
            break;
        case VFC_FORM_REAL_ONLY:
            f_csDataType.Format(_T("Real"));
            break;
        case VFC_FORM_IMGN_ONLY:
            f_csDataType.Format(_T("Imaginary"));
            break;
        case VFC_FORM_COMPLEX:
            f_csDataType.Format(_T("Complex"));
            break;
        case VFC_FORM_POLAR:
            f_csDataType.Format(_T("Polar"));
            break;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetGatingPeriod
* Description: 	This function is used 
* Parameter: 	const int f_protocol_index :
* Parameter: 	const int f_process_index :
* Parameter: 	float & f_gating_period :
* Returns:   	int
*************************************************************/
int CvfcDBMgr::GetGatingPeriod(const int f_protocol_index, const int f_process_index, 
                               float &f_gating_period,VfDBHandle_t & f_vfd)
{
    VfPathElem_t l_image_path[4] = { SVN_ST_PROTOCOL,   f_protocol_index,
                                     SVN_ST_PROCESSED, f_process_index,
                                     SVN_ST_ECHO, 0,
                                     SVN_ST_POSITION,      0
                                   };
    return GetGatingPeriod(l_image_path, f_gating_period,f_vfd);
}

int CvfcDBMgr::ClearMappedDLOID(VfPathElem_t vf_path[], int depth,
                                char * strloid,VfDBHandle_t & f_vfd)
{
    try{
        TIPREP_TRACE(CvfcDBMgr::ClearMappedDLOID,MRTraceMarker9);
        VfFieldSpec_t		l_fieldspec[2];
        //status_t			l_nstatus = 0;
        //int					l_nCnt = 0;	
        //    int			        l_nNum_done;
        //    int					l_nSize;
        //char *buffer = NULL;
        if (strloid == NULL)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("ClearMappedDLOID() function paramters NULL"),__LINE__,__FILE__);
            return E_ERROR;
        }
        int sz = strlen(strloid);
        strloid[sz + 1] = (char)'/0';
        // written argument set.
        VFF_SET_ARGS( l_fieldspec[0], SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID,
            strlen(strloid) + 1  , strloid, VFO_ALLOCATE | VFO_REPLACE);
        
        int nm = 0;
        time_t tm;
        int  status =  VfPutFields( f_vfd, vf_path, depth, 
            l_fieldspec, 1, &nm, &tm);
        if_error( status )
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("VfPutFields() failed"),__LINE__,__FILE__);
            return E_VF_UPDATE_FAIL; 
        }
        
        if(( nm != 1 ) || ( l_fieldspec[0].status != E_NO_ERROR ))
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("VfPutFields() failed"),__LINE__,__FILE__);
            return E_VF_UPDATE_FAIL; 
        }
        
        return E_NO_ERROR;
        
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CvfcDBMgr::UpdateLOIDForProcessNode(VfPathElem_t vf_path[], char * series_loid , 
                                        char ** imgLoids, int nImgs,VfDBHandle_t & f_vfd)
{
    try{
        TIPREP_TRACE(CvfcDBMgr::UpdateLOIDForProcessNode,MRTraceMarker9);
        int i = 0;
        if (NULL == series_loid || NULL == imgLoids )
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("UpdateLOIDForProcessNode() function NULL parameters"),__LINE__,__FILE__);
            return (E_ERROR);
        }
        int l_nstatus = 0;
        l_nstatus = ClearMappedDLOID(vf_path,SVD_ST_PROCESSED,series_loid,f_vfd);
        if (l_nstatus != E_NO_ERROR)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("ClearMappedDLOID() return FAIL"),__LINE__,__FILE__);
            
            return l_nstatus;
        }
        int l_nTotProcessNodes = 0;
        int l_nTotEchoNodes = 0;
        int l_nProcessCnt= 0 ,l_nImgCnt = 0;
        
        l_nstatus = GetSubNodes(vf_path, SVD_ST_PROCESSED,l_nTotProcessNodes,f_vfd);
        
        if(E_NO_ERROR != l_nstatus || 0 == l_nTotProcessNodes) {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetSubNodes() return FAIL"),__LINE__,__FILE__);
            
            return(E_Vf_GET_ERR); 
        }
        for (l_nProcessCnt = 0;l_nProcessCnt < l_nTotProcessNodes;l_nProcessCnt++){ //echo nodes
            vf_path[2].index = l_nProcessCnt;
            vf_path[3].index = 0;
            l_nstatus = GetSubNodes(vf_path, SVD_ST_ECHO,l_nTotEchoNodes,f_vfd);
            if(E_NO_ERROR != l_nstatus || 0 == l_nTotEchoNodes) {
                CTiPrepLogHelper::WriteToErrorLog(_T("GetSubNodes() return FAIL"),__LINE__,__FILE__);
                
                return(E_Vf_GET_ERR); 
            }
            for (l_nImgCnt = 0;l_nImgCnt < l_nTotEchoNodes;l_nImgCnt++){ //position nodes
                
                vf_path[3].index = l_nImgCnt;
                l_nstatus = ClearMappedDLOID(vf_path,SVD_ST_POSITION,imgLoids[i],f_vfd);
                if (l_nstatus != E_NO_ERROR)
                {
                    CTiPrepLogHelper::WriteToErrorLog(_T("ClearMappedDLOID() return FAIL"),__LINE__,__FILE__);
                    
                    return l_nstatus;
                }
                i++;
                
            }
        }
        return E_NO_ERROR;
        
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CvfcDBMgr::GetProcSubVFPaths(VfPathElem_t vf_path[], 
                                 CStringArray &imgpaths,
                                 VfDBHandle_t & f_vfd)
{
    try{
        TIPREP_TRACE(CvfcDBMgr::GetProcSubVFPaths,MRTraceMarker9);
        
        //int i = 0;
        int l_nTotProcessNodes = 0;
        int l_nTotEchoNodes = 0;
        int l_nProcessCnt = 0, l_nImgCnt = 0;
        CString vfpath (_T(""));
        imgpaths.RemoveAll();
        int l_nstatus = GetSubNodes(vf_path, SVD_ST_PROCESSED,l_nTotProcessNodes,f_vfd);
        
        if(E_NO_ERROR != l_nstatus || 0 == l_nTotProcessNodes) {
            CString l_csstatus ;
            l_csstatus.Format(_T("GetSubNodes() return status : %d"),__LINE__,__FILE__);
            
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus,__LINE__,__FILE__);
            return(E_Vf_GET_ERR); 
        }
        for (l_nProcessCnt = 0;l_nProcessCnt < l_nTotProcessNodes;l_nProcessCnt++){ //echo nodes
            //  vf_path[0].index = f_PortIdx;
            // vf_path[1].index = l_nProcessCnt;
            vf_path[2].index = l_nProcessCnt;
            vf_path[3].index = 0;
            l_nstatus = GetSubNodes(vf_path, SVD_ST_ECHO,l_nTotEchoNodes,f_vfd);
            // imgpaths.RemoveAll();
            for (l_nImgCnt = 0;l_nImgCnt < l_nTotEchoNodes;l_nImgCnt++){ //position nodes
                
                vf_path[3].index = l_nImgCnt;
                vfpath.Format(_T("1.%d/7.%d/8.%d/9.%d"),vf_path[0].index,vf_path[1].index,vf_path[2].index,vf_path[3].index);
                imgpaths.Add(vfpath);
            }
        }
        return E_NO_ERROR;
        
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VFCDBMGR, __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
//sb
int CvfcDBMgr::GetVfTimeStamp(int f_nProtIdx,int f_nProcidx,
                              int f_nEchoIdx,int f_nImgidx, 
                              float &initTI, float &time_stamp,
                              VfDBHandle_t & f_vfd )
{
    int         i = 0, j = 0, numget = 0;
    char        *vfnames[1];
    status_t        status = E_NO_ERROR;
    VfFieldSpec_t   vfreq[5];
    VfPathElem_t l_pathP[4] = { SVN_ST_PROTOCOL,   f_nProtIdx,
        SVN_ST_PROCESSED, f_nProcidx,
        SVN_ST_ECHO, f_nEchoIdx,
        SVN_ST_POSITION,f_nImgidx
        };
    TIPREP_TRACE(CvfcDBMgr::GetVfTimeStamp,MRTraceMarker9);
    i = 0;
    vfnames[i] = "SVN_PLN_TI";      /*  time stamp of the image */
    vfreq[i].status = E_NO_ERROR;
    VFF_SET_ARGS(vfreq[i], SVN_PLN_TI, SVT_PLN_TI,
        sizeof(time_stamp), &time_stamp, 0);
    i++;
    if_no_error(status = VfGetFields(f_vfd, l_pathP, SVD_ST_POSITION,
        vfreq, i, &numget)) {
        for(j = 0; j < i; j++) {
            if((VF_ERROR(vfreq[j].status))) {
                CString l_csTemp;
                l_csTemp.Format((" %s  0x%08x\n", vfnames[j], vfreq[j].status));
                CTiPrepLogHelper::WriteToErrorLog (l_csTemp, __LINE__, __FILE__);
                status = E_ERROR;
            }
        }
    }
    else {
        CString l_csTemp;
        l_csTemp.Format(("VfGetFields return error, status : 0x%08x\n", status));
        CTiPrepLogHelper::WriteToErrorLog (l_csTemp, __LINE__, __FILE__);
        status = E_ERROR;
    }
    
    if(E_ERROR == status) {
        i = 0;
        vfnames[i] = "SVN_PLN_TI";      /*  time stamp of the image */
        VFF_SET_ARGS(vfreq[i], SVN_PLN_TI, SVT_PLN_TI, sizeof(initTI), &initTI, 0);
        i++;
        status = VfGetFields(f_vfd, l_pathP, SVD_ST_PROTOCOL, vfreq, i, &numget);        
        if(E_NO_ERROR != status) {
            initTI = 0;
            CString l_csTemp;
            l_csTemp.Format(("VfGetFields is error, status = %d \n", status));
            CTiPrepLogHelper::WriteToErrorLog (l_csTemp, __LINE__, __FILE__);// set init ti value =0, when ti value is not found
        }        
        CString l_csTemp;
        l_csTemp.Format(_T("Failed to get TI from image level generate time stamp using TI present at protocol level"));
        CTiPrepLogHelper::WriteToErrorLog (l_csTemp, __LINE__, __FILE__);
        return (E_ERROR); //then only timestamp can be generated and graph plotted proeprly// bug fix
    }    
    return(status);
}
int CvfcDBMgr::GetVfTI(int f_nProtIdx, 
                              float &initTI,
                              VfDBHandle_t & f_vfd )
{
    int         i = 0, j = 0, numget = 0;
    char        *vfnames[1];
    status_t        status = E_NO_ERROR;
    VfFieldSpec_t   vfreq[2];
    VfPathElem_t l_pathP[1] = { SVN_ST_PROTOCOL,   f_nProtIdx };

    TIPREP_TRACE(CvfcDBMgr::GetVfTI,MRTraceMarker9);
    
    i = 0;
    vfnames[i] = "SVN_PLN_TI";      /*  time stamp of the image */
    VFF_SET_ARGS(vfreq[i], SVN_PLN_TI, SVT_PLN_TI, sizeof(initTI), &initTI, 0);
    i++;
    status = VfGetFields(f_vfd, l_pathP, SVD_ST_PROTOCOL, vfreq, i, &numget);        
    if(E_NO_ERROR != status) {
        initTI = 0.0f;
        CString l_csTemp;
        l_csTemp.Format(("VfGetFields is error, status = %d \n", status));
        CTiPrepLogHelper::WriteToErrorLog (l_csTemp, __LINE__, __FILE__);// set init ti value =0, when ti value is not found
    }        
    return(status);
}