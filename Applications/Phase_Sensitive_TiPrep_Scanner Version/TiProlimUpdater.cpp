// TiProlimUpdater.cpp: implementation of the CTiProlimUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tiprep.h"
#include "TiProlimUpdater.h"
#include <iostream>
#include <vector>
#include <map>
using std::vector;
#include <winsock2.h>
#include <tami/common/basicdefs.h>
#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/vfStudy/vfs_structure.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <tami/libpl/plSeqSelDB.h>
#include <tami/common/globalSwitches.h>
#include <tami/libseq/seq.h>
//#include <tami/libpltree/Plsftree.h>
#include <tami/libpl/plParDefs.h>
//#include <tami/libvfToUi/vfToUiproto.h>

#include <tami/vfStudy/vf_appcodes.h>  
#include "tami/libWNP/WNP_types.h"
#include <LibPlTreePvtWrapper/LibPlTreePvtWrapper.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern  "C" {
	status_t	VuiCopyProtocolToFile	(VfDBHandle_t,int,char *,int,
		int,int *);
	status_t	VuiCopyProtocolFromFile	(VfDBHandle_t,int,char *,int,
		int,int *);
	status_t VuiCopyProtocolFromPrivateProlimScratchPad(
		VfDBHandle_t vfd_dst,
		int	vf_dst_index,
		char* src_file,
		int	vf_src_index,
		int	image_group,
		int*	protocol
		);
	
}
CTiProlimUpdater::CTiProlimUpdater()
{
	
}

