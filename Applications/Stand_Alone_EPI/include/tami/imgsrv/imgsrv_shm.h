/*****************************************************************************
 *	imgsrv_shm.h
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_shm_h__
#define __imgsrv_shm_h__

#ifdef __cplusplus
extern "C" {
#endif

int	imgsrv_shm_init(imgsrv_shm_t *shm);
void *	imgsrv_shm_attach(imgsrv_job_ds_t *jobds, imgsrv_shm_t *shm);
int	imgsrv_shm_detach(imgsrv_job_ds_t *jobds, imgsrv_shm_t *shm);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_shm_h__ */
