// ScrPadHandler.h: interface for the CScrPadHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHANDLER_H__C62F01E4_DA0A_44D7_AA2F_54FFE62C33E3__INCLUDED_)
#define AFX_SPHANDLER_H__C62F01E4_DA0A_44D7_AA2F_54FFE62C33E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NULL 0
extern "C" {
#include <tami/common/basicdefs.h>
#include <tami/libvfToUi/vfToUiproto.h>    
};


class CScrPadHandler 
{
public:
	CScrPadHandler();
	virtual ~CScrPadHandler();
	int CreateScrPad(char * f_csScratchPadName = "/usr/tmp/PDTOOL.tmp_file", char * f_csStudyFileName = NULL, int f_nprot_idx = 0);	
	int DeleteScrPad();	
	int GetSeqName(char * f_csSeqName);
    int ReleaseHandle();
    int createNewProtocol(CString f_csStudyFileName,int f_nSrcProt,int &f_nDstProt);

    status_t vuiCopySubtree_(VfDBHandle_t vfd_dst, VfPathElem_t vf_dst_path[], int vf_dst_depth, VfDBHandle_t vfd_src, VfPathElem_t vf_src_path[], int vf_src_depth, int image_group, VfName_t subtree_name);
    status_t vuiCopyNode_(VfDBHandle_t vfd_dst, VfPathElem_t vf_dst_path[], int vf_dst_depth, VfDBHandle_t vfd_src, VfPathElem_t vf_src_path[], int vf_src_depth, int image_group);
    status_t vuiCopyProtocol_(VfDBHandle_t vfd_dst, int vf_dst_index, VfDBHandle_t vfd_src, int vf_src_index, int image_group, int *protocol);
    status_t VuiCopyProtocolToFile_(VfDBHandle_t vfd_src,
        int		vf_src_index,
        char		*dst_file,
        int		vf_dst_index,
        int		image_group,
        int		*protocol);
private:
    void vuiDelMoveCouchMethod_(VfDBHandle_t vfd, int protocol);
    void vuiDelPrePos_ (VfDBHandle_t vfd, int protocol);
    void vuiDelCds_ (VfDBHandle_t vfd, int protocol);
    void vuiDelSyncHost_ (VfDBHandle_t vfd, int protocol);
    void vuiDelDegeneracy_ (VfDBHandle_t vfd, int protocol);
    void vuiDelPhaseCorrection_params_ (VfDBHandle_t vfd, int protocol);
    void vuiDelPVCMB_params_ (VfDBHandle_t vfd, int protocol);
    void vuiDelCorrect_params_ (VfDBHandle_t vfd, int protocol);
    void vuiDelInjectTime_ (VfDBHandle_t vfd, int protocol);
    void vuiDelAutoProEdit_ (VfDBHandle_t vfd, int protocol);
    status_t vuiCheckSpeeder_ (VfDBHandle_t vfd, int protocol);
    status_t VuiCopyProtocol_(VfDBHandle_t vfd, int vf_src_index, int vf_dst_index, int image_group, int *protocol);
    status_t VuiCopyProtocolFromFile_(VfDBHandle_t vfd_dst, int vf_dst_index, char *src_file, int vf_src_index, int image_group, int *protocol);
    status_t VuiUpdateAcqOrder_(VfDBHandle_t studyHandle, VuiAcqOrder_t AcqOrder);
    status_t VuiGetVfAcqOrder_(VfDBHandle_t vfHandle, VuiAcqOrder_t *AcqOrder);
    status_t VuiAddToAcqOrder_(VfDBHandle_t studyHandle, int branch);
private:
    char	m_csScrPadName[128];
	char	m_csSeqName[128];//vilas.rev..not used
	char	m_csStudyFileName[256];
	int		m_nProtocolindex;
	
};

#endif // !defined(AFX_SPHANDLER_H__C62F01E4_DA0A_44D7_AA2F_54FFE62C33E3__INCLUDED_)
