/******************************************************************************
 *  File Name:		libIrs/IrsEventFields.h
 *
 *  File Overview:
 *	This file contains the Information Registry Service (IRS) event
 *	field definitions.
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 93-02-02	hbw		Created.
 * End of History */


#ifndef IrsEventFields_h__
#define IrsEventFields_h__


/*-----------------------------------------------------------------------------
 *
 *	Event field types
 *
 *---------------------------------------------------------------------------*/

/* Event Field Types */


#define IRS_EF_ACQMAN		10000

#define IRS_EF_DISPLAY		20000

#define IRS_EF_FILEMAN		30000

#define IRS_EF_FILMQDAEMON	40000

#define	IRS_EF_FMDAEMON		50000

#define IRS_EF_MEMORY		60000

#define IRS_EF_PDET		70000

#define IRS_EF_PQM		80000

#define IRS_EF_UTIL		90000

#define IRS_EF_CONFIG	100000


#define IRS_EF_EVENT		101	/* obsolete from FileMan */
#define	IRS_EF_IMAGELIST	102	/* obsolete from FileMan */
#define IRS_EF_QUEUE		103	/* obsolete from FileMan */
#define IRS_EF_SIZE		104	/* obsolete from FileMan */
#define IRS_EF_STUDYLIST	105	/* obsolete from FileMan */

#define	IRS_EF_ACQBUTTONS		106	/* acquire buttons */

#define IRS_EF_ACQ_STATE		107	/* state of acqman */

#define IRS_EF_BBOX_LEDS		108	/* acquire buttons box LEDs */
#define IRS_EF_BBOX_BUTTON_PRESS	109	/* acquire buttons box
						   pressed */

#define	IRS_EF_HELIUM_LEVEL	110	/* helium level from acqman */

#define IRS_EF_IMAGE_ADDED	111	/* recon<->display image added */
#define IRS_EF_LAST_IMAGE_DONE	112	/* recon<->display last image done */


#define IRS_EF_SHUT		113	/* shutdown message from utilities */

#define IRS_EF_STUDY		114	/* study file */

#define	IRS_EF_VF_PATH		115	/* varfield path */
#define	IRS_EF_VF_PATHLENGTH	116	/* varfield length */

#define IRS_EF_FILMQ     	117 /* map the camera queue window */

#define IRS_EF_FILEMAN_RESTART 	118  /* restart fileman */

#define IRS_EF_DIAGNOSTIC_STATE 119 /* service diagnostics are on */

#define IRS_EF_PQM_STATE	120  /* request for open study file */

#define IRS_EF_PQM_TYPE		121  /* pqm type; IMAGING/SCHEDULING */

#define IRS_EF_UTL_STAT_CLOSE	122  /* utilities' status menu closing*/

#define IRS_EF_CMAP_CHANGED	123  /* iselector and display */

#define IRS_EF_SCAN_START       124  /* scan start event */

#ifdef __WNP_MPLUS__
// new events added. When IRS receives messages from CFW applications,
// it adds IRS_EF_CFW_OFFSET to original message to create new message ID

#define IRS_EF_CFW_OFFSET       100  /* offset for message from cfw applications */

#define CFW_EF_IMAGE_ADDED      (IRS_EF_CFW_OFFSET+IRS_EF_IMAGE_ADDED)

#define CFW_EF_LAST_IMAGE_DONE  (IRS_EF_CFW_OFFSET+IRS_EF_LAST_IMAGE_DONE)

#define CFW_EF_VF_PATH          (IRS_EF_CFW_OFFSET+IRS_EF_VF_PATH)

#define CFW_EF_VF_PATHLENGTH    (IRS_EF_CFW_OFFSET+IRS_EF_VF_PATHLENGTH)

#define CFW_EF_MIN_VALUE	CFW_EF_IMAGE_ADDED

#define CFW_EF_MAX_VALUE	(IRS_EF_CFW_OFFSET+IRS_EF_SCAN_START)

#endif

#endif /* IrsEventFields_h__ */
