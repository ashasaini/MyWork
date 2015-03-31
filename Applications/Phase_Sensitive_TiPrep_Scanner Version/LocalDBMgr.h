// LocalDBMgr.h: CLocalDBMgr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_LOCALDBMGR_H__249EA752_67C9_4860_B86F_6C739EBEB48C__INCLUDED_)
#define AFX_LOCALDBMGR_H__249EA752_67C9_4860_B86F_6C739EBEB48C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <LibDS/GeneralDefinations.h>
#define TIPREP_IMG_TAG 0x7FE00010
#define TIPREP_IMG_SCALE 0xE4011008
#define TIPREP_IMG_INVERSION_TIME 0x00180082
#define TIPREP_SERIES_PATH 0x00291051
#define TIPREP_IMG_PATH 0x00291052
#define TIPREP_PROTOCOL_PATH 0x00291054
#define TIPREP_IMG_DATA_TYPE 0x00191293
#define TIPREP_SERIES_ACQ_CONDITION 0x0019108C //KJ 2013
#define TIPREP_SERIES_ACQ_STATUS 0x001910CF//KJ 2014
#define TIPREP_SERIES_NUMBER 0x00200011 //KJ 2013
#define TIPREP_IMAGE_HEIGHT  0x00280010
#define TIPREP_IMAGE_WIDTH   0x00280011
#define TIPREP_IMAGE_BITS    0x00280100
#define TIPREP_GATING_PERIOD 0x001911FB
#define TIPREP_MAX_IMG_PIXEL_VALUE 0x00280107 //Mpower/20130722/PN/added//OFFSET CHECK
#define TIPREP_PLN_TEMPLATE_SEQUENCE 0x00181030//0x001910CA 
#define TIPREP_STUDY_PATH 0x00291050
#define MAX_BUFFER 256
#define NUM_10 10
#define EMPTY_STRING _T("")

/**
    This class is used to access MPluse data base.
*/

#include <DBSALib/DBSALib.h>
#include <DBSALib/DBSAVR.h>
#include <DBServer\MpDbsBaseObjectInfo.h>
#include <DBServer\MpDbsSeriesInfo.h>
using namespace MRMPlus::DBSALib;
using namespace MRMPlus::DBSAVR;
using namespace MPlus::DBServer;
using namespace std;

class CLocalDBMgr
{
//construction,copy & destruction
public:
	int UpdateVFPathInDB(CString & csLOID, LPCTSTR  vfpath,LOID_TYPE type);
    CLocalDBMgr();
    virtual ~CLocalDBMgr();

// Implementation
public:
	int GetStudyNameFromStudyLoid(CString &loid,CString &studyname); //AN scanner
    int TagReadingForHeightWidthImage(CString &f_image_loid,                                      
        unsigned short *f_image_width,
        unsigned short *f_image_height);
    int GetChildNodeLOIDs(CString & f_csloid, /*vector<CString> & f_csOutloid,*/ 
        int &f_nnumNodes, LOID_TYPE db_level = LOID_TYPE_SERIES); 
    int getImgDataFromDB(CString &f_ImageLoid,
                         void **f_ImgData,
                         int *size,
                         float *f_fScale,
                         int *f_nTime_stamp,
                         unsigned short *f_nlData_Type,int &f_nImgNo); //sb
    int GetProtocolNameFromImageLevelDB(CString &f_ImageLoid,
                                        CString &f_csProtocolName,CString &f_csStudyName,CString &f_csCompletepath);
    int GetProtocolNameFromSeriesLevelDB(CString &f_csSeriesLoid, 
        CString &f_csProtocolName, CString &f_csProcessName);

    int GetProtocolIndexFromSeriesLevelDB(CString &f_csSeriesLoid, 
        int &f_nProtcolIndex);

    int CreateSeriesCopy(/*in*/vector<CString> &f_inputImgLOIDs,
                               /*out*/vector<CString> &f_outImgLOIDs, CString & f_csseries);

    int UpdateImageBuffer(CString &f_csimgLoid, void * imgBuff, unsigned int imgSz);
    int GetImgBuffer(CString &imgLOID, void ** imgBuff, unsigned int *imgSz);
    int GetParentLOID(LPCTSTR child_node_loid,  LOID_TYPE child_node_loid_type, CString &parent);
    int UpdateAcqOrder(CString & SeriesLoid);
    int GetNumImagesInSeries(CString &series_loid, int &f_nnumImages);
    int getProtAcqOrderFromDB(CString &f_csStudyLoid,
                              int *f_nseries_count,
                              vector<CString> &f_csAcqVec,
                              LPCTSTR **f_SeriesLOIDArray);
    void CleanUpLocalDBMgr(CDBSAVR *leaf,
                           CDBSAVRNode *node,
                           CDBSAStudy *out_node_study,
                           CDBSASeries *out_node_series,
                           CDBSAImage *out_node_image,
                           DBSAFactory *factory,
                           CDBSATagErrorList *error_list);
    int ConvertAcqOrderToString(const WORD* f_acqorder,
                                const int f_acqordersize,
                                /*out*/vector<CString> &f_csAcqVec);  

    int GetSeriesNumberFromDB(CString &f_csSriesLoid, long *f_isSeriesNumString);
    int GetAcqStatusFromDB(CString &f_csSriesLoid, long *f_AcqStatus);
    int TagReadingForImageData(CString &f_image_loid,
                               int *f_size,
                               unsigned short *f_image_width,
                               unsigned short *f_image_height,
                               unsigned short *f_image_bit,
                               void **f_img_data,
                               float *f_img_scale_factor);
    int TagReadingForGatingPeriod(CString &f_image_loid,
                                  int *f_size,
                                  float f_gating_period);
    void InitTest();  
	int TagReadingForMaxPixelValueOfImage(CString &f_image_loid,
										   signed short *f_max_pixel_value_of_img_from_tag);
    int GetProtocolSequenceFromSeriesLevelDB(CString &f_csSeriesLoid, CString &f_csProtocolSequence);
    int getStudyPathFromDB(CString &f_csStudyLoid, CString &f_StyudyPath);


// Implementation
private:
    int CopySeries(CString& out_series, LPCTSTR parent_study_loid, LPCTSTR source_series_loid);
    int CopyImage(CString& out_img_loid, LPCTSTR parent_series_loid, LPCTSTR source_image_loid);
};

#endif // !defined(AFX_LOCALDBMGR_H__249EA752_67C9_4860_B86F_6C739EBEB48C__INCLUDED_)
