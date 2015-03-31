// NodeCopy.h: interface for the CNodeCopy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECOPY_H__0979D8D8_9E7E_4295_988B_D1D036E26B9F__INCLUDED_)
#define AFX_NODECOPY_H__0979D8D8_9E7E_4295_988B_D1D036E26B9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
#include <string.h>
#include <tami/common/basicdefs.h>
#include <tami/libvf/varfields.h>
#include <tami/libvf/vf_errors.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/libvfToUi/vfToUiproto.h>

class CNodeCopy  
{
public:
	CNodeCopy();
	virtual ~CNodeCopy();
    status_t CopySubtree(VfDBHandle_t    vfd_dst,
        VfPathElem_t    vf_dst_path[],
        int     vf_dst_depth,
        VfDBHandle_t    vfd_src,
        VfPathElem_t    vf_src_path[],
        int     vf_src_depth,
        int     image_group,
        VfName_t        subtree_name,
        int         src_process_node,
                int         *dest_process_node);

    status_t CopyProcessedNode(VfDBHandle_t vfd_dst,
        int      vf_dst_index,
        VfDBHandle_t vfd_src,
        int      vf_src_index,
        int      image_group,
        /*int      *protocol,*/
        int      src_process_node,
                       int      *dest_process_node);

    status_t pdNodeCopy(char *srcfile, int srcProtIndex, int dstProtIndex,
                    int src_process_node, int *dest_process_node);
};

#endif // !defined(AFX_NODECOPY_H__0979D8D8_9E7E_4295_988B_D1D036E26B9F__INCLUDED_)
