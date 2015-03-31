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
 * Aug.11.97	�Êڒ��K	Modified
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
/*	�X���u�h�c
	�O���t�B�b�N���P�[�^�A�C���[�W�}�g���N�X�A�f�B�X�v���C�ł̃X���u�ԍ�
	�\���̂��߂Ɏg�p����B�O���t�B�b�N���P�[�^�ł� VISUALGROUP ���ɒ�`
	�����l���g�p���Aprolim �͂��̒l���e SLICEGROUP �ɃR�s�[����B�č\
	���͂��̒l���G�R�[�m�[�h�ɃR�s�[����B�f�B�X�v���C�A�C���[�W�}�g���b
	�N�X�͂��̒l��\������B
 */

#define SVN_PLN_LDB_MAPPED_ID		(SVN_BASE_INDEPENDENT_PLAN + 2)
#define	SVT_PLN_LDB_MAPPED_ID		VFT_STRING
#define	SVS_PLN_LDB_MAPPED_ID		"LocalDB mapped identifier"
/*	�e�m�[�h�̑Ή�����LocalDB�̃X�L�[�}ID��ݒ肷��B
 */

/*****************************************************************************
  STUDY name space
*/

#define SVN_BASE_STUDY_PLAN	SVN_MIN_STUDY+SVN_OFF_PLAN


#define	SVN_PLN_PROT_ACQ_ORDER		(SVN_BASE_STUDY_PLAN + 0)
#define	SVT_PLN_PROT_ACQ_ORDER		VFT_INT32
#define	SVS_PLN_PROT_ACQ_ORDER		"Protocol Acquisition Order"
/*	�v���g�R���̎��W�����L�q����t�B�[���h�B�C���f�b�N�X�ԍ��̔z��B
	�o�p�l�ɂ���ċL�q�����B
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
/*	�p�C���b�g�X�L�����ɂ̂ݓK�p����I�t�Z�b�g�x�N�^�̋L���̈�B
	PQM �ɂ���Đݒ�^���p�����B�{�� PQM �̃�������ɋL������
	����̂����APQM �ċN�����ɏI�����̏�Ԃ�ێ����邽�߂ɃX�^�f�B
	�t�@�C�����Ɋi�[����B
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
/*	�f�B�X�v���C�ŕ\������V�[�P���X�����L�q����B�l�� seqselDB ��
	��`�����Bprolim �����W�����ɕ����ē��e��ύX����ꍇ������B
	
	This is the name by which the sequence is defined in the sequence
	database.  It is intended for use by ProEdit only.
	Written by proEdit.
	*/

#define SVN_PLN_TEMPLATE_SEQ		(SVN_BASE_PROTOCOL_PLAN + 1)
#define SVT_PLN_TEMPLATE_SEQ		VFT_STRING
#define SVS_PLN_TEMPLATE_SEQ		"Template sequence used"
/*	�V�[�P���X�����L�q����Bseqgen,prolim ����������ۂ̃L�[�Ƃ���
	�g�p�����BproselSeqselDB ���Ŏg�p����Ă��镶����Ɠ������̂�
	�X�^�f�B�t�@�C���ŋL�q����Ȃ���΂Ȃ�Ȃ��B

	This is the name of the sequence (seqgen) used in this protocol.
	The contents of this field originate in the sequence database.
	Written by proEdit.
	*/

/* GAP in numbering..... */

#define	SVN_PLN_IMAGING_MODE		(SVN_BASE_PROTOCOL_PLAN + 3)
#define	SVT_PLN_IMAGING_MODE		VFT_INT32
#define	SVS_PLN_IMAGING_MODE		"Imaging mode (2D/3D)"
/*	�C���[�W���O���[�h�i�Q�c�^�R�c�j���L�q����B

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
/*	�̓��␳�Ɋւ���L�q���s�Ȃ��B���݂� VFC_MSUP_BIT_FLOW_COMP �̒l��
	�ݒ肳��邩�O�ȊO�ɂ��蓾�Ȃ��B

	This indicates which motion supression techniques have been
	applied to this protocol.  The contents of this field are selected
	by the operator in ProEdit.  This is a bit field.  The bit masks
	for this field are defined in vf_appcodes.h
	Written by proEdit.
	*/

#define	SVN_PLN_PRESAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 6)
#define	SVT_PLN_PRESAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_PRESAT_FLIP_ANGLE	"Flip angle for presat regions"
/*	�v���T�`�����[�V�����̃t���b�v�p���L�q����B

	This is the flip angle in degrees to be be applied to each
	presaturation region in the protocol.  The contents of this
	field are selected by the operator in ProEdit.
	*/

#define	SVN_PLN_SELECTED_CONTRAST_TE	(SVN_BASE_PROTOCOL_PLAN + 7)
#define	SVT_PLN_SELECTED_CONTRAST_TE	VFT_FLT32
#define	SVS_PLN_SELECTED_CONTRAST_TE	"Selected contrast TE"
/*	�v���g�R���m�[�h�ɋL�q�����s�d�B�Q�G�R�[�̏ꍇ�͂��̃t�B�[���h��
	�z��Ƃ��Ďg�p����B�Ⴆ�� SE2580 �ł� v[0] = 0.025,v[1] = 0.08 ��
	�L�q����B�P�ʂ� sec �ł���B���l�̒l���C���[�W�m�[�h�ɃR�s�[�����B
	�R�s�[��̖��O�� SVN_PLN_CONTRAST_TE �ł���B�������A�C���[�W�m�[�h
	�̓G�R�[�����̃m�[�h�����݂��邽�߃R�s�[��ł͔z��Ƃ͂Ȃ�Ȃ��B

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
/*	�G�R�[���i�Por�Q�j

	This field describes the number of echoes produced for each slice
	in this protocol.  It is selected as a consequence of the operator's
	selection in ProEdit of the TE(s) for this protocol.  It is
	propagated to another VarField in the PLAN part of the SLICE name
	space.
	*/

#define	SVN_PLN_SELECTED_SLICE_THK	(SVN_BASE_PROTOCOL_PLAN + 9)
#define	SVT_PLN_SELECTED_SLICE_THK	VFT_FLT32
#define	SVS_PLN_SELECTED_SLICE_THK	"Selected thickness"
/*	�v���g�R���m�[�h�ɋL�q�����X���C�X���B�X���C�X�m�[�h�ɂ�����
	�l���R�s�[�����B�X���C�X�m�[�h�ł� SVN_PLN_THICKNESS �̖��O��
	�L�q�����B���O�̗R���̓��[�U�ɂ���� proEdit ��őI�������l
	�ł���Ƃ������Ƃł���B

	This is the thickness in meters of each slice in the protocol.
	It is a quantity selected by the operator in ProEdit.  This
	information is propagated to another VarField in the PLAN part
	of the SLICEGROUP name space.
	*/

#define SVN_PLN_SELECTED_SLICE_GAP	(SVN_BASE_PROTOCOL_PLAN + 10)
#define SVT_PLN_SELECTED_SLICE_GAP	VFT_FLT32
#define SVS_PLN_SELECTED_SLICE_GAP	"Selected gap"
/*	�v���g�R���m�[�h�ɋL�q�����X���C�X�M���b�v�B�X���C�X�m�[�h�ɓ���
	�l���R�s�[�����Ȃ� SVN_PLN_SELECTED_SLICE_THK �Ɠ����ł���B�X��
	�C�X�m�[�h�ł� SVN_PLN_SLICE_GAP �ƂȂ�B

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
/*	�܂�Ԃ��h�~�I�[�o�[�T���v�����O�i�o�d�E�q�n�j

	This describes the types of wraparound reduction selected by the
	operator in ProEdit.  This is a bit field.  The bit masks for this
	field are defined in vf_appcodes.h
	*/

#define	SVN_PLN_FOV			(SVN_BASE_PROTOCOL_PLAN + 14)
#define	SVT_PLN_FOV			VFT_FLTVEC2
#define	SVS_PLN_FOV			"Field of view of final image"
/*	�����ʂ�e�n�u�B���[�U���v���G�f�B�b�g�Ŏw�肷��B

	This describes the in plane FOV in meters of the image prior to any
	zooming, scaling, etc. that the operator may apply in Display.  These
	are the quantities selected by the operator in ProEdit and represent
	how they expect the image to appear.  Other applications should use
	this field to describe the FOV in labeling, etc.
	*/

#define	SVN_PLN_DISPLAYED_MATRIX	(SVN_BASE_PROTOCOL_PLAN + 15)
#define	SVT_PLN_DISPLAYED_MATRIX	VFT_FLTVEC2
#define	SVS_PLN_DISPLAYED_MATRIX	"Matrix size of final image"
/*	�\���}�g���b�N�X�B���W�}�g���b�N�X�Ƃ͈قȂ�B���[�U�� proEdit 
	�Ŏw�肷��B

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
/*	�t���b�v�p�B

	This field is the flip angle in degrees.  It is used for both field
	echo and spin echo sequences.  In case of spin echo, it is the flip
	angle of the excitation pulse. This quantity is selected by the
	operator in ProEdit.
	*/

#define SVN_PLN_GATING_METHOD		(SVN_BASE_PROTOCOL_PLAN + 17)
#define SVT_PLN_GATING_METHOD		VFT_BITFIELD
#define SVS_PLN_GATING_METHOD		"Gating Method(s) used"
/*	�Q�[�g�\�[�X���L�q����B
		VFC_GAT_METHOD_BIT_ECG		1	ECG 
		VFC_GAT_METHOD_BIT_PERIPHERAL	(1<<1)	Peripheral
		VFC_GAT_METHOD_BIT_RESPIRATORY	(1<<2)	Respiratory
	�̂R����`����Ă���B

	This describes the types of gating used in the protocol.  These
	include ECG, Peripheral, Respiratory and some combinations of these.
	This quantity is selected by the operator in ProEdit.  This is a bit
	field.  Bit masks for this field are defined in vf_appcodes.h.  AT THIS
	TIME GATING IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_GATE_PARAM		(SVN_BASE_PROTOCOL_PLAN + 18)
#define	SVT_PLN_GATE_PARAM		VFT_GATING_PARAM
#define	SVS_PLN_GATE_PARAM		"Gating parameters"
/*	�Q�[�g�X�L�����Ɋւ���p�����[�^���L�q����t�B�[���h�B vf_apptypes.h �Q�ƁB

	This will likely describe various parameters related to gating.  The
	details of this field have not been determined.  AT THIS TIME GATING IS
	NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_GATE_MGR		(SVN_BASE_PROTOCOL_PLAN + 19)
#define	SVT_PLN_GATE_MGR		VFT_GATING_MANAGER
#define	SVS_PLN_GATE_MGR		"Gate Manager control"
/*	�Q�[�g�}�l�[�W���[�Ɋւ���p�����[�^���L�q����t�B�[���h�B
	vf_apptypes.h �Q�ƁB

	This will likely describe various parameters related to the gate
	manager.  The details of this field have not been determined.  AT THIS
	TIME GATING IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/

#define	SVN_PLN_FAST_TR_FLAG		(SVN_BASE_PROTOCOL_PLAN + 20)
#define	SVT_PLN_FAST_TR_FLAG		VFT_BOOL
#define	SVS_PLN_FAST_TR_FLAG		"Fast TR"
/*	�o���A�u���s�q���w�肷��t�B�[���h�B�^�̏ꍇ�o���A�u���s�q�Ɖ���
	�����B

	This flag indicates whether mixed TR is used to acquire images in
	a reduced amount of time.  When set, the CONTRAST_TR VarField
	indicates only the apparent TR of the image.  The TR of each
	echo collected by the host is in another VarField in the PROTOCOL
	name space.  This quantity is selected by the operator in ProEdit.
	*/

#define SVN_PLN_CONTRAST_TR		(SVN_BASE_PROTOCOL_PLAN + 21)
#define SVT_PLN_CONTRAST_TR		VFT_FLT32
#define SVS_PLN_CONTRAST_TR		"Contrast TR"
/*	�s�q���w�肷��t�B�[���h�B

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
/*	�C���o�[�W�����p���X��t������ۂ̂s�h���Ԃ��L�q����B�P�ʂ͕b�B

	This field describes the Inversion Time, TI, of the images in
	seconds.  This quantity is selected by the operator in ProEdit.
	*/

#define	SVN_PLN_COVERAGES		(SVN_BASE_PROTOCOL_PLAN + 23)
#define	SVT_PLN_COVERAGES		VFT_INT32
#define	SVS_PLN_COVERAGES		"Number of coverages"
/*	�J�o���[�W���B�J�o���[�W�F�P��̃X�L�����łƂ�Ȃ��X���C�X����
	���w�肳�ꂽ�ꍇ�V�[�P���X(seqgen)�̕�����􂵂őΉ�����B����
	�ۂ̂P��� seqgen ���J�o���[�W�ƌĂԁB

	This field describes the number of different anatomical coverages
	to be performed in this protocol.  Each coverage will contain the
	same number of slices but in a different spatial location.  This
	quantity is selected by the operator in ProEdit.
	*/

#define	SVN_PLN_REPETITIONS		(SVN_BASE_PROTOCOL_PLAN + 24)
#define	SVT_PLN_REPETITIONS		VFT_INT32
#define	SVS_PLN_REPETITIONS		"Number of repetitions"
/*	��������� seqgen �𕡐���􂷁B���̍ۂ̌J��Ԃ��񐔂��w�肷��B
	�݌v�����A�_�C�i�~�b�N�͂��̌J��Ԃ��ɂ���đΉ�����͂���������
	seqgen �̃��[�h���ԁE�q�l�Ƃ̒ʐM���̎��Ԃ������ł��Ȃ����� V2.0
	�ȍ~ seqgen �̕�����􂵂ł̃_�C�i�~�b�N�Ή��͍s�Ȃ��Ă��Ȃ��B
	V2.0 �ȍ~���̃t�B�[���h�͒[�ɌJ��Ԃ��񐔂��L�q����݂̂ŕ����I��
	�Ӗ��������Ȃ��B

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
/*	�A�x���[�W���O�񐔁BVariNAQ �Ή��̂���VFT_FLT32 �ƂȂ��Ă���B

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
/*	���W�������L�q����t�B�[���h�B�Ƃ蓾��l��
		VFC_ACQ_ORDER_FORWARD	0
		VFC_ACQ_ORDER_BACKWARD	1
		VFC_ACQ_ORDER_CENTRIC	2
	��ɃQ�[�g�X�L�����Ŏg�p����Z���g���b�N�����W�����Ɖ��߂��A�]��
	VFT_BOOL �ł��������̃t�B�[���h�� VFT_INT32 �ƍĒ�`����L�R��
	�p�����[�^���Ƃ蓾��悤�ɕύX�����B

	This field describes whether slices are acquired in a forward or
	reverse order.  When the flag is set it indicates that the slices are
	acquired in forward order.  The contents of this field are selected by
	the operator in ProEdit.
	*/

#define	SVN_PLN_INTERL_FLAG		(SVN_BASE_PROTOCOL_PLAN + 30)
#define	SVT_PLN_INTERL_FLAG		VFT_INT32
#define	SVS_PLN_INTERL_FLAG		"Slice interleaving flag"
/*	�C���^�[���[�u���@���L�q����t�B�[���h�B�Ƃ蓾��l��
		VFC_INTRL_NONE		0
		VFC_INTRL_ALTERNATE	1
	�ł���B�݌v�����iTAMI�j�C���^�[���[�u�͒P�Ȃ�t���O��������
	�����I�ɑ��̃C���^�[���[�u�@���`���邱�Ƃ�z�肵�Č^��VFT_BOOL
	���� VFT_INT32 �ɕύX�����B
	
  	This field describes whether slices are acquired interleaved (TRUE)
	or not (FALSE).  The contents of this field are selected by the
	operator in ProEdit.
	*/

#define	SVN_PLN_TOTAL_SLICES		(SVN_BASE_PROTOCOL_PLAN + 31)
#define	SVT_PLN_TOTAL_SLICES		VFT_INT32
#define	SVS_PLN_TOTAL_SLICES		"Total slices"
/*	�X���C�X�����̑��a�B

	This field describes the total number of spatially distinct slices
	to be acquired in this entire protocol.  Temporal differences in
	the same physical slice are not considered to be different slices.
	This selection is made by the operator in ProEdit.
	*/

/* GAP in numbering..... */

#define	SVN_PLN_TOTAL_TIME		(SVN_BASE_PROTOCOL_PLAN + 33)
#define	SVT_PLN_TOTAL_TIME		VFT_INT32
#define	SVS_PLN_TOTAL_TIME		"Total for protocol (predicted)"
/*	�����W���ԁB

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
/*	���g�p�B

	This inane field describes which of the autovoice options are to be
	broadcast to the patient during the scan.  At this time the operation
	of Autovoice is undefined.  It is expected that this will be a bit
	field.  Bit masks for this field are defined in vf_appcodes.h.  AT THIS
	TIME AUTOVOICE IS NOT SUPPORTED AND THIS FIELD SHOULD NOT BE USED.
	*/


#define	SVN_PLN_APP_CODE		(SVN_BASE_PROTOCOL_PLAN + 35)
#define	SVT_PLN_APP_CODE		VFT_INT32
#define	SVS_PLN_APP_CODE		"Application selection"
/*	�A�v���P�[�V�����R�[�h�Bvf_appcode.h �Q�ƁB

	This field describes which application the operator has selected in
	ProEdit.  The contents of this field should be one of the values
	defined in vf_appcodes.h for this field.
	Written by proEdit.
	*/

#define SVN_PLN_SPATIAL_PRESAT		(SVN_BASE_PROTOCOL_PLAN + 36)
#define SVT_PLN_SPATIAL_PRESAT		VFT_BITFIELD
#define SVS_PLN_SPATIAL_PRESAT		"Spatial Presaturation Selection"
/*	�ʓ��T�`�����[�V�������r�b�g�_���a�Ŏw�肷��B vf_appcode.h �Q�ƁB

	This field describes the type of spatial presaturation convenience
	selection has been made.  The selection is made by the operator in
	ProEdit.  This is a bit field.  Bit masks for this field are defined in
	vf_appcodes.h.
	Written by proEdit.
	*/

#define SVN_PLN_FREEHAND_PRESAT		(SVN_BASE_PROTOCOL_PLAN + 37)
#define SVT_PLN_FREEHAND_PRESAT		VFT_BOOL
#define SVS_PLN_FREEHAND_PRESAT		"Freehand Presaturation flag"
/*	�t���[�n���h�v���T�`�����[�V�������w��\���ǂ����̃t���O�B
	���̃v���T�`�����[�V�������w�肳��邱�Ƃɂ��V�X�e���ŋ����Ă���
	�T�`�����[�V�����̌����I�[�o�[�������ɂ��̃t�B�[���h���U�ƂȂ�B

	This field is a flag to indicate if it is possible for the operator to
	define additional freehand presaturation regions in the Graphic
	Locator.
	Written by proEdit.
	*/

#define SVN_PLN_TURBO_PREP_TIME		(SVN_BASE_PROTOCOL_PLAN + 38)
#define SVT_PLN_TURBO_PREP_TIME		VFT_FLT32
#define SVS_PLN_TURBO_PREP_TIME		"Time of Turbo Preparation Pulse"
/*	���g�p�B

	This field describes the time in seconds of the preparation pulse used
	in a turbo sequence.  This quantity is selected by the operator in
	ProEdit.
	*/

#define SVN_PLN_TYPE_DELAY_BETWEEN_REPS	(SVN_BASE_PROTOCOL_PLAN + 39)
#define SVT_PLN_TYPE_DELAY_BETWEEN_REPS	VFT_INT32
#define SVS_PLN_TYPE_DELAY_BETWEEN_REPS	"Type of Delay Between Repetitions"
/*	�s�`�l�h�ɂ���č��ꂽ�t�B�[���h�B�T�u�v���g�R�����̌J��Ԃ����w�肷��
	�ۂɌJ��Ԃ����s�Ȃ����Ƀu���X�z�[���h���̃|�[�Y�𔭐�������d�g�݂����
	���Ƃ��Ă����悤�ł���B���݂͎g���Ă��Ȃ��B

	This field describes the type (constant, variable or unspecified) of
	delay between successive repetitions.  This quantity is selected by the
	operator in ProEdit.  The contents of this field should be one of the
	values defined in vf_appcodes.h for this field.
	Written by ProEdit.
	*/

