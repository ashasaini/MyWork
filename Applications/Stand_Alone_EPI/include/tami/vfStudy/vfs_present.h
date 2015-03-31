/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield names for the PRESENT subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The PRESENT subspace contains fields associated with the presentation
 *	of MRI data, whether on a display monitor or on film.
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
 * 92-11-02	scottm		Added SVN_PRS_IMAGE_MATRIX_ICON_TYPE,
 *				      SVN_PRS_IMAGE_MATRIX_ICON_DIM.
 * 92-11-02	scottm		Added SVN_PRS_IMAGE_MATRIX_ICON.
 * 92-06-09	scottm		Created
 * End of History */



#ifndef tami_vfStudy_vfs_present_H__
#define tami_vfStudy_vfs_present_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_BASE_INDEPENDENT_PRESENT  SVN_MIN_INDEPENDENT + SVN_OFF_PRESENT

#define SVN_PRS_TAG_DATA		(SVN_BASE_INDEPENDENT_PRESENT + 0)
#define SVT_PRS_TAG_DATA                VFT_INT8
#define SVS_PRS_TAG_DATA                "tag data "
/*  This is the name for the Tag structure. It's written and read by the
 *  virtual film library. It's a chunck of memory which has to be casted
 *  to the right structures.
 */

#define SVN_PRS_TAG_ZOOM                (SVN_BASE_INDEPENDENT_PRESENT + 1)
#define SVT_PRS_TAG_ZOOM                VFT_ZOOM_STRUCT
#define SVS_PRS_TAG_ZOOM                "tag zoom"
/*	Zoom is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_TAG_TRANSFORM               (SVN_BASE_INDEPENDENT_PRESENT + 2)
#define SVT_PRS_TAG_TRANSFORM               VFT_TRANSFORM_STRUCT
#define SVS_PRS_TAG_TRANSFORM               "tag rotation"
/*	Rotation is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_TAG_SCROLL                (SVN_BASE_INDEPENDENT_PRESENT + 3)
#define SVT_PRS_TAG_SCROLL                VFT_SCROLL_STRUCT
#define SVS_PRS_TAG_SCROLL                "tag scroll"
/*	Scroll is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_TAG_CONTRAST                (SVN_BASE_INDEPENDENT_PRESENT + 4)
#define SVT_PRS_TAG_CONTRAST                VFT_CONTRAST_STRUCT
#define SVS_PRS_TAG_CONTRAST                "tag contrast"
/*	Contrast is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_TAG_GRID                (SVN_BASE_INDEPENDENT_PRESENT + 5)
#define SVT_PRS_TAG_GRID                VFT_GRID_STRUCT
#define SVS_PRS_TAG_GRID                "tag grid"
/*	Grid is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_TAG_CROP                (SVN_BASE_INDEPENDENT_PRESENT + 6)
#define SVT_PRS_TAG_CROP                VFT_CROP_STRUCT
#define SVS_PRS_TAG_CROP                "tag crop"
/*	Crop is a part of the view attributes of an image which can be saved.
	*/

#define SVN_PRS_IMAGE_MATRIX_ICON	(SVN_BASE_INDEPENDENT_PRESENT + 7)
#define SVS_PRS_IMAGE_MATRIX_ICON	"Image Matrix icon"
/*	Each SVN_GEN_DATA stored in the SVN_ST_PROCESSED branch of the
	study file can have a SVN_PRS_IMAGE_MATRIX_ICON stored with it.
	This ICON is generally produced during image reconstruction/processing,
	and is used to represent the image in the image matrix during display,
	etc.
	*/

#define SVN_PRS_IMAGE_MATRIX_ICON_TYPE	(SVN_BASE_INDEPENDENT_PRESENT + 8)
#define SVT_PRS_IMAGE_MATRIX_ICON_TYPE	VFT_INT32
#define SVS_PRS_IMAGE_MATRIX_ICON_TYPE	"Image Matrix icon data type"
/*	The data type of the icon stored in a corresponding
	SVN_PRS_IMAGE_MATRIX_ICON field.
	*/

