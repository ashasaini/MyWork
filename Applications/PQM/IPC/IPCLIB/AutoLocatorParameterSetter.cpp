// AutoLocatorParameterSetter.cpp: implementation of the CAutoLocatorParameterSetter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoLocatorParameterSetter.h"
#include <PQM/PQMCommanMessages.h>
#include "PqmPmAcqman.h"
#include "ScanProcedure.h"
#include "LibPLTreeWrapper.h"
#include "pqm.h"
#include "PosSlabInfo.h"
#include "PqmProtocol.h"
#include "PqmPrMode.h"
#include "GPLibAdapter.h"

namespace
{

/**
 * class CVectorTables
 * It get Vector Table Fields at Root Node which written by Auto Locator app,
 * and update Vector at Visual Group Node by PrivateProlim.
 */
class CVectorTables : private MR::ACQ::SM::COMMON::NonCopyable<CVectorTables>
{
public:
    CVectorTables(const VfName_t f_offset_vec_name, const VfType_t f_offset_vec_type,
                  const VfName_t f_phase_vec_name, const VfType_t f_phase_vec_type,
                  const VfName_t f_slice_vec_name, const VfType_t f_slice_vec_type);
    ~CVectorTables();

    status_t GetFields(CVarFieldHandle* const vfdb_handle);
    status_t PutFieldsToScratchPad(CVarFieldHandle* f_scratchpad_handle, const int f_vg_id, const int f_ori_id);

private:
    /**
     * class CVecTable
     * data class.
     */
    class CVecTable
    {
    public:
        VfName_t m_tbl_name;
        VfType_t m_tbl_type;
        VfName_t m_vg_name;
        VfType_t m_vg_type;
        flt32_t  m_value[6][3];

        CVecTable(const VfName_t tbl_name, const VfType_t tbl_type,
                  const VfName_t vg_name, const VfType_t vg_type);
        ~CVecTable();
        status_t PutFieldsToScratchPad(CVarFieldHandle* f_scratchpad_handle, const int f_vg_id, const int f_ori_id);
    };

    CVecTable m_offset;
    CVecTable m_phase;
    CVecTable m_slice;

    typedef std::vector<CVecTable*> VecTableVector_t;
    VecTableVector_t m_tables;
};

/**
 * return value for AbstractAutoLocatorParamSetter ::SetParameters
 */
namespace RETVAL
{
typedef enum {
    SUCCESS = 0,
    APP_RUNNING,
    INVALID_CALC_STATUS,
    FAILED_SET_VECTOR,
    INVALID_INPUT_INDEX,
    UNKNOWN
} SetParamRet_e;
} // RETVAL

/**
 * class AbstractAutoLocatorParamSetter
 * Template Method Pattern.
 */
class AbstractAutoLocatorParamSetter  : private MR::ACQ::SM::COMMON::NonCopyable<AbstractAutoLocatorParamSetter >
{
public:
    RETVAL::SetParamRet_e SetParameters(CPqmProtocol* f_protocol);
    int32_t GetInputProtocolIndex();

protected:
    CPqm* m_pqmptr;
    std::string m_scratch_pad;

    AbstractAutoLocatorParamSetter(CPqm* f_pqmptr, const AUTOLOCATOR_TYPE f_type,
                                   const VfName_t f_input_index_name, const VfType_t f_input_index_type,
                                   const VfName_t f_calc_status_name, const VfType_t f_calc_status_type,
                                   const VfName_t f_offset_vec_name, const VfType_t f_offset_vec_type,
                                   const VfName_t f_phase_vec_name, const VfType_t f_phase_vec_type,
                                   const VfName_t f_slice_vec_name, const VfType_t f_slice_vec_type,
                                   const pr_tag_e f_planned_status_tag, const int32_t f_planned_status_started);

    ~AbstractAutoLocatorParamSetter();

    // SetParameters() sub functions
    virtual int GetPlannedStatus(CPqmProtocol* f_protocol) = 0;
    int32_t GetCalculatedStatus();
    BOOL IsValidInputProtocolIndex();

    // PlanScanOffVector() sub functions
    virtual bool IsValidOrientationId(const int f_ori_id) const = 0;
    virtual bool IsValidCalculatedStatus(const int f_ori_id, const int f_calculated_status) const {
        UNREFERENCED_PARAMETER(f_ori_id);
        UNREFERENCED_PARAMETER(f_calculated_status);
        return true;
    }
    virtual int ConvertOrientationIdForVecTable(const int f_ori_id) const = 0;

    status_t SetFieldsToProtocolNode(const int f_protocol,
                                     const pr_tag_e f_tag, const void* f_value, const VfSize_t f_size);

private:
    const AUTOLOCATOR_TYPE m_type;
    const VfName_t m_input_index_name;
    const VfType_t m_input_index_type;
    const VfName_t m_calc_status_name;
    const VfType_t m_calc_status_type;
    const VfName_t m_offset_vec_name;
    const VfType_t m_offset_vec_type;
    const VfName_t m_phase_vec_name;
    const VfType_t m_phase_vec_type;
    const VfName_t m_slice_vec_name;
    const VfType_t m_slice_vec_type;
    const pr_tag_e m_planned_status_tag;
    const int32_t m_planned_status_started;

