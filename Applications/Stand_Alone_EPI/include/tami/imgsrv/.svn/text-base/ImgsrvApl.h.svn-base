/*****************************************************************************
 *	画像サーバーインターフェースライブラリ
 *	ImgsrvApl.h
 *	2002/02/28 y.morita	V0.01 P1.14 extra-information(icon)
 *	2002/01/18 y.morita	V0.01 P1.12 msImgsrvInfo_t.
 *	2001/12/19 y.morita	V0.01 P1.08 extra-information
 *	2001/12/14 y.morita	V0.01 P1.07 Initial Version.
 *****************************************************************************/
#ifndef __ImgsrvApl_H__
#define __ImgsrvApl_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/common/basicdefs.h>
#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <tami/libmsVf/libmsData.h>
#include <tami/imgsrv/Imgsrv.h>


/*********************************************************************
 *	構造体定義
 *********************************************************************/
typedef struct msImgsrvInfo_s {
  char		study[MS_STUDY_NAME_LEN];
  msPath_t	path;
  flt32_t	max;
  flt32_t	min;
  flt32_t	scale;
  flt32_t actualShift;  /* --- Bikram --- */
  flt32_t applyShift;   /* --- Bikram --- */
  int32_t noChannels;   /* --- Bikram --- */
  flt32_t channelShift[128]; /* --- Bikram --- */
  int16_t	icon[64*64];	/* iconの型とサイズは固定 */
} msImgsrvInfo_t;


/*********************************************************************
 *	プロトタイプ宣言
 *********************************************************************/
int32_t	msImgsrvStartStableSock(
		int32_t		socket,
		VfType_t	type,
		VfForm_t	form,
		int32_t		row,
		int32_t		col,
		int32_t		slice
	);

int32_t	msImgsrvStartStable(
		VfType_t	type,
		VfForm_t	form,
		int32_t		row,
		int32_t		col,
		int32_t		slice
	);


int32_t	msImgsrvStartCyclicSock(
		int32_t		socket,
		VfType_t	type,
		VfForm_t	form,
		int32_t		row,
		int32_t		col,
		int32_t		slice
	);

int32_t	msImgsrvStartCyclic(
		VfType_t	type,
		VfForm_t	form,
		int32_t		row,
		int32_t		col,
		int32_t		slice
	);

int32_t	msImgsrvFinishSock(
		int32_t		socket,
		int32_t		jobid
	);

int32_t	msImgsrvFinish(
		int32_t		jobid
	);

int32_t	msImgsrvPutSock(
		int32_t		socket,
		int32_t		jobid,
		int32_t		sliceno,
		void		*data,
		int32_t		nbyte,
		msImgsrvInfo_t	*info,
		int32_t		nsec
	);

int32_t	msImgsrvPut(
		int32_t		jobid,
		int32_t		sliceno,
		void		*data,
		int32_t		nbyte,
		msImgsrvInfo_t	*info,
		int32_t		nsec
	);

int32_t	msImgsrvGetSock(
		int32_t		socket,
		int32_t		jobid,
		int32_t		sliceno,
		void		*data,
		int32_t		nbyte,
		msImgsrvInfo_t	*info,
		int32_t		nsec
	);

int32_t	msImgsrvGetImdt(
		int32_t		jobid,
		int32_t		sliceno,
		void		*data,
		int32_t		nbyte,
		msImgsrvInfo_t	*info
	);

int32_t	msImgsrvGetSync(
		int32_t		reqid,
		int32_t		nsec
	);

int32_t	msImgsrvGetCancel(
		int32_t		reqid
	);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #define __ImgsrvApl_H__ */
