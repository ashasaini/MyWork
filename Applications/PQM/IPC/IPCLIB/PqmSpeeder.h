//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
// Speeder.h: interface for the CSpeeder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQM_SPEEDER_H__
#define __SM_IPCLIB_PQM_SPEEDER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/speeder.h>
extern "C" {
#include <tami/libAcq/mCouchInfo.h>
}
#include <libStudyManager/NonCopyable.h>
#include <PQM/CoilDefinitions.h>
#include <PQM/PQMCommanMessages.h>
#include "AutoInsertConstant.h"


#define MS_CHECK_ALL_POINT_MODE		0
#define MS_CHECK_SLAB_CENTER_MODE	1
#define MS_CHECK_SLICE_CENTER_MODE	2



#define	ERRCODE_SPEEDER_MAP	            -1
#define	ERRCODE_SPEEDER_AREA	        -2
#define	ERRCODE_SPEEDER_DEGENERACY	    -3						/* V9.20 */
#define	ERRCODE_SPEEDER_DEGENERACY_MAP	-4						/* V9.20 */
#define ERRCODE_TX_INTENSITY_NOTFOUND -5
#define ERRCODE_INTENSITY_NOTFOUND -6
#define ERRCODE_TX_DEGENERACY_MAP -7
#define ERRCODE_TX_INTENSITY_AREA -8

//#define	IDS_ERR_SPEEDER_COIL	"ERR_ILLEGAL_SPEEDER_COIL"
//#define	IDS_ERR_SPEEDER_SWITCHCOIL	"ERR_PROTOCOL"
//#define	IDS_ERR_SPEEDER_MAP		"ERR_CAN_NOT_SPEEDER_MAP"
//#define	IDS_ERR_SPEEDER_AREA	"ERR_OUT_RANGE_SPEEDER"
//#define	IDS_ERR_SPEEDER_DEGENERACY	"ERR_SPEEDER_DEGENERACY"		/* V9.20 */
//#define	IDS_ERR_SPEEDER_COUCH	"ERR_COUCH"
//#define	IDS_ERR_SPEEDER_STUDY	"ERR_STUDY"
//#define	IDS_ERR_SPEEDER_RATIO_OVER		"ERR_SPEEDER_MAX_RATIO"		/* V6.20 */
//
//#define	ERR_INTENSITY_COIL	"ERR_ILLEGAL_INTENSITY_COIL"
//#define	ERR_INTENSITY_MAP	"ERR_CAN_NOT_INTENSITY_MAP"
//#define	ERR_INTENSITY_AREA	"ERR_OUT_RANGE_INTENSITY"
#define PQM_WARN_SCAN_DEGENERACY_MAP	112

// Type redefinition error By Amit Patel
/*#define	MAXIMUM_COIL_IN_PORT   16
typedef struct  PqmSaturnCoil_s {
	int			numCoil;
	CCoil	coiltbl_p[MAXIMUM_COIL_IN_PORT] ;
} PqmSaturnRcvCoil_t;
*/
class CPqm;
class CPqmProtocol;
class CPqmSpeeder;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

typedef void (CPqmSpeeder::*MemFunc)(int);

class CPqmSpeeder : private MR::ACQ::SM::COMMON::NonCopyable<CPqmSpeeder>
{
public:
    //Constructor / Destructor
    CPqmSpeeder();
    virtual ~CPqmSpeeder();

    int  MapCoilCheckSaturn(PqmSaturnRcvCoil_t* coil_t);
    int  MapCoilCheck(const int coilId);
    void SpeederTimerSetup();
    void SpeederDone(CPqmProtocol* const curObj, const int sts);
    void InitializeSpeeder();
    int	 IntensityCheck(CPqmProtocol* const obj,
                        const mode_automap f_map_mode = MAP_NORMAL_MODE,
                        const float f_target_couch_pos = 0.0f);
    int	 MovingBed_SpeederCheck(CPqmProtocol* const obj);
    int  SpeederCheck(CPqmProtocol* const obj,
                      const mode_automap map_mode = MAP_NORMAL_MODE,
                      const float target_couch_pos = 0.0f);


    CString	GetErrorString(const int error_id); // Added By KT/ACE-2/15-06-2009/Multilingualisation

    void    SetPqmPtr(CPqm* f_pqm_ptr);

    bool IsValidSpeederCoil(CPqmProtocol* const f_protocol_obj);
    bool CanSupportB1Correction(CPqmProtocol* const f_protocol_obj) const;
    CPqm* GetPqm();


private:

    static CPqmSpeeder* m_self_ptr;

    CPqm*        m_pqm_ptr;
    int          m_num_speeder;
    unsigned     m_speeder_timeout;
    spdb_rec_p*  s_speederTbl;

    status_t   SpeederCoilCheck(const int coilId);
    status_t   SpeederWholeBodyCheck(const int coilId);
    BOOL   SpeederMAPCoilCheck(CPqmProtocol* const curObj , CPqmProtocol* const mapObj)const;
    status_t   SpeederSearch(CPqmProtocol* curObj , CPqmProtocol** mapObj,
                             const mode_automap map_mode = MAP_NORMAL_MODE,
                             const float target_couch_pos = 0.0f);
    status_t   SpeederSetWBCoilSaturn(CPqmProtocol* const obj, PqmCoil_t* const coil_p);
    void       TimeOut(int sig);
    void       SpeederTimerReset(bool f_Timeout = false);
    status_t   SpeederCouchPosiCheck(const float Current_couchPosi, const int mapProt,
                                     const mode_automap map_mode = MAP_NORMAL_MODE,
                                     const float target_couch_pos = 0.0f);
    status_t   SpeederMaxRatioCheck(CPqmProtocol*  const prObj);
    spdb_rec_p SpeederGetCoil(spdb_rec_p* tbl , const int n, const int coilid);
    int	       SpeederGetrfsLimitInSaturn(spdb_rec_p* tbl , const int num,
                                          PqmSaturnRcvCoil_t* coil_t);

    status_t   SpeederOffSwitchCoil(CVarFieldHandle* const f_vf_handle, const int l_nprot); //Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
    status_t   SpeederOnSwitchCoil(CVarFieldHandle* const vf_handle, const int prot);//+Patni-NP/2010Jul27/Added/IR-161
    status_t   pqmSpeederChangeSwitchCoil(CVarFieldHandle* const vf_handle, const int prot, const int receiveWB_enable); //+Patni-NP/2010Jul27/Added/IR-161

    bool CheckB1ConfigValue() const;
    bool CanSupportB1Anatomy(CPqmProtocol* const f_protocol_obj) const;


    static void SpeederTimeOut(int sig);
};
#endif // #ifndef __SM_IPCLIB_SPEEDER_H__ 
