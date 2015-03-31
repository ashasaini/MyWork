//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
// PqmCoilData.h: interface for the CPqmCoilData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMCOILDATA_H__
#define __SM_IPCLIB_PQMCOILDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/CoilDefinitions.h>
#include "ICoilData.h"

class CCoil;

class CPqmCoilData : public ICoilData
{

public:

    CPqmCoilData();

    virtual ~CPqmCoilData();


    void AddData(void* f_key, cdb_rec_p f_db_rec_p);
    void CoilMaintain();


    status_t PqmIsSameCoilInSaturn(PqmSaturnCoil_t* f_coil_a, PqmSaturnCoil_t* f_coil_b, PqmSaturnRcvCoil_t* f_tbl_a, PqmSaturnRcvCoil_t* f_tbl_b);
    bool PqmIsSameCoilInCDS(PqmSaturnCoil_t* f_coil_cur, PqmSaturnCoil_t* f_coil_last, PqmSaturnRcvCoil_t* const f_tbl_cur, PqmSaturnRcvCoil_t* const f_tbl_last) const;

    status_t PqmInstallCheckSaturn(VftCoilListInfo_t* const  coillist)const;
    bool_t PqmCoilIsDegeneracy(const int f_num_Section, VftSectionRecvrCoil_t	f_recvr_Coil[])const;
    status_t PqmIsSameCoilInSaturnforSPEEDER(PqmSaturnCoil_t* coil_a, PqmSaturnCoil_t* coil_b)const;


    status_t PqmCoilGetSaturnSectionMax(VftSectionRecvrCoil_t* f_recvrCoil, int* max_sec);
    PqmCoil_t* PqmGetCDSRWBInSaturn(VftCoilListInfo_t* f_coillist, int f_mask) ;

    bool GetWBCoil(CCoil& f_wb_coil);

    //*******************************************************
    //      Coil Related Functions.
    //*******************************************************
    bool PqmCoilGetCoil(const int f_coil_id, CCoil& f_coil)const;

    bool PqmCoilGetCoilGr(const int f_coil_id, CCoil& f_coil)const;

    bool PqmCoilGetCoilRWB(const int f_coil_id, CCoil& f_coil)const;

    bool PqmCoilGetEnableCoil(const int f_coil_id, CCoil& f_coil)const;

    bool PqmCoilGetTxCoil(const int f_rcvCoil_id, CCoil& f_coil)const;

    //*******************************************************
    //      Saturn Coil Related Functions.
    //*******************************************************

    bool PqmCoilGetSaturnCoil(VftSectionRecvrCoil_t*	const f_coil, CCoil& f_satrun_coil)const;

    bool PqmCoilGetSaturnCoilGr(VftSectionRecvrCoil_t* const 	f_coil, CCoil& f_satrun_coil)const;

    bool PqmCoilGetSaturnCoilRWB(VftSectionRecvrCoil_t* const 	f_coil, CCoil& f_satrun_coil)const;

    bool PqmCoilGetSaturnEnableCoil(VftSectionRecvrCoil_t* const 	f_coil, CCoil& f_satrun_coil)const;

    bool PqmCoilGetSaturnTxCoil(VftSectionRecvrCoil_t*  const f_coil, CCoil& f_satrun_coil)const;

    status_t PqmCoilGetSaturnCombine(VftCoilListInfo_t* const  f_coilList, int*	f_combine)const;

    bool PqmCoilidGetSaturnCoil(const int  f_coilid, const char f_port,
                                const char f_channel, CCoil& f_satrun_coil)const;

    status_t PqmCoilGetSaturnSection(VftSectionRecvrCoil_t*	 f_recvrCoil,	// Coil Information
                                     int*		 f_sec_cnt, sedb_rec_t**	 f_sec_p)const;

    status_t PqmAllCoilGetSaturn(PqmSaturnCoil_t*     f_coil, PqmSaturnRcvCoil_t* const  f_coil_tbl)const;

    int PqmCoilGetMinChannel(const int f_num_Section, VftSectionRecvrCoil_t f_recvr_Coil[])const;

    status_t PqmGetRflTuneInSaturn(PqmSaturnRcvCoil_t* const f_coil_t, int* f_rfl_limit, int* f_tune_limit)const;

    bool GetRWBInSaturn(PqmSaturnRcvCoil_t* const f_saturn_recv_coil, const int f_mask, CCoil& f_ccoil)const;

    int GetInOutSideLimit(PqmSaturnRcvCoil_t* const f_saturn_recv_coil, int* f_in_limit, int* f_out_limit)const;

    status_t GetSpeederInSaturn(PqmSaturnRcvCoil_t* const coil_t, const int mask)const;
    status_t PqmCoilGetPathDB(const int key , path_db_rec_p p)const;
    static cdb_coil_port_t GetWholeBodyPort();

private:

    PqmCoil_t*   m_wb_coil;

};

#endif // #ifndef __SM_IPCLIB_PQMCOILDATA_H__ 