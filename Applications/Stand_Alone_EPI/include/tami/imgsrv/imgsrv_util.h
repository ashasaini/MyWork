/*****************************************************************************
 *	imgsrv_util.h
 *	2002/01/11 y.morita	V0.01 P1.10 get_pname.
 *	2001/12/10 y.morita	V0.01 P1.06 log(mask/output).
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_util_h__
#define __imgsrv_util_h__

#ifdef __cplusplus
extern "C" {
#endif

long	imgsrv_sizeof_type(long type);
long	imgsrv_sizeof_form(long form);
char *	imgsrv_label(long req, imgsrv_label_t *label);
int	imgsrv_input_num(char *title, int minimum, int maximum, int def);
char *	imgsrv_input_string(char *title, int maxlen);
char *	imgsrv_get_pname(int pid);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_util_h__ */
