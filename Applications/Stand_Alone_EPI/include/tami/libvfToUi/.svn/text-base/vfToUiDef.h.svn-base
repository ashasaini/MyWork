/*********************************************************************
 *
 *  (c) Copyright 1996 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libvfToUi/vfToUiDef.h,v $  ($Revision: 1.1.1.1 $)
 *  Last Modified:   $Date: 2010/01/06 06:45:42 $
 *  Revision Log:    $Log: vfToUiDef.h,v $
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.2.28.1.2.3  2003/03/31 07:25:41  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2002/08/05 05:35:36  purandar
 *  Revision Log:    Merging new code with ported code
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2000/08/24 07:14:15  full
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/06/08 11:01:58  full
 *  Revision Log:    Change Include Path
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/04/21 08:59:19  ohba
 *  Revision Log:    From Persia
 *  Revision Log:
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Feb.22.96	Naoyuki FURUDATE	Created
 * End of History */



#ifndef vfToUiDef_H__
#define vfToUiDef_H__

/* #define DEBUG  */
/* system includes */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>

#include <tami/common/basicdefs.h>
#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <tami/libvf/vf_errors.h>
#include <tami/common/study_id.h>
#include <tami/libTl/Tl.h>

#define	MAX_BITFLD			512
#define MAX_DEPTH			  8
#define	MAX_STRING_DATA			128
#define SHRT_STRING_DATA		 32
#define MAX_LIST_DATA			512
#define	MAX_PROTO_DATA			256
#define	SUMMRY_NULL_NUM			  3

#define PROTOCOLLEVEL			SVD_ST_PROTOCOL
#define SUBPROTOCOLLEVEL		SVD_ST_SUBPROTOCOL
#define SLICEGROUPLEVEL			SVD_ST_SLICEGROUP

#define MAX_PRINT_DATA			128
#define DISPLAY_BLOCK_NUM		0
#define DISPLAY_POSITION		1
#define DISPLAY_CHAR_NUM		2
#define DISPLAY_FORMAT			3

#define DISPLAY_POSITION_LEFT		0 
#define DISPLAY_POSITION_CENTER		1
#define DISPLAY_POSITION_RIGHT		2

#define DISPLAY_BLOCK_ALL		0
#define DISPLAY_BLOCK_1			(1L<<0)
#define DISPLAY_BLOCK_2			(1L<<1)
#define DISPLAY_BLOCK_3			(1L<<2)
#define DISPLAY_BLOCK_4			(1L<<3)

#define VUI_DEF_LISTDATA		"proDispDictionary"
#define VUI_DEF_LISTENV			getenv("GP_PRO_DIR")

#define VUI_DEF_BREAK			","
#define VUI_DEF_DEFINE			"define"
#define VUI_DEF_UNIT			"unit"
#define VUI_DEF_CONVERT			"convert"
#define VUI_DEF_LABEL			"label"

#define VUI_DEF_NULLDATA		"NONE"
#define VUI_DEF_NULLWORD		"NULL"
#define VUI_DEF_ADDCALLBACK		'@'

#define VUI_SHIFT_MODE_VG		(1<<0)
#define VUI_SHIFT_MODE_SG		(1<<1)

#define VUI_SHIFT_X			(1<<0)
#define VUI_SHIFT_Y			(1<<1)
#define VUI_SHIFT_Z			(1<<2)

#if 0
/***********/
/***********/

#define NULLLINE			'-'
#define SEMICOLON			";"
#define COLON				":"
#define BLUNK				" "
#define CBLUNK				' '
#define DOLLER				"$"

#define PROTOCOL			"PROTOCOL"
#define SUBPROTOCOL			"SUBPROTOCOL"
#define SLICEGROUP			"SLICEGROUP"

#define CONVERT_SEPARATES		3
#define DEFINE_SEPARATES		2

#define DEFINE_LEVEL			0
#define DEFINE_FIELD			1

#define CONVERT_FORMAT			0
#define CONVERT_FUNC			1

#define INTTYPE				0
#define FLTTYPE				1
#define STRTYPE				2

#define ItoI				0
#define ItoF				1
#define FtoI				2
#define FtoF				3

#endif

/*******************************************************
 * Label & Data Info Struct 
 *******************************************************/
typedef struct VuiInfo_s {
    char *	indexString;
    char *	shortLabel;
    char *	longLabel;
    int32_t	block;
    int32_t 	start_posi;
    int32_t 	length;
    char *	displayChar;
} VuiInfo_t;

typedef status_t (*addConvertfunc_t)(void *data,int index,char *disp);

typedef struct	VuiAcqOrder_s{
    int			*VuiAcqOrder;
    int			NumAcqOrder;
    int			SzAcqOrder;
} VuiAcqOrder_t;

#endif /* vfToUiDef_H__ */
