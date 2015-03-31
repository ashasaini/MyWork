/******************************************************************************
 *  
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *      
 *
 *****************************************************************************/
/* Revision History:
 * yyyy-mm-dd     modified-by     modifications-done
 * 2003-06-13     Y.Watanabe      Created.
 * End of History */



#ifndef msGetHostAdr_H__
#define msGetHostAdr_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NUTC__
#define INET6_ADDRSTRLEN        46
#ifndef __WIN_NATIVE_PORTING__
#include <sys/socket.h>
#endif /* __WIN_NATIVE_PORTING__ */
#endif

#ifdef __FREE_BSD__
#include <sys/socket.h>
#endif

status_t msGetHostAdr(char *hostName,
		      char **hostAddress);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* msGetConfig_H__ */

