// AMBDialogUtility.h: interface for the CAMBDialogUtility class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_AMB_DLG_UTILITY_H__
#define __SM_AMB_DLG_UTILITY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/pqm/pqmAmbMsg.h>
#include <libStudyManager/NonCopyable.h>

class CAMBDialogUtility
    : private MR::ACQ::SM::COMMON::NonCopyable<CAMBDialogUtility>
{
public:
    explicit CAMBDialogUtility(amb_t& f_amb_status);
    virtual ~CAMBDialogUtility();

    void AmbEditFindFirstStation(const int f_segment, int* f_station) const;
    void ambEditRefreshPrep(int* f_prep, int* f_prepEnable) ;
    void ambEditRefreshAutoVoice() const;
    void ambEditRefreshRepeat() const;

    void AmbEditGetStatusCopy(amb_t* f_status) const;
    void ambEditSetFirstCellPrepEnable(const int  f_prepEnable) ;
    status_t AmbEditSetDirection(const int f_segment, const int f_direction) ;
    status_t AmbEditSetCellRepeat(const int f_segment, const int f_station, const int f_repeat) ;
    status_t AmbEditSetStationOrder(const int f_order) ;
    status_t AmbEditSetShimming(const int f_shimming) ;
    status_t AmbEditSetNumSegment(const int f_segment) ;
    status_t AmbEditSetMap(const int f_map) ;
    status_t AmbEditSetFirstCellPrep(const int f_prep);

    amb_t& GetAMBStatus() const;

protected:


private:

    amb_t& m_amb_status;

    int m_ambAutoVoiceKind;
    int m_ambAutoVoice[AMB_STATION_MAX];

    status_t AmbEditCheckStatus(amb_t* f_status, int* f_codeNo) const;
    status_t ambEditCheckNumStation(const int f_station) const;
    status_t ambEditCheckNumSegment(const int f_segment) const;
    status_t ambEditCheckStationOrder(const int f_order) const;
    status_t ambEditCheckDirection(const int f_segment, const int f_direction) const;
    status_t ambEditCheckShimming(const int f_shimming) const;
    status_t ambEditCheckMap(const int f_map) const;
    status_t ambEditCheckAutoVoiceKind(const int f_autoVoiceKind) const;
    status_t ambEditCheckMapAutoVoice(const int f_station, const int f_autoVoice) const;
    status_t ambEditCheckCellPrepEnable(const int f_segment, const int f_station, const int f_enable) const;
    status_t ambEditCheckCellPrep(const int f_segment, const int f_station, const int f_prep, const int f_enable) const;
    status_t ambEditCheckCellRepeat(const int f_segment, const int f_station, const int f_repeat) const;

    void AmbEditSetStatusCopy(amb_t* f_status) ;
    status_t AmbEditSetNumStation(const int f_station) ;
    status_t AmbEditSetAutoVoiceKind(const int f_autoVoiceKind) ;
    status_t AmbEditSetMapAutoVoice(const int f_station, const int f_autoVoice) ;
    status_t AmbEditSetCellPrepEnable(const int f_segment, const int f_station, const int f_enable) ;
    status_t AmbEditSetCellPrep(const int f_segment, const int f_station, const int f_prep) ;
};

#endif // #ifndef __SM_AMB_DLG_UTILITY_H__