    RETVAL::SetParamRet_e PlanScanOffVector(CPqmProtocol* f_protocol);

    typedef std::map<int, int32_t> OriIdMap_t;
    BOOL GetOrientationIdMap(CPqmProtocol* f_protocol, OriIdMap_t* f_orientation_id_map);

    status_t GetFieldFromRootNode(const VfName_t f_name, const VfType_t f_type,
                                  void* f_value, const VfSize_t f_size);

    RETVAL::SetParamRet_e PutVectorToScratchPad(const OriIdMap_t& f_orientation_id_map,
                                                CVectorTables* const f_vec_tables);

    status_t CreateScratchPad(const int f_protocol) const;
    void DeleteScratchPad() const;

    status_t UpdateVectorTableToStudy(const int f_insert_protocol) const;
};

/**
 * class CCardiacParamSetter
 */
class CCardiacParamSetter : public AbstractAutoLocatorParamSetter
{
public:
    CCardiacParamSetter(CPqm* f_pqmptr);
    ~CCardiacParamSetter();

private:
    BOOL GetPlannedStatus(CPqmProtocol* f_protocol);
    bool IsValidOrientationId(const int f_ori_id) const;
    int ConvertOrientationIdForVecTable(const int f_ori_id) const;
};

/**
 * class CNeuroParamSetter
 */
class CNeuroParamSetter : public AbstractAutoLocatorParamSetter
{
public:
    CNeuroParamSetter(CPqm* f_pqmptr);
    ~CNeuroParamSetter();

private:
    BOOL GetPlannedStatus(CPqmProtocol* f_protocol);
    bool IsValidOrientationId(const int f_ori_id) const;
    bool IsValidCalculatedStatus(const int f_ori_id, const int f_calculated_status) const;
    int ConvertOrientationIdForVecTable(const int f_ori_id) const;
};

//***************************Method Header*************************************
//Method Name   :CVectorTables
//Author        :
//Purpose       :Constructor
//*****************************************************************************
CVectorTables::CVectorTables(const VfName_t f_offset_vec_name, const VfType_t f_offset_vec_type,
                             const VfName_t f_phase_vec_name, const VfType_t f_phase_vec_type,
                             const VfName_t f_slice_vec_name, const VfType_t f_slice_vec_type)
    : m_offset(f_offset_vec_name, f_offset_vec_type,
               SVN_PLN_VG_OFFSET_VECTOR, SVT_PLN_VG_OFFSET_VECTOR),
      m_phase(f_phase_vec_name, f_phase_vec_type,
              SVN_PLN_VG_PHASE_ORI_VECTOR, SVT_PLN_VG_PHASE_ORI_VECTOR),
      m_slice(f_slice_vec_name, f_slice_vec_type,
              SVN_PLN_VG_SLICE_ORI_VECTOR, SVT_PLN_VG_SLICE_ORI_VECTOR)
{
    m_tables.reserve(3);
    m_tables.push_back(&m_offset);
    m_tables.push_back(&m_phase);
    m_tables.push_back(&m_slice);
}

//***************************Method Header*************************************
//Method Name   :~CVectorTables
//Author        :
//Purpose       :Destructor
//*****************************************************************************
CVectorTables::~CVectorTables()
{
}

//***************************Method Header*************************************
//Method Name   :GetFields
//Author        :
//Purpose       :Get Vector Table Fields from Root Node.
//*****************************************************************************
status_t CVectorTables::GetFields(CVarFieldHandle* vfdb_handle)
{
    LPCTSTR FUNC_NAME = _T("CVectorTables::GetFields");

    VfFieldSpec_t* vfReq = new VfFieldSpec_t[m_tables.size()];
    memset(vfReq, 0, sizeof(*vfReq) * m_tables.size());

    int i = 0;
    VecTableVector_t::const_iterator itr = m_tables.begin();

    while (itr != m_tables.end()) {
        VFF_SET_ARGS(vfReq[i], (*itr)->m_tbl_name, (*itr)->m_tbl_type,
                     sizeof((*itr)->m_value), (*itr)->m_value, 0);
        ++itr;
        i++;
    }

    int num = 0;
    const status_t l_status = vfdb_handle->GetFields(NULL, SVD_ST_STUDY, vfReq, i, &num);
    DEL_PTR_ARY(vfReq);

    if ((E_NO_ERROR != l_status) || (i != num)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed GetFields()"));
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name   :PutFieldsToScratchPad
//Author        :
//Purpose       :Put each Vector Table data to VisualGroup to Scartch Pad
//*****************************************************************************
status_t CVectorTables::PutFieldsToScratchPad(CVarFieldHandle* f_scratchpad_handle, const int f_vg_id, const int f_ori_id)
{
    LPCTSTR FUNC_NAME = _T("CVectorTables::PutFieldsToScratchPad");

    VecTableVector_t::const_iterator itr = m_tables.begin();

    while (itr != m_tables.end()) {
        if (E_NO_ERROR != (*itr)->PutFieldsToScratchPad(f_scratchpad_handle,
                f_vg_id, f_ori_id)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed"));
            return E_ERROR;
        }

        ++itr;
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name   :CVecTable
//Author        :
//Purpose       :Constructor
//*****************************************************************************
CVectorTables::CVecTable::CVecTable(const VfName_t tbl_name, const VfType_t tbl_type,
                                    const VfName_t vg_name, const VfType_t vg_type)
    : m_tbl_name(tbl_name), m_tbl_type(tbl_type), m_vg_name(vg_name), m_vg_type(vg_type)
{
    memset(m_value, 0, sizeof(m_value));
}

//***************************Method Header*************************************
//Method Name   :~CVecTable
//Author        :
//Purpose       :Destructor
//*****************************************************************************
CVectorTables::CVecTable::~CVecTable()
{
}

//***************************Method Header*************************************
//Method Name   :PutFieldsToScratchPad
//Author        :
//Purpose       :Put Vector Table data to VisualGroup to Scratch Pad
//*****************************************************************************
status_t CVectorTables::CVecTable::PutFieldsToScratchPad(CVarFieldHandle* f_scratchpad_handle, const int f_vg_id, const int f_ori_id)
{
    LPCTSTR FUNC_NAME = _T("CVecTable::PutFieldsToScratchPad");
    if (!f_scratchpad_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scratchpad handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t spVPath[] = {
        {SVN_ST_PROTOCOL,    0},
        {SVN_ST_VISUALGROUP, f_vg_id}
    };

    f32vec3_t l_vector;

    l_vector.x = m_value[f_ori_id][0];
    l_vector.y = m_value[f_ori_id][1];
    l_vector.z = m_value[f_ori_id][2];

    int l_num = 0, l_numDone = 0;

    time_t l_ts = 0;

    VfFieldSpec_t l_putfieldSpec[1] = {0};

    VFF_SET_ARGS(l_putfieldSpec[l_num], m_vg_name, m_vg_type, sizeof(l_vector), &l_vector, VFO_REPLACE);

    l_num++;

    const status_t l_status = f_scratchpad_handle->PutFields(spVPath, SVD_ST_VISUALGROUP, l_putfieldSpec, l_num, &l_numDone, &l_ts);

    if (E_NO_ERROR != l_status || l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to write tag value to study.."));
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name   :AbstractAutoLocatorParamSetter
//Author        :
//Purpose       :Constructor
//*****************************************************************************
AbstractAutoLocatorParamSetter ::AbstractAutoLocatorParamSetter(CPqm* f_pqmptr, const AUTOLOCATOR_TYPE f_type,
        const VfName_t f_input_index_name, const VfType_t f_input_index_type,
        const VfName_t f_calc_status_name, const VfType_t f_calc_status_type,
        const VfName_t f_offset_vec_name, const VfType_t f_offset_vec_type,
        const VfName_t f_phase_vec_name, const VfType_t f_phase_vec_type,
        const VfName_t f_slice_vec_name, const VfType_t f_slice_vec_type,
        const pr_tag_e f_planned_status_tag, const int32_t f_planned_status_started)
    : m_pqmptr(f_pqmptr),
      m_scratch_pad(CPqmUtilities::GetCardiacProtScratchPadPath()),
      m_type(f_type),
      m_input_index_name(f_input_index_name),
      m_input_index_type(f_input_index_type),
      m_calc_status_name(f_calc_status_name),
      m_calc_status_type(f_calc_status_type),
      m_offset_vec_name(f_offset_vec_name),
      m_offset_vec_type(f_offset_vec_type),
      m_phase_vec_name(f_phase_vec_name),
      m_phase_vec_type(f_phase_vec_type),
      m_slice_vec_name(f_slice_vec_name),
      m_slice_vec_type(f_slice_vec_type),
      m_planned_status_tag(f_planned_status_tag),
      m_planned_status_started(f_planned_status_started)
{
}

//***************************Method Header*************************************
//Method Name   :AbstractAutoLocatorParamSetter
//Author        :
//Purpose       :Destructor
//*****************************************************************************
AbstractAutoLocatorParamSetter ::~AbstractAutoLocatorParamSetter()
{
}

//***************************Method Header*************************************
//Method Name   :SetParameters
//Author        :
//Purpose       :Set Vector  Parameters
//*****************************************************************************
RETVAL::SetParamRet_e AbstractAutoLocatorParamSetter ::SetParameters(CPqmProtocol* f_protocol)
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::SetParameters");

    if (!f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return RETVAL::UNKNOWN;
    }

    if (!m_pqmptr->GetAutoLocatorLicense(m_type)) {
        return RETVAL::UNKNOWN;
    }

    int l_planned_status = this->GetPlannedStatus(f_protocol);

    if (l_planned_status == PLANNED_STATUS_NONE) {
        return RETVAL::SUCCESS;
    }

    if ((l_planned_status == PLANNED_STATUS_NOVECTOR)
        || (l_planned_status == PLANNED_STATUS_STARTED)) {

        if (m_pqmptr->IsAutoLocatorRunning()) {
            return RETVAL::APP_RUNNING;
        }

        if (0 >= this->GetCalculatedStatus()) {
            return RETVAL::INVALID_CALC_STATUS;
        }

        const RETVAL::SetParamRet_e retval = this->PlanScanOffVector(f_protocol);

        if (RETVAL::SUCCESS != retval) {
            return retval;
        }

        // l_planned_status may be updated in PlanScanOffVector, so get again.
        l_planned_status = this->GetPlannedStatus(f_protocol);
    }

    if ((l_planned_status == PLANNED_STATUS_PLANNED)
        || (l_planned_status == PLANNED_STATUS_EDITED)
        || (l_planned_status == PLANNED_STATUS_STARTED)) {

        if (FALSE == this->IsValidInputProtocolIndex()) {
            return RETVAL::INVALID_INPUT_INDEX;
        }
    }

    return RETVAL::SUCCESS;
}

//***************************Method Header*************************************
//Method Name   :GetInputProtocolIndex
//Author        :
//Purpose       :To get the input protocol index
//*****************************************************************************
int32_t AbstractAutoLocatorParamSetter ::GetInputProtocolIndex()
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::GetInputProtocolIndex");

    int32_t l_protocol_index = 0;
    const status_t status = this->GetFieldFromRootNode(m_input_index_name, m_input_index_type,
                            &l_protocol_index, sizeof(l_protocol_index));

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFieldFromRootNode failed."));
        return -1;
    }

    if (!m_pqmptr->GetStudy()->GetProtocol(l_protocol_index)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("invalid input protocol index."));
        return -1;
    }

    return l_protocol_index;
}

//***************************Method Header*************************************
//Method Name   :GetCalculatedStatus
//Author        :
//Purpose       :Get calculated status
//*****************************************************************************
int32_t AbstractAutoLocatorParamSetter ::GetCalculatedStatus()
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::GetCalculatedStatus");

    int32_t l_calculated_status = 0;
    const status_t status = this->GetFieldFromRootNode(m_calc_status_name, m_calc_status_type,
                            &l_calculated_status, sizeof(l_calculated_status));

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFieldFromRootNode failed."));
        l_calculated_status = 0;
    }

    return l_calculated_status;
}

//***************************Method Header*************************************
//Method Name   :IsValidInputProtocolIndex
//Author        :
//Purpose       :To Check whether input protocol index is valid or not
//*****************************************************************************
BOOL AbstractAutoLocatorParamSetter ::IsValidInputProtocolIndex()
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::IsValidInputProtocolIndex");

    int32_t l_protocol_index = this->GetInputProtocolIndex();

    if (l_protocol_index == -1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol_index is invalid"));
        return FALSE;
    }

    CPqmProtocol* l_protocol = m_pqmptr->GetStudy()->GetProtocol(l_protocol_index);

    if (!l_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocl is NULL"));
        return FALSE;
    }

    return (l_protocol->GetAutoLocatorType() == m_type) ? TRUE : FALSE;
}

//***************************Method Header*************************************
//Method Name   :SetFieldsToProtocolNode
//Author        :
//Purpose       :Set field to protocol node
//*****************************************************************************
status_t AbstractAutoLocatorParamSetter ::SetFieldsToProtocolNode(const int f_protocol,
        const pr_tag_e f_tag, const void* f_value, const VfSize_t f_size)
{
    BITFLD_DECL(mask, K_ALL_PR) = {0};
    BITFLD_INIT(mask, K_ALL_PR, 0);
    return m_pqmptr->GetStudy()->SetProtocolValues(m_pqmptr->GetStudy()->GetVfDBHandle(),
            f_protocol, mask, f_tag, f_value, f_size, NULL);
}

//***************************Method Header*************************************
//Method Name   :PlanScanOffVector
//Author        :
//Purpose       :to set offset vectors for auto locator protocol
//*****************************************************************************
RETVAL::SetParamRet_e AbstractAutoLocatorParamSetter ::PlanScanOffVector(CPqmProtocol* f_protocol)
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::PlanScanOffVector");

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (!l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    OriIdMap_t l_orientation_id_map;

    if (!this->GetOrientationIdMap(f_protocol, &l_orientation_id_map)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed GetOrientationIdMap"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    CVectorTables vec_tables(m_offset_vec_name, m_offset_vec_type,
                             m_phase_vec_name, m_phase_vec_type,
                             m_slice_vec_name, m_slice_vec_type);

    if (E_NO_ERROR != vec_tables.GetFields(l_vfdb_handle)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed GetFields"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    if (E_NO_ERROR != this->CreateScratchPad(f_protocol->GetProtocol())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in creating Cardiac Scratchpad"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    RETVAL::SetParamRet_e l_result = this->PutVectorToScratchPad(l_orientation_id_map, &vec_tables);

    if (l_result == RETVAL::SUCCESS) {

        CLibPLTreeWrapper l_obj_lib_wrapp(l_vfdb_handle);

        if (!(l_obj_lib_wrapp.ExecutePlUpdateWrapper(m_pqmptr->GetStudy(), f_protocol->GetProtocol(), const_cast<char * const>(m_scratch_pad.c_str())))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Execute PlUpdate failed"));
            DeleteScratchPad();
            return RETVAL::FAILED_SET_VECTOR;
        }

        if (E_NO_ERROR != this->UpdateVectorTableToStudy(f_protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Failed UpdateVectorTableToStudy."));
            l_result = RETVAL::FAILED_SET_VECTOR;
        }

        m_pqmptr->GetStudy()->UpdateSARControlInfoOnProtocolUpdate(f_protocol);

        int l_pln_status = f_protocol->GetPlnStatus() | VFC_PLN_STATUS_GLOC;

        if (E_NO_ERROR != this->SetFieldsToProtocolNode(f_protocol->GetProtocol(),
                K_PR_STATUS, &l_pln_status, sizeof(l_pln_status))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed SetFieldsToProtocolNode() for pln status"));
            l_result = RETVAL::FAILED_SET_VECTOR;
        }

        int l_planned_status = m_planned_status_started;

        if (E_NO_ERROR != this->SetFieldsToProtocolNode(f_protocol->GetProtocol(),
                m_planned_status_tag, &l_planned_status, sizeof(l_planned_status))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed SetFieldsToProtocolNode() for planned status"));
            l_result = RETVAL::FAILED_SET_VECTOR;
        }

        m_pqmptr->GetStudy()->GetProtocolItemValues(l_vfdb_handle, f_protocol);
    }


    DeleteScratchPad();

    return l_result;
}

//***************************Method Header*************************************
//Method Name   :GetOrientationIdMap
//Author        :
//Purpose       :Get Orientation ID
//*****************************************************************************
BOOL AbstractAutoLocatorParamSetter ::GetOrientationIdMap(CPqmProtocol* f_protocol, OriIdMap_t* f_orientation_id_map)
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::GetOrientationIdMap");

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (!l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return FALSE;
    }

    VfPathElem_t spVPath[] = {
        {SVN_ST_PROTOCOL,    f_protocol->GetProtocol()},
        {SVN_ST_VISUALGROUP, 0},
    };

    BITFLD_DECL(mask, MAX_BITFLD) = {0};
    BITFLD_INIT(mask, MAX_BITFLD, 0);
    int l_active = 0;
    int l_total = MAX_BITFLD;

    status_t l_vf_stat = l_vfdb_handle->ScanSubtree(spVPath, SVD_ST_PROTOCOL, SVN_ST_VISUALGROUP,
                         mask, &l_total, &l_active);

    if (E_NO_ERROR != l_vf_stat) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanSubtree failed to calculate the visualgroup nodes"));
        return FALSE;
    }

    VfFieldSpec_t fieldSpec[1] = {0};

    for (int cntV = 0; cntV < l_total; cntV++) {
        if (BITFLD_ON(mask, cntV)) {
            spVPath[SVD_ST_VISUALGROUP - 1].index = cntV;
            memset(fieldSpec, 0, sizeof(fieldSpec));

            int i = 0;
            int32_t l_orientation_id = 0;
            VFF_SET_ARGS(fieldSpec[i], SVN_PLN_CALA_ORIENTATION_ID, VFT_INT32,
                         sizeof(l_orientation_id), &l_orientation_id, 0L);
            i++;

            int numDone = 0;
            status_t l_status = l_vfdb_handle->GetFields(spVPath, SVD_ST_VISUALGROUP, fieldSpec, i, (int*)&numDone);

            if (l_status == E_NO_ERROR && i == numDone) {
                f_orientation_id_map->insert(std::make_pair(cntV, l_orientation_id));
            }
        }
    }

    return TRUE;
}

//***************************Method Header*************************************
//Method Name   :GetFieldFromRootNode
//Author        :
//Purpose       :Get Field from Root Node
//*****************************************************************************
status_t AbstractAutoLocatorParamSetter ::GetFieldFromRootNode(const VfName_t f_name, const VfType_t f_type,
        void* f_value, const VfSize_t f_size)
{
    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (!l_vfdb_handle) {
        return E_ERROR;
    }

    VfFieldSpec_t vfReq[1] = {0};

    VFF_SET_ARGS(vfReq[0], f_name, f_type, f_size, f_value, VFO_VARIABLE_LEN);

    int num = 0;
    const status_t status = l_vfdb_handle->GetFields(NULL, SVD_ST_STUDY, vfReq, 1, &num);
    return ((status == E_NO_ERROR) && (1 == num)) ? E_NO_ERROR : E_ERROR;
}

//***************************Method Header*************************************
//Method Name   :PutVectorToScratchPad
//Author        :
//Purpose       :Put vector to scratch pad
//*****************************************************************************
RETVAL::SetParamRet_e AbstractAutoLocatorParamSetter::PutVectorToScratchPad(const OriIdMap_t& f_orientation_id_map,
        CVectorTables* const f_vec_tables)
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter::PutVectorToScratchPad");

