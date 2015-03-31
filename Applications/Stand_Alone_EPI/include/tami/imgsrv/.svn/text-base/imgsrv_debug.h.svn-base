/*****************************************************************************
 *	imgsrv_debug.h
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_debug_h__
#define __imgsrv_debug_h__

#ifdef __cplusplus
extern "C" {
#endif

#define IMGSRV_DEBUG_BUFLEN		512
static char buf[IMGSRV_DEBUG_BUFLEN];
static char *endbuf;


void	imgsrv_debug_print_res(imgsrv_job_res_t *res);
void	imgsrv_debug_print_sync(long mode, imgsrv_job_sync_t *sync);
void	imgsrv_debug_print_jobds(imgsrv_job_ds_t *jobds);
void	imgsrv_debug_print_jobnode(imgsrv_job_node_t *node);
void	imgsrv_debug_set_default_res(imgsrv_job_res_t *res);
int	imgsrv_debug_input_res(char *title, imgsrv_label_t *entry, int def);
int	imgsrv_debug_input_num(char *title, int minimum, int maximum, int def);
void	imgsrv_debug_input_job_res(imgsrv_job_res_t *res);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_debug_h__ */
