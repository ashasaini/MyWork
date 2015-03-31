/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 2003-05-12	S.Uchizono        created.
 * End of History */


#ifndef __msMvfcp_apl_h__
#define __msMvfcp_apl_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/common/errors.h>
/*===========================================================================*
 *	íËêî
 *===========================================================================*/
#define MS_MVF_TMP_FILE	"/gp/tmp/test_study"	
#define MS_MVF_CONSOLE_HOST	"mrsm00"
#define MS_MVF_ENGINE_HOST	"mrengine00"

#ifdef __NUTC__

#define MS_MVF_CP_CMD	      "ucp.exe"
#define MS_MVF_MKDIR_CMD     "mkdir.exe"

#else

#define MS_MVF_CP_CMD	     "rcp"
#define MS_MVF_MKDIR_CMD     "mkdir"

#endif

#define MS_MVF_MAX_PROTOCOL  99999

status_t msMvfcpAM(char *srcStudy,int protocol,char *opt);
status_t msMvfcpRb(char *dstStudy,int protocol,char *opt);
void	msMvfcpAM_return_point(void);
#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __msMvfcp_apl_h__ */







