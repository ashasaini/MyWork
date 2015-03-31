/*********************************************************************
 *
 *  (c) Copyright 1992-97 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *	This file contains the varfield names for the PLAN subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The PLAN subspace contains fields associated with planning patient
 *	protocols, planning what operator-controlled parameters should be
 *	used to produce images.
 *
 *    Assumptions:
 *	These values are not in the range of
 *		VFN_MIN_RESERVED...VFN_MAX_RESERVED
 *
 *    Objects with external linkage defined in this file:
 *
 *********************************************************************/
/* Revision History:
 * Aug.11.97	古舘直幸	Modified
 * End of History */


#ifndef tami_vfStudy_vfs_plan_H__
#define tami_vfStudy_vfs_plan_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */


/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_BASE_INDEPENDENT_PLAN	SVN_MIN_INDEPENDENT+SVN_OFF_PLAN

#define SVN_PLN_MEMSET_NAME	(SVN_BASE_INDEPENDENT_PLAN + 0)
#define SVT_PLN_MEMSET_NAME	VFT_STRING
#define SVS_PLN_MEMSET_NAME	"Name of memorized protocol set"
/*	This field contains the name of the memorized protocol set for
	use with memorized protocols. In the future we may want PQM
	propagating this field into the protocols as they are recalled
	from memory so we know where the protocol come from
	*/


#define SVN_PLN_SLAB_ID		(SVN_BASE_INDEPENDENT_PLAN + 1)
#define	SVT_PLN_SLAB_ID		VFT_INT32
#define	SVS_PLN_SLAB_ID		"Slab identifier"
/*	スラブＩＤ
	グラフィックロケータ、イメージマトリクス、ディスプレイでのスラブ番号
	表示のために使用する。グラフィックロケータでは VISUALGROUP 内に定義
	される値を使用し、prolim はその値を各 SLICEGROUP にコピーする。再構
	成はこの値をエコーノードにコピーする。ディスプレイ、イメージマトリッ
	クスはこの値を表示する。
 */

#define SVN_PLN_LDB_MAPPED_ID		(SVN_BASE_INDEPENDENT_PLAN + 2)
#define	SVT_PLN_LDB_MAPPED_ID		VFT_STRING
#define	SVS_PLN_LDB_MAPPED_ID		"LocalDB mapped identifier"
/*	各ノードの対応するLocalDBのスキーマIDを設定する。
 */

/*****************************************************************************
  STUDY name space
*/

#define SVN_BASE_STUDY_PLAN	SVN_MIN_STUDY+SVN_OFF_PLAN


#define	SVN_PLN_PROT_ACQ_ORDER		(SVN_BASE_STUDY_PLAN + 0)
#define	SVT_PLN_PROT_ACQ_ORDER		VFT_INT32
#define	SVS_PLN_PROT_ACQ_ORDER		"Protocol Acquisition Order"
/*	プロトコルの収集順を記述するフィールド。インデックス番号の配列。
	ＰＱＭによって記述される。
	This field contains an array of integers defining the order in
	which protocols were acquired. It should be used by any application
	that needs to show information in the order protocols were
	acquired. Written by PQM.
	*/

#define	SVN_PLN_LAST_SCAN_PROTOCOL	(SVN_BASE_STUDY_PLAN + 1)
#define	SVT_PLN_LAST_SCAN_PROTOCOL	VFT_INT32
#define	SVS_PLN_LAST_SCAN_PROTOCOL	"Protocol to use for last scan option"
/*
	*/

#define SVN_PLN_CENTER_OFFSET		(SVN_BASE_STUDY_PLAN + 2)
#define	SVT_PLN_CENTER_OFFSET		VFT_FLTVEC3
#define	SVS_PLN_CENTER_OFFSET		"Center offset for pilot scan"
/*	パイロットスキャンにのみ適用するオフセットベクタの記憶領域。
	PQM によって設定／利用される。本来 PQM のメモリ上に記憶され
	るものだが、PQM 再起動時に終了時の状態を保持するためにスタディ
	ファイル中に格納する。
	*/
  
/*****************************************************************************
  PROTOCOL name space
*/

#define SVN_BASE_PROTOCOL_PLAN	SVN_MIN_PROTOCOL+SVN_OFF_PLAN


/* ---- These correspond to parameters selected through proEdit.
 *	Unless commented otherwise, the field is written by Support
 *	Equations (some are written by proEdit).
 */

#define SVN_PLN_SEQ_IDENT		(SVN_BASE_PROTOCOL_PLAN + 0)
#define	SVT_PLN_SEQ_IDENT		VFT_STRING
#define	SVS_PLN_SEQ_IDENT		"Sequence name for display"
/*	ディスプレイで表示するシーケンス名を記述する。値は seqselDB で
	定義される。prolim が収集条件に併せて内容を変更する場合がある。
	
	This is the name by which the sequence is defined in the sequence
	database.  It is intended for use by ProEdit only.
	Written by proEdit.
	*/

#define SVN_PLN_TEMPLATE_SEQ		(SVN_BASE_PROTOCOL_PLAN + 1)
#define SVT_PLN_TEMPLATE_SEQ		VFT_STRING
#define SVS_PLN_TEMPLATE_SEQ		"Template sequence used"
/*	シーケンス名を記述する。seqgen,prolim を検索する際のキーとして
	使用される。proselSeqselDB 内で使用されている文字列と同じものが
	スタディファイルで記述されなければならない。

	This is the name of the sequence (seqgen) used in this protocol.
	The contents of this field originate in the sequence database.
	Written by proEdit.
	*/

/* GAP in numbering..... */

#define	SVN_PLN_IMAGING_MODE		(SVN_BASE_PROTOCOL_PLAN + 3)
#define	SVT_PLN_IMAGING_MODE		VFT_INT32
#define	SVS_PLN_IMAGING_MODE		"Imaging mode (2D/3D)"
/*	イメージングモード（２Ｄ／３Ｄ）を記述する。

	This indicates if the data is acquired in a 2D or 3D mode.  This
	information originates in the sequence database.  The contents of
	this field should be one of the values defined in vf_appcodes.h
	for this field.
	Written by proEdit.
	*/

#define	SVN_PLN_NUM_EXCITATIONS		(SVN_BASE_PROTOCOL_PLAN + 4)
#define	SVT_PLN_NUM_EXCITATIONS		VFT_INT32
#define	SVS_PLN_NUM_EXCITATIONS		"Number of excitation encodings"
/*	This indicates the number of excitations or RF encodings within
	each echo. The contents of this field originate in sequence database.
	Written by proEdit.
	*/

#define	SVN_PLN_MOTION_SUPPRESSION	(SVN_BASE_PROTOCOL_PLAN + 5)
#define	SVT_PLN_MOTION_SUPPRESSION	VFT_BITFIELD
#define	SVS_PLN_MOTION_SUPPRESSION	"Type of motion supression used"
/*	体動補正に関する記述を行なう。現在は VFC_MSUP_BIT_FLOW_COMP の値が
	設定されるか０以外にあり得ない。

	This indicates which motion supression techniques have been
	applied to this protocol.  The contents of this field are selected
	by the operator in ProEdit.  This is a bit field.  The bit masks
	for this field are defined in vf_appcodes.h
	Written by proEdit.
	*/

#define	SVN_PLN_PRESAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 6)
#define	SVT_PLN_PRESAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_PRESAT_FLIP_ANGLE	"Flip angle for presat regions"
/*	プリサチュレーションのフリップ角を記述する。

	This is the flip angle in degrees to be be applied to each
	presaturation region in the protocol.  The contents of this
	field are selected by the operator in ProEdit.
	*/

#define	SVN_PLN_SELECTED_CONTRAST_TE	(SVN_BASE_PROTOCOL_PLAN + 7)
#define	SVT_PLN_SELECTED_CONTRAST_TE	VFT_FLT32
#define	SVS_PLN_SELECTED_CONTRAST_TE	"Selected contrast TE"
/*	プロトコルノードに記述されるＴＥ。２エコーの場合はこのフィールドを
	配列として使用する。例えば SE2580 では v[0] = 0.025,v[1] = 0.08 を
	記述する。単位は sec である。同様の値がイメージノードにコピーされる。
	コピー先の名前は SVN_PLN_CONTRAST_TE である。ただし、イメージノード
	はエコー数分のノードが存在するためコピー先では配列とはならない。

	This field is the TE of the images in seconds.  It is an array of
	TEs describing the contrast of the images acquired in this protocol.
	These are the quantities selected by the operator in ProEdit.  Anyone
	interested in labeling images or describing protocols should use
	these values.  The actual TEs of the individual echoes that make up
	the image are described in another VarField in the SEQUENCE part of
	the IMAGE name space.  The contents of this field originate in the
	sequence database.  This information is propagated to another
	VarField in the PLAN part of the IMAGE name space.
	*/

#define	SVN_PLN_NUMBER_OF_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 8)
#define	SVT_PLN_NUMBER_OF_ECHOES	VFT_INT32
#define	SVS_PLN_NUMBER_OF_ECHOES	"Number of echoes of each slice"
/*	エコー数（１or２）

	This field describes the number of echoes produced for each slice
	in this protocol.  It is selected as a consequence of the operator's
	selection in ProEdit of the TE(s) for this protocol.  It is
	propagated to another VarField in the PLAN part of the SLICE name
	space.
	*/

#define	SVN_PLN_SELECTED_SLICE_THK	(SVN_BASE_PROTOCOL_PLAN + 9)
#define	SVT_PLN_SELECTED_SLICE_THK	VFT_FLT32
#define	SVS_PLN_SELECTED_SLICE_THK	"Selected thickness"
/*	プロトコルノードに記述されるスライス厚。スライスノードにも同じ
	値がコピーされる。スライスノードでは SVN_PLN_THICKNESS の名前で
	記述される。名前の由来はユーザによって proEdit 上で選択される値
	であるということである。

	This is the thickness in meters of each slice in the protocol.
	It is a quantity selected by the operator in ProEdit.  This
	information is propagated to another VarField in the PLAN part
	of the SLICEGROUP name space.
	*/

#define SVN_PLN_SELECTED_SLICE_GAP	(SVN_BASE_PROTOCOL_PLAN + 10)
#define SVT_PLN_SELECTED_SLICE_GAP	VFT_FLT32
#define SVS_PLN_SELECTED_SLICE_GAP	"Selected gap"
/*	プロトコルノードに記述されるスライスギャップ。スライスノードに同じ
	値がコピーされるなど SVN_PLN_SELECTED_SLICE_THK と同じである。スラ
	イスノードでは SVN_PLN_SLICE_GAP となる。

	This is the gap in meters between each neighboring slice in a
	slice group.  It is a quantity selected by the operator in ProEdit.
	This information is propagated to another VarField in the PLAN part
	of the SLICEGROUP name space.
	*/

#define SVN_PLN_SLICE_ORIENTATION	(SVN_BASE_PROTOCOL_PLAN + 11)
#define SVT_PLN_SLICE_ORIENTATION	VFT_INT32
#define SVS_PLN_SLICE_ORIENTATION	"Orientation of slices"
/*	Orientation of slices.  The contents of this field are selected by the
	operator in ProEdit and should be one of the values defined in
	vf_appcodes.h for this field.
	*/

#define SVN_PLN_PHASE_DIRECTION		(SVN_BASE_PROTOCOL_PLAN + 12)
#define SVT_PLN_PHASE_DIRECTION		VFT_INT32
#define SVS_PLN_PHASE_DIRECTION		"Phase direction of slices"
/*	Phase encoding direction of slices.  The contents of this field are
	selected by the operator in ProEdit and should be one of the values
	defined in vf_appcodes.h for this field.
	*/

#define	SVN_PLN_WRAPAROUND_REDUCT	(SVN_BASE_PROTOCOL_PLAN + 13)
#define	SVT_PLN_WRAPAROUND_REDUCT	VFT_BITFIELD
#define	SVS_PLN_WRAPAROUND_REDUCT	"Type of wraparound reduction used"
/*	折り返し防止オーバーサンプリング（ＰＥ・ＲＯ）

	This describes the types of wraparound reduction selected by the
	operator in ProEdit.  This is a bit field.  The bit masks for this
	field are defined in vf_appcodes.h
	*/

#define	SVN_PLN_FOV			(SVN_BASE_PROTOCOL_PLAN + 14)
#define	SVT_PLN_FOV			VFT_FLTVEC2
#define	SVS_PLN_FOV			"Field of view of final image"
/*	文字通りＦＯＶ。ユーザがプロエディットで指定する。

	This describes the in plane FOV in meters of the image prior to any
	zooming, scaling, etc. that the operator may apply in Display.  These
	are the quantities selected by the operator in ProEdit and represent
	how they expect the image to appear.  Other applications should use
	this field to describe the FOV in labeling, etc.
	*/

#define	SVN_PLN_DISPLAYED_MATRIX	(SVN_BASE_PROTOCOL_PLAN + 15)
#define	SVT_PLN_DISPLAYED_MATRIX	VFT_FLTVEC2
#define	SVS_PLN_DISPLAYED_MATRIX	"Matrix size of final image"
/*	表示マトリックス。収集マトリックスとは異なる。ユーザが proEdit 
	で指定する。

	This describes the number of in plane pixels that make up the
	image before any scaling, zooming, etc. that is needed to make
	the image conform to a particular Display layout is performed.
	Note that the quantities in this field may therefore not match
	the number of pixels in the image displayed on the monitor at any
	given time.  However, these quantities are indicators of image
	quality and the time needed to acquire an image.  Therefore,
	applications interested in showing matrix size should probably use
	these quantities no matter what the size of the image on the
	monitor may be.  These are the quantities selected by the operator
	in ProEdit and represent how they think about the image.
	*/

#define	SVN_PLN_FLIP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 16)
#define	SVT_PLN_FLIP_ANGLE		VFT_INT32
#define	SVS_PLN_FLIP_ANGLE		"Flip angle for field echo sequences"
/*	フリップ角。

	This field is the flip angle in degrees.  It is used for both field
	echo and spin echo sequences.  In case of spin echo, it is the flip
	angle of the excitation pulse. This quantity is selected by the
	operator in ProEdit.
	*/

#define SVN_PLN_GATING_METHOD		(SVN_BASE_PROTOCOL_PLAN + 17)
#define SVT_PLN_GATING_METHOD		VFT_BITFIELD
#define SVS_PLN_GATING_METHOD		"Gating Method(s) used"
/*	ゲートソースを記述する。
		VFC_GAT_METHOD_BIT_ECG		1	ECG 
		VFC_GAT_METHOD_BIT_PERIPHERAL	(1<<1)	Peripheral
		VFC_GAT_METHOD_BIT_RESPIRATORY	(1<<2)	Respiratory
	の３つが定義されている。

	This describes the types of gating used in the protocol.  These
	include ECG, Peripheral, Respiratory and some combinations of these.
	This quantity is selected by the operator in ProEdit.  This is a bit
	field.  Bit masks for this field are defined in vf_appcodes.h.  AT THIS
	TIME GATING IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_GATE_PARAM		(SVN_BASE_PROTOCOL_PLAN + 18)
#define	SVT_PLN_GATE_PARAM		VFT_GATING_PARAM
#define	SVS_PLN_GATE_PARAM		"Gating parameters"
/*	ゲートスキャンに関するパラメータを記述するフィールド。 vf_apptypes.h 参照。

	This will likely describe various parameters related to gating.  The
	details of this field have not been determined.  AT THIS TIME GATING IS
	NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_GATE_MGR		(SVN_BASE_PROTOCOL_PLAN + 19)
#define	SVT_PLN_GATE_MGR		VFT_GATING_MANAGER
#define	SVS_PLN_GATE_MGR		"Gate Manager control"
/*	ゲートマネージャーに関するパラメータを記述するフィールド。
	vf_apptypes.h 参照。

	This will likely describe various parameters related to the gate
	manager.  The details of this field have not been determined.  AT THIS
	TIME GATING IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_FAST_TR_FLAG		(SVN_BASE_PROTOCOL_PLAN + 20)
#define	SVT_PLN_FAST_TR_FLAG		VFT_BOOL
#define	SVS_PLN_FAST_TR_FLAG		"Fast TR"
/*	バリアブルＴＲを指定するフィールド。真の場合バリアブルＴＲと解釈
	される。

	This flag indicates whether mixed TR is used to acquire images in
	a reduced amount of time.  When set, the CONTRAST_TR VarField
	indicates only the apparent TR of the image.  The TR of each
	echo collected by the host is in another VarField in the PROTOCOL
	name space.  This quantity is selected by the operator in ProEdit.
	*/

#define SVN_PLN_CONTRAST_TR		(SVN_BASE_PROTOCOL_PLAN + 21)
#define SVT_PLN_CONTRAST_TR		VFT_FLT32
#define SVS_PLN_CONTRAST_TR		"Contrast TR"
/*	ＴＲを指定するフィールド。

	This field describes the TR of the images in seconds.  This will
	be an actual TR if the FAST_TR_FLAG is not set or an apparent TR
	if the flag is set.  This quantity is selected by the operator in
	ProEdit.  The actual TRs of the individual echoes that make up
	the image are described in another VarField in the SEQUENCE part of
	the IMAGE name space. 
	*/

#define	SVN_PLN_TI			(SVN_BASE_PROTOCOL_PLAN + 22)
#define	SVT_PLN_TI			VFT_FLT32
#define	SVS_PLN_TI			"Inversion Time, TI"
/*	インバージョンパルスを付加する際のＴＩ時間を記述する。単位は秒。

	This field describes the Inversion Time, TI, of the images in
	seconds.  This quantity is selected by the operator in ProEdit.
	*/

#define	SVN_PLN_COVERAGES		(SVN_BASE_PROTOCOL_PLAN + 23)
#define	SVT_PLN_COVERAGES		VFT_INT32
#define	SVS_PLN_COVERAGES		"Number of coverages"
/*	カバレージ数。カバレージ：１回のスキャンでとれないスライス枚数
	が指定された場合シーケンス(seqgen)の複数回廻しで対応する。その
	際の１回の seqgen をカバレージと呼ぶ。

	This field describes the number of different anatomical coverages
	to be performed in this protocol.  Each coverage will contain the
	same number of slices but in a different spatial location.  This
	quantity is selected by the operator in ProEdit.
	*/

#define	SVN_PLN_REPETITIONS		(SVN_BASE_PROTOCOL_PLAN + 24)
#define	SVT_PLN_REPETITIONS		VFT_INT32
#define	SVS_PLN_REPETITIONS		"Number of repetitions"
/*	同一条件の seqgen を複数回廻す。その際の繰り返し回数を指定する。
	設計当初、ダイナミックはこの繰り返しによって対応するはずだったが
	seqgen のロード時間・ＲＭとの通信等の時間が無視できないため V2.0
	以降 seqgen の複数回廻しでのダイナミック対応は行なっていない。
	V2.0 以降このフィールドは端に繰り返し回数を記述するのみで物理的な
	意味を持たない。

	This field describes the number times that the slices selected in
	the protocol are repeated in identical acquisitions.  The slices
	remain fixed from one acquisition to the next, the only difference
	is temporal.  This quantity is selected by the operator in ProEdit.
	*/

#define SVN_PLN_GATING_PTS		(SVN_BASE_PROTOCOL_PLAN + 25)
#define SVT_PLN_GATING_PTS		VFT_INT32
#define SVS_PLN_GATING_PTS		"Gating points"
/*	This field describes the number of temporal points in a gated
	sequence.  Each slice will occupy this number of temporal points.
	This quantity is selected by the operator in ProEdit.  AT THIS TIME
	GATING IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define SVN_PLN_GATING_ROTAT		(SVN_BASE_PROTOCOL_PLAN + 26)
#define SVT_PLN_GATING_ROTAT		VFT_INT32
#define SVS_PLN_GATING_ROTAT		"Gating rotations"
/*	This field describes the order of acquisition of the different
	rotations of a gated protocol.  Options are expected to be Forward or
	Reverse.  The contents of this field are selected by the operator in
	ProEdit and should be one of the values defined in vf_appcodes.h for
	this field.  AT THIS TIME GATING IS NOT SUPPORTED AND THIS FIELD SHOULD
	NOT BE USED.
	*/

#define	SVN_PLN_RAW_AVERAG_MODE		(SVN_BASE_PROTOCOL_PLAN + 27)
#define	SVT_PLN_RAW_AVERAG_MODE		VFT_INT32
#define	SVS_PLN_RAW_AVERAG_MODE		"Raw averaging types"
/*	This describes the manner in which the raw NMR data is averaged.
	Examples are AFI and Selective Averaging.  This field does not describe
	the way that processed data (images) may be averaged in Display.  The
	contents of this field are selected by the operator in ProEdit and
	should be one of the values defined in vf_appcodes.h for this field.
	*/

#define	SVN_PLN_NEX			(SVN_BASE_PROTOCOL_PLAN + 28)	
#define	SVT_PLN_NEX			VFT_FLT32
#define	SVS_PLN_NEX			"Number of raw averages"
/*	アベレージング回数。VariNAQ 対応のためVFT_FLT32 となっている。

	This field describes the fundamental number of averages that the
	NMR data as an entity received.  The contents of this field are
	selected by the operator in ProEdit.  This field does not describe
	how many averages took place to produce each echo collected by the
	host.  That information in in another VarField in the PROTOCOL
	name space.
	*/

#define	SVN_PLN_IS_ACQ_FORWARD		(SVN_BASE_PROTOCOL_PLAN + 29)
#define	SVT_PLN_IS_ACQ_FORWARD		VFT_INT32
#define	SVS_PLN_IS_ACQ_FORWARD		"Flag to indicate if acquired forward"
/*	収集順序を記述するフィールド。とり得る値は
		VFC_ACQ_ORDER_FORWARD	0
		VFC_ACQ_ORDER_BACKWARD	1
		VFC_ACQ_ORDER_CENTRIC	2
	主にゲートスキャンで使用するセントリックを収集順序と解釈し、従来
	VFT_BOOL であったこのフィールドを VFT_INT32 と再定義し上記３つの
	パラメータをとり得るように変更した。

	This field describes whether slices are acquired in a forward or
	reverse order.  When the flag is set it indicates that the slices are
	acquired in forward order.  The contents of this field are selected by
	the operator in ProEdit.
	*/

#define	SVN_PLN_INTERL_FLAG		(SVN_BASE_PROTOCOL_PLAN + 30)
#define	SVT_PLN_INTERL_FLAG		VFT_INT32
#define	SVS_PLN_INTERL_FLAG		"Slice interleaving flag"
/*	インターリーブ方法を記述するフィールド。とり得る値は
		VFC_INTRL_NONE		0
		VFC_INTRL_ALTERNATE	1
	である。設計当初（TAMI）インターリーブは単なるフラグだったが
	将来的に他のインターリーブ法を定義することを想定して型をVFT_BOOL
	から VFT_INT32 に変更した。
	
  	This field describes whether slices are acquired interleaved (TRUE)
	or not (FALSE).  The contents of this field are selected by the
	operator in ProEdit.
	*/

#define	SVN_PLN_TOTAL_SLICES		(SVN_BASE_PROTOCOL_PLAN + 31)
#define	SVT_PLN_TOTAL_SLICES		VFT_INT32
#define	SVS_PLN_TOTAL_SLICES		"Total slices"
/*	スライス枚数の総和。

	This field describes the total number of spatially distinct slices
	to be acquired in this entire protocol.  Temporal differences in
	the same physical slice are not considered to be different slices.
	This selection is made by the operator in ProEdit.
	*/

/* GAP in numbering..... */

#define	SVN_PLN_TOTAL_TIME		(SVN_BASE_PROTOCOL_PLAN + 33)
#define	SVT_PLN_TOTAL_TIME		VFT_INT32
#define	SVS_PLN_TOTAL_TIME		"Total for protocol (predicted)"
/*	総収集時間。

	This field indicates the total expected amount of time
	required for execution of the entire protocol in seconds.  It is the
	expected or predicted amount of time because the acquisition time for
	protocols such as gated protocols can not be exactly determined.  The
	quantity in this field is determined by the support equations and
	communicated to ProEdit for display.
	*/