#define	SVN_PLN_ANGIO_OPTION		(SVN_BASE_PROTOCOL_PLAN + 40)
#define	SVT_PLN_ANGIO_OPTION		VFT_INT32
#define	SVS_PLN_ANGIO_OPTION		"Angiography Options"
/*	�A���M�I�̃I�v�V�������w�肷��t�B�[���h V2.0 ���� SVN_PLN_APP_OPTION
	�ɓ������ꂽ�B

	This indicates the type of angiographic protocol to be executed.  This
	information originates in the sequence database.  The contents of this
	field should be one of the values defined in vf_appcodes.h for this
	field.
	Written by proEdit.
	*/

#define	SVN_PLN_SE_OPTION		(SVN_BASE_PROTOCOL_PLAN + 41)
#define	SVT_PLN_SE_OPTION		VFT_BITFIELD
#define	SVS_PLN_SE_OPTION		"Spin Echo Options"
/*	SE �̃I�v�V�������w�肷��t�B�[���h V2.0 ���� SVN_PLN_APP_OPTION ��
	�������ꂽ�B

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_RES			(SVN_BASE_PROTOCOL_PLAN + 42)
#define	SVT_PLN_RES			VFT_FLTVEC2
#define	SVS_PLN_RES			"Resolving power of protocol"
/*	�𑜓x�B

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
/*	�V�[�P���X�N���X�BV2.0�܂ł� 2D,3D �̂Q�킾�������AV2.0+���ȍ~�ł�
	�V�[�P���X�했�ɐݒ�B

	This describes the class of the sequence for prolim tree restructuring.
	When prolim switches between sequences of the same class prolim can
	successfully restructure the tree.
	*/

#define SVN_PLN_NUM_FSE_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 44)
#define SVT_PLN_NUM_FSE_ECHOES		VFT_INT32
#define SVS_PLN_NUM_FSE_ECHOES		"FSE echo train length"
/*	FastSE �̃G�R�[�g���C���B

	This describes the number of echoes acquired at one time for a FSE
	sequence.  It has no direct connection with the number of images
	produced per slice.
	*/

#define	SVN_PLN_APP_OPTION		(SVN_BASE_PROTOCOL_PLAN + 45)
#define	SVT_PLN_APP_OPTION		VFT_INT32
#define	SVS_PLN_APP_OPTION		"Application Options"
/*	�A�v���P�[�V�����I�v�V�����B�V�[�P���X��ؑւ��邽�߂̏����B
	�]���� SE_OPTIONS, ANGIO_OPTIONS �̂悤�ɃV�[�P���X���ɃI�v�V������
	�t�B�[���h��ێ����Ă����� V2.0 ����P�ɓ������ꂽ�B���̂��Ƃ�
	���V�K�V�[�P���X��ǉ������ꍇ�ł��I�v�V�����̒ǉ���p�ӂɍs�Ȃ�
	���Ƃ��ł���B

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_APP_CONDITION		(SVN_BASE_PROTOCOL_PLAN + 46)
#define	SVT_PLN_APP_CONDITION		VFT_BITFIELD
#define	SVS_PLN_APP_CONDITION		"Application Condition"
/*	�A�v���P�[�V�����R���f�B�V�����B���[�U�� proEdit ��Ŏw�肷��B
	���̒l�� seqgen ���Q�Ƃ��邱�Ƃɂ��O������ seqgen �ɑ΂���
	������n�����Ƃ��ł���B���̃t�B�[���h�̋�̓I�Ȏg�p���@�͂�
	�V�[�P���X�ɂ���ĂɌ��߂���B���̂��ߎ�Ɏ����I�ȗv�f�Ŏg
	�p�����B

	This indicates the type of spin echo protocol to be executed.  This
	information originates in the sequence database.  This is a bit field.
	The bit masks for this field are defined in vf_appcodes.h.
	Written by proEdit.
	*/

#define	SVN_PLN_FLOP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 47)
#define	SVT_PLN_FLOP_ANGLE		VFT_INT32
#define	SVS_PLN_FLOP_ANGLE		"Flop angle for field echo sequences"
/*	�t���b�v�p
	
	This field is the flop angle in degrees.  This quantity is selected by
	the operator in ProEdit.
	*/

#define SVN_PLN_NUM_DUMMY_SHOT		(SVN_BASE_PROTOCOL_PLAN + 48)
#define SVT_PLN_NUM_DUMMY_SHOT		VFT_INT32
#define SVS_PLN_NUM_DUMMY_SHOT		"The number of dummy shot"
/*	��ł��񐔁B
	*/

#define SVN_PLN_PE_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 49)
#define SVT_PLN_PE_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_PE_FLOW_ENCODE_VALUE	"Flow encode value(PE)"
/*	�o�r-�l�q�`�ł̃t���[�G���R�[�h�ʁi�o�d�����j
	���[�U�� proEdit �Őݒ肷��B
	*/

#define SVN_PLN_RO_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 50)
#define SVT_PLN_RO_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_RO_FLOW_ENCODE_VALUE	"Flow encode value(RO)"
/*	�o�r-�l�q�`�ł̃t���[�G���R�[�h�ʁi�q�n�����j
	���[�U�� proEdit �Őݒ肷��B
	*/

#define SVN_PLN_SE_FLOW_ENCODE_VALUE	(SVN_BASE_PROTOCOL_PLAN + 51)
#define SVT_PLN_SE_FLOW_ENCODE_VALUE	VFT_FLT32
#define SVS_PLN_SE_FLOW_ENCODE_VALUE	"Flow encode value(SE)"
/*	�o�r-�l�q�`�ł̃t���[�G���R�[�h�ʁi�r�d�����j
	���[�U�� proEdit �Őݒ肷��B
	*/

#define SVN_PLN_IR_PULSE		(SVN_BASE_PROTOCOL_PLAN + 52)
#define SVT_PLN_IR_PULSE		VFT_INT32
#define SVS_PLN_IR_PULSE		"Inversion Recovery Pulse (on/off)"
/*	�C���o�[�W�����p���X�̂n�m�^�n�e�e���L�q����t�B�[���h�B

	1994.10.06
	�ǉ��FIR�̃��[�h���S���łR�z�肳���BR�P�ł́ATI=20-40ms ��
	�Ή�����P���t���p�^�[����p���邪�ATI��TR�̊֌W��link�\�����ω�
	���󂯂�B
	TI, TR, TE �Ɉˑ����A���[�h�ɂ��X���C�X�����̌v�Z�@�ɉe����^����B
	���݂́ABool �Œ�`����Ă��邽��int32 �ɕύX����B
	VFC_IR_PULSE_NONE	0
	VFC_IR_PULSE_FMM	1 : Frequency Multiplexed Method (TAMI����)
	VFC_IR_PULSE_STIR	2 : Time Multiplexed Method 
	VFC_IR_PULSE_FLAIR	3 : 	�V
	VFC_IR_PULSE_IR2	4 : 	�V
	*/

#define SVN_PLN_TIME_CTRL_PARAM		(SVN_BASE_PROTOCOL_PLAN + 53)
#define SVT_PLN_TIME_CTRL_PARAM		VFT_TIME_CTRL_PARAM
#define SVS_PLN_TIME_CTRL_PARAM		"Time control parameters."
/*	���Ԏ������փX�L�������s�Ȃ��ꍇ�̃p�����[�^�B
	VFT_TIME_CTRL_PARAM �� vf_apptype.h �Œ�`����Ă���B
	*/

#define SVN_PLN_SCAN_ANATOMY		(SVN_BASE_PROTOCOL_PLAN + 54)
#define SVT_PLN_SCAN_ANATOMY		VFT_INT32
#define SVS_PLN_SCAN_ANATOMY		"Scan anatomy"
/*	���ʃR�[�h���L�q����t�B�[���h�B���[�U���o�p�l�ɂĎw�肷��B
	�r�`�q�v�Z�Ɏg�p�����B
	*/

#define SVN_PLN_TIME_DIM_SCAN		(SVN_BASE_PROTOCOL_PLAN + 55)
#define SVT_PLN_TIME_DIM_SCAN		VFT_BOOL
#define SVS_PLN_TIME_DIM_SCAN		"Time dimension scan flag"
/*	���Ԏ������ւ̃X�L���������邩�ǂ����̃t���O�B���݁A���Ԏ������ւ�
	�X�L�������s�Ȃ����̂́A�_�C�i�~�b�N�X�L�����ƃQ�[�g�X�L�����ł̃V
	�l�݂̂ł���B
	*/

#define SVN_PLN_RECOVERY_TIME		(SVN_BASE_PROTOCOL_PLAN + 56)
#define SVT_PLN_RECOVERY_TIME		VFT_FLT32
#define SVS_PLN_RECOVERY_TIME		"Recovery Time"
/*	���J�o���[�^�C���iFastFE �ɂĎg�p�����j�B
	���[�U�� proEdit �Őݒ肷��B�Q�c�ł͏�ɂO�ƂȂ�B
	*/

#define SVN_PLN_3D_EXTRA_SLICES		(SVN_BASE_PROTOCOL_PLAN + 57)
#define SVT_PLN_3D_EXTRA_SLICES		VFT_INT32
#define SVS_PLN_3D_EXTRA_SLICES		"Extra slices for 3D"
/*	�R�c�̐܂�Ԃ��΍��p�̃t�B�[���h�B���[�U�w�薇���̂P������
	�Ŏ��W���č\����ɗ]��X���C�X���폜��������B�X���C�X�ԍ�
	�̎Ⴂ������폜���閇�����w�肷��B
		D[0] = �폜�����i�X���C�X�J�n�[�j
		D[1] = �폜�����i�X���C�X�I�[�j
	*/

#define SVN_PLN_DYNAMIC_SCAN		(SVN_BASE_PROTOCOL_PLAN + 58)
#define SVT_PLN_DYNAMIC_SCAN		VFT_BOOL
#define SVS_PLN_DYNAMIC_SCAN		"Dynamic scan flag"
/*	�_�C�i�~�b�N�X�L���������邩�ǂ����̃t���O�B�_�C�i�~�b�N�X
	�L�����̏ꍇ���Ԏ������̃X�L����������Ƃ����t���O
		SVN_PLN_TIME_CTRL_PARAM
	�������ɐݒ肳���B
	*/

#define SVN_PLN_QUAD_MUX		(SVN_BASE_PROTOCOL_PLAN + 59)
#define SVT_PLN_QUAD_MUX		VFT_INT32
#define SVS_PLN_QUAD_MUX		"Quad scan mux"
/*	QUAD SCAN �ł� MUX�B
	�P��̗�N�p���X�łƂ�閇��
	*/

#define SVN_PLN_SELECTED_PRESAT_THK	(SVN_BASE_PROTOCOL_PLAN + 60)
#define SVT_PLN_SELECTED_PRESAT_THK	VFT_FLT32
#define SVS_PLN_SELECTED_PRESAT_THK	"Selected Presat Thickness"
/*	�v���g�R���m�[�h�ɋL�q�����v���T�`�����[�V�������B���̒l��
	�r�W���A���O���[�v�A�v���T�b�g�m�[�h�ɃR�s�[�����B�v������
	���V�K�ɃT�`�����[�V������ǉ�����ۂɂ͂��̒l���g�p����B
  	*/

#define SVN_PLN_AFI_MODE		(SVN_BASE_PROTOCOL_PLAN + 61)
#define SVT_PLN_AFI_MODE		VFT_BITFIELD
#define SVS_PLN_AFI_MODE		"flag for AFI mode (axis and direction)"
/*	AFI �č\���̗L���ƁA�K�p����鎲���w�肷��t���O�B���ɂ��Ă� Bit Or ��
        �I�����s�Ȃ��B
	VFC_AFI_PE_HEAD		1
        VFC_AFI_PE_TAIL		2
	VFC_AFI_RO_HEAD		4
	VFC_AFI_RO_TAIL 	8
	VFC_AFI_SE_HEAD 	16
	VFC_AFI_SE_TAIL 	32
	�v�f�Q�̂P�����z��Ƃ��A�f���A���R���g���X�g�ɑΉ�����B
  	*/

#define SVN_PLN_AFI_IMAGE_MODE		(SVN_BASE_PROTOCOL_PLAN + 62)
#define SVT_PLN_AFI_IMAGE_MODE		VFT_INT32
#define SVS_PLN_AFI_IMAGE_MODE		"Imaging mode (negative pixcel on/off) flag"
/*	AFI �č\���̍ہA���̉摜�l��\������t���O�B
	0: ���̒l�Ƃ��ĕ\��
	1: ��Ɋۂ߂�
	2: ��Βl���Ƃ萳�̒l�Ƃ��ĕ\��
	�v�f�Q�̂P�����z��Ƃ��A�f���A���R���g���X�g�ɑΉ�����B
	*/


#define SVN_PLN_ECHO_FACTOR		(SVN_BASE_PROTOCOL_PLAN + 63)
#define SVT_PLN_ECHO_FACTOR		VFT_INT32
#define SVS_PLN_ECHO_FACTOR		"Echo Factor"
/*	�����X�L�����iFastSE, MultiShot EPI, GRASE)�ɂ������G�R�[��
	AFI �𕹗p���Ȃ��ꍇ�A
	GRASE : numEchoFactor = numRF_Echoes * numEPI_Echoes
	EPI   : numEchoFactor = numEPI_Echoes
	FastSE: numEchoFactor = numRF_Echoes
	AFI ���g�p�����ꍇ�A
	��L�ɂ��A���߂���{���ɂ����AFI �ɂ��ʑ��G���R�[�h������
	����������镔�����܂ށB�Ⴆ�΁AnumRF_Echoes =3 �ŁAAFI��g��
	���킹���numEchoFactor = 5 �ƂȂ�B 
	*/

#define SVN_PLN_NUM_ECHO_FACTOR_SHOTS	(SVN_BASE_PROTOCOL_PLAN + 64)
#define SVT_PLN_NUM_ECHO_FACTOR_SHOTS	VFT_INT32
#define SVS_PLN_NUM_ECHO_FACTOR_SHOTS	"number of echo factor shots"
/*	  �����X�L�����iFastSE, MultiShot EPI, GRASE)�ɂ��ʑ��G���R�[�h
	�V���b�g���B�������AnumDummyShot �� numTemplateShot ���܂܂Ȃ�
	���ۂ̉摜�č\���ɗp������V���b�g�B
	vf.PeFull( �ʑ��G���R�[�h�}�g���b�N�X�T�C�Y��NoWrapParameter)
	�� vf.EchoFactor ��p���āA
	vf.EchoFactorShot = floor (vf.PeFull/ vf.EchoFactor)
	if (vf.EchoFactorShot % 2 != 0 && vf.Nex % 2 != 0 ) vf.EchoFactorShot
	�܂��ADC�A�[�`�t�@�N�g�΍�̂��߁A��A�x���[�W�̍ۂɂ̓V���b�g����
	�����ɑI�ԕK�v������B
	  FASE �̏ꍇ�A�]���Ƃ͋t��numShots ���A���[�U�[�w��p�����[�^�Ƃ��A
	numRF_Echoes �̕����}�g���b�N�X�T�C�Y�Ɉˑ�����U���p�����[�^�Ƃ���B
	*/

#define SVN_PLN_NUM_FASE_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 65)
#define SVT_PLN_NUM_FASE_ECHOES		VFT_INT32
#define SVS_PLN_NUM_FASE_ECHOES		"number of FASE echoes"
/*	FASE�X�L�����ɂ��G�R�[���B�R���g���X�g�����肷��p�����[�^�ł���X�L����
	���Ԃ����E����numRF_Echoes �Ƃ͕ʁB
	*/

#define SVN_PLN_NUM_RF_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 66)
#define SVT_PLN_NUM_RF_ECHOES		VFT_INT32
#define SVS_PLN_NUM_RF_ECHOES		"number of RF Echoes"
/*	Hybrid EPI, FastSE, FASE �ɂ�����RF �G�R�[���B�݊����̂��߂ɐV�݁B
	*/

#define SVN_PLN_NUM_ECHO_TRAIN		(SVN_BASE_PROTOCOL_PLAN + 67)
#define SVT_PLN_NUM_ECHO_TRAIN		VFT_INT32
#define SVS_PLN_NUM_ECHO_TRAIN		"number of echo train"
/*	numEchoTrain = numRF_Echoes* numEPI_Echoes
	�Œ�`�����}���`�V���b�g�n�����X�L�����ɂ��g�[�^���̃G�R�[��
	*/

#define SVN_PLN_MAX_IMAGING_PULSES	(SVN_BASE_PROTOCOL_PLAN + 68)
#define SVT_PLN_MAX_IMAGING_PULSES	VFT_INT32
#define SVS_PLN_MAX_IMAGING_PULSES	"max imaging pulses per coverage"
/*	�P�X�L�����i�J�o���[�W�j�łƂ蓾��ő�X���C�X���i�X���u�j���B
	*/

#define SVN_PLN_NUM_EPI_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 69)
#define SVT_PLN_NUM_EPI_ECHOES		VFT_INT32
#define SVS_PLN_NUM_EPI_ECHOES		"number of EPI echoes"
/*	(MultiShot)EPI�X�L�����ɂ��G�R�[���B
	��`����Ȃ����͂P�inumRF_Echoes �����l�j
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
/*	�f�B�X�v���C�ŕ\������V�[�P���X�I�v�V�������L�q����B���e��
	���W�����ɕ����� prolim ���쐬����B
	*/

#define SVN_PLN_NUM_PS_ECHOES		(SVN_BASE_PROTOCOL_PLAN + 77)
#define SVT_PLN_NUM_PS_ECHOES		VFT_INT32
#define SVS_PLN_NUM_PS_ECHOES		"The number of PS echoes"
/*	PSMRA �̃G�R�[��(�Q�_�@�^�S�_�@)���L�q����B
	*/

#define SVN_PLN_SLICE_OVERWRAP		(SVN_BASE_PROTOCOL_PLAN + 78)
#define SVT_PLN_SLICE_OVERWRAP		VFT_INT32
#define SVS_PLN_SLICE_OVERWRAP		"Slice Overwarp for MRA 3D multi Cov."
/*	MRA3D �}���`�J�o���[�W�I�[�o�[���b�v�ʁB
	*/

#define SVN_PLN_AFI_DIRECTION		(SVN_BASE_PROTOCOL_PLAN + 79)
#define SVT_PLN_AFI_DIRECTION		VFT_INT32
#define SVS_PLN_AFI_DIRECTION		"flag for AFI mode (axis and direction)"
/*	AFI �č\���̗L���ƁA�K�p����鎲���w�肷��t���O�B���ɂ��Ă� Bit Or ��
        �I�����s�Ȃ��B
	VFC_AFI_PE		0x03
	VFC_AFI_RO		0x0C
	VFC_AFI_SE		0x30
	�v�f�Q�̂P�����z��Ƃ��A�f���A���R���g���X�g�ɑΉ�����B
  	*/

#define SVN_PLN_MIN_TR		(SVN_BASE_PROTOCOL_PLAN + 80)
#define SVT_PLN_MIN_TR		VFT_FLT32
#define SVS_PLN_MIN_TR		"Minimu TR"
/*	�ŏ�TR�i�T�`�����[�V�������܂ށj
	*/

#define SVN_PLN_SELECTED_TAG_THK	(SVN_BASE_PROTOCOL_PLAN + 81)
#define SVT_PLN_SELECTED_TAG_THK	VFT_FLT32
#define SVS_PLN_SELECTED_TAG_THK	"Selected Tag Thickness"
/*	�v���g�R���m�[�h�ɋL�q�����^�O���B���̒l���r�W���A���O���[�v�A
	�v���T�b�g�m�[�h�ɃR�s�[�����B�v���������V�K�Ƀ^�O��ǉ�����ۂɂ�
	���̒l���g�p����B
  	*/

