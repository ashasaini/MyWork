/******************************************************************************
 *  
 *    File Overview:
 *	This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *	
 *
 *****************************************************************************/
/* Revision History:
 * yyyy-mm-dd	modified-by	modifications-done
 * 2005-01-11	K.Kokubun	Created
 * End of History */


#ifndef msGetNRcomment_H__
#define msGetNRcomment_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __FREE_BSD__
status_t msGetNRcomment(uint64_t kspaceNo, uint64_t filterNo, char **comment);
status_t msGetRefineComment(uint64_t kspaceNo, uint64_t filterNo, char **comment);
status_t msGetEMToneComment(uint64_t filterNo, char **comment);
#else
status_t msGetNRcomment(int kspaceNo, int filterNo, char **comment);
status_t msGetRefineComment(int kspaceNo, int filterNo, char **comment);
status_t msGetEMToneComment(int filterNo, char **comment);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif 	/* msGetNRcomment_H__ */

