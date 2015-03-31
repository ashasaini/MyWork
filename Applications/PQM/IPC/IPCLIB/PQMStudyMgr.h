//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMStudyMgr.h
 *  Overview: Structure of CPQMStudyMgr class.
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
//This class will contain all the study related information like the patient name etc.

#ifndef __SM_IPCLIB_PQMSTUDYMGR_H__
#define __SM_IPCLIB_PQMSTUDYMGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DBSALib/DBSAVR.h>
#include <PASCOMLib/PASCOMSequence.h>
#include <libStudyManager/NonCopyable.h>
#include "AutoInsertConstant.h"


extern Data_decl_list  g_sequence_param_name_list;
extern SequenceList m_dbsa_seq_list;

namespace MRMPlus
{
namespace DBSALib
{
class CDBSAStudy;
class CDBSASeries;
}

namespace DBSAVR
{
class CDBSAVRNode;
class CDBSATagErrorList;
}
}
class CPASCOMSequence;
class CPqm;
class CPqmProtocol;

class CPQMStudyMgr : private MR::ACQ::SM::COMMON::NonCopyable<CPQMStudyMgr>

{
public:

    // Default CPQMStudy constructor and destructor
    explicit CPQMStudyMgr(CPqm* pqm);
    virtual ~CPQMStudyMgr();

    BOOL UpdateRealtimeSAR(const int f_acq_order) const;
    BOOL UpdateTotalSAEValue(const int& f_acq_order);
    BOOL WriteSeriesForDBSAVRUSFORSAE(CPqmProtocol* f_pqm_protocol, int32_t f_values[]);
    BOOL WriteSeriesForDBSAVRSQFORRTSAR(CPqmProtocol* f_pqm_protocol, std::vector<f32vec3_t>* f_tag_values) const;
    BOOL WriteStudyIDInlocalDB(const CString& f_run_num)const;
    BOOL SetScanDateTimeInLocalDB(char* f_scan_date_time);
    BOOL ReadSeriesNo(BSTR f_series_loid, long& f_series_no);
    BOOL ReadProtectedStatus(BSTR f_series_loid);
    int  UpdateIECInfo(const long f_series_number, const int f_sar_op_mode, const int f_dbdt_op_mode);
    BOOL WriteSeriesInformationAtSeriesLevel(const CString& f_series_loid);
    BOOL CheckSeriesInMPlusDB(LPCTSTR f_series_loid, DBSAIndex_t& f_series_index);
    void UpdateAcqOrderInLocalDB(WORD* f_acqorder, const int f_acqordersize)const;
    CString ConvertAcqOrderToString(const WORD* f_acqorder, const int f_acqordersize) const;
    bool LoadStudyLOID(LPCTSTR f_loid, const bool f_flag);
    void FreeStudyLoid();
    BOOL DuplicateProtocol(CPASCOMSequence* f_dup_seq_List, const int f_selindex, const int f_index_duplicating_prot);
    void MakeSeries(SequenceList* f_snd_seq_List, LPCTSTR f_studyloid, const mode_automap f_mode = MAP_NORMAL_MODE, const int f_targetpos = -1);
    BOOL DeleteAll(BSTR f_bstr);
    BOOL WriteStudyNameatStudyLevel(const CString& f_study_id);
    BOOL WriteStudyNameatSeriesLevel(const CString& f_study_id);
    void DBSA_Init(); // Init DBSALib
    BOOL AddAcqConAndHomePosAtStudyLevel(const float f_home_position, const  int f_acq_condition);
    BOOL AddAcqConAndHomePosAtSeriesLevel(const CString& f_series_loid, const float f_home_position, const int f_acq_condition);
    BOOL UpdateAutoTransferValueInMPlusDB(CPqmProtocol* f_pqm_protocol);
    BOOL UpdateAnatomyValueInMPlusDB(CPqmProtocol* f_pqm_protocol) const;
    bool AddSeriesReference(CPqmProtocol* f_protocol, LPCTSTR f_series_loid);
    bool CreateSeries(SequenceList* f_snd_seq_List, const int f_firstuisequences = -1, const mode_automap f_mode = MAP_NORMAL_MODE,
                      const int f_targetpos = -1);
    void UpdateComment(BSTR f_seqlid, BSTR f_scancomment);
    BOOL UpdateGUIControlsInDB(VARIANT* f_pdata, BSTR f_seriesid, int f_scan_index = -1); //+Patni-RSG/Added/2009Nov25/MSA0248-0033
    //Patni-PJS/2010Oct26/Added/WFDA/IR-97
    //void CopyWFDAProtocolTags(CPqmProtocol* f_from_obj, CPqmProtocol* f_to_obj);



