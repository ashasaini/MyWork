/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield names for the NMRTOOLS subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The NMRTOOLS subspace contains fields associated with the NMR
 *	toolkit.
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
 * 93-04-14	JR		Added phantom size
 * 92-06-26	salil		Consoladated structures & corrected SVSS stuff 
 * 92-06-23	mrk		added JR's def's
 * 92-06-22     salil           Added NMR toolkit VF tagging definitions.
 * 92-06-09	scottm		Created
 * End of History */


#ifndef tami_vfStudy_vfs_nmrtools_H__
#define tami_vfStudy_vfs_nmrtools_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_NMR_FIT       	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 0)
#define SVT_NMR_FIT       	VFT_NMR_FIT
#define SVS_NMR_FIT       	"Curve fit data structure"

#define SVN_NMR_FWHM      	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 1)
#define SVT_NMR_FWHM      	VFT_NMR_FWHM
#define SVS_NMR_FWHM      	"Full Width Half Maximum data structure"

#define SVN_NMR_LFWHM     	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 2)
#define SVT_NMR_LFWHM     	VFT_NMR_FWHM
#define SVS_NMR_LFWHM     	"Local FWHM data structure"

#define SVN_NMR_MAX       	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 3)
#define SVT_NMR_MAX       	VFT_NMR_MINMAX
#define SVS_NMR_MAX       	"Maximum data structure"

#define SVN_NMR_MIN       	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 4)
#define SVT_NMR_MIN       	VFT_NMR_MINMAX
#define SVS_NMR_MIN       	"Minimum data structure"

#define SVN_NMR_SLOPE     	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 5)
#define SVT_NMR_SLOPE     	VFT_NMR_SLOPE
#define SVS_NMR_SLOPE     	"Slope data structure"

#define SVN_NMR_PHANTOM_SIZE	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 6)
#define SVT_NMR_PHANTOM_SIZE	VFT_INT32
#define SVS_NMR_PHANTOM_SIZE    "Phantom size array"

#define SVN_NMR_SIGNAL          (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 7)
#define SVT_NMR_SIGNAL          VFT_NMR_SIGNAL
#define SVS_NMR_SIGNAL          "AQA phantom signal data structure"

#define SVN_NMR_UNIFORM         (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 8)
#define SVT_NMR_UNIFORM         VFT_NMR_UNIFORM
#define SVS_NMR_UNIFORM         "AQA phantom uniformity data structure"

#define SVN_NMR_NOISE           (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 9)
#define SVT_NMR_NOISE           VFT_NMR_NOISE
#define SVS_NMR_NOISE           "AQA phantom noise data structure"

#define SVN_NMR_EDGE_SPREAD     (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 10)
#define SVT_NMR_EDGE_SPREAD     VFT_NMR_EDGE_SPREAD
#define SVS_NMR_EDGE_SPREAD     "AQA phantom edge spread"

#define SVN_NMR_PARALLEL        (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 11)
#define SVT_NMR_PARALLEL        VFT_NMR_PARALLEL
#define SVS_NMR_PARALLEL        "AQA phantom parallelogram"

#define	SVN_NMR_RESOL		(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 12)
#define	SVT_NMR_RESOL		VFT_NMR_RESOL
#define	SVS_NMR_RESOL		"AQA phantom resolution"

#define	SVN_NMR_SLICE_PROF	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 13)
#define	SVT_NMR_SLICE_PROF	VFT_NMR_SLICE_PROF
#define	SVS_NMR_SLICE_PROF	"AQA phantom slice profile"

#define SVN_NMR_SLICE_WALK      (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 14)
#define SVT_NMR_SLICE_WALK      VFT_FLT32
#define SVS_NMR_SLICE_WALK      "AQA phantom slice walking"

#define SVN_NMR_QUADRATURE      (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 15)
#define SVT_NMR_QUADRATURE      VFT_FLT32
#define SVS_NMR_QUADRATURE      "AQA phantom signal quadrature"

#define SVN_NMR_BLEED_REPL      (SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 16)
#define SVT_NMR_BLEED_REPL      VFT_FLT32
#define SVS_NMR_BLEED_REPL      "AQA phantom signal bleeding/replication"

