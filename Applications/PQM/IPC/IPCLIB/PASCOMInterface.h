//Redmine-783
//Redmine-774 : Unnecessary functions removal
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PASCOMInterface.h
 *  Overview: Structure of CPASCOMInterface class.
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
#ifndef __SM_IPCLIB_PASCOM_INTERFACE_H__
#define __SM_IPCLIB_PASCOM_INTERFACE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Pqm.h" - Commented
#include <libStudyManager/NonCopyable.h>

//Patni-Sribanta/2010Dec06/Added/IR-170_Reopen
extern "C" {
#include <tami/libvfToUi/vfToUiDef.h>
}

//Patni-AA/2010Nov11/Modified/V2.00#IR-171
#include <PASCOMLib/SequenceParameter.h>
#include <PASCOMLib/IPASCOMListener.h>
#include <PASCOMLib/PASCOMVarFieldLayer.h>

class CPASDBMetaData;
class CPASCOMLayer;
class CPqm;

class CPASCOMInterface : public IPASCOMListener
    , private MR::ACQ::SM::COMMON::NonCopyable<CPASCOMInterface>
{
public:
    //Constructor / Destructor
    explicit CPASCOMInterface(const int f_mode);
    virtual ~CPASCOMInterface();


    void ReadSequenceParameterXML(Data_decl_list& f_data_decl_list);
    void SendSequence(SequenceList& f_seq_to_send);

    //Patni-Sribanta/2010Oct28/Added/IR-170
    bool CheckLicense(const CString& f_pas_path) const ;

    //Patni-Sribanta/2011Feb25/Added/IR-170_Part3
    bool IsValidLicenseInLabelFile(const CString& f_pas_path);

    bool GetAllSequences(SequenceList& seq_list, int& mode);

    bool GetAllSequences(SequenceList& f_sequence_list,
                         const CString& f_container_path,
                         const PasUniqueId& f_pas_id);

    void GetLicensedAcqOrder(VuiAcqOrder_t& f_acq_order, const CString& f_pas_path);
    //ADD SEQUENCE FIX 25 June 08
    CPASCOMSequence* GetSingleSequence(const CString& f_path, long& f_acqordr);

    int  GetMode() const;
    void SetMode(const int mode);

    void SetPqm(CPqm* p_pqm);

protected:

    //Patni-AA/2010Nov11/Modified/V2.00#IR-171
    CPASCOMVarFieldLayer* m_pascom;
    CPqm* m_ptr_pqm;

private:

    bool m_study_null_msg_displayed;
    int  m_mode;

    //+Patni-Sribanta/2010Dec06/Added/IR-170_Reopen
    void MemInitSeqDB()const;
    bool IsValidLicenseFor(const CString& f_pas_path, const long& f_acqorder);
    //-Patni-Sribanta/2010Dec06/Added/IR-170_Reopen

    bool SequenceSending_Callback(const PASCOM_COMM_MSG& f_pascom_msg);
    bool SequenceReceiving_Callback(const PASCOM_COMM_MSG& f_pascom_msg);
    bool HandlePASCOMMsg(const PASCOM_COMM_MSG& f_pascom_msg);
    void ConvertSequenceToXML(CPASCOMSequence* f_seq, TCHAR* l_seq_str);
    void CreateSequenceformString(const CString& f_seq_data);
    void AddReceivedSequence(SequenceList& f_seq_list);

    /////////////////////////////////////////////
    //          PAS related operations
    /////////////////////////////////////////////
    bool DuplicatePAS(PAS& f_pas,
                      const CString& f_container_path)const;

    bool DeletePAS(const PasUniqueId&    f_pas_id,
                   const CString&   f_container_path)const;

    bool SortPAS(PasUniqueIdList& f_pas_id_list,
                 const CString&    f_container_path)const;

    void CreatePASDB(const CString& PASDBpath,
                     const CString& f_mapfilePath,
                     CPASDBMetaData* f_metadata)const;

    bool CreatePAS(PAS& f_pas,
                   const CString& f_container_path);

    bool RenamePAS(const PasUniqueId& f_pas_id,
                   const CString&     f_container_path,
                   const CString&     f_new_name)const;

    bool GetAllPAS(PasList& f_pas_list,
                   const CString& f_container_path);

    /////////////////////////////////////////////
    //          Sequence related operations
    /////////////////////////////////////////////
    bool PasteSequences(SequenceList& f_sequence_list,
                        const CString&       f_container_path,
                        const PasUniqueId&   f_pas_id)const;

    bool DeleteSequences(const CString& f_container_path,
                         SequenceUniqueIdList& f_sequence_id_list,
                         const PasUniqueId& f_pas_id)const;

    bool SortSequence(SequenceUniqueIdList& f_sequence_id_list,
                      const CString& f_container_path,
                      const PasUniqueId& f_pas_id)const;

    bool RenameSequences(SequenceUniqueIdList& f_sequence_id_list,
                         const CString& f_pas_path,
                         const CString& f_new_name);


};
#endif // #ifndef __SM_IPCLIB_PASCOM_INTERFACE_H__ 