    if (f_orientation_id_map.empty()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No Orientation ID"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    CVarFieldHandle l_scratchpad_handle(m_scratch_pad.c_str());

    if (E_NO_ERROR != l_scratchpad_handle.VuiOpen()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open Scratch pad"));
        return RETVAL::FAILED_SET_VECTOR;
    }

    const int l_calculated_status = this->GetCalculatedStatus();

    for (OriIdMap_t::const_iterator itr = f_orientation_id_map.begin();
         itr != f_orientation_id_map.end(); ++itr) {
        const int l_vg_id = itr->first;
        int l_ori_id = itr->second;

        if (l_ori_id == VFC_ALA_ORIENTATION_ID_NONE) {
            continue;
        }

        if (!this->IsValidOrientationId(l_ori_id)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid orientation ID."));
            return RETVAL::FAILED_SET_VECTOR;
        }

        if (!IsValidCalculatedStatus(l_ori_id, l_calculated_status)) {
            return RETVAL::INVALID_CALC_STATUS;
        }

        l_ori_id = this->ConvertOrientationIdForVecTable(l_ori_id);

        if (E_NO_ERROR != f_vec_tables->PutFieldsToScratchPad(&l_scratchpad_handle, l_vg_id, l_ori_id)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed PutFieldsToScratchPad()"));
            return RETVAL::FAILED_SET_VECTOR;
        }
    }

    return RETVAL::SUCCESS;
}

//***************************Method Header*************************************
//Method Name   :CreateScratchPad
//Author        :
//Purpose       :Creates the scratch pad
//*****************************************************************************
status_t AbstractAutoLocatorParamSetter ::CreateScratchPad(const int f_protocol) const
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::CreateScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    FILE* l_file_pointer = NULL;

