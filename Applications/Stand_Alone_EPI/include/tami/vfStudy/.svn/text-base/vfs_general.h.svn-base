/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield names for the GENERAL subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The GENERAL subspace contains fields not associated with any other
 *	subspace, but rather overlapping several subspaces.
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



#ifndef tami_vfStudy_vfs_general_H__
#define tami_vfStudy_vfs_general_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_GEN_DATA		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 0)
#define	SVS_GEN_DATA		"Image data"
/*	Container for image data.  The data may be acquired, processed,
	or reprocessed.  What state the data is in should be indicated
	by its location within the study file, e.g. a SVN_GEN_DATA field
	located in the SVN_ST_IMAGE subtree (part of the acquired data
	branch of the study file).
	*/

#define SVN_GEN_DATA_TYPE	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 1)
#define SVT_GEN_DATA_TYPE	VFT_INT32
#define SVS_GEN_DATA_TYPE	"Image data type"
/*	The type of data contained in a SVN_GEN_DATA varfield.  The contents
	of this field should be a varfield data type (such as those defined
	in vf_basictypes.h or vf_apptypes.h).
	*/

#define SVN_GEN_DATA_FORM	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 2)
#define SVT_GEN_DATA_FORM	VFT_INT32
#define SVS_GEN_DATA_FORM	"Image data form"
/*	The form of the data contained in a SVN_GEN_DATA varfield.  The
	contents of this field should be one of the values defined in
	vf_appcodes.h for this field.
	*/

#define SVN_GEN_DATA_DIM	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 3)
#define SVT_GEN_DATA_DIM	VFT_INT32
#define SVS_GEN_DATA_DIM	"Image data dimensions"
/*	An array of values describing the size of each dimension of
	data contained in a SVN_GEN_DATA varfield.  For example, if
	a SVN_GEN_DATA varfield contained two-dimensional data, 256x128,
	then a SVN_GEN_DATA_DIM varfield should exist that has two elements,
	the first containing the value 256 and the second containing 128.

	The correct method for finding the number of dimensions in a
	SVN_GEN_DATA varfield is to find the number of elements in the
	corresponding SVN_GEN_DATA_DIM varfield.  This is the number of bytes
	stored in the field divided by the size of each element (i.e. the
	size of an int32).

	Note that the number of dimensions in any given SVN_GEN_DATA is
	independent of the "dimensionality" of the imaging technique as
	indicated in the SVN_PLN_IMAGING_MODE varfield.  For example, it is
	not necessarily true that data from a 3D data acquisition will be
	stored in 3D SVN_GEN_DATA varfields (though it _may_ be true).
	*/

#define SVN_GEN_DATA_DESC	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 4)
#define SVT_GEN_DATA_DESC	VFT_INT32
#define	SVS_GEN_DATA_DESC	"Image data dimension descriptions"
/*	An array of values describing the meaning of each dimension of
	data contained in a SVN_GEN_DATA varfield.  The contents of this
	field should be one of the values defined in vf_appcodes.h for
	this field.
	*/

#define SVN_GEN_DATA_SCALE	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 5)
#define SVT_GEN_DATA_SCALE	VFT_FLT32
#define SVS_GEN_DATA_SCALE	"Image data scale factor"
/*	The scale factor that was used to normalize data in an SVN_GEN_DATA
	varfield.  To restore the data to its original range, divide the
	data by this scale factor.
	*/

#define SVN_GEN_DATA_MAX_MAG	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 6)
#define SVT_GEN_DATA_MAX_MAG	VFT_FLT32
#define SVS_GEN_DATA_MAX_MAG	"Image data maximum magnitude"
/*	For real data: the maximum absolute value of data in a
	SVN_GEN_DATA varfield. For complex data: the maximum magnitude
	of data in a SVN_GEN_DATA varfield.
	*/

#define SVN_GEN_DATA_MAX	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 7)
#define SVT_GEN_DATA_MAX	VFT_FLT32
#define SVS_GEN_DATA_MAX	"Image data maximum value"
/*	Pretty self explanatory. For complex data it is also simply the
	maximum value to be found in the SVN_GEN_DATA field. It may be the
	real component or the imaginary component.
	*/

#define SVN_GEN_DATA_MIN	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 8)
#define SVT_GEN_DATA_MIN	VFT_FLT32
#define SVS_GEN_DATA_MIN	"Image data minimum value"
/*	See previous field.
	*/