#define SVN_PRS_IMAGE_MATRIX_ICON_DIM	(SVN_BASE_INDEPENDENT_PRESENT + 9)
#define SVT_PRS_IMAGE_MATRIX_ICON_DIM	VFT_INT32
#define SVS_PRS_IMAGE_MATRIX_ICON_DIM	"Image Matrix icon dimensions"
/*	A field containing the dimension of the SVN_PRS_IMAGE_MATRIX_ICON,
	The icon is assumed to be square.
	*/

#define SVN_PRS_SLICE_ID_START		(SVN_BASE_INDEPENDENT_PRESENT + 10)
#define SVT_PRS_SLICE_ID_START		VFT_INT32
#define SVS_PRS_SLICE_ID_START		"Slice id of first slice in slicegroup"
/*
 * Id. for first slice in slice group. Actual value stored is
 * (Id. * PL_SLICE_STEP (plPar.h)). See SVN_PRS_SLICE_ID below.
 */

#define SVN_PRS_SLICE_ID_STEP		(SVN_BASE_INDEPENDENT_PRESENT + 11)
#define SVT_PRS_SLICE_ID_STEP		VFT_INT32
#define SVS_PRS_SLICE_ID_STEP		"Slice id step"
/*
 * Slice id. increment for sucessive slices in a slicegroup.
 * Actual value stored is (increment * PL_SLICE_STEP (plPar.h))
 * The increment is sequence dependent e.g. standard spin echo
 * has a slice increment of 1 while quad scan has a slice increment
 * of 4.
 */

#define SVN_PRS_SLICE_ID_VG_ID		(SVN_BASE_INDEPENDENT_PRESENT + 12)
#define SVT_PRS_SLICE_ID_VG_ID		VFT_INT32
#define SVS_PRS_SLICE_ID_VG_ID		"Id of vg for this slice group"
/*
 * Id of visual group to which this slicegroup belongs.
 */

#define SVN_PRS_SLICE_ID		(SVN_BASE_INDEPENDENT_PRESENT + 13)
#define SVT_PRS_SLICE_ID		VFT_INT32
#define SVS_PRS_SLICE_ID		"Slice id."
/*
 * Unique identification for each slice. The ids are generated by
 * prolim and stored at the slice group level using the ..._START
 * and ..._STEP fields defined above. The reconstruction process uses
 * the ..._START and ..._STEP values to generate an id for each image.
 * Currently this id is stored with the image at the POSITION node.
 *
 * The id is a decimal number with log10(PL_SLICE_STEP) decimal places.
 * The actual value stored is (slice id. * PL_SLICE_STEP (plPar.h)),
 * so that the id can be stored as an integer.
 *
 * The id. is displayed by the image selector and connects the image
 * with the slice positions shown by the graphic locator. If the
 * reconstruction process changes the slice position in some way, or adds
 * new slices, then the fractional part of the slice id. will change
 * accordingly. e.g. For acquired data with ids. 1.0, 2.0, 3.0, ... that is
 * shifted in the slice direction by 0.3 of the slice center to center
 * distance, during reconstruction, the processed slice ids. will be
 * 1.3, 2.3, 3.3, ...
 * If the midslice option is selected for reconstruction the the processed
 * slice ids. will be 1.0, 1.5, 2.0, 2.5, 3.0, ...
 */

#define SVN_PRS_CONTRAST_MIN		(SVN_BASE_INDEPENDENT_PRESENT + 14)
#define SVT_PRS_CONTRAST_MIN		VFT_FLT32
#define SVS_PRS_CONTRAST_MIN		"minimum pixel value for contrast"
/*	輝度の決定するための画素最小値
 */