    try {
        l_file_pointer = fopen(m_scratch_pad.c_str(), "a");

    } catch (...) {
        return E_ERROR;
    }

    if (l_file_pointer) {
        fclose(l_file_pointer);
        l_file_pointer = NULL;
    }

    return m_pqmptr->GetStudy()->MaintainScratchPadForPrivateProlim(f_protocol,
            const_cast<char*>(m_scratch_pad.c_str()), SP_CREATE_MODE_PRIVATE_PROLIM);
}

//***************************Method Header*************************************
//Method Name   :DeleteScratchPad
//Author        :
//Purpose       :Deletes the scratch pad
//*****************************************************************************
void AbstractAutoLocatorParamSetter ::DeleteScratchPad() const
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::DeleteScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    WIN32_FIND_DATA FindFileData;

    const CString l_str_filename(m_scratch_pad.c_str());
    HANDLE hFindFile = FindFirstFile(l_str_filename, &FindFileData);

    if (INVALID_HANDLE_VALUE != hFindFile) {
        DeleteFile(l_str_filename);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not find the file for deletion"));
    }

    FindClose(hFindFile);
    hFindFile = NULL;
}

//***************************Method Header*************************************
//Method Name   :UpdateVectorTableToStudy
//Author        :
//Purpose       :Updates the vector table in study
//*****************************************************************************
status_t AbstractAutoLocatorParamSetter ::UpdateVectorTableToStudy(const int f_insert_protocol) const
{
    LPCTSTR FUNC_NAME = _T("AbstractAutoLocatorParamSetter ::UpdateVectorTableToStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (NULL == l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfdb_handle is NULL ..!!"));
        return E_ERROR;
    }

    ClibVfToUiAdapter vuiAdapter(l_vfdb_handle);

    status_t l_status = vuiAdapter.VuiCopyProtocolFromPrivateProlimScratchPad(f_insert_protocol,
                        const_cast<char*>(m_scratch_pad.c_str()), 0, NULL, NULL);

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not Update the Values to the Study file"));
    }

    if (E_ERROR == m_pqmptr->GetStudy()->RestoreSavedTagsAfterScratchpad(f_insert_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("RestoreSavedTagsAfterScratchpad failed to restore SVN tags value in varfield"));
    }

    return l_status;
}