#define SVN_GEN_STUDY_VERSION	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 9)
#define SVT_GEN_STUDY_VERSION	VFT_INT32
#define SVS_GEN_STUDY_VERSION	"Study version"
/*	Study file version, pdet will set this field.

	スタディファイルバージョン。PDETによって格納される。
	*/

#define SVN_GEN_VF_GROUP	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 10)
#define SVT_GEN_VF_GROUP	VFT_INT32
#define SVS_GEN_VF_GROUP	"Group file id"
/*	Group file ID.	pqm and mui set this field.

	バーフィールドライブラリ V2.0 により必要となったフィールド。グループ
	IDを格納する。PQMとMUIが生データ・画像データを作成する前にグループ番
	号を確保しておく。
	*/

#define SVN_GEN_NUM_IMAGES_UP	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 11)
#define SVT_GEN_NUM_IMAGES_UP	VFT_INT32
#define SVS_GEN_NUM_IMAGES_UP	"The number of images up"
/*	作成画像枚数を保存する。画像を作成したプロセスがこのフィールド
	をカウントアップする。SVN_GEN_NUM_IMAGES_DOWN を引くと保有画像
	枚数を求めることができる。
	*/

#define SVN_GEN_NUM_IMAGES_DOWN	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 12)
#define SVT_GEN_NUM_IMAGES_DOWN	VFT_INT32
#define SVS_GEN_NUM_IMAGES_DOWN	"The number of images up"
/*	削除画像枚数を保存する。画像を削除したプロセスがこのフィールドを
	カウントアップする。SVN_GEN_NUM_IMAGES_UP からこの値を引くことに
	より保有画像枚数を求めることができる。
	*/

#define SVN_GEN_OFFSET_TABLE	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 13)
#define SVT_GEN_OFFSET_TABLE	VFT_SORT_TABLE_ELEMENT
#define SVS_GEN_OFFSET_TABLE	"Size/position description for acquired data"
/*      The table is an array with an entry for each acquired line of data.
	Each entry describes the offset into the data field at which the
	line can be found, the length of the acquired line, the length of
	the buffer into which the acquired line should be placed, and how to
	copy the data into the buffer.
	See type description in vf_apptypes.h.
	*/

#define SVN_GEN_TEMPLATE_DATA	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 14)
#define SVS_GEN_TEMPLATE_DATA	"Template data"
/*      Container for template data. Template data may be stored separately
	from image data at the same node, so a separate field name is
	required.
	See description of SVN_GEN_DATA.
	*/

#define SVN_GEN_TEMPLATE_OFFSET_TABLE \
                                    (SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 15)
#define SVS_GEN_TEMPLATE_OFFSET_TABLE \
     "Size/position description for acquired template data"
/*      See type description of SVN_GEN_OFFSET_TABLE.
	*/

#define SVN_GEN_TEMPL_DIM	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 16)
#define SVS_GEN_TEMPL_DIM	"Template data dimensions"
/*	An array of values describing the size of each dimension of
	data contained in a SVN_GEN_TEMPLATE_DATA varfield.
	See the description of SVN_GEN_DATA_DIM above.
	*/
#define SVN_GEN_LINE_TEMPLATE_DATA	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 17)
#define SVS_GEN_LINE_TEMPLATE_DATA	"Line Template data"
/*      Container for line template data. Line Template data may be stored
	separately from image data at the same node, so a separate field name
	isrequired.
	See description of SVN_GEN_DATA.
	*/

#define SVN_GEN_LINE_TEMPLATE_OFFSET_TABLE \
                                    (SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 18)
#define SVS_GEN_LINE_TEMPLATE_OFFSET_TABLE \
     "Size/position description for acquired line template data"
/*      See type description of SVN_GEN_OFFSET_TABLE.
	*/

#define SVN_GEN_LINE_TEMPL_DIM	(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 19)
#define SVS_GEN_LINE_TEMPL_DIM	"line template data dimensions"
/*	An array of values describing the size of each dimension of
	data contained in a SVN_GEN_LINE_TEMPLATE_DATA varfield.
	See the description of SVN_GEN_DATA_DIM above.
	*/

#define SVN_GEN_DATA1		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 20)
#define	SVS_GEN_DATA1		"Image data"

#define SVN_GEN_DATA2		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 21)
#define	SVS_GEN_DATA2		"Image data"

#define SVN_GEN_DATA3		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 22)
#define	SVS_GEN_DATA3		"Image data"

#define SVN_GEN_DATA4		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 23)
#define	SVS_GEN_DATA4		"Image data"

