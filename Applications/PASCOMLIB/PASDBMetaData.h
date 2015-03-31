//Redmine-777
//REDMINE-780
//Redmine-783
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASDBMetaData.h
 *  Overview		:
 *  Last Modified	: [6/6/2008]
 *******************************************************************************
 *                       Revision History
 *******************************************************************************
 *  Revision	Author		Date & Time			Changes
 *==============================================================================
 *   1.0		Patni       [6/6/2008]			Added file header
 *
 ******************************************************************************/
#ifndef __SM_PASCOM_PAS_DBMETA_DATA_H__
#define __SM_PASCOM_PAS_DBMETA_DATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CPASType;



class AFX_EXT_CLASS CPASDBMetaData
{

    typedef CList<CPASType*, CPASType*> PASTypeList;

public:

    //Patni-RBJ/2011Jan20/Redmine-783
    static CPASDBMetaData* GetInstance();
    void GetAllChild(CPASType* f_pastype);

    //used by Memory
    void GetPASTypes(PASTypeList& f_pas_type_list);

private:

    static CPASDBMetaData* m_pas_db_meta_data;
    PASTypeList m_pastype_list;

    CPASDBMetaData();
    virtual ~CPASDBMetaData();
    CPASDBMetaData(const CPASDBMetaData& f_PASDBMetaData);
    CPASDBMetaData& operator=(const CPASDBMetaData&  f_pas_dbmetadata);
    static void DestroyInstance();
};

#endif // #ifndef __SM_PASCOM_PAS_DBMETA_DATA_H__