//***************************Method Header*************************************
//Method Name   :CCardiacParamSetter
//Author        :
//Purpose       :Constructor
//*****************************************************************************
CCardiacParamSetter::CCardiacParamSetter(CPqm* f_pqmptr)
    : AbstractAutoLocatorParamSetter(f_pqmptr, AUTOLOCATOR_CARDIAC,
                                     SVN_PLN_CALA_INPUT_PROTOCOL_INDEX, SVT_PLN_CALA_INPUT_PROTOCOL_INDEX,
                                     SVN_PLN_CALA_CALCULATED_STATUS, SVT_PLN_CALA_CALCULATED_STATUS,
                                     SVN_PLN_CALA_OFFSET_VEC_TABLE, SVT_PLN_CALA_OFFSET_VEC_TABLE,
                                     SVN_PLN_CALA_PHASE_VEC_TABLE, SVT_PLN_CALA_PHASE_VEC_TABLE,
                                     SVN_PLN_CALA_SLICE_VEC_TABLE, SVT_PLN_CALA_SLICE_VEC_TABLE,
                                     K_PR_CALA_PLANNED_STATUS, VFC_CALA_PLANNED_STATUS_SCAN_STARTED)
{}

//***************************Method Header*************************************
//Method Name   :~CCardiacParamSetter
//Author        :
//Purpose       :Destructor
//*****************************************************************************
CCardiacParamSetter::~CCardiacParamSetter()
{}

