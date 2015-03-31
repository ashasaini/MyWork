//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMDBSALib.h
 *  Overview: Structure of CPQMDBSALib class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMDBSA_LIB_H__
#define __SM_IPCLIB_PQMDBSA_LIB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusDicomTag/MPlusDicomTag.h>
#include <DBSALib/DBSALib.h>
#include <DBSALib/DBSAVR.h>
#include <libStudyManager/NonCopyable.h>

class CPASCOMSequence;

using namespace MRMPlus::DBSALib;
using namespace MRMPlus::DBSAVR;

namespace MRMPlus
{
namespace DBSAVR
{

class CPQMDBSALib : private MR::ACQ::SM::COMMON::NonCopyable<CPQMDBSALib>
{
public:
    //Constructor / Destructor
    CPQMDBSALib() {
        m_pascom_sequence = NULL;
        this->m_make_objtype = MRC_DBSA_OBJ_NULL;
    }
    explicit CPQMDBSALib(DBSALib_ObjType_e ObjType) {
        m_pascom_sequence = NULL;
        this->m_make_objtype = ObjType;
    }
    ~CPQMDBSALib() {}

    CDBSAVRNode* MakeTagXML(const int f_leafcount_series, const int f_nodecount, const int f_dataset = MRC_FALSE);

    void SetPascomSequence(CPASCOMSequence* f_seq);

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    CPQMDBSALib(DBSALib_ObjType_e ObjType) {
        this->m_make_objtype = ObjType;
        m_pascom_sequence=NULL;
    }
    */
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

private:

    CPASCOMSequence* m_pascom_sequence;
    DBSALib_ObjType_e m_make_objtype;

    bool MakeVR(CDBSAVR* f_vrleaf, const int f_dataset)const;
};

}
}
#endif // #ifndef __SM_IPCLIB_PQMDBSA_LIB_H__