#define SVN_PLN_VARI_FLIP_FLAG		(SVN_BASE_PROTOCOL_PLAN + 82)
#define SVT_PLN_VARI_FLIP_FLAG		VFT_BOOL
#define SVS_PLN_VARI_FLIP_FLAG		"Variable Flip Angle Flag"
/*	FFE�ł̃o���A�u���t���b�v�A���O���̂��߂̃t���O
	���̃t���O���^�̏ꍇ�� SVN_PLN_FLIP_ANGLE �̈Ӗ����ς��B
	SVN_PLN_FLIP_MIN_ANGLE < x < SVN_PLN_FLIP_ANGLE
	�ł̃o���A�u���t���b�v�A���O���ƂȂ�BSAR�̓t���b�v�p�ő�l��
	�v�Z���邽�߁ASAR�֘A�̃\�[�X�R�[�h���C������K�v�͖����B
	*/

#define	SVN_PLN_FLIP_MIN_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 83)
#define	SVT_PLN_FLIP_MIN_ANGLE		VFT_INT32
#define	SVS_PLN_FLIP_MIN_ANGLE		"Flip angle for field echo sequences"
/*	�o���A�u���t���b�v�A���O���w�莞�̃t���b�v�p�ŏ��l
	*/

#define SVN_PLN_NUM_EPI_TE_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 84)
#define SVT_PLN_NUM_EPI_TE_ECHOES	VFT_INT32
#define SVS_PLN_NUM_EPI_TE_ECHOES	"Number of contrast ETL for single shot EPI (SE and FE)"
/*	AFI�č\���̂��߂̃p�����[�^�ƁA�V�[�P���X�g�ݗ��Ă̂��߂�
	�p�����[�^�����˂�Bk��Ԓ��S�܂ł�ETL(�G�R�[�g���C����)��
	�w��B���̃p�����[�^��PE�����̃}�g���b�N�X�T�C�Y�A�V���b�g���A
	NoWrap����NUM_EPI_ECHOES���Z�o����B
	This is parameter for AFI recon and sequence stracture.
	Echo train length of first to k space center.
	*/

#define SVN_PLN_SNAP_SHOT_MODE		(SVN_BASE_PROTOCOL_PLAN + 85)
#define SVT_PLN_SNAP_SHOT_MODE		VFT_INT32
#define SVS_PLN_SNAP_SHOT_MODE		"Flag for snap shot mode"
/*	Dynamic+TR>=WaitTime�̎��A���[�U�[���̓p�����[�^�Ƃ��āA
	�_�~�[�V���b�g�A�e���v���[�g�V���b�g���Ɏw�肷�邽�߂�
	�t���O�B

	When dynamic scan and wait time < TR, this switch provide
	mode select dummy and template shot are zero or default 
	values. 
	*/

#define SVN_PLN_SKIP_SAT_FLAG		(SVN_BASE_PROTOCOL_PLAN + 86)
#define SVT_PLN_SKIP_SAT_FLAG		VFT_INT32
#define SVS_PLN_SKIP_SAT_FLAG		"Flag for skipping sat/fat"
/*	�X�L�b�s���O�T�`�����[�V�����t���O
 */

#define SVN_PLN_SKIP_SAT_INTERVAL	(SVN_BASE_PROTOCOL_PLAN + 87)
#define SVT_PLN_SKIP_SAT_INTERVAL	VFT_FLT32
#define SVS_PLN_SKIP_SAT_INTERVAL	"Interval time for skipping saturation"
/*	�L���v���T�`�����[�V�����p���X���ԁB
	�v���T�`�����[�V�����p���X�𗣂����Ƃ��ł�����E���ԁB�i�P��:�b�j
	*/

#define SVN_PLN_SKIP_FAT_INTERVAL	(SVN_BASE_PROTOCOL_PLAN + 88)
#define SVT_PLN_SKIP_FAT_INTERVAL	VFT_FLT32
#define SVS_PLN_SKIP_FAT_INTERVAL	"Interval time for skipping fatsat"
/*	�L���t�@�b�g�T�b�g�p���X���ԁB
	�t�@�b�g�T�b�g�p���X�𗣂����Ƃ��ł�����E���ԁB�i�P��:�b�j
	*/

#define	SVN_PLN_BBTI			(SVN_BASE_PROTOCOL_PLAN + 89)
#define	SVT_PLN_BBTI			VFT_FLT32
#define	SVS_PLN_BBTI			"Black Blood Inversion Time, TI"
/*	�u���b�O�u���b�h�C���o�[�W�����p���X��t������ۂ�TI����(bbTI)���Ԃ�
	�L�q����B�P�ʂ͕b�B
	*/

#define SVN_PLN_BBIR_PULSE		(SVN_BASE_PROTOCOL_PLAN + 90)
#define SVT_PLN_BBIR_PULSE		VFT_INT32
#define SVS_PLN_BBIR_PULSE		"BB Inversion Recovery Pulse (on/off)"
/*	�u���b�O�u���b�b�h�C���o�[�W�����p���X�̂n�m�^�n�e�e���L�q����t�B�[���h�B
	*/

#define SVN_PLN_INV_PULSE_TYPE	(SVN_BASE_PROTOCOL_PLAN + 91)
#define SVT_PLN_INV_PULSE_TYPE	VFT_INT32
#define SVS_PLN_INV_PULSE_TYPE	"Inversion Recovery Pulse Type"
/*	�C���o�[�W�����p���X�̌^���L�q����t�B�[���h�B
	0:	SINC
	1:	ADIABATIC
	*/
	
#define	SVN_PLN_PRESAT_FAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 92)
#define	SVT_PLN_PRESAT_FAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_PRESAT_FAT_FLIP_ANGLE	"Flip angle for presat (fat)"
/*	�v���T�`�����[�V����(���b)�t���b�v�A���O��
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
/*	PROLIM �łr�`�q�v�Z���s�Ȃ����ǂ����������t�B�[���h�B
	PQM���L�q����BAPC�ȑO�ł���΋U�BAPC�Ȍ��PROLIM���N�������
	�ꍇ���̃t�B�[���h��^�ɂ��邱�Ƃłr�`�q�����������v�����ƂȂ�B
	*/

#define SVN_PLN_STATUS			(SVN_BASE_PROTOCOL_PLAN + 102)
#define SVT_PLN_STATUS			VFT_BITFIELD
#define SVS_PLN_STATUS			"Plan Status"
/*	���W�����ҏW�̂��߂ɋN�����ꂽ�v���Z�X�^�e�摜�̗L����o�^����B

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
/*	�悉�R���g���X�g�̉摜�Ɏg�p����G�R�[�̂����A�X�L�����ɂ����
	���ۂɎ��W����G�R�[�̐��B
	Dual contrast scan�̂��Ƃ��l�����āA�v�f���͂Q�Ƃ���B
	*/

#define SVN_PLN_NUM_ECHO_AFI_FILLED	(SVN_BASE_PROTOCOL_PLAN + 104)
#define SVT_PLN_NUM_ECHO_AFI_FILLED	VFT_INT32
#define SVS_PLN_NUM_ECHO_AFI_FILLED	"Number of echoes filled by AFI recon"
/*	�悉�R���g���X�g�̉摜�Ɏg�p����G�R�[�̂����AAFI�����ɂ���ď[�U����
	�G�R�[�̐��BDual contrast scan�̂��Ƃ��l�����āA�v�f���͂Q�Ƃ���B
	*/

#define	SVN_PLN_NUM_ECHO_SHARED		(SVN_BASE_PROTOCOL_PLAN + 105)
#define SVT_PLN_NUM_ECHO_SHARED		VFT_INT32
#define SVS_PLN_NUM_ECHO_SHARED		"Number of echoes shared by i th and i+1 th contrast image"
/*	�悉�R���g���X�g�Ƒ悉+1�R���g���X�g�̉摜�ŋ��p����G�R�[�̐��B
	����ł͊e�R���g���X�g�̉摜�̂����A����TE�̖ʂŗ׍���Ȃ��摜���m�ł�
	�G�R�[�f�[�^�̋��p�͖������̂Ƃ���B
	*/

#define SVN_PLN_IR_EXCITED_SLICES	(SVN_BASE_PROTOCOL_PLAN + 106)
#define SVT_PLN_IR_EXCITED_SLICES	VFT_INT32
#define SVS_PLN_IR_EXCITED_SLICES	"Actual excitaion number of slices on FLAIR mode"
/*	�X���C�X�I�t�Z�b�g���[�h�Ŏ��s�����IR, FLAIR �Ȃǂɂ����āA
	�f�[�^���W�̑��ɗ]���ȃX���C�X�̗�N���s�Ȃ����ARF�p���X��
	�X���C�X�Ԋ��x������ጸ���邽�߁AOff�ł����ASAR �I�ɂ͗]����
	�X���C�X������SAR�v�Z�ɎZ������K�v������B
	*/

#define SVN_PLN_IR_PULSE_BAND_WIDTH	(SVN_BASE_PROTOCOL_PLAN + 107)
#define SVT_PLN_IR_PULSE_BAND_WIDTH	VFT_INT32
#define SVS_PLN_IR_PULSE_BAND_WIDTH	"RF band width for IR pulse."
/*	IR, FLAIR ���[�h�ɂ����ẮAIR �p���X�̑ш敝�ɂ��MTC 
	���ʂ��ω����A�摜�R���g���X�g�ɉe����^����B����ɁA
	narrow band ���������邽�߂ɁA90+90 �̂Q��������������
	�����e��IR�p���X�g�`���A���[�U�[�C���^�[�t�F�C�X����
	�w��\�Ƃ���B
	*/

#define SVN_PLN_AUTO_VOICE	(SVN_BASE_PROTOCOL_PLAN + 108)
#define SVT_PLN_AUTO_VOICE	VFT_INT32
#define SVS_PLN_AUTO_VOICE	"The flag for auto voice."
/*	FASE3D,Dyanmic�B�e�ł̃I�[�g�{�C�X�g�p�̗L����\���B
	FASE3D�ł́A���������̂��߂̏\���Ȏ��ԓI�]�T���Ȃ��ꍇ�͎����I��
	OFF�ƂȂ�B
	0:�I�[�g�{�C�XOFF
	1:���~�߉����P		�u�����z���Ď~�߂Ă��������v
	2:���~�߉����Q		�u����f���Ď~�߂Ă��������v
	3:���~�߉����R		�u�����z���ēf���Ď~�߂Ă��������v
	4:�̓��h�~����		�u�����Ȃ��ł��������v
	*/

#define SVN_PLN_AUTO_VOICE_TPCTRL	(SVN_BASE_PROTOCOL_PLAN + 109)
#define SVT_PLN_AUTO_VOICE_TPCTRL	VFT_INT32
#define SVS_PLN_AUTO_VOICE_TPCTRL	"The flag for auto voice with TP CTRL."
/*	FASE3D,Dyanmic�B�e�ł�TP���荞�݂ɗv�I�[�g�{�C�X�g�p�̗L����\���B
	������� PLN_AUTO_VOICE ���g�p����B
	*/

#define SVN_PLN_TAG_IR_THICKNESS	(SVN_BASE_PROTOCOL_PLAN + 110)
#define SVT_PLN_TAG_IR_THICKNESS	VFT_FLT32
#define SVS_PLN_TAG_IR_THICKNESS	"TAG IR Thickness for ASL"	
/*	ASL�ł�TAG IR�̌���
 */

#define SVN_PLN_CONTROL_IR_OFFSET	(SVN_BASE_PROTOCOL_PLAN + 111)
#define SVT_PLN_CONTROL_IR_OFFSET	VFT_FLT32
#define SVS_PLN_CONTROL_IR_OFFSET	"CONTROL IR Thickness for ASL"	
/*	ASL�ł�CONTROL IR���S�ƃX���C�X���S�Ƃ̋���
 */

#define SVN_PLN_TAG_IR_OFFSET	(SVN_BASE_PROTOCOL_PLAN + 112)
#define SVT_PLN_TAG_IR_OFFSET	VFT_FLT32
#define SVS_PLN_TAG_IR_OFFSET	"TAG IR Thickness for ASL"	
/*	ASL�ł�TAG IR���S�ƃX���C�X���S�Ƃ̋���
 */

#define SVN_PLN_PREP_SCAN	(SVN_BASE_PROTOCOL_PLAN + 113)
#define SVT_PLN_PREP_SCAN	VFT_INT32
#define SVS_PLN_PREP_SCAN	"Prep Scan Flag"
/*	PrepScan ���g�p���邩�ǂ�����\���B
 */

#define SVN_PLN_RF_SPOILING	(SVN_BASE_PROTOCOL_PLAN + 114)
#define SVT_PLN_RF_SPOILING	VFT_INT32
#define SVS_PLN_RF_SPOILING	"RF Spoiling flag"
/*	FE�AFFE�ATOF,PSMRA�V�[�P���X��T1�R���g���X�g������������
	�����R���g���X�g���グ��@�\�Ƃ���RF Spoiling�Ƃ����@�\��
	�g�p���Ă���B
	�������A���̋@�\��p�����T2�l�̒����]�Ґ��t�̐M���͒ቺ����
	���܂��̂ŁA��������Ώۂ��]�Ґ��t�ł���ꍇ�ɂ́ASpoiling��
	�I�t�ɂ��邱�Ƃ��K�v�B
	�܂��AFE�V�[�P���X�̏ꍇ�AT2�������V�[�P���X�ł�Spoiling��
	�I�t�ɂ������B

	�O �� Spoiling�̓I�t�B
	�P �� Spoiling�̓I���B
	*/

#define SVN_PLN_INTERMIT_FLAG	(SVN_BASE_PROTOCOL_PLAN + 115)
#define SVT_PLN_INTERMIT_FLAG	VFT_INT32
#define	SVS_PLN_INTERMIT_FLAG	"Intermittent breath hold"
/*	�Ԍ����~�߂��w�肷��B
	*/

#define SVN_PLN_INTERMIT_REPEAT	(SVN_BASE_PROTOCOL_PLAN + 116)
#define SVT_PLN_INTERMIT_REPEAT	VFT_INT32
#define SVS_PLN_INTERMIT_REPEAT	"Slice encode before intermittent breath hold"
/*	�Ԍ����~�߂̑O�ɌJ��Ԃ��X���C�X�G���R�[�h���B
	SPEED�̏ꍇ�́A�d�Ȃ荇���Q�X���C�X���P�X���C�X�G���R�[�h��
	�l����B
	*/

#define SVN_PLN_INTERMIT_TIME	(SVN_BASE_PROTOCOL_PLAN + 117)
#define SVT_PLN_INTERMIT_TIME	VFT_INT32
#define SVS_PLN_INTERMIT_TIME	"Rest time for intermittent breath hold"
/*	�Ԍ����~�߂��s���x�~���Ԃ��w�肷��B�P�ʂ͕b�B
	*/

#define SVN_PLN_T2_PLUS		(SVN_BASE_PROTOCOL_PLAN + 118)
#define SVT_PLN_T2_PLUS		VFT_INT32
#define SVS_PLN_T2_PLUS		"T2 Plus"
/*	T2 Plus��ON/OFF���w�肷��B

	T2 Plus�̓f�[�^�����W������ɁA���Ԃ������ďc�����ւ̉񕜂�
	�҂̂ł͂Ȃ��A90��RF�p���X��������邱�ƂŁA�����I�ɏc�����ւ�
	�񕜂𑣂����@�ł���B�����I�ɉ񕜂����邱�ƂŁA�J�Ԃ����Ԃ�
	�Z�����Ă�T1�AT2�̒��������̐M�������߂邱�Ƃ��ł��AT2�����B���ł�
	���ԒZ�k��}�邱�Ƃ��ł���B
	T2 Plus���́A�G�R�[�n��̃f�[�^�����W������ɒu����A������180��
	RF�p���X�ƂP�� 90��RF�p���X�ō\�������B
 */

#define SVN_PLN_PRECONTRAST_TE		(SVN_BASE_PROTOCOL_PLAN + 119)
#define SVT_PLN_PRECONTRAST_TE		VFT_FLT32
#define SVS_PLN_PRECONTRAST_TE		"Precontrast pulse TE"
/* Node:SVN_ST_PROTOCOL
	�v���R���g���X�g(PreC)�p���X��t������ۂ�TE���Ԃ̑�����
    (PreCTI)���L�q����B�P�ʂΕb�B*/

 
#define SVN_PLN_B_VALUE		(SVN_BASE_PROTOCOL_PLAN + 120)
#define SVT_PLN_B_VALUE		VFT_INT32
#define SVS_PLN_B_VALUE		"B Value"
/* Node:SVN_ST_PROTOCOL
	�����ɂ��ʑ��V�t�g�̓x����\���p�����[�^�ŁA$[s/m^2]$�̒P��
    �ŕ\�������́B������SVN_PLN_IMAGE_PATTERN�ɂ���Č��肷��B*/

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
/*	�R�����g�B

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

	acqman �̎B�e���[�h�����肷��B
		VFC_SCAN_HOLD :  �v���X�L�����I����A�{�X�L�����̑O�ňꎞ��~�B
				�h���~�ߎB�e�h�p�B
		VFC_RFL_HOLD  :  �`�o�b���O�ňꎞ��~�B�h���~�߂`�o�b�h�p�B
		VFC_CFA_HOLD  :  �b�e�`���O�ňꎞ��~�B
		VFC_RGN_HOLD  :  �q�f�m���O�ňꎞ��~�B
	
	�e���[�h�̒l�͈ȉ��̒ʂ�
		VFC_SCAN_HOLD	= 1
		VFC_RFL_HOLD	= 2
		VFC_CFA_HOLD	= 4
		VFC_RGN_HOLD	= 8
	*/

#define	SVN_PLN_RECVR_COIL		(SVN_BASE_PROTOCOL_PLAN + 206)
#define	SVT_PLN_RECVR_COIL		VFT_INT32
#define	SVS_PLN_RECVR_COIL		"Receiver coil Database Key"
/*	��M�R�C���B

	This field describes which receiver coil is used in acquiring this
	protocol.  The contents of this field are selected by the operator
	in the PQM only.
	*/

#define	SVN_PLN_PATIENT_ORIENT		(SVN_BASE_PROTOCOL_PLAN + 207)
#define	SVT_PLN_PATIENT_ORIENT		VFT_PATORI
#define	SVS_PLN_PATIENT_ORIENT		"Patient orientation in the magnet"
/*	���ґ}�������B

	This field describes the orientation of the patient relative to the
	magnet.  The contents of this field are selected by the operator in
	the PQM only.  The contents of the structure that makes up this
	field should be one of the values in vf_appcodes.h for this field.
	*/

#define	SVN_PLN_LOCATOR_IMAGES		(SVN_BASE_PROTOCOL_PLAN + 208)
#define	SVT_PLN_LOCATOR_IMAGES		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGES		"1st locator image used for this protocol"
/*	�Q�Ɖ摜�B

	This field is an array of paths to the image used as locator
	for this protocol in the 1st viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/


#define	SVN_PLN_TRANSMIT_COIL		(SVN_BASE_PROTOCOL_PLAN + 209)
#define	SVT_PLN_TRANSMIT_COIL		VFT_INT32
#define	SVS_PLN_TRANSMIT_COIL		"Transmiter Coil Database Key"
/*	���M�R�C���B

	This field describes which transmit coil is used in acquiring this
	protocol.  The contents of this field are selected by the operator
	in the PQM only.
	*/

