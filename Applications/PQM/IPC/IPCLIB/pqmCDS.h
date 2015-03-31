//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
// pqmCDS.h: interface for the CpqmCDS class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMCDS_H__
#define __SM_IPCLIB_PQMCDS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <tami/libvf/varfields.h>
#include <libStudyManager/NonCopyable.h>

//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include <PQM/CDSCommonStructures.h>
#include <PQM/CoilDefinitions.h>
#include "pqmCDSDebug.h"

class CPqm;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345
struct CpqmCDSInfo;

typedef struct PqmAcqStatic_s {

    /* CDS-time information on the scan  */
    PqmSaturnCoil_t		m_lastCdsCoil;				/* Coil */
    PqmSaturnRcvCoil_t	m_lastCdstbl;					/* CoilDBtbl */
    float				m_lastCdscouchPos;			/* Sleeping position */
    int					m_lastCdsscanNumber;			/* Floodlight aircraft */

    PqmAcqStatic_s() :  m_lastCdscouchPos(0), m_lastCdsscanNumber(0) {
        memset(&m_lastCdsCoil, 0, sizeof(m_lastCdsCoil));
        memset(&m_lastCdstbl, 0, sizeof(m_lastCdstbl));
    }

} PqmAcqStatic;

enum CDSCheckCoilEnum {CDSCheckCoil_None = 0, CDSCheckCoil_Level1, CDSCheckCoil_Level2 } ;

enum CDS_COIL_SEL_STATUS {
    CDS_NONE,		//CDS in the confirmation was not
    CDS_ON ,        //CDS is being reviewed after
    CDS_WAIT        //After waiting to see CDS (timer)
};


class CpqmCDS : private MR::ACQ::SM::COMMON::NonCopyable<CpqmCDS>
{

public:
    //Constrcutor / Destrcutor
    CpqmCDS();
    virtual ~CpqmCDS();

    bool PqmCdsInit(CPqm* f_pqm) ;
    void PqmCdsSetAutoIn(const float f_dist);
    int  PqmCdsCheckCoil(VftCoilListInfo_t* const f_coillist)const;
    bool PqmCdsSetListCoil(CPqmProtocol* const f_pqmprotocol) const;
    void pqmUiDebugSetCdsCB(COMPASS_Debugg_Param* f_compass_debug_param) ;
    void pqmUiDebugCancelCdsCB(COMPASS_Debugg_Param* f_compass_debug_param) ;
    void OnOffCDSDebug(const BOOL f_OnOff);
    void PqmCdsReturnCoil(PqmSaturnCoil_t* coil);
    void PqmCdsBackUpCoil(PqmSaturnCoil_t* f_coil);


    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    int  PqmCdsChangeCouch(const float f_couch) const;
    void PqmCdsSetCoordinatetbl(Vft_CdsCoilLocation_t* const f_locationtbl, int32_t* f_coordinate, const float f_couch)const ;
    void PqmCdsAdjustCoilPosition(CPqmProtocol* const f_curObj);
    void PqmCdsPositionForCoilSel(CPqmProtocol* const f_curObj);
    //Patni-MP/2010Dec02/Modified/REDMINE-932
    int  PqmCdsScanLastCheck(CPqmProtocol* const f_current_obj, const bool f_check_for_fixed_coils = false) ;

    bool CopyCDSCCoilInfoAsPerBatchSetting(CPqmProtocol* f_cds_protocol) const;
    bool CopyCoilInfoFrom(CPqmProtocol* src, CPqmProtocol* dest) const;

    bool PqmUiCdsDebug() const;
    void PqmUiCdsDebugWindowCB(CPqmProtocol* const f_protocol_obj);
    void PqmCoilSelEnd(CPqmProtocol* const f_pmObj);
    void PqmPmCoilSelAfterAutoCoil(const int f_prot);
    void pqmPmCoilSelCdsTimeOut();


