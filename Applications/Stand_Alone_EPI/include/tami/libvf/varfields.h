/******************************************************************************
 *  File Name:		$Source: /mri/cvsroot/develop/src.include/libvf/varfields.h,v $  ($Revision: 1.1.1.1.28.1.2.5.54.3.2.1 $)
 *  Last Modified:	$Date: 2006/12/08 08:20:23 $
 *  Revision Log:	$Log: varfields.h,v $
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5.54.3.2.1  2006/12/08 08:20:23  honma
 *  Revision Log:	MCM0188-00050 Modify VfFieldSpec_t (Appen subgrp)
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5.54.3  2006/10/13 11:21:45  honma
 *  Revision Log:	MCM0177-00208 It once return
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5.54.2  2006/10/05 13:18:26  honma
 *  Revision Log:	MCM0177-00208 Modify VfFieldSpec_t (Appen subgrp)
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5.54.1  2006/10/02 11:32:57  mhirota
 *  Revision Log:	MCM0177-00002 Recon Host Windows Native Porting
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5  2003/11/11 10:16:24  pc_honma
 *  Revision Log:	MCM0102-00148 Append vfOpneLock & vfOpenUnlock
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.4  2003/09/18 10:19:35  pc_yutaka
 *  Revision Log:	MCM0102-00013
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:40  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.2  2002/11/14 15:40:18  develop
 *  Revision Log:	Changed to DLL
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2002/04/16 07:15:05  cvsadmin
 *  Revision Log:	Imported
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/06/08 11:01:57  full
 *  Revision Log:	Change Include Path
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/04/21 08:59:23  ohba
 *  Revision Log:	From Persia
 *  Revision Log:	
 * Revision 1.4  92/10/16  15:43:19  park
 * patch to include stdio.h
 * 
 * Revision 1.3  92/10/16  15:37:47  park
 * Patch to add VFT_SUBTREE to public file.
 * 
 * Revision 1.2  92/10/15  18:39:36  park
 * Varfields Release 1.1 (GP Phase 2)
 * 
 * Revision 1.1  92/01/09  23:18:42  park
 * Initial revision
 * 
 *  
 *    File Overview:
 *	This file defines the fundamental typedefs and macro constants
 *	for utilizing the Varfields library package, as documented in
 *	the Varfields Functional Specification.
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 91-11-06	jyp		Created/Installed
 * End of History */



#ifndef tami_libvf_varfields_H__
#define tami_libvf_varfields_H__


#ifndef stdin
#include <stdio.h>
#endif

#ifndef tami_common_basicdefs_H__
#include	<tami/common/basicdefs.h>
#endif

#ifndef SPECIAL_STUFF_FOR_ILYA__REMOVE_SOON
/* THIS SHOULD REALLY BE MOVED TO vfStudy/something.h!!! */
typedef	uint32_t	VfForm_t;
#endif


typedef union {
    char		*filename;
    int			pipe_fd;
    struct {
	void		*addr;
	unsigned long	size;
    } memblock;
} VfDBName_t;

#define	VFFT_FILE	0
#define	VFFT_MEMORY	1
#define	VFFT_PIPE	2

#ifndef O_CREAT
#include <fcntl.h>
#endif

#ifndef _TIME_T_
#include <time.h>
#endif


typedef	void *		VfDBHandle_t;


typedef	uint32_t	VfName_t;

/*
 * The reserved range for VF names:
 */
#define	VFN_MIN_RESERVED	0xffff0000
#define	VFN_MAX_RESERVED	0xffffffff

/* Unused but reserved range (for future expansion) */
#define	VFN_MIN_UNUSED_RSRVD	0xffff0000
#define	VFN_MAX_UNUSED_RSRVD	0xffffefff

/* Subrange for Vf system-internal: (4K values reserved) */
#define	VFN_MIN_SYS_RSRVD	0xfffff000
#define	VFN_MAX_SYS_RSRVD	0xffffffff


typedef	uint32_t	VfType_t;

/*
 * The reserved range for VF types:
 */
#define VFN_TYPE_MASK		0x00ffffff