#define SVN_PRS_CONTRAST_MAX		(SVN_BASE_INDEPENDENT_PRESENT + 15)
#define SVT_PRS_CONTRAST_MAX		VFT_FLT32
#define SVS_PRS_CONTRAST_MAX		"maximum pixel value for contrast"
/*	輝度の決定するための画素最大値
 */

#define SVN_PRS_IMG_INFO_COMMENT	(SVN_BASE_INDEPENDENT_PRESENT + 16)
#define SVT_PRS_IMG_INFO_COMMENT	VFT_UINT8
#define SVS_PRS_IMG_INFO_COMMENT	"Functional image comment"
/*	脳機能イメージング後処理（統計処理）時のコメント
 */


/*****************************************************************************
  STUDY name space
*/


/*****************************************************************************
  PROTOCOL name space
*/

#define SVN_BASE_PROTOCOL_PRESENT  SVN_MIN_PROTOCOL + SVN_OFF_PRESENT

#define SVN_PRS_FILM_LAYOUT		(SVN_BASE_PROTOCOL_PRESENT + 0)
#define SVT_PRS_FILM_LAYOUT		VFT_STRING
#define SVS_PRS_FILM_LAYOUT		"Film Layout"
/*  This is the name of the film layout to use when generating film of
 *  images in this protocol.  The film layout is contained in a file
 *  in the display data directory under FilmLayout/.
 */


#define	SVN_PRS_RECVR_COIL		(SVN_BASE_PROTOCOL_PRESENT + 1)
#define	SVT_PRS_RECVR_COIL		VFT_STRING
#define	SVS_PRS_RECVR_COIL		"Receiver Coil"
/*	受信コイル。
	*/

#define	SVN_PRS_TRANSMIT_COIL		(SVN_BASE_PROTOCOL_PRESENT + 2)
#define	SVT_PRS_TRANSMIT_COIL		VFT_STRING
#define	SVS_PRS_TRANSMIT_COIL		"Transmiter Coil"
/*	送信コイル。
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

#define SVN_PRS_TRANSFORM	(SVN_MIN_POSITION+SVN_OFF_PRESENT + 0)
#define SVT_PRS_TRANSFORM	VFT_IMAGE_TRANSFORM_STRUCT
#define SVS_PRS_TRANSFORM	"Image Transform"
/*	画像変換記憶領域。View にて行なった画像変換情報を記憶する。
	SVN_PRS_TAG_TRANSFORM があったが拡張性に乏しいため、新規
	フィールドを作成。以前のフィールドは互換性維持のため V4.0
	まで保存する。
	*/

#define SVN_PRS_CONTRAST	(SVN_MIN_POSITION+SVN_OFF_PRESENT + 1)
#define SVT_PRS_CONTRAST	VFT_IMAGE_CONTRAST
#define SVS_PRS_CONTRAST	"Image Contrast"
/*	画像輝度記憶領域。View にて行なった輝度変換情報を記憶する。
	SVN_PRS_TAG_CONTRAST があったが拡張性に乏しいため、新規
	フィールドを作成。以前のフィールドは互換性維持のため V4.0
	まで保存する。
	*/

#define SVN_PRS_TAG_ROI		(SVN_MIN_POSITION+SVN_OFF_PRESENT + 2)
#define SVT_PRS_TAG_ROI		VFT_INT8
#define SVS_PRS_TAG_ROI		"ROI data for v3.5 display"
/*	typedef struct {
	    int32_t     x;
            int32_t     y;
         } vDBRoiPoint_t;

         typedef struct {
	    dClass_t    class;
            vDBRoiPoint_t       center;
            flt64_t             zoomH;
            flt64_t             zoomV;
            flt64_t             rotate;
            int32_t             numPoints;
            vDBRoiPoint_t       *points;
          } vDBRoiDesc_t;

	  上記の行動でＲＯＩが保存される。
	*/
   

/*****************************************************************************
  IMCALC name space
*/




#endif /* tami_vfStudy_vfs_present_H__ */
