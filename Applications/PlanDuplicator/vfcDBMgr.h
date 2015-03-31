// CreateFileName.h: interface for the CCreateFileName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEFILENAME_H__A21FFA30_B781_4D5B_BF44_4D40CC72B1C4__INCLUDED_)
#define AFX_CREATEFILENAME_H__A21FFA30_B781_4D5B_BF44_4D40CC72B1C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <fstream.h>
#include <fstream>
#include<vector>
using namespace std;
#include "DBMgr.h"
#include <tami/libvf/varfields.h>       /* Varfield definitions         */
#include <tami/vfStudy/vf_appcodes.h>   /* Values for varfield defs     */
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vfstudy.h>       /* Study hierarchy defs         */
#include <tami/vfStudy/vf_apptypes.h>
#include "dataTypes.h"
#define MAX_VF_BRANCHES 256

#define SCANNED_PROTOCOL    1
#define UNSCANNED_PROTOCOL  0
//+Patni-VP/2012Aug06/Review/i did not study in detail but may be these structure variables can be merged into a single class 'processinfo'
//avoid structures if possible
typedef struct {
    int index;
    int totalImages;
} EchoInfo , *pEchoInfo;

typedef struct {
    bool isGDCKind;
    int totalEchoNodes;
    EchoInfo *echoInfo;
} ProcessInfo , *pProcessInfo;

typedef struct {
    
    int totalProcessNodes;
    ProcessInfo *processInfo;
} TotalProcessInfo , *pTotalProcessInfo;


class CvfcDBMgr  : public CDBMgr
{
public:
    CvfcDBMgr();
    virtual ~CvfcDBMgr();
    CvfcDBMgr(CvfcDBMgr& f_vfcmgr);
    CvfcDBMgr& operator = (CvfcDBMgr& f_vfcmgr);
    int OpenDBFile(const CString &l_cpStudyfilename);
    int protTraverse(const char *f_studyName,const int& f_port_index,vector<CString> &f_csFileName,int &f_ntotalimg);
    
    int CheckIsProtocolScanned(int f_nprotIndx,int &f_prot_type);  //Mpower 030/2012-06-26/AS/Modified/cpp change
    int getProtAcqOrder(int *f_pnarray) ;
    
    //CString GetStudyName(void);
    
 //   pTotalProcessInfo GetTotalProcessNodeInfo() {return m_procInfo;}
    int CloseDBFile(void);
    
    //VOI calculations
    int GetSliceThickness(float *f_fslice_thickness,const int &f_nprotindex);
    int GetSliceGap(float *f_fslice_gap,const int &f_nprotindex);
    int GetSliceNum(int *f_nslice_num, const int &f_nprotindex);
    //int GetPLNHold(int *f_nplnhold, const int &f_nprotindex);
    int GetFOV(f32vec2_t *f_fov,const int &f_nprotindex);
    int TraverseProt(const char * f_scartchpad_name);
    int GetScannedCenterSliceNumber(const int &f_nprotindex,int &f_nProcIdx);
    
private:
    int GetSubNodes(VfDBHandle_t srcVf, VfPathElem_t pathP[],int curLevel,int *f_ntotalsubnodes);//Mpower 040/2012-08-09/AS/Modified/made private
    void SetStudyName(const CString& f_study_name);             //Mpower 040/2012-08-09/AS/Modified/made private
    status_t GetLOID(int f_node_depth, CString &f_study_loid);  //Mpower 040/2012-08-09/AS/Modified/made private
    void InitVfPath(void);                                       //Mpower 040/2012-08-09/AS/Modified/made private
    int protTraverseGDC(const int &f_ScannedPortIdx,int &f_processIdx);         //Mpower 040/2012-08-09/AS/Modified/Function made private
    int32_t Get_GDC_Image_Kind(VfDBHandle_t f_vfId,VfPathElem_t f_pathP[]);
    pTotalProcessInfo m_procInfo;
    CString m_study_name;
    VfDBHandle_t m_vfd;
    VfPathElem_t    m_path[IMG_PATH_LEVEL];
};

#endif // !defined(AFX_CREATEFILENAME_H__A21FFA30_B781_4D5B_BF44_4D40CC72B1C4__INCLUDED_)