#define SVN_GEN_DATA5		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 24)
#define	SVS_GEN_DATA5		"Image data"

#define SVN_GEN_DATA6		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 25)
#define	SVS_GEN_DATA6		"Image data"

#define SVN_GEN_DATA7		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 26)
#define	SVS_GEN_DATA7		"Image data"

#define SVN_GEN_DATA8		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 27)
#define	SVS_GEN_DATA8		"Image data"

#define SVN_GEN_DATA9		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 28)
#define	SVS_GEN_DATA9		"Image data"

#define SVN_GEN_DATA10		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 29)
#define	SVS_GEN_DATA10		"Image data"

#define SVN_GEN_DATA11		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 30)
#define	SVS_GEN_DATA11		"Image data"

#define SVN_GEN_DATA12		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 31)
#define	SVS_GEN_DATA12		"Image data"

#define SVN_GEN_DATA13		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 32)
#define	SVS_GEN_DATA13		"Image data"

#define SVN_GEN_DATA14		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 33)
#define	SVS_GEN_DATA14		"Image data"

#define SVN_GEN_DATA15		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 34)
#define	SVS_GEN_DATA15		"Image data"

#define SVN_GEN_DATA16		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 35)
#define	SVS_GEN_DATA16		"Image data"

#define SVN_GEN_DATA17		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 36)
#define	SVS_GEN_DATA17		"Image data"

#define SVN_GEN_DATA18		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 37)
#define	SVS_GEN_DATA18		"Image data"

#define SVN_GEN_DATA19		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 38)
#define	SVS_GEN_DATA19		"Image data"

#define SVN_GEN_DATA20		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 39)
#define	SVS_GEN_DATA20		"Image data"

#define SVN_GEN_DATA21		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 40)
#define	SVS_GEN_DATA21		"Image data"

#define SVN_GEN_DATA22		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 41)
#define	SVS_GEN_DATA22		"Image data"

#define SVN_GEN_DATA23		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 42)
#define	SVS_GEN_DATA23		"Image data"

#define SVN_GEN_DATA24		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 43)
#define	SVS_GEN_DATA24		"Image data"

#define SVN_GEN_DATA25		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 44)
#define	SVS_GEN_DATA25		"Image data"

#define SVN_GEN_DATA26		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 45)
#define	SVS_GEN_DATA26		"Image data"

#define SVN_GEN_DATA27		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 46)
#define	SVS_GEN_DATA27		"Image data"

#define SVN_GEN_DATA28		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 47)
#define	SVS_GEN_DATA28		"Image data"

#define SVN_GEN_DATA29		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 48)
#define	SVS_GEN_DATA29		"Image data"

#define SVN_GEN_DATA30		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 49)
#define	SVS_GEN_DATA30		"Image data"

#define SVN_GEN_DATA31		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 50)
#define	SVS_GEN_DATA31		"Image data"

#define SVN_GEN_DATA32		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 51)
#define	SVS_GEN_DATA32		"Image data"

#define SVN_GEN_DATA33		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 52)
#define	SVS_GEN_DATA33		"Image data"

#define SVN_GEN_DATA34		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 53)
#define	SVS_GEN_DATA34		"Image data"

#define SVN_GEN_DATA35		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 54)
#define	SVS_GEN_DATA35		"Image data"

#define SVN_GEN_DATA36		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 55)
#define	SVS_GEN_DATA36		"Image data"

#define SVN_GEN_DATA37		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 56)
#define	SVS_GEN_DATA37		"Image data"

#define SVN_GEN_DATA38		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 57)
#define	SVS_GEN_DATA38		"Image data"

#define SVN_GEN_DATA39		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 58)
#define	SVS_GEN_DATA39		"Image data"

#define SVN_GEN_DATA40		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 59)
#define	SVS_GEN_DATA40		"Image data"

#define SVN_GEN_DATA41		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 60)
#define	SVS_GEN_DATA41		"Image data"

#define SVN_GEN_DATA42		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 61)
#define	SVS_GEN_DATA42		"Image data"

#define SVN_GEN_DATA43		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 62)
#define	SVS_GEN_DATA43		"Image data"

#define SVN_GEN_DATA44		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 63)
#define	SVS_GEN_DATA44		"Image data"

#define SVN_GEN_DATA45		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 64)
#define	SVS_GEN_DATA45		"Image data"

#define SVN_GEN_DATA46		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 65)
#define	SVS_GEN_DATA46		"Image data"