#define	SVN_PLN_AUTOVOICE		(SVN_BASE_PROTOCOL_PLAN + 34)
#define	SVT_PLN_AUTOVOICE		VFT_BITFIELD
#define	SVS_PLN_AUTOVOICE		"Autovoice options"
/*	未使用。

	This inane field describes which of the autovoice options are to be
	broadcast to the patient during the scan.  At this time the operation
	of Autovoice is undefined.  It is expected that this will be a bit
	field.  Bit masks for this field are defined in vf_appcodes.h.  AT THIS
	TIME AUTOVOICE IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/


#define	SVN_PLN_APP_CODE		(SVN_BASE_PROTOCOL_PLAN + 35)
#define	SVT_PLN_APP_CODE		VFT_INT32
#define	SVS_PLN_APP_CODE		"Application selection"
/*	アプリケーションコード。vf_appcode.h 参照。

	This field describes which application the operator has selected in
	ProEdit.  The contents of this field should be one of the values
	defined in vf_appcodes.h for this field.
	Written by proEdit.
	*/

#define SVN_PLN_SPATIAL_PRESAT		(SVN_BASE_PROTOCOL_PLAN + 36)
#define SVT_PLN_SPATIAL_PRESAT		VFT_BITFIELD
#define SVS_PLN_SPATIAL_PRESAT		"Spatial Presaturation Selection"
/*	面内サチュレーションをビット論理和で指定する。 vf_appcode.h 参照。

	This field describes the type of spatial presaturation convenience
	selection has been made.  The selection is made by the operator in
	ProEdit.  This is a bit field.  Bit masks for this field are defined in
	vf_appcodes.h.
	Written by proEdit.
	*/

#define SVN_PLN_FREEHAND_PRESAT		(SVN_BASE_PROTOCOL_PLAN + 37)
#define SVT_PLN_FREEHAND_PRESAT		VFT_BOOL
#define SVS_PLN_FREEHAND_PRESAT		"Freehand Presaturation flag"
/*	フリーハンドプリサチュレーションを指定可能かどうかのフラグ。
	他のプリサチュレーションが指定されることによりシステムで許可している
	サチュレーションの個数をオーバーした時にこのフィールドが偽となる。

	This field is a flag to indicate if it is possible for the operator to
	define additional freehand presaturation regions in the Graphic
	Locator.
	Written by proEdit.
	*/

#define SVN_PLN_TURBO_PREP_TIME		(SVN_BASE_PROTOCOL_PLAN + 38)
#define SVT_PLN_TURBO_PREP_TIME		VFT_FLT32
#define SVS_PLN_TURBO_PREP_TIME		"Time of Turbo Preparation Pulse"
/*	未使用。

	This field describes the time in seconds of the preparation pulse used
	in a turbo sequence.  This quantity is selected by the operator in
	ProEdit.
	*/

#define SVN_PLN_TYPE_DELAY_BETWEEN_REPS	(SVN_BASE_PROTOCOL_PLAN + 39)
#define SVT_PLN_TYPE_DELAY_BETWEEN_REPS	VFT_INT32
#define SVS_PLN_TYPE_DELAY_BETWEEN_REPS	"Type of Delay Between Repetitions"
/*	ＴＡＭＩによって作られたフィールド。サブプロトコル毎の繰り返しを指定する
	際に繰り返しを行なう時にブレスホールド等のポーズを発生させる仕組みを作ろ
	うとしていたようである。現在は使われていない。

	This field describes the type (constant, variable or unspecified) of
	delay between successive repetitions.  This quantity is selected by the
	operator in ProEdit.  The contents of this field should be one of the
	values defined in vf_appcodes.h for this field.
	Written by ProEdit.
	*/

#define	SVN_PLN_ANGIO_OPTION		(SVN_BASE_PROTOCOL_PLAN + 40)
#define	SVT_PLN_ANGIO_OPTION		VFT_INT32
#define	SVS_PLN_ANGIO_OPTION		"Angiography Options"
/*	アンギオのオプションを指定するフィールド V2.0 から SVN_PLN_APP_OPTION
	に統合された。

	This indicates the type of angiographic protocol to be executed.  This
	information originates in the sequence database.  The contents of this
	field should be one of the values defined in vf_appcodes.h for this
	field.
	Written by proEdit.
	*/

#define	SVN_PLN_SE_OPTION		(SVN_BASE_PROTOCOL_PLAN + 41)
#define	SVT_PLN_SE_OPTION		VFT_BITFIELD
#define	SVS_PLN_SE_OPTION		"Spin Echo Options"
/*	SE のオプションを指定するフィールド V2.0 から SVN_PLN_APP_OPTION に
	統合された。

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_RES			(SVN_BASE_PROTOCOL_PLAN + 42)
#define	SVT_PLN_RES			VFT_FLTVEC2
#define	SVS_PLN_RES			"Resolving power of protocol"
/*	解像度。

	This describes the resolving power of the protocol.  It indicates the
	smallest structure that the protocol can resolve.  This should not be
	interpreted as the mm/pixel resolution of an image being displayed as
	various zooming or scalings may have been applied.  This parameter is
	not selected by the operator but is a result of the selected FOV and
	displayed matrix.
	*/

#define SVN_PLN_SEQ_CLASS		(SVN_BASE_PROTOCOL_PLAN + 43)
#define SVT_PLN_SEQ_CLASS		VFT_STRING
#define SVS_PLN_SEQ_CLASS		"Sequence class for prolim tree restructuring"
/*	シーケンスクラス。V2.0までは 2D,3D の２種だったが、V2.0+α以降では
	シーケンス種毎に設定。

	This describes the class of the sequence for prolim tree restructuring.
	When prolim switches between sequences of the same class prolim can
	successfully restructure the tree.
	*/

#define SVN_PLN_NUM_FSE_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 44)
#define SVT_PLN_NUM_FSE_ECHOES		VFT_INT32
#define SVS_PLN_NUM_FSE_ECHOES		"FSE echo train length"
/*	FastSE のエコートレイン。

	This describes the number of echoes acquired at one time for a FSE
	sequence.  It has no direct connection with the number of images
	produced per slice.
	*/

#define	SVN_PLN_APP_OPTION		(SVN_BASE_PROTOCOL_PLAN + 45)
#define	SVT_PLN_APP_OPTION		VFT_INT32
#define	SVS_PLN_APP_OPTION		"Application Options"
/*	アプリケーションオプション。シーケンスを切替えるための条件。
	従来は SE_OPTIONS, ANGIO_OPTIONS のようにシーケンス毎にオプションの
	フィールドを保持していたが V2.0 から１つに統合された。このことに
	より新規シーケンスを追加した場合でもオプションの追加を用意に行なう
	ことができる。

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_APP_CONDITION		(SVN_BASE_PROTOCOL_PLAN + 46)
#define	SVT_PLN_APP_CONDITION		VFT_BITFIELD
#define	SVS_PLN_APP_CONDITION		"Application Condition"
/*	アプリケーションコンディション。ユーザが proEdit 上で指定する。
	この値を seqgen が参照することにより外部から seqgen に対して
	条件を渡すことができる。このフィールドの具体的な使用方法はは
	シーケンスによってに決められる。そのため主に実験的な要素で使
	用される。

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_FLOP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 47)
#define	SVT_PLN_FLOP_ANGLE		VFT_INT32
#define	SVS_PLN_FLOP_ANGLE		"Flop angle for field echo sequences"
/*	フロップ角
	
	This field is the flop angle in degrees.  This quantity is selected by
	the operator in ProEdit.
	*/

#define SVN_PLN_NUM_DUMMY_SHOT		(SVN_BASE_PROTOCOL_PLAN + 48)
#define SVT_PLN_NUM_DUMMY_SHOT		VFT_INT32
#define SVS_PLN_NUM_DUMMY_SHOT		"The number of dummy shot"
/*	空打ち回数。
	*/

#define SVN_PLN_PE_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 49)
#define SVT_PLN_PE_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_PE_FLOW_ENCODE_VALUE	"Flow encode value(PE)"
/*	ＰＳ-ＭＲＡでのフローエンコード量（ＰＥ方向）
	ユーザが proEdit で設定する。
	*/

#define SVN_PLN_RO_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 50)
#define SVT_PLN_RO_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_RO_FLOW_ENCODE_VALUE	"Flow encode value(RO)"
/*	ＰＳ-ＭＲＡでのフローエンコード量（ＲＯ方向）
	ユーザが proEdit で設定する。
	*/

#define SVN_PLN_SE_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 51)
#define SVT_PLN_SE_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_SE_FLOW_ENCODE_VALUE	"Flow encode value(SE)"
/*	ＰＳ-ＭＲＡでのフローエンコード量（ＳＥ方向）
	ユーザが proEdit で設定する。
	*/

#define SVN_PLN_IR_PULSE		(SVN_BASE_PROTOCOL_PLAN + 52)
#define SVT_PLN_IR_PULSE		VFT_INT32
#define SVS_PLN_IR_PULSE		"Inversion Recovery Pulse (on/off)"
/*	インバージョンパルスのＯＮ／ＯＦＦを記述するフィールド。

	1994.10.06
	追加：IRのモードが全部で３つ想定される。R１では、TI=20-40ms に
	対応する単純付加パターンを用いるが、TIとTRの関係でlink構造が変化
	を受ける。
	TI, TR, TE に依存し、モードによりスライス枚数の計算法に影響を与える。
	現在は、Bool で定義されているためint32 に変更する。
	VFC_IR_PULSE_NONE	0
	VFC_IR_PULSE_FMM	1 : Frequency Multiplexed Method (TAMI方式)
	VFC_IR_PULSE_STIR	2 : Time Multiplexed Method 
	VFC_IR_PULSE_FLAIR	3 : 	〃
	VFC_IR_PULSE_IR2	4 : 	〃
	*/

#define SVN_PLN_TIME_CTRL_PARAM		(SVN_BASE_PROTOCOL_PLAN + 53)
#define SVT_PLN_TIME_CTRL_PARAM		VFT_TIME_CTRL_PARAM
#define SVS_PLN_TIME_CTRL_PARAM		"Time control parameters."
/*	時間軸方向へスキャンを行なう場合のパラメータ。
	VFT_TIME_CTRL_PARAM は vf_apptype.h で定義されている。
	*/

#define SVN_PLN_SCAN_ANATOMY		(SVN_BASE_PROTOCOL_PLAN + 54)
#define SVT_PLN_SCAN_ANATOMY		VFT_INT32
#define SVS_PLN_SCAN_ANATOMY		"Scan anatomy"
/*	部位コードを記述するフィールド。ユーザがＰＱＭにて指定する。
	ＳＡＲ計算に使用される。
	*/

#define SVN_PLN_TIME_DIM_SCAN		(SVN_BASE_PROTOCOL_PLAN + 55)
#define SVT_PLN_TIME_DIM_SCAN		VFT_BOOL
#define SVS_PLN_TIME_DIM_SCAN		"Time dimension scan flag"
/*	時間軸方向へのスキャンをするかどうかのフラグ。現在、時間軸方向への
	スキャンを行なうものは、ダイナミックスキャンとゲートスキャンでのシ
	ネのみである。
	*/

#define SVN_PLN_RECOVERY_TIME		(SVN_BASE_PROTOCOL_PLAN + 56)
#define SVT_PLN_RECOVERY_TIME		VFT_FLT32
#define SVS_PLN_RECOVERY_TIME		"Recovery Time"
/*	リカバリータイム（FastFE にて使用される）。
	ユーザが proEdit で設定する。２Ｄでは常に０となる。
	*/

#define SVN_PLN_3D_EXTRA_SLICES		(SVN_BASE_PROTOCOL_PLAN + 57)
#define SVT_PLN_3D_EXTRA_SLICES		VFT_INT32
#define SVS_PLN_3D_EXTRA_SLICES		"Extra slices for 3D"
/*	３Ｄの折り返し対策用のフィールド。ユーザ指定枚数の１割増し
	で収集し再構成後に余剰スライスを削除する方式。スライス番号
	の若い方から削除する枚数を指定する。
		D[0] = 削除枚数（スライス開始端）
		D[1] = 削除枚数（スライス終端）
	*/

#define SVN_PLN_DYNAMIC_SCAN		(SVN_BASE_PROTOCOL_PLAN + 58)
#define SVT_PLN_DYNAMIC_SCAN		VFT_BOOL
#define SVS_PLN_DYNAMIC_SCAN		"Dynamic scan flag"
/*	ダイナミックスキャンをするかどうかのフラグ。ダイナミックス
	キャンの場合時間軸方向のスキャンをするというフラグ
		SVN_PLN_TIME_CTRL_PARAM
	も同時に設定される。
	*/

#define SVN_PLN_QUAD_MUX		(SVN_BASE_PROTOCOL_PLAN + 59)
#define SVT_PLN_QUAD_MUX		VFT_INT32
#define SVS_PLN_QUAD_MUX		"Quad scan mux"
/*	QUAD SCAN での MUX。
	１回の励起パルスでとれる枚数
	*/

#define SVN_PLN_SELECTED_PRESAT_THK	(SVN_BASE_PROTOCOL_PLAN + 60)
#define SVT_PLN_SELECTED_PRESAT_THK	VFT_FLT32
#define SVS_PLN_SELECTED_PRESAT_THK	"Selected Presat Thickness"
/*	プロトコルノードに記述されるプリサチュレーション幅。この値が
	ビジュアルグループ、プリサットノードにコピーされる。プロリム
	が新規にサチュレーションを追加する際にはこの値を使用する。
  	*/

#define SVN_PLN_AFI_MODE		(SVN_BASE_PROTOCOL_PLAN + 61)
#define SVT_PLN_AFI_MODE		VFT_BITFIELD
#define SVS_PLN_AFI_MODE		"flag for AFI mode (axis and direction)"
/*	AFI 再構成の有無と、適用される軸を指定するフラグ。軸については Bit Or で
        選択を行なう。
	VFC_AFI_PE_HEAD		1
        VFC_AFI_PE_TAIL		2
	VFC_AFI_RO_HEAD		4
	VFC_AFI_RO_TAIL 	8
	VFC_AFI_SE_HEAD 	16
	VFC_AFI_SE_TAIL 	32
	要素２の１次元配列とし、デュアルコントラストに対応する。
  	*/

#define SVN_PLN_AFI_IMAGE_MODE		(SVN_BASE_PROTOCOL_PLAN + 62)
#define SVT_PLN_AFI_IMAGE_MODE		VFT_INT32
#define SVS_PLN_AFI_IMAGE_MODE		"Imaging mode (negative pixcel on/off) flag"
/*	AFI 再構成の際、負の画像値を表現するフラグ。
	0: 負の値として表示
	1: 零に丸める
	2: 絶対値をとり正の値として表示
	要素２の１次元配列とし、デュアルコントラストに対応する。
	*/


#define SVN_PLN_ECHO_FACTOR		(SVN_BASE_PROTOCOL_PLAN + 63)
#define SVT_PLN_ECHO_FACTOR		VFT_INT32
#define SVS_PLN_ECHO_FACTOR		"Echo Factor"
/*	高速スキャン（FastSE, MultiShot EPI, GRASE)による実効エコー数
	AFI を併用しない場合、
	GRASE : numEchoFactor = numRF_Echoes * numEPI_Echoes
	EPI   : numEchoFactor = numEPI_Echoes
	FastSE: numEchoFactor = numRF_Echoes
	AFI を使用した場合、
	上記により、求められる倍率にさらにAFI により位相エンコード方向に
	高速化される部分を含む。例えば、numRF_Echoes =3 で、AFIを組み
	合わせるとnumEchoFactor = 5 となる。 
	*/

#define SVN_PLN_NUM_ECHO_FACTOR_SHOTS	(SVN_BASE_PROTOCOL_PLAN + 64)
#define SVT_PLN_NUM_ECHO_FACTOR_SHOTS	VFT_INT32
#define SVS_PLN_NUM_ECHO_FACTOR_SHOTS	"number of echo factor shots"
/*	  高速スキャン（FastSE, MultiShot EPI, GRASE)による位相エンコード
	ショット数。ただし、numDummyShot と numTemplateShot を含まない
	実際の画像再構成に用いられるショット。
	vf.PeFull( 位相エンコードマトリックスサイズ＊NoWrapParameter)
	と vf.EchoFactor を用いて、
	vf.EchoFactorShot = floor (vf.PeFull/ vf.EchoFactor)
	if (vf.EchoFactorShot % 2 != 0 && vf.Nex % 2 != 0 ) vf.EchoFactorShot
	また、DCアーチファクト対策のため、奇数アベレージの際にはショット数を
	偶数に選ぶ必要がある。
	  FASE の場合、従来とは逆にnumShots を、ユーザー指定パラメータとし、
	numRF_Echoes の方をマトリックスサイズに依存する誘導パラメータとする。
	*/

#define SVN_PLN_NUM_FASE_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 65)
#define SVT_PLN_NUM_FASE_ECHOES		VFT_INT32
#define SVS_PLN_NUM_FASE_ECHOES		"number of FASE echoes"
/*	FASEスキャンによるエコー数。コントラストを決定するパラメータでありスキャン
	時間を左右するnumRF_Echoes とは別。
	*/

#define SVN_PLN_NUM_RF_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 66)
#define SVT_PLN_NUM_RF_ECHOES		VFT_INT32
#define SVS_PLN_NUM_RF_ECHOES		"number of RF Echoes"
/*	Hybrid EPI, FastSE, FASE におけるRF エコー数。互換性のために新設。
	*/

#define SVN_PLN_NUM_ECHO_TRAIN		(SVN_BASE_PROTOCOL_PLAN + 67)
#define SVT_PLN_NUM_ECHO_TRAIN		VFT_INT32
#define SVS_PLN_NUM_ECHO_TRAIN		"number of echo train"
/*	numEchoTrain = numRF_Echoes* numEPI_Echoes
	で定義されるマルチショット系高速スキャンによるトータルのエコー数
	*/

#define SVN_PLN_MAX_IMAGING_PULSES	(SVN_BASE_PROTOCOL_PLAN + 68)
#define SVT_PLN_MAX_IMAGING_PULSES	VFT_INT32
#define SVS_PLN_MAX_IMAGING_PULSES	"max imaging pulses per coverage"
/*	１スキャン（カバレージ）でとり得る最大スライス枚（スラブ）数。
	*/

#define SVN_PLN_NUM_EPI_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 69)
#define SVT_PLN_NUM_EPI_ECHOES		VFT_INT32
#define SVS_PLN_NUM_EPI_ECHOES		"number of EPI echoes"
/*	(MultiShot)EPIスキャンによるエコー数。
	定義されない時は１（numRF_Echoes も同様）
	*/

#define SVN_PLN_TURBO_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 71)
#define SVT_PLN_TURBO_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_TURBO_FLIP_ANGLE	"Flip Angle of FFE prep pulse"
/*	This is the flip angle in degrees of the preparation pulse for FFE. */

#define SVN_PLN_VENC_DIR		(SVN_BASE_PROTOCOL_PLAN + 72)
#define SVT_PLN_VENC_DIR		VFT_INT32
#define SVS_PLN_VENC_DIR		"Velocity Encoding Direction"
/*	This is the velocity encoding direction for Phase Shift Vascular
	Imaging.  The contents of this field should be one of the values
	defined in vf_appcodes.h for this field.
	*/

#define SVN_PLN_VENC_VAL		(SVN_BASE_PROTOCOL_PLAN + 73)
#define SVT_PLN_VENC_VAL		VFT_FLT32
#define SVS_PLN_VENC_VAL		"Velocity Encoding Value in m/sec"
/*	This is the velocity encoding value for Phase Shift Vascular Imaging.
	*/

#define SVN_PLN_FLD_STBL_SLICE_TIME		(SVN_BASE_PROTOCOL_PLAN + 74)
#define SVT_PLN_FLD_STBL_SLICE_TIME		VFT_FLT32
#define SVS_PLN_FLD_STBL_SLICE_TIME		"Imaging time per FS slice"
/*      Field stability slice time, set in prolim
	*/

#define SVN_PLN_LINE_TEMPLATES		(SVN_BASE_PROTOCOL_PLAN + 75)
#define SVT_PLN_LINE_TEMPLATES		VFT_BOOL
#define SVS_PLN_LINE_TEMPLATES		"Field stability line templates"
/*      Are there field stability line templates
	*/

#define SVN_PLN_SEQ_OPTION		(SVN_BASE_PROTOCOL_PLAN + 76)
#define	SVT_PLN_SEQ_OPTION		VFT_STRING
#define	SVS_PLN_SEQ_OPTION		"Sequence option for display"
/*	ディスプレイで表示するシーケンスオプションを記述する。内容は
	収集条件に併せて prolim が作成する。
	*/

#define SVN_PLN_NUM_PS_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 77)
#define SVT_PLN_NUM_PS_ECHOES		VFT_INT32
#define SVS_PLN_NUM_PS_ECHOES		"The number of PS echoes"
/*	PSMRA のエコー数(２点法／４点法)を記述する。
	*/

#define SVN_PLN_SLICE_OVERWRAP		(SVN_BASE_PROTOCOL_PLAN + 78)
#define SVT_PLN_SLICE_OVERWRAP		VFT_INT32
#define SVS_PLN_SLICE_OVERWRAP		"Slice Overwarp for MRA 3D multi Cov."
/*	MRA3D マルチカバレージオーバーラップ量。
	*/

#define SVN_PLN_AFI_DIRECTION		(SVN_BASE_PROTOCOL_PLAN + 79)
#define SVT_PLN_AFI_DIRECTION		VFT_INT32
#define SVS_PLN_AFI_DIRECTION		"flag for AFI mode (axis and direction)"
/*	AFI 再構成の有無と、適用される軸を指定するフラグ。軸については Bit Or で
        選択を行なう。
	VFC_AFI_PE		0x03
	VFC_AFI_RO		0x0C
	VFC_AFI_SE		0x30
	要素２の１次元配列とし、デュアルコントラストに対応する。
  	*/

#define SVN_PLN_MIN_TR		(SVN_BASE_PROTOCOL_PLAN + 80)
#define SVT_PLN_MIN_TR		VFT_FLT32
#define SVS_PLN_MIN_TR		"Minimu TR"
/*	最小TR（サチュレーションを含む）
	*/

#define SVN_PLN_SELECTED_TAG_THK	(SVN_BASE_PROTOCOL_PLAN + 81)
#define SVT_PLN_SELECTED_TAG_THK	VFT_FLT32
#define SVS_PLN_SELECTED_TAG_THK	"Selected Tag Thickness"
/*	プロトコルノードに記述されるタグ幅。この値がビジュアルグループ、
	プリサットノードにコピーされる。プロリムが新規にタグを追加する際には
	この値を使用する。
  	*/

#define SVN_PLN_VARI_FLIP_FLAG		(SVN_BASE_PROTOCOL_PLAN + 82)
#define SVT_PLN_VARI_FLIP_FLAG		VFT_BOOL
#define SVS_PLN_VARI_FLIP_FLAG		"Variable Flip Angle Flag"
/*	FFEでのバリアブルフリップアングルのためのフラグ
	このフラグが真の場合は SVN_PLN_FLIP_ANGLE の意味が変わる。
	SVN_PLN_FLIP_MIN_ANGLE < x < SVN_PLN_FLIP_ANGLE
	でのバリアブルフリップアングルとなる。SARはフリップ角最大値で
	計算するため、SAR関連のソースコードを修正する必要は無い。
	*/

#define	SVN_PLN_FLIP_MIN_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 83)
#define	SVT_PLN_FLIP_MIN_ANGLE		VFT_INT32
#define	SVS_PLN_FLIP_MIN_ANGLE		"Flip angle for field echo sequences"
/*	バリアブルフリップアングル指定時のフリップ角最小値
	*/