#define	SVN_PLN_INV_FLIP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 210)
#define	SVT_PLN_INV_FLIP_ANGLE		VFT_INT32
#define	SVS_PLN_INV_FLIP_ANGLE		"Flip angle for inversion pulse"
/*	�C���o�[�W�����p���X�t���b�v�p

	This is the flip angle in degrees applied to each inversion RF pulse.
	It is currently set by prolim or seqinit.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_2		(SVN_BASE_PROTOCOL_PLAN + 211)
#define	SVT_PLN_LOCATOR_IMAGE_2		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_2		"2nd locator image used for this protocol"
/*	�Q�}�C�ʒu���ߎ��̂Q���ڂ̎Q�Ɖ摜�B

	This field is an array of paths to the image used as a locator
	for this protocol in the 2nd viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_3		(SVN_BASE_PROTOCOL_PLAN + 212)
#define	SVT_PLN_LOCATOR_IMAGE_3		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_3		"3rd locator image used for this protocol"
/*	�Q���ʒu���ߎ��̂R���ڂ̎Q�Ɖ摜�B

	This field is an array of paths to the image used as locator
	for this protocol in the 3rd viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_4 	(SVN_BASE_PROTOCOL_PLAN + 213)
#define	SVT_PLN_LOCATOR_IMAGE_4		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_4		"4th locator image used for this protocol"
/*	���g�p�B

	This field is an array of paths to the image used as locator
	for this protocol in the 4th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_5		(SVN_BASE_PROTOCOL_PLAN + 214)
#define	SVT_PLN_LOCATOR_IMAGE_5		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_5		"5th locator image used for this protocol"
/*	���g�p�B

	This field is an array of paths to the image used as locator
	for this protocol in the 5th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define	SVN_PLN_LOCATOR_IMAGE_6		(SVN_BASE_PROTOCOL_PLAN + 215)
#define	SVT_PLN_LOCATOR_IMAGE_6		VFT_PATHELEM
#define	SVS_PLN_LOCATOR_IMAGE_6		"6th locator image used for this protocol"
/*	���g�p�B

	This field is an array of paths to the image used as locator
	for this protocol in the 6th viewport.  The contents of this
	field are written only by Display acting as the Graphic Locator.
	*/

#define SVN_PLN_FATSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 216)
#define SVT_PLN_FATSAT_PULSE		VFT_INT32
#define SVS_PLN_FATSAT_PULSE		"Fat Suppress Pulse (on/off)"
/*	�t�@�b�g�T�v���X�p���X�̂n�m�^�n�e�e���L�q����t�B�[���h�B
	
	VFT_BOOL �ł��ǂ����A�t�@�b�g�p���X�̊g���ɔ����Ă�����
	VFT_INT32 �Œ�`����B����(V2.1)�ł�

	0: �t�@�b�g�p���X OFF
	1: �t�@�b�g�p���X ON

	�Ƃ���B
	*/

#define	SVN_PLN_FATSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 217)
#define	SVT_PLN_FATSAT_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_FATSAT_FLIP_ANGLE	"Flip angle for fat-suppress pulse"
/*	�t�@�b�g�T�v���X�p���X�t���b�v�p

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
/*	SORS�T�v���X�p���X�̂n�m�^�n�e�e���L�q����t�B�[���h�B
	*/

#define	SVN_PLN_SORS_FLIP_ANGLE		(SVN_BASE_PROTOCOL_PLAN + 223)
#define	SVT_PLN_SORS_FLIP_ANGLE		VFT_INT32
#define	SVS_PLN_SORS_FLIP_ANGLE		"Flip angle for SORS pulse"
/*	SORS�p���X�t���b�v�p

	This is the flip angle in degrees applied to SORS RF pulse.
	It is currently set by prolim or seqinit.
	*/

#define	SVN_PLN_SHIM_ENABLE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 224)
#define	SVT_PLN_SHIM_ENABLE_FLAG	VFT_BOOL
#define	SVS_PLN_SHIM_ENABLE_FLAG	"Exist effective SHIM Data"
/*	����V�[�P���X�̎��b�}�����s�Ȃ����߂̗L���ȃV�~���O�f�[�^��
	���݂��邩�ǂ����̃t���O�B
	True  :  �L���ȃV�~���O�f�[�^�����݂���B
	False :  ���݂��Ȃ��B
	*/

#define SVN_PLN_ISCE_MODE		(SVN_BASE_PROTOCOL_PLAN + 225)
#define SVT_PLN_ISCE_MODE		VFT_INT32
#define SVS_PLN_ISCE_MODE		"ISCE Mode"
/*	ISCE ���[�h

	�W�����[�h	VFC_ISCE_MODE_NORMAL		0
	1:2		VFC_ISCE_MODE_1_2		1
	1:3		VFC_ISCE_MODE_1_3		2
	*/

#define	SVN_PLN_SCAN_COMMENTS		(SVN_BASE_PROTOCOL_PLAN + 226)
#define	SVT_PLN_SCAN_COMMENTS		VFT_STRING
#define	SVS_PLN_SCAN_COMMENTS		"Scan Comments"
/*	�X�L�����R�����g�B�o�p�l�Őݒ肷��X�L�����h�c�Ƃ͈قȂ�B
	*/

#define SVN_PLN_NUM_SEGMENTS		(SVN_BASE_PROTOCOL_PLAN + 227)
#define SVT_PLN_NUM_SEGMENTS		VFT_INT32
#define SVS_PLN_NUM_SEGMENTS		"The number of segments"
/*	�Z�O�����g��(�e�e�d) �B
	*/

#define SVN_PLN_SEGMENTATION_TYPE	(SVN_BASE_PROTOCOL_PLAN + 228)
#define SVT_PLN_SEGMENTATION_TYPE	VFT_INT32
#define SVS_PLN_SEGMENTATION_TYPE	"Type of FFE segmentation"
/*	�e�e�d�Z�O�����e�[�V�����^�C�v
	
	VFC_SEGMENT_TYPE_MOSAIC		0	(V4.04�ȍ~���g�p)
	VFC_SEGMENT_TYPE_INTERLEAVE	1
	VFC_SEGMENT_TYPE_SLICE_CENTRIC	2
	VFC_SEGMENT_TYPE_SEQUENTIAL	3
	VFC_SEGMENT_TYPE_SWIRL		4
	*/

#define SVN_PLN_SEGMENT_TIME		(SVN_BASE_PROTOCOL_PLAN + 229)
#define SVT_PLN_SEGMENT_TIME		VFT_FLT32
#define SVS_PLN_SEGMENT_TIME		"Time for segment (predicted)"
/*	�e�e�d�Z�O�����g�P�ʂ̎��W���ԁB
	*/

#define SVN_PLN_MTCSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 230)
#define SVT_PLN_MTCSAT_PULSE		VFT_INT32
#define SVS_PLN_MTCSAT_PULSE		"Number of MTC saturation pulses"
/*	0:������Ȃ�	�X�C�b�` Off
	#:�p���X��	MTC �p���X��
	MTC saturation �p���X�����������邱�ƂŁAMTC���ʂ������
	�L���I�ɃR���g���X�g�ɉe��������@�\�B
	*/

#define SVN_PLN_MTCSAT_OFFSET_FSY	(SVN_BASE_PROTOCOL_PLAN + 231)
#define SVT_PLN_MTCSAT_OFFSET_FSY	VFT_FLT32
#define SVS_PLN_MTCSAT_OFFSET_FSY	"Frequency offset for MTC saturation pulses"
/*	MTC���ʂ͒��S���g������̎��g����ɂ�����Ⴂ�ɂ���Ă��قȂ�B
	�\���Ƃ��Ă͗Ⴆ�΃�fmt��-1200Hz�����K�v�B
	*/

#define SVN_PLN_MTCSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 232)
#define SVT_PLN_MTCSAT_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_MTCSAT_FLIP_ANGLE	"Flip Angle for MTC saturation pulses"
/*	MTC���ʂ͂���FlipAngle�ɂ���Ă��قȂ邽�߁A
	�\���Ƃ��Ă�FAmt=1000deg�����K�v�B
	*/

#define SVN_PLN_NUM_SLABS		(SVN_BASE_PROTOCOL_PLAN + 233)
#define SVT_PLN_NUM_SLABS		VFT_INT32
#define SVS_PLN_NUM_SLABS		"The number of slabs"
/*	�X���u���B��Ƃ��ĂR�c�}���`�X���u�Ŏg�p����B
	*/


#define SVN_PLN_PE_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 234)
#define SVT_PLN_PE_MPG_GRAD VFT_FLT32
#define SVS_PLN_PE_MPG_GRAD "GRD strength of MPG along PE direction"
/*
	PE������MPG�p���X�̌X�Ύ��ꋭ�x��[Gauss/m]�̒P�ʂŕ\��������
*/

#define SVN_PLN_RO_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 235)
#define SVT_PLN_RO_MPG_GRAD VFT_FLT32
#define SVS_PLN_RO_MPG_GRAD "GRD strength of MPG along RO direction"
/*
	RO������MPG�p���X�̌X�Ύ��ꋭ�x��[Gauss/m]�̒P�ʂŕ\��������
*/

#define SVN_PLN_SS_MPG_GRAD (SVN_BASE_PROTOCOL_PLAN + 236)
#define SVT_PLN_SS_MPG_GRAD VFT_FLT32
#define SVS_PLN_SS_MPG_GRAD "GRD strength of MPG along SS direction"
/*
	SS������MPG�p���X�̌X�Ύ��ꋭ�x��[Gauss/m]�̒P�ʂŕ\��������
*/

#define SVN_PLN_RX_PHASE_CONTROL (SVN_BASE_PROTOCOL_PLAN + 237)
#define SVT_PLN_RX_PHASE_CONTROL VFT_INT32
#define SVS_PLN_RX_PHASE_CONTROL "Rx Phase control(on/off)"
/*
	��M�ɂ��ʑ��R���g���[����prolim�̃f�t�H���g�l�i�V�[�P���X
	�ɂ��Œ�j�ɂ��w��B����ɂ���A�x���[�W���O���ɋ���
	�V���b�g������������A���S���Y�����~����B
*/

#define SVN_PLN_NUM_SPIN_CONDITIONING (SVN_BASE_PROTOCOL_PLAN + 238)
#define SVT_PLN_NUM_SPIN_CONDITIONING VFT_INT32
#define SVS_PLN_NUM_SPIN_CONDITIONING "number of dummy shot before user resume"
/*
	���[�UPAUSE����RESUME�̎��̋󂤂��񐔁B
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
/*	�O���b�h�^�O�̎w��
	*/

#define SVN_PLN_NUM_GRID_TAG	(SVN_BASE_PROTOCOL_PLAN + 242)
#define SVT_PLN_NUM_GRID_TAG	VFT_BITFIELD
#define SVS_PLN_NUM_GRID_TAG	"number of Grid-Tag"
/*	�O���b�h�^�O��RF�p���X��
	*/

#define	SVN_PLN_AUTO_HOLD	(SVN_BASE_PROTOCOL_PLAN + 243)
#define	SVT_PLN_AUTO_HOLD	VFT_INT32
#define	SVS_PLN_AUTO_HOLD	"Wait for Start button"
/*	acqman �̎B�e���[�h�����肷��B
	PQM �ɂ�莩�����A����ȊO�� PLN_HOLD �Ɠ����Bacqman ��
	PLN_HOLD,PLN_AUTO_HOLD �� OR �ɂ��B�e���[�h�𐧌䂷��B
	
  |--->  Pre Scan --------------------------->|---> Scan  ---------------->
  ________________________________________________________________________
  |  Coil Tune  |   APC   |   CFA   |   RGN     Main Scan  |  SUB ......|
  ��~~~~~~~~~~~~��~~~~~~~��~~~~~~~~��~~~~~~~~~��~~~~~~~~~~~~~��~~~~~~~~~~~
  PRE_SCAN      APC      CFA       RGN        MAIN_SCAN      COVERAGE

��`
	  VFC_HOLD_PRE_SCAN	:  �B�e�����̍ŏ��Ŏ~�܂�B�i�ʏ�̎B�e�����j
	  VFC_HOLD_MAIN_SCAN	:  �v���X�L�����I����A�{�X�L�����̑O�ňꎞ��~�B
	  VFC_HOLD_COVERAGE	:  �J�o���[�W���ƂɎ~�܂�B

	  VFC_HOLD_APC	:  �`�o�b���O�ňꎞ��~�B
	  VFC_HOLD_CFA	:  �b�e�`���O�ňꎞ��~�B
	  VFC_HOLD_RGN	:  �q�f�m���O�ňꎞ��~�B
	  VFC_HOLD_CDS	:  �b�c�r���O�ňꎞ��~�B

	���j
	  VFC_HOLD_MAIN_SCAN �� VFC_HOLD_COVERAGE �̂ǂ��炩���w�肳���
	�����ꍇ�A�{�X�L�����̑O�ňꎞ��~����B

	*/

#define SVN_PLN_WATERSAT_PULSE		(SVN_BASE_PROTOCOL_PLAN + 244)
#define SVT_PLN_WATERSAT_PULSE		VFT_INT32
#define SVS_PLN_WATERSAT_PULSE		"Number of WATERSAT pulses"
/*	0:������Ȃ�	�X�C�b�` Off
	#:�p���X��	WATERSAT �p���X��
	*/

#define SVN_PLN_WATERSAT_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 245)
#define SVT_PLN_WATERSAT_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_WATERSAT_FLIP_ANGLE	"Flip Angle for WATERSAT pulses"
/*	WATERSAT Pulse �t���b�v�A���O���B
	�p���X���Ƀt���b�v�A���O���𐧌䂷��H
	*/

#define SVN_PLN_PE_PER_SEGMENT		(SVN_BASE_PROTOCOL_PLAN + 246)
#define SVT_PLN_PE_PER_SEGMENT		VFT_INT32
#define SVS_PLN_PE_PER_SEGMENT		"The number of PE in segment"
/*	�Z�O�����g����̃G���R�[�h��
 */

#define SVN_PLN_AFI_PE_ACQ_RATIO	(SVN_BASE_PROTOCOL_PLAN + 247)
#define SVT_PLN_AFI_PE_ACQ_RATIO	VFT_FLT32
#define SVS_PLN_AFI_PE_ACQ_RATIO	"AFI PE Ratio"
/*	AFI Ratio (PE)
 */

#define SVN_PLN_MRS_VOXCEL		(SVN_BASE_PROTOCOL_PLAN + 248)
#define SVT_PLN_MRS_VOXCEL		VFT_FLT32
#define SVS_PLN_MRS_VOXCEL		"MRS Voxcel Size"
/*	VFT_FLT32 �̔z��B�v�f���͂R�œ��e�͈ȉ��̂Ƃ���B
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define SVN_PLN_MRS_MATRIX		(SVN_BASE_PROTOCOL_PLAN + 249)
#define SVT_PLN_MRS_MATRIX		VFT_INT32
#define SVS_PLN_MRS_MATRIX		"MRS Matrix"
/*	VFT_INT32 �̔z��B�v�f���͂R�œ��e�͈ȉ��̂Ƃ���B
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define	SVN_PLN_BBINV_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 250)
#define	SVT_PLN_BBINV_FLIP_ANGLE	VFT_INT32
#define	SVS_PLN_BBINV_FLIP_ANGLE	"Flip angle for BB inversion pulse"
/*	�u���b�O�u���b�h�C���o�[�W�����p���X�t���b�v�p
	*/

#define SVN_PLN_SPEED_FLAG		(SVN_BASE_PROTOCOL_PLAN + 251)
#define SVT_PLN_SPEED_FLAG		VFT_INT32
#define	SVS_PLN_SPEED_FLAG		"SPEED Flag"
/*	SPEED���ʎq
 	*/

#define SVN_PLN_MRS_SELECTED		(SVN_BASE_PROTOCOL_PLAN + 252)
#define SVT_PLN_MRS_SELECTED		VFT_FLT32
#define SVS_PLN_MRS_SELECTED		"MRS Selected"
/*	VFT_INT32 �̔z��B�v�f���͂R�œ��e�͈ȉ��̂Ƃ���B
	Width  : v[0]
	Height : v[1]
	Depth  : v[2]
	*/

#define SVN_PLN_IMAGE_PATTERN	(SVN_BASE_PROTOCOL_PLAN + 253)
#define SVT_PLN_IMAGE_PATTERN	VFT_BITFIELD
#define SVS_PLN_IMAGE_PATTERN	"Contrast Image in simultaneous acquisition"
/*	�ǂ̂悤�Ȏ�ނ̉摜��1�x�Ɏ��W���邩��\���p�����[�^�B 
	Diffusion�C���[�W���O�̏ꍇ�AMPG��������Ȃ��ꍇ, MPG���X���C�X
	�����Ɉ�������ꍇ�AMPG�����[�h�����Ɉ�������ꍇ�AMPG���G���R�[�h
	�����Ɉ�������ꍇ�ɂ��Ď��W����̂ŁA���̒l��4�ƂȂ�B
	defualt�l��0�Ƃ��A0�̏ꍇ�y�уt�B�[���h�����݂��Ȃ��ꍇ�ɂ́A
	�]���ƌ݊��̓�����s�����̂Ƃ���B
	
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
	
	�ݒ��:	prolim, pqm
	
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
/*	�v���R���g���X�g(PreC)�p���X�̃t���b�v�p
 */

#define SVN_PLN_PREC_FLOP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 255)
#define SVT_PLN_PREC_FLOP_ANGLE	VFT_INT32
#define SVS_PLN_PREC_FLOP_ANGLE	"Precontrast pulse flop angle"
/*	�v���R���g���X�g(PreC)�p���X�̃t���b�v�p
 */


#define SVN_PLN_PREC_PULSE	(SVN_BASE_PROTOCOL_PLAN + 256)
#define SVT_PLN_PREC_PULSE	VFT_INT32
#define SVS_PLN_PREC_PULSE	"Precontrast pulse flag"
/*	�v���R���g���X�g(PreC)�p���X��ON/OFF���L�q����t�B�[���h�B
	(�����̊g�����̂��߁ABOOL�ł͂Ȃ�INT�^�Œ�`����
	VFC_PRECONTRAST_PULSE_NONE	0
	VFC_PRECONTRAST_PULSE_DWI	1
	VFC_PRECONTRAST_PULSE_T2	2
	*/

#define SVN_PLN_MRS_BASE_INFO	(SVN_BASE_PROTOCOL_PLAN + 257)
#define SVT_PLN_MRS_BASE_INFO	VFT_INT32
#define SVS_PLN_MRS_BASE_INFO	"MRS Acquisition start point and round direction"
/*	MRS���W�̋N�_����щ�]�������`����B
	VFC_MRS_BASE_LTR	001(2)	1
	VFC_MRS_BASE_LTL	000(2)	0
	VFC_MRS_BASE_RTR	101(2)	5
	VFC_MRS_BASE_RTL	100(2)	4
	VFC_MRS_BASE_LBR	011(2)	3
	VFC_MRS_BASE_LBL	010(2)	2
	VFC_MRS_BASE_RBR	111(2)	7
	VFC_MRS_BASE_RBL	110(2)	6
	�Ō�̂R�����̂����ŏ��̂Q�������N�_��\���B
	L:�� R:�E T:�� B:��
	�Ō�̂P�����ŋN�_����ǂ�������̏��ɐ��f�[�^���i�[����邩��\���B
	L:�������� R:�E������
 */

#define SVN_PLN_MOVING_COUCH_FLG	(SVN_BASE_PROTOCOL_PLAN + 258)
#define SVT_PLN_MOVING_COUCH_FLG	VFT_INT32
#define SVS_PLN_MOVING_COUCH_FLG	"Moving couch flag"
/*   (1)Moving Bed�t���O�B
 	Moving Bed�B�e�𔻒f���邽�߂ɗp����B�K�����݂���t�B�[���h�B
 		0) �ʏ�B�e
 		1) Moving Bed�B�e(�A������)
 		2) Moving Bed�B�e(�蓮���Ԑ������)
 		3) Moving Bed�B�e(�������Ԑ������)
     (2) prolim���Z�b�g����B
     (3) �e�v���O�����͂��̒l������Moving Bed���ǂ����𔻒f����B
 */