#define SVN_GEN_DATA47		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 66)
#define	SVS_GEN_DATA47		"Image data"

#define SVN_GEN_DATA48		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 67)
#define	SVS_GEN_DATA48		"Image data"

#define SVN_GEN_DATA49		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 68)
#define	SVS_GEN_DATA49		"Image data"

#define SVN_GEN_DATA50		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 69)
#define	SVS_GEN_DATA50		"Image data"

#define SVN_GEN_DATA51		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 70)
#define	SVS_GEN_DATA51		"Image data"

#define SVN_GEN_DATA52		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 71)
#define	SVS_GEN_DATA52		"Image data"

#define SVN_GEN_DATA53		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 72)
#define	SVS_GEN_DATA53		"Image data"

#define SVN_GEN_DATA54		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 73)
#define	SVS_GEN_DATA54		"Image data"

#define SVN_GEN_DATA55		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 74)
#define	SVS_GEN_DATA55		"Image data"

#define SVN_GEN_DATA56		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 75)
#define	SVS_GEN_DATA56		"Image data"

#define SVN_GEN_DATA57		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 76)
#define	SVS_GEN_DATA57		"Image data"

#define SVN_GEN_DATA58		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 77)
#define	SVS_GEN_DATA58		"Image data"

#define SVN_GEN_DATA59		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 78)
#define	SVS_GEN_DATA59		"Image data"

#define SVN_GEN_DATA60		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 79)
#define	SVS_GEN_DATA60		"Image data"

#define SVN_GEN_DATA61		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 80)
#define	SVS_GEN_DATA61		"Image data"

#define SVN_GEN_DATA62		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 81)
#define	SVS_GEN_DATA62		"Image data"

#define SVN_GEN_DATA63		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 82)
#define	SVS_GEN_DATA63		"Image data"

#define SVN_GEN_DATA64		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 83)
#define	SVS_GEN_DATA64		"Image data"

#define SVN_GEN_DATA65		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 84)
#define	SVS_GEN_DATA65		"Image data"

#define SVN_GEN_DATA66		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 85)
#define	SVS_GEN_DATA66		"Image data"

#define SVN_GEN_DATA67		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 86)
#define	SVS_GEN_DATA67		"Image data"

#define SVN_GEN_DATA68		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 87)
#define	SVS_GEN_DATA68		"Image data"

#define SVN_GEN_DATA69		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 88)
#define	SVS_GEN_DATA69		"Image data"

#define SVN_GEN_DATA70		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 89)
#define	SVS_GEN_DATA70		"Image data"

#define SVN_GEN_DATA71		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 90)
#define	SVS_GEN_DATA71		"Image data"

#define SVN_GEN_DATA72		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 91)
#define	SVS_GEN_DATA72		"Image data"

#define SVN_GEN_DATA73		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 92)
#define	SVS_GEN_DATA73		"Image data"

#define SVN_GEN_DATA74		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 93)
#define	SVS_GEN_DATA74		"Image data"

#define SVN_GEN_DATA75		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 94)
#define	SVS_GEN_DATA75		"Image data"

#define SVN_GEN_DATA76		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 95)
#define	SVS_GEN_DATA76		"Image data"

#define SVN_GEN_DATA77		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 96)
#define	SVS_GEN_DATA77		"Image data"

#define SVN_GEN_DATA78		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 97)
#define	SVS_GEN_DATA78		"Image data"

#define SVN_GEN_DATA79		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 98)
#define	SVS_GEN_DATA79		"Image data"

#define SVN_GEN_DATA80		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 99)
#define	SVS_GEN_DATA80		"Image data"

#define SVN_GEN_DATA81		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 100)
#define	SVS_GEN_DATA81		"Image data"

#define SVN_GEN_DATA82		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 101)
#define	SVS_GEN_DATA82		"Image data"

#define SVN_GEN_DATA83		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 102)
#define	SVS_GEN_DATA83		"Image data"

#define SVN_GEN_DATA84		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 103)
#define	SVS_GEN_DATA84		"Image data"

#define SVN_GEN_DATA85		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 104)
#define	SVS_GEN_DATA85		"Image data"

#define SVN_GEN_DATA86		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 105)
#define	SVS_GEN_DATA86		"Image data"

#define SVN_GEN_DATA87		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 106)
#define	SVS_GEN_DATA87		"Image data"

#define SVN_GEN_DATA88		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 107)
#define	SVS_GEN_DATA88		"Image data"