#define SVN_PLN_NUM_EPI_TE_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 84)
#define SVT_PLN_NUM_EPI_TE_ECHOES	VFT_INT32
#define SVS_PLN_NUM_EPI_TE_ECHOES	"Number of contrast ETL for single shot EPI (SE and FE)"
/*	AFI再構成のためのパラメータと、シーケンス組み立てのための
	パラメータを兼ねる。k空間中心までのETL(エコートレイン長)を
	指定。このパラメータとPE方向のマトリックスサイズ、ショット数、
	NoWrapからNUM_EPI_ECHOESを算出する。
	This is parameter for AFI recon and sequence stracture.
	Echo train length of first to k space center.
	*/

#define SVN_PLN_SNAP_SHOT_MODE		(SVN_BASE_PROTOCOL_PLAN + 85)
#define SVT_PLN_SNAP_SHOT_MODE		VFT_INT32
#define SVS_PLN_SNAP_SHOT_MODE		"Flag for snap shot mode"
/*	Dynamic+TR>=WaitTimeの時、ユーザー入力パラメータとして、
	ダミーショット、テンプレートショットを零に指定するための
	フラグ。

	When dynamic scan and wait time < TR, this switch provide
	mode select dummy and template shot are zero or default 
	values. 
	*/

#define SVN_PLN_SKIP_SAT_FLAG		(SVN_BASE_PROTOCOL_PLAN + 86)
#define SVT_PLN_SKIP_SAT_FLAG		VFT_INT32
#define SVS_PLN_SKIP_SAT_FLAG		"Flag for skipping sat/fat"
/*	スキッピングサチュレーションフラグ
 */

#define SVN_PLN_SKIP_SAT_INTERVAL	(SVN_BASE_PROTOCOL_PLAN + 87)
#define SVT_PLN_SKIP_SAT_INTERVAL	VFT_FLT32
#define SVS_PLN_SKIP_SAT_INTERVAL	"Interval time for skipping saturation"
/*	有効プリサチュレーションパルス時間。
	プリサチュレーションパルスを離すことができる限界時間。（単位:秒）
	*/

#define SVN_PLN_SKIP_FAT_INTERVAL	(SVN_BASE_PROTOCOL_PLAN + 88)
#define SVT_PLN_SKIP_FAT_INTERVAL	VFT_FLT32
#define SVS_PLN_SKIP_FAT_INTERVAL	"Interval time for skipping fatsat"
/*	有効ファットサットパルス時間。
	ファットサットパルスを離すことができる限界時間。（単位:秒）
	*/

#define	SVN_PLN_BBTI			(SVN_BASE_PROTOCOL_PLAN + 89)
#define	SVT_PLN_BBTI			VFT_FLT32
#define	SVS_PLN_BBTI			"Black Blood Inversion Time, TI"
/*	ブラッグブラッドインバージョンパルスを付加する際のTI時間(bbTI)時間を
	記述する。単位は秒。
	*/

#define SVN_PLN_BBIR_PULSE		(SVN_BASE_PROTOCOL_PLAN + 90)
#define SVT_PLN_BBIR_PULSE		VFT_INT32
#define SVS_PLN_BBIR_PULSE		"BB Inversion Recovery Pulse (on/off)"
/*	ブラッグブラッッドインバージョンパルスのＯＮ／ＯＦＦを記述するフィールド。
	*/

#define SVN_PLN_INV_PULSE_TYPE	(SVN_BASE_PROTOCOL_PLAN + 91)
#define SVT_PLN_INV_PULSE_TYPE	VFT_INT32
#define SVS_PLN_INV_PULSE_TYPE	"Inversion Recovery Pulse Type"
/*	インバージョンパルスの型を記述するフィールド。
	0:	SINC
	1:	ADIABATIC
	*/
	
#define	SVN_PLN_PRESAT_FAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 92)
#define	SVT_PLN_PRESAT_FAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_PRESAT_FAT_FLIP_ANGLE	"Flip angle for presat (fat)"
/*	プリサチュレーション(脂肪)フリップアングル
 */

#define SVN_PLN_SEQ_FAMILY		(SVN_BASE_PROTOCOL_PLAN + 93)
#define SVT_PLN_SEQ_FAMILY		VFT_STRING
#define SVS_PLN_SEQ_FAMILY		"Sequence family for NSDE"
/*	This describes the family name of the sequence parameters.
	*/

#define SVN_PLN_NSDE_OPTION		(SVN_BASE_PROTOCOL_PLAN + 94)
#define SVT_PLN_NSDE_OPTION		VFT_BITFIELD
#define SVS_PLN_NSDE_OPTION		"Sequence options for NSDE"
/*	This describes options of the sequence.
	*/

/* Gap in numbering..... */


/* ---- Written by the PQM */

#define	SVN_PLN_RTL_FLAG		(SVN_BASE_PROTOCOL_PLAN + 100)
#define	SVT_PLN_RTL_FLAG		VFT_BOOL
#define	SVS_PLN_RTL_FLAG		"RTL indicator flag"
/*	This field indicates whether the protocol is a RTL (TRUE) or
	whether it is not (FALSE).  It is written by the PQM.  AT THIS TIME
        THE RTL IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_CALC_SAR_FLAG		(SVN_BASE_PROTOCOL_PLAN + 101)
#define	SVT_PLN_CALC_SAR_FLAG		VFT_BOOL
#define	SVS_PLN_CALC_SAR_FLAG		"Calc SAR Flag"
/*	PROLIM でＳＡＲ計算を行なうかどうかを示すフィールド。
	PQMが記述する。APC以前であれば偽。APC以後にPROLIMが起動される
	場合このフィールドを真にすることでＳＡＲを加味したプランとなる。
	*/

#define SVN_PLN_STATUS			(SVN_BASE_PROTOCOL_PLAN + 102)
#define SVT_PLN_STATUS			VFT_BITFIELD
#define SVS_PLN_STATUS			"Plan Status"
/*	収集条件編集のために起動されたプロセス／親画像の有無を登録する。

	VFC_PLN_STATUS_PARENT		(1<<0)
	
	VFC_PLN_STATUS_PROEDIT		(1<<1)
	VFC_PLN_STATUS_GLOC		(1<<2)
	VFC_PLN_STATUS_GATING		(1<<3)
	VFC_PLN_STATUS_DYNAMIC		(1<<4)
	VFC_PLN_STATUS_VGCOPY		(1<<5)

	VFC_PLN_STATUS_PROMON		(1<<16)
	VFC_PLN_STATUS_PRODEBUG		(1<<17)
	*/
  
#define SVN_PLN_NUM_ECHO_ACQUIRED	(SVN_BASE_PROTOCOL_PLAN + 103)
#define SVT_PLN_NUM_ECHO_ACQUIRED	VFT_INT32
#define SVS_PLN_NUM_ECHO_ACQUIRED	"Number of aquired echoes for echo train fast scan"
/*	第ｉコントラストの画像に使用するエコーのうち、スキャンによって
	実際に収集するエコーの数。
	Dual contrast scanのことを考慮して、要素数は２とする。
	*/

#define SVN_PLN_NUM_ECHO_AFI_FILLED	(SVN_BASE_PROTOCOL_PLAN + 104)
#define SVT_PLN_NUM_ECHO_AFI_FILLED	VFT_INT32
#define SVS_PLN_NUM_ECHO_AFI_FILLED	"Number of echoes filled by AFI recon"
/*	第ｉコントラストの画像に使用するエコーのうち、AFI処理によって充填する
	エコーの数。Dual contrast scanのことを考慮して、要素数は２とする。
	*/

#define	SVN_PLN_NUM_ECHO_SHARED		(SVN_BASE_PROTOCOL_PLAN + 105)
#define SVT_PLN_NUM_ECHO_SHARED		VFT_INT32
#define SVS_PLN_NUM_ECHO_SHARED		"Number of echoes shared by i th and i+1 th contrast image"
/*	第ｉコントラストと第ｉ+1コントラストの画像で共用するエコーの数。
	現状では各コントラストの画像のうち、実効TEの面で隣合わない画像同士での
	エコーデータの共用は無いものとする。
	*/

#define SVN_PLN_IR_EXCITED_SLICES	(SVN_BASE_PROTOCOL_PLAN + 106)
#define SVT_PLN_IR_EXCITED_SLICES	VFT_INT32
#define SVS_PLN_IR_EXCITED_SLICES	"Actual excitaion number of slices on FLAIR mode"
/*	スライスオフセットモードで実行されるIR, FLAIR などにおいて、
	データ収集の他に余分なスライスの励起を行なうが、RFパルスは
	スライス間感度ムラを低減するため、Offできず、SAR 的には余分な
	スライス枚数をSAR計算に算入する必要がある。
	*/

#define SVN_PLN_IR_PULSE_BAND_WIDTH	(SVN_BASE_PROTOCOL_PLAN + 107)
#define SVT_PLN_IR_PULSE_BAND_WIDTH	VFT_INT32
#define SVS_PLN_IR_PULSE_BAND_WIDTH	"RF band width for IR pulse."
/*	IR, FLAIR モードにおいては、IR パルスの帯域幅によりMTC 
	効果が変化し、画像コントラストに影響を与える。さらに、
	narrow band を実現するために、90+90 の２回印加も検討中で
	これら各種IRパルス波形を、ユーザーインターフェイスから
	指定可能とする。
	*/

#define SVN_PLN_AUTO_VOICE	(SVN_BASE_PROTOCOL_PLAN + 108)
#define SVT_PLN_AUTO_VOICE	VFT_INT32
#define SVS_PLN_AUTO_VOICE	"The flag for auto voice."
/*	FASE3D,Dyanmic撮影でのオートボイス使用の有無を表す。
	FASE3Dでは、音声発生のための十分な時間的余裕がない場合は自動的に
	OFFとなる。
	0:オートボイスOFF
	1:息止め音声１		「息を吸って止めてください」
	2:息止め音声２		「息を吐いて止めてください」
	3:息止め音声３		「息を吸って吐いて止めてください」
	4:体動防止音声		「動かないでください」
	*/

#define SVN_PLN_AUTO_VOICE_TPCTRL	(SVN_BASE_PROTOCOL_PLAN + 109)
#define SVT_PLN_AUTO_VOICE_TPCTRL	VFT_INT32
#define SVS_PLN_AUTO_VOICE_TPCTRL	"The flag for auto voice with TP CTRL."
/*	FASE3D,Dyanmic撮影でのTP割り込みに要オートボイス使用の有無を表す。
	音声種は PLN_AUTO_VOICE を使用する。
	*/

#define SVN_PLN_TAG_IR_THICKNESS	(SVN_BASE_PROTOCOL_PLAN + 110)
#define SVT_PLN_TAG_IR_THICKNESS	VFT_FLT32
#define SVS_PLN_TAG_IR_THICKNESS	"TAG IR Thickness for ASL"	
/*	ASLでのTAG IRの厚さ
 */

#define SVN_PLN_CONTROL_IR_OFFSET	(SVN_BASE_PROTOCOL_PLAN + 111)
#define SVT_PLN_CONTROL_IR_OFFSET	VFT_FLT32
#define SVS_PLN_CONTROL_IR_OFFSET	"CONTROL IR Thickness for ASL"	
/*	ASLでのCONTROL IR中心とスライス中心との距離
 */

#define SVN_PLN_TAG_IR_OFFSET	(SVN_BASE_PROTOCOL_PLAN + 112)
#define SVT_PLN_TAG_IR_OFFSET	VFT_FLT32
#define SVS_PLN_TAG_IR_OFFSET	"TAG IR Thickness for ASL"	
/*	ASLでのTAG IR中心とスライス中心との距離
 */

#define SVN_PLN_PREP_SCAN	(SVN_BASE_PROTOCOL_PLAN + 113)
#define SVT_PLN_PREP_SCAN	VFT_INT32
#define SVS_PLN_PREP_SCAN	"Prep Scan Flag"
/*	PrepScan を使用するかどうかを表す。
 */

#define SVN_PLN_RF_SPOILING	(SVN_BASE_PROTOCOL_PLAN + 114)
#define SVT_PLN_RF_SPOILING	VFT_INT32
#define SVS_PLN_RF_SPOILING	"RF Spoiling flag"
/*	FE、FFE、TOF,PSMRAシーケンスでT1コントラストを強調したり
	血流コントラストを上げる機能としてRF Spoilingという機能を
	使用している。
	ただし、この機能を用いるとT2値の長い脳脊髄液の信号は低下して
	しまうので、流速測定対象が脳脊髄液である場合には、Spoilingを
	オフにすることが必要。
	また、FEシーケンスの場合、T2＊強調シーケンスではSpoilingを
	オフにしたい。

	０ → Spoilingはオフ。
	１ → Spoilingはオン。
	*/

#define SVN_PLN_INTERMIT_FLAG	(SVN_BASE_PROTOCOL_PLAN + 115)
#define SVT_PLN_INTERMIT_FLAG	VFT_INT32
#define	SVS_PLN_INTERMIT_FLAG	"Intermittent breath hold"
/*	間欠息止めを指定する。
	*/

#define SVN_PLN_INTERMIT_REPEAT	(SVN_BASE_PROTOCOL_PLAN + 116)
#define SVT_PLN_INTERMIT_REPEAT	VFT_INT32
#define SVS_PLN_INTERMIT_REPEAT	"Slice encode before intermittent breath hold"
/*	間欠息止めの前に繰り返すスライスエンコード数。
	SPEEDの場合は、重なり合う２スライスを１スライスエンコードと
	考える。
	*/

#define SVN_PLN_INTERMIT_TIME	(SVN_BASE_PROTOCOL_PLAN + 117)
#define SVT_PLN_INTERMIT_TIME	VFT_INT32
#define SVS_PLN_INTERMIT_TIME	"Rest time for intermittent breath hold"
/*	間欠息止めを行う休止時間を指定する。単位は秒。
	*/

#define SVN_PLN_T2_PLUS		(SVN_BASE_PROTOCOL_PLAN + 118)
#define SVT_PLN_T2_PLUS		VFT_INT32
#define SVS_PLN_T2_PLUS		"T2 Plus"
/*	T2 PlusのON/OFFを指定する。

	T2 Plusはデータを収集した後に、時間をおいて縦磁化への回復を
	待つのではなく、90°RFパルスを印加することで、強制的に縦磁化への
	回復を促す方法である。強制的に回復させることで、繰返し時間を
	短くしてもT1、T2の長い成分の信号を強めることができ、T2強調撮像での
	時間短縮を図ることができる。
	T2 Plus部は、エコー系列のデータを収集した後に置かれ、複数の180°
	RFパルスと１つの 90°RFパルスで構成される。
 */

#define SVN_PLN_PRECONTRAST_TE		(SVN_BASE_PROTOCOL_PLAN + 119)
#define SVT_PLN_PRECONTRAST_TE		VFT_FLT32
#define SVS_PLN_PRECONTRAST_TE		"Precontrast pulse TE"
/* Node:SVN_ST_PROTOCOL
	プレコントラスト(PreC)パルスを付加する際のTE時間の増加分
    (PreCTI)を記述する。単位ば秒。*/

 
#define SVN_PLN_B_VALUE		(SVN_BASE_PROTOCOL_PLAN + 120)
#define SVT_PLN_B_VALUE		VFT_INT32
#define SVS_PLN_B_VALUE		"B Value"
/* Node:SVN_ST_PROTOCOL
	動きによる位相シフトの度合を表すパラメータで、$[s/m^2]$の単位
    で表したもの。方向はSVN_PLN_IMAGE_PATTERNによって決定する。*/

#define SVN_PLN_SELECTED_BASE_TE    (SVN_BASE_PROTOCOL_PLAN + 121)
#define SVT_PLN_SELECTED_BASE_TE    VFT_FLT32
#define SVS_PLN_SELECTED_BASE_TE    "Selected  base contrast TE"
/*      This is the TE that the selected sequences is Based on (usually
        the minimun TE for the sequence)
        The contents of this field originate in the
        sequence database.
        */

#define SVN_PLN_BASE_RF_ECHO_SPACE    (SVN_BASE_PROTOCOL_PLAN + 122)
#define SVT_PLN_BASE_RF_ECHO_SPACE    VFT_FLT32
#define SVS_PLN_BASE_RF_ECHO_SPACE    "Base RF Echo Space"
/*      This is the RF echo space that the selected sequences is Based on (usually
        the minimun RF echo space for the sequence)
        The contents of this field originate in the sequence database.
        */

#define SVN_PLN_BASE_GR_ECHO_SPACE    (SVN_BASE_PROTOCOL_PLAN + 123)
#define SVT_PLN_BASE_GR_ECHO_SPACE    VFT_FLT32
#define SVS_PLN_BASE_GR_ECHO_SPACE    "Base Grad Echo Space"
/*      This is the Grad echo space that the selected sequences is Based on (usually
        the minimun gradient echo space for the sequence)
        The contents of this field originate in the sequence database.
        */

#define SVN_PLN_NUM_TE_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 124)
#define SVT_PLN_NUM_TE_ECHOES	VFT_INT32
#define SVS_PLN_NUM_TE_ECHOES	"Number of echoes between TE"
/*	This is the number of echoes between TE
        In case of EPI, it coresponds to SVN_PLN_NUM_EPI_TE_ECHOES.
        In case of FASE, it corresponds to SVN_PLN_NUM_FASE_ECHOES.
        SVN_PLN_NUM_FSE_TE_ECHOES has similar name, but it has different meening.
        It is contructed from TE echoes, echo factor and actual ETL.
        In case of conventinal SE and FE, this field should be one.
	*/

#define SVN_PLN_SELECTED_RF_TYPE    (SVN_BASE_PROTOCOL_PLAN + 125)
#define SVT_PLN_SELECTED_RF_TYPE    VFT_INT32
#define SVS_PLN_SELECTED_RF_TYPE    "Selected RF type"
/*      This is RF pulse type which is used by NSDE.
        Sequence designer defines a set of RF type which includes two types
        of information; slice sharpness and SAR reduction. Based on
        this parameter, pre-defined parameter set is used by NSDE.
        */

#define SVN_PLN_SELECTED_GR_TYPE    (SVN_BASE_PROTOCOL_PLAN + 126)
#define SVT_PLN_SELECTED_GR_TYPE    VFT_INT32
#define SVS_PLN_SELECTED_GR_TYPE    "Selected GR type"
/*      This is gradient type which is used by NSDE.
        Sequence designer defines a set of GR type. Based on
        this parameter, pre-defined parameter set is used by NSDE.
        */

#define SVN_PLN_NUM_RF_ECHO_FACTOR      (SVN_BASE_PROTOCOL_PLAN + 127)
#define SVT_PLN_NUM_RF_ECHO_FACTOR      VFT_INT32
#define SVS_PLN_NUM_RF_ECHO_FACTOR      "Number of RF echo factor"
/*      Echo factor will be constructed from RF and GR echo factors.
        */

#define SVN_PLN_NUM_GR_ECHO_FACTOR      (SVN_BASE_PROTOCOL_PLAN + 128)
#define SVT_PLN_NUM_GR_ECHO_FACTOR      VFT_INT32
#define SVS_PLN_NUM_GR_ECHO_FACTOR      "Number of GR echo factor"
/*      Echo factor will be constructed from RF and GR echo factors.
        */


/* GAP in numbering..... */

/* ---- Mutts.
   These planning varfields are potentially written by more than
   one process.  Responsibility for them is shared by ProEdit, Memory,
   the PQM and perhaps Display. */

#define	SVN_PLN_SCAN_PLAN		(SVN_BASE_PROTOCOL_PLAN + 200)
#define	SVT_PLN_SCAN_PLAN		VFT_BOOL
#define	SVS_PLN_SCAN_PLAN		"Scan Plan Flag"
/*	This field is a flag indicating whether the operator wishes to
	perform scan planning on the protocol (TRUE) or not (FALSE).
	Scan planning indicates to the PQM that the protocol should not
	be executed automatically when it reaches the top of the queue.
	*/

#define	SVN_PLN_COMMENTS		(SVN_BASE_PROTOCOL_PLAN + 201)
#define	SVT_PLN_COMMENTS		VFT_STRING
#define	SVS_PLN_COMMENTS		"Comments"
/*	コメント。

	This field is the operator entered comments (or PAS ID for those
	who hail from Japan).  Generally, the contents of this field are
	initially entered in Memory but may be modified in the PQM.
	*/

#define	SVN_PLN_CONTR_AGENT_FL		(SVN_BASE_PROTOCOL_PLAN + 202)
#define	SVT_PLN_CONTR_AGENT_FL		VFT_BOOL
#define	SVS_PLN_CONTR_AGENT_FL		"Contrast agent used?"
/*	This field is a flag indicating whether a contrast agent was
	used (TRUE) or not (FALSE) in this protocol.  This field defaults
	to FALSE but may be modified in the PQM.
	*/

#define	SVN_PLN_POST_ACQ_MODE		(SVN_BASE_PROTOCOL_PLAN + 203)
#define	SVT_PLN_POST_ACQ_MODE		VFT_BITFIELD
#define	SVS_PLN_POST_ACQ_MODE		"Type of Post Acquistion Processing"
/*	This field indicates the type of Post Acquisition processing to be
	performed on this protocol.  This field defaults to the site's
	preferred Autoview option specified in the configuration file but it
	may be modified in the PQM.  This is a bit field.  Bit masks for this
	field are defined in vf_appcodes.h.
	*/

#define	SVN_PLN_VIEWING_DIR		(SVN_BASE_PROTOCOL_PLAN + 204)
#define	SVT_PLN_VIEWING_DIR		VFT_INT32
#define	SVS_PLN_VIEWING_DIR		"Image viewing direction"
/*	This field, only necessary to satisfy weird and incomprehensible
	Japanese market pseudo-requirements, indicates the "default"
	direction from which Display is to show the images from this
	protocol.  The contents of this field are most likely set by
	Memory or ProEdit initially, based on the contents of the
	configuration database.  The contents of the field may be
	modified by the operator in the PQM.  The contents for this field
	should be one of the values defined in vf_appcodes.h for this
	field.
	*/

#define	SVN_PLN_HOLD			(SVN_BASE_PROTOCOL_PLAN + 205)
#define	SVT_PLN_HOLD			VFT_INT32
#define	SVS_PLN_HOLD			"Wait for Start button"
/*	This field is a flag indicating whether AcqMan must arm the "Start"
	buttons and wait for the operator to hit that button in order to
	begin execution of the given protocol.  The contents of this field
	are most likely set by Memory, ProEdit or the PQM initially based on
	rules that are TBD.  The operator may modify the contents of this
	field in the PQM.

	acqman の撮影モードを決定する。
		VFC_SCAN_HOLD :  プレスキャン終了後、本スキャンの前で一時停止。
				”息止め撮影”用。
		VFC_RFL_HOLD  :  ＡＰＣ直前で一時停止。”息止めＡＰＣ”用。
		VFC_CFA_HOLD  :  ＣＦＡ直前で一時停止。
		VFC_RGN_HOLD  :  ＲＧＮ直前で一時停止。
	
	各モードの値は以下の通り
		VFC_SCAN_HOLD	= 1
		VFC_RFL_HOLD	= 2
		VFC_CFA_HOLD	= 4
		VFC_RGN_HOLD	= 8
	*/

#define	SVN_PLN_RECVR_COIL		(SVN_BASE_PROTOCOL_PLAN + 206)
#define	SVT_PLN_RECVR_COIL		VFT_INT32
#define	SVS_PLN_RECVR_COIL		"Receiver coil Database Key"
/*	受信コイル。

	This field describes which receiver coil is used in acquiring this
	protocol.  The contents of this field are selected by the operator
	in the PQM only.
	*/