#define SVN_PLN_COUCH_TABLE	(SVN_BASE_PROTOCOL_PLAN + 259)
#define SVT_PLN_COUCH_TABLE	VFT_INT32
#define SVS_PLN_COUCH_TABLE	"Moving couch flag"
/*  (1) �Q��ړ��̌v���ݒ肷��BVFT_INT32 �̔z���
        �v�f���͈ړ��񐔂Ɉˑ�����B
        �ړ��񐔂�SVN_PLN_REPETITIONS �l�Ƃ���B
	�A���ő�P�U���Ƃ���B
	v[0] : �ړ���
	v[1] : �P��ڂ̈ړ�����
	v[2] : �Q���  �V
	...
	v[n] : n���  �V        �i n = SVN_PLN_REPETITIONS �j
        �z��ɃZ�b�g����ړ������͎B�e�J�n���̐Q��ʒu����̈ړ������ł���B
	�Ⴆ�΁A�B�e�J�n���̐Q��ʒu�� P �Ƃ���Ƃ��e�Q��ړ�����
        �Q��ʒu�͈ȉ��̂悤�ɂȂ�B

	P[0] = P
	P[1] = P + v[1]
	P[2] = P + v[2]
	P[3] = P + v[3]
	...
	���̃t�B�[���h�����݂��Ă�SVN_PLN_MOVING_COUCH_FLG��0�Ȃ�Βʏ�
 	�X�L�����ƂȂ�B
     (2) prolim���Z�b�g����B
     (3) acqman�͂��̃e�[�u�����Q�Ƃ��čŏ��̃X�L�����̌�C�ړ�������TCM
 	�Ɏw�����Q����ړ������ăX�L����������B������ړ��񐔌J��Ԃ��B
 */


#define SVN_PLN_COUCH_MOVE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 260)
#define SVT_PLN_COUCH_MOVE_FLAG	VFT_INT32
#define SVS_PLN_COUCH_MOVE_FLAG	"Couch Move Flag"
/*	���ꒆ�S�ւ̐Q��ړ��t���O
	�Q��ړ��w�肪ON  : 1
	�Q��ړ��w�肪OFF : 0
	���j�{�t���O�̓X���u���S����Ɏ��ꒆ�S���܂ޕ��ʏ�i��������
	���̋ߖT�j�Ɉړ������邱�Ƃɂ�莉�b�}�����ʂ����߂�@�\��
	����������̂ł���, SVN_PLN_MOVING_COUCH_FLG �Ƃ͈قȂ�_��
	���ӂ��邱�ƁB
	*/

#define SVN_PLN_NOWRAP_RATIO	(SVN_BASE_PROTOCOL_PLAN + 261)
#define SVT_PLN_NOWRAP_RATIO	VFT_FLTVEC3
#define SVS_PLN_NOWRAP_RATIO	"Nowrap ratio"
/*
�m�[���b�v�̊����B
�iSPEEDER phase1�ł́A���s���Q�Œ�Ƃ���j
*/

#define SVN_PLN_SPEEDER_RATIO	(SVN_BASE_PROTOCOL_PLAN + 262)
#define SVT_PLN_SPEEDER_RATIO	VFT_FLTVEC3
#define SVS_PLN_SPEEDER_RATIO	"Speeder ratio"
/*
SPEEDER�̍��������B
�iSPEEDER phase1�ł́A���s���Q�Œ�Ƃ���j
*/

#define SVN_PLN_SPEEDER_FLAG	(SVN_BASE_PROTOCOL_PLAN + 263)
#define SVT_PLN_SPEEDER_FLAG	VFT_BITFIELD
#define SVS_PLN_SPEEDER_FLAG	"Speeder Flag"
/*
SPEEDER�V�[�P���X���ǂ����̃t���O�B
     �o�d������SPEEDER       VFC_SPEEDER_FLAG_BIT_PE     1
     �q�n������SPEEDER       VFC_SPEEDER_FLAG_BIT_RO     (1<<1)
     �r�d������SPEEDER       VFC_SPEEDER_FLAG_BIT_SE     (1<<2)
*/

#define SVN_PLN_SPEEDER_TYPE	(SVN_BASE_PROTOCOL_PLAN + 264)
#define SVT_PLN_SPEEDER_TYPE	VFT_INT32
#define SVS_PLN_SPEEDER_TYPE	"Speeder Type"
/*
SPEEDER�̃^�C�v�B
    �ʏ�X�L����           VFC_SPEEDER_TYPE_NONE         0
    ���x����p�X�L����     VFC_SPEEDER_TYPE_SENSITIVITY  1
    SPEEDER�{�X�L����     VFC_SPEEDER_TYPE_SPEEDER       2
*/

#define SVN_PLN_MAXSLICE_COVERAGE		(SVN_BASE_PROTOCOL_PLAN + 265)
#define SVT_PLN_MAXSLICE_COVERAGE		VFT_INT32
#define SVS_PLN_MAXSLICE_COVERAGE		"Max Slice / Coverage"
/*	proEdit�̑Θb���͒l�����ɐݒ肵�Aprolim���Q�Ƃ���B
	*/

#define SVN_PLN_SLICES_IN_PERCENT_RR	(SVN_BASE_PROTOCOL_PLAN + 266)
#define SVT_PLN_SLICES_IN_PERCENT_RR	VFT_INT32
#define SVS_PLN_SLICES_IN_PERCENT_RR	"Number of slices in %R-R"
/*	�V�[�P���V����(�p�b�L���O)���[�h���ɁAprolim��%R-R���Ɏ��܂�X���C�X����
	���߂Đݒ肵�Aseqgen���Q�Ƃ���B*/


#define SVN_PLN_CONTROL_IR_FLAG	(SVN_BASE_PROTOCOL_PLAN + 267)
#define SVT_PLN_CONTROL_IR_FLAG	VFT_INT32
#define SVS_PLN_CONTROL_IR_FLAG	"CONTROL IR Flag for ASL"
/*	ASL�ł�CONTROL IR��On/Off�t���O�B
On:1, Off:0
*/


#define SVN_PLN_ASL_NSS_TEC_NUM	(SVN_BASE_PROTOCOL_PLAN + 268)
#define SVT_PLN_ASL_NSS_TEC_NUM	VFT_INT32
#define SVS_PLN_ASL_NSS_TEC_NUM	"Number of nssIR & TEC pulse for ASL"
/*	ASL�ł�nssIR��TEC(TagEndCut)�̃p���X���B
�ŏ���1�ŁATEC��On�̏ꍇ�͍ő��5�܂ŁATEC��Off�̏ꍇ�ɂ͍ő��4�܂Ŏw��\�B

*/


#define SVN_PLN_ASL_NSS_TEC_TI	(SVN_BASE_PROTOCOL_PLAN + 269)
#define SVT_PLN_ASL_NSS_TEC_TI	VFT_FLT32
#define SVS_PLN_ASL_NSS_TEC_TI	"Inversion Time of nssIR & TEC pulse for ASL"
/*	ASL�ł�nssIR��TEC(TagEndCut)�̂s�h�B
�v�f��5�̔z��ŁASVN_PLN_ASL_NSS_TEC_NUM���L���BImaging�ɋ߂��p���X�̏��Ɋi�[�����BSVN_PLN_ASL_TEC_FLAG��On�̏ꍇ�ASVN_PLN_ASL_TEC_INDEX�Ԗڂ�TEC�p��TI�ƂȂ�B

*/


#define SVN_PLN_ASL_TEC_FLAG	(SVN_BASE_PROTOCOL_PLAN + 270)
#define SVT_PLN_ASL_TEC_FLAG	VFT_INT32
#define SVS_PLN_ASL_TEC_FLAG	"TEC(TagEndCut) On/Off Flag for ASL"
/*	TEC(TagEndCut)��On/Off�t���O�B
On:1, Off:0

*/


#define SVN_PLN_ASL_TEC_INDEX	(SVN_BASE_PROTOCOL_PLAN + 271)
#define SVT_PLN_ASL_TEC_INDEX	VFT_INT32
#define SVS_PLN_ASL_TEC_INDEX	"TEC(TagEndCut) Index Number for ASL"
/*	TEC(TagEndCut)�̏��ԁB
SVN_PLN_ASL_TEC_FLAG��On�̏ꍇ�ɁASVN_PLN_ASL_NSS_TEC_NUM�Ŏw�肳�ꂽ�p���X�̂����AImaging�ɋ߂��ق�����TEC�����Ԗڂ��������C���f�b�N�X�B

*/




#define SVN_PLN_NSSIR_FLIP_ANGLE	(SVN_BASE_PROTOCOL_PLAN + 272)
#define SVT_PLN_NSSIR_FLIP_ANGLE	VFT_INT32
#define SVS_PLN_NSSIR_FLIP_ANGLE	"Flip angle for nssIR pulse"
/*	nssIR�p���X�̃t���b�v�p

*/


#define SVN_PLN_SKIP_PREPULSE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 273)
#define SVT_PLN_SKIP_PREPULSE_FLAG	VFT_INT32
#define SVS_PLN_SKIP_PREPULSE_FLAG	"Skip PrePulse Flag for FFE3D Skip"
/*	FFE3D Skip�V�[�P���X�ł̃v���p���XOn/Off�t���O�B
On:1, Off:0�B
*/


#define SVN_PLN_SKIP_PREPULSE_CONTRAST	(SVN_BASE_PROTOCOL_PLAN + 274)
#define SVT_PLN_SKIP_PREPULSE_CONTRAST	VFT_INT32
#define SVS_PLN_SKIP_PREPULSE_CONTRAST	"Skip PrePulse Contrast for FFE3D Skip"
/*	FFE3D Skip�ł̃v���p���X�̌�����Bappcode�Ƃ��ĂR�i�K����BNORMAL�͂Ȃ��B
   VFC_SKIP_PREPULSE_CONTRAST_NORMAL   0
   VFC_SKIP_PREPULSE_CONTRAST_HEAVY    1
   VFC_SKIP_PREPULSE_CONTRAST_MIDDLE   2
   VFC_SKIP_PREPULSE_CONTRAST_LIGHT    3

*/


#define SVN_PLN_SKIP_SEGMENT_SIZE	(SVN_BASE_PROTOCOL_PLAN + 275)
#define SVT_PLN_SKIP_SEGMENT_SIZE	VFT_INT32
#define SVS_PLN_SKIP_SEGMENT_SIZE	"Each Segment Size for FFE3D Skip"
/*	FFE3D Skip�ł̊e�Z�O�����g(Area)��Shot���B�v�f��6�̔z��B

[0]: Area0 : A shot ... �����g�����̃G���R�[�h�����Scan�J�n
[1]: Area1 : B shot ... Echo Stabilize + PrePulse with sampling
[2]: Area2 : C% * PhaseEncode * Slice Encode shot
[3]: Area3 : D% * PhaseEncode * Slice Encode shot
[4]: Area4 : E% * PhaseEncode * Slice Encode shot
[5]: �c��  : (TotalShot &#8211; Area0 &#8211; Area1 &#8211; Area2 &#8211; Area3 &#8211; Area4) shot 

�V�[�P���X�e�[�u��(prolim def)��ł�Area2,3,4�͕S����(%)�ŗ^�����邪�Aprolim��Shot���ɕϊ����ăZ�b�g����B


*/


#define SVN_PLN_SKIP_NUM_SKIP	(SVN_BASE_PROTOCOL_PLAN + 276)
#define SVT_PLN_SKIP_NUM_SKIP	VFT_INT32
#define SVS_PLN_SKIP_NUM_SKIP	"Number of FlipPulse per PrePulse for FFE3D Skip"
/*	FFE3D Skip�ł̊e�Z�O�����g(Area)�ł�Skip���B�v�f��6�̔z��B
Skip���̓v���p���X�P��ɂ��ăt���b�v�p���X�����񂠂邩�̐��B

[0]: Area0 ��Skip���B
[1]: Area1 ��Skip���B
[2]: Area2 ��Skip���B
[3]: Area3 ��Skip���B
[4]: Area4 ��Skip���B
[5]: Area5 ��Skip���B

*/


#define SVN_PLN_AUTO_PLAN_FLG	(SVN_BASE_PROTOCOL_PLAN + 277)
#define SVT_PLN_AUTO_PLAN_FLG	VFT_BOOL
#define SVS_PLN_AUTO_PLAN_FLG	"Auto Plan Flag "
/*	�ȈՎB�e�Ŏ����I��PLAN���s�Ȃ����𔻒f������t���O�B
*/


#define SVN_PLN_AUTO_POSTPROC_FLG	(SVN_BASE_PROTOCOL_PLAN + 278)
#define SVT_PLN_AUTO_POSTPROC_FLG	VFT_BOOL
#define SVS_PLN_AUTO_POSTPROC_FLG	"Auto Postproc Flag "
/*	�B�e�A���������s�Ȃ����𔻒f������t���O�B
*/


#define SVN_PLN_AUTO_FILM_FLG	(SVN_BASE_PROTOCOL_PLAN + 279)
#define SVT_PLN_AUTO_FILM_FLG	VFT_BOOL
#define SVS_PLN_AUTO_FILM_FLG	"Auto Film Flag "
/*	�ȈՎB�e��AUTOFILM�Ώۂ̃v���g�R�����𔻒f������t���O�B
*/


#define SVN_PLN_FILM_PRESET	(SVN_BASE_PROTOCOL_PLAN + 280)
#define SVT_PLN_FILM_PRESET	VFT_STRING
#define SVS_PLN_FILM_PRESET	"Film Preset Filename "
/*	Film�Ŏg�p����v���Z�b�g�t�@�C�������`����B
*/

#define SVN_PLN_EASYSCAN_FLG	(SVN_BASE_PROTOCOL_PLAN + 281)
#define SVT_PLN_EASYSCAN_FLG	VFT_BOOL
#define SVS_PLN_EASYSCAN_FLG	"Easy Scan Flag"
/*	�ȈՎB�e�t���O
*/


#define SVN_PLN_SAMPLE_IMAGE	(SVN_BASE_PROTOCOL_PLAN + 282)
#define SVT_PLN_SAMPLE_IMAGE	VFT_INT8
#define SVS_PLN_SAMPLE_IMAGE	"Easy Scan Sample Image data"
/*	�ȈՎB�e�ŎB�e�����T���v���摜
*/

#define SVN_PLN_DEFAULT_CONTRAST        (SVN_BASE_PROTOCOL_PLAN + 283)
#define SVT_PLN_DEFAULT_CONTRAST        VFT_DEF_CONTRAST
#define SVS_PLN_DEFAULT_CONTRAST        "Image default contrast"
/*      �ȈՎB�e�pPAS�ɓo�^����āA
�ȈՎB�e�Ŏg����摜�P�x�̃f�t�H���g�e�[�u��
�f�[�^�^�C�v :SVN_PLN_DEFAULT_CONTRAST
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
/*      �ȈՎB�e�Ŏg����T���v���摜(SVN_PLN_SAMPLE_IMAGE)�̃f�[�^���
typedef struct VftPbm_s {
            char     type[2];     date type
            int32_t width ;       ����
            int32_t height ;      ����
            int32_t max ;         �ő�l
} VftPbm_t
*/

#define SVN_PLN_PREP_IMAGE_TYPE (SVN_BASE_PROTOCOL_PLAN + 285)
#define SVT_PLN_PREP_IMAGE_TYPE VFT_INT32
#define SVS_PLN_PREP_IMAGE_TYPE "Prep Scan Image type"
/*      �r�W���A���v���b�v�̉摜�^�C�v


Appcode
#define VFC_PREP_IMAGE_TYPE_IMAGE       0
#define VFC_PREP_IMAGE_TYPR_MIP 1
*/

#define SVN_PLN_ACQ_SPLIT_TABLE (SVN_BASE_PROTOCOL_PLAN + 286)
#define SVT_PLN_ACQ_SPLIT_TABLE VFT_INT32
#define SVS_PLN_ACQ_SPLIT_TABLE "acquisition Split table"
/*      �B�e�����e�[�u��
Val[0]:������
Val[1]:1��ڂ̎B�e��
Val[2]:2��ڂ̎B�e��
  :      :
*/

#define SVN_PLN_ACQ_SPLIT_MODE  (SVN_BASE_PROTOCOL_PLAN + 287)
#define SVT_PLN_ACQ_SPLIT_MODE  VFT_INT32
#define SVS_PLN_ACQ_SPLIT_MODE  "acquisition Split mode"
/*      �B�e�������[�h


appcode
#define VFC_ACQ_SPLIT_MODE_AUTOPAUSE    1
#define VFC_ACQ_SPLIT_MODE_PREP 2
*/

#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION      (SVN_BASE_PROTOCOL_PLAN + 288)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION      VFT_BITFIELD
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION      "Auto MovingBed Subtraction Flag"
/*      MovingBed�����̗L��
TRUE   MovingBed��������
FALSE  MovingBed�����Ȃ�
*/
#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       (SVN_BASE_PROTOCOL_PLAN + 289)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       VFT_BITFIELD
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES       "Auto MovingBed Subtraction Images"
/*      MovingBed�����摜��


Appcode
 #define VFC_SUBTRACTION_NONE     (1L<<0)    �����Ȃ��摜�쐬
 #define VFC_SUBTRACTION_ABSOLUTE (1L<<1)    ��Βl�����摜�쐬
 #define VFC_SUBTRACTION_COMPLEX  (1L<<2)    ���f�����摜�쐬
*/

#define SVN_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE (SVN_BASE_PROTOCOL_PLAN + 290)
#define SVT_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE VFT_INT32
#define SVS_PLN_AUTO_MOVINGBED_SUBTRACTION_BASE "Auto MovingBed Subtraction Base Phase"
/*      MovingBed����������Z�O�����g
*/

#define SVN_PLN_COUCH_SEG_TABLE (SVN_BASE_PROTOCOL_PLAN + 291)
#define SVT_PLN_COUCH_SEG_TABLE VFT_INT32
#define SVS_PLN_COUCH_SEG_TABLE "MovingBed Segment table"
/*      MovingBed�Z�O�����g�e�[�u��
Val[0]:�Z�O�����g��
Val[2]:1�Z�O�����g�ł̎B�e��
 :            :
*/

#define SVN_PLN_GATE_CTRL_PARAM (SVN_BASE_PROTOCOL_PLAN + 292)
#define SVT_PLN_GATE_CTRL_PARAM VFT_TIME_CTRL_PARAM
#define SVS_PLN_GATE_CTRL_PARAM "Gate control parameters"
/*      Gate�B�e���s�Ȃ����̃p�����[�^
*/

#define SVN_PLN_ACQ_SPLIT_INDEX (SVN_BASE_PROTOCOL_PLAN + 293)
#define SVT_PLN_ACQ_SPLIT_INDEX VFT_INT32
#define SVS_PLN_ACQ_SPLIT_INDEX "Acquisition Split Index"
/*      �����B�e�e�[�u��(SVN_PLN_ACQ_SPLIT_TABLE)���̃J�����g���W�C���f�b�N�X�ԍ�


*/

#define SVN_PLN_DATA_SAMPL_BAND (SVN_BASE_PROTOCOL_PLAN + 294)
#define SVT_PLN_DATA_SAMPL_BAND VFT_STRING
#define SVS_PLN_DATA_SAMPL_BAND "Sampling Band Width Name"
/*      (1)�T���v�����O�o���h����
(2)�o���h���I���ŕ\������Ă��郉�x����prolim(proEdit)���ݒ肵�A�\���n�����p����
*/

#define SVN_PLN_DATA_SAMPL_PITCH        (SVN_BASE_PROTOCOL_PLAN + 295)
#define SVT_PLN_DATA_SAMPL_PITCH        VFT_FLT32
#define SVS_PLN_DATA_SAMPL_PITCH        "Sampling Pitch"
/*      (1)�T���v�����O�s�b�`
(2)prolim(proEdit)��PROTOCOL�m�[�h�ɐݒ肷��B
*/

