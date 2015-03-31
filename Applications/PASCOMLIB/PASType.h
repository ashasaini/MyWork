//Redmine-777
//REDMINE-780
//Redmine-783
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASType.h
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

#ifndef __SM_PASCOM_PAS_TYPE_H__
#define __SM_PASCOM_PAS_TYPE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <map>




class AFX_EXT_CLASS CPASType
{
    //Gives error if moved below
    typedef CList<CPASType*, CPASType*> PASTypeList;

public:

    CPASType();
    virtual ~CPASType();

    CPASType(const CPASType& f_pastype);

    CPASType& operator =(const CPASType& f_pastype);

    CPASType* CreateChild(const CString& f_name);

    void SetPasFolderPath(const CString& f_path);
    void GetChildPASTypes(PASTypeList& l_pas_type_list)const;
    CString GetPasTypeName()const;

    UINT GetId()const;
    void SetId(const UINT f_id);

    CString GetPasTypePath()const;
    void SetPasTypePath(const CString& f_path);

    CPASType* GetParent();
    void SetParent(CPASType* f_parent);


private:

    typedef std::map<CString, CString> AnatomyMapping;
    PASTypeList     m_pas_type_list;
    CPASType*       m_parent;
    CString         m_name;
    CString         m_path;
    CString         m_str_PasFolderPath;
    UINT            m_id;

    void AddChildPASType(CPASType* f_pas_type);
    CPASType* GetPasType(UINT f_id);
    void SetPasTypeName(const CString& f_name);
};


#endif // #ifndef __SM_PASCOM_PAS_TYPE_H__
