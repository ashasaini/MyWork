/*****************************************************************************
 *	imgsrv_time.h
 *	2001/10/24 y.morita	V0.01 P1.02 dont use this function. 
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_time_h__
#define __imgsrv_time_h__

#ifdef __cplusplus
extern "C" {
#endif

void imgsrv_time_reset_backup();
void imgsrv_time_stamp_backup();

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_time_h__ */