#define	SVN_PLN_PATIENT_ORIENT		(SVN_BASE_PROTOCOL_PLAN + 207)
#define	SVT_PLN_PATIENT_ORIENT		VFT_PATORI
#define	SVS_PLN_PATIENT_ORIENT		"Patient orientation in the magnet"
/*	患者挿入方向。

	This field describes the orientation of the patient relative to the
	magnet.  The contents of this field are selected by the operator in
	the PQM only.  The contents of the structure that makes up this
	field should be one of the values in vf_appcodes.h for this field.
	*/

#define	SVN_PLN_LOCATOR_IMAGES		(SVN_BASE_PROTOCOL_PLAN + 208)
#define	SVT_PLN_LOCATOR_IMAGES		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGES		"1st locator image used for this protocol"
/*	参照画像。

	This field is an array of paths to the image used as locator
	for this protocol in the 1st viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/


#define	SVN_PLN_TRANSMIT_COIL		(SVN_BASE_PROTOCOL_PLAN + 209)
#define	SVT_PLN_TRANSMIT_COIL		VFT_INT32
#define	SVS_PLN_TRANSMIT_COIL		"Transmiter Coil Database Key"
/*	送信コイル。

	This field describes which transmit coil is used in acquiring this
	protocol.  The contents of this field are selected by the operator
	in the PQM only.
	*/

#define	SVN_PLN_INV_FLIP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 210)
#define	SVT_PLN_INV_FLIP_ANGLE		VFT_INT32
#define	SVS_PLN_INV_FLIP_ANGLE		"Flip angle for inversion pulse"
/*	インバージョンパルスフリップ角

	This is the flip angle in degrees applied to each inversion RF pulse.
	It is currently set by prolim or seqinit.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_2		(SVN_BASE_PROTOCOL_PLAN + 211)
#define	SVT_PLN_LOCATOR_IMAGE_2		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_2		"2nd locator image used for this protocol"
/*	２マイ位置決め時の２枚目の参照画像。

	This field is an array of paths to the image used as a locator
	for this protocol in the 2nd viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_3		(SVN_BASE_PROTOCOL_PLAN + 212)
#define	SVT_PLN_LOCATOR_IMAGE_3		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_3		"3rd locator image used for this protocol"
/*	２枚位置決め時の３枚目の参照画像。

	This field is an array of paths to the image used as locator
	for this protocol in the 3rd viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_4 	(SVN_BASE_PROTOCOL_PLAN + 213)
#define	SVT_PLN_LOCATOR_IMAGE_4		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_4		"4th locator image used for this protocol"
/*	未使用。

	This field is an array of paths to the image used as locator
	for this protocol in the 4th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_5		(SVN_BASE_PROTOCOL_PLAN + 214)
#define	SVT_PLN_LOCATOR_IMAGE_5		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_5		"5th locator image used for this protocol"
/*	未使用。

	This field is an array of paths to the image used as locator
	for this protocol in the 5th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_6		(SVN_BASE_PROTOCOL_PLAN + 215)
#define	SVT_PLN_LOCATOR_IMAGE_6		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_6		"6th locator image used for this protocol"
/*	未使用。

	This field is an array of paths to the image used as locator
	for this protocol in the 6th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define SVN_PLN_FATSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 216)
#define SVT_PLN_FATSAT_PULSE		VFT_INT32
#define SVS_PLN_FATSAT_PULSE		"Fat Suppress Pulse (on/off)"
/*	ファットサプレスパルスのＯＮ／ＯＦＦを記述するフィールド。
	
	VFT_BOOL でも良いが、ファットパルスの拡張に備えてあえて
	VFT_INT32 で定義する。現状(V2.1)では

	0: ファットパルス OFF
	1: ファットパルス ON

	とする。
	*/

#define	SVN_PLN_FATSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 217)
#define	SVT_PLN_FATSAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_FATSAT_FLIP_ANGLE	"Flip angle for fat-suppress pulse"
/*	ファットサプレスパルスフリップ角

	This is the flip angle in degrees applied to each fat-uppress RF pulse.
	It is currently set by prolim or seqinit.
	*/

#define	SVN_PLN_ANATOMICAL_CLASS	(SVN_BASE_PROTOCOL_PLAN + 218)
#define	SVT_PLN_ANATOMICAL_CLASS	VFT_INT32
#define	SVS_PLN_ANATOMICAL_CLASS	"Anatomical Class in Memory"
/*	This field describes the anatomical class the protocol was
	stored in Memory.  The contents of this field are written by PQM,
	and established when the protocol is copied from the Memorized 
	Protocol Set.
	*/

#define SVN_PLN_LOCATOR_COUCH_POS	(SVN_BASE_PROTOCOL_PLAN + 219)
#define SVT_PLN_LOCATOR_COUCH_POS	VFT_FLT32
#define SVS_PLN_LOCATOR_COUCH_POS	"Couch position of locator image"
/*	Couch position in meters.
	*/

#define SVN_PLN_FFE_WAIT_TIME	(SVN_BASE_PROTOCOL_PLAN + 220)	
#define SVT_PLN_FFE_WAIT_TIME	VFT_FLT32
#define SVS_PLN_FFE_WAIT_TIME	"Wait time before next preparation pulse"
/*	This field describes the wait time in seconds before the next
	preparation pulse in a FFE sequence. This quantity is selected by the
	operator in ProEdit.
	*/

#define	SVN_PLN_SAR_ANATOMY		(SVN_BASE_PROTOCOL_PLAN + 221)
#define	SVT_PLN_SAR_ANATOMY		VFT_INT32
#define	SVS_PLN_SAR_ANATOMY		"SAR Anatomical Class for the protocol"
/*	This field describes the anatomical part for the protocol to be used
	in SAR calculations. The contents of this field are written by PQM,
	and legal-values are defined in vf_appcodes.h.
	*/

#define SVN_PLN_SORS_PULSE		(SVN_BASE_PROTOCOL_PLAN + 222)
#define SVT_PLN_SORS_PULSE		VFT_INT32
#define SVS_PLN_SORS_PULSE		"SORS Pulse (on/off)"
/*	SORSサプレスパルスのＯＮ／ＯＦＦを記述するフィールド。
	*/

#define	SVN_PLN_SORS_FLIP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 223)
#define	SVT_PLN_SORS_FLIP_ANGLE		VFT_INT32
#define	SVS_PLN_SORS_FLIP_ANGLE		"Flip angle for SORS pulse"
/*	SORSパルスフリップ角

	This is the flip angle in degrees applied to SORS RF pulse.
	It is currently set by prolim or seqinit.
	*/

#define	SVN_PLN_SHIM_ENABLE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 224)
#define	SVT_PLN_SHIM_ENABLE_FLAG	VFT_BOOL
#define	SVS_PLN_SHIM_ENABLE_FLAG	"Exist effective SHIM Data"
/*	あるシーケンスの脂肪抑制を行なうための有効なシミングデータが
	存在するかどうかのフラグ。
	True  :  有効なシミングデータが存在する。
	False :  存在しない。
	*/

#define SVN_PLN_ISCE_MODE		(SVN_BASE_PROTOCOL_PLAN + 225)
#define SVT_PLN_ISCE_MODE		VFT_INT32
#define SVS_PLN_ISCE_MODE		"ISCE Mode"
/*	ISCE モード

	標準モード	VFC_ISCE_MODE_NORMAL		0
	1:2		VFC_ISCE_MODE_1_2		1
	1:3		VFC_ISCE_MODE_1_3		2
	*/

#define	SVN_PLN_SCAN_COMMENTS		(SVN_BASE_PROTOCOL_PLAN + 226)
#define	SVT_PLN_SCAN_COMMENTS		VFT_STRING
#define	SVS_PLN_SCAN_COMMENTS		"Scan Comments"
/*	スキャンコメント。ＰＱＭで設定するスキャンＩＤとは異なる。
	*/

#define SVN_PLN_NUM_SEGMENTS		(SVN_BASE_PROTOCOL_PLAN + 227)
#define SVT_PLN_NUM_SEGMENTS		VFT_INT32
#define SVS_PLN_NUM_SEGMENTS		"The number of segments"
/*	セグメント数(ＦＦＥ) 。
	*/

#define SVN_PLN_SEGMENTATION_TYPE	(SVN_BASE_PROTOCOL_PLAN + 228)
#define SVT_PLN_SEGMENTATION_TYPE	VFT_INT32
#define SVS_PLN_SEGMENTATION_TYPE	"Type of FFE segmentation"
/*	ＦＦＥセグメンテーションタイプ
	
	VFC_SEGMENT_TYPE_MOSAIC		0	(V4.04以降未使用)
	VFC_SEGMENT_TYPE_INTERLEAVE	1
	VFC_SEGMENT_TYPE_SLICE_CENTRIC	2
	VFC_SEGMENT_TYPE_SEQUENTIAL	3
	VFC_SEGMENT_TYPE_SWIRL		4
	*/

#define SVN_PLN_SEGMENT_TIME		(SVN_BASE_PROTOCOL_PLAN + 229)
#define SVT_PLN_SEGMENT_TIME		VFT_FLT32
#define SVS_PLN_SEGMENT_TIME		"Time for segment (predicted)"
/*	ＦＦＥセグメント単位の収集時間。
	*/

#define SVN_PLN_MTCSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 230)
#define SVT_PLN_MTCSAT_PULSE		VFT_INT32
#define SVS_PLN_MTCSAT_PULSE		"Number of MTC saturation pulses"
/*	0:印加しない	スイッチ Off
	#:パルス数	MTC パルス数
	MTC saturation パルスを幾つか印加することで、MTC効果をさらに
	有効的にコントラストに影響させる機能。
	*/

#define SVN_PLN_MTCSAT_OFFSET_FSY	(SVN_BASE_PROTOCOL_PLAN + 231)
#define SVT_PLN_MTCSAT_OFFSET_FSY	VFT_FLT32
#define SVS_PLN_MTCSAT_OFFSET_FSY	"Frequency offset for MTC saturation pulses"
/*	MTC効果は中心周波数からの周波数上における違いによっても異なる。
	表示としては例えばΔfmt＝-1200Hz等が必要。
	*/

#define SVN_PLN_MTCSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 232)
#define SVT_PLN_MTCSAT_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_MTCSAT_FLIP_ANGLE	"Flip Angle for MTC saturation pulses"
/*	MTC効果はそのFlipAngleによっても異なるため、
	表示としてはFAmt=1000deg等が必要。
	*/

#define SVN_PLN_NUM_SLABS		(SVN_BASE_PROTOCOL_PLAN + 233)
#define SVT_PLN_NUM_SLABS		VFT_INT32
#define SVS_PLN_NUM_SLABS		"The number of slabs"
/*	スラブ数。主として３Ｄマルチスラブで使用する。
	*/


#define SVN_PLN_PE_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 234)
#define SVT_PLN_PE_MPG_GRAD VFT_FLT32
#define SVS_PLN_PE_MPG_GRAD "GRD strength of MPG along PE direction"
/*
	PE方向のMPGパルスの傾斜磁場強度を[Gauss/m]の単位で表したもの
*/

#define SVN_PLN_RO_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 235)
#define SVT_PLN_RO_MPG_GRAD VFT_FLT32
#define SVS_PLN_RO_MPG_GRAD "GRD strength of MPG along RO direction"
/*
	RO方向のMPGパルスの傾斜磁場強度を[Gauss/m]の単位で表したもの
*/

#define SVN_PLN_SS_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 236)
#define SVT_PLN_SS_MPG_GRAD VFT_FLT32
#define SVS_PLN_SS_MPG_GRAD "GRD strength of MPG along SS direction"
/*
	SS方向のMPGパルスの傾斜磁場強度を[Gauss/m]の単位で表したもの
*/

#define SVN_PLN_RX_PHASE_CONTROL (SVN_BASE_PROTOCOL_PLAN + 237)
#define SVT_PLN_RX_PHASE_CONTROL VFT_INT32
#define SVS_PLN_RX_PHASE_CONTROL "Rx Phase control(on/off)"
/*
	受信による位相コントロールをprolimのデフォルト値（シーケンス
	により固定）により指定。これにより奇数アベレージング時に偶数
	ショット数を強制するアルゴリズムを停止する。
*/

#define SVN_PLN_NUM_SPIN_CONDITIONING (SVN_BASE_PROTOCOL_PLAN + 238)
#define SVT_PLN_NUM_SPIN_CONDITIONING VFT_INT32
#define SVS_PLN_NUM_SPIN_CONDITIONING "number of dummy shot before user resume"
/*
	ユーザPAUSEからRESUMEの時の空うち回数。
*/

#define SVN_PLN_GRID_TAG_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 239)
#define SVT_PLN_GRID_TAG_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_GRID_TAG_FLIP_ANGLE	"Flip Angle for Grid-Tag pulses"
/*	Grid-Tag Flip Angle
	*/

#define SVN_PLN_FH_TAG_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 240)
#define SVT_PLN_FH_TAG_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_FH_TAG_FLIP_ANGLE	"Flip Angle for Freehand-Tag pulses"
/*	Freehand-Tag Flip Angle
	*/

#define SVN_PLN_GRID_TAG	(SVN_BASE_PROTOCOL_PLAN + 241)
#define SVT_PLN_GRID_TAG	VFT_BITFIELD
#define SVS_PLN_GRID_TAG	"Grid-Tag"
/*	グリッドタグの指定
	*/

#define SVN_PLN_NUM_GRID_TAG	(SVN_BASE_PROTOCOL_PLAN + 242)
#define SVT_PLN_NUM_GRID_TAG	VFT_BITFIELD
#define SVS_PLN_NUM_GRID_TAG	"number of Grid-Tag"
/*	グリッドタグのRFパルス数
	*/

#define	SVN_PLN_AUTO_HOLD	(SVN_BASE_PROTOCOL_PLAN + 243)
#define	SVT_PLN_AUTO_HOLD	VFT_INT32
#define	SVS_PLN_AUTO_HOLD	"Wait for Start button"
/*	acqman の撮影モードを決定する。
	PQM により自動復帰する以外は PLN_HOLD と同じ。acqman は
	PLN_HOLD,PLN_AUTO_HOLD の OR により撮影モードを制御する。
	
  |--->  Pre Scan --------------------------->|---> Scan  ---------------->
  ________________________________________________________________________
  |  Coil Tune  |   APC   |   CFA   |   RGN     Main Scan  |  SUB ......|
  ↑~~~~~~~~~~~~↑~~~~~~~↑~~~~~~~~↑~~~~~~~~~↑~~~~~~~~~~~~~↑~~~~~~~~~~~
  PRE_SCAN      APC      CFA       RGN        MAIN_SCAN      COVERAGE

定義
	  VFC_HOLD_PRE_SCAN	:  撮影処理の最初で止まる。（通常の撮影処理）
	  VFC_HOLD_MAIN_SCAN	:  プレスキャン終了後、本スキャンの前で一時停止。
	  VFC_HOLD_COVERAGE	:  カバレージごとに止まる。

	  VFC_HOLD_APC	:  ＡＰＣ直前で一時停止。
	  VFC_HOLD_CFA	:  ＣＦＡ直前で一時停止。
	  VFC_HOLD_RGN	:  ＲＧＮ直前で一時停止。
	  VFC_HOLD_CDS	:  ＣＤＳ直前で一時停止。

	注）
	  VFC_HOLD_MAIN_SCAN か VFC_HOLD_COVERAGE のどちらかが指定されて
	いた場合、本スキャンの前で一時停止する。

	*/

#define SVN_PLN_WATERSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 244)
#define SVT_PLN_WATERSAT_PULSE		VFT_INT32
#define SVS_PLN_WATERSAT_PULSE		"Number of WATERSAT pulses"
/*	0:印加しない	スイッチ Off
	#:パルス数	WATERSAT パルス数
	*/

#define SVN_PLN_WATERSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 245)
#define SVT_PLN_WATERSAT_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_WATERSAT_FLIP_ANGLE	"Flip Angle for WATERSAT pulses"
/*	WATERSAT Pulse フリップアングル。
	パルス毎にフリップアングルを制御する？
	*/

#define SVN_PLN_PE_PER_SEGMENT		(SVN_BASE_PROTOCOL_PLAN + 246)
#define SVT_PLN_PE_PER_SEGMENT		VFT_INT32
#define SVS_PLN_PE_PER_SEGMENT		"The number of PE in segment"
/*	セグメント当りのエンコード数
 */

#define SVN_PLN_AFI_PE_ACQ_RATIO	(SVN_BASE_PROTOCOL_PLAN + 247)
#define SVT_PLN_AFI_PE_ACQ_RATIO	VFT_FLT32
#define SVS_PLN_AFI_PE_ACQ_RATIO	"AFI PE Ratio"
/*	AFI Ratio (PE)
 */

#define SVN_PLN_MRS_VOXCEL		(SVN_BASE_PROTOCOL_PLAN + 248)
#define SVT_PLN_MRS_VOXCEL		VFT_FLT32
#define SVS_PLN_MRS_VOXCEL		"MRS Voxcel Size"
/*	VFT_FLT32 の配列。要素数は３で内容は以下のとおり。
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define SVN_PLN_MRS_MATRIX		(SVN_BASE_PROTOCOL_PLAN + 249)
#define SVT_PLN_MRS_MATRIX		VFT_INT32
#define SVS_PLN_MRS_MATRIX		"MRS Matrix"
/*	VFT_INT32 の配列。要素数は３で内容は以下のとおり。
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define	SVN_PLN_BBINV_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 250)
#define	SVT_PLN_BBINV_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_BBINV_FLIP_ANGLE	"Flip angle for BB inversion pulse"
/*	ブラッグブラッドインバージョンパルスフリップ角
	*/

#define SVN_PLN_SPEED_FLAG		(SVN_BASE_PROTOCOL_PLAN + 251)
#define SVT_PLN_SPEED_FLAG		VFT_INT32
#define	SVS_PLN_SPEED_FLAG		"SPEED Flag"
/*	SPEED識別子
 	*/

#define SVN_PLN_MRS_SELECTED		(SVN_BASE_PROTOCOL_PLAN + 252)
#define SVT_PLN_MRS_SELECTED		VFT_FLT32
#define SVS_PLN_MRS_SELECTED		"MRS Selected"
/*	VFT_INT32 の配列。要素数は３で内容は以下のとおり。
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define SVN_PLN_IMAGE_PATTERN	(SVN_BASE_PROTOCOL_PLAN + 253)
#define SVT_PLN_IMAGE_PATTERN	VFT_BITFIELD
#define SVS_PLN_IMAGE_PATTERN	"Contrast Image in simultaneous acquisition"
/*	どのような種類の画像を1度に収集するかを表すパラメータ。 
	Diffusionイメージングの場合、MPGを印加しない場合, MPGをスライス
	方向に印加した場合、MPGをリード方向に印加した場合、MPGをエンコード
	方向に印加した場合について収集するので、この値は4となる。
	defualt値は0とし、0の場合及びフィールドが存在しない場合には、
	従来と互換の動作を行うものとする。
	
	VFC_IVIM_IMAGE_REF    0x0001
	VFC_IVIM_IMAGE_PE     0x0002
	VFC_IVIM_IMAGE_RO     0x0004
	VFC_IVIM_IMAGE_SS     0x0008
	
	(MSB)                         (LSB)
	0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0
	| | | +--  reference
	| | +-----  diffusion PE
	| +--------  diffusion RO
	+-----------  diffusion SS
	
	設定者:	prolim, pqm
	
	This Bit field indicates the kinds of the contrast images
	in one acquirsition. In case of the Diffusion imageing,
	four kind of contrast images are acquired.
	One is imgae with no MPG pulse is applied and the other are 
	images with MPG are applied along the Read, Phase encode and
	slice direction.
	
	*/

#define SVN_PLN_PREC_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 254)
#define SVT_PLN_PREC_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_PREC_FLIP_ANGLE	"Precontrast pulse flip angle"
/*	プレコントラスト(PreC)パルスのフリップ角
 */

#define SVN_PLN_PREC_FLOP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 255)
#define SVT_PLN_PREC_FLOP_ANGLE	VFT_INT32
#define SVS_PLN_PREC_FLOP_ANGLE	"Precontrast pulse flop angle"
/*	プレコントラスト(PreC)パルスのフロップ角
 */


#define SVN_PLN_PREC_PULSE	(SVN_BASE_PROTOCOL_PLAN + 256)
#define SVT_PLN_PREC_PULSE	VFT_INT32
#define SVS_PLN_PREC_PULSE	"Precontrast pulse flag"
/*	プレコントラスト(PreC)パルスのON/OFFを記述するフィールド。
	(将来の拡張性のため、BOOLではなくINT型で定義する
	VFC_PRECONTRAST_PULSE_NONE	0
	VFC_PRECONTRAST_PULSE_DWI	1
	VFC_PRECONTRAST_PULSE_T2	2
	*/

#define SVN_PLN_MRS_BASE_INFO	(SVN_BASE_PROTOCOL_PLAN + 257)
#define SVT_PLN_MRS_BASE_INFO	VFT_INT32
#define SVS_PLN_MRS_BASE_INFO	"MRS Acquisition start point and round direction"
/*	MRS収集の起点および回転方向を定義する。
	VFC_MRS_BASE_LTR	001(2)	1
	VFC_MRS_BASE_LTL	000(2)	0
	VFC_MRS_BASE_RTR	101(2)	5
	VFC_MRS_BASE_RTL	100(2)	4
	VFC_MRS_BASE_LBR	011(2)	3
	VFC_MRS_BASE_LBL	010(2)	2
	VFC_MRS_BASE_RBR	111(2)	7
	VFC_MRS_BASE_RBL	110(2)	6
	最後の３文字のうち最初の２文字が起点を表す。
	L:左 R:右 T:上 B:下
	最後の１文字で起点からどちら方向の順に生データが格納されるかを表す。
	L:左回り方向 R:右回り方向
 */

#define SVN_PLN_MOVING_COUCH_FLG	(SVN_BASE_PROTOCOL_PLAN + 258)
#define SVT_PLN_MOVING_COUCH_FLG	VFT_INT32
#define SVS_PLN_MOVING_COUCH_FLG	"Moving couch flag"
/*   (1)Moving Bedフラグ。
 	Moving Bed撮影を判断するために用いる。必ず存在するフィールド。
 		0) 通常撮影
 		1) Moving Bed撮影(連続方式)
 		2) Moving Bed撮影(手動時間制御方式)
 		3) Moving Bed撮影(自動時間制御方式)
     (2) prolimがセットする。
     (3) 各プログラムはこの値を見てMoving Bedかどうかを判断する。
 */

#define SVN_PLN_COUCH_TABLE	(SVN_BASE_PROTOCOL_PLAN + 259)
#define SVT_PLN_COUCH_TABLE	VFT_INT32
#define SVS_PLN_COUCH_TABLE	"Moving couch flag"
/*  (1) 寝台移動の計画を設定する。VFT_INT32 の配列で
        要素数は移動回数に依存する。
        移動回数はSVN_PLN_REPETITIONS 値とする。
	但し最大１６個迄とする。
	v[0] : 移動回数
	v[1] : １回目の移動距離
	v[2] : ２回目  〃
	...
	v[n] : n回目  〃        （ n = SVN_PLN_REPETITIONS ）
        配列にセットする移動距離は撮影開始時の寝台位置からの移動距離である。
	例えば、撮影開始時の寝台位置を P とするとき各寝台移動時の
        寝台位置は以下のようになる。

	P[0] = P
	P[1] = P + v[1]
	P[2] = P + v[2]
	P[3] = P + v[3]
	...
	このフィールドが存在してもSVN_PLN_MOVING_COUCH_FLGが0ならば通常
 	スキャンとなる。
     (2) prolimがセットする。
     (3) acqmanはこのテーブルを参照して最初のスキャンの後，移動距離をTCM
 	に指示し寝台を移動させてスキャンをする。これを移動回数繰り返す。
 */


