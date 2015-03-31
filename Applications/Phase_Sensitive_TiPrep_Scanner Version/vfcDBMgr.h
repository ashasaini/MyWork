// vfcDBMgr.h: interface for the CvfcDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFCDBMGR_H__0F98F29B_792F_42A1_B42B_ECFE5325B4EB__INCLUDED_)
#define AFX_VFCDBMGR_H__0F98F29B_792F_42A1_B42B_ECFE5325B4EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <fstream>
#include<vector>
using namespace std;
#include <tami/libvf/varfields.h>       /* Varfield definitions         */
#include <tami/vfStudy/vf_appcodes.h>   /* Values for varfield defs     */
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vfstudy.h>       /* Study hierarchy defs         */
#include <tami/vfStudy/vf_apptypes.h>

#define IMG_PATH_LEVEL 4
class CvfcDBMgr
{
public:
    int GetProcSubVFPaths(VfPathElem_t vf_path[], CStringArray &imgpaths,VfDBHandle_t & f_vfd); 
	int UpdateLOIDForProcessNode(VfPathElem_t vf_path[], char * series_loid , 
        char ** imgLoids, int nImgs,VfDBHandle_t & f_vfd); 
	int ClearMappedDLOID(VfPathElem_t vf_path[], int depth,char * strloid,VfDBHandle_t & f_vfd);
    CvfcDBMgr();
    virtual ~CvfcDBMgr();
    CvfcDBMgr(CvfcDBMgr& f_vfcmgr);
    CvfcDBMgr& operator = (CvfcDBMgr& f_vfcmgr);
    int OpenDBFile(const CString &l_cpStudyfilename, VfDBHandle_t & f_vfd,bool moderdwrite = false);//vp..1-aug
    int CloseDBFile(VfDBHandle_t & f_vfd);//vp..1-aug
    int getGDCImageKind(VfPathElem_t f_pathP[], int &f_nGDCImgkind,VfDBHandle_t & f_vfd);//vp..1-aug
    int getGDCImageKind(const int f_PortIdx, const int f_procidx, int &f_nGDCImgkind,VfDBHandle_t & f_vfd); //AN
    int _getImageDataForm(VfPathElem_t f_pathP[], int &f_nImgDataForm,VfDBHandle_t & f_vfd);//vp..1-aug
    int getImageDataForm(const int f_PortIdx, const int f_procidx, 
        int &f_nImgDataForm, CString &f_csDataType,VfDBHandle_t & f_vfd); 
    //+Mpower/20130618/PN/added
    int GetGatingPeriod(VfPathElem_t f_img_path[], float &f_gating_period,VfDBHandle_t & f_vfd);
    int GetGatingPeriod(const int f_protocol_index, const int f_process_index, 
        float &f_gating_period,VfDBHandle_t & f_vfd);

    //-Mpower/20130618/PN/added
    int getImageDataType(const int f_protocol_index, const int f_process_index, 
        int &f_nImgDataType, CString &f_csDataType,VfDBHandle_t & f_vfd); 
    int GetSubNodes(VfPathElem_t f_pathP[], int f_curLevel,
                    int &f_ntotalsubnodes,VfDBHandle_t & f_vfd);
    int protTraverse(const int &f_PortIdx,
                     int f_nSingleButtonFlag,
                     imgProcessData_t *f_sImagData,
                     int *f_nDataFlag,VfDBHandle_t & f_vfd);
    int GetVfMapID(VfPathElem_t *f_ImgPath, CString &f_csImageLoid,VfDBHandle_t & f_vfd);
    int GetVfTimeStamp(int f_nProtIdx,int f_nProcidx,int f_nEchoIdx,
        int f_nImgidx, float &initTI,float &time_stamp,VfDBHandle_t & f_vfd);//sb
    int GetVfTI(int f_nProtIdx, float &initTI, VfDBHandle_t & f_vfd );

private:
    int GetNodeInfo(VfPathElem_t  *f_ImgPath,
                    int &f_nImgGDCType, int &l_nImgCnt, 
                    int &l_nNumNode,VfDBHandle_t & f_vfd);
    int GetVfDataKind(VfPathElem_t *f_ImgPath, int &l_nDataKind,VfDBHandle_t & f_vfd);
    int SetProtocolInfo(int  f_nData_Form,
                        int  f_nNumNode,
                        int  f_nImgsUpCnt,
                        int  l_nImgType,
                        imgProcessData_t *f_sImagData,
                        int *f_nDataFlag);
    int MemAllocImgData(int index,
                        imgProcessData_t *f_sImagData);
    int SetImagePath(int imgcnt,
                     int Data_Form,
                     int protIndex,
                     int proceIndex,
                     int echoIndex,
                     int imgIndex,
                     imgProcessData_t *f_sImagData,
                     int *f_nDataFlag);
private:
    //CString m_study_name;//vp..1-aug
    //VfDBHandle_t m_vfd;
    //VfPathElem_t    m_path[IMG_PATH_LEVEL];//vp..1-aug


};

#endif // !defined(AFX_VFCDBMGR_H__0F98F29B_792F_42A1_B42B_ECFE5325B4EB__INCLUDED_)