//***************************Method Header*************************************
//Method Name   :GetPlannedStatus
//Author        :
//Purpose       :Get planned status
//*****************************************************************************
BOOL CCardiacParamSetter::GetPlannedStatus(CPqmProtocol* f_protocol)
{
    return f_protocol->GetCalaPlannedStatus();
}

//***************************Method Header*************************************
//Method Name   :IsValidOrientationId
//Author        :
//Purpose       :Validate Orientation ID
//*****************************************************************************
bool CCardiacParamSetter::IsValidOrientationId(const int f_ori_id) const
{
    return (f_ori_id >= 0 && f_ori_id <= 6) ? true : false;
}

//***************************Method Header*************************************
//Method Name   :ConvertOrientationIdForVecTable
//Author        :
//Purpose       :Convert orientation ID for Vector Table index
//*****************************************************************************
int CCardiacParamSetter::ConvertOrientationIdForVecTable(const int f_ori_id) const
{
    return f_ori_id - 1;
}

//***************************Method Header*************************************
//Method Name   :CNeuroParamSetter
//Author        :
//Purpose       :Constructor
//*****************************************************************************
CNeuroParamSetter::CNeuroParamSetter(CPqm* f_pqmptr)
    : AbstractAutoLocatorParamSetter(f_pqmptr, AUTOLOCATOR_NEURO,
                                     SVN_PLN_HALA_INPUT_PROTOCOL_INDEX, SVT_PLN_HALA_INPUT_PROTOCOL_INDEX,
                                     SVN_PLN_HALA_CALCULATED_STATUS, SVT_PLN_HALA_CALCULATED_STATUS,
                                     SVN_PLN_HALA_OFFSET_VEC_TABLE, SVT_PLN_HALA_OFFSET_VEC_TABLE,
                                     SVN_PLN_HALA_PHASE_VEC_TABLE, SVT_PLN_HALA_PHASE_VEC_TABLE,
                                     SVN_PLN_HALA_SLICE_VEC_TABLE, SVT_PLN_HALA_SLICE_VEC_TABLE,
                                     K_PR_HALA_PLANNED_STATUS, VFC_ALA_PLANNED_STATUS_SCAN_STARTED)
{}

