//Redmine-783
//Redmine-780
//Redmine-782
// CoilSelectionTool.h: interface for the CCoilSelectionTool class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_COILSELECTION_TOOL_H__
#define __SM_IPCLIB_COILSELECTION_TOOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CoilDefinitions.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

// pre-declaration
class CPqm;
class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CCoilSelectionTool : private MR::ACQ::SM::COMMON::NonCopyable<CCoilSelectionTool>
{
public:

    //Constructor / Destructor
    CCoilSelectionTool();
    virtual ~CCoilSelectionTool();
    explicit CCoilSelectionTool(CPqm* f_pqm);

    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    // + CoilName AMT
    //    bool CSSetCoilName(
    //        CPqmProtocol* f_obj
    //    );

    bool CSSetCoilName(CPqmProtocol* const f_obj, CVarFieldHandle* const vf_handle = NULL)const;
    // - CoilName AMT
    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

    VftCoilListInfo_t CoilSelConnectCoil(const AM_AcqManCoilInfo_t& f_coil_Info)const;
    bool CoilSelSaveConnectCoil(const AM_AcqManCoilInfo_t& f_coil_Info);
    bool SaveSelectedListToStudy(coilSelectListStruct_t* f_coil_list, const int f_acq_order)const;
    //bool SetConnectedCoilDummy();
    bool CoilSetProtocol(CPqmProtocol* const f_obj,
                         coilSelect_t* const f_selected_coil)const;

    bool SaveSelectedCoilToStudy(coilSelect_t*  f_selected_coil,
                                 const int f_acq_order)const;

    void ResetAcqInfoFlags();

    VftCoilListInfo_t* GetConnectCoilInformation();
    VftCoilListInfo_t* CoilSelGetConnectCoil();

    BOOL SetConnectCoil(const AM_AcqManCoilInfo_t&	f_coil_info);

    BOOL GetCoilSelectionFlagActive() const;
    void SetCoilSelectionFlagActive(const BOOL f_flag);

    //Patni-PJS/2009Oct23/Added/MVC005086
    BOOL GetAcqSendFlag() const;
    void SetAcqSendFlag(const BOOL f_acq_send);


private:

    CPqm* m_pqm;
    BOOL  m_acq_send_flag;
    BOOL  m_coil_selection_active;

    VftCoilListInfo_t m_coil_sel_last_acq_coil;
    VftCoilListInfo_t m_connect_coil;
};

#endif // #ifndef __SM_IPCLIB_COILSELECTION_TOOL_H__ 