CTiProlimUpdater::~CTiProlimUpdater()
{
	
}
status_t UpdateTi(char * scratchpad, float ti,int protocol)
{
	VfPathElem_t        path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_VISUALGROUP,    0},
    };
	VfDBHandle_t            vfd_dst = NULL;
    VfDBName_t              vfName;
    status_t                status = E_ERROR;
	VfFieldSpec_t       vfReq[11] = {0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections
	int n = 0, num = 0, plan_status = VFC_PLN_STATUS_GLOC;
	long timstamp;
	
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 11);
	vfName.filename = scratchpad;
    if_error(VfOpen(VFFT_FILE, vfName, O_RDWR, &vfd_dst))
    {
        printf("E: *** in VfOpen(%s) (%d,%s).\n",
			vfName.filename, __LINE__, __FILE__);
		return 1;
    }
	//	VFF_SET_ARGS(vfReq[0], SVN_PLN_STATUS, SVT_PLN_STATUS,
	//	sizeof(plan_status), &plan_status, VFO_REPLACE); // not required confirmed
	// n++;
	VFF_SET_ARGS(vfReq[n], SVN_PLN_TI, SVT_PLN_TI,
		sizeof(ti), &ti, VFO_REPLACE);
    n++;
	status = VfPutFields(vfd_dst, (path), SVD_ST_PROTOCOL,vfReq,n ,&num,  &timstamp);
	return status;
	//relimitG.studyFile
}
status_t GetSeqNameAndArchitecture(VfDBHandle_t hndl, CString &seqname, CString &seqarch, int protocol)
{
	VfPathElem_t        path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_VISUALGROUP,    0},
    };
    status_t                status = E_ERROR;
	VfFieldSpec_t       vfReq[11] = {0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections
	int n = 0, num = 0, plan_status = VFC_PLN_STATUS_GLOC;
	char l_ident[60] = {0};
	char* l_sequence_architecture;
	VFF_SET_ARGS(vfReq[0], SVN_PLN_TEMPLATE_SEQ, SVT_PLN_TEMPLATE_SEQ,
		60, l_ident, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
	VFF_SET_ARGS(vfReq[n], SVN_SEQ_ARCHITECTURE, SVT_SEQ_ARCHITECTURE, 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
	status = VfGetFields(hndl, (path), SVD_ST_PROTOCOL,vfReq,n ,&num);
	if (vfReq[1].buffer && E_NO_ERROR == status && (2 == num)) {
        l_sequence_architecture = (char*)(vfReq[1].buffer);
        seqarch = l_sequence_architecture;
		seqname = l_ident;
    }
	
	return status;
	//relimitG.studyFile
}
int CTiProlimUpdater::ProlimTi()
{
	char studyname [512] = {'\0'};
	char *scratchpad = "/usr/tmp/PDTOOL.tmp_file";
	wcstombs(studyname,(LPCTSTR)m_csstudyfile,512);
	VfDBHandle_t    vfd = NULL;
    VfDBName_t      vfName;
    status_t        status = E_ERROR;
    unlink(scratchpad);
    vfName.filename = scratchpad;
	//	vfName.filename = studyname;
	
    status = VfOpen(VFFT_FILE, vfName, O_CREAT |O_RDWR, &vfd);
    if(E_NO_ERROR != status)
    {
		// printf("Could not open the study file %s \n", relimitG.studyFile);
        return status;//(E_ERROR);
    }
	
	
	/*	status = VuiCopyProtocolToFile(vfd, protocol,
	"/usr/tmp/PDTOOL.tmp_file",
	0,
	0, NULL);*/
	status = VuiCopyProtocolFromFile(vfd, 0,
		studyname,
		m_nprotocol,
		0, NULL);
	
	if(status != 0)
    {
        VfClose(vfd);
		//        WriteToErrorLog("\n,error ",__LINE__,ERROR_LOGONLY);
        return status;//(E_ERROR);
    }
    VfClose(vfd);
status = 	UpdateTi(scratchpad,m_ftival,m_nprotocol);
if (status != E_NO_ERROR) return status;
	HINSTANCE m_module = LoadLibrary(_T("libPlTreePvtWrapper.dll"));;
	bool l_status = false;
#if 1
	CLibPlTreePvtWrapper *m_prolim_private_wrapper = NULL;
    if (INVALID_HANDLE_VALUE == m_module || NULL == m_module) {
        m_prolim_private_wrapper = NULL;
		//        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to load Prolim Library"));
        return 1;
    }
	
    typedef CLibPlTreePvtWrapper* (*FUNC_PTR_INSTANCE)(void);
    FUNC_PTR_INSTANCE l_instance_ptr = (FUNC_PTR_INSTANCE)GetProcAddress(m_module, "GetInstance");
	
    if (l_instance_ptr != NULL) {
        m_prolim_private_wrapper = l_instance_ptr();
		
    } else {
        return 1;
    }
	
    CString l_sequence_name = _T("FFE3");
	
    CString l_seq_arch =  _T("SDE");
	GetSeqNameAndArchitecture(vfd,l_sequence_name,l_seq_arch,m_nprotocol);
    wchar_t* l_seq_archtre = l_seq_arch.GetBuffer(512);
    wchar_t* l_seq_name = l_sequence_name.GetBuffer(512);
	
	
	
    std::wstring l_path;
    l_status = m_prolim_private_wrapper->ExecutePlUpdate(l_seq_name, l_seq_archtre, &l_path, "/usr/tmp/PDTOOL.tmp_file");
	
	
    if (l_status) {
        CString l_exe_path(l_path.c_str());
		vfName.filename = studyname;
		//	vfName.filename = studyname;
		
		status = VfOpen(VFFT_FILE, vfName, O_RDWR, &vfd);
		if(E_NO_ERROR != status)
		{
			// printf("Could not open the study file %s \n", relimitG.studyFile);
			return status;//(E_ERROR);
		}
		status = VuiCopyProtocolFromPrivateProlimScratchPad(vfd, m_nprotocol, scratchpad,
			0, 0, NULL);
    } else {
    }
	l_sequence_name.ReleaseBuffer();
    l_seq_arch.ReleaseBuffer();
#endif
	
	return 0;
	//Prolim code comes here
	return 0;
}

void CTiProlimUpdater::SetData(int prot, float ti,CString &study)
{
	m_ftival = ti;
	m_nprotocol = prot;
	m_csstudyfile = study;
	
}