#define SVN_GEN_DATA89		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 108)
#define	SVS_GEN_DATA89		"Image data"

#define SVN_GEN_DATA90		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 109)
#define	SVS_GEN_DATA90		"Image data"

#define SVN_GEN_DATA91		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 110)
#define	SVS_GEN_DATA91		"Image data"

#define SVN_GEN_DATA92		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 111)
#define	SVS_GEN_DATA92		"Image data"

#define SVN_GEN_DATA93		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 112)
#define	SVS_GEN_DATA93		"Image data"

#define SVN_GEN_DATA94		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 113)
#define	SVS_GEN_DATA94		"Image data"

#define SVN_GEN_DATA95		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 114)
#define	SVS_GEN_DATA95		"Image data"

#define SVN_GEN_DATA96		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 115)
#define	SVS_GEN_DATA96		"Image data"

#define SVN_GEN_DATA97		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 116)
#define	SVS_GEN_DATA97		"Image data"

#define SVN_GEN_DATA98		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 117)
#define	SVS_GEN_DATA98		"Image data"

#define SVN_GEN_DATA99		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 118)
#define	SVS_GEN_DATA99		"Image data"

#define SVN_GEN_DATA100		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 119)
#define	SVS_GEN_DATA100		"Image data"

#define SVN_GEN_DATA101		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 120)
#define	SVS_GEN_DATA101		"Image data"

#define SVN_GEN_DATA102		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 121)
#define	SVS_GEN_DATA102		"Image data"

#define SVN_GEN_DATA103		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 122)
#define	SVS_GEN_DATA103		"Image data"

#define SVN_GEN_DATA104		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 123)
#define	SVS_GEN_DATA104		"Image data"

#define SVN_GEN_DATA105		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 124)
#define	SVS_GEN_DATA105		"Image data"

#define SVN_GEN_DATA106		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 125)
#define	SVS_GEN_DATA106		"Image data"

#define SVN_GEN_DATA107		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 126)
#define	SVS_GEN_DATA107		"Image data"

#define SVN_GEN_DATA108		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 127)
#define	SVS_GEN_DATA108		"Image data"

#define SVN_GEN_DATA109		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 128)
#define	SVS_GEN_DATA109		"Image data"

#define SVN_GEN_DATA110		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 129)
#define	SVS_GEN_DATA110		"Image data"

#define SVN_GEN_DATA111		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 130)
#define	SVS_GEN_DATA111		"Image data"

#define SVN_GEN_DATA112		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 131)
#define	SVS_GEN_DATA112		"Image data"

#define SVN_GEN_DATA113		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 132)
#define	SVS_GEN_DATA113		"Image data"

#define SVN_GEN_DATA114		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 133)
#define	SVS_GEN_DATA114		"Image data"

#define SVN_GEN_DATA115		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 134)
#define	SVS_GEN_DATA115		"Image data"

#define SVN_GEN_DATA116		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 135)
#define	SVS_GEN_DATA116		"Image data"

#define SVN_GEN_DATA117		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 136)
#define	SVS_GEN_DATA117		"Image data"

#define SVN_GEN_DATA118		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 137)
#define	SVS_GEN_DATA118		"Image data"

#define SVN_GEN_DATA119		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 138)
#define	SVS_GEN_DATA119		"Image data"

#define SVN_GEN_DATA120		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 139)
#define	SVS_GEN_DATA120		"Image data"

#define SVN_GEN_DATA121		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 140)
#define	SVS_GEN_DATA121		"Image data"

#define SVN_GEN_DATA122		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 141)
#define	SVS_GEN_DATA122		"Image data"

#define SVN_GEN_DATA123		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 142)
#define	SVS_GEN_DATA123		"Image data"

#define SVN_GEN_DATA124		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 143)
#define	SVS_GEN_DATA124		"Image data"

#define SVN_GEN_DATA125		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 144)
#define	SVS_GEN_DATA125		"Image data"

#define SVN_GEN_DATA126		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 145)
#define	SVS_GEN_DATA126		"Image data"

#define SVN_GEN_DATA127		(SVN_MIN_INDEPENDENT+SVN_OFF_GENERAL + 146)
#define	SVS_GEN_DATA127		"Image data"
/*	SVN_EXE_PAC_SCAN_FLAG が TRUE(1) で、かつ、SVN_EXE_NUM_PAC_SCAN
	が ２ の時 有効であり、RB I/F #1 から受信した RAW データを格納する。
	RB I/F #0 から受信した RAW データは SVN_DEN_DATA に格納する。
	将来の４チャネル対応も考え、SVN_GEN_DATA2 および SVN_GEN_DATA3 を
	予約する。
	さらに８チャネルスキャン対応で追加する。
	さらに１２８チャネルスキャン対応で追加する。
	*/