//***************************Method Header*************************************
//Method Name   :~CNeuroParamSetter
//Author        :
//Purpose       :Destructor
//*****************************************************************************
CNeuroParamSetter::~CNeuroParamSetter()
{}

//***************************Method Header*************************************
//Method Name   :GetPlannedStatus
//Author        :
//Purpose       :Get planned status
//*****************************************************************************
BOOL CNeuroParamSetter::GetPlannedStatus(CPqmProtocol* f_protocol)
{
    return f_protocol->GetHALAPlannedStatus();
}

//***************************Method Header*************************************
//Method Name   :IsValidOrientationId
//Author        :
//Purpose       :Validate Orientation ID
//*****************************************************************************
bool CNeuroParamSetter::IsValidOrientationId(const int f_ori_id) const
{
    switch (f_ori_id) {
        case VFC_ALA_ORIENTATION_ID_HALA_SG:
        case VFC_ALA_ORIENTATION_ID_HALA_AX:
        case VFC_ALA_ORIENTATION_ID_HALA_CO:
        case VFC_ALA_ORIENTATION_ID_HALA_EXT1:
        case VFC_ALA_ORIENTATION_ID_HALA_EXT2:
        case VFC_ALA_ORIENTATION_ID_HALA_EXT3:
            return true;

        default:
            return false;
    }
}

//***************************Method Header*************************************
//Method Name   :IsValidCalculatedStatus
//Author        :
//Purpose       :Check whether Auto locator calculated data is valid
//*****************************************************************************
bool CNeuroParamSetter::IsValidCalculatedStatus(const int f_ori_id, const int f_calculated_status) const
{
    const int bit_ori_id = 1 << (f_ori_id - VFC_ALA_ORIENTATION_ID_HALA_SG);
    return ((bit_ori_id & f_calculated_status) == bit_ori_id) ? true : false;
}

//***************************Method Header*************************************
//Method Name   :ConvertOrientationIdForVecTable
//Author        :
//Purpose       :Convert orientation ID for Vector Table index
//*****************************************************************************
int CNeuroParamSetter::ConvertOrientationIdForVecTable(const int f_ori_id) const
{
    switch (f_ori_id) {
        case VFC_ALA_ORIENTATION_ID_HALA_SG:
        case VFC_ALA_ORIENTATION_ID_HALA_AX:
        case VFC_ALA_ORIENTATION_ID_HALA_CO:
            return f_ori_id - VFC_ALA_ORIENTATION_ID_HALA_SG;

        case VFC_ALA_ORIENTATION_ID_HALA_EXT1:
        case VFC_ALA_ORIENTATION_ID_HALA_EXT2:
        case VFC_ALA_ORIENTATION_ID_HALA_EXT3:
            return f_ori_id - VFC_ALA_ORIENTATION_ID_HALA_SG - 1;

        default:
            return 0;
    }
}
} // namespace

//***************************Method Header*************************************
//Method Name   :CAutoLocatorParameterSetter
//Author        :
//Purpose       :Constructor
//*****************************************************************************
CAutoLocatorParameterSetter::CAutoLocatorParameterSetter(CPqm* f_pqmptr)
    : m_pqmptr(f_pqmptr)
{

}

