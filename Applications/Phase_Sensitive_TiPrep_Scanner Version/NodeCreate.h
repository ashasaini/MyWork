// NodeCreate.h: interface for the NodeCreate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECREATE_H__F568D1B2_1B60_430B_BFE7_02BD4A749DF7__INCLUDED_)
#define AFX_NODECREATE_H__F568D1B2_1B60_430B_BFE7_02BD4A749DF7__INCLUDED_

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


class NodeCreate  
{
public:
	NodeCreate();
	virtual ~NodeCreate();
    static status_t pCopyNode(VfDBHandle_t vfd_dst,
        VfPathElem_t vf_dst_path[],
        int          vf_dst_depth,
        VfDBHandle_t vfd_src,
        VfPathElem_t vf_src_path[],
        int          vf_src_depth,
        int          image_group,
        int          group,
        int          subgroup);
   static  status_t pCopySubtree(VfDBHandle_t  vfd_dst,
        VfPathElem_t  vf_dst_path[],
        int       vf_dst_depth,
        VfDBHandle_t  vfd_src,
        VfPathElem_t  vf_src_path[],
        int       vf_src_depth,
        int       image_group,
                      VfName_t      subtree_name, int, int);

private:
//    static int mask;

};

#endif // !defined(AFX_NODECREATE_H__F568D1B2_1B60_430B_BFE7_02BD4A749DF7__INCLUDED_)
