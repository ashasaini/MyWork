//Redmine-777
//REDMINE-780
//REDMINE-782
//REDMINE-774
//Redmine-783
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASParamStore.h
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

#ifndef __SM_PASCOM_PAS_PARAM_STORE_H__
#define __SM_PASCOM_PAS_PARAM_STORE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <map>




// This is a PASCOM library Class.
// When ever a new datatype is added, we need to derive a class from this
//  at client side and need to write the  getter and setter methods for that
//  new datatype
class AFX_EXT_CLASS CPASParamStore
{

public:

    CPASParamStore();
    explicit CPASParamStore(const CString& f_name);
    virtual ~CPASParamStore();

    CPASParamStore(const CPASParamStore& f_objParamStore);
    CPASParamStore& operator=(const CPASParamStore& f_objParamStore); // overloaded assignment operator

    void Serialize(CArchive& archive);

    //Patni-RBJ/2011Jan20/Redmine-783
    CPASParamStore* AddNestParamStore(const CString& f_name);

    static CPASParamStore* DeSerialize(CArchive& archive);

    int GetInt(LPCTSTR f_valueName) const;
    void SetInt(LPCTSTR f_valueName, const int f_value);

    VARIANT GetVector(LPCTSTR f_valuename, CString& f_str_value)const;
    void SetVector(LPCTSTR f_valuename, const CString& f_value);

    float GetFloat(LPCTSTR f_valueName)const;
    void SetFloat(LPCTSTR f_valueName, const float f_value);

    bool GetBool(LPCTSTR f_valueName)const;
    void SetBool(LPCTSTR f_valueName, const bool f_value);

    CString GetString(LPCTSTR f_valueName)const;
    void SetString(LPCTSTR f_valueName, const CString& f_value);

private:

    typedef std::map<CString, CString>	CNameValue;
    typedef std::map<CString, CPASParamStore*>	CNestedParams;

    CNameValue		m_ps;
    CNestedParams	m_nested_ps;
    CString			m_name;

    void ParseLine(
        const CString& f_strline,
        CString& f_name,
        CString& f_key,
        CString& f_value
    )const;

    void ConvertStringToVector(
        const CString& f_vector_data,
        VARIANT& f_variant_data
    )const;

    void WriteToFile(const CString& f_file_name)const;
    void WriteToFile(CStdioFile& l_file) const;
    void LoadFromFile(CStdioFile& l_file);

    static CPASParamStore* LoadFromFile(const CString& f_file_name);
    CPASParamStore* GetNestParamStore(const CString& f_name);

    const CString Get(const CString& f_key) const;
    void Set(const CString& f_key, const CString& f_value);

    void CPASParamStore::CopyMapValue(CNestedParams& f_ps_to_copy, const CNestedParams& f_ps_from_copy);
};

#endif // #ifndef __SM_PASCOM_PAS_PARAM_STORE_H__
