/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield subtree names for subtrees in
 *	the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *    Assumptions:
 *	These values are not in the range of
 *		VFN_MIN_RESERVED...VFN_MAX_RESERVED
 *
 *    Objects with external linkage defined in this file:
 *	
 *
 *****************************************************************************/
/* Revision History:
 * 92-06-09	scottm		Major restructuring.  Changed name subspaces,
                                split into separate (vfs_*.h) files.
 * 92-05-19	rjg		Corrected typo.  Corrected type and comments
 *				for some fields.
 * 92-04-29	rjg		Corrected placement of some fields.
 * 92-04-27	rjg		Added fields needed by the PQM.
 * 92-04-23	scottm		Comments, formatting.
 * 92-04-16	rjg		Added fields needed for ProSel.
 * 92-04-13	rickm		Added fields needed for New Echo calculation.
 *				(First change under RCS).
 * 92-03-26	scottm		Added SVN_FEP.
 * 92-02-19	jcw		Remove double "?" to avoid inadvertant trigraph
 * 92-01-27	jcw		Move SVN_HOLD and SVN_TOTAL_TIME from
 *				study to protocol.
 * 92-01-14	rickm		Added SVN_RP_SCALE_FLAG, fixed some typos.
 * 92-01-03	sdm		Restored SVN_ST_STUDY.
 * 91-12-18	jcw		Change RFL_ATTEN to RFL_GAIN
 * 91-12-11	jcw		Add more definitions for prescan.
 * 91-12-04	jcw		Added more definitions for acquisition.
 *				Changed PATIENT to STUDY
 * 91-11-30	rickm		Moved type definitions to vf_apptypes.h,
 *				added more Phase I definitions.
 * 91-11-21	scottm		Created name ranges and initial Phase I
 *				definitions.  Changed VFx to SVx.
 * 91-11-19	jeth		Changed VFN_ST_RECON, VFN_ST_ORIENTATION to
 *				VFN_ST_PROCESSED and VFN_ST_ECHO.
 * 91-10-29	jeth		Created
 * End of History */

#ifndef tami_vfStudy_vfstudy_H__
#define tami_vfStudy_vfstudy_H__

/* Prefixes used in this file:

	SVN	Study Varfield Name
	SVD	Study Varfield node Depth
	SVT	Study Varfield Type
	SVS	Study Varfield String description

   The proper naming convention for a field definition is:

        SVx_app_name

   where x is one of N, D, T, or S; app is one of the name subspaces
   defined in vfs_structure.h (e.g. PLN for PLAN); and name is the name of
   the varfield definition.

   Older varfield definitions may not strictly adhere to this convention;
   All new definitions SHOULD adhere to it (and older ones should be updated
   whenever possible).
*/

/*
  #include the files that, together, define the study varfields.
*/

/* Major Version * 100 + Minor version */
#define	VFC_STUDY_VERSION	(200)

#include <vfs_structure.h>	/* This must be the first #included */
#include <vfs_general.h>
#include <vfs_nmrtools.h>
#include <vfs_schedule.h>
#include <vfs_plan.h>
#include <vfs_sequence.h>
#include <vfs_execution.h>
#include <vfs_process.h>
#include <vfs_present.h>
#include <vfs_dicom.h>
#include <vfs_mrs.h>


#endif /* tami_vfStudy_vfstudy_H__ */
