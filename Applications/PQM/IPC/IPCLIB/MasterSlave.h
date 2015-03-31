// MasterSlave.h: interface for the CMasterSlave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERSLAVE_H__EA6092EB_6530_400B_9E77_FA2515CA123D__INCLUDED_)
#define AFX_MASTERSLAVE_H__EA6092EB_6530_400B_9E77_FA2515CA123D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

typedef enum  {
    PLAN_DYNAMIC = 1,
    AUTO_COPY_PROCON,	//On Plan Dynamic
    AUTO_COPY_PQM,		//On Clicking plan mark from gui
    AUTO_COPY_MEMORY	//On Slave transfer
} overwrite_info_mode;

typedef enum {
    STATUS_ERROR = -1,
    NOT_SAME,
    SUCCESS
} status_masterslave_s;

typedef union AutoCopyParameter_type_t {
    int*	m_protocol_index;
    char*	m_scratchpad;
} AutoCopyParameter_type;

//Class forward declarations
class CPqm;
class CPqmProtocol;
class CPQMStudy;
class CVarFieldHandle;
struct SDynamicParamCtrls;
class CMasterSlave  : private MR::ACQ::SM::COMMON::NonCopyable<CMasterSlave>
{
public:
    explicit CMasterSlave(CPqm* f_pqm_ptr);
    virtual ~CMasterSlave();

    status_t PqmPmSaveForMasterSlaveProtocol(CPqmProtocol* const f_master_protocol,
            AutoCopyParameter_type f_parameter, const overwrite_info_mode f_overwrite_info_mode);


    void	AllocateGroupIdForDuplicateProtocols(VARIANT* f_pdata);

    void	UpdateGroupNum();
    int		IsMasterSlaveProtocol(const CPqmProtocol* const f_protocol) const;
    void	OnDeleteUpdateMasterSlave(VARIANT* f_pdata);
    int		CheckForSlaveScan(CPqmProtocol* const f_current_prot);
    int		CheckMasterSlaveProtocol(CPqmProtocol* const f_current_prot, const float f_couch_pos, const int f_scan_number);
    void	ChangeToNormalProtocol(CPqmProtocol* const f_current_prot, const bool f_update_gui = true);

    void	PerformAutoCopyForMasterSlave(VARIANT* const f_pdata);
    void	UpdateSlaveForMaster(CPqmProtocol* const f_slave_protocol);
    int		GetProtocolIndexToUpdate()const;
    int		GetSlavesProtocolIndex(const int f_master_group_id, int** f_slaves_index) const;
    void	GetMasterListForPlanMark(VARIANT* f_pdata);
    void	SetMasterProtocolForAllSlaves();
    void	SetGroupIdForTransferredProtocol(CPqmProtocol* const f_current_protocol);
    CPqmProtocol* GetMasterProtocolForGroupId(const int f_group_id) const;
    void	SetGroupNum(const int f_group_num);

private:

    CPqm*		m_pqm_ptr;
    CPQMStudy*	m_pqm_study;
    int			m_group_num;		//to maintain group num.
    int			m_protocol_index_to_update; //To update protocol on GUI


    void IdentifiyMasterSlaveGroupID(const int f_selected_count, int* const f_selected_items);

    bool DecideGroupIdForDuplicated(const int f_selected_count, int* const f_selected_items);
    int DecideGroupIdForDuplicatedSlave(const int* const f_selected_items, const int f_selected_count, const int f_index) const;


    void DeleteSlaveGroupId(const int f_master_group_id);

    status_t PqmPmSaveForPlanDynamic(CPqmProtocol* const f_master_protocol, const char* const f_scratchpad_file,
                                     SDynamicParamCtrls* f_plan_dynamic_ctrls, const int f_plan_dynamic_ctrls_length);

    status_t PqmPmSaveForAutoCopy(CPqmProtocol* const f_master_protocol, AutoCopyParameter_type f_parameter,
                                  SDynamicParamCtrls* const f_autocopy_dynamic_ctrls, const int f_autocopy_dynamic_ctrls_length,
                                  const overwrite_info_mode f_overwrite_info_mode);

    status_t ReadDynamicParams(SDynamicParamCtrls* const f_dynamic_ctrls, const int f_dynamic_ctrls_length,
                               const int f_protocol_acq_order, const char* const f_scratchpad_file);

    status_t UpdateDynamicParams(const char* const f_scratchpad_file, SDynamicParamCtrls* const f_dynamic_ctrls,
                                 const int f_dynamic_ctrls_length);

    void DeleteBuffer(SDynamicParamCtrls* const f_dynamic_ctrls, const int f_dynamic_ctrls_length);

    bool DisplayWarningMessage(const CString& f_warning_message);

    // Coil,Anatomy,couch position, home position are same for master and slave (return: 1 for YES, 0 for NOT, -1 for ERROR)
    status_masterslave_s     IsMasterSlaveInfoSame(CPqmProtocol* const f_slave_prot, CPqmProtocol* const f_master_prot);
    // FOV, SLICE_ORI_VECTOR,PHASE_ORI_VECTOR,OFFSET_VECTOR are same for master and slave (return: 1 for YES, 0 for NOT, -1 for ERROR)
    status_masterslave_s  IsMasterSlaveVectorSame(CPqmProtocol* const f_slave_prot, CPqmProtocol* const f_master_prot);

    bool IsAlreadyMasterBeforeCurrent() const;

    void AbortScan(CPqmProtocol* const f_current_prot);


    void	CreateNewMaster(CPqmProtocol* const f_master_protocol);
    status_t UpdateProtocolInformation(CVarFieldHandle* f_vf_handle, CPqmProtocol* const f_current_protocol);
    void	GetAutoCopyScratchPadPath(char* const f_path) const;
    int		GetNewGroupId();
    void	SetGroupNumToVFC();
    int		GetGroupNumFromVec();

};

#endif // !defined(AFX_MASTERSLAVE_H__EA6092EB_6530_400B_9E77_FA2515CA123D__INCLUDED_)
