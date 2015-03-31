//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine -777
//Redmine-780
// AutoMapShimStudy.h: interface for the CAutoInsertProtocolStudy class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_H__
#define __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AutoInsertXmlDataManager.h"
#include "PqmAutoInsert.h"

class CPQMStudy ;
class CPqm;
class CPqmProtocol;
class CScanProcedure;
class CVarFieldHandle;
class CVector3D ;
typedef struct msPosSlabInfo_s msPosSlabInfo_t;



class CAutoInsertProtocolStudy	:
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtocolStudy>
{
public:
    //Constructor / Destructor

    explicit CAutoInsertProtocolStudy(CPQMStudy* f_pqm_study);
    virtual ~CAutoInsertProtocolStudy();

    CPqmProtocol* AddProtocol(const int f_target_position, const int* f_acqorder)const;
    CPqmProtocol* AddAutoProtocol(const int f_target_position, const scan_mode f_mode, CString& f_error_id);
    int CopyProtocolFromFile(const int f_target_position, int& f_acqorder, const scan_mode f_mod);
    bool CopyScanModeFrom(const int f_from_position, const int f_to_position, const scan_mode f_mode);
    float CalcTargetPosition(CPqmProtocol* f_protocol)const;

    void ScanInit(CPqmProtocol* f_targ_protocol, const scan_mode f_mode);
    status_t ConfigureAutoInsertedProtocol(CVarFieldHandle* const vf_handle,
                                           const int32_t tarProtocol,
                                           const int32_t insertProt,
                                           const scan_mode f_mode,
                                           const bool f_can_display_message,
                                           bool& f_has_displayed_message);

    void SetParentInfoToAutoIsertedProtocol(CPqmProtocol* f_from_protocol, CPqmProtocol* f_to_protocol)const;
    void SetContrastAgentToAutoInsertedProtocol(CPqmProtocol* f_auto_protocol, CPqmProtocol* f_contrast_target, const int f_contrast, char* f_contrast_name)const;

    CString GetAutoInsertFilePath(const scan_mode f_mode)const;

private:

    CPQMStudy* m_pqm_study;
    CPqm* m_pqm_ptr;
    CAutoInsertXmlDataManager m_autoinsert_manager;

    void UpdateLocatorImageInfo(const int f_target_position, const int f_from_protocol)const;
    void SetPlnStatusOfAutoInsertProtocol(int f_pln_status, CPqmProtocol* const f_to_protocol)const;
    void CopyCOMPASSSetting(CPqmProtocol* const f_from_protocol, CPqmProtocol* const f_to_protocol, const scan_mode f_mode) const;
    void Init();
    void CalculateShimMapStudyData(CPqmProtocol* f_targ_protocol, const scan_mode f_mode);
    bool CanPerformAutoMapAutoShim() const;
    void SetPqmStudy(CPQMStudy* f_pqm_study);
    void UpdateDegeneracyFlagOfAutoInsertProt(const int f_from_position, const int f_to_position, CPqmProtocol* f_to_Protocol);
    ShimMapStudy* GetShimMapStudy() const ;
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_H__
