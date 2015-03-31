//Redmine-783
//Redmine-774
//Redmine-780
//Redmine-782
// PqmMRSScan1.h: interface for the CPqmMRSScan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMMRSSCAN_H__
#define __SM_IPCLIB_PQMMRSSCAN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"
#include "PqmLocalShimming.h"
extern "C" {
#include <tami/MRS/mrsTate.h>
}
static struct {
    VfName_t	name;
    VfType_t	type;
    void*	shim;
    int		shimSize;
} shimTbl[] = {
#define DEF(n)	{ SVN_SEQ_##n, SVT_SEQ_##n,NULL,0}
    DEF(SHIM_GRAD_OFFSET),
    DEF(AXI_FSY_OFFSET),
    DEF(SAG_FSY_OFFSET),
    DEF(COR_FSY_OFFSET),
    DEF(SHIM_FSY_FLAG),
};

class CPreScan;
class CPqmProtocol;		//Patni-Hemant/03May2011/Added/IR-168_Part3

class CPqmMRSScan : public CScanTransaction
{
public:
    //Constrcutor / Destructor
    explicit CPqmMRSScan(CScanProcedure* scan_proc);
    virtual ~CPqmMRSScan();


    int  InitMRS();

    int  GetDefaultValue(SMRSScan_t*)const;

    void SetPrescan(CScanTransaction*);
    int  SetScanKind(SMRSScan_t*);

private:

    CPreScan* m_prescan;
    CPqmLocalShimming m_localshimming;

    int  ShimSetupScratchPad(char* , char*, const int, VfPathElem_t*);
    int  MrsShimStudyOpen();
    int  MrsRefModeRead(int* f_iref_mode_array)const;
    int  MrsRefInfoRead()const;
    int  MrsWsatInfoRead()const;
    int  MrsLocalShimInfoRead();
    int  ShimGlobalInit(char* f_study_file, char* f_scratchpad, const int f_protocol_index);
    bool PrepareRequest();
    int  ShimSetupF0ScratchPad();
    int  ShimHrInfoRead()const;
    MrsSliceCkeckRes_e MrsCheckVoxelMatrixInside()const;
    int  MrsPositionRead()const;
    void MrsPopErrorDialog(const int , const int error_id , const CString& f_errmsg)const;		// Added By KT/ACE-2/11-06-2009/Multilingualisation
    int  MrsWSATSelectionRead(SMRSScan_t* f_mrs_default_value)const; //Patni-Ravindra Acharya/15Dec2010/Added/IR-168
    void SetEddyAxisExecute()const;
    int  LocalShimDone();
    void ProcessResponse(AM_ClientMessageHeader_t* f_hdr, AM_AcqManToClient_t* f_body);

    //Patni-Hemant/03May2011/Added/IR-168_Part3
    bool CanDisplaySpectrumGraph(const CPqmProtocol* f_protocol) const ;

};

#endif // #ifndef __SM_IPCLIB_PQMMRSSCAN_H__ 
