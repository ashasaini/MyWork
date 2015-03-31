/******************************************************************************
 *  File Name:		$Source: /mri/cvsroot/develop/src.include/common/globalSwitches.h,v $  ($Revision: 1.1.28.1.2.3 $)
 *  Last Modified:	$Date: 2003/03/31 07:25:10 $
 *  Revision Log:	$Log: globalSwitches.h,v $
 *  Revision Log:	Revision 1.1.28.1.2.3  2003/03/31 07:25:10  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.4  2003/01/02 13:05:14  develop
 *  Revision Log:	Merged V5.40 changes in V5.00 code
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.22.1  2002/11/27 02:14:23  tate
 *  Revision Log:	Loc
 *  Revision Log:	
 *  Revision Log:	Revision 1.1  2000/05/16 10:15:03  full
 *  Revision Log:	Change include/common
 *  Revision Log:
 *  Revision Log:	Revision 1.2  2000/05/16 08:37:51  full
 *  Revision Log:	Symbolic Link
 *  Revision Log:	
 * Revision 1.7  93/03/31  13:50:23  arkady
 * Removed lots of PROEDIT items. Added PROCON items.
 * 
 * Revision 1.6  92/12/10  10:29:03  ricardo
 * Added pixmap directory options needed for the protocol mover.
 * 
 * Revision 1.5  92/12/01  14:34:04  ricardo
 * Added switch needed for release tree in the protocol mover.
 * 
 * Revision 1.4  92/09/29  22:03:42  lib
 * Converted to the new directory structure
 * 
 * Revision 1.3  92/09/03  11:29:41  miket
 * added PQM and Memory switches
 * 
 * Revision 1.2  92/09/03  10:55:35  arkady
 * Added "scratchPad" to proEdit
 * 
 * Revision 1.1  92/08/10  15:00:25  arkady
 * Initial revision
 * 
 *  
 *    File Overview:
 *	This file #defines command line switch strings, resource names
 *	and resource classes
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 91-03-31	arkady		Created
 * End of History */



#ifndef tami_common_globalSwitches_H__
#define tami_common_globalSwitches_H__

/******************************************************************************
 *
 * "#defines" in this file  are for shared use by X applicatons to:
 *
 *	retrieve command line arguments and map them to resources
 *
 *	retrieve resources
 *
 *	environment variables
 *
 *	costruct commands to envoke other applications
 *	
 *******************************************************************************
 *
 * All items needed to initilize XrmOptionDescRec structure
 * and XtResource structure are defined in this file.
 *
 * Note:	The XrmOptionDescRec structure is used in XtAppInitialize()
 * 		and XtOpenDisplay() to match command line arguments with
 *		resources.
 *
 * 		The XtResource structure to use with XtGetApplicationResources()
 *
 *******************************************************************************
 *
 * The defines are:	GSN_...	resorce Name
 *			GSC_...	resource Class
 *			GST_... resource Type (one of XtR...)
 *			GSD_... resource Default for XtResource structure,
 *				MUST BE of type XtRImmediate
 *			GSW_...	command line argument (sWitch)
 *			GSK_... argument Kind (one of Xrmoption...)
 *			GSV_... value to provide if GSK_ is XrmoptionNoArg
 *				else define as NULL
 *			GSE_... Envoronment variable, if applicable
 *
 *			GS_EXECUTABLE_... names of executables.
 *
 * Usage in structures (see macros below):
 *
 * XtResource . resource_name	= GSN_...
 * XtResource . resource_class	= GSC_...
 * XtResource . resource_type	= GST_...
 * XtResource . resource_size	=
 * XtResource . resource_offset	=
 * XtResource . default_type	= XtRImmediate
 * XtResource . default_address	= GSD_...
 *
 * XrmOptionDescRec . option    = GSW_...
 * XrmOptionDescRec . specifier = GSN_...
 * XrmOptionDescRec . argKind   = GSK_...
 * XrmOptionDescRec . value     = GSV_...
 *
 *******************************************************************************
 *
 * The folowing rules are adopted for resource/switch strings:
 *
 *	Use full words for swithces and resources.
 *
 *	Resource names begin with a lower case character.
 *
 *	Class is resource name with first character capitalized.
 *
 *	Preference is given to hungarian spelling. E.g.: subProtocol.
 *
 *	Switch words MUST match the resource names. In other words
 *	switch *is* resource name prefixed with a "-".
 *
 *******************************************************************************
 * PS: This file can and should be used as on-line documentation for
 *     command line arguments and resources that can be specified on
 *     the command line (is it the same as all application specific
 *     resources?).
 *****************************************************************************/