#define SVN_PLN_SSFP_PRE_FLIP_ANGLE     (SVN_BASE_PROTOCOL_PLAN + 296)
#define SVT_PLN_SSFP_PRE_FLIP_ANGLE     VFT_INT32
#define SVS_PLN_SSFP_PRE_FLIP_ANGLE     "Flip angle for SSFP preFlip pulse"
/*      (1)SSFP�ppreFlip�p���X�̃t���b�v�p
(2)prolim���Z�b�g���Alibsar���Q�Ƃ���B
*/

#define SVN_PLN_SSFP_POST_FLIP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 297)
#define SVT_PLN_SSFP_POST_FLIP_ANGLE    VFT_INT32
#define SVS_PLN_SSFP_POST_FLIP_ANGLE    "Flip angle for SSFP postFlip pulse"
/*      (1)SSFP�ppostFlip�p���X�̃t���b�v�p
(2)prolim���Z�b�g���Alibsar���Q�Ƃ���B
*/

#define SVN_PLN_SSFP_POST_FLIP_NUM      (SVN_BASE_PROTOCOL_PLAN + 298)
#define SVT_PLN_SSFP_POST_FLIP_NUM      VFT_INT32
#define SVS_PLN_SSFP_POST_FLIP_NUM      "Number of SSFP postFlip"
/*      (1)SSFP�ppostFlip�p���X�̈����
(2)prolim���Z�b�g���Alibsar���Q�Ƃ���B
*/

#define SVN_PLN_NUM_DUMMY_SEGMENT       (SVN_BASE_PROTOCOL_PLAN + 299)
#define SVT_PLN_NUM_DUMMY_SEGMENT       VFT_INT32
#define SVS_PLN_NUM_DUMMY_SEGMENT       "The number of dummy segment"
/*      (1)��ŃZ�O�����g��
*/

#define SVN_PLN_DYNAMIC_MIN_PERIOD      (SVN_BASE_PROTOCOL_PLAN + 300)
#define SVT_PLN_DYNAMIC_MIN_PERIOD      VFT_INT32
#define SVS_PLN_DYNAMIC_MIN_PERIOD      "Minimum period of Dynamic scan"
/*      (1)Dynamic scan����minimum period
	(2)prolim���Z�b�g��acqman���Q�Ƃ���B
*/

#define SVN_PLN_PROEDIT_MODE            (SVN_BASE_PROTOCOL_PLAN + 301)
#define SVT_PLN_PROEDIT_MODE            VFT_INT32
#define SVS_PLN_PROEDIT_MODE            "proEdit page mode"
/*      (1)proEdit�̃y�[�W��؂�ւ���t���O�B
	(2)���I�ɐ؂�ւ����āA�v���g�R�����ƂɈႤ���[�h�ɂȂ�̂ŁA
	   �t�B�[���h�Ƃ��Ď����Ƃɂ���B

Appcode
#define VFC_PROEDIT_MODE_NORMAL 0
#define VFC_PROEDIT_MODE_USER1  1
#define VFC_PROEDIT_MODE_USER2  2
*/

#define SVN_PLN_PROBE_CORRECTION        (SVN_BASE_PROTOCOL_PLAN + 302)
#define SVT_PLN_PROBE_CORRECTION        VFT_INT32
#define SVS_PLN_PROBE_CORRECTION        "Real Time Motion Correction"
/*      (1)proEdit�Őݒ肷��B
*/

#define SVN_PLN_PROBE_FLAG            (SVN_BASE_PROTOCOL_PLAN + 303)
#define SVT_PLN_PROBE_FLAG            VFT_INT32
#define SVS_PLN_PROBE_FLAG            "Enable Prove Edit flag"
/*      (1)SVN_PROBE_CORRECTION ��ON�̂Ƃ��Aprolim���P���Z�b�g����B
	(2)Probe�V�[�P���X�͂Q���Z�b�g����Ă���B
           �B���J�n����pqm���P���Z�b�g���Ȃ����B
	(3)�l�������Ă���΁AproEdit����v���[�u�ҏW���I���ł���t���O�B
	0:	NONE
	1:	RTMC (Real Time Motion Correction)
	2:	RTMP (Real Time Motion Probing)
*/

#define SVN_PLN_FATSAT_PULSE_TYPE	(SVN_BASE_PROTOCOL_PLAN + 304)
#define SVT_PLN_FATSAT_PULSE_TYPE	VFT_INT32
#define SVS_PLN_FATSAT_PULSE_TYPE	"Fatsat Pulse Type"
/*	�t�@�b�g�T�b�g�p���X�̌^���L�q����t�B�[���h�B
	0:	STANDARD
	1:	ROBUST
	*/

#define SVN_PLN_FLUORO_MODE            (SVN_BASE_PROTOCOL_PLAN + 305)
#define SVT_PLN_FLUORO_MODE            VFT_INT32
#define SVS_PLN_FLUORO_MODE            "FLUORO mode"
/*      FLUORO�̓��샂�[�h�B
Appcode
#define VFC_FLUORO_NONE     0    
#define VFC_FLUORO_DIRECT   1         
#define VFC_FLUORO_SEQ_RM   2
*/

#define SVN_PLN_NUM_RETRO_PHASE		(SVN_BASE_PROTOCOL_PLAN + 306)
#define SVT_PLN_NUM_RETRO_PHASE		VFT_INT32
#define SVS_PLN_NUM_RETRO_PHASE		"Number of phase for recon of retro"
/*	���g���č\���t�F�[�Y���B
	*/

#define SVN_PLN_RETRO_MODE            (SVN_BASE_PROTOCOL_PLAN + 307)
#define SVT_PLN_RETRO_MODE            VFT_INT32
#define SVS_PLN_RETRO_MODE            "Retro mode"
/*      RETRO�̓��샂�[�h�B
Appcode
#define VFC_RETRO_MODE_NONE     0    
#define VFC_RETRO_MODE_1        1         
#define VFC_RETRO_MODE_2        2
*/

#define SVN_PLN_RETRO_CTRL_PARAM (SVN_BASE_PROTOCOL_PLAN + 308)
#define SVT_PLN_RETRO_CTRL_PARAM VFT_TIME_CTRL_PARAM
#define SVS_PLN_RETRO_CTRL_PARAM "Retro control parameters"
/*      ���g���B�e���s�Ȃ����̃p�����[�^
*/

#define SVN_PLN_SPEEDER_RATIO_INPUT	(SVN_BASE_PROTOCOL_PLAN + 309)
#define SVT_PLN_SPEEDER_RATIO_INPUT	VFT_FLTVEC3
#define SVS_PLN_SPEEDER_RATIO_INPUT	"Input Speeder ratio"
/*
SPEEDER�̍��������̓��͒l�B
�}�g���N�X���ݒ�\�ɂȂ�悤�ɍČv�Z���āA���������������߁A
SVN_PLN_SPEEDER_RATIO�ɃZ�b�g����B
*/

#define SVN_PLN_AUTO_PROEDIT_FLAG	(SVN_BASE_PROTOCOL_PLAN + 310)
#define SVT_PLN_AUTO_PROEDIT_FLAG	VFT_INT32
#define SVS_PLN_AUTO_PROEDIT_FLAG	"Auto ProEdit Flag"
/*
�O�̎B�e���I��������Ɏ�����ProEdit�̋N���L�����Z�b�g����
�擪�v���g�R���̏ꍇ�͋N������Ȃ��B
�{���̖ړI��3D Locator��PLAN�ݒ���s������� ProEdit��K���s���K�v������
���ׂ̈�3D Locator���Z�b�g����PQM���g���B
#define VFC_AUTO_PROEDIT_NONE     0    
#define VFC_AUTO_PROEDIT_3DLOCATOR        1
*/

#define SVN_PLN_LOCATOR_ROI_STYLE	(SVN_BASE_PROTOCOL_PLAN + 311)
#define SVT_PLN_LOCATOR_ROI_STYLE	VFT_INT32
#define SVS_PLN_LOCATOR_ROI_STYLE	"ROI style for inset"
/*      
�ʒu���ߎ��ɐݒ肳�ꂽ�q�n�h�X�^�C��
�Q�ƕ\�����Ɉʒu���ߎ��Ɠ����X�^�C���łq�n�h��\��������v�]�Ή�
sc-locator���Z�b�g����
dsc_inset,dsc_refer,dsc_dRefer,libInset�Ȃǂ��Q�Ƃ���B
*/

#define SVN_PLN_PROBE_FLIP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 312)
#define SVT_PLN_PROBE_FLIP_ANGLE    VFT_INT32
#define SVS_PLN_PROBE_FLIP_ANGLE    "Flip angle for probe flip pulse"
/*      (1)Probe�pFlip�p���X�̃t���b�v�p
(2)prolim���Z�b�g���Alibsar���Q�Ƃ���B
*/

#define SVN_PLN_PROBE_FLOP_ANGLE    (SVN_BASE_PROTOCOL_PLAN + 313)
#define SVT_PLN_PROBE_FLOP_ANGLE    VFT_INT32
#define SVS_PLN_PROBE_FLOP_ANGLE    "Flop angle for probe flop pulse"
/*      (1)Probe�pFlop�p���X�̃t���b�v�p
(2)prolim���Z�b�g���Alibsar���Q�Ƃ���B
*/

#define SVN_PLN_ENCODE_ORDER	(SVN_BASE_PROTOCOL_PLAN + 314)
#define SVT_PLN_ENCODE_ORDER	VFT_INT32
#define SVS_PLN_ENCODE_ORDER	"Phase Encode Order Type"
/*	�ʑ��G���R�[�h�����B
	0:	SEQUENTIAL
	1:	CENTRIC
	*/

#define SVN_PLN_EPI_SORT_TYPE	(SVN_BASE_PROTOCOL_PLAN + 315)
#define SVT_PLN_EPI_SORT_TYPE	VFT_INT32
#define SVS_PLN_EPI_SORT_TYPE	"FFE_EPI Sort Type"
/*	FFE_EPI�ɂ�����G���R�[�h�����B
	0:	NONE
	1:	T1
	2:      T2
	*/

#define SVN_PLN_CAN_OBLIQUE_FLAG	(SVN_BASE_PROTOCOL_PLAN + 316)
#define SVT_PLN_CAN_OBLIQUE_FLAG	VFT_BOOL
#define SVS_PLN_CAN_OBLIQUE_FLAG	"Can Oblique Flag"
/*	�I�u���[�N�\�t���O
*/

#define SVN_PLN_POSTPROC_RESERVED	(SVN_BASE_PROTOCOL_PLAN + 317)
#define SVT_PLN_POSTPROC_RESERVED	VFT_INT32
#define SVS_PLN_POSTPROC_RESERVED	"Post Proc. Flag"
/*	�A���㏈���ɕK�v�Ȉꕔ�̃t�B�[���h���ȑO��seqgen��������ł��܂����B
	�č\���͂��̃t�B�[���h���Q�Ƃ��Č㏈�����N�����Ă��܂����B
	�Ƃ��낪�AV5.0�̋@�\�ǉ��ɂ���āA�A���㏈�����N�����邩�ǂ�����
	���seqgen�̋N���O�ɕK�v�ɂȂ�܂����B
	�����ŁA���c�̌��ʁAstudyFile��prolim�̕ϐ��Ƃ��Ăɉ��L�̕ϐ���ǉ�
	���Ă��炢�A�V�[�P���X��prolim�̕ϐ������O�ɃZ�b�g���邱�Ƃōč\��
	�ɏ���`�������Ǝv���܂��B
*/

#define SVN_PLN_MOVINGBED_NUM_SEGMENT (SVN_BASE_PROTOCOL_PLAN + 318)
#define SVT_PLN_MOVINGBED_NUM_SEGMENT VFT_INT32
#define SVS_PLN_MOVINGBED_NUM_SEGMENT "Number of segments for MovingBed"
/*      MovingBed�Z�O�����g��
*/

#define SVN_PLN_ASL_TEC_TI	(SVN_BASE_PROTOCOL_PLAN + 319)
#define SVT_PLN_ASL_TEC_TI	VFT_FLT32
#define SVS_PLN_ASL_TEC_TI	"Inversion Time of TEC pulse for ASL"
/*	ASL�ł�TEC(TagEndCut)�̂s�h�B
	TI����SVN_PLN_ASL_NSS_TEC_TI��TEC�ɑΉ����鎞�Ԃ��������l�B
	prolim�ŃZ�b�g��view���t�я��ɕ\������B
*/

#define SVN_PLN_PROBE_MOTION_RATIO  (SVN_BASE_PROTOCOL_PLAN + 320)
#define SVT_PLN_PROBE_MOTION_RATIO   VFT_FLTVEC3
#define SVS_PLN_PROBE_MOTION_RATIO   "Motion Ratio for Probing scan"
/*      ���A���^�C�������⏞�ɂ�����A���u���̕ψʂɑ΂���S����
	�ψʁB
*/

#define SVN_PLN_FFEEPI_TE	(SVN_BASE_PROTOCOL_PLAN + 321)
#define SVT_PLN_FFEEPI_TE	VFT_FLT32
#define SVS_PLN_FFEEPI_TE	"FFE_EPI TE"
/*	FFE_EPI��TE
*/

#define SVN_PLN_NUM_FSE_TE_ECHOES	(SVN_BASE_PROTOCOL_PLAN + 322)
#define SVT_PLN_NUM_FSE_TE_ECHOES	VFT_INT32
#define SVS_PLN_NUM_FSE_TE_ECHOES	"Number of contrast ETL for FSE"
/*	FSE��TE��echoFactor�̌��e�[�u��
*/

#define SVN_PLN_3D_EXTRA_SLICES_APPEND	(SVN_BASE_PROTOCOL_PLAN + 323)
#define SVT_PLN_3D_EXTRA_SLICES_APPEND	VFT_INT32
#define SVS_PLN_3D_EXTRA_SLICES_APPEND	"Extra slices for 3D"
/*	SVN_PLN_3D_EXTRA_SLICES�ɑ΂��ASENoWrapRatio���l�����Ȃ�
	�������Z�b�g����B�č\���ŎQ�Ƃ���B
	*/

#define SVN_PLN_ALTERNATED_ACQ_FLAG	(SVN_BASE_PROTOCOL_PLAN + 324)
#define SVT_PLN_ALTERNATED_ACQ_FLAG	VFT_INT32
#define SVS_PLN_ALTERNATED_ACQ_FLAG	"Alternated flag"
/*	1shot�i1segment�j�ڑI��IR��ON�A2shot�i2segment�j�ڑI��IR��OFF�Ƃ���
        ���݂ɎB�e����t���O�B �B�e���Ԃ͔{�ɂȂ�B
�@�@�@�@studyFile�ɂ͍č\���ɂ��G�R�[�m�[�h(1.X/2.0/7.0/8.{0,1}/9.x)��
�@�@�@�@�ǉ�����2�g�̉摜���i�[����B
*/

#define SVN_PLN_NON_SELECTIVE_IR_PULSE	(SVN_BASE_PROTOCOL_PLAN + 325)
#define SVT_PLN_NON_SELECTIVE_IR_PULSE	VFT_INT32
#define SVS_PLN_NON_SELECTIVE_IR_PULSE	"Flag for Non-selective IR Pulse"
/*	�u���b�N�u���b�h�C���o�[�W�����p���X�̔�I��IR�p���X��ON/OFF
        ���L�q����t�B�[���h�B�f�t�H���g(�l���O�̏ꍇ)��ON�ł��邱�Ƃɒ��ӁB
*/

#define SVN_PLN_TSLIT_THICKNESS	(SVN_BASE_PROTOCOL_PLAN + 326)
#define SVT_PLN_TSLIT_THICKNESS	VFT_FLT32
#define SVS_PLN_TSLIT_THICKNESS	"tSLIT Thickness"	
/*	tSLIT�̌���
 */

#define SVN_PLN_RO_DEPHASE_AREA_RATIO	(SVN_BASE_PROTOCOL_PLAN + 327)
#define SVT_PLN_RO_DEPHASE_AREA_RATIO	VFT_FLT32
#define SVS_PLN_RO_DEPHASE_AREA_RATIO	"Read dephase area ratio"	
/*	Flow-Spoied FBI�̖{�X�L�����ɂ����āA�I�y���[�^�ɂ����͂��ꂽ
	RO�����Ɉ������Dephasing Pulse�̋��x�B�p���X���x�́A�Ǐo���p��
	�X�Ύ���̗���肩��G�R�[���S�܂ł̖ʐς��P�Ƃ������́ADephase
	�p���X�̖ʐϔ�ŕ\���B
 */

#define SVN_PLN_FLOW_PREP_PARAM	(SVN_BASE_PROTOCOL_PLAN + 328)
#define SVT_PLN_FLOW_PREP_PARAM	VFT_FLT32
#define SVS_PLN_FLOW_PREP_PARAM	"Flow prep parameter"	
/*	FlowPrep�X�L�����ɂ����āA�V�[�P���X�ɂ��ݒ肳�ꂽRO���邢��PE
	�Ɉ������Dephasing Pulse�̏������x�l�A���x�̍��ݒl�A�񐔁B�v�f���R
	[0]�F�������x
	[1]�F����
	[2]�F��
 */

#define SVN_PLN_ALTERNATED_DELAY_TIME	(SVN_BASE_PROTOCOL_PLAN + 329)
#define SVT_PLN_ALTERNATED_DELAY_TIME	VFT_INT32
#define SVS_PLN_ALTERNATED_DELAY_TIME	"Alternated delay time"
/*      Flow-Spoiled FBI�X�L�����ɂ����āAAlternated�X�L�������w�肳�ꂽ
  �@�@�@�ꍇ�̂Q��ڂ̃X�L�����̃f�B���C���ԁB�P�ʂ�msec�B
*/

#define SVN_PLN_LOCAL_CF_FLAG	(SVN_BASE_PROTOCOL_PLAN + 330)
#define SVT_PLN_LOCAL_CF_FLAG	VFT_INT32
#define SVS_PLN_LOCAL_CF_FLAG	"Local CF flag"
/*     �Ǐ�CF NONE/ON���L�q����
*/

#define	SVN_PLN_FLOW_COMP	(SVN_BASE_PROTOCOL_PLAN + 331)
#define	SVT_PLN_FLOW_COMP	VFT_INT32
#define	SVS_PLN_FLOW_COMP	"Flow comp direction"
/*	Flow comp ����

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
/*	PreC Flip �p���X��
        setting     PROLIM
        reference   PROLIM

*/

#define SVN_PLN_PREC_FLOP_PULSE_NUM		(SVN_BASE_PROTOCOL_PLAN + 338)
#define SVT_PLN_PREC_FLOP_PULSE_NUM		VFT_INT32
#define SVS_PLN_PREC_FLOP_PULSE_NUM		"Number of PreC Flop Pulse"
/*	PreC Flop �p���X��
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
/*	Advanced MovingBed �B�����\�ȃV�[�P���X�ł��邩�������B
�@�@ProMgr����������B
	TRUE    Advanced MovingBed �B�����\�V�[�P���X
	FALSE   Advanced MovingBed �B�����ł��Ȃ��V�[�P���X
*/

#define	SVN_PLN_AMB_MODE_FLG	(SVN_BASE_PROTOCOL_PLAN + 344)
#define	SVT_PLN_AMB_MODE_FLG	VFT_INT32
#define	SVS_PLN_AMB_MODE_FLG	"Advanced MovingBed Mode flag"
/*	Advanced MovingBed protocol Mode 
	Slave Protocol��AMB�B�����̎����������ꂽ�v���g�R�����Ӗ�����B
	����protocol�̓V�[�P���X�R�s�[�y��PAS�o�^�ł��Ȃ��B
	PQM���ݒ肷��B�@PAS�X�V�Ώ�

	#define	VFC_AMB_MODE_NONE	0	 �ʏ�B��
	#define	VFC_AMB_MODE_MASTER	1	 AMB Mater protocol
	#define	VFC_AMB_MODE_SLAVE	2	 AMB SLAVE protocol    
*/