/*****************************************************************************
  STUDY name space
*/
#define SVN_BASE_STUDY_GENERAL	SVN_MIN_STUDY+SVN_OFF_GENERAL

#define SVN_GEN_PAS_COMMENT		(SVN_BASE_STUDY_GENERAL+0)
#define SVT_GEN_PAS_COMMENT		VFT_STRING
#define SVS_GEN_PAS_COMMENT		"Comment for PAS."
/*	PAS に記述されるコメント。memory 及び関連ツールが PAS を管理する
	ために使用する。
	このフィールドはＰＡＳにのみ記述されるもので通常使用するスタディ
	ファイルに記述されることは無い。
	*/

#define SVN_GEN_DICOM_INSTANCE_CREATOR_UID	(SVN_BASE_STUDY_GENERAL + 1)
#define SVT_GEN_DICOM_INSTANCE_CREATOR_UID	VFT_STRING
#define SVS_GEN_DICOM_INSTANCE_CREATOR_UID	"Instance Creator UID"
/*      UID (Unique IDentifier) of the system that created the study
 */

#define SVN_GEN_DICOM_STUDY_INSTANCE_UID	(SVN_BASE_STUDY_GENERAL + 2)
#define SVT_GEN_DICOM_STUDY_INSTANCE_UID	VFT_STRING
#define SVS_GEN_DICOM_STUDY_INSTANCE_UID	"Study Instance UID"
/*      UID (Unique IDentifier) of the the study
 */

#define SVN_GEN_SYSTEM_VERSION	(SVN_BASE_STUDY_GENERAL + 3)
#define SVT_GEN_SYSTEM_VERSION	VFT_STRING
#define SVS_GEN_SYSTEM_VERSION	"System version"
/*	System version, pdet will set this field.

	システムバージョン。PDETによって格納される。
	*/

/*****************************************************************************
  PROTOCOL name space
*/
#define SVN_BASE_PROTOCOL_GENERAL	SVN_MIN_PROTOCOL+SVN_OFF_GENERAL

#define SVN_GEN_SCAN_NUMBER	(SVN_BASE_PROTOCOL_GENERAL + 0)
#define SVT_GEN_SCAN_NUMBER	VFT_INT32
#define SVS_GEN_SCAN_NUMBER	"Scan number"
/*	撮影済 protocol に対して撮影順番を記録する。
 */

#define SVN_GEN_SYSTEM_TYPE     (SVN_BASE_PROTOCOL_GENERAL + 1)
#define SVT_GEN_SYSTEM_TYPE     VFT_STRING
#define SVS_GEN_SYSTEM_TYPE     "System type"
/*      収集データがどのシステムで撮られたものかを判断するために使用する。
 */

/*****************************************************************************
  SUBPROTOCOL name space
*/
#define SVN_BASE_SUBPROTOCOL_GENERAL	SVN_MIN_SUBPROTOCOL+SVN_OFF_GENERAL

#define SVN_GEN_PHASE_CORRECTION_DATA	(SVN_BASE_SUBPROTOCOL_GENERAL + 0)
#define	SVS_GEN_PHASE_CORRECTION_DATA	"Phase Correction data"



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
#define SVN_BASE_POSITION_GENERAL	SVN_MIN_POSITION+SVN_OFF_GENERAL

#define SVN_GEN_FILM_FLAG		(SVN_BASE_POSITION_GENERAL+0)
#define SVT_GEN_FILM_FLAG		VFT_BOOL
#define SVS_GEN_FILM_FLAG		"The filming flag for each image."
/*	イメージャにフィルム出力された画像と同じノードに、このフィールドが存在し、
	True がセットされます。フィールドが存在しない場合は、フィルム出力されて
	いません。
	*/

#define SVN_GEN_CSV_DATA		(SVN_BASE_POSITION_GENERAL+1)
#define SVT_GEN_CSV_DATA		VFT_STRING
#define SVS_GEN_CSV_DATA		"csv data"
/*	GPWWWが画像に付属するCSVデータを取得するために使用する。
	*/

/*****************************************************************************
  IMCALC name space
*/




#endif /* tami_vfStudy_vfs_general_H__ */
