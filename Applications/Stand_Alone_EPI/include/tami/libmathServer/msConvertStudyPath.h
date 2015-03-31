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
 * 2002-01-28     Y.Watanabe      Created.
 * End of History */



#ifndef msConvertStudyPath_H__
#define msConvertStudyPath_H__

#ifdef __cplusplus
extern "C" {
#endif

status_t msConvertStudyPath(msGetConfig_t *config,
			    char          *studyName,
			    char          *convName);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* msConvertStudyPath_H__ */