#define	VFT_MIN_RESERVED	(0xffff0000 & VFN_TYPE_MASK)
#define	VFT_MAX_RESERVED	(0xffffffff & VFN_TYPE_MASK)

/* Subrange for "universal" types (4K values reserved): */
#define	VFT_MIN_BASIC_RSRVD	(0xffff0000 & VFN_TYPE_MASK)
#define	VFT_MAX_BASIC_RSRVD	(0xffff0fff & VFN_TYPE_MASK)

/* Unused but reserved range (for future expansion) */
#define	VFT_MIN_UNUSED_RSRVD	(0xffff1000 & VFN_TYPE_MASK)
#define	VFT_MAX_UNUSED_RSRVD	(0xffffefff & VFN_TYPE_MASK)

/* Subrange for Vf system-internal (4K values reserved): */
#define	VFT_MIN_SYS_RSRVD	(0xfffff000 & VFN_TYPE_MASK)
#define	VFT_MAX_SYS_RSRVD	(0xffffffff & VFN_TYPE_MASK)

#ifndef VFT_SUBTREE
#define	VFT_SUBTREE	(VFT_MIN_SYS_RSRVD + 2)
#endif

typedef	int32_t		VfSize_t;

typedef struct {
    VfName_t	subtree_name;
    int		index;
} VfPathElem_t;

typedef struct VfFieldSpec {
    VfName_t		name;
    uint32_t		group:8;	/* type + group must be 32bits */
    VfType_t		type:24;	/* must match VFN_TYPE_MASK !! */
    uint32_t		subgrp;		/* sub group Append V9.00      */
    VfSize_t		size;
    VfSize_t		offset;
    void		*buffer;
    unsigned long	opts;
    unsigned long	status;
    struct VfFieldSpec	*next;		/* For internal use only! */
} VfFieldSpec_t;


#define	VFO_DELETE		0x00000001
#define	VFO_REPLACE		0x00000002
#define	VFO_ALLOCATE		0x00000004
#define	VFO_VARIABLE_LEN	0x00000008
#define	VFO_DATA_OFFSET		0x00000010


#define	VFF_SET_ARGS(req, rname, rtype, rsize, rbuf, ropts)	\
    ((void)((req).name=(rname),(req).type=(rtype),(req).size=(rsize),\
    (req).buffer=(rbuf),(req).opts=(ropts),(req).group=0,(req).subgrp=0))
#define	VFF_SET_ARGSG(req, rname, rtype, rsize, rbuf, ropts, fg)	\
    ((void)((req).name=(rname),(req).type=(rtype),(req).size=(rsize),\
    (req).buffer=(rbuf),(req).opts=(ropts),(req).group=fg,(req).subgrp=0))
#define	VFF_SET_ARGSGS(req, rname, rtype, rsize, rbuf, ropts, fg, sg)	\
    ((void)((req).name=(rname),(req).type=(rtype),(req).size=(rsize),\
    (req).buffer=(rbuf),(req).opts=(ropts),(req).group=fg,(req).subgrp=sg))


#define	VFE_ERROR_BIT		0x80000000
#define	VF_ERROR(status)	((status) & VFE_ERROR_BIT)
#define	VFE_NO_ERROR		0x0
#define	VFE_NOT_FOUND		(0x1 | VFE_ERROR_BIT)
#define	VFE_TYPE_MISMATCH	(0x2 | VFE_ERROR_BIT)
#define	VFE_SIZE_MISMATCH	(0x3 | VFE_ERROR_BIT)
#define	VFE_TRUNCATED		0x4
#define	VFE_BAD_NAME		(0x5 | VFE_ERROR_BIT)
#define	VFE_BAD_TYPE		(0x6 | VFE_ERROR_BIT)
#define	VFE_SYS_FIELD		(0x7 | VFE_ERROR_BIT)
#define	VFE_NOT_DONE		(0x8 | VFE_ERROR_BIT)
#define	VFE_EXISTS		(0x9 | VFE_ERROR_BIT)
#define	VFE_NO_SPACE		(0xa | VFE_ERROR_BIT)
#define	VFE_SYS_PROBLEM		(0xb | VFE_ERROR_BIT)
#define	VFE_MAX_ERRNUM		0xb	/* Maintain this! */

