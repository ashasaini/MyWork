/*****************************************************************************
 *	imgsrv_config.h
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version
 *****************************************************************************/
#ifndef __imgsrv_config_h__
#define __imgsrv_config_h__

#ifdef __cplusplus
extern "C" {
#endif

#define IMGSRV_CONFIG_BUF_SIZE		256

/*---------------------------------------------------------------------------*
 *	prototype
 *---------------------------------------------------------------------------*/
FILE *	imgsrv_config_open(char *file);
void	imgsrv_config_close(FILE *fp);
char *	imgsrv_config_get(FILE *fp, char *key);
char *	imgsrv_config_opengetclose(char *file, char *key);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_config_h__ */
