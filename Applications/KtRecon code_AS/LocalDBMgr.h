// LocalDBMgr.h: interface for the CLocalDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALDBMGR_H__53D50897_83E8_44F1_B305_DBD9E7C9FF9C__INCLUDED_)
#define AFX_LOCALDBMGR_H__53D50897_83E8_44F1_B305_DBD9E7C9FF9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DBSALib/DBSALib.h>
#include <DBSALib/DBSAVR.h>
#include <DBServer\MpDbsBaseObjectInfo.h>
#include <DBServer\MpDbsSeriesInfo.h>
using namespace MRMPlus::DBSALib;
using namespace MRMPlus::DBSAVR;
using namespace MPlus::DBServer;
using namespace std;

#define KTRECON_IMG_PATH 0x00291052
#define KTRECON_SERIES_NUMBER 0x00200011 
#define KTRECON_STUDY_PATH 0x00291050

#define MAX_BUFFER 256
#define NUM_10 10
enum LOID_TYPE {
	LOID_TYPE_INVALID = -1,
		LOID_TYPE_PATIENT = 0,
		LOID_TYPE_STUDY,
		LOID_TYPE_SERIES,
		LOID_TYPE_IMAGE,
		LOID_TYPE_NONIMAGE
};
class CLocalDBMgr  
{
public:
	int GetStudyFileNameFromStudyLOID(CString &studyloid, CString &Studyname);
	CLocalDBMgr();
	virtual ~CLocalDBMgr();
    int GetProtocolNameFromImageLevelDB(CString f_ImageLoid,
                                        CString &f_csProtocolName,CString &f_csStudyName,CString &f_csCompletepath);

	int GetProtAcqOrderFromDB(CString &f_csStudyLoid,
										   int *f_nseries_count,
										   vector<CString> &f_csAcqVec,
                                           LPCTSTR **f_SeriesLOIDArray);
	int GetSeriesNumberFromDB(CString &f_csSriesLoid, long *f_isSeriesNumString);
	int ConvertAcqOrderToString(const WORD* f_acqorder,
											const int f_acqordersize,
                                         /*out*/vector<CString> &f_csAcqVec);
	int GetParentLOID(LPCTSTR f_child_node_loid,
                      LOID_TYPE f_child_node_loid_type,
                      CString &f_csParentLoid);
};

#endif // !defined(AFX_LOCALDBMGR_H__53D50897_83E8_44F1_B305_DBD9E7C9FF9C__INCLUDED_)
