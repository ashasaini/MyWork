/*****************************************************************************
 *	画像サーバー(ログ出力)
 *	imgsrv_log.h
 *	2002/04/19 y.morita	V0.01 P1.15 coloring
 *	2001/12/09 y.morita	V0.01 P1.04 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_log_h__
#define __imgsrv_log_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <tami/libdbg/debug.h>

#define	IMGSRV_DO(opts, action) \
	(db_option_on(opts)?(void)action:(void)0)

#define	IMGSRV_DO2(opts, opt2, action) \
	((db_option_on(opts)&&db_option_on(opt2))?(void)action:(void)0)

#define	IMGSRV_PRINTF(opts, arglist) \
	{ \
	  IMGSRV_DO2(opts, "T", imgsrv_log_printtime(stdout)); \
	  IMGSRV_DO2(opts, "P", fprintf(stdout,"(%d) ",getpid())); \
	  IMGSRV_DO (opts,      imgsrv_log_printf arglist); \
	  IMGSRV_DO2(opts, "F", fputs(Db_func_desc, stdout)); \
	  IMGSRV_DO2(opts, "L", imgsrv_log_printline(stdout,__FILE__,__LINE__)); \
	  IMGSRV_DO (opts,      fputs("\n", stdout)); \
	  fflush(stdout); \
	}

#define	IMGSRV_ERROR(arglist) \
	{ \
	  fprintf(stderr,"\033[31m"); \
	  IMGSRV_DO("T", imgsrv_log_printtime(stderr)); \
	  IMGSRV_DO("P", fprintf(stderr,"(%d) ",getpid())); \
	                 imgsrv_log_printferr arglist ; \
	  IMGSRV_DO("F", fputs(Db_func_desc, stderr)); \
	  IMGSRV_DO("L", imgsrv_log_printline(stderr,__FILE__,__LINE__)); \
	                 fputs("\n", stderr) ; \
	  fprintf(stderr,"\033[0m"); \
	  fflush(stderr); \
	}

#define	IMGSRV_WARNING(arglist) \
	{ \
	  fprintf(stderr,"\033[35m"); \
	  IMGSRV_DO("T", imgsrv_log_printtime(stderr)); \
	  IMGSRV_DO("P", fprintf(stderr,"(%d) ",getpid())); \
	                 imgsrv_log_printfwrn arglist ; \
	  IMGSRV_DO("F", fputs(Db_func_desc, stderr)); \
	  IMGSRV_DO("L", imgsrv_log_printline(stderr,__FILE__,__LINE__)); \
	                 fputs("\n", stderr) ; \
	  fprintf(stderr,"\033[0m"); \
	  fflush(stderr); \
	}

#define	IMGSRV_FUNC_ENTER(func_desc_str) \
	static char *Db_func_desc = func_desc_str; \
	IMGSRV_PRINTF("f", ("Entered."))

#define IMGSRV_FUNC_EXIT() \
	IMGSRV_PRINTF("f", ("Exit."))

#define IMGSRV_LOG_INIT(file) \
	static char *Db_func_desc = "[main]"; \
	imgsrv_log_init(file);  \
	IMGSRV_PRINTF("f", ("Entered."))


int	imgsrv_log_printtime(FILE *fp);
int	imgsrv_log_printf(char *fmt, ...);
int	imgsrv_log_printline(FILE *fp, char *f, int l);
void	imgsrv_log_change_output(char *out);
void	imgsrv_log_change_mask(char *mask);
void	imgsrv_log_init(char *file, char *mask);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* #ifndef __imgsrv_log_h__ */
