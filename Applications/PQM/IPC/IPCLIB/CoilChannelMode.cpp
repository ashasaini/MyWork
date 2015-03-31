// CoilChannelMode.cpp: implementation of the CCoilChannelMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoilChannelMode.h"
#include <math.h>
#include <PQM/PqmUtilities.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <PQM/PQMLogMgr.h>
#include "PQMStudy.h"
#include "PqmShimSharedData.h"
#include "PqmProtocol.h"
#include "VarFieldHandle.h"
#include "ScanProcedure.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "DataManager.h"

using namespace MR::ACQ::SM::COMMON;
using namespace MPlus::EH;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoilChannelMode::CCoilChannelMode(CPQMStudy* f_pqm_studyptr) :
    m_pqm_studyptr(f_pqm_studyptr),
    m_system_enable_channel(0),
    m_limited_channel(0),
    m_use_coil_channel_mode(FALSE)
{

}

CCoilChannelMode::~CCoilChannelMode()
{

}

//****************************Method Header************************************
//Method Name   :GetNumSignalfromSelectedSection()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCoilChannelMode::GetNumSignalfromSelectedSection(const int f_selected_item, VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::GetNumSignalfromSelectedSection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pdata pointer is NULL"));
        return;
    }

    CPqmProtocol* l_curr_protocol = m_pqm_studyptr->GetProtocol(f_selected_item);
    f_pdata->intVal = -1;

    if (NULL == l_curr_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_protocol is NULL"));
        return;
    }

    PqmSaturnCoil_t* l_saturn_coil =  l_curr_protocol->GetSaturnCoil();

    if (NULL == l_saturn_coil) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_saturn_coil pointer is NULL"));
        return;
    }

    int l_total_num_signal = 0;

    for (int j = 0 ; j < l_saturn_coil->numSection; j++) {
        l_total_num_signal += l_saturn_coil->Signal[j].num;
    }

    f_pdata->intVal = l_total_num_signal;
}

//****************************Method Header************************************
//Method Name   :UpdateCoil()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
int CCoilChannelMode::UpdateCoil(const int f_prot, const int f_numchannels)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::UpdateCoil");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_update_coil_status = -1;


    CPqmProtocol* pr_obj_p = m_pqm_studyptr->GetProtocol(f_prot);

    if (pr_obj_p == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol pointer is NULL"));
        return l_update_coil_status;
    }



    PqmSaturnCoil_t* l_saturn_coil = pr_obj_p->GetSaturnCoil();
    CDataManager* l_data_mgr = CDataManager::GetInstance();

    if (NULL == l_data_mgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Data Manager pointer is NULL"));
        return l_update_coil_status;
    }

    DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
    DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();

    if ((NULL == l_saturn_coil) || (NULL == l_coil_head) || (NULL == l_section_head)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_saturn_coil || l_coil_head || l_section_head pointer is NULL"));
        return l_update_coil_status;
    }

    AcqGetcoilSignal(f_numchannels, l_saturn_coil->numSection,
                     l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                     l_coil_head, l_section_head);

    l_update_coil_status = m_pqm_studyptr->MinimumChannelCheck(l_saturn_coil->numSection, l_saturn_coil->CoilList.num, l_saturn_coil->RcvCoil, l_saturn_coil->Signal);

    if (TRUE == l_update_coil_status) {
        m_pqm_studyptr->PutSaturnCoil(m_pqm_studyptr->GetVfDBHandle(), f_prot, l_saturn_coil);
        pr_obj_p->SetSaturnCoil(l_saturn_coil);
    }

    return l_update_coil_status;
}
//****************************Method Header************************************
//Method Name   :GetChannelModeSelection()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCoilChannelMode::GetChannelModeSelection(const int f_selected_item, VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::GetChannelModeSelection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_coilchannelmode = -1;

    if (NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol pointer or f_pdata pointer is NULL"));
        return;
    }

    //CPqmProtocol* l_curr_protocol = m_pqm_studyptr->GetItemFromPosition(f_selected_item +1); // This method returns NULL if we pass 0, this reused as it was earlier implementation
    CPqmProtocol* l_curr_protocol = m_pqm_studyptr->GetProtocol(f_selected_item);

    if ((NULL == l_curr_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol pointer is NULL"));
        f_pdata->intVal = -1;
        return;
    }

    BITFLD_DECL(mask, K_ALL_PR) = {0};
    status_t l_status = m_pqm_studyptr->GetProtocolValues(m_pqm_studyptr->GetVfDBHandle(),
                        l_curr_protocol->GetProtocol(),
                        mask,
                        K_PR_MAX_COIL_CHANNEL_MODE,
                        &l_coilchannelmode,
                        NULL,
                        NULL);

    if (E_ERROR == l_status) {
        f_pdata->intVal = -1;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Channel Mode tag is not present"));
        return;
    }

    //l_coilchannelmode = l_curr_protocol->GetcoilChannelMode();
    f_pdata->intVal = l_coilchannelmode;
}

