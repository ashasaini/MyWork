//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-777
//Redmine-780
//Redmine-782
// CheckSequence.h: interface for the CCheckSequence class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_CHECKSEQUENCE_H__
#define __SM_IPCLIB_CHECKSEQUENCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CoilDefinitions.h>

#define CONSTANT_INT_ZERO 0
/*      for Reconstruction Machine */
#define ENGINE "ENGINE"        /* V5.00 */
#define CONNECT "CONNECT"       /* V5.00 */
#define NONE "NONE"          /* V5.00 */
#define PLAY_REVERSE_SIZE 65536	/* 256x256x2x512/1024 */
#define	CHECK_SCALE	(1000*1000)


typedef struct PqmAcqRatio_s {
    f32vec2_t	m_matrix;
    int32_t		m_rf_echoes;
    int32_t		m_epi_echoes;
    flt32_t		m_tr ;
    flt32_t		m_grad_echo_space;
    flt32_t		m_rf_echo_space ;
    int32_t		m_channel_prevfep;
    int32_t		m_imaging_mode;
    int32_t		m_app_code;
    int32_t		m_num_slabs;
    int32_t		m_num_echoes;
    int32_t		m_num_slices;

    PqmAcqRatio_s() :
        m_rf_echoes(0),
        m_epi_echoes(0),
        m_tr(0.0f),
        m_grad_echo_space(0.0f),
        m_rf_echo_space(0.0f),
        m_channel_prevfep(0),
        m_imaging_mode(0),
        m_app_code(0),
        m_num_slabs(0),
        m_num_echoes(0),
        m_num_slices(0) {
        m_matrix.x = 0 ;
        m_matrix.y = 0 ;
    }

} PqmAcqRatio_t , *PqmAcqRatio_p ;

// pre-declaration
class CPqm;
class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CCheckSequence : private MR::ACQ::SM::COMMON::NonCopyable<CCheckSequence>
{
public:
    //Constructor / Destructor
    explicit CCheckSequence(CPqm* f_pqmptr);
    virtual ~CCheckSequence();

    //Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space
    BOOL SkipMPlusDiskCheck()const;
    int CheckDiskSpace(CPqmProtocol* const f_protocol)const;
    int CheckMultiProtocolDiskSpace(int32_t* f_prot_tbl, const int f_num , uint32_t& f_amb_image_totalsize)const;
    int CheckAMBDiskSpace(uint32_t f_amb_image_totalsize);
    int UpdateCoilRelatedData(CPqmProtocol* const f_protocol)const;
private:
    CPqm* m_pqmptr;


    int	PqmPrSetChannelGainData(CVarFieldHandle* const f_vf_handle, CPqmProtocol* const  f_protocol,
                                PqmCoil_t* const l_coil_ptr)const;

    int CheckVapRatio(CVarFieldHandle* const f_vf_handle , const int f_prot)const;
    int GetNumPointsforTR(CVarFieldHandle* const f_vf_handle, const int f_prot, PqmAcqRatio_p const f_acqdata)const;
};

#endif // #ifndef __SM_IPCLIB_CHECKSEQUENCE_H__ 