#define SVN_PLN_COUCH_MOVE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 260)
#define SVT_PLN_COUCH_MOVE_FLAG	VFT_INT32
#define SVS_PLN_COUCH_MOVE_FLAG	"Couch Move Flag"
/*	磁場中心への寝台移動フラグ
	寝台移動指定がON  : 1
	寝台移動指定がOFF : 0
	注）本フラグはスラブ中心が常に磁場中心を含む平面上（もしくは
	その近傍）に移動させることにより脂肪抑制効果を高める機能を
	実現するものであり, SVN_PLN_MOVING_COUCH_FLG とは異なる点に
	注意すること。
	*/

#define SVN_PLN_NOWRAP_RATIO	(SVN_BASE_PROTOCOL_PLAN + 261)
#define SVT_PLN_NOWRAP_RATIO	VFT_FLTVEC3
#define SVS_PLN_NOWRAP_RATIO	"Nowrap ratio"
/*
ノーラップの割合。
（SPEEDER phase1では、実行時２固定とする）
*/

#define SVN_PLN_SPEEDER_RATIO	(SVN_BASE_PROTOCOL_PLAN + 262)
#define SVT_PLN_SPEEDER_RATIO	VFT_FLTVEC3
#define SVS_PLN_SPEEDER_RATIO	"Speeder ratio"
/*
SPEEDERの高速化率。
（SPEEDER phase1では、実行時２固定とする）
*/

#define SVN_PLN_SPEEDER_FLAG	(SVN_BASE_PROTOCOL_PLAN + 263)
#define SVT_PLN_SPEEDER_FLAG	VFT_BITFIELD
#define SVS_PLN_SPEEDER_FLAG	"Speeder Flag"
/*
SPEEDERシーケンスかどうかのフラグ。
     ＰＥ方向のSPEEDER       VFC_SPEEDER_FLAG_BIT_PE     1
     ＲＯ方向のSPEEDER       VFC_SPEEDER_FLAG_BIT_RO     (1<<1)
     ＳＥ方向のSPEEDER       VFC_SPEEDER_FLAG_BIT_SE     (1<<2)
*/

#define SVN_PLN_SPEEDER_TYPE	(SVN_BASE_PROTOCOL_PLAN + 264)
#define SVT_PLN_SPEEDER_TYPE	VFT_INT32
#define SVS_PLN_SPEEDER_TYPE	"Speeder Type"
/*
SPEEDERのタイプ。
    通常スキャン           VFC_SPEEDER_TYPE_NONE         0
    感度推定用スキャン     VFC_SPEEDER_TYPE_SENSITIVITY  1
    SPEEDER本スキャン     VFC_SPEEDER_TYPE_SPEEDER       2
*/

#define SVN_PLN_MAXSLICE_COVERAGE		(SVN_BASE_PROTOCOL_PLAN + 265)
#define SVT_PLN_MAXSLICE_COVERAGE		VFT_INT32
#define SVS_PLN_MAXSLICE_COVERAGE		"Max Slice / Coverage"
/*	proEditの対話入力値を元に設定し、prolimが参照する。
	*/

#define SVN_PLN_SLICES_IN_PERCENT_RR	(SVN_BASE_PROTOCOL_PLAN + 266)
#define SVT_PLN_SLICES_IN_PERCENT_RR	VFT_INT32
#define SVS_PLN_SLICES_IN_PERCENT_RR	"Number of slices in %R-R"
/*	シーケンシャル(パッキング)モード時に、prolimが%R-R内に収まるスライス数を
	求めて設定し、seqgenが参照する。*/


#define SVN_PLN_CONTROL_IR_FLAG	(SVN_BASE_PROTOCOL_PLAN + 267)
#define SVT_PLN_CONTROL_IR_FLAG	VFT_INT32
#define SVS_PLN_CONTROL_IR_FLAG	"CONTROL IR Flag for ASL"
/*	ASLでのCONTROL IRのOn/Offフラグ。
On:1, Off:0
*/


#define SVN_PLN_ASL_NSS_TEC_NUM	(SVN_BASE_PROTOCOL_PLAN + 268)
#define SVT_PLN_ASL_NSS_TEC_NUM	VFT_INT32
#define SVS_PLN_ASL_NSS_TEC_NUM	"Number of nssIR & TEC pulse for ASL"
/*	ASLでのnssIRとTEC(TagEndCut)のパルス数。
最小は1で、TECがOnの場合は最大で5個まで、TECがOffの場合には最大で4個まで指定可能。

*/


#define SVN_PLN_ASL_NSS_TEC_TI	(SVN_BASE_PROTOCOL_PLAN + 269)
#define SVT_PLN_ASL_NSS_TEC_TI	VFT_FLT32
#define SVS_PLN_ASL_NSS_TEC_TI	"Inversion Time of nssIR & TEC pulse for ASL"
/*	ASLでのnssIRとTEC(TagEndCut)のＴＩ。
要素数5個の配列で、SVN_PLN_ASL_NSS_TEC_NUM個が有効。Imagingに近いパルスの順に格納される。SVN_PLN_ASL_TEC_FLAGがOnの場合、SVN_PLN_ASL_TEC_INDEX番目がTEC用のTIとなる。

*/


#define SVN_PLN_ASL_TEC_FLAG	(SVN_BASE_PROTOCOL_PLAN + 270)
#define SVT_PLN_ASL_TEC_FLAG	VFT_INT32
#define SVS_PLN_ASL_TEC_FLAG	"TEC(TagEndCut) On/Off Flag for ASL"
/*	TEC(TagEndCut)のOn/Offフラグ。
On:1, Off:0

*/


#define SVN_PLN_ASL_TEC_INDEX	(SVN_BASE_PROTOCOL_PLAN + 271)
#define SVT_PLN_ASL_TEC_INDEX	VFT_INT32
#define SVS_PLN_ASL_TEC_INDEX	"TEC(TagEndCut) Index Number for ASL"
/*	TEC(TagEndCut)の順番。
SVN_PLN_ASL_TEC_FLAGがOnの場合に、SVN_PLN_ASL_NSS_TEC_NUMで指定されたパルスのうち、Imagingに近いほうからTECが何番目かを示すインデックス。

*/




#define SVN_PLN_NSSIR_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 272)
#define SVT_PLN_NSSIR_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_NSSIR_FLIP_ANGLE	"Flip angle for nssIR pulse"
/*	nssIRパルスのフリップ角

*/


#define SVN_PLN_SKIP_PREPULSE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 273)
#define SVT_PLN_SKIP_PREPULSE_FLAG	VFT_INT32
#define SVS_PLN_SKIP_PREPULSE_FLAG	"Skip PrePulse Flag for FFE3D Skip"
/*	FFE3D SkipシーケンスでのプリパルスOn/Offフラグ。
On:1, Off:0。
*/


#define SVN_PLN_SKIP_PREPULSE_CONTRAST	(SVN_BASE_PROTOCOL_PLAN + 274)
#define SVT_PLN_SKIP_PREPULSE_CONTRAST	VFT_INT32
#define SVS_PLN_SKIP_PREPULSE_CONTRAST	"Skip PrePulse Contrast for FFE3D Skip"
/*	FFE3D Skipでのプリパルスの効き具合。appcodeとして３段階ある。NORMALはなし。
   VFC_SKIP_PREPULSE_CONTRAST_NORMAL   0
   VFC_SKIP_PREPULSE_CONTRAST_HEAVY    1
   VFC_SKIP_PREPULSE_CONTRAST_MIDDLE   2
   VFC_SKIP_PREPULSE_CONTRAST_LIGHT    3

*/


#define SVN_PLN_SKIP_SEGMENT_SIZE	(SVN_BASE_PROTOCOL_PLAN + 275)
#define SVT_PLN_SKIP_SEGMENT_SIZE	VFT_INT32
#define SVS_PLN_SKIP_SEGMENT_SIZE	"Each Segment Size for FFE3D Skip"
/*	FFE3D Skipでの各セグメント(Area)のShot数。要素数6個の配列。

[0]: Area0 : A shot ... 高周波成分のエンコードからのScan開始
[1]: Area1 : B shot ... Echo Stabilize + PrePulse with sampling
[2]: Area2 : C% * PhaseEncode * Slice Encode shot
[3]: Area3 : D% * PhaseEncode * Slice Encode shot
[4]: Area4 : E% * PhaseEncode * Slice Encode shot
[5]: 残り  : (TotalShot &#8211; Area0 &#8211; Area1 &#8211; Area2 &#8211; Area3 &#8211; Area4) shot 

シーケンステーブル(prolim def)上ではArea2,3,4は百分率(%)で与えられるが、prolimがShot数に変換してセットする。


*/


#define SVN_PLN_SKIP_NUM_SKIP	(SVN_BASE_PROTOCOL_PLAN + 276)
#define SVT_PLN_SKIP_NUM_SKIP	VFT_INT32
#define SVS_PLN_SKIP_NUM_SKIP	"Number of FlipPulse per PrePulse for FFE3D Skip"
/*	FFE3D Skipでの各セグメント(Area)でのSkip数。要素数6個の配列。
Skip数はプリパルス１回についてフリップパルスが何回あるかの数。

[0]: Area0 のSkip数。
[1]: Area1 のSkip数。
[2]: Area2 のSkip数。
[3]: Area3 のSkip数。
[4]: Area4 のSkip数。
[5]: Area5 のSkip数。

*/


#define SVN_PLN_AUTO_PLAN_FLG	(SVN_BASE_PROTOCOL_PLAN + 277)
#define SVT_PLN_AUTO_PLAN_FLG	VFT_BOOL
#define SVS_PLN_AUTO_PLAN_FLG	"Auto Plan Flag "
/*	簡易撮影で自動的にPLANを行なうかを判断させるフラグ。
*/


#define SVN_PLN_AUTO_POSTPROC_FLG	(SVN_BASE_PROTOCOL_PLAN + 278)
#define SVT_PLN_AUTO_POSTPROC_FLG	VFT_BOOL
#define SVS_PLN_AUTO_POSTPROC_FLG	"Auto Postproc Flag "
/*	撮影連動処理を行なうかを判断させるフラグ。
*/


#define SVN_PLN_AUTO_FILM_FLG	(SVN_BASE_PROTOCOL_PLAN + 279)
#define SVT_PLN_AUTO_FILM_FLG	VFT_BOOL
#define SVS_PLN_AUTO_FILM_FLG	"Auto Film Flag "
/*	簡易撮影でAUTOFILM対象のプロトコルかを判断させるフラグ。
*/


#define SVN_PLN_FILM_PRESET	(SVN_BASE_PROTOCOL_PLAN + 280)
#define SVT_PLN_FILM_PRESET	VFT_STRING
#define SVS_PLN_FILM_PRESET	"Film Preset Filename "
/*	Filmで使用するプリセットファイル名を定義する。
*/

#define SVN_PLN_EASYSCAN_FLG	(SVN_BASE_PROTOCOL_PLAN + 281)
#define SVT_PLN_EASYSCAN_FLG	VFT_BOOL
#define SVS_PLN_EASYSCAN_FLG	"Easy Scan Flag"
/*	簡易撮影フラグ
*/


#define SVN_PLN_SAMPLE_IMAGE	(SVN_BASE_PROTOCOL_PLAN + 282)
#define SVT_PLN_SAMPLE_IMAGE	VFT_INT8
#define SVS_PLN_SAMPLE_IMAGE	"Easy Scan Sample Image data"
/*	簡易撮影で撮影されるサンプル画像
*/

#define SVN_PLN_DEFAULT_CONTRAST        (SVN_BASE_PROTOCOL_PLAN + 283)
#define SVT_PLN_DEFAULT_CONTRAST        VFT_DEF_CONTRAST
#define SVS_PLN_DEFAULT_CONTRAST        "Image default contrast"
/*      簡易撮影用PASに登録されて、
簡易撮影で使われる画像輝度のデフォルトテーブル
データタイプ :SVN_PLN_DEFAULT_CONTRAST
typedef struct VftDefContrast_s {
            int32_t processed_node ;     processed node
            int32_t echo_node ;          echo node
            int32_t position_node ;      position node
            flt32_t wl ;                 Window Level(%)
            flt32_t ww ;                 Window Width(%)
            int32_t pos_neg ;            1:POSITIVE   -1:NEGATIVE
} VftDefContrast_t

*/

#define SVN_PLN_SAMPLE_IMAGE_TYPE       (SVN_BASE_PROTOCOL_PLAN + 284)
#define SVT_PLN_SAMPLE_IMAGE_TYPE       VFT_PBM
#define SVS_PLN_SAMPLE_IMAGE_TYPE       "Sample Image Type"
/*      簡易撮影で使われるサンプル画像(SVN_PLN_SAMPLE_IMAGE)のデータ情報
typedef struct VftPbm_s {
            char     type[2];     date type
            int32_t width ;       横幅
            int32_t height ;      高さ
            int32_t max ;         最大値
} VftPbm_t
*/

#define SVN_PLN_PREP_IMAGE_TYPE (SVN_BASE_PROTOCOL_PLAN + 285)
#define SVT_PLN_PREP_IMAGE_TYPE VFT_INT32
#define SVS_PLN_PREP_IMAGE_TYPE "Prep Scan Image type"
/*      ビジュアルプレップの画像タイプ


Appcode
#define VFC_PREP_IMAGE_TYPE_IMAGE       0
#define VFC_PREP_IMAGE_TYPR_MIP 1
*/

#define SVN_PLN_ACQ_SPLIT_TABLE (SVN_BASE_PROTOCOL_PLAN + 286)
#define SVT_PLN_ACQ_SPLIT_TABLE VFT_INT32
#define SVS_PLN_ACQ_SPLIT_TABLE "acquisition Split table"
/*      撮影分割テーブル
Val[0]:分割回数
Val[1]:1回目の撮影回数
Val[2]:2回目の撮影回数
  :      :
*/

#define SVN_PLN_ACQ_SPLIT_MODE  (SVN_BASE_PROTOCOL_PLAN + 287)
#define SVT_PLN_ACQ_SPLIT_MODE  VFT_INT32
#define SVS_PLN_ACQ_SPLIT_MODE  "acquisition Split mode"
/*      撮影分割モード


appcode
#define VFC_ACQ_SPLIT_MODE_AUTOPAUSE    1
#define VFC_ACQ_SPLIT_MODE_PREP 2
*/

#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION      (SVN_BASE_PROTOCOL_PLAN + 288)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION      VFT_BITFIELD
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION      "Auto MovingBed Subtraction Flag"
/*      MovingBed差分の有無
TRUE   MovingBed差分あり
FALSE  MovingBed差分なし
*/
#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       (SVN_BASE_PROTOCOL_PLAN + 289)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       VFT_BITFIELD
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       "Auto MovingBed Subtraction Images"
/*      MovingBed差分画像種


Appcode
 #define VFC_SUBTRACTION_NONE     (1L<<0)    差分なし画像作成
 #define VFC_SUBTRACTION_ABSOLUTE (1L<<1)    絶対値差分画像作成
 #define VFC_SUBTRACTION_COMPLEX  (1L<<2)    複素差分画像作成
*/

#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE (SVN_BASE_PROTOCOL_PLAN + 290)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE VFT_INT32
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE "Auto MovingBed Subtraction Base Phase"
/*      MovingBed差分処理基準セグメント
*/

#define SVN_PLN_COUCH_SEG_TABLE (SVN_BASE_PROTOCOL_PLAN + 291)
#define SVT_PLN_COUCH_SEG_TABLE VFT_INT32
#define SVS_PLN_COUCH_SEG_TABLE "MovingBed Segment table"
/*      MovingBedセグメントテーブル
Val[0]:セグメント数
Val[2]:1セグメントでの撮影回数
 :            :
*/

#define SVN_PLN_GATE_CTRL_PARAM (SVN_BASE_PROTOCOL_PLAN + 292)
#define SVT_PLN_GATE_CTRL_PARAM VFT_TIME_CTRL_PARAM
#define SVS_PLN_GATE_CTRL_PARAM "Gate control parameters"
/*      Gate撮影を行なう時のパラメータ
*/

#define SVN_PLN_ACQ_SPLIT_INDEX (SVN_BASE_PROTOCOL_PLAN + 293)
#define SVT_PLN_ACQ_SPLIT_INDEX VFT_INT32
#define SVS_PLN_ACQ_SPLIT_INDEX "Acquisition Split Index"
/*      分割撮影テーブル(SVN_PLN_ACQ_SPLIT_TABLE)内のカレント収集インデックス番号


*/

#define SVN_PLN_DATA_SAMPL_BAND (SVN_BASE_PROTOCOL_PLAN + 294)
#define SVT_PLN_DATA_SAMPL_BAND VFT_STRING
#define SVS_PLN_DATA_SAMPL_BAND "Sampling Band Width Name"
/*      (1)サンプリングバンド幅名
(2)バンド幅選択で表示されているラベルをprolim(proEdit)が設定し、表示系が利用する
*/

#define SVN_PLN_DATA_SAMPL_PITCH        (SVN_BASE_PROTOCOL_PLAN + 295)
#define SVT_PLN_DATA_SAMPL_PITCH        VFT_FLT32
#define SVS_PLN_DATA_SAMPL_PITCH        "Sampling Pitch"
/*      (1)サンプリングピッチ
(2)prolim(proEdit)がPROTOCOLノードに設定する。
*/

#define SVN_PLN_SSFP_PRE_FLIP_ANGLE     (SVN_BASE_PROTOCOL_PLAN + 296)
#define SVT_PLN_SSFP_PRE_FLIP_ANGLE     VFT_INT32
#define SVS_PLN_SSFP_PRE_FLIP_ANGLE     "Flip angle for SSFP preFlip pulse"
/*      (1)SSFP用preFlipパルスのフリップ角
(2)prolimがセットし、libsarが参照する。
*/

#define SVN_PLN_SSFP_POST_FLIP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 297)
#define SVT_PLN_SSFP_POST_FLIP_ANGLE    VFT_INT32
#define SVS_PLN_SSFP_POST_FLIP_ANGLE    "Flip angle for SSFP postFlip pulse"
/*      (1)SSFP用postFlipパルスのフリップ角
(2)prolimがセットし、libsarが参照する。
*/

#define SVN_PLN_SSFP_POST_FLIP_NUM      (SVN_BASE_PROTOCOL_PLAN + 298)
#define SVT_PLN_SSFP_POST_FLIP_NUM      VFT_INT32
#define SVS_PLN_SSFP_POST_FLIP_NUM      "Number of SSFP postFlip"
/*      (1)SSFP用postFlipパルスの印加回数
(2)prolimがセットし、libsarが参照する。
*/

#define SVN_PLN_NUM_DUMMY_SEGMENT       (SVN_BASE_PROTOCOL_PLAN + 299)
#define SVT_PLN_NUM_DUMMY_SEGMENT       VFT_INT32
#define SVS_PLN_NUM_DUMMY_SEGMENT       "The number of dummy segment"
/*      (1)空打セグメント数
*/

#define SVN_PLN_DYNAMIC_MIN_PERIOD      (SVN_BASE_PROTOCOL_PLAN + 300)
#define SVT_PLN_DYNAMIC_MIN_PERIOD      VFT_INT32
#define SVS_PLN_DYNAMIC_MIN_PERIOD      "Minimum period of Dynamic scan"
/*      (1)Dynamic scan時のminimum period
	(2)prolimがセットしacqmanが参照する。
*/

#define SVN_PLN_PROEDIT_MODE            (SVN_BASE_PROTOCOL_PLAN + 301)
#define SVT_PLN_PROEDIT_MODE            VFT_INT32
#define SVS_PLN_PROEDIT_MODE            "proEdit page mode"
/*      (1)proEditのページを切り替えるフラグ。
	(2)動的に切り替えられて、プロトコルごとに違うモードになるので、
	   フィールドとして持つことにする。

Appcode
#define VFC_PROEDIT_MODE_NORMAL 0
#define VFC_PROEDIT_MODE_USER1  1
#define VFC_PROEDIT_MODE_USER2  2
*/

#define SVN_PLN_PROBE_CORRECTION        (SVN_BASE_PROTOCOL_PLAN + 302)
#define SVT_PLN_PROBE_CORRECTION        VFT_INT32
#define SVS_PLN_PROBE_CORRECTION        "Real Time Motion Correction"
/*      (1)proEditで設定する。
*/

#define SVN_PLN_PROBE_FLAG            (SVN_BASE_PROTOCOL_PLAN + 303)
#define SVT_PLN_PROBE_FLAG            VFT_INT32
#define SVS_PLN_PROBE_FLAG            "Enable Prove Edit flag"
/*      (1)SVN_PROBE_CORRECTION がONのとき、prolimが１をセットする。
	(2)Probeシーケンスは２がセットされている。
           撮像開始時にpqmが１をセットしなおす。
	(3)値が入っていれば、proEditからプローブ編集が選択できるフラグ。
	0:	NONE
	1:	RTMC (Real Time Motion Correction)
	2:	RTMP (Real Time Motion Probing)
*/

#define SVN_PLN_FATSAT_PULSE_TYPE	(SVN_BASE_PROTOCOL_PLAN + 304)
#define SVT_PLN_FATSAT_PULSE_TYPE	VFT_INT32
#define SVS_PLN_FATSAT_PULSE_TYPE	"Fatsat Pulse Type"
/*	ファットサットパルスの型を記述するフィールド。
	0:	STANDARD
	1:	ROBUST
	*/

#define SVN_PLN_FLUORO_MODE            (SVN_BASE_PROTOCOL_PLAN + 305)
#define SVT_PLN_FLUORO_MODE            VFT_INT32
#define SVS_PLN_FLUORO_MODE            "FLUORO mode"
/*      FLUOROの動作モード。
Appcode
#define VFC_FLUORO_NONE     0    
#define VFC_FLUORO_DIRECT   1         
#define VFC_FLUORO_SEQ_RM   2
*/

#define SVN_PLN_NUM_RETRO_PHASE		(SVN_BASE_PROTOCOL_PLAN + 306)
#define SVT_PLN_NUM_RETRO_PHASE		VFT_INT32
#define SVS_PLN_NUM_RETRO_PHASE		"Number of phase for recon of retro"
/*	レトロ再構成フェーズ数。
	*/

#define SVN_PLN_RETRO_MODE            (SVN_BASE_PROTOCOL_PLAN + 307)
#define SVT_PLN_RETRO_MODE            VFT_INT32
#define SVS_PLN_RETRO_MODE            "Retro mode"
/*      RETROの動作モード。
Appcode
#define VFC_RETRO_MODE_NONE     0    
#define VFC_RETRO_MODE_1        1         
#define VFC_RETRO_MODE_2        2
*/

#define SVN_PLN_RETRO_CTRL_PARAM (SVN_BASE_PROTOCOL_PLAN + 308)
#define SVT_PLN_RETRO_CTRL_PARAM VFT_TIME_CTRL_PARAM
#define SVS_PLN_RETRO_CTRL_PARAM "Retro control parameters"
/*      レトロ撮影を行なう時のパラメータ
*/

#define SVN_PLN_SPEEDER_RATIO_INPUT	(SVN_BASE_PROTOCOL_PLAN + 309)
#define SVT_PLN_SPEEDER_RATIO_INPUT	VFT_FLTVEC3
#define SVS_PLN_SPEEDER_RATIO_INPUT	"Input Speeder ratio"
/*
SPEEDERの高速化率の入力値。
マトリクスが設定可能になるように再計算して、実高速化率を求め、
SVN_PLN_SPEEDER_RATIOにセットする。
*/