#ifdef INCLUDE_ERRSTRS
static const char *vfFieldErrorStrs[] = {
    "no error",
    "field not found",
    "request/field type mismatch",
    "request/field size mismatch",
    "field truncated",
    "bad name",
    "bad type",
    "system field -- illegal",
    "not done",
    "field exists",
    "out of space",
    "system problem",
    0};
#endif	/* INCLUDE_ERRSTRS */

/* Study Headr部分に登録される、文字タイプ情報 V6.10  2003/09/18 */
  /* PC化の為、日本語漢字コードタイプを追加 V6.10  yutaka  2003/09/12 */
#define VF_KANJI_EUC		1
#define VF_KANJI_SJIS		2
#define VF_KANJI_UNICODE	3

  /* PC化の為、ENDIANタイプを追加  V6.10  yutaka  2003/09/12 */
#define VF_ENDIAN_BIG		1
#define VF_ENDIAN_LITTLE	2




/******************************************************************************
 *
 *	This section defines the externally visible functions
 *	for the Varfields Library.
 *
 *****************************************************************************/
#ifdef __NUTC__
#ifdef  DLL_EXPORTS
#define DLLVF __declspec(dllexport)
#else
#define DLLVF /* __declspec(dllimport)	*/
#endif
#else   /* else __NUTC__ */
#define DLLVF
#endif

DLLVF status_t VfGetFields
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfFieldSpec_t	field_list[],
     int		field_list_len,
     int		*num_found /* A bit of a misnomer -- actually, more
				    * num_done; fields found but not copied due
				    * to errs do not count in this total.
				    */
);

DLLVF status_t VfGetAllFields
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     void		*arg,
     VfFieldSpec_t	**fld_list,
     int		*field_list_len);

DLLVF status_t VfPutFields
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfFieldSpec_t	field_list[],
     int		field_list_len,
     int		*num_put,
     time_t		*mod_timestamp);

DLLVF status_t VfLockNode
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     int		lock_type);	/* One of: F_RDLCK or F_WRLCK */

DLLVF status_t VfUnlockNode
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth);

DLLVF status_t VfOpen
    (int		vf_db_type,
     VfDBName_t		name,
     int		opts,
     VfDBHandle_t	*vf_file_id);

DLLVF status_t VfClose
    (VfDBHandle_t	vf_file_id);

DLLVF status_t VfCreateSubtree
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfName_t		new_node_name,
     int		initial_size);

DLLVF status_t VfDeleteSubtree
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfName_t		branch_node_name);

DLLVF status_t VfDeleteBranch
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth);

DLLVF status_t VfAddBranch
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth);

DLLVF status_t VfCopySubtree
    (VfDBHandle_t	src_file_id,
     VfPathElem_t	src_node_path[],
     int		src_node_depth,
     VfDBHandle_t	dest_file_id,
     VfPathElem_t	dest_node_path[],
     int		dest_node_depth);

DLLVF status_t VfCopyPath
    (VfDBHandle_t	src_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfDBHandle_t	dest_file_id);

DLLVF status_t VfScanSubtree
    (VfDBHandle_t	vf_file_id,
     VfPathElem_t	node_path[],
     int		node_depth,
     VfName_t		node_name,
     bitfield_t		real_branches[],
     int		*total,
     int		*active);

DLLVF char *VfFuncErrorStr
    (status_t		errcode,
     char		*errmsg,
     FILE		*output);

DLLVF char *VfFieldErrorStr
    (unsigned long	errcode,
     char		*errmsg,
     FILE		*output);

DLLVF status_t VfGetKanjiEndianType 
	(VfDBHandle_t	vf_file_id,
	int32_t			*kanji_type,
	int32_t			*endian_type);

DLLVF status_t VfPutKanjiEndianType 
	(VfDBHandle_t	vf_file_id,
	int32_t			*kanji_type,
	int32_t			*endian_type);

DLLVF void VfFreeMem(void *ptr);

#endif /* !tami_libvf_varfields_H__ */