    status_t PqmCoilCDSAutoCoilSet(CPqmProtocol* const curObj)const ;

    sedb_rec_t* GetSectionDataForSectionID(const int f_coilid, const int f_sectionid) const;
    bool CheckForLinkedSections(PqmSaturnCoil_t* f_saturn_coil, int& f_total_coils, CpqmCDSInfo* cdsInfo) const;
    status_t PqmCoilSetCDSCoil(CVarFieldHandle* const vf_handle, CPqmProtocol* const prObj)const ;


    void SetCSConfigParams(const sCSConfigParams& f_csconfigparams);

    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_18
    void GetASCOMPASSParameter(CDSCompassSettingParams* f_cdscompasssetting_param) const;
    void SetASCOMPASSParameter(CDSCompassSettingParams* f_cdscompasssetting_param) ;

    int GetCDSCheckCoilFlag() const;
    void SetCDSCheckCoilFlag(const int f_flag);


    CPqmProtocol* GetCurrentCDSProtocol()const;
    CDS_COIL_SEL_STATUS GetCDSCoilSelStatus()const;
    PqmAcqStatic* GetPqmAcqStatic();
    void  GetCDSDebugParam(COMPASS_Debugg_Param* f_compass_debug_param) const;
    float GetMagneticCenterPosForCoilSel() const ;

    void SetCDSCoilSelStatus(const CDS_COIL_SEL_STATUS f_cds_coil_sel_status);
    int GetCoilLableSize()const;

private:

    CPqm*        m_pqm ;							//main pqm pointer to get otehr data and interect with others
    CpqmCDSDebug m_cds_debug ;					//Performs CDS debug related functions

    CDSParamStruct m_cds_param ;
    CDSCompassSettingParams m_compass_setting_param ;		//AS-compass setting params

    PqmSaturnCoil_t m_cdsbackup_coil ;			//back up coil, to be used while CDS abort

    /* CDS Auto check after coil V9.50 */
    int         m_cds_CheckCoil ;						//weather to confirm with COIL dialog or not for CDS
    PqmAcqStatic m_PqmAcqStatic;
    CDS_COIL_SEL_STATUS m_pqmPmCoilSelCDSFlg;


    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    float PqmCdsGetCenterPos(float	f_couch) const;
    void  OnCDSDebugCompleted()const;
    int   pqmCdsChangeCoordinate(const int f_location, const float f_couch) const;
    void  pqmCdsSearchOverlapCoil(CpqmCDSInfo* f_cdsInfo, const int f_section_p, int* f_left, int32_t* f_distl, int* f_right, int32_t* f_distR) const;
    void  pqmPmCoilSelCdsTimerSetUp()const;
    void  pqmPmCoilSelCdsTimerReset();
    void  pqmCdsSort(int* f_index, int32_t* f_locationtbl, const int f_num);

    bool CanIgnoreCDSExecution(CPqmProtocol* f_current_protocol, CPqmProtocol* f_src_protocol, int& f_copy_coil_from, const bool f_check_for_fixed_coils = false) const;

    CpqmCDSDebug* GetCDSDebug();
    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    int GetCDSRange() const;
    int GetCDSRatio() const;


    void SetCDSDebugFlag(const bool f_cdsdebug_flag) ;


    int GetCoilindex(int f_coil_id, CpqmCDSInfo* cdsInfo) const;
    bool IsGroupedCoil(int f_coil_id, int f_linked_coil_id, CpqmCDSInfo* cdsInfo) const;
    bool IsLinkedSectionOfGroupedCoil(sedb_rec_t* section_data, sedb_rec_t* linked_section_data, CpqmCDSInfo* cdsInfo) const;
    bool IsLinkedSectionOfSameCoil(sedb_rec_t* section_data, sedb_rec_t* linked_section_data)const;
};

#endif // #ifndef __SM_IPCLIB_PQMCDS_H__ 