#define SVN_PLN_AUTO_PROEDIT_FLAG	(SVN_BASE_PROTOCOL_PLAN + 310)
#define SVT_PLN_AUTO_PROEDIT_FLAG	VFT_INT32
#define SVS_PLN_AUTO_PROEDIT_FLAG	"Auto ProEdit Flag"
/*
前の撮影が終わった時に自動にProEditの起動有無をセットする
先頭プロトコルの場合は起動されない。
本来の目的は3D LocatorでPLAN設定を行った後に ProEditを必ず行う必要があり
その為に3D LocatorがセットしてPQMが使う。
#define VFC_AUTO_PROEDIT_NONE     0    
#define VFC_AUTO_PROEDIT_3DLOCATOR        1
*/

#define SVN_PLN_LOCATOR_ROI_STYLE	(SVN_BASE_PROTOCOL_PLAN + 311)
#define SVT_PLN_LOCATOR_ROI_STYLE	VFT_INT32
#define SVS_PLN_LOCATOR_ROI_STYLE	"ROI style for inset"
/*      
位置決め時に設定されたＲＯＩスタイル
参照表示時に位置決め時と同じスタイルでＲＯＩを表示させる要望対応
sc-locatorがセットして
dsc_inset,dsc_refer,dsc_dRefer,libInsetなどが参照する。
*/

#define SVN_PLN_PROBE_FLIP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 312)
#define SVT_PLN_PROBE_FLIP_ANGLE    VFT_INT32
#define SVS_PLN_PROBE_FLIP_ANGLE    "Flip angle for probe flip pulse"
/*      (1)Probe用Flipパルスのフリップ角
(2)prolimがセットし、libsarが参照する。
*/

#define SVN_PLN_PROBE_FLOP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 313)
#define SVT_PLN_PROBE_FLOP_ANGLE    VFT_INT32
#define SVS_PLN_PROBE_FLOP_ANGLE    "Flop angle for probe flop pulse"
/*      (1)Probe用Flopパルスのフリップ角
(2)prolimがセットし、libsarが参照する。
*/

#define SVN_PLN_ENCODE_ORDER	(SVN_BASE_PROTOCOL_PLAN + 314)
#define SVT_PLN_ENCODE_ORDER	VFT_INT32
#define SVS_PLN_ENCODE_ORDER	"Phase Encode Order Type"
/*	位相エンコード順序。
	0:	SEQUENTIAL
	1:	CENTRIC
	*/

#define SVN_PLN_EPI_SORT_TYPE	(SVN_BASE_PROTOCOL_PLAN + 315)
#define SVT_PLN_EPI_SORT_TYPE	VFT_INT32
#define SVS_PLN_EPI_SORT_TYPE	"FFE_EPI Sort Type"
/*	FFE_EPIにおけるエンコード順序。
	0:	NONE
	1:	T1
	2:      T2
	*/

#define SVN_PLN_CAN_OBLIQUE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 316)
#define SVT_PLN_CAN_OBLIQUE_FLAG	VFT_BOOL
#define SVS_PLN_CAN_OBLIQUE_FLAG	"Can Oblique Flag"
/*	オブリーク可能フラグ
*/

#define SVN_PLN_POSTPROC_RESERVED	(SVN_BASE_PROTOCOL_PLAN + 317)
#define SVT_PLN_POSTPROC_RESERVED	VFT_INT32
#define SVS_PLN_POSTPROC_RESERVED	"Post Proc. Flag"
/*	連続後処理に必要な一部のフィールドを以前はseqgenが書込んでいました。
	再構成はそのフィールドを参照して後処理を起動していました。
	ところが、V5.0の機能追加によって、連続後処理を起動するかどうかの
	情報がseqgenの起動前に必要になりました。
	そこで、協議の結果、studyFileとprolimの変数としてに下記の変数を追加
	してもらい、シーケンスがprolimの変数を事前にセットすることで再構成
	に情報を伝えたいと思います。
*/

#define SVN_PLN_MOVINGBED_NUM_SEGMENT (SVN_BASE_PROTOCOL_PLAN + 318)
#define SVT_PLN_MOVINGBED_NUM_SEGMENT VFT_INT32
#define SVS_PLN_MOVINGBED_NUM_SEGMENT "Number of segments for MovingBed"
/*      MovingBedセグメント数
*/

#define SVN_PLN_ASL_TEC_TI	(SVN_BASE_PROTOCOL_PLAN + 319)
#define SVT_PLN_ASL_TEC_TI	VFT_FLT32
#define SVS_PLN_ASL_TEC_TI	"Inversion Time of TEC pulse for ASL"
/*	ASLでのTEC(TagEndCut)のＴＩ。
	TIからSVN_PLN_ASL_NSS_TEC_TIのTECに対応する時間を引いた値。
	prolimでセットしviewが付帯情報に表示する。
*/

#define SVN_PLN_PROBE_MOTION_RATIO  (SVN_BASE_PROTOCOL_PLAN + 320)
#define SVT_PLN_PROBE_MOTION_RATIO   VFT_FLTVEC3
#define SVS_PLN_PROBE_MOTION_RATIO   "Motion Ratio for Probing scan"
/*      リアルタイム動き補償における、横隔膜の変位に対する心臓の
	変位。
*/

#define SVN_PLN_FFEEPI_TE	(SVN_BASE_PROTOCOL_PLAN + 321)
#define SVT_PLN_FFEEPI_TE	VFT_FLT32
#define SVS_PLN_FFEEPI_TE	"FFE_EPI TE"
/*	FFE_EPIのTE
*/

#define SVN_PLN_NUM_FSE_TE_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 322)
#define SVT_PLN_NUM_FSE_TE_ECHOES	VFT_INT32
#define SVS_PLN_NUM_FSE_TE_ECHOES	"Number of contrast ETL for FSE"
/*	FSEのTEとechoFactorの候補テーブル
*/

#define SVN_PLN_3D_EXTRA_SLICES_APPEND	(SVN_BASE_PROTOCOL_PLAN + 323)
#define SVT_PLN_3D_EXTRA_SLICES_APPEND	VFT_INT32
#define SVS_PLN_3D_EXTRA_SLICES_APPEND	"Extra slices for 3D"
/*	SVN_PLN_3D_EXTRA_SLICESに対し、SENoWrapRatioを考慮しない
	枚数をセットする。再構成で参照する。
	*/

#define SVN_PLN_ALTERNATED_ACQ_FLAG	(SVN_BASE_PROTOCOL_PLAN + 324)
#define SVT_PLN_ALTERNATED_ACQ_FLAG	VFT_INT32
#define SVS_PLN_ALTERNATED_ACQ_FLAG	"Alternated flag"
/*	1shot（1segment）目選択IRをON、2shot（2segment）目選択IRをOFFとして
        交互に撮影するフラグ。 撮影時間は倍になる。
　　　　studyFileには再構成によりエコーノード(1.X/2.0/7.0/8.{0,1}/9.x)を
　　　　追加して2組の画像を格納する。
*/

#define SVN_PLN_NON_SELECTIVE_IR_PULSE	(SVN_BASE_PROTOCOL_PLAN + 325)
#define SVT_PLN_NON_SELECTIVE_IR_PULSE	VFT_INT32
#define SVS_PLN_NON_SELECTIVE_IR_PULSE	"Flag for Non-selective IR Pulse"
/*	ブラックブラッドインバージョンパルスの非選択IRパルスのON/OFF
        を記述するフィールド。デフォルト(値が０の場合)がONであることに注意。
*/

#define SVN_PLN_TSLIT_THICKNESS	(SVN_BASE_PROTOCOL_PLAN + 326)
#define SVT_PLN_TSLIT_THICKNESS	VFT_FLT32
#define SVS_PLN_TSLIT_THICKNESS	"tSLIT Thickness"	
/*	tSLITの厚さ
 */

#define SVN_PLN_RO_DEPHASE_AREA_RATIO	(SVN_BASE_PROTOCOL_PLAN + 327)
#define SVT_PLN_RO_DEPHASE_AREA_RATIO	VFT_FLT32
#define SVS_PLN_RO_DEPHASE_AREA_RATIO	"Read dephase area ratio"	
/*	Flow-Spoied FBIの本スキャンにおいて、オペレータにより入力された
	RO方向に印加するDephasing Pulseの強度。パルス強度は、読出し用の
	傾斜磁場の立上りからエコー中心までの面積を１とした時の、Dephase
	パルスの面積比で表す。
 */

#define SVN_PLN_FLOW_PREP_PARAM	(SVN_BASE_PROTOCOL_PLAN + 328)
#define SVT_PLN_FLOW_PREP_PARAM	VFT_FLT32
#define SVS_PLN_FLOW_PREP_PARAM	"Flow prep parameter"	
/*	FlowPrepスキャンにおいて、シーケンスにより設定されたROあるいはPE
	に印加するDephasing Pulseの初期強度値、強度の刻み値、回数。要素数３
	[0]：初期強度
	[1]：刻み
	[2]：回数
 */

#define SVN_PLN_ALTERNATED_DELAY_TIME	(SVN_BASE_PROTOCOL_PLAN + 329)
#define SVT_PLN_ALTERNATED_DELAY_TIME	VFT_INT32
#define SVS_PLN_ALTERNATED_DELAY_TIME	"Alternated delay time"
/*      Flow-Spoiled FBIスキャンにおいて、Alternatedスキャンが指定された
  　　　場合の２回目のスキャンのディレイ時間。単位はmsec。
*/

#define SVN_PLN_LOCAL_CF_FLAG	(SVN_BASE_PROTOCOL_PLAN + 330)
#define SVT_PLN_LOCAL_CF_FLAG	VFT_INT32
#define SVS_PLN_LOCAL_CF_FLAG	"Local CF flag"
/*     局所CF NONE/ONを記述する
*/

#define	SVN_PLN_FLOW_COMP	(SVN_BASE_PROTOCOL_PLAN + 331)
#define	SVT_PLN_FLOW_COMP	VFT_INT32
#define	SVS_PLN_FLOW_COMP	"Flow comp direction"
/*	Flow comp 方向

*/

#define	SVN_PLN_WATEREXCIT_PULSE	(SVN_BASE_PROTOCOL_PLAN + 332)
#define	SVT_PLN_WATEREXCIT_PULSE	VFT_INT32
#define	SVS_PLN_WATEREXCIT_PULSE	"Water Excitation Pulse Flag"
/*	Water Excitation Pulse Flag ... On/Off

*/

#define	SVN_PLN_WATEREXCIT_TYPE	(SVN_BASE_PROTOCOL_PLAN + 333)
#define	SVT_PLN_WATEREXCIT_TYPE	VFT_INT32
#define	SVS_PLN_WATEREXCIT_TYPE	"Water Excitation Pulse Type"
/*	Water Excitation Pulse Type
        0 : 1-1
        1 : 1-2-1
        2 : 1-3-3-1

*/

#define	SVN_PLN_WATEREXCIT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 334)
#define	SVT_PLN_WATEREXCIT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_WATEREXCIT_FLIP_ANGLE	"Water Excitation Pulse Max Flip Angle"
/*	Water Excitation Pulse Max Flip Angle

*/

#define	SVN_PLN_FLEX_TE	(SVN_BASE_PROTOCOL_PLAN + 335)
#define	SVT_PLN_FLEX_TE	VFT_FLT32
#define	SVS_PLN_FLEX_TE	"Flexible TE"
/*	Flexible TE
        setting     PROLIM
        reference   PROLIM
        
*/

#define SVN_PLN_GATE_TRIG_TYPE	(SVN_BASE_PROTOCOL_PLAN + 336)
#define SVT_PLN_GATE_TRIG_TYPE	VFT_INT32
#define SVS_PLN_GATE_TRIG_TYPE	"Gate Trigger Type"
/*	Gate Trigger Type
        setting     PROLIM
        reference   PROLIM

		VFC_GATE_TRIG_TYPE_NORMAL	0
		VFC_GATE_TRIG_TYPE_ACROSS	1
        
*/

#define SVN_PLN_PREC_FLIP_PULSE_NUM		(SVN_BASE_PROTOCOL_PLAN + 337)
#define SVT_PLN_PREC_FLIP_PULSE_NUM		VFT_INT32
#define SVS_PLN_PREC_FLIP_PULSE_NUM		"Number of PreC Flip Pulse"
/*	PreC Flip パルス数
        setting     PROLIM
        reference   PROLIM

*/

#define SVN_PLN_PREC_FLOP_PULSE_NUM		(SVN_BASE_PROTOCOL_PLAN + 338)
#define SVT_PLN_PREC_FLOP_PULSE_NUM		VFT_INT32
#define SVS_PLN_PREC_FLOP_PULSE_NUM		"Number of PreC Flop Pulse"
/*	PreC Flop パルス数
        setting     PROLIM
        reference   PROLIM

*/

#define	SVN_PLN_FLIP_ANGLE_RATIO	(SVN_BASE_PROTOCOL_PLAN + 339)
#define	SVT_PLN_FLIP_ANGLE_RATIO	VFT_FLT32
#define	SVS_PLN_FLIP_ANGLE_RATIO	"Imaging Flip Angle Ratio"
/*	Imaging Flip Angle Ratio for Water Excitation Pulse
        setting     PROLIM
        reference   PROLIM

*/

#define	SVN_PLN_WATEREXCIT_PULSE_TABLE	(SVN_BASE_PROTOCOL_PLAN + 340)
#define	SVT_PLN_WATEREXCIT_PULSE_TABLE	VFT_INT32
#define	SVS_PLN_WATEREXCIT_PULSE_TABLE	"Table of Flip Pulse for Water Excitation"
/*	Table of Flip Pulse for Water Excitation
        setting     PROLIM
        reference   sequence
    (Ex.)
        1-1    :[0]1 [1]1
        1-2-1  :[0]1 [1]2 [2]1
        1-3-3-1:[0]1 [1]3 [2]3 [3]1

*/

#define	SVN_PLN_WATEREXCIT_PULSE_NUM	(SVN_BASE_PROTOCOL_PLAN + 341)
#define	SVT_PLN_WATEREXCIT_PULSE_NUM	VFT_INT32
#define	SVS_PLN_WATEREXCIT_PULSE_NUM	"Number of Flip Pulse for Water Excitation"
/*	Number of Flip Pulse for Water Excitation
        setting     PROLIM
        reference   sequence
    (Ex.)
        2 : 1-1
        3 : 1-2-1
        4 : 1-3-3-1

*/

#define	SVN_PLN_FLEX_TE2	(SVN_BASE_PROTOCOL_PLAN + 342)
#define	SVT_PLN_FLEX_TE2	VFT_FLT32
#define	SVS_PLN_FLEX_TE2	"Flexible TE2"
/*	Flexible TE2
        setting     PROLIM
        reference   PROLIM
        
*/

#define	SVN_PLN_AMB_ENABLE_FLG	(SVN_BASE_PROTOCOL_PLAN + 343)
#define	SVT_PLN_AMB_ENABLE_FLG	VFT_INT32
#define	SVS_PLN_AMB_ENABLE_FLG	"Advanced MovingBed Enable flag"
/*	Advanced MovingBed 撮像が可能なシーケンスであるかを示す。
　　ProMgrが生成する。
	TRUE    Advanced MovingBed 撮像が可能シーケンス
	FALSE   Advanced MovingBed 撮像ができないシーケンス
*/

#define	SVN_PLN_AMB_MODE_FLG	(SVN_BASE_PROTOCOL_PLAN + 344)
#define	SVT_PLN_AMB_MODE_FLG	VFT_INT32
#define	SVS_PLN_AMB_MODE_FLG	"Advanced MovingBed Mode flag"
/*	Advanced MovingBed protocol Mode 
	Slave ProtocolはAMB撮像時の自動生成されたプロトコルを意味する。
	このprotocolはシーケンスコピー及びPAS登録できない。
	PQMが設定する。　PAS更新対象

	#define	VFC_AMB_MODE_NONE	0	 通常撮像
	#define	VFC_AMB_MODE_MASTER	1	 AMB Mater protocol
	#define	VFC_AMB_MODE_SLAVE	2	 AMB SLAVE protocol    
*/

#define	SVN_PLN_AMB_COUCH_TABLE	(SVN_BASE_PROTOCOL_PLAN + 345)
#define	SVT_PLN_AMB_COUCH_TABLE	VFT_INT32
#define	SVS_PLN_AMB_COUCH_TABLE	"Advanced MovingBed Couch table"
/*	Advanced MovingBed 寝台移動テーブル
　　Acqman制御マルチプロトコル収集にて使用する。
    VFT_INT32 の配列で要素数は撮像プロトコル数に依存する。
	但し最大６４個迄とする。　SVN_PLN_AMB_PROTOCOL_TABLEと同じ個数存在する。
	v[0] : 撮像プロトコル数
	v[1] : １回目の移動距離　(事前に移動済なのでゼロになる)
	v[2] : ２回目  〃
	...
	v[n] : n回目  〃  
        撮像開始時の寝台位置からの相対位置（移動距離）である。
     (1) PQMがセットする。
     (2) acqmanはSVN_PLN_AMB_MODE_FLGがVFC_AMB_SLAVEのときだけ参照する。
 */

#define	SVN_PLN_AMB_PROTOCOL_TABLE	(SVN_BASE_PROTOCOL_PLAN + 346)
#define	SVT_PLN_AMB_PROTOCOL_TABLE	VFT_INT32
#define	SVS_PLN_AMB_PROTOCOL_TABLE	"Advanced MovingBed Protocol table"
/*	Advanced MovingBed 撮像プロトコルテーブル
	Acqman制御マルチプロトコル収集にて使用する。
    VFT_INT32 の配列で要素数は撮像プロトコル数に依存する。
	但し最大６４個迄とする。SVN_PLN_AMB_COUCH_TABLEと同じ個数存在する。
	v[0] : 撮像プロトコル数
	v[1] : １回目の撮像プロトコル番号
	v[2] : ２回目  〃
	...
	v[n] : n回目  〃  
     (1) PQMがセットする。
     (2) acqmanはSVN_PLN_AMB_MODE_FLGがVFC_AMB_SLAVEのときだけ参照する。
 */

#define	SVN_PLN_AMB_ACTUAL_COUCH	(SVN_BASE_PROTOCOL_PLAN + 347)
#define	SVT_PLN_AMB_ACTUAL_COUCH	VFT_INT32
#define	SVS_PLN_AMB_ACTUAL_COUCH	"Advanced MovingBed Actual Couch position"
/*	Advanced MovingBed 撮像時の実寝台位置
　  AMB撮像時にacqmanがセットする。
 */

#define	SVN_PLN_RF_INTERLOCK_VALUE	(SVN_BASE_PROTOCOL_PLAN + 348)
#define	SVT_PLN_RF_INTERLOCK_VALUE	VFT_FLT32
#define	SVS_PLN_RF_INTERLOCK_VALUE	"RF Interlock Value(W)"
/*	
	PQMが設定してAcqmanが使う。AMBのAcqman制御マルチプロトコル収集に有効
	従来はPQMがAcqmanに通信でRF INTERLOCK値を通知していたが、AMBでは
	Acqman制御マルチプロトコル収集になるのでフィールド渡しにする。
　  AMB撮像時にacqmanがセットする。
 */

#define SVN_PLN_AMB_SUBTRACTION      (SVN_BASE_PROTOCOL_PLAN + 349)
#define SVT_PLN_AMB_SUBTRACTION      VFT_BITFIELD
#define SVS_PLN_AMB_SUBTRACTION      "Advanced MovingBed Subtraction Flag"
/*  Advanced MovingBed差分の有無
	TRUE   MovingBed差分あり
	FALSE  MovingBed差分なし
*/

#define SVN_PLN_AMB_SUBTRACTION_IMAGES       (SVN_BASE_PROTOCOL_PLAN + 350)
#define SVT_PLN_AMB_SUBTRACTION_IMAGES       VFT_BITFIELD
#define SVS_PLN_AMB_SUBTRACTION_IMAGES       "Advanced MovingBed Subtraction Images"
/*  Advanced MovingBed差分画像種
Appcode
 #define VFC_SUBTRACTION_NONE     (1L<<0)    差分なし画像作成
 #define VFC_SUBTRACTION_ABSOLUTE (1L<<1)    絶対値差分画像作成
 #define VFC_SUBTRACTION_COMPLEX  (1L<<2)    複素差分画像作成
*/

#define SVN_PLN_AMB_SUBTRACTION_PROTOCOL (SVN_BASE_PROTOCOL_PLAN + 351)
#define SVT_PLN_AMB_SUBTRACTION_PROTOCOL VFT_INT32
#define SVS_PLN_AMB_SUBTRACTION_PROTOCOL "Advanced MovingBed Subtraction Protocol index"
/*  Advanced MovingBed差分処理プロトコル番号
*/

#define SVN_PLN_AMB_PARAM           (SVN_BASE_PROTOCOL_PLAN + 352)
#define SVT_PLN_AMB_PARAM           VFT_AMB_PARAM
#define SVS_PLN_AMB_PARAM           "Advanced MovingBed Parameter"
/*  Advanced MovingBed 撮像パラメータを記述するフィールド。
    vf_apptypes.h 参照。
*/

#define SVN_PLN_AMB_CELL            (SVN_BASE_PROTOCOL_PLAN + 353)
#define SVT_PLN_AMB_CELL            VFT_AMB_CELL
#define SVS_PLN_AMB_CELL            "Advanced MovingBed Cell"
/*  Advanced MovingBed 撮像のCELL情報を記述するフィールド。
    vf_apptypes.h 参照。
*/

#define SVN_PLN_AMB_POSTPROC        (SVN_BASE_PROTOCOL_PLAN + 354)
#define SVT_PLN_AMB_POSTPROC        VFT_AMB_POST_PROC
#define SVS_PLN_AMB_POSTPROC        "Advanced MovingBed Post Proc."
/*  Advanced MovingBed 撮影連動処理パラメータを記述するフィールド。
　　vf_apptypes.h 参照。
*/

#define SVN_PLN_OBLIQUE_EPI_ANGLE_TABLE (SVN_BASE_PROTOCOL_PLAN + 355)
#define SVT_PLN_OBLIQUE_EPI_ANGLE_TABLE VFT_FLT32
#define SVS_PLN_OBLIQUE_EPI_ANGLE_TABLE "Oblique Angle Table for Single Shot EPI"
/*
    シングルショットＥＰＩ用のOblique Angle Table

    設定者 PROLIM
    使用者 PROLIM,PQM
    
    とり得る値
           第一要素：直交の限界角度[deg]
           第二要素：シングルオブリークの限界角度[deg]
*/

#define SVN_PLN_SCANOFFSET_SET_FLAG (SVN_BASE_PROTOCOL_PLAN + 356)
#define SVT_PLN_SCANOFFSET_SET_FLAG VFT_INT32
#define SVS_PLN_SCANOFFSET_SET_FLAG "Scan Offset Set flag"
/*
    スキャンオフセット設定フラグ
    設定者 PQM,Locator
    使用者 PQM
    0：スキャンオフセット設定なし
    1：スキャンオフセット設定あり
*/

#define SVN_PLN_GATE_INTERVAL_FLAG (SVN_BASE_PROTOCOL_PLAN + 357)
#define SVT_PLN_GATE_INTERVAL_FLAG VFT_INT32
#define SVS_PLN_GATE_INTERVAL_FLAG "Flag for dummy shot in gating scan"
/*
    Gate空打ちスキャンフラグ
    0：Gate空打ちなし
    1：Gate空打ちあり
*/

#define SVN_PLN_FREQ_SUPPRESSION_PASTA (SVN_BASE_PROTOCOL_PLAN + 358)
#define SVT_PLN_FREQ_SUPPRESSION_PASTA VFT_INT32
#define SVS_PLN_FREQ_SUPPRESSION_PASTA "Frequency Suppression PASTA"
/*
    周波数抑制法PASTAのOn/Off
    設定者 PROLIM
    使用者 SEQ
    0：脂肪抑制なし
    1：PASTAあり

    #define VFC_SUPPRESSION_NONE    0
    #define VFC_SUPPRESSION_FAT     1
    #define VFC_SUPPRESSION_SI      2
*/