#define	SVN_PLN_AMB_COUCH_TABLE	(SVN_BASE_PROTOCOL_PLAN + 345)
#define	SVT_PLN_AMB_COUCH_TABLE	VFT_INT32
#define	SVS_PLN_AMB_COUCH_TABLE	"Advanced MovingBed Couch table"
/*	Advanced MovingBed �Q��ړ��e�[�u��
�@�@Acqman����}���`�v���g�R�����W�ɂĎg�p����B
    VFT_INT32 �̔z��ŗv�f���͎B���v���g�R�����Ɉˑ�����B
	�A���ő�U�S���Ƃ���B�@SVN_PLN_AMB_PROTOCOL_TABLE�Ɠ��������݂���B
	v[0] : �B���v���g�R����
	v[1] : �P��ڂ̈ړ������@(���O�Ɉړ��ςȂ̂Ń[���ɂȂ�)
	v[2] : �Q���  �V
	...
	v[n] : n���  �V  
        �B���J�n���̐Q��ʒu����̑��Έʒu�i�ړ������j�ł���B
     (1) PQM���Z�b�g����B
     (2) acqman��SVN_PLN_AMB_MODE_FLG��VFC_AMB_SLAVE�̂Ƃ������Q�Ƃ���B
 */

#define	SVN_PLN_AMB_PROTOCOL_TABLE	(SVN_BASE_PROTOCOL_PLAN + 346)
#define	SVT_PLN_AMB_PROTOCOL_TABLE	VFT_INT32
#define	SVS_PLN_AMB_PROTOCOL_TABLE	"Advanced MovingBed Protocol table"
/*	Advanced MovingBed �B���v���g�R���e�[�u��
	Acqman����}���`�v���g�R�����W�ɂĎg�p����B
    VFT_INT32 �̔z��ŗv�f���͎B���v���g�R�����Ɉˑ�����B
	�A���ő�U�S���Ƃ���BSVN_PLN_AMB_COUCH_TABLE�Ɠ��������݂���B
	v[0] : �B���v���g�R����
	v[1] : �P��ڂ̎B���v���g�R���ԍ�
	v[2] : �Q���  �V
	...
	v[n] : n���  �V  
     (1) PQM���Z�b�g����B
     (2) acqman��SVN_PLN_AMB_MODE_FLG��VFC_AMB_SLAVE�̂Ƃ������Q�Ƃ���B
 */

#define	SVN_PLN_AMB_ACTUAL_COUCH	(SVN_BASE_PROTOCOL_PLAN + 347)
#define	SVT_PLN_AMB_ACTUAL_COUCH	VFT_INT32
#define	SVS_PLN_AMB_ACTUAL_COUCH	"Advanced MovingBed Actual Couch position"
/*	Advanced MovingBed �B�����̎��Q��ʒu
�@  AMB�B������acqman���Z�b�g����B
 */

#define	SVN_PLN_RF_INTERLOCK_VALUE	(SVN_BASE_PROTOCOL_PLAN + 348)
#define	SVT_PLN_RF_INTERLOCK_VALUE	VFT_FLT32
#define	SVS_PLN_RF_INTERLOCK_VALUE	"RF Interlock Value(W)"
/*	
	PQM���ݒ肵��Acqman���g���BAMB��Acqman����}���`�v���g�R�����W�ɗL��
	�]����PQM��Acqman�ɒʐM��RF INTERLOCK�l��ʒm���Ă������AAMB�ł�
	Acqman����}���`�v���g�R�����W�ɂȂ�̂Ńt�B�[���h�n���ɂ���B
�@  AMB�B������acqman���Z�b�g����B
 */

#define SVN_PLN_AMB_SUBTRACTION      (SVN_BASE_PROTOCOL_PLAN + 349)
#define SVT_PLN_AMB_SUBTRACTION      VFT_BITFIELD
#define SVS_PLN_AMB_SUBTRACTION      "Advanced MovingBed Subtraction Flag"
/*  Advanced MovingBed�����̗L��
	TRUE   MovingBed��������
	FALSE  MovingBed�����Ȃ�
*/

#define SVN_PLN_AMB_SUBTRACTION_IMAGES       (SVN_BASE_PROTOCOL_PLAN + 350)
#define SVT_PLN_AMB_SUBTRACTION_IMAGES       VFT_BITFIELD
#define SVS_PLN_AMB_SUBTRACTION_IMAGES       "Advanced MovingBed Subtraction Images"
/*  Advanced MovingBed�����摜��
Appcode
 #define VFC_SUBTRACTION_NONE     (1L<<0)    �����Ȃ��摜�쐬
 #define VFC_SUBTRACTION_ABSOLUTE (1L<<1)    ��Βl�����摜�쐬
 #define VFC_SUBTRACTION_COMPLEX  (1L<<2)    ���f�����摜�쐬
*/

#define SVN_PLN_AMB_SUBTRACTION_PROTOCOL (SVN_BASE_PROTOCOL_PLAN + 351)
#define SVT_PLN_AMB_SUBTRACTION_PROTOCOL VFT_INT32
#define SVS_PLN_AMB_SUBTRACTION_PROTOCOL "Advanced MovingBed Subtraction Protocol index"
/*  Advanced MovingBed���������v���g�R���ԍ�
*/

#define SVN_PLN_AMB_PARAM           (SVN_BASE_PROTOCOL_PLAN + 352)
#define SVT_PLN_AMB_PARAM           VFT_AMB_PARAM
#define SVS_PLN_AMB_PARAM           "Advanced MovingBed Parameter"
/*  Advanced MovingBed �B���p�����[�^���L�q����t�B�[���h�B
    vf_apptypes.h �Q�ƁB
*/

#define SVN_PLN_AMB_CELL            (SVN_BASE_PROTOCOL_PLAN + 353)
#define SVT_PLN_AMB_CELL            VFT_AMB_CELL
#define SVS_PLN_AMB_CELL            "Advanced MovingBed Cell"
/*  Advanced MovingBed �B����CELL�����L�q����t�B�[���h�B
    vf_apptypes.h �Q�ƁB
*/

#define SVN_PLN_AMB_POSTPROC        (SVN_BASE_PROTOCOL_PLAN + 354)
#define SVT_PLN_AMB_POSTPROC        VFT_AMB_POST_PROC
#define SVS_PLN_AMB_POSTPROC        "Advanced MovingBed Post Proc."
/*  Advanced MovingBed �B�e�A�������p�����[�^���L�q����t�B�[���h�B
�@�@vf_apptypes.h �Q�ƁB
*/

#define SVN_PLN_OBLIQUE_EPI_ANGLE_TABLE (SVN_BASE_PROTOCOL_PLAN + 355)
#define SVT_PLN_OBLIQUE_EPI_ANGLE_TABLE VFT_FLT32
#define SVS_PLN_OBLIQUE_EPI_ANGLE_TABLE "Oblique Angle Table for Single Shot EPI"
/*
    �V���O���V���b�g�d�o�h�p��Oblique Angle Table

    �ݒ�� PROLIM
    �g�p�� PROLIM,PQM
    
    �Ƃ蓾��l
           ���v�f�F�����̌��E�p�x[deg]
           ���v�f�F�V���O���I�u���[�N�̌��E�p�x[deg]
*/

#define SVN_PLN_SCANOFFSET_SET_FLAG (SVN_BASE_PROTOCOL_PLAN + 356)
#define SVT_PLN_SCANOFFSET_SET_FLAG VFT_INT32
#define SVS_PLN_SCANOFFSET_SET_FLAG "Scan Offset Set flag"
/*
    �X�L�����I�t�Z�b�g�ݒ�t���O
    �ݒ�� PQM,Locator
    �g�p�� PQM
    0�F�X�L�����I�t�Z�b�g�ݒ�Ȃ�
    1�F�X�L�����I�t�Z�b�g�ݒ肠��
*/

#define SVN_PLN_GATE_INTERVAL_FLAG (SVN_BASE_PROTOCOL_PLAN + 357)
#define SVT_PLN_GATE_INTERVAL_FLAG VFT_INT32
#define SVS_PLN_GATE_INTERVAL_FLAG "Flag for dummy shot in gating scan"
/*
    Gate��ł��X�L�����t���O
    0�FGate��ł��Ȃ�
    1�FGate��ł�����
*/

#define SVN_PLN_FREQ_SUPPRESSION_PASTA (SVN_BASE_PROTOCOL_PLAN + 358)
#define SVT_PLN_FREQ_SUPPRESSION_PASTA VFT_INT32
#define SVS_PLN_FREQ_SUPPRESSION_PASTA "Frequency Suppression PASTA"
/*
    ���g���}���@PASTA��On/Off
    �ݒ�� PROLIM
    �g�p�� SEQ
    0�F���b�}���Ȃ�
    1�FPASTA����

    #define VFC_SUPPRESSION_NONE    0
    #define VFC_SUPPRESSION_FAT     1
    #define VFC_SUPPRESSION_SI      2
*/

#define SVN_PLN_SLICE_NUMBER_METHOD (SVN_BASE_PROTOCOL_PLAN + 359)
#define SVT_PLN_SLICE_NUMBER_METHOD VFT_INT32
#define SVS_PLN_SLICE_NUMBER_METHOD "Slice Number Method"
/*
    �X���C�X�ԍ��\������
    �ݒ�� PROLIM,PQM
    �g�p�� sc_locator,dsc_refer,dsc_dRefer,dsc_inset,offscreen,Recon,Display
    0�F�X���u�Ԃł̃V�[�P���V�����ȃX���C�X�ԍ��\��
    1�F�X���u���̃X���C�X�ԍ��\��

    #define VFC_SLICE_NUMBER_METHOD_SLICES    0
    #define VFC_SLICE_NUMBER_METHOD_SLAB      1
*/

#define SVN_PLN_BBTI_PREP_PARAM	(SVN_BASE_PROTOCOL_PLAN + 360)
#define SVT_PLN_BBTI_PREP_PARAM	VFT_FLT32
#define SVS_PLN_BBTI_PREP_PARAM	"BBTI prep parameters"
/*
    BBTIPrep�X�L�����ɂ�����BBTI�̏����l�A�J��Ԃ����ԁA�J��Ԃ��񐔁B�v�f���R
    [0]�F�����l
    [1]�F�J��Ԃ�����
    [2]�F�J��Ԃ���
 */

#define SVN_PLN_GATE_INTERVAL_NUM_COLLECTION (SVN_BASE_PROTOCOL_PLAN + 361)
#define SVT_PLN_GATE_INTERVAL_NUM_COLLECTION VFT_INT32
#define SVS_PLN_GATE_INTERVAL_NUM_COLLECTION "Number of collection for dummy shot in gating scan"
/*
    Gate��ł��X�L�����ɂ�����%R-R���ł̎��W�񐔁B
 */


#define SVN_PLN_GATE_INTERVAL_SEGMENTATION_TYPE (SVN_BASE_PROTOCOL_PLAN + 362)
#define SVT_PLN_GATE_INTERVAL_SEGMENTATION_TYPE VFT_INT32
#define SVS_PLN_GATE_INTERVAL_SEGMENTATION_TYPE "Type of segmantation for dummy shot in gating scan"
/*
    Gate��ł��X�L�����ɂ�����Z�O�����e�[�V�����^�C�v�B

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
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    �I�����ꂽ�R�C����Section���B
    �����Ɏw�肳�ꂽ�������A��M�R�C����Section���y��Signal��񂪑��݂���B
*/

#define SVN_PLN_SECTION_RECVR_COIL (SVN_BASE_PROTOCOL_PLAN + 365)
#define SVT_PLN_SECTION_RECVR_COIL VFT_SECTION_RECVR_COIL
#define SVS_PLN_SECTION_RECVR_COIL "Section recver coil"
/*
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    ��M�R�C����Section���
    ���̃t�B�[���h��PAS�X�V�Ώۂł���B
    �I�����ꂽSection��(SVN_PLN_NUM_SECTION_RECVR_COIL)�����z�񂪑��݂��āA
    Signal���(SVN_PLN_SECTION_RECVR_COIL_SIGNAL)�ƃy�A�ɂȂ��Ă���B
    typedef struct VftSectionRecvrCoil_s{
        uint8_t     portID[2];  ������A���l�߂ŋ󂫂̓X�y�[�X�����
        uint16_t    coilID;     �����I�ȃR�C��ID
        uint8_t     SectionID;  Saturn�R�C���̂݁A�]���R�C���̏ꍇ�̓[��
        uint8_t     channel;    �]���R�C���̏ꍇ�����L���ŁA�`���l���ԍ�
    } VftSectionRecvrCoil_t;

    PortID�ɂ̓|�[�g��(AG)������A�܂�G�|�[�g�̂悤�ɓ����|�[�g���������݂���
    �ꍇ�͂��̔ԍ����ݒ肳���B
    ��jG�|�[�g�̂T�Ԃ̏ꍇ�@�hG5�h�AF�|�[�g�ꍇ�gF �h
    �]���R�C���̂悤��SectionDB���Ȃ��R�C���̏ꍇ��SectionID��0�Ƃ��A���̑���A
    Channel�������B
    Saturn�R�C���̏ꍇ��channel���ɈӖ��Ȃ��B
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL   (SVN_BASE_PROTOCOL_PLAN + 366)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL   VFT_SECTION_SIGNAL 
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL   "SECTION SIGNAL INFOMATION"

/*
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    Section Signal���
    �I�����ꂽSection��(SVN_PLN_NUM_SECTION_RECVR_COIL)�����z�񂪑��݂��āA
    ��M�R�C����Section���(SVN_PLN_SECTION_RECVR_COIL)�ƃy�A�ɂȂ��Ă���B
    PAS�X�V�̑Ώۂł͂Ȃ��B
    typedef struct VftSectionSignal_s{
        int16_t     num;               �L��Signal��
        uint16_t    signal_no[16];      Signal ���
        uint16_t    hybrid_signal[16];  Hybrid ���
    }VftSectionSignal_t;

    signalNo�ɂ�SectionDB���ɒ�`����Ă���ASignalNo��ݒ肷��B
    �܂�Hybrid���Ƃ��Ă͈ȉ��̏�񂪓���B
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
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    �I�����ꂽSection���u����2�v
    SPEEDER MAP���A�v���g�R���̓r���ŃR�C�����ς�鎞�Ɏg����R�C�����B
    �I�����ꂽ�R�C����Section���B
    �����Ɏw�肳�ꂽ�������A��M�R�C����Section���Q�y��Signal���Q�����݂���B
*/

#define SVN_PLN_SECTION_RECVR_COIL2      (SVN_BASE_PROTOCOL_PLAN + 368)
#define SVT_PLN_SECTION_RECVR_COIL2      VFT_SECTION_RECVR_COIL
#define SVS_PLN_SECTION_RECVR_COIL2      "SECTION RECVR COIL2"
/*
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    ��M�R�C����Section���u����2�v
    SVN_PLN_SECTION_RECVR_COIL
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL2   (SVN_BASE_PROTOCOL_PLAN + 369)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL2   VFT_SECTION_SIGNAL 
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL2   "SECTION SIGNAL INFOMATION2"
/*
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    Section Signal���u����2�v
    SVN_PLN_SECTION_RECVR_COIL_SIGNAL
*/

#define SVN_PLN_RECVR_COIL_SHOW_LIST   (SVN_BASE_PROTOCOL_PLAN + 370)
#define SVT_PLN_RECVR_COIL_SHOW_LIST   VFT_COIL_SHOWLIST 
#define SVS_PLN_RECVR_COIL_SHOW_LIST   "RECVR COIL SHOW LIST"
/*
    Saturn�V�X�e���ȍ~�̐VRF�R�C���V�X�e���ŏꍇ�Ɏg������
    �\���R�C�����
    Saturn�V�X�e���̃R�C���I�������ŁA�ŏ��ɕ\��������R�C�����B
    �����ɓo�^�������R�C����񂪃R�C���I����ʂ̃R�C�����X�g�ɔ��f�����B
    PAS�X�V�̑Ώۂł���B
    typedef struct VftCoilListInfo_s {
        int32_t num;                �R�C����
        struct {
            uint8_t     portID[2];  Port�ԍ�
            uint16_t    coilID;     CoilID �����ԍ�
            uint8_t     channel;    Channel���
            uint8_t     coordinate; �\���ʒu
            uint8_t    filler[2];
        }�@coillist[10];
    }VftCoilListInfo_t;
*/

#define SVN_PLN_NUM_PE_LINE_NOISE   (SVN_BASE_PROTOCOL_PLAN + 371)
#define SVT_PLN_NUM_PE_LINE_NOISE   VFT_INT32
#define SVS_PLN_NUM_PE_LINE_NOISE   "Number of PE line noise"
/*
    ch�ԃQ�C���E�m�C�Y�}�g���N�X�␳�̂��߂�
    �m�C�Y�����W����PE�G���R�[�h��
    �ݒ�� PROLIM
    �g�p�� PROLIM,RECON,SEQ
*/

#define SVN_PLN_PHASE_CORRECTION_PRESCAN_FLAG   (SVN_BASE_PROTOCOL_PLAN + 372)
#define SVT_PLN_PHASE_CORRECTION_PRESCAN_FLAG   VFT_INT32 
#define SVS_PLN_PHASE_CORRECTION_PRESCAN_FLAG   "Phase correction prescan flag"
/*
    �ʑ��␳�v���X�L�����t���O
    0:prescan�����s���Ȃ�
    1:prescan�����s
    PROLIM�Ő��䂷��B

    #define VFC_PHASE_CORRECTION_PRESCAN_OFF   0
    #define VFC_PHASE_CORRECTION_PRESCAN_ON    1
*/

#define SVN_PLN_DEGENERACY_FLAG   (SVN_BASE_PROTOCOL_PLAN + 373)
#define SVT_PLN_DEGENERACY_FLAG   VFT_BOOL
#define SVS_PLN_DEGENERACY_FLAG   "Raw data Degeneracy flag"
/*
    ��M�f�[�^(���f�[�^)�̏k�ނ��s�����ATRUE(1)�ɂ���B
    ���̃t�B�[���h�����݂��Ȃ����A�܂��́AFALSE(0)��
    �ݒ肳��Ă��鎞�͏k�ނ��Ȃ��B

    �ݒ�� PROLIM
    �g�p�� PROLIM,acqman,pqm,recon
*/

#define SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   (SVN_BASE_PROTOCOL_PLAN + 374)
#define SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   VFT_SECTION_SIGNAL
#define SVS_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY   "SECTION SIGNAL INFOMATION for Degeneracy"
/*
    ��M�f�[�^(���f�[�^)�̏k�ނ��s������Section Signal���B

    �ݒ�� pqm
    �g�p�� acqman
*/

#define SVN_PLN_K_SPACE_TRAJECTORY_TYPE   (SVN_BASE_PROTOCOL_PLAN + 375)
#define SVT_PLN_K_SPACE_TRAJECTORY_TYPE   VFT_INT32
#define SVS_PLN_K_SPACE_TRAJECTORY_TYPE   "K space trajectory type"
/*
    JET��Key�t���O�B
    ������L�[�ɂ��āA�V�[�P���X�APROLIM�A�č\����JET��F������B

    �ݒ�� PROLIM
    �g�p�� PROLIM,recon,pqm

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
    JET���W����Blade��

    �ݒ�� PROLIM
    �g�p�� PROLIM,recon,sequence
*/

#define SVN_PLN_SWITCH_COIL_TYPE   (SVN_BASE_PROTOCOL_PLAN + 377)
#define SVT_PLN_SWITCH_COIL_TYPE   VFT_INT32
#define SVS_PLN_SWITCH_COIL_TYPE   "Switch coil type"
/*
    �R�C���؂�ւ��B���̃^�C�v

    �ݒ�� PROLIM
    �g�p�� PROLIM,pqm,seqence

    #define VFC_SWITCH_COIL_TYPE_NONE       0
    #define VFC_SWITCH_COIL_TYPE_MAP        1
*/

