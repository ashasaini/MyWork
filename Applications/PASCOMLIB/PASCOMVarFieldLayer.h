//Redmine-777
//REDMINE-780
//Redmine-774 : Unnecessary functions removal
//Redmine-783
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASCOMVarFieldLayer.h
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

#ifndef __SM_PASCOM_VARFIELD_LAYER_H__
#define __SM_PASCOM_VARFIELD_LAYER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//+Patni-AA/2010OCT28/Added/V2.00#IR-171
#include "SequenceParameter.h"
#include "PASCOM_COMM_Def.h"
//+Patni-AA/2010OCT28/Added/V2.00#IR-171

class CPASDBMetaData;
class CPASCOMLayerFactory;//USHA TAREVIEW
class CCommunicator;
class CMPlusVarfieldManager;
class CPASType;
class CPASParamStore;

class AFX_EXT_CLASS CPASCOMVarFieldLayer
{
    friend class CPASCOMLayerFactory;//USHA TAREVIEW

public:
    CPASCOMVarFieldLayer(); // Singleton class

    CPASCOMVarFieldLayer(const CString& f_channel_name, const CString& f_app_name,
                         const CString& f_mode);

    virtual ~CPASCOMVarFieldLayer();

    void QueryPASInformation()const;

    bool PASCOMSeqSendingCallback(PASCOM_COMM_MSG& f_pascom_msg);

    bool PASCOMSeqReceivingCallback(PASCOM_COMM_MSG& f_pascom_msg);

    bool PASCOMSeqSelectedCallback(PASCOM_COMM_MSG& f_pascom_msg);

    void ConvertSequenceToXML(CPASCOMSequence* f_seq, TCHAR* l_seq_str);

    void CreatePASDB(const CString& PASDBpath, const CString& f_mapfilePath,
                     CPASDBMetaData* f_metadata)const;

    void CreatePASDB(const CString& PASDBpath, const CString& f_mapfilePath,
                     CPASDBMetaData* f_metadata, CList<CString, CString&> &f_anatomies,
                     CList<CString, CString&> &f_anatomydirectories)const;

    void DuplicatePAS(PAS& pas, const CString& xmlpath)const;

    void DeletePAS(const CString& container_path, const PasUniqueId& Pas_uniqueID)const;

    void SortPAS(PasUniqueIdList& pasIDList, const CString& container_folder_Path)const;

    void CreatePAS(PAS& pas, const CString& xmlpath);

    void RenamePAS(const PasUniqueId&	unique_id, const CString&	containerfolderPath,
                   const CString& newname)const;

    //Patni-PJS/2010Jun14/Modified/TMSC_REDMINE-116
    bool PasteSequence(const CString&	xmlpath, const PASCOM_STORE::SeqInsertType f_flag = PASCOM_STORE::TRANSFER_MEMORY)const;

    void DeleteSequences(const CString& container_path,
                         SequenceUniqueIdList& seq_uniqueIDlist, const PasUniqueId& Pas_uniqueID)const;

    void SortSequence(SequenceUniqueIdList& seqIDList,
                      const PasUniqueId& pas_uniqueID, const CString& container_folder_path)const ;

    void ReadSequenceParameterXML(Data_decl_list& f_data_decl_list);

    void AddReceivedSequence(SequenceList& f_seq_list);

    void CreateSequenceformString();

    void Communicate(WPARAM wParam, LPARAM lParam, CString& f_connect_to_app);

    void Communicate(WPARAM wParam, LPARAM lParam, CString& f_connect_to_app,
                     const int f_mode) ;//MEENAKSHI N

    bool RegisterListener(IPASCOMListener* f_listener);

    //Patni-Sribanta/2010Oct25/Added/IR-170
    bool CheckLicense(const CString& f_pas_path)const;

    //+Patni-NP/2009Aug28/Modified/License check
    void MemInitSeqDB()const;
    bool IsValidLicenseFor(const CString& f_pas_path, const long& f_acqorder);	//Patni-Sribanta/2010Dec10/Added/IR-170-reopen
    //-Patni-NP/2009Aug28/Modified/License check

    void RenameSequences(SequenceUniqueIdList&	seq_uniqueIDlist,
                         const CString& pasfolderPath, const CString& newname, const CString& seq_comment)const;

    //+Patni-Nitin/Added/ACE-2/WFDA
    void UpdatePguids(SequenceUniqueIdList& seq_uniqueIDlist,
                      const CString& pasfolderPath, CString Pguid)const;
    //-Patni-Nitin/Added/ACE-2/WFDA

    void CopySequence(
        const int f_acqordersize,
        SequenceUniqueIdList& f_seqlist,
        CString f_seqpath,
        const PASCOM_STORE::SeqInsertType f_flag = PASCOM_STORE::TRANSFER_MEMORY);

