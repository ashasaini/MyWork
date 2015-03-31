/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 2003-05-08	S.Uchizono        created.
 * End of History */


#ifndef __msMvfcp_lib_h__
#define __msMvfcp_lib_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/common/errors.h>
/*===========================================================================*
 *	定数
 *===========================================================================*/
#define MS_MVF_MAX_NODES		2048
#define MS_MVF_MAX_STRING		 256

#define MS_MVF_PATH_DELIM_SNODE	"/"
#define MS_MVF_PATH_DELIM_INODE	'/'
#define MS_MVF_PATH_DELIM_IFILE	':'
#define MS_MVF_PATH_REGEX		"^[^:]+(:(/[0-9]+\\.[0-9]+)+)?$"

#define MS_MVF_OPTION_DECL		BITFLD_DECL(optflg, 64)
#define MS_MVF_OPTION_INIT()	BITFLD_INIT(optflg, 64, 0)
#define MS_MVF_OPTION_ON(opt)	BITFLD_ON(optflg, opt-'A')
#define MS_MVF_OPTION_SET(opt)	BITFLD_SET(optflg, opt-'A')
#define MS_MVF_OPTION_RESET(opt)	BITFLD_RESET(optflg, opt-'A')

#define MS_MVF_ERROR(str)		{ \
				  fprintf(stderr,"%s:%d *** ",__FILE__,__LINE__); \
				  print_error str ; \
				}

#define MS_MVF_ERROR_RETURN(str,sts)	{ \
				  MS_MVF_ERROR(str) \
				  return(sts); \
				}

#define MS_MVF_ERROR_EXIT(str,sts)	{ \
				  MS_MVF_ERROR(str) \
				  exit(sts); \
				}

/* 終了ステータス */
#define MS_MVFCP_E_NO_ERROR	        0	/* 正常終了 */
#define MS_MVFCP_E_ERROR		  1	/* コマンドライン引数間違い等 */

#define MS_MVFCP_E_ILL_PATH_SRC	  2	/* illegal path (src) */
#define MS_MVFCP_E_ILL_PATH_DST      3	/* illegal path (dst) */
#define MS_MVFCP_E_DEPTH_NOT_EQ	  4	/* depth not equal */
#define MS_MVFCP_E_EXT_PATH_SRC	  5	/* extract src path index failed (src) */
#define MS_MVFCP_E_EXT_PATH_DST	  6	/* extract src path index failed (dst) */

/* 終了ステータス */
#define MS_MVFCP_E_NO_ERROR	        0	/* 正常終了 */
#define MS_MVFCP_E_ERROR		  1	/* コマンドライン引数間違い等 */

#define MS_MVFCP_E_ILL_PATH_SRC	  2	/* illegal path (src) */
#define MS_MVFCP_E_ILL_PATH_DST      3	/* illegal path (dst) */
#define MS_MVFCP_E_DEPTH_NOT_EQ	  4	/* depth not equal */
#define MS_MVFCP_E_EXT_PATH_SRC	  5	/* extract src path index failed (src) */
#define MS_MVFCP_E_EXT_PATH_DST	  6	/* extract src path index failed (dst) */
#define MS_MVFCP_E_PROT_M_MATCH	  7	/* protocol index mismatched. */
#define MS_MVFCP_E_VF_OPEN_SRC	  8	/* src study open failed (src) */
#define MS_MVFCP_E_VF_OPEN_DST	  9	/* src study open failed (dst) */
#define MS_MVFCP_E_PATH_CREATE	 10	/* dst path create failed. */
#define MS_MVFCP_E_SUBTREE_COPY	 11	/* subtree copy failed. */
#define MS_MVFCP_E_ROOT_COPY	       12	/* root fields copy failed. */
#define MS_MVFCP_E_SEGV		 13	/* segmentation fault */ 

/* 注意 : MVFC_E_ID_MIS_MATCHのステータス番号は変更してはいけません
 *	  studyCopyで使用しています。 2002.06.07 */
#define MS_MVFCP_E_ID_MIS_MATCH	127	/* study_id mismatched */

status_t msMvfcp(char *src, char *dst, int optArgc, char *optArgv[]);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __msMvfcp_lib_h__ */