#define SVN_PLN_SLICE_NUMBER_METHOD (SVN_BASE_PROTOCOL_PLAN + 359)
#define SVT_PLN_SLICE_NUMBER_METHOD VFT_INT32
#define SVS_PLN_SLICE_NUMBER_METHOD "Slice Number Method"
/*
    スライス番号表示方式
    設定者 PROLIM,PQM
    使用者 sc_locator,dsc_refer,dsc_dRefer,dsc_inset,offscreen,Recon,Display
    0：スラブ間でのシーケンシャルなスライス番号表示
    1：スラブ毎のスライス番号表示

    #define VFC_SLICE_NUMBER_METHOD_SLICES    0
    #define VFC_SLICE_NUMBER_METHOD_SLAB      1
*/

#define SVN_PLN_BBTI_PREP_PARAM	(SVN_BASE_PROTOCOL_PLAN + 360)
#define SVT_PLN_BBTI_PREP_PARAM	VFT_FLT32
#define SVS_PLN_BBTI_PREP_PARAM	"BBTI prep parameters"
/*
    BBTIPrepスキャンにおけるBBTIの初期値、繰り返し時間、繰り返し回数。要素数３
    [0]：初期値
    [1]：繰り返し時間
    [2]：繰り返し回数
 */

#define SVN_PLN_GATE_INTERVAL_NUM_COLLECTION (SVN_BASE_PROTOCOL_PLAN + 361)
#define SVT_PLN_GATE_INTERVAL_NUM_COLLECTION VFT_INT32
#define SVS_PLN_GATE_INTERVAL_NUM_COLLECTION "Number of collection for dummy shot in gating scan"
/*
    Gate空打ちスキャンにおける%R-R内での収集回数。
 */


#define SVN_PLN_GATE_INTERVAL_SEGMENTATION_TYPE (SVN_BASE_PROTOCOL_PLAN + 362)
#define SVT_PLN_GATE_INTERVAL_SEGMENTATION_TYPE VFT_INT32
#define SVS_PLN_GATE_INTERVAL_SEGMENTATION_TYPE "Type of segmantation for dummy shot in gating scan"
/*
    Gate空打ちスキャンにおけるセグメンテーションタイプ。

    #define VFC_GATE_INTERVAL_SEGMENT_TYPE_SEQUENTIAL  1
    #define VFC_GATE_INTERVAL_SEGMENT_TYPE_REVERSE     2
 */

#define SVN_PLN_SHIM_RETRIED_FLAG (SVN_BASE_PROTOCOL_PLAN + 363)
#define SVT_PLN_SHIM_RETRIED_FLAG VFT_INT32
#define SVS_PLN_SHIM_RETRIED_FLAG "Shimming is retried or not"
/*
  This flag indicates shimming routine is retried or not.

  0: not retried.
  1: retried.

 */

#define SVN_PLN_NUM_SECTION_RECVR_COIL (SVN_BASE_PROTOCOL_PLAN + 364)
#define SVT_PLN_NUM_SECTION_RECVR_COIL VFT_INT32
#define SVS_PLN_NUM_SECTION_RECVR_COIL "Number of Section recver coil"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    選択されたコイルのSection数。
    ここに指定された数だけ、受信コイルのSection情報及びSignal情報が存在する。
*/

#define SVN_PLN_SECTION_RECVR_COIL (SVN_BASE_PROTOCOL_PLAN + 365)
#define SVT_PLN_SECTION_RECVR_COIL VFT_SECTION_RECVR_COIL
#define SVS_PLN_SECTION_RECVR_COIL "Section recver coil"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    受信コイルのSection情報
    このフィールドはPAS更新対象である。
    選択されたSection数(SVN_PLN_NUM_SECTION_RECVR_COIL)だけ配列が存在して、
    Signal情報(SVN_PLN_SECTION_RECVR_COIL_SIGNAL)とペアになっている。
    typedef struct VftSectionRecvrCoil_s{
        uint8_t     portID[2];  文字列、左詰めで空きはスペースいれる
        uint16_t    coilID;     物理的なコイルID
        uint8_t     SectionID;  Saturnコイルのみ、従来コイルの場合はゼロ
        uint8_t     channel;    従来コイルの場合だけ有効で、チャネル番号
    } VftSectionRecvrCoil_t;

    PortIDにはポート名(AG)が入る、またGポートのように同じポートが複数存在する
    場合はその番号も設定される。
    例）Gポートの５番の場合　”G5”、Fポート場合“F ”
    従来コイルのようにSectionDBがないコイルの場合はSectionIDを0とし、その代わり、
    Channel情報を持つ。
    Saturnコイルの場合はchannel情報に意味ない。
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL   (SVN_BASE_PROTOCOL_PLAN + 366)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL   VFT_SECTION_SIGNAL 
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL   "SECTION SIGNAL INFOMATION"

/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    Section Signal情報
    選択されたSection数(SVN_PLN_NUM_SECTION_RECVR_COIL)だけ配列が存在して、
    受信コイルのSection情報(SVN_PLN_SECTION_RECVR_COIL)とペアになっている。
    PAS更新の対象ではない。
    typedef struct VftSectionSignal_s{
        int16_t     num;               有効Signal数
        uint16_t    signal_no[16];      Signal 情報
        uint16_t    hybrid_signal[16];  Hybrid 情報
    }VftSectionSignal_t;

    signalNoにはSectionDB内に定義されている、SignalNoを設定する。
    またHybrid情報としては以下の情報が入る。
    0000 Non
    0001 DQ1
    0002 QD2
    0003 AntiQD1
    0004 AntiQD2
    0005 RL-
    0006 RL+
    0007 AP1
    0008 AP2
    0009 RL1
    000A RL2
*/

#define SVN_PLN_NUM_SECTION_RECVR_COIL2    (SVN_BASE_PROTOCOL_PLAN + 367)
#define SVT_PLN_NUM_SECTION_RECVR_COIL2    VFT_INT32
#define SVS_PLN_NUM_SECTION_RECVR_COIL2    "Number of Section recvr coil2"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    選択されたSection数「その2」
    SPEEDER MAP等、プロトコルの途中でコイルが変わる時に使われるコイル情報。
    選択されたコイルのSection数。
    ここに指定された数だけ、受信コイルのSection情報２及びSignal情報２が存在する。
*/

#define SVN_PLN_SECTION_RECVR_COIL2      (SVN_BASE_PROTOCOL_PLAN + 368)
#define SVT_PLN_SECTION_RECVR_COIL2      VFT_SECTION_RECVR_COIL
#define SVS_PLN_SECTION_RECVR_COIL2      "SECTION RECVR COIL2"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    受信コイルのSection情報「その2」
    SVN_PLN_SECTION_RECVR_COIL
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL2   (SVN_BASE_PROTOCOL_PLAN + 369)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL2   VFT_SECTION_SIGNAL 
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL2   "SECTION SIGNAL INFOMATION2"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    Section Signal情報「その2」
    SVN_PLN_SECTION_RECVR_COIL_SIGNAL
*/

#define SVN_PLN_RECVR_COIL_SHOW_LIST   (SVN_BASE_PROTOCOL_PLAN + 370)
#define SVT_PLN_RECVR_COIL_SHOW_LIST   VFT_COIL_SHOWLIST 
#define SVS_PLN_RECVR_COIL_SHOW_LIST   "RECVR COIL SHOW LIST"
/*
    Saturnシステム以降の新RFコイルシステムで場合に使われる情報
    表示コイル情報
    Saturnシステムのコイル選択処理で、最初に表示させるコイル情報。
    ここに登録させたコイル情報がコイル選択画面のコイルリストに反映される。
    PAS更新の対象である。
    typedef struct VftCoilListInfo_s {
        int32_t num;                コイル数
        struct {
            uint8_t     portID[2];  Port番号
            uint16_t    coilID;     CoilID 物理番号
            uint8_t     channel;    Channel情報
            uint8_t     coordinate; 表示位置
            uint8_t    filler[2];
        }　coillist[10];
    }VftCoilListInfo_t;
*/

#define SVN_PLN_NUM_PE_LINE_NOISE   (SVN_BASE_PROTOCOL_PLAN + 371)
#define SVT_PLN_NUM_PE_LINE_NOISE   VFT_INT32
#define SVS_PLN_NUM_PE_LINE_NOISE   "Number of PE line noise"
/*
    ch間ゲイン・ノイズマトリクス補正のための
    ノイズ情報収集分のPEエンコード数
    設定者 PROLIM
    使用者 PROLIM,RECON,SEQ
*/

#define SVN_PLN_PHASE_CORRECTION_PRESCAN_FLAG   (SVN_BASE_PROTOCOL_PLAN + 372)
#define SVT_PLN_PHASE_CORRECTION_PRESCAN_FLAG   VFT_INT32 
#define SVS_PLN_PHASE_CORRECTION_PRESCAN_FLAG   "Phase correction prescan flag"
/*
    位相補正プリスキャンフラグ
    0:prescanを実行しない
    1:prescanを実行
    PROLIMで制御する。

    #define VFC_PHASE_CORRECTION_PRESCAN_OFF   0
    #define VFC_PHASE_CORRECTION_PRESCAN_ON    1
*/

#define SVN_PLN_DEGENERACY_FLAG   (SVN_BASE_PROTOCOL_PLAN + 373)
#define SVT_PLN_DEGENERACY_FLAG   VFT_BOOL
#define SVS_PLN_DEGENERACY_FLAG   "Raw data Degeneracy flag"
/*
    受信データ(生データ)の縮退を行う時、TRUE(1)にする。
    このフィールドが存在しない時、または、FALSE(0)が
    設定されている時は縮退しない。

    設定者 PROLIM
    使用者 PROLIM,acqman,pqm,recon
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   (SVN_BASE_PROTOCOL_PLAN + 374)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   VFT_SECTION_SIGNAL
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   "SECTION SIGNAL INFOMATION for Degeneracy"
/*
    受信データ(生データ)の縮退を行う時のSection Signal情報。

    設定者 pqm
    使用者 acqman
*/

#define SVN_PLN_K_SPACE_TRAJECTORY_TYPE   (SVN_BASE_PROTOCOL_PLAN + 375)
#define SVT_PLN_K_SPACE_TRAJECTORY_TYPE   VFT_INT32
#define SVS_PLN_K_SPACE_TRAJECTORY_TYPE   "K space trajectory type"
/*
    JETのKeyフラグ。
    これをキーにして、シーケンス、PROLIM、再構成がJETを認識する。

    設定者 PROLIM
    使用者 PROLIM,recon,pqm

    #define VFC_K_SPACE_TRAJECTORY_TYPE_NONE        0
    #define VFC_K_SPACE_TRAJECTORY_TYPE_2DJET       1
    #define VFC_K_SPACE_TRAJECTORY_TYPE_2DRADIAL    2
    #define VFC_K_SPACE_TRAJECTORY_TYPE_3DRADIAL    3
    #define VFC_K_SPACE_TRAJECTORY_TYPE_SPIRAL      4
*/

#define SVN_PLN_NUM_BLADE_FOR_JET   (SVN_BASE_PROTOCOL_PLAN + 376)
#define SVT_PLN_NUM_BLADE_FOR_JET   VFT_INT32
#define SVS_PLN_NUM_BLADE_FOR_JET   "Number of blade for JET"
/*
    JET収集時のBlade数

    設定者 PROLIM
    使用者 PROLIM,recon,sequence
*/

#define SVN_PLN_SWITCH_COIL_TYPE   (SVN_BASE_PROTOCOL_PLAN + 377)
#define SVT_PLN_SWITCH_COIL_TYPE   VFT_INT32
#define SVS_PLN_SWITCH_COIL_TYPE   "Switch coil type"
/*
    コイル切り替え撮像のタイプ

    設定者 PROLIM
    使用者 PROLIM,pqm,seqence

    #define VFC_SWITCH_COIL_TYPE_NONE       0
    #define VFC_SWITCH_COIL_TYPE_MAP        1
*/

#define SVN_PLN_NUM_SWITCH_COIL     (SVN_BASE_PROTOCOL_PLAN + 378)
#define SVT_PLN_NUM_SWITCH_COIL     VFT_INT32
#define SVS_PLN_NUM_SWITCH_COIL     "Number of switch coil"
/*
    コイル切り替え撮像にて切り替えるコイルの数

    設定者 PROLIM
    使用者 PROLIM,seqence
*/

#define SVN_PLN_FSBB_B_VALUE        (SVN_BASE_PROTOCOL_PLAN + 379)
#define SVT_PLN_FSBB_B_VALUE        VFT_FLT32
#define SVS_PLN_FSBB_B_VALUE        "b-value for FSBB"
/*
    FSBB用のディフェーズ量(b値)

    設定者 PROLIM
    使用者 display
*/

#define SVN_PLN_BODY_PART           (SVN_BASE_PROTOCOL_PLAN + 380)
#define SVT_PLN_BODY_PART           VFT_STRING
#define SVS_PLN_BODY_PART           "Body part examined"
/*
    検査部位
    DICOM TAG (0018,0015)にセットされ送信される情報である。
    従来はSAR部位を撮影部位としてDICOM転送していたが、
    さらなる詳細な検査部位情報を必要とするため新設された。

    setting     pqm
    reference   DICOM
*/

#define SVN_PLN_NOWRAP_RATIO_INPUT  (SVN_BASE_PROTOCOL_PLAN + 381)
#define SVT_PLN_NOWRAP_RATIO_INPUT  VFT_FLTVEC3
#define SVS_PLN_NOWRAP_RATIO_INPUT  "Input NoWrap ratio"
/*
    NoWrap Ratioの入力値

    setting     PROLIM
    reference   PROLIM
*/

#define SVN_PLN_FATSAT_PULSE_NUM    (SVN_BASE_PROTOCOL_PLAN + 382)
#define SVT_PLN_FATSAT_PULSE_NUM    VFT_INT32
#define SVS_PLN_FATSAT_PULSE_NUM    "Number of fatsat pulse"
/*
    Number of fatsat pulse(including all fatsat pulse).
    e.g. Double-FS = 2, Triple-FS = 3.

    setting     PROLIM
    reference   PROLIM,seqgen
*/

#define SVN_PLN_FATSAT_MULTI_FLIP_ANGLE     (SVN_BASE_PROTOCOL_PLAN + 383)
#define SVT_PLN_FATSAT_MULTI_FLIP_ANGLE     VFT_INT32
#define SVS_PLN_FATSAT_MULTI_FLIP_ANGLE     "Multi-flip angle for fatsat pulse"
/*
    Multi-flip angle for fatsat pulse

    setting     PROLIM
    reference   PROLIM,seqgen
*/

#define SVN_PLN_VARI_FLOP_FLAG      (SVN_BASE_PROTOCOL_PLAN + 384)
#define SVT_PLN_VARI_FLOP_FLAG      VFT_INT32
#define SVS_PLN_VARI_FLOP_FLAG      "Variable Flop Angle Flag"
/*
    FSE系でのFlop角をSweepすることを制御する。

    setting     PROLIM
    reference   seqgen

    #define VFC_VFA_FOR_NONE           0
    #define VFC_VFA_FOR_2D             1
    #define VFC_VFA_FOR_3DBRAIN_T2     2
    #define VFC_VFA_FOR_3DBRAIN_PD     3
    #define VFC_VFA_FOR_3DOTHER_T2     4
    #define VFC_VFA_FOR_3DOTHER_PD     5
*/

#define SVN_PLN_GATE_CTRL_TYPE      (SVN_BASE_PROTOCOL_PLAN + 385)
#define SVT_PLN_GATE_CTRL_TYPE      VFT_INT32
#define SVS_PLN_GATE_CTRL_TYPE      "Gate control parameters type"
/*
    FFE Cine系シーケンスのTime Stampの計算アルゴリズムを指定する。

    VFC_GATE_CTRL_TYPE_BYRECON(0) あるいはフィールドがない場合：
     Cine で Segment 指定されている場合は Recon で TimeStampを計算する。
    0以外の場合：
     prolim で TimeStamp を計算する。

    setting     PROLIM
    reference   RECON

    #define VFC_GATE_CTRL_TYPE_BYRECON      0
    #define VFC_GATE_CTRL_TYPE_BYPROLIM     1
*/

#define	SVN_PLN_SELECTED_ADC_PITCH	(SVN_BASE_PROTOCOL_PLAN + 386)
#define	SVT_PLN_SELECTED_ADC_PITCH	VFT_FLT32
#define	SVS_PLN_SELECTED_ADC_PITCH	"Selected ADC Pitch"
/*	Selected ADC Pitch
        setting     PROLIM
        reference   PROLIM
*/

#define	SVN_PLN_SELECTED_GRAB_RATIO	(SVN_BASE_PROTOCOL_PLAN + 387)
#define	SVT_PLN_SELECTED_GRAB_RATIO	VFT_FLT32
#define	SVS_PLN_SELECTED_GRAB_RATIO	"Selected Grab Ratio"
/*	Selected Grab Ratio
        setting     PROLIM
        reference   PROLIM
*/

#define	SVN_PLN_FLEX_BW_FLAG	(SVN_BASE_PROTOCOL_PLAN + 388)
#define	SVT_PLN_FLEX_BW_FLAG	VFT_INT32
#define	SVS_PLN_FLEX_BW_FLAG	"Flexible BW Flag"
/*	Flexible BW Flag
        setting     PROLIM
        reference   PROLIM,seqgen
*/

#define	SVN_PLN_ESTIMATED_ADC_TIME	(SVN_BASE_PROTOCOL_PLAN + 389)
#define	SVT_PLN_ESTIMATED_ADC_TIME	VFT_FLT32
#define	SVS_PLN_ESTIMATED_ADC_TIME	"Estimated ADC Time"
/*	ADC Time
        setting     PROLIM
        reference   PROLIM
*/

#define	SVN_PLN_ADC_POINTS	(SVN_BASE_PROTOCOL_PLAN + 390)
#define	SVT_PLN_ADC_POINTS	VFT_INT32
#define	SVS_PLN_ADC_POINTS	"ADC Points"
/*	ADC Points
        setting     PROLIM
        reference   PROLIM
*/

#define	SVN_PLN_SAR_CALC_MODE	(SVN_BASE_PROTOCOL_PLAN + 391)
#define	SVT_PLN_SAR_CALC_MODE	VFT_INT32
#define	SVS_PLN_SAR_CALC_MODE	"SAR Calculation Mode"
/*	SAR Calculation Mode
        VFC_SAR_CALC_MODE_MODEL     (=0) Based on model (for SDE)
        VFC_SAR_CALC_MODE_SEQ       (=1) Based on sequence (for NSDE)
*/

#define	SVN_PLN_TI_PREP_PARAM	(SVN_BASE_PROTOCOL_PLAN + 392)
#define	SVT_PLN_TI_PREP_PARAM	VFT_FLT32
#define	SVS_PLN_TI_PREP_PARAM	"TI prep parameters"
/*
    TIPrepスキャンにおけるTIの初期値[sec]、繰り返し時間[sec]、繰り返し回数[回]。要素数３
    [0]：初期値
    [1]：繰り返し時間
    [2]：繰り返し回数
    setting     PROLIM
    reference   RECON
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID	(SVN_BASE_PROTOCOL_PLAN + 393)
#define	SVT_PLN_LOCATOR_IMAGE_ID	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID	"1st locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_2	(SVN_BASE_PROTOCOL_PLAN + 394)
#define	SVT_PLN_LOCATOR_IMAGE_ID_2	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_2	"2nd locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_3	(SVN_BASE_PROTOCOL_PLAN + 395)
#define	SVT_PLN_LOCATOR_IMAGE_ID_3	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_3	"3rd locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_4	(SVN_BASE_PROTOCOL_PLAN + 396)
#define	SVT_PLN_LOCATOR_IMAGE_ID_4	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_4	"4th locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_5	(SVN_BASE_PROTOCOL_PLAN + 397)
#define	SVT_PLN_LOCATOR_IMAGE_ID_5	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_5	"5th locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_6	(SVN_BASE_PROTOCOL_PLAN + 398)
#define	SVT_PLN_LOCATOR_IMAGE_ID_6	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_6	"6th locator image loid"
/*
    親画像情報を参照するためのLogical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_NUM_IR_MULTIPLEX	(SVN_BASE_PROTOCOL_PLAN + 399)
#define	SVT_PLN_NUM_IR_MULTIPLEX	VFT_INT32
#define	SVS_PLN_NUM_IR_MULTIPLEX	"Number of IR multiplex"
/*	Number of IR multiplex
        setting     PROLIM
        reference   seqgen
 */

#define	SVN_PLN_CDS_FLAG	(SVN_BASE_PROTOCOL_PLAN + 400)
#define	SVT_PLN_CDS_FLAG	VFT_INT32
#define	SVS_PLN_CDS_FLAG	"Coil detect scan flag"
/*
    コイル自動設定処理の実行フラグ
    pqm画面の［コイル自動設定］ボタンの状態を保持する.
    0:［コイル自動設定］ボタンがオフ
    1:［コイル自動設定］ボタンがオン
    PAS更新対象
    設定者：pqm
    使用者：pqm
    #define VFC_CDS_FLAG_OFF 0
    #define VFC_CDS_FLAG_ON  1
*/

#define	SVN_PLN_CDS_STATUS	(SVN_BASE_PROTOCOL_PLAN + 401)
#define	SVT_PLN_CDS_STATUS	VFT_INT32
#define	SVS_PLN_CDS_STATUS	"CDS status"
/*
    コイル自動設定の動作内容。撮像後も値は保持される
    トラブル解析用にあとでCDS結果を参照するために使用する
    PAS更新対象外
    設定者：pqm
    使用者：pqm
    #define VFC_CDS_STATUS_AUTO_SELECT  0x0001
    #define VFC_CDS_STATUS_SCAN         0x0002
    #define VFC_CDS_STATUS_USE_PREVIOUS 0x0004
*/

#define	SVN_PLN_CDS_COIL_COORDINATE	(SVN_BASE_PROTOCOL_PLAN + 402)
#define	SVT_PLN_CDS_COIL_COORDINATE	VFT_CDS_COIL_LOCATION
#define	SVS_PLN_CDS_COIL_COORDINATE	"CDS coil coordinate"
/*
    寝台の磁石側端からコイル中心までの距離
    PAS更新対象外
    設定者：pqm
    使用者：pqm
    {CoilID + detectedFlag + コイル位置}の配列。
    配列順序はacqmanからpqmへの接続コイル情報と同じ
    配列順序 = {A/空/空/D1/D2/F/G1/--/G7}（１３個）
    コイル位置はZ軸座標。
    SVN_EXE_CDS_COIL_LOCATIONと同じフォーマット
*/

#define	SVN_PLN_REPETITIONS_CDS		(SVN_BASE_PROTOCOL_PLAN + 403)
#define	SVT_PLN_REPETITIONS_CDS		VFT_INT32
#define	SVS_PLN_REPETITIONS_CDS		"Number of repetitions of CDS scan"
/*
    CDSのダイナミックスキャン回数。コイル切り替え数である
    PAS更新対象外。
    設定者：pqm
    使用者：acqman
*/

#define	SVN_PLN_NUM_SECTION_RECVR_COIL_CDS	(SVN_BASE_PROTOCOL_PLAN + 404)
#define	SVT_PLN_NUM_SECTION_RECVR_COIL_CDS	VFT_INT32
#define	SVS_PLN_NUM_SECTION_RECVR_COIL_CDS	"Number of Section recver coil of CDS"
/*
    CDSスキャンするコイルのsection数
    SVN_PLN_NUM_SECTION_RECVR_COIL の構造で、
    SVN_PLN_REPETITIONS_CDS の数ぶん、つながった構造。
    PAS更新対象外。
    設定者：pqm
    使用者：acqman
*/

