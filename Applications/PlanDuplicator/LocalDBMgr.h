// LocalDBMgr.h: interface for the CLocalDBMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALDBMGR_H__D4536F39_5559_4046_A3A0_2A2DEEDC0D15__INCLUDED_)
#define AFX_LOCALDBMGR_H__D4536F39_5559_4046_A3A0_2A2DEEDC0D15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "groupdefs.h"
//#include "dataTypes.h"
#include <DBServer\MpDbsBaseObjectInfo.h>
#include <DBServer\MpDbsDBServer_Defs.h>
#include <DBSALib/DBSALib.h>
#include <DBSALib/DBSAVR.h>
#include <DBSALib/DBSA.h>
#include "DBMgr.h"
using namespace MPlus::DBServer;

class CImageInfo;
class CLocalDBMgr  : public CDBMgr
{
public:
    CLocalDBMgr();
    virtual ~CLocalDBMgr();
    int GetTagValue(dataElementT *f_group,CDBSAVR* f_leaf,const int f_nsize);
    
    // Set attribute tag and type to get
    HRESULT  SetAttrForGet(  MPlus::DBServer::OBJECT_LEVEL_e eDBLevel, 
        const CString& csLoid, 
        const std::vector<TAGINFO_t>& vTagInfo,
        MpDbsBaseObjectInfo* pDBObj );
    
    // Set attribute tag and type to get
    HRESULT  SetAttrForGet(  MPlus::DBServer::OBJECT_LEVEL_e eDBLevel, 
        const CString& csLoid, 
        const TAGINFO_t& vTagInfo,
        MpDbsBaseObjectInfo* pDBObj );
    int SetMpDbsCommit(const CString& f_loid_stdLevel, CString& f_loid_patLevel);
    int buildGroup(dataElementT *f_group,const int f_numElementsInGroup,CImageInfo * f_imginfo) ;

};

#endif // !defined(AFX_LOCALDBMGR_H__D4536F39_5559_4046_A3A0_2A2DEEDC0D15__INCLUDED_)
