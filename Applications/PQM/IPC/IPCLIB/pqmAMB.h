// pqmAMB.h: interface for the CpqmAMB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAMB_H__
#define __SM_IPCLIB_PQMAMB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/AMBCommonStructures.h>
#include "pqmAMBStructs.h"



class CPqmProtocol;
class CPqm;
class CVarFieldHandle;
class CPqmPmAcqman;

//class CInterLock;

typedef struct AMBMasterReArrangeStruct_s {
    int m_master_pos_org;
    int m_master_pos_new ;
    int  m_count;
} AMBMasterReArrangeStruct_t;

class CpqmAMB : private MR::ACQ::SM::COMMON::NonCopyable<CpqmAMB>
{
public:
    CpqmAMB(CPqmPmAcqman* acq_manager);
    virtual ~CpqmAMB();

    BOOL CheckAmbInitialPlan(const int* f_selected_items,  const int f_count);
    status_t pqmCheckAMBplan(amb_t*	const f_ambplan);
    status_t pqmGetAMBPlan(amb_t* const  ambpaln, const int	prot) const;
    void PqmAmbEachAcquisitionDone(CPqmProtocol* const 	f_obj, const bool_t f_seg2_flag);
    void PqmAmbEachAcquisitionStart(CPqmProtocol* const f_obj, const bool_t	f_pro_check) ;
    status_t pqmAMBScanstart(amb_t*	const ambplan) ;
    status_t pqmAMBScanAbort() ;
    bool_t PqmAMBScanSeg2Active(void) const;
    void PqmPmAbortAmb(BOOL f_is_dialog_end = TRUE);
    bool PqmAMBCheckScanStatus(const int f_sts) const;
    void PqmAmbDeleteSlave() ;
    void PqmAmbDeleteAutoShimMapFromVarfieldOnly();
    void PqmAmbPrepStart(void);
    void PqmAmbPrepEnd(void);
    void PqmAMBVisualPrepEnd(const int f_prot) ;
    status_t PqmPqAMBSlaveLost(int* const f_selected_list , int* const f_selected_count) const;

    BOOL CheckAMBMenuClickConditions(const int* f_selected_items, const int f_count) const;
    status_t UpdateSecondOnwardsSlaveData();


    int GetAMBIndexToUdpate() const;
    void SetAMBIndexToUpdateOrDelete(const int f_index) ;
    void GetAMBShimMapArray(VARIANT**  f_data) ;
    void GetAMBAutoScanItems(int** f_arr_items, int* f_count, BOOL* f_is_auto_scan_on) const;
    void GetAMBMasterProtocols(VARIANT**  f_data) ;
    void GetAMBSlaveCount(int* f_count) const;
    void GetRearrangeIndex(int* f_protocol_index_to_move, int* f_target_index) const;
    int GetIndexForParentImageRemoved() const;

    bool CanPerformPrep() const;
private:
    int m_acq_order_for_ui;
    CPqm* m_pqmptr;
    CPqmPmAcqman* m_acqman_ptr;

    PqmPmAmbT m_pqmpm_amb;
    PqmAmbScanProt_t m_PqmAmbScanProt;

    ambSlave_t m_slave_list;

    AMBMasterReArrangeStruct_s m_master_rearrange_struct ;
    AMB_shim_map_struct_t m_shim_map_list;
    int m_index_to_update_or_delete;

    status_t pqmAmbCouchCheck(void) ;
    status_t pqmAmbCheckMoveCouch(int*	const f_msg) ;
    status_t pqmAmbPopupCheckParent(void* f_data) ;
    void pqmAmbPopupNormalCancel();
    void pqmSendAMBScanStart_Error();
    void pqmAmbSystemError();

    int  pqmAmbCouchParent(void);
    status_t pqmAmbCheckParentCouchOffset(CPqmProtocol*	 const f_obj, const float	f_ccp) const;
    status_t pqmAmbPopupParentClear() ;
    void pqmAmbInitPatientPath(CPqmProtocol* f_obj) const;
    status_t pqmAmbPopupScanGo(void*	f_data);
    status_t pqmAMBmakeProtocol(void);
    status_t pqmAmbSortSeg1Proc(int* const f_prot_list, const int f_num);
    status_t pqmAmbMakeDuplicateList(int* const prot_list, const int count) const;
    status_t pqmAmbSortShim_MAP(int* const f_prot_list, const int f_num) const;
    status_t pqmAmbMakeScanTbl(void);
    void PqmAmbSetMovingCouch(const int	f_prot, const int f_master_prot = -1);
    void pqmPmAmbEnd() ;
    status_t pqmAmbSEG2ScanSetup(void);
    void	PqmAMBScanStartSeg2(CPqmProtocol* const f_prObj);
    void	PqmAMBScanEndSeg2(void);
    status_t pqmAmbAcquisitionCheck(PqmAmbScanTbl_t* const f_ScanTbl) ;
    status_t pqmAmbDuplicate(int* f_prot_list, const int f_count);
    PqmAmbScanTbl_t* pqmAMBGetScantblFromProt(const int	f_prot) ;
    PqmAmbScanProt_t* pqmAmbGetScanProtbl() ;
    status_t pqmAbmDiskSpaceCheck() ;
    void AMBMasterScanStart() const;

    void MakeAutoSeries(SequenceList* const f_sequence_list, const int f_target_position) const;

    bool CreateMPlusSeriesAndGUIProtcol(const int f_count) ;
    CPASCOMSequence* CreateMPlusSeriesAndGUIProtcol(const int f_acq_order, const CString& f_file_path);

    void SetSeriesNoInDB(CPqmProtocol* f_protocol) const;
    void InitParameters();
    void SetAMBScanLabel();
    void UpdatePrepForAMBMaster();


};


#endif // #ifndef __SM_IPCLIB_PQMAMB_H__