    void InitAnatomyStructure(const CString& f_path)const;

    BOOL CreateProconScratchPad(
        CString f_pasfilepath, const int f_index);


    void ReplaceModifiedSequence(const long f_index, const CString&	xmlpath,
                                 SequenceUniqueIdList* f_slave_seq_index)const;

    CPASCOMSequence* GetLastSequence(const CString& f_filepath);
    void GetAllPas(PasList& pas_list, const CString& container_path);

    bool GetAllSequences(SequenceList& seq_list, int& mode);

    void GetAllSequences(SequenceList&	seq_list, const CString& path,
                         const PasUniqueId& PAS_id);

    CPASCOMSequence* GetSingleSequence(const CString& f_filename, const long& f_acqorder);


    void UpdateGroupIdInVFC(SequenceUniqueIdList&	f_seq_uniqueIDlist,
                            const CString&	f_pasfolderPath, const int f_group_id);
    void UpdateGroupNumInVFC(const CString&	f_pasfolderPath, const int f_group_num);


    //Patni-Sudhir/2011Jan17/Added/Redmine-434
    CString GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG)const;

    const CString GetEMTFilterStringForProtocol(const int f_acq_order)const;
    void SetAddseqAnatomyPath(const CString& f_anantomypath)const;

    //Patni-Sribanta/2011Feb16/Added/IR-170_Part3
    bool IsValidLicenseInLabelFile(const CString& f_pas_path);

    void CPASCOMVarFieldLayer::UpdateAutoLocatorPlannedStatus(const CString& f_vfpasfile, const int f_acqorder);
    void CPASCOMVarFieldLayer::UpdateCurrentpageTagToNone(const CString& f_vfpasfile, const int f_acqorder);
    void GetSequenceArchitecture(const CString& f_path, CString& f_seq_arch, const int f_acqorder);
    void GetSequenceName(const CString& f_path, CString& f_seq_arch, const int f_acqorder);

private:

    static CPASCOMVarFieldLayer* m_varfieldlayer;

    Data_decl_list      m_data_decl_list;
    PasUniqueId	        m_curr_pas;
    PasUniqueId         m_dup_pas;
    AttrList            m_seq_name_list;
    PASCOMListenerList  m_listner_list;
    SequenceList        m_acquire_received_seq_list;
    SequenceList        m_schedule_received_seq_list;

    //Patni-RBJ/2011Jan20/Redmine-783
    CMPlusVarfieldManager* m_var_fld_manager;
    CPASCOMSequence* m_series;
    CCommunicator* m_comm;
    CPASType* m_pas_type;

    CString m_curr_container_path;
    CString m_sequenceparameter_xml_filepath;
    CString m_seq_para_xml_filepath;
    CString m_pascom_config_directorypath;
    CString m_strappname;
    CString m_strmode;

    //Patni-Sudhir/2011Jan17/Added/Redmine-434
    CString m_sequence_path;

    long* m_acq_order_list;
    long m_acq_order_size;
    int m_mode;


    //Patni-NP/2009Aug28/Modified/License check
    bool MemCheckSeqLicense(const char* seqname)const;

    void CreatePasUniqueId(PasUniqueId& f_postion);

    void AddPAS(PAS& f_pas, const CString& f_path);

    //Patni-PJS/2010Jun14/Modified/TMSC_REDMINE-116
    bool AddSequences(SequenceList& seq_list, const CString xmlpath,
                      PasUniqueId pas_unique_id);

    ERROR_CODE SetResource()const;

    void CreateSeqParamList(AttrList& attr_list, AttrValues& attr_values,
                            CPASParamStore* f_objParamStore);

    void CheckVarfieldDB(const CString& f_inifilepath)const;

    void CheckConfigurationFiles();

    void FillSequence(CPASParamStore* f_objParamStore, AttrList& attr_list,
                      AttrValues& attr_values);

    void StringToValue(const CString&	seqParamIn, CString& seqParamOut)const;

    void StringToValue(const CString& strval, long& value);

    void StringToValue(const CString& f_seqParamIn, float& f_seqParamOut)const;

    BOOL IsNumerical(const CString& f_validate_string)const;

    bool CheckLicenseForEachTokens(CString& f_lineinfile);

    void CheckForValidPas(CString& f_linein_file, TCHAR* f_pasname, TCHAR* f_pastype, bool& f_valid_pas);

    void GetSequence(const CString& f_seq_path, SequenceList& f_seq_list);

    CString GetConfigPath(void)const;

    CString GetLicenseStringForPas(CStdioFile& l_labelfile, const CString& l_srcfilename) const;

    int GetMode()const;
    void SetMode(const int mode);


};

#endif // #ifndef __SM_PASCOM_VARFIELD_LAYER_H__
