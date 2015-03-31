// pqmAMBConstants.h: interface for the CpqmAMBConstants class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAMBCONSTANTS_H__
#define __SM_IPCLIB_PQMAMBCONSTANTS_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	AMB_DEF_SEG			2
#define	AMB_DEF_DIRECTION	0
#define	AMB_DEF_SHIMMING	OFF
#define	AMB_DEF_MAP			OFF
#define	AMB_DEF_VOICE		OFF
#define	AMB_DEF_PREP		OFF
#define	AMB_DEF_REPEAT		1

///**************
#define	COUCH_ERR_NON		0x00000000
#define	COUCH_ERR_LEVEL1	0x00000001
#define	COUCH_ERR_LEVEL2	0x00000010
#define	COUCH_ERR_LEVEL3	0x00000100

/////*********************
/************ ERROR CODE ****************/
#define	PQM_AMB_SEQ_COPY				0x0001	/* Slave�̓R�s�[�ł��܂��� */
#define	PQM_AMB_PAS_REGI				0x0002	/* Slave��PAS�o�^�ł��܂��� */
#define	PQM_AMB_START_NONPLAN			0x0003	/* �ʒu���߂��Ă��Ȃ� */
#define	PQM_AMB_START_MOVE_COUCH		0x0004	/* �v���g�R�������������Ȃ� */
#define PQM_AMB_START_DISABLE_SEQ		0x0005	/* ������ĂȂ��V�[�P���X */
#define PQM_AMB_START_BAD_PARAMETER		0x0006	/* �B��������������Ă��Ȃ� */
#define PQM_AMB_START_SELECT_SEQ		0x0007	/* �P�����I������Ă��Ȃ� */
#define PQM_AMB_START_SELECT_SEQ_OVER	0x0008	/* �I�𐔂������� */
#define PQM_AMB_START_ILLEGAL_PARENT	0x0009	/* �ʒu���ߐe�摜������ */
#define PQM_AMB_START_MATRIX_OVER		0x000a	/* �č\��Matrix�I�[�o�[ */
#define PQM_AMB_START_COUCH_OVER		0x000b	/* �Q��ړ��������߂��� */
#define	PQM_AMB_COUCH_POSI_ILLEGAL		0x000c	/* �Q��ʒu����s�\ */
#define PQM_AMB_COUCH_MOVE_NORMAL		0x000d	/* �Q��ړ������B�� */
#define PQM_AMB_START_PREP_NONPLAN		0x000e	/* Prep�̈ʒu���߂Ȃ� */
#define PQM_AMB_START_PROTOCOL_OVER		0x000f	/* 64������ꍇ */
#define PQM_AMB_START_RECIVED_COIL		0x0010	/* �R�C����������Ă��Ȃ� */
#define PQM_AMB_START_WITH_CDS		0x0011	/* COMPASS�Ƃ͕��p�ł��Ȃ� */

#define PQM_AMB_ILLEGAL_INFOMATION		0x0100
#define PQM_AMB_SYSTEM_ERROR			0x1000

////*****
/********   Amb_status  **********/
#define	AMB_GO_SCAN     0x0001
#define	AMB_SEG1_SCAN   0x0002
#define	AMB_SEG2_SCAN   0x0004
#define	AMB_PREP_SCAN   0x0008

//************
#define	LOOP_MASK			0x000000ff
#define	STATION_MASK		0x0000ff00
#define	SEG_MASK			0x00ff0000
#define	TYPE_MASK			0xff000000

#define	GET_LOOP(a)		( (a & LOOP_MASK) )
#define	GET_STATION(a)	( (a & STATION_MASK) >>8)
#define	GET_SEG(a)		( (a & SEG_MASK) >>16)
#define	GET_TYPE(a)		( (a & TYPE_MASK) >>24)

#define	PUT_LOOP(a)		((a & LOOP_MASK))
#define	PUT_STATION(a)	((a << 8) & STATION_MASK)
#define	PUT_SEG(a)		((a << 16) & SEG_MASK)
#define	PUT_TYPE(a)		((a << 24) & TYPE_MASK)

#define	AMB_SHIMMING		0x1
#define	AMB_SPEEDER_MAP		0x2
#define	AMB_SCAN			0x4

#define	AMB_MASTER			0x8
#define	AMB_SLAVE			0x0
#define	AMB_SCAN_DONE		0x10
#define	AMB_PREP			0x20

#define OFFSET_RATE_AMB	1000.0

#define AMB_SHIMMING_STUDY "\\gp\\data\\prosel\\AmbAASStudy"
#define AMB_MAP_STUDY "\\gp\\data\\prosel\\AMBMapStudy"
#define AMB_SHIM    1
#define AMB_MAP     2
#endif // #ifndef __SM_IPCLIB_PQMAMBCONSTANTS_H__
