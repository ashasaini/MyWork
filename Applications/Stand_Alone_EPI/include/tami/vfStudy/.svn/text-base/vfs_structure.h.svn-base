/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield subtree names for subtrees in
 *	the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	Varfield definitions for the nodes in a study file, and the space
 *	and subspace allocations for varfield definitions, are contained
 *	in this file.
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
 * yy-mm-dd	modified-by	modifications-done
 * 92-06-09	scottm		Created
 * End of History */



#ifndef tami_vfStudy_vfs_structure_H__
#define tami_vfStudy_vfs_structure_H__

/* Required #include files */
#include <common/basicdefs.h>
#include <vf_basictypes.h>
#include <vf_apptypes.h>
#include <vf_appnames.h>


/*****************************************************************************
  Node depths by subtree */

/* Common nodes
*/
#define         SVD_ST_STUDY    	0
#define		SVD_ST_PROTOCOL		1

/* Acquired data nodes
*/
#define		SVD_ST_SUBPROTOCOL	2
#define		SVD_ST_SLICEGROUP	3
#define		SVD_ST_PRESAT		3	/* Same as SLICEGROUP */
#define		SVD_ST_FLD_STBL_SL	3	/* Same as SLICEGROUP */
#define		SVD_ST_SLICE		4
#define		SVD_ST_IMAGE		5

/* Processed data nodes
*/
#define		SVD_ST_PROCESSED	2
#define		SVD_ST_ECHO		3
#define		SVD_ST_POSITION		4

/* Special pan protocol image calculations
   get their own node at the protocol level

   –¢Žg—p
*/
#define		SVD_ST_IMCALC		1

/* Visual groups get their own node at the
   subprotocol level.  Currently, this is
   for the exclusive use of the ProLim and
   the Graphic Locator.
*/
#define		SVD_ST_VISUALGROUP	2

/* Notes on SVD_ST_PRESAT:

   This branch of the tree is nearly the same as the slicegroup branch.
   It is identified separately to distinguish between slice groups and
   presat regions.  The varfields used in the presat branch are a subset
   of those used in the slicegroup branch.
   */


/*****************************************************************************
  Partition the name space
*/

/* N.B. This space must not exceed the space allowed for in
   vf_appnames.h (VFN_APP_RANGE).
*/

/* Name space for node names */
#define         SVN_MIN_NODENAME        VFN_STUDY_BASE +       0

/* Name space for level-independent names */
#define         SVN_MIN_INDEPENDENT     VFN_STUDY_BASE +     100

/* Name spaces for nodes grouped by node
*/
#define         SVN_MIN_STUDY   	VFN_STUDY_BASE +   20000
#define		SVN_MIN_PROTOCOL	VFN_STUDY_BASE +   40000

/*	Acquired subtree	*/
#define         SVN_MIN_SUBPROTOCOL     VFN_STUDY_BASE +   60000
#define         SVN_MIN_SLICEGROUP      VFN_STUDY_BASE +   80000
#define         SVN_MIN_SLICE           VFN_STUDY_BASE +  100000
#define         SVN_MIN_IMAGE           VFN_STUDY_BASE +  120000

/*      Processed subtree       */
#define         SVN_MIN_PROCESSED       VFN_STUDY_BASE +  140000
#define         SVN_MIN_ECHO            VFN_STUDY_BASE +  160000
#define         SVN_MIN_POSITION        VFN_STUDY_BASE +  180000

/*      Calculated images       */
#define		SVN_MIN_IMCALC	        VFN_STUDY_BASE +  200000

/*	Visual groups */
#define		SVN_MIN_VISUALGROUP	VFN_STUDY_BASE +  220000


/* Within each node name space, there is a further partitioning of the
   space into a subspace by application group using the following offsets.
   The appropriate abbreviations are shown in comments following the
   definition.
*/

#define         SVN_OFF_GENERAL		    0	/* GEN */
#define         SVN_OFF_NMRTOOLS	 1000	/* NMR */
#define		SVN_OFF_SCHEDULE	 2000	/* SCH */
#define		SVN_OFF_PLAN		 3000	/* PLN */
#define         SVN_OFF_SEQUENCE	 4000	/* SEQ */
#define		SVN_OFF_EXECUTION	 5000	/* EXE */
#define		SVN_OFF_PROCESS		 6000	/* PRC */
#define		SVN_OFF_PRESENT		 7000	/* PRS */
#define		SVN_OFF_DICOM		 8000	/* DCM */
#define		SVN_OFF_MRS		 9000	/* MRS */


/*****************************************************************************
  NODENAME name space
*/

#define         SVN_ST_STUDY      	(SVN_MIN_NODENAME + 0)
#define		SVS_ST_STUDY		"Patient and Study Information"

#define         SVN_ST_PROTOCOL         (SVN_MIN_NODENAME + 1)
#define         SVS_ST_PROTOCOL         "Protocol subtree"

#define         SVN_ST_SUBPROTOCOL      (SVN_MIN_NODENAME + 2)
#define         SVS_ST_SUBPROTOCOL      "Subprotocol subtree"

#define         SVN_ST_SLICEGROUP       (SVN_MIN_NODENAME + 3)
#define         SVS_ST_SLICEGROUP       "Slice group subtree"

#define		SVN_ST_PRESAT		(SVN_MIN_NODENAME + 4)
#define		SVS_ST_PRESAT		"Presat subtree"

#define         SVN_ST_SLICE            (SVN_MIN_NODENAME + 5)
#define         SVS_ST_SLICE            "Slice subtree"

#define         SVN_ST_IMAGE            (SVN_MIN_NODENAME + 6)
#define         SVS_ST_IMAGE            "Image subtree"

#define         SVN_ST_PROCESSED        (SVN_MIN_NODENAME + 7)
#define         SVS_ST_PROCESSED        "Processed subtree"

#define         SVN_ST_ECHO             (SVN_MIN_NODENAME + 8)
#define         SVS_ST_ECHO             "Echo subtree"

#define         SVN_ST_POSITION         (SVN_MIN_NODENAME + 9)
#define         SVS_ST_POSITION         "Position subtree"

#define         SVN_ST_IMCALC           (SVN_MIN_NODENAME + 10)
#define         SVS_ST_IMCALC           "Image calc. subtree"

#define		SVN_ST_VISUALGROUP	(SVN_MIN_NODENAME + 11)
#define		SVS_ST_VISUALGROUP	"Visual group subtree"

#define		SVN_ST_FLD_STBL_SL	(SVN_MIN_NODENAME + 12)
#define		SVS_ST_FLD_STBL_SL	"Field stability slice subtree"

#endif /* tami_vfStudy_vfs_structure_H__ */