/******************************************************************************
 *
 * Macro to initilize XrmOptionDescRec structure: GS_XtResource(x)
 * Usage:
 *   static XrmOptionDescRec options[] = {
 *	 { GS_OptionDescRec(STUDY) },
 *	 { GS_OptionDescRec(PROTOCOL) },
 *	 { GS_OptionDescRec(SUBPROTOCOL) }
 *   };
 * ****************************************************************************/

#define GS_OptionDescRec(x) \
    GSW_ ## x, GSN_ ## x, GSK_ ## x, (caddr_t) (GSV_ ## x)

/******************************************************************************
 *
 * Macro to initilize part of XtResource structure: GS_XtResource(x)
 * Usage:
 *   static XtResource resources[] = {
 *       {GS_XtResource(STUDY,       sizeof(), XtOffset()) },
 *       {GS_XtResource(PROTOCOL,    sizeof(), XtOffset()) },
 *       {GS_XtResource(SUBPROTOCOL, sizeof(), XtOffset()) },
 *   };
 *****************************************************************************/

#define GS_XtResource(x,size,offset)  \
    GSN_ ## x, GSC_ ## x, GST_ ## x, size, offset, XtRImmediate, \
    (XtPointer) (GSD_ ## x)

/******************************************************************************
 * resources/switches common to all applications
 *****************************************************************************/

/* Debug flag */
#define GSN_DEBUG		"debug"
#define GSC_DEBUG		"Debug"
#define GST_DEBUG		XtRBoolean
#define GSD_DEBUG		FALSE
#define GSW_DEBUG		"-" GSN_DEBUG
#define GSK_DEBUG		XrmoptionNoArg
#define GSV_DEBUG		"on"
#define GSE_DEBUG		"DEBUG"

/* Fluoro flag */
#define GSN_FLUORO		"fluoro"
#define GSC_FLUORO		"Fluoro"
#define GST_FLUORO		XtRBoolean
#define GSD_FLUORO		FALSE
#define GSW_FLUORO		"-" GSN_FLUORO
#define GSK_FLUORO		XrmoptionNoArg
#define GSV_FLUORO		"on"
#define GSE_FLUORO		"FLUORO"

/* Help flag */
#define GSN_HELP		"help"
#define GSC_HELP		"Help"
#define GST_HELP		XtRBoolean
#define GSD_HELP		FALSE
#define GSW_HELP		"-" GSN_HELP
#define GSK_HELP		XrmoptionNoArg
#define GSV_HELP		"on"
#define GSE_HELP		"HELP"

/*	pointer to study file */
#define GSN_STUDY		"study"
#define GSC_STUDY		"Study"
#define GST_STUDY		XtRString
#define GSD_STUDY		NULL
#define GSW_STUDY		"-" GSN_STUDY
#define GSK_STUDY		XrmoptionSepArg
#define GSV_STUDY		NULL
#define GSE_STUDY		"STUDY"

/*	Node number of protocol */
#define GSN_PROTOCOL		"protocol"
#define GSC_PROTOCOL		"Protocol"
#define GST_PROTOCOL		XtRInt
#define GSD_PROTOCOL		0
#define GSW_PROTOCOL		"-" GSN_PROTOCOL
#define GSK_PROTOCOL		XrmoptionSepArg
#define GSV_PROTOCOL		NULL
#define GSE_PROTOCOL		"PROTOCOL"

/*	Node number of sub-protocol */
#define GSN_SUBPROTOCOL		"subProtocol"
#define GSC_SUBPROTOCOL		"SubProtocol"
#define GST_SUBPROTOCOL		XtRInt
#define GSD_SUBPROTOCOL		0
#define GSW_SUBPROTOCOL		"-" GSN_SUBPROTOCOL
#define GSK_SUBPROTOCOL		XrmoptionSepArg
#define GSV_SUBPROTOCOL		NULL
#define GSE_SUBPROTOCOL		"SUBPROTOCOL"

/*	Read file descriptor for communicating with parent process. */
#define GSN_READFD		"readFD"
#define GSC_READFD		"ReadFD"
#define GST_READFD		XtRInt
#define GSD_READFD		(-1)
#define GSW_READFD		"-" GSN_READFD
#define GSK_READFD		XrmoptionSepArg
#define GSV_READFD		NULL

/* Write file descriptor for communicating with parent process. */
#define GSN_WRITEFD		"writeFD"
#define GSC_WRITEFD		"WriteFD"
#define GST_WRITEFD		XtRInt
#define GSD_WRITEFD		(-1)
#define GSW_WRITEFD		"-" GSN_WRITEFD
#define GSK_WRITEFD		XrmoptionSepArg
#define GSV_WRITEFD		NULL

/* "Root" directory for sequences */
#define GSN_SEQDIR		"sequenceDirectory"
#define GSC_SEQDIR		"SequenceDirectory"
#define GST_SEQDIR		XtRString
#define GSD_SEQDIR		NULL
#define GSW_SEQDIR		"-" GSN_SEQDIR
#define GSK_SEQDIR		XrmoptionSepArg
#define GSV_SEQDIR		NULL
#define GSE_SEQDIR		"SEQDIR"

/*	Search path for sequences */
#define GSN_SEQPATH		"sequencePath"
#define GSC_SEQPATH		"SequencePath"
#define GST_SEQPATH		XtRString
#define GSD_SEQPATH		NULL
#define GSW_SEQPATH		"-" GSN_SEQPATH
#define GSK_SEQPATH		XrmoptionSepArg
#define GSV_SEQPATH		NULL
#define GSE_SEQPATH		"SEQPATH"

/* "Root" directory for study files */

#define GSN_STUDYDIR		"studyDirectory"
#define GSC_STUDYDIR		"StudyDirectory"
#define GST_STUDYDIR		XtRString
#define GSD_STUDYDIR		NULL
#define GSW_STUDYDIR		"-" GSN_STUDYDIR
#define GSK_STUDYDIR		XrmoptionSepArg
#define GSV_STUDYDIR		NULL
#define GSE_STUDYDIR		"STUDYDIR"


/* "Root" directory for release study files */

#define GSN_RELSTUDYDIR		"relStudyDirectory"
#define GSC_RELSTUDYDIR		"RelStudyDirectory"
#define GST_RELSTUDYDIR		XtRString
#define GSD_RELSTUDYDIR		NULL
#define GSW_RELSTUDYDIR		"-" GSN_RELSTUDYDIR
#define GSK_RELSTUDYDIR		XrmoptionSepArg
#define GSV_RELSTUDYDIR		NULL
#define GSE_RELSTUDYDIR		"STUDYDIR"


/* Search path for study files */
#define GSN_STUDYPATH		"studyPath"
#define GSC_STUDYPATH		"StudyPath"
#define GST_STUDYPATH		XtRString
#define GSD_STUDYPATH		NULL
#define GSW_STUDYPATH		"-" GSN_STUDYPATH
#define GSK_STUDYPATH		XrmoptionSepArg
#define GSV_STUDYPATH		NULL
#define GSE_STUDYPATH		"STUDYPATH"

/* Window Id of the Invoking process.*/
#define GSN_WIN_ID		"win"
#define GSC_WIN_ID		"Win"
#define GST_WIN_ID		XtRInt
#define GSD_WIN_ID		0
#define GSW_WIN_ID		"-" GSN_WIN_ID
#define GSK_WIN_ID		XrmoptionSepArg
#define GSV_WIN_ID		NULL

/* AutoView flag */
#define GSN_AUTOVIEW		"autoview"
#define GSC_AUTOVIEW		"Autoview"
#define GST_AUTOVIEW		XtRBoolean
#define GSD_AUTOVIEW		FALSE
#define GSW_AUTOVIEW		"-" GSN_AUTOVIEW
#define GSK_AUTOVIEW		XrmoptionNoArg
#define GSV_AUTOVIEW		"on"
#define GSE_AUTOVIEW		"AUTOVIEW"

/******************************************************************************
 * resources/switches for individual applications applications
 * (to be shared by the application and whoever envokes it)
 *****************************************************************************/

/*
 * pqm
 */

#define GS_EXECUTABLE_PQM	"pqm"

/* PQM envocation mode.
   0 = Scheduling; 1 = Imaging
   */
#define GSN_PQM_MODE		"mode"
#define GSC_PQM_MODE		"Mode"
#define GST_PQM_MODE		XtRInt
#define GSD_PQM_MODE		0
#define GSW_PQM_MODE		"-" GSN_PQM_MODE
#define GSK_PQM_MODE		XrmoptionSepArg
#define GSV_PQM_MODE		NULL

/*
 * Memory
 */

#define GS_EXECUTABLE_MEMORY	"memory"

/* Memory Invocation Mode */
#define GSN_MEMORY_MODE		"mode"
#define GSC_MEMORY_MODE		"Mode"
#define GST_MEMORY_MODE		XtRString
#define GSD_MEMORY_MODE		"recall"
#define GSW_MEMORY_MODE		"-" GSN_MEMORY_MODE
#define GSK_MEMORY_MODE		XrmoptionSepArg
#define GSV_MEMORY_MODE		NULL

/* Memory Anatomical Class */
#define GSN_MEMORY_CLASS	"class"
#define GSC_MEMORY_CLASS	"Class"
#define GST_MEMORY_CLASS	XtRInt
#define GSD_MEMORY_CLASS	-1
#define GSW_MEMORY_CLASS	"-" GSN_MEMORY_CLASS
#define GSK_MEMORY_CLASS	XrmoptionSepArg
#define GSV_MEMORY_CLASS	NULL

/* Memory Protocol Set */
#define GSN_MEMORY_SET  	"set"
#define GSC_MEMORY_SET  	"Set"
#define GST_MEMORY_SET  	XtRInt
#define GSD_MEMORY_SET  	-1
#define GSW_MEMORY_SET  	"-" GSN_MEMORY_SET
#define GSK_MEMORY_SET  	XrmoptionSepArg
#define GSV_MEMORY_SET  	NULL

/* directory for release pixmaps */

#define GSN_RELEASEPIXMAPS	"releasePixmaps"
#define GSC_RELEASEPIXMAPS	"ReleasePixmaps"
#define GST_RELEASEPIXMAPS 	XtRString
#define GSD_RELEASEPIXMAPS	NULL
#define GSW_RELEASEPIXMAPS 	"-" GSN_RELEASEPIXMAPS
#define GSK_RELEASEPIXMAPS 	XrmoptionSepArg
#define GSV_RELEASEPIXMAPS 	NULL
#define GSE_RELEASEPIXMAPS	"RELEASEPIXMAPS"

/* directory for site (local) pixmaps */
#define GSN_SITEPIXMAPS     	"sitePixmaps"
#define GSC_SITEPIXMAPS 	"sitePixmaps"
#define GST_SITEPIXMAPS 	XtRString
#define GSD_SITEPIXMAPS		NULL
#define GSW_SITEPIXMAPS 	"-" GSN_SITEPIXMAPS
#define GSK_SITEPIXMAPS 	XrmoptionSepArg
#define GSV_SITEPIXMAPS 	NULL
#define GSE_SITEPIXMAPS		"SITEPIXMAPS"

/*
 * proCon.
 */

#define GS_EXECUTABLE_PROCON	"proCon"

/* temporary copy of study file containing the protocol to edit */
/* this one might also be used by X applications later on */
#define GSN_PROCON_SCRATCHPAD	"scratchPad"
#define GSC_PROCON_SCRATCHPAD	"ScratchPad"
#define GST_PROCON_SCRATCHPAD	XtRString
#define GSD_PROCON_SCRATCHPAD	NULL
#define GSW_PROCON_SCRATCHPAD	"-" GSN_PROCON_SCRATCHPAD
#define GSK_PROCON_SCRATCHPAD	XrmoptionSepArg
#define GSV_PROCON_SCRATCHPAD	NULL

/* name of user interface to start. multiple "-ui xxx" allowed */
#define GSW_PROCON_UI		"-ui"
/* restart a user interface if it exits abnormaly */
#define GSW_PROCON_RESTART      "-restart"
/* allow multiole invocations of the same ui */
#define GSW_PROCON_ALLOW_DUPL	"-allowDupl"
						   
/*
 * proEdit
 */

#define GS_EXECUTABLE_PROEDIT	"proEdit"

/* "dictionary" to define selections in proEdit */
#define GSN_PROEDIT_DICTIONARY	"dictionary"
#define GSC_PROEDIT_DICTIONARY	"Dictionary"
#define GST_PROEDIT_DICTIONARY	XtRString
#define GSD_PROEDIT_DICTIONARY	"proEditDictionary"
#define GSW_PROEDIT_DICTIONARY	"-" GSN_PROEDIT_DICTIONARY
#define GSK_PROEDIT_DICTIONARY	XrmoptionSepArg
#define GSV_PROEDIT_DICTIONARY	NULL
#define GSE_PROEDIT_DICTIONARY  "PROEDIT_DICTIONARY"

/* timeout interval to turn watch cursor on */
#define GSN_PROEDIT_WATCHTIME	"watchTime"
#define GSC_PROEDIT_WATCHTIME	"WatchTime"
#define GST_PROEDIT_WATCHTIME	XtRInt
#define GSD_PROEDIT_WATCHTIME	NULL
#define GSW_PROEDIT_WATCHTIME	"-" GSN_PROEDIT_WATCHTIME
#define GSK_PROEDIT_WATCHTIME	XrmoptionSepArg
#define GSV_PROEDIT_WATCHTIME	50

    
#endif /* tami_common_globalSwitches_H__ */