    BOOL AddWFDACAPPValue(const CString l_series_loid_prev_prot, const CString l_series_loid_curr_prot);
    BOOL WriteCharSetAtStudyLevel() const;
    MRMPlus::DBSALib::CDBSAStudy* GetStudy();
    int GetSeriesCountInLocalDB();
    CString GetPatientUID(LPCTSTR f_patient_loid);
    CString GetStudyUID(LPCTSTR f_study_loid);

    CStringArray* GetNewSeriesLoids()const;
    int GetSeriesCount()const;
    CString GetStudyNameatStudyLevel();
    CString GetSeriesLoid(const CString& f_study_loid, int* f_index);


    bool GetSeriesBias(unsigned short* f_series_bias);
    void SetSeriesBias(const int f_series_bias);

    int  GetTotalCount() const;
    void SetTotalCount(const int f_totalcount);

    int  GetLeafCount() const;
    void SetLeafCount(const int f_leafcount);

    int  GetNodeCount() const;
    void SetNodeCount(const int f_nodecount);

    LPCTSTR GetLOID()const;
    void SetLoid(LPCTSTR f_loid);


    int  SetSeriesNoInDB(CString f_series_loid, const int f_acqStatus, const int f_prot_num = -1);
    BOOL SetAcquisitionStatusInLocalDB(BSTR f_series_loid, const int f_acq_status);
    void SetLeafCountSeries(const int f_seriescount);
    void SetSeriesOrder(BSTR f_bstr, const int f_dropindex);
    void SetchangeOrder(LPCTSTR f_loid);

private:

    WORD* m_wacqorder;
    int	m_series_bias;

    CStringArray* m_new_series_loids;//Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved message
    int m_leafcount_series;
    int m_series_count;
    //TMSC-REDMINE-718
    int m_create_series_cnt;
    LPCTSTR m_stloid;
    CStringArray m_searray;
    MRMPlus::DBSALib::CDBSAStudy* m_dbsa_study;
    CPqm* m_pqm_ptr_dbsa;
    int m_totalcount;
    int m_leafcount;
    int m_node_count;

    BOOL ReadSeriesData(MRMPlus::DBSALib::CDBSASeries* f_series, CPASParamStore* f_objParamStore);
    void TagErrorProcess(MRMPlus::DBSAVR::CDBSATagErrorList* f_taglist)const;
    void UpdateAllSeriesUnderProtocol(std::vector<char*>* f_vect_series_loid, const int f_scan_index,
                                      MRMPlus::DBSAVR::CDBSAVRNode* f_dbsa_node);
    bool UpdateAllImageLoidUnderProtocol(const int f_scan_index, LPCTSTR f_seriesid, CDBSAVRNode* f_dbsa_node);
    bool ValidateSeriesNo(const int f_series_num, LPCTSTR f_series_loid, const unsigned short f_bias);

    LPTSTR* GetDBSAVRCSStrings(const CString& f_dbsavrcsstr, int& f_vm) const;


    LPTSTR GetObjectLevelString(const DBSALib_ObjType_e f_objtype)const;
    LPTSTR GetDCMTypeString(const unsigned int f_dcmtype)const;
    void   GetTagString(LPTSTR f_tagstring, DBSAErrTag_t* f_tagpath, const unsigned int f_pathcnt)const;
    void   GetVMSizeString(LPTSTR f_vmstring, const int f_vmmin, const int f_vmmax)const;
    void   GetUsedString(LPTSTR f_usedstring, DBSALib_Type_e* f_type, const unsigned int f_usedcnt)const;
    void   GetMatchedSeriesLOIDForProtocol(std::vector<CString> *f_vect_str_series_loid, const int f_scan_index);
    void   GetDicomStringforAnatomy(CPqmProtocol* f_pqm_protocol, CString& f_dicom_string) const;

    bool   SetSeriesBiasVal();
    BOOL   SetSeriesForDBSAVRFL(CDBSAVRNode* f_vr_node, CPqmProtocol* f_protocol, const  DBSATag_t f_tag, const f32vec3_t& f_value) const;
    BOOL   SetSeriesForDBSAVRTM(CDBSAVRNode* f_vr_node, CPqmProtocol* f_protocol, const  DBSATag_t f_tag, DateTime_t& f_value) const;

};

#endif // #ifndef __SM_IPCLIB_PQMSTUDYMGR_H__ 
