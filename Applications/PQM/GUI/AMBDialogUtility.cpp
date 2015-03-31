// AMBDialogUtility.cpp: implementation of the CAMBDialogUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AMBDialogUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


/****************************************************************************
 * defines for return values
 ****************************************************************************/
#define AMB_E_NO_ERROR                  E_NO_ERROR
#define AMB_E_BASE                      1
#define AMB_E_ERROR                     (AMB_E_BASE+0)
#define AMB_E_PARAMETER_FAILED          (AMB_E_BASE+1)
#define AMB_E_TOO_MANY_PROTOCOL         (AMB_E_BASE+2)

/****************************************************************************/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAMBDialogUtility::CAMBDialogUtility(amb_t& f_amb_status)
    : m_amb_status(f_amb_status),
      m_ambAutoVoiceKind(0)
{

}

CAMBDialogUtility::~CAMBDialogUtility()
{

}

//************************************Method Header**********************************
// Method Name  : AmbEditCheckStatus
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditCheckStatus(amb_t* f_status, int* f_codeNo) const
{




    *f_codeNo = 0;
    (*f_codeNo)++; /* 1 */

    if (ambEditCheckNumStation((int)f_status->numStation) != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

    (*f_codeNo)++; /* 2 */

    if (ambEditCheckNumSegment((int)f_status->numSegment) != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

    (*f_codeNo)++; /* 3 */

    if (ambEditCheckStationOrder((int)f_status->stationOrder) != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

    (*f_codeNo)++; /* 4 */

    int  l_segmentIndex = 1;

    for (; l_segmentIndex <= AMB_SEGMENT_MAX ; l_segmentIndex++) {
        if (ambEditCheckDirection(l_segmentIndex, (int)f_status->direction[l_segmentIndex - 1]) != AMB_E_NO_ERROR)
            return AMB_E_PARAMETER_FAILED;
    }

    (*f_codeNo)++; /* 5 */

    if (ambEditCheckShimming((int)f_status->shimming) != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

    (*f_codeNo)++; /* 6 */

    if (ambEditCheckMap((int)f_status->map) != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

    (*f_codeNo)++; /* 7 */

    int  l_stationIndex = 1;

    for (; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {
        if (ambEditCheckAutoVoiceKind((int)f_status->mapAutoVoice[l_stationIndex - 1]) != AMB_E_NO_ERROR)
            return AMB_E_PARAMETER_FAILED;
    }

    (*f_codeNo)++; /* 8 */

    for (l_segmentIndex = 1; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {
        for (l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {
            if (ambEditCheckCellPrepEnable(l_segmentIndex, l_stationIndex,
                                           (int)f_status->cell[l_segmentIndex - 1][l_stationIndex - 1].prepEnable)
                != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

            if (ambEditCheckCellPrep(l_segmentIndex, l_stationIndex,
                                     (int)f_status->cell[l_segmentIndex - 1][l_stationIndex - 1].prep,
                                     (int)f_status->cell[l_segmentIndex - 1][l_stationIndex - 1].prepEnable)
                != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;

            if (ambEditCheckCellRepeat(l_segmentIndex, l_stationIndex,
                                       (int)f_status->cell[l_segmentIndex - 1][l_stationIndex - 1].repeat)
                != AMB_E_NO_ERROR) return AMB_E_PARAMETER_FAILED;
        }
    }

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : ambEditCheckNumStation
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckNumStation(const int f_station) const
{


    if (f_station < AMB_STATION_MIN || f_station > AMB_STATION_MAX) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckNumSegment
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckNumSegment(const int f_segment) const
{


    if (f_segment < AMB_SEGMENT_MIN || f_segment > AMB_SEGMENT_MAX) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckStationOrder
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckStationOrder(const int f_order) const
{


    if (f_order != AMB_COUCH_OUTSIDE && f_order != AMB_COUCH_INSIDE) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckDirection
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckDirection(const int   f_segment,
        const int   f_direction) const
{


    if (f_segment < 1 || f_segment > AMB_SEGMENT_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_direction != AMB_COUCH_OUTSIDE && f_direction != AMB_COUCH_INSIDE) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckShimming
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckShimming(const int  f_shimming) const
{


    if (f_shimming != OFF && f_shimming != ON) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckMap
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckMap(const int f_map)const
{


    if (f_map != OFF && f_map != ON) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckAutoVoiceKind
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckAutoVoiceKind(const int f_autoVoiceKind)const
{



    if (f_autoVoiceKind != VFC_AUTO_VOICE_NONE &&
        f_autoVoiceKind != VFC_AUTO_VOICE_BREATH1 &&
        f_autoVoiceKind != VFC_AUTO_VOICE_BREATH2 &&
        f_autoVoiceKind != VFC_AUTO_VOICE_BREATH3 &&
        f_autoVoiceKind != VFC_AUTO_VOICE_MOTION) {
        return AMB_E_PARAMETER_FAILED;
    }

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckMapAutoVoice
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckMapAutoVoice(const int f_station,
        const int f_autoVoice)const
{



    if (f_station < 1 || f_station > AMB_STATION_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_autoVoice != OFF && f_autoVoice != ON) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckCellPrepEnable
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckCellPrepEnable(const int f_segment,
        const int  f_station,
        const int  f_enable) const
{




    if (f_segment < 1 || f_segment > AMB_SEGMENT_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_station < 1 || f_station > AMB_STATION_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_enable != ENABLE && f_enable != DISABLE) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckCellPrep
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckCellPrep(const int f_segment,
        const int f_station,
        const int f_prep,
        const int f_enable) const
{



    if (f_segment < 1 || f_segment > AMB_SEGMENT_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_station < 1 || f_station > AMB_STATION_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_prep != OFF && f_prep != ON) return AMB_E_PARAMETER_FAILED;

    if (f_enable == ON && f_enable == DISABLE) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : ambEditCheckCellRepeat
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::ambEditCheckCellRepeat(const int f_segment,
        const int  f_station,
        const int  f_repeat) const
{



    if (f_segment < 1 || f_segment > AMB_SEGMENT_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_station < 1 || f_station > AMB_STATION_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_repeat < AMB_REPEAT_MIN || f_repeat > AMB_REPEAT_MAX) return AMB_E_PARAMETER_FAILED;

    if (f_segment == 1 && f_repeat != 1) return AMB_E_PARAMETER_FAILED;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetNumStation
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetNumStation(const int f_station)
{



    if (ambEditCheckNumStation(f_station) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    const int32_t l_oldStation = m_amb_status.numStation;
    m_amb_status.numStation = (int32_t)f_station;

    /*if(f_station > l_oldStation) {
        if(ambEditCheckAdditionProtocol(&AmbStatus) == AMB_E_TOO_MANY_PROTOCOL) {
            AmbStatus.numStation = oldStation;
            return AMB_E_TOO_MANY_PROTOCOL;
        }
    }*/

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetNumSegment
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetNumSegment(const int   f_segment)
{


    if (ambEditCheckNumSegment(f_segment) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    const int32_t l_oldSegment = m_amb_status.numSegment;
    m_amb_status.numSegment = (int32_t)f_segment;

    /*if(f_segment > oldSegment) {
        if(ambEditCheckAdditionProtocol(&AmbStatus) == AMB_E_TOO_MANY_PROTOCOL) {
            AmbStatus.numSegment = oldSegment;
            return AMB_E_TOO_MANY_PROTOCOL;
        }
    }*/

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetStationOrder
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetStationOrder(const int f_order)
{


    if (ambEditCheckStationOrder(f_order) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_amb_status.stationOrder = (int32_t)f_order;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetDirection
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetDirection(const int f_segment,
        const int    f_direction)
{


    if (ambEditCheckDirection(f_segment, f_direction) ==  AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_amb_status.direction[f_segment - 1] = (int32_t)f_direction;

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetShimming
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetShimming(const int f_shimming)
{



    if (ambEditCheckShimming(f_shimming) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    const int32_t l_oldShimming = m_amb_status.shimming;
    m_amb_status.shimming = (int32_t)f_shimming;

    /*if(f_shimming == ON) {
        if(ambEditCheckAdditionProtocol(&AmbStatus) == AMB_E_TOO_MANY_PROTOCOL) {
            AmbStatus.shimming = oldShimming;
            return AMB_E_TOO_MANY_PROTOCOL;
        }
    }*/

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetMap
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetMap(const int  f_map)
{

    if (ambEditCheckMap(f_map) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    const int32_t l_oldMap = m_amb_status.map;
    m_amb_status.map = (int32_t)f_map;

    /*if(f_map == ON) {
        if(ambEditCheckAdditionProtocol(&AmbStatus) == AMB_E_TOO_MANY_PROTOCOL) {
            AmbStatus.map = oldMap;
            return AMB_E_TOO_MANY_PROTOCOL;
        }
    }*/

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetAutoVoiceKind
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetAutoVoiceKind(const int f_autoVoiceKind)
{


    if (ambEditCheckAutoVoiceKind(f_autoVoiceKind) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_ambAutoVoiceKind = f_autoVoiceKind;

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetMapAutoVoice
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetMapAutoVoice(const int f_station,
        const int f_autoVoice)
{


    if (ambEditCheckMapAutoVoice(f_station, f_autoVoice) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_ambAutoVoice[f_station - 1] = f_autoVoice;

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetCellPrepEnable
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetCellPrepEnable(const int f_segment,
        const int f_station,
        const int f_enable)
{


    if (ambEditCheckCellPrepEnable(f_segment, f_station, f_enable) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_amb_status.cell[f_segment - 1][f_station - 1].prepEnable = (int32_t)f_enable;

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetCellPrep
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetCellPrep(const int f_segment,
        const int f_station,
        const int f_prep)
{


    if (ambEditCheckCellPrep(f_segment, f_station, f_prep, m_amb_status.cell[f_segment - 1][f_station - 1].prepEnable)
        == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    m_amb_status.cell[f_segment - 1][f_station - 1].prep = (int32_t)f_prep;

    return AMB_E_NO_ERROR;
}
//************************************Method Header**********************************
// Method Name  : AmbEditSetFirstCellPrep
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetFirstCellPrep(const int f_prep)
{


    const int l_segment = 2;
    int l_station = 0;
    AmbEditFindFirstStation(l_segment, &l_station);

    int l_segmentIndex = 1;
    int l_stationIndex = 1;

    status_t l_result = AMB_E_NO_ERROR;

    for (; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {
        for (l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {

            if (l_segmentIndex == l_segment && l_stationIndex == l_station) {
                l_result = AmbEditSetCellPrep(l_segmentIndex, l_stationIndex, f_prep);

            } else {
                l_result = AmbEditSetCellPrep(l_segmentIndex, l_stationIndex, OFF);
            }
        }
    }

    return l_result;
}
//************************************Method Header**********************************
// Method Name  : AmbEditFindFirstStation
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::AmbEditFindFirstStation(const int  f_segment, int* f_station) const
{

    const int l_firstStation = 1;
    const int l_lastStation = (int)m_amb_status.numStation;

    const int32_t l_stationOrder = m_amb_status.stationOrder;
    const int32_t l_direction = m_amb_status.direction[f_segment - 1];

    if (l_stationOrder == AMB_COUCH_OUTSIDE) {
        if (l_direction == AMB_COUCH_OUTSIDE) {
            *f_station = l_firstStation;

        } else {
            *f_station = l_lastStation;
        }

    } else {
        if (l_direction == AMB_COUCH_OUTSIDE) {
            *f_station = l_lastStation;

        } else {
            *f_station = l_firstStation;
        }
    }
}


//************************************Method Header**********************************
// Method Name  : AmbEditSetCellRepeat
// Author       : iGATE
// Purpose      :
//***********************************************************************************
status_t CAMBDialogUtility::AmbEditSetCellRepeat(
    const int   f_segment,
    const int   f_station,
    const int   f_repeat)
{


    if (ambEditCheckCellRepeat(f_segment, f_station, f_repeat) == AMB_E_PARAMETER_FAILED) return AMB_E_PARAMETER_FAILED;

    const int32_t l_oldRepeat = m_amb_status.cell[f_segment - 1][f_station - 1].repeat;
    m_amb_status.cell[f_segment - 1][f_station - 1].repeat = (int32_t)f_repeat;

    /*if(f_repeat > oldRepeat) {
        if(ambEditCheckAdditionProtocol(&AmbStatus) == AMB_E_TOO_MANY_PROTOCOL) {
            AmbStatus.cell[f_segment-1][f_station-1].repeat = oldRepeat;
            return AMB_E_TOO_MANY_PROTOCOL;
        }
    }*/

    return AMB_E_NO_ERROR;
}

//************************************Method Header**********************************
// Method Name  : AmbEditSetStatusCopy
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::AmbEditSetStatusCopy(amb_t* f_status)
{


    if (f_status == NULL) return;

    memcpy(&m_amb_status, f_status, sizeof(amb_t));

    int l_autoVoice = VFC_AUTO_VOICE_NONE;
    int l_firstFlag = 1;

    for (int l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {
        l_autoVoice = f_status->mapAutoVoice[l_stationIndex - 1];

        if (l_autoVoice != VFC_AUTO_VOICE_NONE) {
            m_ambAutoVoice[l_stationIndex - 1] = ON;

            if (l_firstFlag == 1) {
                m_ambAutoVoiceKind = l_autoVoice;
                l_firstFlag = 0;
            }
        }
    }
}
//************************************Method Header**********************************
// Method Name  : AmbEditGetStatusCopy
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::AmbEditGetStatusCopy(amb_t* f_status) const
{



    if (f_status == NULL) return;

    memcpy(f_status, &m_amb_status, sizeof(amb_t));
}
//************************************Method Header**********************************
// Method Name  : GetAMBStatus
// Author       : iGATE
// Purpose      :
//***********************************************************************************
inline amb_t& CAMBDialogUtility::GetAMBStatus() const
{
    return m_amb_status;
}
//************************************Method Header**********************************
// Method Name  : ambEditSetFirstCellPrepEnable
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::ambEditSetFirstCellPrepEnable(const int  f_prepEnable)
{


    const int l_segment = 2;
    int l_station;
    AmbEditFindFirstStation(l_segment, &l_station);

    m_amb_status.cell[l_segment - 1][l_station - 1].prepEnable = f_prepEnable;
}
//************************************Method Header**********************************
// Method Name  : ambEditRefreshPrep
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::ambEditRefreshPrep(int* f_prep,
        int* f_prepEnable)
{



    *f_prep = OFF;
    *f_prepEnable = DISABLE;

    for (int l_segmentIndex = 1; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {
        for (int l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {

            if (m_amb_status.cell[l_segmentIndex - 1][l_stationIndex - 1].prepEnable == ENABLE) {
                *f_prepEnable = ON;

                if (m_amb_status.cell[l_segmentIndex - 1][l_stationIndex - 1].prep == ON) {
                    *f_prep = ON;
                }
            }

            AmbEditSetCellPrep(l_segmentIndex, l_stationIndex, OFF);
            AmbEditSetCellPrepEnable(l_segmentIndex, l_stationIndex, DISABLE);
        }
    }

    ambEditSetFirstCellPrepEnable(*f_prepEnable);
    AmbEditSetFirstCellPrep(*f_prep);
}

//************************************Method Header**********************************
// Method Name  : ambEditRefreshAutoVoice
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::ambEditRefreshAutoVoice() const
{



    for (int l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {
        if (m_ambAutoVoice[l_stationIndex] == ON) {
            m_amb_status.mapAutoVoice[l_stationIndex] = m_ambAutoVoiceKind;

        } else {
            m_amb_status.mapAutoVoice[l_stationIndex] = VFC_AUTO_VOICE_NONE;
        }
    }
}
//************************************Method Header**********************************
// Method Name  : ambEditRefreshRepeat
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CAMBDialogUtility::ambEditRefreshRepeat() const
{



    for (int l_segmentIndex = 1; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {
        for (int l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {

            if (l_segmentIndex > m_amb_status.numSegment || l_stationIndex > m_amb_status.numStation) {
                m_amb_status.cell[l_segmentIndex - 1][l_stationIndex - 1].repeat = 1;
            }
        }
    }
}