//Redmine-777
//REDMINE-780
//Redmine-783
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASCOMSequence.h
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

#ifndef __SM_PASCOM_SEQUENCE_H__
#define __SM_PASCOM_SEQUENCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SequenceParameter.h"
#include "PASParamStore.h"


class AFX_EXT_CLASS CPASCOMSequence
{
public:

    CPASCOMSequence();

    CPASCOMSequence(
        const CString& f_seq_name,
        const SEQUENCE_PARAM& f_seq_param,
        const CString&  f_seq_path,
        const PasUniqueId& f_pas_id
    );

    virtual ~CPASCOMSequence();

    CPASCOMSequence(const CPASCOMSequence& f_copy_obj);
    CPASCOMSequence& operator=(const CPASCOMSequence& f_objSeq);

    ///MEENAKSHI N
    void SetMode(const int mode);

    void GetSequencePath(CString& f_path)const;
    void SetSequencePath(const CString& f_path);

    //WFDA
    void GetSequenceID(SequenceUniqueId& f_seq_id)const;
    void SetSequenceID(const SequenceUniqueId& f_seq_id);

    //Used by Memory
    void GetSequenceName(CString& f_seq_name)const;
    void SetSequenceName(const CString& f_seqname);

    CPASParamStore* GetSequenceParam()const;
    void SetSequenceParam(const CPASParamStore* f_SeqParameter);

private:

    SEQUENCE_PARAM		m_seq_param;		// Sequence Parameter structure.
    PasUniqueId			m_unique_id;		// Seq Unique ID
    CPASParamStore*		m_objParamStore;    // Sequence Parameter store object
    CString				m_seq_path;		    // Sequence Xml path
    CString				m_study_loid;       //WFDA
    _TCHAR*             m_seq_name;
    int m_mode;                             ///MEENAKSHI N

    void Serialize(CArchive& archive);
    bool LoadData();

    static CPASCOMSequence* DeSerialize(CArchive& archive);

    //+Patni-RBJ/2011Jan20/Redmine-783
    ///MEENAKSHI N
    int GetMode()const;

    void GetStudyLoid(CString& f_study_loid)const;
    //-Patni-RBJ/2011Jan20/Redmine-783

    //WFDA
    void SetStudyLoid(const CString& f_study_loid);
};

#endif // #ifndef __SM_PASCOM_SEQUENCE_H__