//***************************Method Header*************************************
//Method Name   :~CAutoLocatorParameterSetter
//Author        :
//Purpose       :Destructor
//*****************************************************************************
CAutoLocatorParameterSetter::~CAutoLocatorParameterSetter()
{
}

//***************************Method Header*************************************
//Method Name   :SetOffsetParameters
//Author        :
//Purpose       :To set the offset vectors as per orientation id
//*****************************************************************************
BOOL CAutoLocatorParameterSetter::SetOffsetParameters(CPqmProtocol* f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorParameterSetter::SetOffsetParameters");

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return FALSE;
    }

    //call appropriate offset parameter setter
    RETVAL::SetParamRet_e retval = RETVAL::UNKNOWN;

    if (f_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE) {
        CCardiacParamSetter setter(m_pqmptr);
        retval = setter.SetParameters(f_protocol);

    } else if (f_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {
        CNeuroParamSetter setter(m_pqmptr);
        retval = setter.SetParameters(f_protocol);

    } else {
        return TRUE;
    }

    if (retval == RETVAL::SUCCESS) {
        return TRUE;

    } else {
        CString l_msg(_T(""));

        switch (retval) {
            case RETVAL::APP_RUNNING:
                l_msg = _T("ID_CARDIAC_SCAN_ABORT_MSG");
                break;

            case RETVAL::INVALID_CALC_STATUS:
                l_msg = _T("IDS_INVALID_CALCULATED_STATUS");
                break;

            case RETVAL::FAILED_SET_VECTOR:
            case RETVAL::INVALID_INPUT_INDEX:
                l_msg = _T("ID_CARDIAC_SCAN_FAILED_MSG");
                break;

            default:
                return FALSE;
        }

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(l_msg),
            MPlus::EH::Information, FUNC_NAME, _T("PQM"));

        return FALSE;
    }
}

//***************************Method Header*************************************
//Method Name   :GetRefParentInfo
//Author        :
//Purpose       :To get refernced parent protocol CPQMPrMode object
//*****************************************************************************
void CAutoLocatorParameterSetter::GetRefParentInfo(CPqmProtocol* f_protocol, CPqmPrMode* f_parent_info)
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorParameterSetter::GetRefParentInfo");

    if (NULL == f_protocol) {
        return;
    }

    int parent = -1 ;

    if (f_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE) {
        CCardiacParamSetter setter(m_pqmptr);
        parent = setter.GetInputProtocolIndex();

    } else if (f_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {
        CNeuroParamSetter setter(m_pqmptr);
        parent = setter.GetInputProtocolIndex();

    } else {
        return;
    }

    if (parent < 0) {
        return;
    }

    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));
    int rcv_coil_id = 0, anatomy = 0, scan_number = 0;
    float couch_pos = 0.0f, home_pos = 0.0f, couch_offset = 0.0f;
    rcv_coil_id = anatomy = scan_number = 0;
    couch_pos = home_pos = 0.0;
    int l_status = m_pqmptr->GetStudy()->GetProtocolValues(m_pqmptr->GetStudy()->GetVfDBHandle(),
                   parent,
                   mask,
                   K_PR_SCAN_NUMBER,   &scan_number,   NULL,
                   K_PR_COUCH,     &couch_pos, NULL,
                   K_PR_HOME_POSITION, &home_pos,  NULL,
                   K_PR_COUCH_OFFSET,  &couch_offset,  NULL,
                   K_PR_ANATOMY,       &anatomy,   NULL,
                   K_PR_RC_COIL,       &rcv_coil_id,   NULL,
                   NULL);

    CString l_strLog = _T("");
    l_strLog.Format(L"Status of GetProtocolValues is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    f_parent_info->SetScanNumber(scan_number);
    f_parent_info->SetCouchPos(couch_pos);
    f_parent_info->SetHomePos(home_pos);
    f_parent_info->SetCouchOffset(couch_offset);
    f_parent_info->SetAnatomy(anatomy);
    CString tmpStr(_T(""));
    tmpStr.Format(_T("f_parent_info->SetAnatomy(anatomy) = %d"), anatomy);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
    f_parent_info->SetRecvCoilId(rcv_coil_id);

    if (BITFLD_ON(mask, K_PR_SCAN_NUMBER)) {
        f_parent_info->SetScanNumber(0);
    }

    if (BITFLD_ON(mask, K_PR_COUCH)) {
        f_parent_info->SetCouchPos(0.0);
    }

    if (BITFLD_ON(mask, K_PR_HOME_POSITION)) {
        f_parent_info->SetHomePos(0.0);
    }

    if (BITFLD_ON(mask, K_PR_COUCH_OFFSET)) {
        f_parent_info->SetCouchOffset(0.0);
    }

    if (BITFLD_ON(mask, K_PR_ANATOMY)) {
        f_parent_info->SetAnatomy(-1);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_parent_info->SetAnatomy = -1"));
    }

    if (BITFLD_ON(mask, K_PR_RC_COIL)) {
        f_parent_info->SetRecvCoilId(0);
    }
}

