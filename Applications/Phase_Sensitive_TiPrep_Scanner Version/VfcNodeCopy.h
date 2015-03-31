// VfcNodeCopy.h: interface for the CVfcNodeCopy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFCNODECOPY_H__60ADF03D_5B6B_4589_909D_2F73F0B03B0A__INCLUDED_)
#define AFX_VFCNODECOPY_H__60ADF03D_5B6B_4589_909D_2F73F0B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "tami/libWNP/WinNativePorting.h"
// #######################################//
#include <stdio.h>
#include <tami/vfStudy/vfs_plan.h>
#include <tami/common/basicdefs.h>
#include <tami/common/bitfields.h>
#include <tami/libvf/varfields.h>
#include <tami/libvf/vf_errors.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/libvfToUi/vfToUiproto.h>

class CVfcNodeCopy  
{
public:
	CVfcNodeCopy();
	virtual ~CVfcNodeCopy();
    int vuiIsNonCopyFieldOnExceptSaveScratchPadToVF(const VfName_t name);
    status_t CopyProcessNode(
        VfDBHandle_t vfd_dst,
        VfPathElem_t vf_dst_path[],
        int vf_dst_depth,
        VfDBHandle_t vfd_src,
        VfPathElem_t vf_src_path[],int vf_src_depth);
    status_t vuiCopyNode(
        VfDBHandle_t vfd_dst,
        VfPathElem_t vf_dst_path[],
        int vf_dst_depth,
        VfDBHandle_t vfd_src,
        VfPathElem_t vf_src_path[],
        int vf_src_depth,
        int image_group,
        int is_copy_from_scratch_pad
);
    int vuiIsNonCopyFieldOnSaveScratchPadToVF(const VfName_t name);
    void clearBuffer(VfFieldSpec_t* req, int length);
    status_t vuiCopyFields(VfDBHandle_t vfd_src, VfPathElem_t* vf_src_path, int vf_src_depth,
        VfDBHandle_t vfd_dst, VfPathElem_t* vf_dst_path, int vf_dst_depth,
        VfFieldSpec_t* vf_dst_req, int vf_dst_req_len, int image_group);
    int vuiGetMaxCopyLength();
    status_t vuiCopySubtree(
        VfDBHandle_t vfd_dst,
        VfPathElem_t vf_dst_path[],
        int	vf_dst_depth,
        VfDBHandle_t vfd_src,
        VfPathElem_t vf_src_path[],
        int	vf_src_depth,
        int	image_group,
        VfName_t subtree_name,
        int is_copy_from_scratch_pad
);
     int vuiIsNonCopyField(const VfName_t name);
};

#endif // !defined(AFX_VFCNODECOPY_H__60ADF03D_5B6B_4589_909D_2F73F0B03B0A__INCLUDED_)