#define	SVN_PLN_SECTION_RECVR_COIL_CDS		(SVN_BASE_PROTOCOL_PLAN + 405)
#define	SVT_PLN_SECTION_RECVR_COIL_CDS		VFT_SECTION_RECVR_COIL
#define	SVS_PLN_SECTION_RECVR_COIL_CDS		"Section recver coil of CDS"
/*
    CDSスキャンする受信コイルのSection情報。
    SVN_PLN_SECTION_RECVR_COIL の構造で、
    SVN_PLN_REPETITIONS_CDS の数ぶん、つながった構造。
    PAS更新対象外。
    設定者：pqm
    使用者：acqman
*/

#define	SVN_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	(SVN_BASE_PROTOCOL_PLAN + 406)
#define	SVT_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	VFT_SECTION_SIGNAL 
#define	SVS_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	"SECTION SIGNAL INFOMATION of CDS"
/*
    CDSスキャンする受信コイルのSection Signal情報。
    SVN_PLN_SECTION_RECVR_COIL_SIGNALの構造で、
    SVN_PLN_REPETITIONS_CDS の数ぶん、つながった構造。
    PAS更新対象外。
    設定者：pqm
    使用者：acqman
*/

#define	SVN_PLN_GATING_SECTION_START_POINT	(SVN_BASE_PROTOCOL_PLAN + 407)
#define	SVT_PLN_GATING_SECTION_START_POINT	VFT_INT32
#define	SVS_PLN_GATING_SECTION_START_POINT	"Gating section start point"
/*
    冠動脈静止区間の開始時間[msec]

    設定者：PROLIM
    使用者：PROLIM
*/

#define	SVN_PLN_GATING_SECTION_END_POINT	(SVN_BASE_PROTOCOL_PLAN + 408)
#define	SVT_PLN_GATING_SECTION_END_POINT	VFT_INT32
#define	SVS_PLN_GATING_SECTION_END_POINT	"Gating section end point"
/*
    冠動脈静止区間の終了時間[msec]

    設定者：PROLIM
    使用者：PROLIM
*/

#define	SVN_PLN_CDS_SEQ_FLAG	(SVN_BASE_PROTOCOL_PLAN + 409)
#define	SVT_PLN_CDS_SEQ_FLAG	VFT_INT32
#define	SVS_PLN_CDS_SEQ_FLAG	"CDS sequence flag"
/*
    CDSシーケンスかどうかのフラグ
    CDSシーケンス       1
    Not CDSシーケンス   0
    PAS更新対象外。

    設定者：PROLIM
    使用者：seqgen
*/

#define	SVN_PLN_MOVE_COUCH_METHOD_FLAG		(SVN_BASE_PROTOCOL_PLAN + 410)
#define	SVT_PLN_MOVE_COUCH_METHOD_FLAG		VFT_INT32
#define	SVS_PLN_MOVE_COUCH_METHOD_FLAG		"Move couch method flag"
/*
    プラン位置の中心を磁場中心に移動するか、親画像の中心位置を磁場中心に
    移動するかを設定するフィールド。
    PAS更新対象。
    VFC_MOVE_COUCH_METHOD_NONE              -1
    VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI  0
    VFC_MOVE_COUCH_METHOD_PARENT_POSI       1

    設定者：pqm
    使用者：pqm,PROLIM
*/

#define	SVN_PLN_COPY_SOURCE_PROTOCOL_NO		(SVN_BASE_PROTOCOL_PLAN + 411)
#define	SVT_PLN_COPY_SOURCE_PROTOCOL_NO		VFT_INT32
#define	SVS_PLN_COPY_SOURCE_PROTOCOL_NO		"Copy source protocol number"
/*
    プリスキャン情報のコピー元のプロトコル番号を設定するフィールド。
    デフォルト値は-1。
    PASから新規シーケンスをpqmに読み込んだときに-1を設定する。
    PAS更新対象外。

    設定者：pqm,PROLIM
    使用者：pqm
*/

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_PLAN	SVN_MIN_SUBPROTOCOL+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define SVN_PLN_SUBPROTOCOL_TIME	(SVN_BASE_SUBPROTOCOL_PLAN + 0)
#define SVT_PLN_SUBPROTOCOL_TIME	VFT_INT32
#define SVS_PLN_SUBPROTOCOL_TIME	"Time for subprotocol (predicted)"
/*	サブプロトコル単位の収集時間。

	This field indicates the total expected amount of time required for
	execution of this specific subprotocol in seconds.  It is the expected
	or predicted amount of time because the acquisition time for sub-
	protocols such as gated subprotocols can not be exactly determined.
	The quantity in this field is determined by the support equations
	and communicated to ProEdit for display.
	*/

#define	SVN_PLN_SCHED_START_TIME	(SVN_BASE_SUBPROTOCOL_PLAN + 1)
#define	SVT_PLN_SCHED_START_TIME	VFT_INT32
#define	SVS_PLN_SCHED_START_TIME	"Start time relative to the end of the preceding subprotocol"
/*	スキャン開始時刻。

	This field indicates the start time in seconds relative to the
	actual end of the preceding subprotocol.  This field is written by
	ProEdit and will be 0 except in the case of protocols with >1
	repetition where the operator has specified the time delay in ProEdit.
	*/

#define	SVN_PLN_CARDIAC_ROT_INDEX  	(SVN_BASE_SUBPROTOCOL_PLAN + 2)
#define	SVT_PLN_CARDIAC_ROT_INDEX	VFT_INT32
#define	SVS_PLN_CARDIAC_ROT_INDEX	"Cardiac rotation of this subprotocol"
/*	This field indicates which cardiac rotation this subprotocol
	corresponds to.  It is written by ProEdit.  AT THIS TIME GATING IS NOT
        SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_SLICES_IN_SP      	(SVN_BASE_SUBPROTOCOL_PLAN + 3)
#define	SVT_PLN_SLICES_IN_SP		VFT_INT32
#define	SVS_PLN_SLICES_IN_SP		"Number of spatially distinct slices in subprotocol"
/*	サブプロトコル中に含まれるスライス数。

	This field indicates the number of spatially distinct slices acquired
	in this subprotocol.  This is not directly selected by the operator but
	is determined for each subprotocol by the ProLim tree restructuring
	routines.
	*/

#define SVN_PLN_NUM_FH_PS_IN_SP		(SVN_BASE_SUBPROTOCOL_PLAN + 4)
#define SVT_PLN_NUM_FH_PS_IN_SP		VFT_INT32
#define SVS_PLN_NUM_FH_PS_IN_SP		"Number of Freehand Presat regions in subprotocol"
/*	フリーハンドサチュレーション数。

	This field indicates the number of freehand presaturation regions in
	this subprotocol.
	*/

#define SVN_PLN_DO_FOVMTX_ITERATION	(SVN_BASE_SUBPROTOCOL_PLAN + 5)
#define SVT_PLN_DO_FOVMTX_ITERATION	VFT_BOOL
#define SVS_PLN_DO_FOVMTX_ITERATION	"Subprotocol is unique for purposes of FOV/Mtx limit computations"
/*	This field indicates if the subprotocol should be iterated over by the
	FOV/Mtx limit computations.
	*/

#define SVN_PLN_DYNAMIC_ID		(SVN_BASE_SUBPROTOCOL_PLAN + 6)
#define SVT_PLN_DYNAMIC_ID		VFT_INT32
#define SVS_PLN_DYNAMIC_ID		"Repetition number for this dynamic study protocol"
/*	This field indicates to which of the repetitions in a dynamic study 
	this subprotocol belongs to
	*/

#define SVN_PLN_NUM_FH_TAG	(SVN_BASE_SUBPROTOCOL_PLAN + 7)
#define SVT_PLN_NUM_FH_TAG	VFT_INT32
#define SVS_PLN_NUM_FH_TAG	"Number of Freehand Tag"
/*	フリーハンドタグ数。
	*/

#define SVN_PLN_NUM_SKIP_SAT		(SVN_BASE_SUBPROTOCOL_PLAN + 8)
#define SVT_PLN_NUM_SKIP_SAT		VFT_INT32
#define SVS_PLN_NUM_SKIP_SAT		"Number of skipping saturation"
/*	サチュレーションパルス間に含まれるスライス枚数
	整数２値の配列。
	val[0] : サチュレーションパルス間のスライス数
	val[1] : サブプロトコル内の総サチュレーションパルスセット数
	１スライスに対応するサチュレーションパルス群を１セットする
	*/

#define SVN_PLN_NUM_SKIP_FAT		(SVN_BASE_SUBPROTOCOL_PLAN + 9)
#define SVT_PLN_NUM_SKIP_FAT		VFT_INT32
#define SVS_PLN_NUM_SKIP_FAT		"Number of skipping fatsat"
/*	脂肪抑制パルス間に含まれるスライス枚数
	整数２値の配列。
	val[0] : 脂肪抑制パルス間のスライス数
	val[1] : サブプロトコル内の総脂肪抑制パルス数
	*/

#define SVN_PLN_NUM_PRESAT_FAT		(SVN_BASE_SUBPROTOCOL_PLAN + 10)
#define SVT_PLN_NUM_PRESAT_FAT		VFT_INT32
#define SVS_PLN_NUM_PRESAT_FAT		"Number of presat (fat)"
/*	プリサチュレーション(脂肪)パルス数
	*/

/*****************************************************************************
  SLICEGROUP/PRESAT name space
*/
    
#define SVN_BASE_SLICEGROUP_PLAN	SVN_MIN_SLICEGROUP+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define	SVN_PLN_SLICE_ORI_VECTOR	(SVN_BASE_SLICEGROUP_PLAN + 0)
#define	SVT_PLN_SLICE_ORI_VECTOR	VFT_FLTVEC3
#define	SVS_PLN_SLICE_ORI_VECTOR	"Unit vector normal to slice plane"
/*	This field is the unit vector in the patient's coordinate system
	normal to the plane of this slice group.  It is written by ProEdit
	and is determined by the operator's manipulations of the slice
	group in either ProEdit or the Graphic Locator.
	*/

#define	SVN_PLN_PHASE_ORI_VECTOR   	(SVN_BASE_SLICEGROUP_PLAN + 1)
#define	SVT_PLN_PHASE_ORI_VECTOR	VFT_FLTVEC3
#define	SVS_PLN_PHASE_ORI_VECTOR   	"Phase direction unit vector"
/*	This field is the unit vector in the patient's coordinate system
	along the phase encoding direction of this slice group.  It is
	written by ProEdit and is determined by the operator's manipulations
	of the slice group in either ProEdit or the Graphic Locator.
	*/

#define	SVN_PLN_OFFSET_VECTOR   	(SVN_BASE_SLICEGROUP_PLAN + 2)
#define	SVT_PLN_OFFSET_VECTOR		VFT_FLTVEC3
#define	SVS_PLN_OFFSET_VECTOR   	"Vector from magnet center to center of center slice"
/*	This field is a non-unit vector in the patient's coordinate system
	from the magnet's center to the center of the center slice of the
	indicated slice group.  It is written by ProEdit and is determined
	by the operator's manipulations of the slice group in either ProEdit
	or the Graphic Locator.
	*/

#define	SVN_PLN_THICKNESS		(SVN_BASE_SLICEGROUP_PLAN + 3)
#define	SVT_PLN_THICKNESS		VFT_FLT32
#define	SVS_PLN_THICKNESS		"Slice thickness"
/*	This field is the thickness of each slice in this slice group in
	meters.  This value is propagated from the selected slice thickness
	at the protocol level.
	*/

#define	SVN_PLN_SLICE_GAP		(SVN_BASE_SLICEGROUP_PLAN + 4)
#define	SVT_PLN_SLICE_GAP		VFT_FLT32
#define	SVS_PLN_SLICE_GAP		"Gap between slices"
/*	This field is the gap of between neighboring slices in this slice
	group in meters.  This value is propagated from the selected slice
	gap at the protocol level.
	*/

#define SVN_PLN_SLICES_IN_SG	(SVN_BASE_SLICEGROUP_PLAN + 5)
#define SVT_PLN_SLICES_IN_SG	VFT_INT32
#define SVS_PLN_SLICES_IN_SG	"Number of selected slices per slice group"
/*	This field is the number of slices for this slice group determined by
	user selection.  This is not necessarily the number of slice nodes in
	the slice group especially for 3D protocols.
	*/

#define	SVN_PLN_MRS_OFFSET_VECTOR   	(SVN_BASE_SLICEGROUP_PLAN + 6)
#define	SVT_PLN_MRS_OFFSET_VECTOR	VFT_FLTVEC3
#define	SVS_PLN_MRS_OFFSET_VECTOR   	"Vector from magnet center to center of MRS Selected"
/*	MRS選択領域のオフセットベクタ。
	*/

#define SVN_PLN_SLICES_IN_SG_SPEEDER	(SVN_BASE_SLICEGROUP_PLAN + 7)
#define SVT_PLN_SLICES_IN_SG_SPEEDER	VFT_INT32
#define SVS_PLN_SLICES_IN_SG_SPEEDER	"Number of slices per slice group for SPEEDER"
/*	SPEEDER展開処理後のスライスグループの枚数
	*/


/*****************************************************************************
  SLICE name space
*/

#define SVN_BASE_SLICE_PLAN		SVN_MIN_SLICE+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define	SVN_PLN_IMAGES_IN_SLICE		(SVN_BASE_SLICE_PLAN + 0)
#define	SVT_PLN_IMAGES_IN_SLICE		VFT_INT32
#define	SVS_PLN_IMAGES_IN_SLICE		"Number of images for each slice"
/*	This field indicates the number of image nodes for each slice.  This
	may not necessarily be the number of echoes for each slice for various
	oddball sequences.
	*/

/*****************************************************************************
  IMAGE name space
*/

#define SVN_BASE_IMAGE_PLAN		SVN_MIN_IMAGE+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define	SVN_PLN_CONTRAST_TE		(SVN_BASE_IMAGE_PLAN + 0)
#define	SVT_PLN_CONTRAST_TE		VFT_FLT32
#define	SVS_PLN_CONTRAST_TE		"Contrast TE"
/*	This field is the TE indicative of the contrast of the image in
	seconds.  The values in these fields are the quantities selected
	by the operator in ProEdit.  They are propagated from another
	VarField in the PLAN part of the PROTOCOL name space.  Anyone
	interested in labeling images or describing protocols should use
	the values in these fields.  The actual TEs of the individual echoes
	that make up the image are described in another VarField in the
	SEQUENCE part of the IMAGE name space.
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




/*****************************************************************************
  IMCALC name space
*/




/*****************************************************************************
  VISUALGROUP name space
*/

#define SVN_BASE_VISUALGROUP_PLAN	SVN_MIN_VISUALGROUP+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define SVN_PLN_SLICES_IN_VG		(SVN_BASE_VISUALGROUP_PLAN + 0)
#define SVT_PLN_SLICES_IN_VG		VFT_INT32
#define SVS_PLN_SLICES_IN_VG		"Number of slices in visual group"
/*	This field describes the total number of slices in the visual group.

	ビジュアルグループ内に存在するスライス数。ここで、ビジュアルグループは
	ユーザが意識するスラブに相当する。
        */

#define SVN_PLN_VG_SLICE_ORI_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 1)
#define SVT_PLN_VG_SLICE_ORI_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_SLICE_ORI_VECTOR	"Unit vector normal to slice visual group"
/*	This field is the unit vector in the patient's coordinate system normal
	to the plane of this visual group.  It is written by ProEdit and is
	determined by the operator's manipulations of the visual group in
	the Graphic Locator.

	スライス法線ベクトル。（単位ベクトル）
	*/

#define SVN_PLN_VG_PHASE_ORI_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 2)
#define SVT_PLN_VG_PHASE_ORI_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PHASE_ORI_VECTOR	"Unit vector in PE direction of slice VG"
/*	This field is the unit vector in the patient's coordinate system along
	the phase encoding direction of this visual group.  It is written by
	ProEdit and is determined by the operator's manipulations of the visual
	group in the Graphic Locator.

	フェーズエンコードベクトル。（単位ベクトル）
	*/

#define SVN_PLN_VG_OFFSET_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 3)
#define SVT_PLN_VG_OFFSET_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_OFFSET_VECTOR	"Vector from magnet center to center of visual group's center slice"
/*	This field is a non-unit vector in the patient's coordinate system from
	the magnet's center to the center of the center slice of the indicated
	visual group.  It is written by ProEdit and is determined by the
	operator's manipulations of the visual group in the Graphic Locator.

	オフセットベクトル。
	*/

#define SVN_PLN_VG_PRESAT_THICKNESS	(SVN_BASE_VISUALGROUP_PLAN + 4)
#define SVT_PLN_VG_PRESAT_THICKNESS	VFT_FLT32
#define SVS_PLN_VG_PRESAT_THICKNESS	"Thickness of presaturation region"
/*	This field is the thickness of the presaturation region described by
	this visual group.  It is determined by the operator's manipulations of
	the presat region in the Graphic Locator.

	プリサット幅。
	*/

#define SVN_PLN_PRESAT_TYPE		(SVN_BASE_VISUALGROUP_PLAN + 5)
#define SVT_PLN_PRESAT_TYPE		VFT_INT32
#define SVS_PLN_PRESAT_TYPE		"Type of presaturation region"
/*	This field indicates whether the presaturation region described by this
	visual group is a slice, phase, frequency, coverage, upstream,
	downstream or freehand presat.

	プリサット型。
		VFC_PRESAT_TYPE_SLICE
		VFC_PRESAT_TYPE_PE
		VFC_PRESAT_TYPE_RO
		VFC_PRESAT_TYPE_COVERAGE（未使用）
		VFC_PRESAT_TYPE_UPSTREAM
		VFC_PRESAT_TYPE_DOWNSTREAM
		VFC_PRESAT_TYPE_FREEHAND
	*/

#define SVN_PLN_PRESAT_MANIPULATED     	(SVN_BASE_VISUALGROUP_PLAN + 6)
#define SVT_PLN_PRESAT_MANIPULATED     	VFT_BOOL
#define SVS_PLN_PRESAT_MANIPULATED     	"Was presat region moved by operator"
/*	This field is a flag to indicate if the operator has explicitly
	manipulated the presaturation region described by this visual group.
	*/

#define SVN_PLN_VG_SP_INDEX		(SVN_BASE_VISUALGROUP_PLAN + 7)
#define SVT_PLN_VG_SP_INDEX		VFT_INT32
#define SVS_PLN_VG_SP_INDEX		"Indices of SPs of SGs in VG"
/*	This field is an array of indices of the subprotocols of the slice
	groups that make up this visual group.

	ビジュアルグループに対応するサブプロトコルスライスグループの
	マップ情報。４つの数値からなりそれぞれは、
		１	先頭のサブプロトコル
		２	先頭のスライスグループ
		３	最終サブプロトコル
		４	最終スライスグループ
	これは、１つのスラブを複数のスキャン（カバレージ）で撮る場合に
	使用するものである。
	*/

#define SVN_PLN_VG_SG_INDEX		(SVN_BASE_VISUALGROUP_PLAN + 8)
#define SVT_PLN_VG_SG_INDEX		VFT_INT32
#define SVS_PLN_VG_SG_INDEX		"Indices of SGs in VG"
/*	This field is an array of indices of the slice groups that make up this
	visual group.
	*/

#define SVN_PLN_VG_TYPE			(SVN_BASE_VISUALGROUP_PLAN + 9)
#define SVT_PLN_VG_TYPE			VFT_INT32
#define SVS_PLN_VG_TYPE			"Type of visual group"
/*	This field indicates whether the visual group is of parallel, radial or
	double radial type.
	*/

#define SVN_PLN_VG_SELECTED		(SVN_BASE_VISUALGROUP_PLAN + 10)
#define SVT_PLN_VG_SELECTED		VFT_BOOL
#define SVS_PLN_VG_SELECTED		"Is VG highlighted in Graphic Locator"
/*	This field indicates if this visual group is highlighted in the Graphic
	Locator and is therefore the visual group for ProLim to base its
	calculations on.
	*/

#define SVN_PLN_VG_PRESAT_ORI_VECTOR    (SVN_BASE_VISUALGROUP_PLAN + 11)
#define SVT_PLN_VG_PRESAT_ORI_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_ORI_VECTOR	"Unit vector normal to presat visual group"

#define SVN_PLN_VG_PRESAT_OFFSET_VECTOR (SVN_BASE_VISUALGROUP_PLAN + 12)
#define SVT_PLN_VG_PRESAT_OFFSET_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_OFFSET_VECTOR	"Vector from magnet center to center of presat visual group"

#define SVN_PLN_VG_SG_MAPPING		(SVN_BASE_VISUALGROUP_PLAN + 13)
#define SVT_PLN_VG_SG_MAPPING		VFT_INT32
#define SVS_PLN_VG_SG_MAPPING		"Mapping of SGs in VG"
/*	This field is an array of the subprotocols and slice groups of the
	first and last slice group making up a visual group.
	*/

#define SVN_PLN_PRESAT_ID		(SVN_BASE_VISUALGROUP_PLAN + 14)
#define SVT_PLN_PRESAT_ID		VFT_INT32
#define SVS_PLN_PRESAT_ID		"Identifier for each presat region"
/*	This field is an identifier used to distinquish presat regions
	within a given presat type.
	*/

#define SVN_PLN_VG_PRESAT_ID		(SVN_BASE_VISUALGROUP_PLAN + 15)
#define SVT_PLN_VG_PRESAT_ID		VFT_INT32
#define SVS_PLN_VG_PRESAT_ID		"Identifier for presat regions in VG"
/*	This field is the ID of the presat region associated with
	this visual group.
	*/

#define SVN_PLN_VG_PRESAT_TYPE		(SVN_BASE_VISUALGROUP_PLAN + 16)
#define SVT_PLN_VG_PRESAT_TYPE		VFT_INT32
#define SVS_PLN_VG_PRESAT_TYPE		"Type of presaturation region"
/*	This field indicates whether the presaturation region described by this
	visual group is a slice, phase, frequency, coverage, upstream,
	downstream or freehand presat.

	プリサット型。
		VFC_PRESAT_TYPE_SLICE
		VFC_PRESAT_TYPE_PE
		VFC_PRESAT_TYPE_RO
		VFC_PRESAT_TYPE_COVERAGE（未使用）
		VFC_PRESAT_TYPE_UPSTREAM
		VFC_PRESAT_TYPE_DOWNSTREAM
		VFC_PRESAT_TYPE_FREEHAND
	*/

/* The header indicated that this was supposed to have been added here,
 * but it was missing, so I put it in --- gwm
 */
#define SVN_SEQUENCE_CLASS		(SVN_BASE_VISUALGROUP_PLAN + 17)
#define SVT_SEQUENCE_CLASS		VFT_INT32
#define SVS_SEQUENCE_CLASS		"Classification of this sequence"
/*	This field is a single value which is used to separate
	sequences into classes of "similar" sequences. Sequences are
	similar if they have similar structures. For example, all
	SE, IR, and FE sequences should probably in a single class,
	but 3D sequences are in a separate class.
	For R0, each sequence is in its own class.
	*/

#define SVN_PLN_VG_ROTATION_HANDLE	(SVN_BASE_VISUALGROUP_PLAN + 18)
#define SVT_PLN_VG_ROTATION_HANDLE	VFT_INT32
#define SVS_PLN_VG_ROTATION_HANDLE	"Ratation handle"
/*	回転ハンドルの保持
	*/

#define SVN_PLN_VG_PRESAT_PHASE_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 19)
#define SVT_PLN_VG_PRESAT_PHASE_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_PHASE_VECTOR	"Unit vector normal to presat visual group"
/*	プリサットフェーズエンコードベクトル。（単位ベクトル）
	*/

#define SVN_PLN_VG_PRESAT_LOC_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 20)
#define SVT_PLN_VG_PRESAT_LOC_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_LOC_VECTOR	"Presat localizer vector for use by GL"
/*	This is another vector to help further define the positioning of the
	plane of a presat region.
	*/

#endif /* tami_vfStudy_vfs_plan_H__ */