#define SVN_PLN_NUM_SWITCH_COIL     (SVN_BASE_PROTOCOL_PLAN + 378)
#define SVT_PLN_NUM_SWITCH_COIL     VFT_INT32
#define SVS_PLN_NUM_SWITCH_COIL     "Number of switch coil"
/*
    �R�C���؂�ւ��B���ɂĐ؂�ւ���R�C���̐�

    �ݒ�� PROLIM
    �g�p�� PROLIM,seqence
*/

#define SVN_PLN_FSBB_B_VALUE        (SVN_BASE_PROTOCOL_PLAN + 379)
#define SVT_PLN_FSBB_B_VALUE        VFT_FLT32
#define SVS_PLN_FSBB_B_VALUE        "b-value for FSBB"
/*
    FSBB�p�̃f�B�t�F�[�Y��(b�l)

    �ݒ�� PROLIM
    �g�p�� display
*/

#define SVN_PLN_BODY_PART           (SVN_BASE_PROTOCOL_PLAN + 380)
#define SVT_PLN_BODY_PART           VFT_STRING
#define SVS_PLN_BODY_PART           "Body part examined"
/*
    ��������
    DICOM TAG (0018,0015)�ɃZ�b�g���ꑗ�M�������ł���B
    �]����SAR���ʂ��B�e���ʂƂ���DICOM�]�����Ă������A
    ����Ȃ�ڍׂȌ������ʏ���K�v�Ƃ��邽�ߐV�݂��ꂽ�B

    setting     pqm
    reference   DICOM
*/

#define SVN_PLN_NOWRAP_RATIO_INPUT  (SVN_BASE_PROTOCOL_PLAN + 381)
#define SVT_PLN_NOWRAP_RATIO_INPUT  VFT_FLTVEC3
#define SVS_PLN_NOWRAP_RATIO_INPUT  "Input NoWrap ratio"
/*
    NoWrap Ratio�̓��͒l

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
    FSE�n�ł�Flop�p��Sweep���邱�Ƃ𐧌䂷��B

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
    FFE Cine�n�V�[�P���X��Time Stamp�̌v�Z�A���S���Y�����w�肷��B

    VFC_GATE_CTRL_TYPE_BYRECON(0) ���邢�̓t�B�[���h���Ȃ��ꍇ�F
     Cine �� Segment �w�肳��Ă���ꍇ�� Recon �� TimeStamp���v�Z����B
    0�ȊO�̏ꍇ�F
     prolim �� TimeStamp ���v�Z����B

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
    TIPrep�X�L�����ɂ�����TI�̏����l[sec]�A�J��Ԃ�����[sec]�A�J��Ԃ���[��]�B�v�f���R
    [0]�F�����l
    [1]�F�J��Ԃ�����
    [2]�F�J��Ԃ���
    setting     PROLIM
    reference   RECON
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID	(SVN_BASE_PROTOCOL_PLAN + 393)
#define	SVT_PLN_LOCATOR_IMAGE_ID	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID	"1st locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_2	(SVN_BASE_PROTOCOL_PLAN + 394)
#define	SVT_PLN_LOCATOR_IMAGE_ID_2	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_2	"2nd locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_3	(SVN_BASE_PROTOCOL_PLAN + 395)
#define	SVT_PLN_LOCATOR_IMAGE_ID_3	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_3	"3rd locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_4	(SVN_BASE_PROTOCOL_PLAN + 396)
#define	SVT_PLN_LOCATOR_IMAGE_ID_4	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_4	"4th locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_5	(SVN_BASE_PROTOCOL_PLAN + 397)
#define	SVT_PLN_LOCATOR_IMAGE_ID_5	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_5	"5th locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
    setting     Locator
    reference   Locator,inset,reference
 */

#define	SVN_PLN_LOCATOR_IMAGE_ID_6	(SVN_BASE_PROTOCOL_PLAN + 398)
#define	SVT_PLN_LOCATOR_IMAGE_ID_6	VFT_STRING
#define	SVS_PLN_LOCATOR_IMAGE_ID_6	"6th locator image loid"
/*
    �e�摜�����Q�Ƃ��邽�߂�Logical Object ID
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
    �R�C�������ݒ菈���̎��s�t���O
    pqm��ʂ́m�R�C�������ݒ�n�{�^���̏�Ԃ�ێ�����.
    0:�m�R�C�������ݒ�n�{�^�����I�t
    1:�m�R�C�������ݒ�n�{�^�����I��
    PAS�X�V�Ώ�
    �ݒ�ҁFpqm
    �g�p�ҁFpqm
    #define VFC_CDS_FLAG_OFF 0
    #define VFC_CDS_FLAG_ON  1
*/

#define	SVN_PLN_CDS_STATUS	(SVN_BASE_PROTOCOL_PLAN + 401)
#define	SVT_PLN_CDS_STATUS	VFT_INT32
#define	SVS_PLN_CDS_STATUS	"CDS status"
/*
    �R�C�������ݒ�̓�����e�B�B������l�͕ێ������
    �g���u����͗p�ɂ��Ƃ�CDS���ʂ��Q�Ƃ��邽�߂Ɏg�p����
    PAS�X�V�ΏۊO
    �ݒ�ҁFpqm
    �g�p�ҁFpqm
    #define VFC_CDS_STATUS_AUTO_SELECT  0x0001
    #define VFC_CDS_STATUS_SCAN         0x0002
    #define VFC_CDS_STATUS_USE_PREVIOUS 0x0004
*/

#define	SVN_PLN_CDS_COIL_COORDINATE	(SVN_BASE_PROTOCOL_PLAN + 402)
#define	SVT_PLN_CDS_COIL_COORDINATE	VFT_CDS_COIL_LOCATION
#define	SVS_PLN_CDS_COIL_COORDINATE	"CDS coil coordinate"
/*
    �Q��̎��Α��[����R�C�����S�܂ł̋���
    PAS�X�V�ΏۊO
    �ݒ�ҁFpqm
    �g�p�ҁFpqm
    {CoilID + detectedFlag + �R�C���ʒu}�̔z��B
    �z�񏇏���acqman����pqm�ւ̐ڑ��R�C�����Ɠ���
    �z�񏇏� = {A/��/��/D1/D2/F/G1/--/G7}�i�P�R�j
    �R�C���ʒu��Z�����W�B
    SVN_EXE_CDS_COIL_LOCATION�Ɠ����t�H�[�}�b�g
*/

#define	SVN_PLN_REPETITIONS_CDS		(SVN_BASE_PROTOCOL_PLAN + 403)
#define	SVT_PLN_REPETITIONS_CDS		VFT_INT32
#define	SVS_PLN_REPETITIONS_CDS		"Number of repetitions of CDS scan"
/*
    CDS�̃_�C�i�~�b�N�X�L�����񐔁B�R�C���؂�ւ����ł���
    PAS�X�V�ΏۊO�B
    �ݒ�ҁFpqm
    �g�p�ҁFacqman
*/

#define	SVN_PLN_NUM_SECTION_RECVR_COIL_CDS	(SVN_BASE_PROTOCOL_PLAN + 404)
#define	SVT_PLN_NUM_SECTION_RECVR_COIL_CDS	VFT_INT32
#define	SVS_PLN_NUM_SECTION_RECVR_COIL_CDS	"Number of Section recver coil of CDS"
/*
    CDS�X�L��������R�C����section��
    SVN_PLN_NUM_SECTION_RECVR_COIL �̍\���ŁA
    SVN_PLN_REPETITIONS_CDS �̐��Ԃ�A�Ȃ������\���B
    PAS�X�V�ΏۊO�B
    �ݒ�ҁFpqm
    �g�p�ҁFacqman
*/

#define	SVN_PLN_SECTION_RECVR_COIL_CDS		(SVN_BASE_PROTOCOL_PLAN + 405)
#define	SVT_PLN_SECTION_RECVR_COIL_CDS		VFT_SECTION_RECVR_COIL
#define	SVS_PLN_SECTION_RECVR_COIL_CDS		"Section recver coil of CDS"
/*
    CDS�X�L���������M�R�C����Section���B
    SVN_PLN_SECTION_RECVR_COIL �̍\���ŁA
    SVN_PLN_REPETITIONS_CDS �̐��Ԃ�A�Ȃ������\���B
    PAS�X�V�ΏۊO�B
    �ݒ�ҁFpqm
    �g�p�ҁFacqman
*/

#define	SVN_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	(SVN_BASE_PROTOCOL_PLAN + 406)
#define	SVT_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	VFT_SECTION_SIGNAL 
#define	SVS_PLN_SECTION_RECVR_COIL_SIGNAL_CDS	"SECTION SIGNAL INFOMATION of CDS"
/*
    CDS�X�L���������M�R�C����Section Signal���B
    SVN_PLN_SECTION_RECVR_COIL_SIGNAL�̍\���ŁA
    SVN_PLN_REPETITIONS_CDS �̐��Ԃ�A�Ȃ������\���B
    PAS�X�V�ΏۊO�B
    �ݒ�ҁFpqm
    �g�p�ҁFacqman
*/

#define	SVN_PLN_GATING_SECTION_START_POINT	(SVN_BASE_PROTOCOL_PLAN + 407)
#define	SVT_PLN_GATING_SECTION_START_POINT	VFT_INT32
#define	SVS_PLN_GATING_SECTION_START_POINT	"Gating section start point"
/*
    �������Î~��Ԃ̊J�n����[msec]

    �ݒ�ҁFPROLIM
    �g�p�ҁFPROLIM
*/

#define	SVN_PLN_GATING_SECTION_END_POINT	(SVN_BASE_PROTOCOL_PLAN + 408)
#define	SVT_PLN_GATING_SECTION_END_POINT	VFT_INT32
#define	SVS_PLN_GATING_SECTION_END_POINT	"Gating section end point"
/*
    �������Î~��Ԃ̏I������[msec]

    �ݒ�ҁFPROLIM
    �g�p�ҁFPROLIM
*/

#define	SVN_PLN_CDS_SEQ_FLAG	(SVN_BASE_PROTOCOL_PLAN + 409)
#define	SVT_PLN_CDS_SEQ_FLAG	VFT_INT32
#define	SVS_PLN_CDS_SEQ_FLAG	"CDS sequence flag"
/*
    CDS�V�[�P���X���ǂ����̃t���O
    CDS�V�[�P���X       1
    Not CDS�V�[�P���X   0
    PAS�X�V�ΏۊO�B

    �ݒ�ҁFPROLIM
    �g�p�ҁFseqgen
*/

#define	SVN_PLN_MOVE_COUCH_METHOD_FLAG		(SVN_BASE_PROTOCOL_PLAN + 410)
#define	SVT_PLN_MOVE_COUCH_METHOD_FLAG		VFT_INT32
#define	SVS_PLN_MOVE_COUCH_METHOD_FLAG		"Move couch method flag"
/*
    �v�����ʒu�̒��S�����ꒆ�S�Ɉړ����邩�A�e�摜�̒��S�ʒu�����ꒆ�S��
    �ړ����邩��ݒ肷��t�B�[���h�B
    PAS�X�V�ΏہB
    VFC_MOVE_COUCH_METHOD_NONE              -1
    VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI  0
    VFC_MOVE_COUCH_METHOD_PARENT_POSI       1

    �ݒ�ҁFpqm
    �g�p�ҁFpqm,PROLIM
*/

#define	SVN_PLN_COPY_SOURCE_PROTOCOL_NO		(SVN_BASE_PROTOCOL_PLAN + 411)
#define	SVT_PLN_COPY_SOURCE_PROTOCOL_NO		VFT_INT32
#define	SVS_PLN_COPY_SOURCE_PROTOCOL_NO		"Copy source protocol number"
/*
    �v���X�L�������̃R�s�[���̃v���g�R���ԍ���ݒ肷��t�B�[���h�B
    �f�t�H���g�l��-1�B
    PAS����V�K�V�[�P���X��pqm�ɓǂݍ��񂾂Ƃ���-1��ݒ肷��B
    PAS�X�V�ΏۊO�B

    �ݒ�ҁFpqm,PROLIM
    �g�p�ҁFpqm
*/

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_PLAN	SVN_MIN_SUBPROTOCOL+SVN_OFF_PLAN


/* ---- Written by ProEdit */

#define SVN_PLN_SUBPROTOCOL_TIME	(SVN_BASE_SUBPROTOCOL_PLAN + 0)
#define SVT_PLN_SUBPROTOCOL_TIME	VFT_INT32
#define SVS_PLN_SUBPROTOCOL_TIME	"Time for subprotocol (predicted)"
/*	�T�u�v���g�R���P�ʂ̎��W���ԁB

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
/*	�X�L�����J�n�����B

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
/*	�T�u�v���g�R�����Ɋ܂܂��X���C�X���B

	This field indicates the number of spatially distinct slices acquired
	in this subprotocol.  This is not directly selected by the operator but
	is determined for each subprotocol by the ProLim tree restructuring
	routines.
	*/

#define SVN_PLN_NUM_FH_PS_IN_SP		(SVN_BASE_SUBPROTOCOL_PLAN + 4)
#define SVT_PLN_NUM_FH_PS_IN_SP		VFT_INT32
#define SVS_PLN_NUM_FH_PS_IN_SP		"Number of Freehand Presat regions in subprotocol"
/*	�t���[�n���h�T�`�����[�V�������B

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
/*	�t���[�n���h�^�O���B
	*/

#define SVN_PLN_NUM_SKIP_SAT		(SVN_BASE_SUBPROTOCOL_PLAN + 8)
#define SVT_PLN_NUM_SKIP_SAT		VFT_INT32
#define SVS_PLN_NUM_SKIP_SAT		"Number of skipping saturation"
/*	�T�`�����[�V�����p���X�ԂɊ܂܂��X���C�X����
	�����Q�l�̔z��B
	val[0] : �T�`�����[�V�����p���X�Ԃ̃X���C�X��
	val[1] : �T�u�v���g�R�����̑��T�`�����[�V�����p���X�Z�b�g��
	�P�X���C�X�ɑΉ�����T�`�����[�V�����p���X�Q���P�Z�b�g����
	*/

#define SVN_PLN_NUM_SKIP_FAT		(SVN_BASE_SUBPROTOCOL_PLAN + 9)
#define SVT_PLN_NUM_SKIP_FAT		VFT_INT32
#define SVS_PLN_NUM_SKIP_FAT		"Number of skipping fatsat"
/*	���b�}���p���X�ԂɊ܂܂��X���C�X����
	�����Q�l�̔z��B
	val[0] : ���b�}���p���X�Ԃ̃X���C�X��
	val[1] : �T�u�v���g�R�����̑����b�}���p���X��
	*/

#define SVN_PLN_NUM_PRESAT_FAT		(SVN_BASE_SUBPROTOCOL_PLAN + 10)
#define SVT_PLN_NUM_PRESAT_FAT		VFT_INT32
#define SVS_PLN_NUM_PRESAT_FAT		"Number of presat (fat)"
/*	�v���T�`�����[�V����(���b)�p���X��
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
/*	MRS�I��̈�̃I�t�Z�b�g�x�N�^�B
	*/

#define SVN_PLN_SLICES_IN_SG_SPEEDER	(SVN_BASE_SLICEGROUP_PLAN + 7)
#define SVT_PLN_SLICES_IN_SG_SPEEDER	VFT_INT32
#define SVS_PLN_SLICES_IN_SG_SPEEDER	"Number of slices per slice group for SPEEDER"
/*	SPEEDER�W�J������̃X���C�X�O���[�v�̖���
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

	�r�W���A���O���[�v���ɑ��݂���X���C�X���B�����ŁA�r�W���A���O���[�v��
	���[�U���ӎ�����X���u�ɑ�������B
        */

#define SVN_PLN_VG_SLICE_ORI_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 1)
#define SVT_PLN_VG_SLICE_ORI_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_SLICE_ORI_VECTOR	"Unit vector normal to slice visual group"
/*	This field is the unit vector in the patient's coordinate system normal
	to the plane of this visual group.  It is written by ProEdit and is
	determined by the operator's manipulations of the visual group in
	the Graphic Locator.

	�X���C�X�@���x�N�g���B�i�P�ʃx�N�g���j
	*/

#define SVN_PLN_VG_PHASE_ORI_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 2)
#define SVT_PLN_VG_PHASE_ORI_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PHASE_ORI_VECTOR	"Unit vector in PE direction of slice VG"
/*	This field is the unit vector in the patient's coordinate system along
	the phase encoding direction of this visual group.  It is written by
	ProEdit and is determined by the operator's manipulations of the visual
	group in the Graphic Locator.

	�t�F�[�Y�G���R�[�h�x�N�g���B�i�P�ʃx�N�g���j
	*/

#define SVN_PLN_VG_OFFSET_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 3)
#define SVT_PLN_VG_OFFSET_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_OFFSET_VECTOR	"Vector from magnet center to center of visual group's center slice"
/*	This field is a non-unit vector in the patient's coordinate system from
	the magnet's center to the center of the center slice of the indicated
	visual group.  It is written by ProEdit and is determined by the
	operator's manipulations of the visual group in the Graphic Locator.

	�I�t�Z�b�g�x�N�g���B
	*/

#define SVN_PLN_VG_PRESAT_THICKNESS	(SVN_BASE_VISUALGROUP_PLAN + 4)
#define SVT_PLN_VG_PRESAT_THICKNESS	VFT_FLT32
#define SVS_PLN_VG_PRESAT_THICKNESS	"Thickness of presaturation region"
/*	This field is the thickness of the presaturation region described by
	this visual group.  It is determined by the operator's manipulations of
	the presat region in the Graphic Locator.

	�v���T�b�g���B
	*/

#define SVN_PLN_PRESAT_TYPE		(SVN_BASE_VISUALGROUP_PLAN + 5)
#define SVT_PLN_PRESAT_TYPE		VFT_INT32
#define SVS_PLN_PRESAT_TYPE		"Type of presaturation region"
/*	This field indicates whether the presaturation region described by this
	visual group is a slice, phase, frequency, coverage, upstream,
	downstream or freehand presat.

	�v���T�b�g�^�B
		VFC_PRESAT_TYPE_SLICE
		VFC_PRESAT_TYPE_PE
		VFC_PRESAT_TYPE_RO
		VFC_PRESAT_TYPE_COVERAGE�i���g�p�j
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

	�r�W���A���O���[�v�ɑΉ�����T�u�v���g�R���X���C�X�O���[�v��
	�}�b�v���B�S�̐��l����Ȃ肻�ꂼ��́A
		�P	�擪�̃T�u�v���g�R��
		�Q	�擪�̃X���C�X�O���[�v
		�R	�ŏI�T�u�v���g�R��
		�S	�ŏI�X���C�X�O���[�v
	����́A�P�̃X���u�𕡐��̃X�L�����i�J�o���[�W�j�ŎB��ꍇ��
	�g�p������̂ł���B
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

	�v���T�b�g�^�B
		VFC_PRESAT_TYPE_SLICE
		VFC_PRESAT_TYPE_PE
		VFC_PRESAT_TYPE_RO
		VFC_PRESAT_TYPE_COVERAGE�i���g�p�j
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
/*	��]�n���h���̕ێ�
	*/

#define SVN_PLN_VG_PRESAT_PHASE_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 19)
#define SVT_PLN_VG_PRESAT_PHASE_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_PHASE_VECTOR	"Unit vector normal to presat visual group"
/*	�v���T�b�g�t�F�[�Y�G���R�[�h�x�N�g���B�i�P�ʃx�N�g���j
	*/

#define SVN_PLN_VG_PRESAT_LOC_VECTOR	(SVN_BASE_VISUALGROUP_PLAN + 20)
#define SVT_PLN_VG_PRESAT_LOC_VECTOR	VFT_FLTVEC3
#define SVS_PLN_VG_PRESAT_LOC_VECTOR	"Presat localizer vector for use by GL"
/*	This is another vector to help further define the positioning of the
	plane of a presat region.
	*/

#endif /* tami_vfStudy_vfs_plan_H__ */