//****************************Method Header************************************
//Method Name   :SetChannelModeSelection()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCoilChannelMode::SetChannelModeSelection(const int f_selected_item, BOOL f_coilchannelmode, CPqmProtocol* f_prot_obj) // Default argument is for study reload case
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::SetChannelModeSelection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (f_selected_item < 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Incorrect acq_order"));
        return;
    }

    BITFLD_DECL(mask, K_ALL_PR) = {0};
    CVarFieldHandle* l_vf_handle = m_pqm_studyptr->GetVfDBHandle();

    if (NULL == l_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
        return;
    }

    CPqmProtocol* l_prot_obj = f_prot_obj;

    if (NULL == l_prot_obj) {
        l_prot_obj = m_pqm_studyptr->GetProtocol(f_selected_item);

        if (NULL == l_prot_obj) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol object is NULL"));
            return;
        }
    }

    BOOL l_update_coil_status = UpdateCoil(l_prot_obj->GetProtocol(), f_coilchannelmode ? GetSystemEnableChannel() : GetLimitedChannel());

    if (TRUE == l_update_coil_status) {
        //If minimum signal check is ok, check for min channel count
        VARIANT l_pdata;
        GetNumSignalfromSelectedSection(l_prot_obj->GetProtocol(), &l_pdata);

        if (l_pdata.intVal > m_limited_channel) {
            l_update_coil_status = FALSE;
        }
    }

    if ((FALSE == l_update_coil_status) && (CHANNEL_MODE_NORMAL == f_coilchannelmode)) {
        f_coilchannelmode = CHANNEL_MODE_MAX;
    }

    m_pqm_studyptr->SetProtocolValues(l_vf_handle,
                                      f_selected_item,
                                      mask,
                                      K_PR_MAX_COIL_CHANNEL_MODE,
                                      &f_coilchannelmode,
                                      sizeof(f_coilchannelmode),
                                      NULL);

    l_prot_obj->SetcoilChannelMode(f_coilchannelmode);
    l_prot_obj->SetSystemChannels(f_coilchannelmode ? GetSystemEnableChannel() : GetLimitedChannel());



}
//****************************Method Header************************************
//Method Name   :SetNumChannelsforChannelMode()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
void CCoilChannelMode::SetNumChannelsforChannelMode(const BOOL f_use_coil_channel_mode, const int f_system_enable_channel, const int f_limited_channel)
{
    m_use_coil_channel_mode = f_use_coil_channel_mode;
    m_system_enable_channel = f_system_enable_channel;
    m_limited_channel = f_limited_channel;
}

int CCoilChannelMode::GetSystemEnableChannel() const
{
    return m_system_enable_channel;
}

int CCoilChannelMode::GetLimitedChannel() const
{
    return m_limited_channel;
}

//****************************Method Header************************************
//Method Name   :IsCoilChannelModeApplicable()
//Author        :PATNI / Pavan
//Purpose       :
//*****************************************************************************
BOOL CCoilChannelMode::IsCoilChannelModeApplicable() const // 21-Oct-11
{
    return m_use_coil_channel_mode;
}