#define	SVN_NMR_ANGLE_OFFSET	(SVN_MIN_INDEPENDENT + SVN_OFF_NMRTOOLS + 17)
#define	SVT_NMR_ANGLE_OFFSET	VFT_FLT32
#define	SVS_NMR_ANGLE_OFFSET	"Phase angle offset"

/*****************************************************************************
  STUDY name space
*/

/* Study Short String description */
/* (same strings as in vfs_structure.h ... but shorter) */

#define	SVS_ST_NMR_STUDY	"study"
#define SVS_ST_NMR_PROTOCOL	"protocol"
#define SVS_ST_NMR_SUBPROTOCOL	"subProtocol"
#define SVS_ST_NMR_SLICEGROUP	"sliceGroup"
#define SVS_ST_NMR_SLICE	"slice"
#define SVS_ST_NMR_IMAGE	"image"
#define SVS_ST_NMR_PROCESSED	"processed"
#define SVS_ST_NMR_ECHO		"echo"
#define SVS_ST_NMR_POSITION	"position"
#define SVS_ST_NMR_IMCALC	"imageCalc"
#define SVS_ST_NMR_FLD_STBL_SL	"fieldStability"

/* Default leaf */

#define SVN_NMR_DEFLEAF 	(SVN_MIN_STUDY + SVN_OFF_NMRTOOLS + 0)
#define SVT_NMR_DEFLEAF		VFT_PATHELEM 
#define SVS_NMR_DEFLEAF   	"default study tree leaf"


/*****************************************************************************
  PROTOCOL name space
*/

/* SM gain: */
/* If this field is present and TRUE do receiver gain prescan using */
/* the SM (instead of the RM) */

#define SVN_NMR_SMGAIN 	(SVN_MIN_PROTOCOL + SVN_OFF_NMRTOOLS + 0)
#define SVT_NMR_SMGAIN	VFT_BOOL
#define SVS_NMR_SMGAIN  "SM receiver gain prescan"

#define SVN_NMR_IMAGE_PHASE_WINDOW_RATIO (SVN_MIN_PROTOCOL + SVN_OFF_NMRTOOLS + 1)
#define SVT_NMR_IMAGE_PHASE_WINDOW_RATIO VFT_FLT32
#define SVS_NMR_IMAGE_PHASE_WINDOW_RATIO "Phase fitting ratio for Hybrid EPI Recon."
/*	Hybrid EPI などで再構成画像上で位相の０次、１次成分の
	フィッティングを行なう範囲が表示上のFOV(マトリクス）に占める割合。

	0.0: フィッティングを行なわない。
	1.0: FOV全体でフィッティングを行なう。
	例）    
	0.5: MX=256,No Wrap なしなら、
	64番目から192番目までのピクセルでフィッティング。
	READOUT,PHASE ENCODEとも共通。
	*/


/*****************************************************************************
  SUBPROTOCOL name space
*/




/*****************************************************************************
  SLICEGROUP/PRESAT name space
*/




/*****************************************************************************
  SLICE name space
*/




/*****************************************************************************
  IMAGE name space
*/




/*****************************************************************************
  PROCESSED name space
*/




/*****************************************************************************
  ECHO name space
*/




/*****************************************************************************
  POSITION name space
*/
#define SVN_NMR_IMAGE_PHASE_COEFF 	(SVN_MIN_POSITION + SVN_OFF_NMRTOOLS + 0)
#define SVT_NMR_IMAGE_PHASE_COEFF	VFT_FLT32
#define SVS_NMR_IMAGE_PHASE_COEFF	"Phase coefficeint degrees for order and channels."
/*	画像の位相分布の各係数（計算結果を3x3の2次元配列として表現する）
	PHASE_COEFF[order][functional channel]
	order              : 0th/1st/dev.
	functional channel : R/E/S           
	order
	0      0 次成分  [deg.]
	　R   readout 
	　E   phase encode
	　S   slice
	
	1      1 次成分   [deg./pixel] 
	　R   readout 
	　E   phase encode 
	　S   slice
	
	2      分散
	　R   readout 
	　E   phase encode 
	　S   slice
	
	注：現状では、2D/3Dでも PHASE_COEFF[0][S],PHASE_COEFF[1][S]は、ア
	ドレスがあるが、値は意味を持たない。
	*/


/*****************************************************************************
  IMCALC name space
*/


#endif /* tami_vfStudy_vfs_nmrtools_H__ */
