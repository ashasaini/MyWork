/*********************************************************************
 *
 *  (c) Copyright 1997 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Nov. 4.97	�Êڒ��K	Created
 * 2007-11-05	sr.Hong     2�G�R�[�摜�ɂ����鎩�������㏈���Ή�
 *                          PROTOCOL name space�ɒǉ�
 *                          SVN_PRC_DUAL_ECHO_COMBINING_TYPE
 *                          SVN_PRC_DUAL_ECHO_COMBINING_RATIO
 *                          SVN_PRC_DUAL_ECHO_DISPLAY_FLAG
 * End of History */



#ifndef tami_vfStudy_vfs_process_H__
#define tami_vfStudy_vfs_process_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_BASE_INDEPENDENT_PROCESS SVN_MIN_INDEPENDENT+SVN_OFF_PROCESS


/* The following are used to store:
      Default reconstruction parameters at the PROTOCOL  level
      Actual  reconstruction parameters at the PROCESSED level
   */

#define SVN_PRC_SELAVG_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 0)
#define SVT_PRC_SELAVG_CORR_FLAG	VFT_BOOL
#define SVS_PRC_SELAVG_CORR_FLAG	"Selective avg. correction flag"
/*      Central lines normalization before fft.
        */

#define SVN_PRC_DC_CORR_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 1)
#define SVT_PRC_DC_CORR_FLAG		VFT_BOOL
#define SVS_PRC_DC_CORR_FLAG		"DC correction flag"
/*      DC value estimation and subtraction from acquired data values
	before fft.
	*/

#define SVN_PRC_SMTH_REFINE_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 2)
#define SVT_PRC_SMTH_REFINE_FLAG	VFT_BOOL
#define SVS_PRC_SMTH_REFINE_FLAG	"Smooth refine correction flag"
/*      Low-pass filtration of acquired data before fft
        */

#define SVN_PRC_THRSH_REFINE_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 3)
#define SVT_PRC_THRSH_REFINE_FLAG	VFT_BOOL
#define SVS_PRC_THRSH_REFINE_FLAG	"Threshold refine correction flag"
/*      Threshold (cloak) filtration of acquired data before fft
        */

#define SVN_PRC_CIRC_REFINE_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 4)
#define SVT_PRC_CIRC_REFINE_FLAG	VFT_BOOL
#define SVS_PRC_CIRC_REFINE_FLAG	"Circular refine correction flag"
/*      Circular 2D low-pass filtration of acquired data before fft
        */

#define SVN_PRC_KZERO_CORR_FLAG   	(SVN_BASE_INDEPENDENT_PROCESS + 5)
#define SVT_PRC_KZERO_CORR_FLAG		VFT_BOOL
#define SVS_PRC_KZERO_CORR_FLAG		"K space zero correction flag"
/*      Constant phase shift of acquired to make the central sample zero-phase
        */

#define SVN_PRC_KCENTER_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 6)
#define SVT_PRC_KCENTER_FLAG		VFT_BOOL
#define SVS_PRC_KCENTER_FLAG		"K space centering flag"
/*      Resampling of acquired data before fft to make the central sample
	maximum-magnitude */

#define SVN_PRC_FID_CORR_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 7)
#define SVT_PRC_FID_CORR_FLAG		VFT_BOOL
#define SVS_PRC_FID_CORR_FLAG		"FID correction flag"
/*  	Suppressing the FID artifact after fft
        */

#define SVN_PRC_STIMECHO_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 8)
#define SVT_PRC_STIMECHO_CORR_FLAG	VFT_BOOL
#define SVS_PRC_STIMECHO_CORR_FLAG	"Stimulated echo correction flag"
/* 	Suppressing the stimulated echo artifact in the second echo after fft
        */

#define SVN_PRC_LEVEL_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 9)
#define SVT_PRC_LEVEL_FLAG		VFT_BOOL
#define SVS_PRC_LEVEL_FLAG		"Level correction flag"
/* 	Suppressing the drift of the  reconstructed (magnitude) image level
        */

#define SVN_PRC_PROC_ORDER		(SVN_BASE_INDEPENDENT_PROCESS + 10)
#define SVT_PRC_PROC_ORDER		VFT_INT32
#define SVS_PRC_PROC_ORDER		"Processing order"
/* 	Order of data dimensions in which to process
        */

#define SVN_PRC_ROPROC_OPTS		(SVN_BASE_INDEPENDENT_PROCESS + 11)
#define SVT_PRC_ROPROC_OPTS		VFT_RECONPROC_OPTS
#define SVS_PRC_ROPROC_OPTS 		"Readout processing flags"
/*	Processing options:
	fftFlag    - do fft,
	swapFlag   - swap data before fft,
	flipFlag   - flip data before fft,
	conjFlag   - do conjugation synthesis of missing data
	zoomFlag   - double the number of samples
	smoothFlag - apply low-pass filter before fft
	*/

#define SVN_PRC_PEPROC_OPTS 		(SVN_BASE_INDEPENDENT_PROCESS + 12)
#define SVT_PRC_PEPROC_OPTS 		VFT_RECONPROC_OPTS
#define SVS_PRC_PEPROC_OPTS		"Phase encode processing flags"
/* 	ditto
        */

#define SVN_PRC_SEPROC_OPTS 		(SVN_BASE_INDEPENDENT_PROCESS + 13)
#define SVT_PRC_SEPROC_OPTS 		VFT_RECONPROC_OPTS
#define SVS_PRC_SEPROC_OPTS 		"Slice encode processing flags"
/* 	ditto
        */

#define SVN_PRC_RPHASE_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 14)
#define SVT_PRC_RPHASE_CORR_FLAG	VFT_BOOL
#define SVS_PRC_RPHASE_CORR_FLAG	"R space phase correction flag"
/* 	Data centering before conjugation synthesis
        */


#define SVN_PRC_REORIENT_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 15)
#define SVT_PRC_REORIENT_FLAG 		VFT_BOOL
#define SVS_PRC_REORIENT_FLAG 		"Re orient data flag"
/* 	Reorientation of data after reconstruction
        */

#define SVN_PRC_ICON			(SVN_BASE_INDEPENDENT_PROCESS + 16)
#define SVT_PRC_ICON 			VFT_ICON_STRUCT
#define SVS_PRC_ICON 			"Image data icon structure"
/*	Icon structure:
	makeF	- make icon flag,
	type	- icon data type,
	side	- number of rows/columns in icon (icon is always square)
        */

#define SVN_PRC_T2_CORR_METHOD		(SVN_BASE_INDEPENDENT_PROCESS + 17)
#define SVT_PRC_T2_CORR_METHOD		VFT_INT32
#define SVS_PRC_T2_CORR_METHOD		"T2 correction method for fast scan sequences"
/*	T2�␳�p�̃t���O�l�A�摜�R���g���X�g�Ȃǂɂ��v���Z�b�g�l��ݒ�
	����B���ݗL���Ȓl�Ƃ��āA
	VFC_T2_CORR_NONE        0    �␳�Ȃ�
	VFC_T2_CORR_FOR_T2W     1    T�QW�p
	VFC_T2_CORR_FOR_T1W     2    T�PW�p
	VFC_T2_CORR_FOR_PD      3    PD�p
	VFC_T2_CORR_FREE        4    Wiener filter �ɂ��␳�p
	�v�f�Q�̂P�����z��Ƃ��A�f���A���R���g���X�g�ɑΉ�����B
	*/

#define SVN_PRC_PHASE_CORR_METHOD	(SVN_BASE_INDEPENDENT_PROCESS + 18)
#define SVT_PRC_PHASE_CORR_METHOD	VFT_INT32
#define SVS_PRC_PHASE_CORR_METHOD	"Phase correction method"
/*	Phase correction method.
	VFC_PHASE_CORR_NONE
	VFC_PHASE_CORR_FULL		Phase Rotation Full
	VFC_PHASE_CORR_CENTER		Phase Rotation Center
	VFC_PHASE_CORR_TIME		Time Domain
	*/

#define SVN_PRC_PE_SORT_METHOD		(SVN_BASE_INDEPENDENT_PROCESS + 19)
#define SVT_PRC_PE_SORT_METHOD		VFT_INT32
#define SVS_PRC_PE_SORT_METHOD		"Sort method (PE)"
/*	�t�F�[�Y�G���R�[�h�����̕��וς����[�����L�q����B
		VFC_SORT_NONE		0
		VFC_SORT_FREE		1
		VFC_SORT_CENTRIC	2
	VFC_SORT_FREE �̏ꍇ�̓\�[�g�e�[�u�����g�p����B
	*/

#define SVN_PRC_PE_SRTTBL_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 20)
#define SVT_PRC_PE_SRTTBL_TYPE		VFT_INT32
#define SVS_PRC_PE_SRTTBL_TYPE		"Sort table type (PE)"
/*	�t�F�[�Y�G���R�[�h�̕��וς��e�[�u���̃^�C�v(VFT_UINT16,VFTUINT32)
	���L�q����B
	*/

#define SVN_PRC_PE_SRTTBL_BITS		(SVN_BASE_INDEPENDENT_PROCESS + 21)
#define SVT_PRC_PE_SRTTBL_BITS		VFT_INT32
#define SVS_PRC_PE_SRTTBL_BITS		"The number of bits for index part"
/*	���בւ��e�[�u���̃C���f�b�N�X���̃r�b�g�����L�q����B
	*/


#define SVN_PRC_PE_SRTTBL_SIZE		(SVN_BASE_INDEPENDENT_PROCESS + 22)
#define SVT_PRC_PE_SRTTBL_SIZE		VFT_INT32
#define SVS_PRC_PE_SRTTBL_SIZE		"The number of entries in sort table"
/*	���בւ��e�[�u���̃G���g���[�����L�q����B
	*/

#define SVN_PRC_SE_SORT_METHOD		(SVN_BASE_INDEPENDENT_PROCESS + 23)
#define SVT_PRC_SE_SORT_METHOD		VFT_INT32
#define SVS_PRC_SE_SORT_METHOD		"Sort method (SE)"
/*	�X���C�X�G���R�[�h�����̕��וς����[�����L�q����B
		VFC_SORT_NONE		0
		VFC_SORT_FREE		1
		VFC_SORT_CENTRIC	2
	VFC_SORT_FREE �̏ꍇ�̓\�[�g�e�[�u�����g�p����B
	�]���X�L�����Ƃ̌݊�����ۂ��߁ARBC �ł̃\�[�g��
	Interleave Flag �ɌW��炸�AVFC_SORT_CENTRIC �̏ꍇ�̂�
	�ɍs�Ȃ��ARECON �ł̃\�[�g��VFC_SORT_CENTRIC �̏ꍇ�ɂ�
	�s�Ȃ�Ȃ��B
	*/

#define SVN_PRC_SE_SRTTBL_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 24)
#define SVT_PRC_SE_SRTTBL_TYPE		VFT_INT32
#define SVS_PRC_SE_SRTTBL_TYPE		"Sort table type (SE)"
/*	�X���C�X�G���R�[�h�̕��וς��e�[�u���̃^�C�v(VFT_UINT16,VFTUINT32)
	���L�q����B
	*/

#define SVN_PRC_SE_SRTTBL_BITS		(SVN_BASE_INDEPENDENT_PROCESS + 25)
#define SVT_PRC_SE_SRTTBL_BITS		VFT_INT32
#define SVS_PRC_SE_SRTTBL_BITS		"The number of bits for index part"
/*	���בւ��e�[�u���̃C���f�b�N�X���̃r�b�g�����L�q����B
	*/

#define SVN_PRC_SE_SRTTBL_SIZE		(SVN_BASE_INDEPENDENT_PROCESS + 26)
#define SVT_PRC_SE_SRTTBL_SIZE		VFT_INT32
#define SVS_PRC_SE_SRTTBL_SIZE		"The number of entries in sort table"
/*	���בւ��e�[�u���̃G���g���[�����L�q����B
	*/

#define SVN_PRC_PROCESSING_CODE		(SVN_BASE_INDEPENDENT_PROCESS + 27)
#define SVT_PRC_PROCESSING_CODE		VFT_STRING
#define SVS_PRC_PROCESSING_CODE		"Process code for reconstruction"
/*	�č\���Ŏg�p�����t�B���^�[�����ʂ��邽�߂̕�����B
	�P�������P�����ɑ�������B�t�B���^�[�̑��[���l�ߏ����Ȃǂ�
	���ʂɂ��g�p����B�\���n�͂��̕���������̂܂ܕ\������B
	�č\���ɂ���ăv���Z�X�h�m�[�h�ɃZ�b�g�����B
	*/

#define SVN_PRC_PROTECT_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 28)
#define SVT_PRC_PROTECT_FLAG		VFT_INT32
#define SVS_PRC_PROTECT_FLAG		"Protect flag"
/*	SVN_ST_PROTOCOL,SVN_ST_PROCESSED �ɋL�q�����t�B�[���h�B
        ���ꂼ��ARawData,Image�̃v���e�N�g�̗L����\���B�������A�݊����ێ�
	�̂��߁A�R�[�h�����݂��Ȃ��ꍇ�ɂ́A�v���e�N�g����Ă���Ƃ݂Ȃ��B
  	*/

#define SVN_PRC_DC_SHAVE		(SVN_BASE_INDEPENDENT_PROCESS + 29)
#define SVT_PRC_DC_SHAVE		VFT_BOOL
#define SVS_PRC_DC_SHAVE		"The flag for shaving"
/*	DC�A�[�`�t�@�N�g���������i�ʏ�颒��j�s�Ȃ����ۂ����w�肷��B
	SVN_ST_PROTOCOL,SVN_ST_PROCESSED �ɋL�q�����B
	*/

#define SVN_PRC_PE_SRTTBL		(SVN_BASE_INDEPENDENT_PROCESS + 30)
#define SVS_PRC_PE_SRTTBL		"PE sorting table"
/*	���בւ��e�[�u���B�^�� SVN_PRC_SE_SRTTBL_TYPE �ɂ���Č��܂�B
	�e�[�u���̃G���g���[�̓X���u�ԍ��i��ʃr�b�g�j�ƃX���C�X�ԍ��i����
	�r�b�g�j�����B�X���C�X���̃r�b�g���� SVN_PRC_SE_SRTTBL_BITS
	�ɂ���Č��܂�X���u���̃r�b�g���� SVN_PRC_SE_SRTTBL_TYPE ��
	SVN_PRC_SE_SRTTBL_BITS �ɂ���Čv�Z�����B
	�e�[�u��������N���i�����j�ł���A���ꂼ��̃X���C�X�ʒu��
	�X���u�ԍ��F�X���C�X�ԍ�
	�ɂ���`�����B
	*/

#define SVN_PRC_SE_SRTTBL		(SVN_BASE_INDEPENDENT_PROCESS + 31)
#define SVS_PRC_SE_SRTTBL		"SE sorting table"
/*	SVN_PRC_PE_SRTTBL �Q�ƁB
	*/

#define SVN_PRC_REFINE_FILTER		(SVN_BASE_INDEPENDENT_PROCESS + 32)
#define SVT_PRC_REFINE_FILTER		VFT_INT32
#define SVS_PRC_REFINE_FILTER		"Refine filter ID"
/*	���t�@�C���t�B���^�[�h�c�B
	��ʂP�U�r�b�g���t�B���^�[��A���ʂP�U�r�b�g���C���f�b�N�X
	�Ƃ���B�t�B���^�[�W���E�\�����̓t�B���^�[�c�a�ɒu�����B
	�ݒ�҂ɂ��v���g�R���m�[�h�Ƀt�B�[���h���쐬�������Z�b�g
	����B�č\���̓v���g�R���m�[�h����v���Z�X�h�m�[�h�ɃR�s�[��
	��B�t�B���^�[�h�c���O�̓t�B���^�[���|���Ȃ����Ƃ��Ӗ�����B
	SVN_ST_PROTOCOL,SVN_ST_PROCESSED �ɐݒ肷��B
	*/

#define SVN_PRC_K_SPACE_FILTER		(SVN_BASE_INDEPENDENT_PROCESS + 33)
#define SVT_PRC_K_SPACE_FILTER 		VFT_INT32
#define SVS_PRC_K_SPACE_FILTER		"K Space filter ID"
/*	�j��ԃt�B���^�[�h�c�B
	��ʂP�U�r�b�g���t�B���^�[��A���ʂP�U�r�b�g���C���f�b�N�X
	�Ƃ���B�t�B���^�[�W���E�\�����̓t�B���^�[�c�a�ɒu�����B
	�ݒ�҂ɂ��v���g�R���m�[�h�Ƀt�B�[���h���쐬�������Z�b�g
	����B�č\���̓v���g�R���m�[�h����v���Z�X�h�m�[�h�ɃR�s�[��
	��B�t�B���^�[�h�c���O�̓t�B���^�[���|���Ȃ����Ƃ��Ӗ�����B
	*/

#define SVN_PRC_BARIQ_MODE		(SVN_BASE_INDEPENDENT_PROCESS + 34)
#define SVT_PRC_BARIQ_MODE		VFT_INT32
#define SVS_PRC_BARIQ_MODE		"BariQ Mode"
/*	�ǉ��F�p���X�V�[�P���X�ł�RF/ADC �ʑ��̃R���g���[���ɂ��ADC 
	�A�[�`�t�@�N�g���摜��Ɍ����ʒu�����S�^�ʑ��G���R�[�h������
	�摜�[�ƕω����邽�߁A�č\�����ł̕␳������ɍ��킹�ĕύX����B
	�V���b�g���������ɂƂ��V�[�P���X�i���s�̂قƂ�ǂ̃V�[�P���X�j
	�ł͌��݂̏�������̕ύX�Ȃ��B

	VFC_BARIQ_NONE		0
	VFC_BARIQ_ON		1
	*/

#define SVN_PRC_WIENER_CORR_METHOD	(SVN_BASE_INDEPENDENT_PROCESS + 35)
#define SVT_PRC_WIENER_CORR_METHOD	VFT_BITFIELD
#define SVS_PRC_WIENER_CORR_METHOD	"Weiner filter method"
/*	Weiner filter �̗L���ƁA�K�p����鎲���w�肷��t���O�B���ɂ��Ă� 
	Weiner filter �̗L���ƁA�K�p����鎲���w�肷��t���O�B���ɂ��Ă� 
	Bit Or �őI������B
	VFC_WIENER_CORR_NONE		0x0000	�␳�Ȃ�
	VFC_WIENER_CORR_ON		0x0001
	VFC_WIENER_SE_TEMPLATE		0x0010
	VFC_WIENER_SE_ESTIMATE		0x0020
	VFC_WIENER_SE_STEP		0x0030
	VFC_WIENER_PE_TEMPLATE		0x0100
	VFC_WIENER_PE_ESTIMATE		0x0200
	VFC_WIENER_PE_STEP		0x0300
	VFC_WIENER_RO_TEMPLATE		0x1000
	VFC_WIENER_RO_ESTIMATE		0x2000
	VFC_WIENER_RO_STEP		0x3000
*/

#define SVN_PRC_WIENER_NOISE_SCALE	(SVN_BASE_INDEPENDENT_PROCESS + 36)
#define SVT_PRC_WIENER_NOISE_SCALE	VFT_FLT32
#define SVS_PRC_WIENER_NOISE_SCALE	"Weiner noise scale"
/*	Weiner filter �̋��x�����肷��p�����[�^�B�ڂ����̓\�t�g�E�F�A
	�V�X�e���d�l��SAM45-0101 ���Q�Ƃ̂��ƁB��`����Ȃ��ꍇ�̃f�t�H���g
	�l��1.0 �Ƃ���B
*/

#define SVN_PRC_T2_ESTIMATE_TIME	(SVN_BASE_INDEPENDENT_PROCESS + 37)
#define SVT_PRC_T2_ESTIMATE_TIME	VFT_FLT32
#define SVS_PRC_T2_ESTIMATE_TIME	"T2 Estimate Time"
/*	T2 �␳�@�̊g���ɔ����A�e���v���[�g���W���s�\�ȃ����V���b�g
	�V�[�P���X�ARO �����Ȃǂɂ�T2 �␳���\�Ƃ���B�������A�e���v���[�g
	�ɂ�茸�������Z�o�ł��Ȃ����߁A�t�B���^�[�W���ȂǂƓ��l�Ƀ��[�U�[��
	���͉\�ȃp�����[�^�Ƃ���B�e���v���[�g�f�[�^�Ƃ̗D�揇�ʂ�T2 �␳
	���[�h�ɂ�茈�肷��B���̎��ԃp�����[�^�Ɠ��l�A�b�P�ʂł̐ݒ�Ƃ���B
*/

#define SVN_PRC_PS_RECON_IMAGE		(SVN_BASE_INDEPENDENT_PROCESS + 38)
#define SVT_PRC_PS_RECON_IMAGE		VFT_BITFIELD
#define SVS_PRC_PS_RECON_IMAGE		"Phase Shift reconstruction images"
/*	PS�[MRA�č\���ōč\������摜��̎��

	���X�s�[�h�摜
	VFC_PS_RECON_SPEED_PE		0x0001
	VFC_PS_RECON_SPEED_RO		0x0002
	VFC_PS_RECON_SPEED_SE		0x0004
	���t���[�摜
	VFC_PS_RECON_FLOW_PE		0x0010
	VFC_PS_RECON_FLOW_RO		0x0020
	VFC_PS_RECON_FLOW_SE		0x0040
	���x�b�Z���摜
	VFC_PS_RECON_VESSEL		0x0100
	���v���g���摜
	VFC_PS_RECON_PROTON		0x1000
*/

#define SVN_PRC_PS_COMBINATION		(SVN_BASE_INDEPENDENT_PROCESS + 40)
#define SVT_PRC_PS_COMBINATION		VFT_BITFIELD
#define SVS_PRC_PS_COMBINATION		"PS Combination"
/*	�e��������(PE,RO,SL)�Ɋւ���A���W�f�[�^�̉����Z�̕���
	PE,RO,SE �̏��ɔz��Ƃ��Ċi�[����B

	VFC_PS_COMB_NONE		0
	VFC_PS_COMB_ADD			1
	VFC_PS_COMB_SUB			2

	VFD_PS_COMBINATION(n,t)		(t<<2*(n-1))

	31				0
	---------------------------------
	|				|
	---------------------------------
				 ||||||||
				 --------
				 | | | |  --> ��P���W	00:None
				 | | |  --> ��Q���W	01:Addition
				 | |  --> ��R���W	10:Subtraction
				 |  --> ��S���W

	�ő�U�_�@�܂ŃT�|�[�g����B�i�Q�_�@������j

	Default:v[0] = -+-+  --> 102	(PE)
	        v[1] = -++-  --> 150	(RO)
		v[2] = --++  -->  90	(SE)
*/

#define SVN_PRC_PS_THRESHOLD		(SVN_BASE_INDEPENDENT_PROCESS + 41)
#define SVT_PRC_PS_THRESHOLD		VFT_FLT32
#define SVS_PRC_PS_THRESHOLD		"Phase shift threshold"
/*	PS-MRA�č\���ł�臒l�����Ŏg�p����
	�l��COEFICIENT,LOW,HIGH�̏��ɔz��Ŋi�[����

	v[0],COEFICIENT:臒l�����̌W����\��
	v[1],LOW       :���ω摜�l�̂����ȉ���臒l���������邩��\��
	v[2],HIGH      :���ω摜�l�̂����ȏ��臒l���������邩��\��
	
	Default: COEFFICIENT = 0.0
	         LOW	     = 1.0
		 HIGH	     = 1.5
*/

#define SVN_PRC_PS_LPF_KC		(SVN_BASE_INDEPENDENT_PROCESS + 42)
#define SVT_PRC_PS_LPF_KC		VFT_FLT32
#define SVS_PRC_PS_LPF_KC		"PS LPF KC"
/*	PS-MRA�č\���ł̒���g�t�B���^�[�����Ŏg�p����
	�e���ڂ�AFI�ŗp������p�����[�^�Ɠ����Ӗ�������

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_PS_LPF_K1		(SVN_BASE_INDEPENDENT_PROCESS + 43)
#define SVT_PRC_PS_LPF_K1		VFT_FLT32
#define SVS_PRC_PS_LPF_K1		"PS LPF K1"
/*	PS-MRA�č\���ł̒���g�t�B���^�[�����Ŏg�p����
	�e���ڂ�AFI�ŗp������p�����[�^�Ɠ����Ӗ�������

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_PS_LPF_K2		(SVN_BASE_INDEPENDENT_PROCESS + 44)
#define SVT_PRC_PS_LPF_K2		VFT_FLT32
#define SVS_PRC_PS_LPF_K2		"PS LPF K2"
/*	PS-MRA�č\���ł̒���g�t�B���^�[�����Ŏg�p����
	�e���ڂ�AFI�ŗp������p�����[�^�Ɠ����Ӗ�������

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_WIENER_SE_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 45)
#define SVT_PRC_WIENER_SE_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_SE_DECAY_TABLE	"Wiener SE Decay table"
/*	  Wiener filter �̌����␳��\���e�[�u���BSVN_PRC_WIENER_CORR_METHOD
	�ɂăG�R�[�f�[�^���W�Ԋu�e�[�u��(VFC_WEINER_SE_ESTIMATE)����`����Ă���
	�ꍇ�ɎQ�Ƃ����B
	  �e���v���[�g���W����␳�������(VFC_WEINER_SE_TEMPLATE)�ɂ��Ă�
	�]����T2 �␳�Ɠ��l�e���v���[�g�f�[�^����v�Z�����������𗘗p����B
*/

#define SVN_PRC_WIENER_PE_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 46)
#define SVT_PRC_WIENER_PE_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_PE_DECAY_TABLE	"Wiener PE Decay table"
/*	  Wiener filter �̌����␳��\���e�[�u���BSVN_PRC_WIENER_CORR_METHOD
	�ɂăG�R�[�f�[�^���W�Ԋu�e�[�u��(VFC_WEINER_PE_ESTIMATE)����`����Ă���
	�ꍇ�ɎQ�Ƃ����B
	  �e���v���[�g���W����␳�������(VFC_WEINER_PE_TEMPLATE)�ɂ��Ă�
	�]����T2 �␳�Ɠ��l�e���v���[�g�f�[�^����v�Z�����������𗘗p����B
*/

#define SVN_PRC_WIENER_RO_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 47)
#define SVT_PRC_WIENER_RO_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_RO_DECAY_TABLE	"Wiener RO Decay table"
/*	  Wiener filter �̌����␳��\���p�����[�^�BSVN_PRC_WIENER_CORR_METHOD
	�ɂăG�R�[�f�[�^���W�Ԋu�e�[�u��(VFC_WEINER_RO_STEP)����`����Ă���
	�ꍇ�ɎQ�Ƃ����B�����o�[�Ƃ��ẮA�X�^�[�g�l�ƃX�e�b�v�ƂȂ�B
	ESTIMATE_T2 �Ƃ��킹�A�����I�Ɍ��������v�Z���邽�߂ɗp������B
	(table �Ŏ�������ƃT���v�����O�_�����p�ӂ���K�v������A�����������j
*/

#define SVN_PRC_PHASE_CORR_DATA		(SVN_BASE_INDEPENDENT_PROCESS + 48)
#define SVT_PRC_PHASE_CORR_DATA		VFT_FLT32
#define SVS_PRC_PHASE_CORR_DATA		"Phase correction parameter for zero shot phase correction"
/*
	  Multi-Shot EPI / Hybrid EPI �ɂ����āA�ʑ��␳���O
	�V���b�g���[�h�̏ꍇ�ɕ␳�Ɏg�p����l�i���[�h������
	�ʑ��␳�ʁj����ł́A1���܂ł̌W����ݒ肷��B
	�v�f���T�̔z��ɌW������������B
	[�P��]
	�O���̈ʑ��␳�ʁB(�P�ʁF�x)
	�P���̈ʑ��␳�ʁB(�P�ʁF�x�^points)
*/

#define SVN_PRC_FSE_PHASE_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 49)
#define SVT_PRC_FSE_PHASE_CORR_FLAG	VFT_BOOL
#define SVS_PRC_FSE_PHASE_CORR_FLAG	"Fast Spin Echo phase correction flag"
/*	Reducing phase discontinuity at the segment boundaries
	in FSE time-domain data
	*/

#define SVN_PRC_FSE_T2_CORR		(SVN_BASE_INDEPENDENT_PROCESS + 50)
#define SVT_PRC_FSE_T2_CORR		VFT_INT32
#define SVS_PRC_FSE_T2_CORR		"Fast Spin Echo T2 correction code"
/*	Reducing type of T2 correction for fse data.
	*/

#define SVN_PRC_PC_PHASE_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 51)
#define SVT_PRC_PC_PHASE_CORR_FLAG	VFT_BOOL
#define SVS_PRC_PC_PHASE_CORR_FLAG	"phase contrast phase correction flag"
/*	Phase correction applied during reconstruction of
	phase contrast angio data
	*/

#define SVN_PRC_ROW_ID			(SVN_BASE_INDEPENDENT_PROCESS + 52)
#define SVT_PRC_ROW_ID			VFT_INT32
#define SVS_PRC_ROW_ID			"Row ID on image selector"
/*	�s����肷�� 1 �ȏ�̐����B
	Echo Node �܂��� Position Node �ɓo�^����B
	���� row-id �����摜�͓����s�ɒu�����B
	row-id �� Processed ���� unique �Ȓl�ƂȂ�B
	*/

#define SVN_PRC_GROUP_ID		(SVN_BASE_INDEPENDENT_PROCESS + 53)
#define SVT_PRC_GROUP_ID		VFT_INT32
#define SVS_PRC_GROUP_ID		"Group ID on image selector"
/*	�s�̒��̃O���[�v����肷�� 1 �ȏ�̐����B
	Echo Node �܂��� Position Node �ɓo�^����B
	���� row-id, group-id �����摜�͓����s�̈�܂Ƃ܂�Ƃ���
	�z�u�����B�قȂ� group-id �Ԃɂ͋󔒂��}�������B
	(�]���̃}���`�X���u�Ɠ��l)
	group-id �͍s���� unique �Ȓl�ƂȂ�B
	*/

#define SVN_PRC_IMG_INFO_DATA		(SVN_BASE_INDEPENDENT_PROCESS + 54)
#define SVT_PRC_IMG_INFO_DATA		VFT_UINT8
#define SVS_PRC_IMG_INFO_DATA		"Image Infomation Data"
/*	�摜��ʂɈقȂ�ŗL���(�摜��ʌŗL���)
	�f�[�^���y�уf�[�^�������ɉϒ��ɂȂ��Ă���̂�
	�^�C�v��VFT_UINT8�Ƃ���B
	�A�������͈ȉ��̗l�ȍ\�������Ă���B
	typedef struct {
	    size_t	   	n		;  �f�[�^�� 
	    struct {
	 	unit32_t   	infoTag		;  �f�[�^�� 
	 	size_t		length		;  �f�[�^�� 
	 	char	   	infoData[length];  ���f�[�^ 
	    }		infoDatas[n] ;
	}ImgInfodata_t ;
	�擪�Ƀf�[�^����(n)���i�[����A����ȍ~�ɂ͊e�f�[�^��
	�^�O(infoTag)�A�T�C�Y(length)�A���f�[�^(infoData)��
	�Z�b�g�Ŋi�[�����B���ꂪ�f�[�^����(n)���i�[�����B

	���������v���Z�X�͕K�v�ɉ����� processed, echo �܂��� 
	position �m�[�h�Ɋi�[����B
	��j�C���Z�b�g���iMIP�AMPR�j�A�W���i�摜�ԉ��Z�A�t�B���^�j
	*/
  
#define SVN_PRC_SNAP_CORR_DATA (SVN_BASE_INDEPENDENT_PROCESS + 55)
#define SVS_PRC_SNAP_CORR_DATA "Copied Image data for Snap mode data correction"
/*	�ʑ��␳�̂��߁A�e���v���[�g�݂̂��܂񂾃X�L�����Ƃ��Ď��s
	���ꂽ�v���g�R���������̓v���X�L�����̃X�^�f�B�t�@�C������
	SVN_GEN_DATA���R�s�[���邱�Ƃɂ��쐬����B
	�쐬�҂́A�v���X�L�����̃R���g���[�����s���v���Z�X
	(pqm or acqman)
	�g�p�҂́A�č\��
	This data is copied from SVN_GEN_DATA for phase correction
	scan.
	Creater: pqm or acqman who control prescan process.
	User: recon
	
	*/

#define SVN_PRC_SNAP_CORR_DATA_TYPE (SVN_BASE_INDEPENDENT_PROCESS + 56)
#define SVT_PRC_SNAP_CORR_DATA_TYPE VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_TYPE "Data type for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA �̃f�[�^�^�C�v
	Data type for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_DIM (SVN_BASE_INDEPENDENT_PROCESS + 57)
#define SVT_PRC_SNAP_CORR_DATA_DIM VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_DIM "Data dimension for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA �̃f�[�^�T�C�Y
	Data dimension for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_FORM (SVN_BASE_INDEPENDENT_PROCESS + 58)
#define SVT_PRC_SNAP_CORR_DATA_FORM VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_FORM "Data form for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA �̃f�[�^�^
	Data form for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_DESC (SVN_BASE_INDEPENDENT_PROCESS + 59)
#define SVT_PRC_SNAP_CORR_DATA_DESC VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_DESC "Data dimension discription for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA �̃f�[�^�f�B�X�N���v�V����
	Data dimension discription for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL (SVN_BASE_INDEPENDENT_PROCESS + 60)
#define SVS_PRC_SNAP_CORR_PE_SRTTBL "PE sorting table"
/*	�ʑ��␳�p�f�[�^���W�ɂ��A�쐬�������בւ��e�[�u���B
	�^�� SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE �ɂ���Č��܂�B
	�e�[�u���̃G���g���[�̓A�g���r���[�g�i��ʃr�b�g�j��
	�ʑ��G���R�[�h�ʒu�i���ʃr�b�g�j�����B�ʑ��G���R�[�h
	�ʒu���̃r�b�g���� SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS��
	����Č��܂�A�g���r���[�g���̃r�b�g���� SVN_PRC_SNAP_
	CORR_SRTTBL_TYPE ��SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS 
	�ɂ���Čv�Z�����B
	�e�[�u��������N���i�����j�ł���A���ꂼ��̈ʑ��G���R
	�[�h�ʁi����Ԕz�u�j�A�e���v���[�g���Ȃǂ̃A�g���r��
	�[�g��񂪍č\���ɓ`������B
	
	seqgen�ɂ���č쐬���ꂽSVN_PRC_PE_SRTTBL����ʑ��␳
	�̃R���g���[���v���Z�X(qpm or acqman)�ɂ��R�s�[����
	��B
	�g�p�҂́A�č\��
	
	Table for phase correction data. Data type determined 
	by SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE. Each entry has
	attribute and pe order parts. Bit length for pe order
	part determined by SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS and
	attribute part calculated by SVN_PRC_SNAP_CORR_PE_SRTTBL
	_TYPE and SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS.
	Table order represent acqusition order, pe order is
	location for k space, attribute is template information.
	
	seqgen generate SVN_PRC_PE_SRTTBL and prescan control
	process copies SVN_PRC_PE_SRTTBL to SVN_PRC_SNAP_CORR
	_PE_SRTTBL.
	User: Recon
	
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS (SVN_BASE_INDEPENDENT_PROCESS + 61)
#define SVT_PRC_SNAP_CORR_PE_SRTTBL_BITS VFT_INT32
#define SVS_PRC_SNAP_CORR_PE_SRTTBL_BITS "The number of bits for index part"
/*	SVN_PRC_SNAP_CORR_PE_SRTTBL��pe order���̒���
	seqgen�ɂ���č쐬���ꂽSVN_PRC_PE_SRTTBL_BITS����ʑ��␳
	�̃R���g���[���v���Z�X(qpm or acqman)�ɂ��R�s�[�����B
	�g�p�҂́A�č\��
	
	seqgen generate SVN_PRC_PE_SRTTBL_BITS and prescan 
	control process copies SVN_PRC_PE_SRTTBL_BITS to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS.
	User: Recon
	
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE (SVN_BASE_INDEPENDENT_PROCESS + 62)
#define SVT_PRC_SNAP_CORR_PE_SRTTBL_TYPE VFT_INT32
#define SVS_PRC_SNAP_CORR_PE_SRTTBL_TYPE "Sort table type (PE)"
/*	SVN_PRC_SNAP_CORR_PE_SRTTBL�̃f�[�^�^
	seqgen�ɂ���č쐬���ꂽSVN_PRC_PE_SRTTBL_TYPE����ʑ��␳
	�̃R���g���[���v���Z�X(qpm or acqman)�ɂ��R�s�[�����B
	�g�p�҂́A�č\��
	
	seqgen generate SVN_PRC_PE_SRTTBL_TYPE and prescan 
	control process copies SVN_PRC_PE_SRTTBL_TYPE to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE.
	User: Recon
	
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL_SIZE (SVN_BASE_INDEPENDENT_PROCESS + 63)
#define SVT_PRC_SNAP_CORR_PE_SRTTBL_SIZE VFT_INT32
#define SVS_PRC_SNAP_CORR_PE_SRTTBL_SIZE "The number of entries in sort table"
/*	SVN_PRC_SNAP_CORR_PE_SRTTBL�̃G���g���[��
	seqgen�ɂ���č쐬���ꂽSVN_PRC_PE_SRTTBL_SIZE����ʑ��␳
	�̃R���g���[���v���Z�X(qpm or acqman)�ɂ��R�s�[�����B
	�g�p�҂́A�č\��
	
	seqgen generate SVN_PRC_PE_SRTTBL_SIZE and prescan 
	control process copies SVN_PRC_PE_SRTTBL_SIZE to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_SIZE.
	User: Recon
	
	*/

#define SVN_PRC_CARDIAC_OUTLINE		(SVN_BASE_INDEPENDENT_PROCESS + 64)
#define SVT_PRC_CARDIAC_OUTLINE		VFT_CARDIAC_OUTLINE
#define SVS_PRC_CARDIAC_OUTLINE		"Outline point of the cardiac muscle."
/*	�摜��̐S�؂̗֊s�_�f�[�^
	CFA(Cardiac Functional Analysis)�Ŏg�p
	Outline point of the cardiac muscle.
	This data is used by CFA(Cardiac Functional Analysis).
	
	*/

#define SVN_PRC_VIEW_SORT_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 65)
#define SVT_PRC_VIEW_SORT_TABLE	VFT_INT32
#define SVS_PRC_VIEW_SORT_TABLE	"View Sort Table"
/*	�_�C�i�~�b�N�Ŏ��W���ꂽ3D���[�f�[�^����A�C�ӂ̎��W�����ƈʒu��
	�G�R�[�𒊏o���čč\�����邽�߂̃e�[�u���B�i�P�����z��j

	�� i �Ԗڂ̒��o��̃f�[�^�́A���o�O�i���W���j�̑� j �Ԗڂ̃G�R�[
	�f�[�^�ɑΉ�����Ƃ���ƁA
	j = SVN_PRC_VIEW_SORT_TABLE[i]
	�Ƃ����Ή��ɂȂ�B���o��̃f�[�^�̑����́A���o��̃f�[�^�̑�����
	�K��������v���Ȃ��B�܂��A����̃G�R�[�f�[�^�������񒊏o������
	��������B
	*/

#define SVN_PRC_VIEW_SRTTBL_SIZE	(SVN_BASE_INDEPENDENT_PROCESS + 66)
#define SVT_PRC_VIEW_SRTTBL_SIZE	VFT_INT32
#define SVS_PRC_VIEW_SRTTBL_SIZE	"Size of View Sort Table"
/*	VIEW_SORT_TABLE�i�P�����z��j�̗v�f���������ϐ��B
	�l�͈̔́F1����pe�����̍ő�matrixSize���X���C�X�����̍ő�matrixSize��
	�_�C�i�~�b�N�̍ő���W�������܂�
	*/

#define SVN_PRC_VIEW_SRTTBL_TYPE	(SVN_BASE_INDEPENDENT_PROCESS + 67)
#define SVT_PRC_VIEW_SRTTBL_TYPE	VFT_INT32
#define SVS_PRC_VIEW_SRTTBL_TYPE	"Type of View Sort Table"
/*	3D DRKS�ɂ����āA���ѕς��鏇���������e�[�u��VIEW_SORT_TABLE
	�̃^�C�v(VFT_UINT16,VFTUINT32)	���L�q����B
	*/

#define SVN_PRC_VIEW_SORT_METHOD	(SVN_BASE_INDEPENDENT_PROCESS + 68)
#define SVT_PRC_VIEW_SORT_METHOD	VFT_INT32
#define SVS_PRC_VIEW_SORT_METHOD	"View Sort Method"
/*	�_�C�i�~�b�N�Ŏ��W���ꂽ3D���[�f�[�^����A�C�ӂ̎��W�����ƈʒu��
	�G�R�[�𒊏o���čč\������A������viewSorting�����郋�[�����L�q�B
	VFC_VIEW_NONE	 0  �F�Ȃɂ����Ȃ��B
	VFC_VIEW_FREE	 1  �FVIEW_SORT_TABLE���g�p����B
	*/

#define SVN_PRC_SNAP_CORR_DATA1 (SVN_BASE_INDEPENDENT_PROCESS + 69)
#define SVS_PRC_SNAP_CORR_DATA1 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA2 (SVN_BASE_INDEPENDENT_PROCESS + 70)
#define SVS_PRC_SNAP_CORR_DATA2 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA3 (SVN_BASE_INDEPENDENT_PROCESS + 71)
#define SVS_PRC_SNAP_CORR_DATA3 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA4 (SVN_BASE_INDEPENDENT_PROCESS + 72)
#define SVS_PRC_SNAP_CORR_DATA4 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA5 (SVN_BASE_INDEPENDENT_PROCESS + 73)
#define SVS_PRC_SNAP_CORR_DATA5 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA6 (SVN_BASE_INDEPENDENT_PROCESS + 74)
#define SVS_PRC_SNAP_CORR_DATA6 "Copied Image data for Snap mode data correction"

#define SVN_PRC_SNAP_CORR_DATA7 (SVN_BASE_INDEPENDENT_PROCESS + 75)
#define SVS_PRC_SNAP_CORR_DATA7 "Copied Image data for Snap mode data correction"
/*
  SVN_PLN_SNAP_SHOT_MODE == 1�̏ꍇ�ɗ��p�����␳�f�[�^�B
  V6.50�ł́ASVN_PRC_SNAP_CORR_DATA1����SVN_PRC_SNAP_CORR_DATA7�܂�
  �t�B�[���h�����ǉ�����B�ǉ����R�́A�␳�f�[�^��PAC�Ή��K�v�Ȃ��߁A
  �ʑ��␳�̂��߁A�e���v���[�g�݂̂��܂񂾃X�L�����Ƃ��Ď��s
  ���ꂽ�v���g�R���������̓v���X�L�����̃X�^�f�B�t�@�C������
  SVN_GEN_DATA���R�s�[���邱�Ƃɂ��쐬����B
  �쐬�҂́A�v���X�L�����̃R���g���[�����s���v���Z�X
  (pqm or acqman)
  �g�p�҂́A�č\��
  This data is copied from SVN_GEN_DATA* for phase correction
  scan.
  Creater: pqm or acqman who control prescan process.
  User: recon

*/


/* GAP in numbering..... */


/* The following are defined here to distinguish between processed and
   intermediate data type and form. These fields are input parameters
   to reconstruction. They do not describe an actual data field in
   the study file.
   */

#define SVN_PRC_IDATA_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 100)
#define SVT_PRC_IDATA_TYPE		VFT_INT32
#define SVS_PRC_IDATA_TYPE		"Intermediate data type"

#define SVN_PRC_IDATA_FORM		(SVN_BASE_INDEPENDENT_PROCESS + 101)
#define SVT_PRC_IDATA_FORM		VFT_INT32
#define SVS_PRC_IDATA_FORM		"Intermediate data form"


#define SVN_PRC_CLOAK_THRESH		(SVN_BASE_INDEPENDENT_PROCESS + 102)
#define SVT_PRC_CLOAK_THRESH 		VFT_FLT32
#define SVS_PRC_CLOAK_THRESH		"Cloak threshold"
/*	 The threshold used in cloak filtering
        */

#define SVN_PRC_ROLLOFF_NUM 		(SVN_BASE_INDEPENDENT_PROCESS + 103)
#define SVT_PRC_ROLLOFF_NUM		VFT_INT32
#define SVS_PRC_ROLLOFF_NUM		"Number of samples rolled off"
/*	Rolling off this number of samples while making in conjugation
	synthesis.  Can be 0
        */

#define SVN_PRC_SE_TRANSLATE		(SVN_BASE_INDEPENDENT_PROCESS + 104)
#define SVT_PRC_SE_TRANSLATE		VFT_FLT32
#define SVS_PRC_SE_TRANSLATE		"Slice encode translation"
/* 	Reslicing the images after reconstruction
        */

#define SVN_PRC_GAIN	   		(SVN_BASE_INDEPENDENT_PROCESS + 105)
#define SVT_PRC_GAIN     		VFT_FLT32
#define SVS_PRC_GAIN	   		"Intensity multiplier for recon"
/*	Multiply data by this constant before fft to avoid clipping the data
	after fft while formatting
	*/

#define SVN_PRC_RO_KERNEL		(SVN_BASE_INDEPENDENT_PROCESS + 106)
#define SVT_PRC_RO_KERNEL		VFT_FLT32
#define SVS_PRC_RO_KERNEL		"Readout smoothing filter kernel"
/*	Convolution kernel used to create the smoothing filter in readout
	dimension
	*/

#define SVN_PRC_PE_KERNEL		(SVN_BASE_INDEPENDENT_PROCESS + 107)
#define SVT_PRC_PE_KERNEL		VFT_FLT32
#define SVS_PRC_PE_KERNEL		"Phase encode smoothing filter kernel"
/*	Convolution kernel used to create the smoothing filter in phase-encode
	dimension
	*/

#define SVN_PRC_SE_KERNEL		(SVN_BASE_INDEPENDENT_PROCESS + 108)
#define SVT_PRC_SE_KERNEL		VFT_FLT32
#define SVS_PRC_SE_KERNEL		"Slice encode smoothing filter kernel"
/*	Convolution kernel used to create the smoothing filter in slice-encode
	dimension
	*/

#define SVN_PRC_RO_DECAY		(SVN_BASE_INDEPENDENT_PROCESS + 109)
#define SVT_PRC_RO_DECAY		VFT_FLT32
#define SVS_PRC_RO_DECAY		"Readout exponential filter decay"
/* 	The exponent used to create the exponential filter in readout dimension
        */

#define SVN_PRC_LEVEL_STRENGTH   	(SVN_BASE_INDEPENDENT_PROCESS + 110)
#define SVT_PRC_LEVEL_STRENGTH   	VFT_FLT32
#define SVS_PRC_LEVEL_STRENGTH   	"Leveler strength"
/* 	The constant (0 < Leveler strength < 1 ) used to create the leveler
        */

#define SVN_PRC_SCALE_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 111)
#define SVT_PRC_SCALE_FLAG	 	VFT_BOOL
#define SVS_PRC_SCALE_FLAG		"Flag to normalize processed data"

#define SVN_PRC_PDATA_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 112)
#define SVT_PRC_PDATA_TYPE		VFT_INT32
#define SVS_PRC_PDATA_TYPE		"Processed data type"

#define SVN_PRC_PDATA_FORM		(SVN_BASE_INDEPENDENT_PROCESS + 113)
#define SVT_PRC_PDATA_FORM		VFT_INT32
#define SVS_PRC_PDATA_FORM		"Processed data form"

#define SVN_PRC_FIRST_RPLC_SMPL		(SVN_BASE_INDEPENDENT_PROCESS + 114)
#define SVT_PRC_FIRST_RPLC_SMPL		VFT_INT32
#define SVS_PRC_FIRST_RPLC_SMPL		"First sample to replace (conj)"
/*	For each data dimension, this is the index of the first sample
	sample to replace when doing conjugation.
	This is an array of either 2 or 3 elements, depending on whether
	the acquisition is 2d or 3d. The order of the
	elements is either {PE, RO},  or {SE, PE, RO}.
	*/

#define SVN_PRC_LAST_RPLC_SMPL		(SVN_BASE_INDEPENDENT_PROCESS + 115)
#define SVT_PRC_LAST_RPLC_SMPL		VFT_INT32
#define SVS_PRC_LAST_RPLC_SMPL		"Last sample to replace (conj)"
/*	For each data dimension, this is the index of the last sample
	sample to replace when doing conjugation.
	This is an array of either 2 or 3 elements, depending on whether
	the acquisition is 2d or 3d. The order of the
	elements is either {PE, RO},  or {SE, PE, RO}.
	*/

#define SVN_PRC_NEP_DONEWECHO_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 116)
#define SVT_PRC_NEP_DONEWECHO_FLAG	VFT_BOOL
#define SVS_PRC_NEP_DONEWECHO_FLAG	"Do new echo after recon flag"
/*	Flag to indicate whether a new echo calculation is required
	after reconstruction.
	*/

#define SVN_PRC_NEP_INTERP_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 117)
#define SVT_PRC_NEP_INTERP_FLAG		VFT_BOOL
#define SVS_PRC_NEP_INTERP_FLAG		"Default interp. flag for New echo"
/*	Interpolation parameter for new echo calculation.
	TRUE  = use linear interpolation.
	FALSE = use exponential interpolation.
	*/

#define SVN_PRC_NEP_TE			(SVN_BASE_INDEPENDENT_PROCESS + 118)
#define SVT_PRC_NEP_TE			VFT_FLT32
#define SVS_PRC_NEP_TE			"Default TE for New echo"
/*	TE to be used in New echo calculation.
	*/

#define SVN_PRC_WAP_DOWEIGHT_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 119)
#define SVT_PRC_WAP_DOWEIGHT_FLAG	VFT_BOOL
#define SVS_PRC_WAP_DOWEIGHT_FLAG	"Do weighted avr. after recon flag"
/*	Flag to indicate whether a weighted combination is required
	after reconstruction.
	*/

#define SVN_PRC_WAP_MODE     		(SVN_BASE_INDEPENDENT_PROCESS + 120)
#define SVT_PRC_WAP_MODE		VFT_INT32
#define SVS_PRC_WAP_MODE		"Code for mode of weighted avr."
/*	Code to indicate mode of weighted average calculation
	0 = arithmetic mean.
	1 = root-mean-square.
	*/

#define SVN_PRC_WAP_NUM_IMAGES		(SVN_BASE_INDEPENDENT_PROCESS + 121)
#define SVT_PRC_WAP_NUM_IMAGES		VFT_INT32
#define SVS_PRC_WAP_NUM_IMAGES		"Number of images for weighted average"
/*	Number of images to be averaged.
	*/

#define SVN_PRC_WAP_WEIGHTS		(SVN_BASE_INDEPENDENT_PROCESS + 122)
#define SVT_PRC_WAP_WEIGHTS		VFT_FLT32
#define SVS_PRC_WAP_WEIGHTS		"Weights for weighted avr."
/*	Weights for weighted average calculation, this field is an array
	of size SVN_PRC_WAP_NUM_IMAGES.
	*/

/* define the rave filter scaling factors
 */
#define SVN_PRC_RF_SCALE_A		(SVN_BASE_INDEPENDENT_PROCESS + 123)
#define SVT_PRC_RF_SCALE_A		VFT_FLT32
#define SVS_PRC_RF_SCALE_A		"Rave filter scale factor A."

#define SVN_PRC_RF_SCALE_B		(SVN_BASE_INDEPENDENT_PROCESS + 124)
#define SVT_PRC_RF_SCALE_B		VFT_FLT32
#define SVS_PRC_RF_SCALE_B		"Rave filter scale factor B."

#define SVN_PRC_RF_SCALE_C		(SVN_BASE_INDEPENDENT_PROCESS + 125)
#define SVT_PRC_RF_SCALE_C		VFT_FLT32
#define SVS_PRC_RF_SCALE_C		"Rave filter scale factor C."

/*
 * New fields for the rave filter parameters, slightly more
 * descriptive than those above. Designed to replace the above,
 * though I won't delete the old ones just yet
 */
#define SVN_PRC_RF_LP_ENHANCE		(SVN_BASE_INDEPENDENT_PROCESS + 126)
#define SVT_PRC_RF_LP_ENHANCE		VFT_FLT32
#define SVS_PRC_RF_LP_ENHANCE		"Rave filter lp enhance level."

#define SVN_PRC_RF_LP_SMOOTH		(SVN_BASE_INDEPENDENT_PROCESS + 127)
#define SVT_PRC_RF_LP_SMOOTH		VFT_FLT32
#define SVS_PRC_RF_LP_SMOOTH		"Rave filter lp smooth level."

#define SVN_PRC_RF_HP_ENHANCE		(SVN_BASE_INDEPENDENT_PROCESS + 128)
#define SVT_PRC_RF_HP_ENHANCE		VFT_FLT32
#define SVS_PRC_RF_HP_ENHANCE		"Rave filter hp enhance level."

#define SVN_PRC_RF_HP_SHARPEN		(SVN_BASE_INDEPENDENT_PROCESS + 129)
#define SVT_PRC_RF_HP_SHARPEN		VFT_FLT32
#define SVS_PRC_RF_HP_SHARPEN		"Rave filter hp sharpen level."

#define SVN_PRC_RF_DORAVE_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 130)
#define SVT_PRC_RF_DORAVE_FLAG	VFT_BOOL
#define SVS_PRC_RF_DORAVE_FLAG	"Do rave after recon flag"
/*	Flag to indicate whether rave filter calculation is required
	after reconstruction.
	*/

#define SVN_PRC_PC_DO_SPEED_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 131)
#define SVT_PRC_PC_DO_SPEED_FLAG	VFT_BOOL
#define SVS_PRC_PC_DO_SPEED_FLAG	"Do pcmra (speed) after recon flag"
/*	Flag to indicate whether phase contrast processing is required
	after reconstruction.
	*/

#define SVN_PRC_PC_DO_VEL_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 132)
#define SVT_PRC_PC_DO_VEL_FLAG		VFT_BOOL
#define SVS_PRC_PC_DO_VEL_FLAG		"Do pcmra (vel) after recon flag"
/*	Flag to indicate whether phase contrast processing should
	write velocity images to study file.
	*/

#define SVN_PRC_FLD_STBL_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 133)
#define SVT_PRC_FLD_STBL_CORR_FLAG	VFT_BOOL
#define SVS_PRC_FLD_STBL_CORR_FLAG	"Field stability correction flag"
/*	Flag to indicate whether field stability slice correction is
	performed
	*/

#define SVN_PRC_FLD_STBL_LINE_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 134)
#define SVT_PRC_FLD_STBL_LINE_CORR_FLAG	VFT_BOOL
#define SVS_PRC_FLD_STBL_LINE_CORR_FLAG	"Field stability line correction flag"
/*	Flag to indicate whether field stability line correction is
	performed
	*/

#define SVN_PRC_RF_DO_SPIKE_FILTER_FLAG (SVN_BASE_INDEPENDENT_PROCESS + 135)
#define SVT_PRC_RF_DO_SPIKE_FILTER_FLAG VFT_BOOL
#define SVS_PRC_RF_DO_SPIKE_FILTER_FLAG "Do spike_filter after recon flag"
/*	Flag to indicate whether spike filter calculation is required
	after reconstruction.
	*/

#define SVN_PRC_RF_BEST_FILTER_SMOOTH (SVN_BASE_INDEPENDENT_PROCESS + 136)
#define SVT_PRC_RF_BEST_FILTER_SMOOTH VFT_FLT32
#define SVS_PRC_RF_BEST_FILTER_SMOOTH "Best filter smooth level."

#define SVN_PRC_RF_DO_BEST_FILTER_FLAG (SVN_BASE_INDEPENDENT_PROCESS + 137)
#define SVT_PRC_RF_DO_BEST_FILTER_FLAG VFT_BOOL
#define SVS_PRC_RF_DO_BEST_FILTER_FLAG "Do best_filter after recon flag"
/*	Flag to indicate whether best filter calculation is required
	after reconstruction.
	*/

#define SVN_PRC_DO_MERGE_POSITION_FLAG (SVN_BASE_INDEPENDENT_PROCESS + 138)
#define SVT_PRC_DO_MERGE_POSITION_FLAG VFT_BOOL
#define SVS_PRC_DO_MERGE_POSITION_FLAG "Do merge_position after recon flag"
/*	Flag to indicate whether merge duplicated position nodes is required
	after reconstruction.
	*/

#define SVN_PRC_RGN_CORR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 139)
#define SVT_PRC_RGN_CORR_FLAG	VFT_INT32
#define SVS_PRC_RGN_CORR_FLAG	"Pixel Value Correction for Reciver Gain"
/*	�č\�����Ƀ��V�[�o�[�Q�C�����̉摜�l�␳���s�����ǂ�����
	�w�肷��t���O
	*/

#define SVN_PRC_GATE_VIEWSHARE_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 140)
#define SVT_PRC_GATE_VIEWSHARE_FLAG		VFT_INT32
#define SVS_PRC_GATE_VIEWSHARE_FLAG		"Gate View Share Image Flag"
/*
  (a) proMui/mui���Z�b�g����B�O�܂��͂P�B
  (b) reconstruction���Q�Ƃ��A�P�̏ꍇ�ɒ��Ԏ����摜���쐬����B
	*/

#define SVN_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   (SVN_BASE_INDEPENDENT_PROCESS + 141)
#define SVT_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   VFT_BITFIELD
#define SVS_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   "Auto Diffusion Alignment Mode"
/*      ����Diffusion�㏈���A���C�������g�␳���[�h
(a)appcode
#define VFC_DIFFUSION_ALIGNMENT_REFER   1L << 0
#define VFC_DIFFUSION_ALIGNMENT_CALC            1L << 1
#define VFC_DIFFUSION_ALIGNMENT_CALC_SAVE       1L << 2
(b)proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb�Ŏw�肳��Ă���l��PROTOCOL�m�[�h�փZ�b�g����B
  Refer to parameters       : VFC_DIFFUSION_ALIGNMENT_REFER
  Calculate parameters      : VFC_DIFFUSION_ALIGNMENT_CALC
  Calculate/Save parameters : VFC_DIFFUSION_ALIGNMENT_CALC_SAVE
(c)msRecon, msPostProc��VFC_DIFFUSION_ALIGNMENT_REFER�̏ꍇ�ɂ͕␳�l�f�[�^�x�[�X���Q�Ƃ��ĉ摜��␳���AVFC_DIFFUSION_ALIGNMENT_CALC�̏ꍇ�ɂ͕␳�l���Z�o���ĉ摜��␳����B�܂��AVFC_DIFFUSION_ALIGNMENT_CALC_SAVE�̏ꍇ�͕␳�l���Z�o���ĉ摜��␳���␳�l���f�[�^�x�[�X�֓o�^����B
�܂��A���ۂ̕␳�l���ė��p���Z�o�������̂���PROCESSED�m�[�h�փZ�b�g����B

*/

#define SVN_PRC_RECON_NR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 142)
#define SVT_PRC_RECON_NR_FLAG	VFT_INT32
#define SVS_PRC_RECON_NR_FLAG	"Noise Reduction flag for Reconstruction"
/* �č\���m�C�Y ���_�N�V�����t���O 
0 : �č\���m�C�Y ���_�N�V������������
1 : �č\���m�C�Y ���_�N�V���������L��

�t�B�[���h���̂����݂��Ȃ��ꍇ�͍č\���m�C�Y ���_�N�V�������������Ƃ���B
*/

#define SVN_PRC_RECON_NR_SET	(SVN_BASE_INDEPENDENT_PROCESS + 143)
#define SVT_PRC_RECON_NR_SET	VFT_INT32
#define SVS_PRC_RECON_NR_SET	"Noise Reduction set ID for Reconstruction"
/* �č\���m�C�Y ���_�N�V�����Z�b�g�p�����[�^ID 
�č\���m�C�Y ���_�N�V���������L��̏ꍇ�A�I�����ꂽ�Z�b�g�p�����[�^�ԍ����Z�b�g����B
�č\���m�C�Y ���_�N�V�������������̏ꍇ�A�f�[�^�̃Z�b�g���s��Ȃ��B
�Θb�\�����ɂ��̒l���f�t�H���g�Ƃ��ĕ\������B
�l��-1�܂��̓t�B�[���h���̂����݂��Ȃ��ꍇ�͍č\���m�C�Y ���_�N�V�������������Ƃ���B
*/
 
#define SVN_PRC_GOP_FILTER           (SVN_BASE_INDEPENDENT_PROCESS + 144)
#define SVT_PRC_GOP_FILTER           VFT_INT32
#define SVS_PRC_GOP_FILTER           "GOP filter ID"
/* GOP�t�B���^ID
�č\���m�C�Y ���_�N�V���������L��̏ꍇ�A�I�����ꂽGOP�p�����[�^�ԍ����Z�b�g����B
�č\���m�C�Y ���_�N�V�������������̏ꍇ�A�f�[�^�̃Z�b�g���s��Ȃ��B
�l��-1�܂��̓t�B�[���h���̂����݂��Ȃ��ꍇ��GOP�t�B���^�������s��Ȃ��B
*/

#define SVN_PRC_K_SPACE_FILTER_NR          (SVN_BASE_INDEPENDENT_PROCESS + 145)
#define SVT_PRC_K_SPACE_FILTER_NR          VFT_INT32
#define SVS_PRC_K_SPACE_FILTER_NR          "K Space filter ID for Noise Reduction"
/* �č\���m�C�Y ���_�N�V����k-space�t�B���^ID
�č\���m�C�Y ���_�N�V���������L��̏ꍇ�A�I�����ꂽk-space filter�ԍ����Z�b�g����B
�č\���m�C�Y ���_�N�V�������������̏ꍇ�A�f�[�^�̃Z�b�g���s��Ȃ��B
*/
 
#define SVN_PRC_AUTO_MIP_FILTER_ID   (SVN_BASE_INDEPENDENT_PROCESS + 146)
#define SVT_PRC_AUTO_MIP_FILTER_ID   VFT_INT32
#define SVS_PRC_AUTO_MIP_FILTER_ID   "filter ID for auto MIP"
/* ����MIP�p�t�B���^ID
����MIP�v���r���[�Ńm�C�Y ���_�N�V���������L��̏ꍇ�A�I�����ꂽMIP filter ID���Z�b�g����B
����MIP�v���r���[�Ńm�C�Y ���_�N�V�������������̏ꍇ�A�f�[�^�̃Z�b�g���s��Ȃ��B
*/

#define SVN_PRC_AUTO_MIP_GOP_FILTER   (SVN_BASE_INDEPENDENT_PROCESS + 147)
#define SVT_PRC_AUTO_MIP_GOP_FILTER   VFT_INT32
#define SVS_PRC_AUTO_MIP_GOP_FILTER   "GOP filter ID for auto MIP"
/* ����MIP�pGOP�t�B���^ID
����MIP�v���r���[�Ńm�C�Y ���_�N�V���������L��̏ꍇ�A�I�����ꂽGOP�p�����[�^�ԍ����Z�b�g����B
����MIP�v���r���[�Ńm�C�Y ���_�N�V�������������̏ꍇ�A�f�[�^�̃Z�b�g���s��Ȃ��B
*/


/*****************************************************************************
  STUDY name space
*/




/*****************************************************************************
  PROTOCOL name space
*/
#define SVN_BASE_PROTOCOL_PROCESS	SVN_MIN_PROTOCOL + SVN_OFF_PROCESS

#define SVN_PRC_T2_THRESH	(SVN_BASE_PROTOCOL_PROCESS + 0)
#define SVT_PRC_T2_THRESH	VFT_FLT32
#define SVS_PRC_T2_THRESH	"Fast scan T2 correction threshold"
/*	  Multi-Shot EPI / Hybrid EPI �ɂ����āA
	SVN_PRC_T2_CORR_METHOD��VFC_T2_CORR_FOR_EPI_2SHOT_MODE
	�̏ꍇ�Ɏg�p����T2�␳���s�Ȃ��^�s�Ȃ�Ȃ������肷�邵�����l
	*/

#define SVN_PRC_BLOCK_SORT_SIZE	(SVN_BASE_PROTOCOL_PROCESS + 1)
#define SVT_PRC_BLOCK_SORT_SIZE	VFT_INT32
#define SVS_PRC_BLOCK_SORT_SIZE	"Block sort table for 3D Hybrid EPI"
/*	�RD�� Hybrid EPI �ɂ����āA���[�f�[�^���RD��FSE���邢��
	�RD��FASE�̕��ѕ��ɕ��ѕς��鏇���������e�[�u��
	BLOCK_SORT table�i�P�����z��j�̗v�f���������ϐ��B
	�l�͈̔́F1����numEchoTrain�̍ő偖�X���C�X������
	�}�g���N�X�̍ő�l�܂� 
	*/

#define SVN_PRC_BLOCK_SORT_TABLE	(SVN_BASE_PROTOCOL_PROCESS + 2)
#define SVT_PRC_BLOCK_SORT_TABLE	VFT_INT32
#define SVS_PRC_BLOCK_SORT_TABLE	"Block sort table"
/*	�RD�� Hybrid EPI �ɂ����āA���[�f�[�^���RD��FSE
	���邢�͂RD��FASE�̕��ѕ��ɕ��ѕς��鏇���������e�[�u��
	BLOCK_SORT table�i�P�����z��A�v�f����SVN_PRC_BLOCK_SORT_SIZE�j
	���וς���̃f�[�^�ő� i �Ԗڂ̃G�R�[�u���b�N�̃f�[�^�́A
	���וς��O�́i���W���́j�G�R�[�f�[�^�̑� j �Ԗڂ̃G�R�[
	�u���b�N�ɑ�������Ƃ���ƁA
	  j = SVN_PRC_BLOCK_SORT_TABLE[i]
	�Ƃ����Ή��ɂȂ�B
	*/

#define SVN_PRC_PE_SRTTBL2	(SVN_BASE_PROTOCOL_PROCESS + 3)
#define SVS_PRC_PE_SRTTBL2	"Phase encode sort table for dual contrast scan"
/*	���e�ɂ��Ă�SVN_PRC_PE_SRTTBL�Ɠ����B
	*/

#define SVN_PRC_B_VALUE (SVN_BASE_PROTOCOL_PROCESS + 4)
#define SVT_PRC_B_VALUE VFT_INTVEC3
#define SVS_PRC_B_VALUE "B value: gradient factor"
/*
	�����ɂ��ʑ��V�t�g�̓x����\���p�����[�^�ŁA[s/m^2]�̒P��
	�ŕ\�������́BX,Y,Z�e�`�����l���̒l��ݒ肷��B
*/

#define SVN_PRC_DIFF_CORR_METHOD (SVN_BASE_PROTOCOL_PROCESS + 5)
#define SVT_PRC_DIFF_CORR_METHOD VFT_INT32
#define SVS_PRC_DIFF_CORR_METHOD "navigator(diffusion)correction method"
/*
	diffusion�ɂ�����i�r�Q�[�^�G�R�[�ɂ��␳���@���w�肷��B
	�Q�v�f�̔z��Ƃ��A
	[0]�␳�p�f�[�^�̐�����@
		(1)�����X�L�����̃f�[�^���g�p�B
		(2)B=0�̃X�L�����̃f�[�^���g�p�B
	[1]�␳���@�̎�ނƂ��ẮA
		(1)�i�r�Q�[�^�f�[�^��FT�������̂����C�����Ƃɏ�Z����B
		(2)�Ȃт��[�����0���A�P���̕␳�ʂ��t�B�b�e�B���O��
		   ��苁�߁A�␳����B
		(3)�i�r�Q�[�^���O���A�P���̕␳�ʂ��q�X�g�O������
		   ��苁�߁A�␳����B
	�Ȃǂ�����B	
*/

#define SVN_PRC_ADC_IMAGE_PNO (SVN_BASE_PROTOCOL_PROCESS + 6)
#define SVT_PRC_ADC_IMAGE_PNO VFT_INT32
#define SVS_PRC_ADC_IMAGE_PNO "protocol No. utilized for ADC image"
/*
	ADC�摜�i�g�U�W���摜�j���v�Z����̂Ɏg�p�����摜�̃v���g�R��
	�ԍ��B�����ݒ肳���B
*/

#define SVN_PRC_PHASE_SHIFT_FLAG (SVN_BASE_PROTOCOL_PROCESS + 7)
#define SVT_PRC_PHASE_SHIFT_FLAG VFT_INT32
#define SVS_PRC_PHASE_SHIFT_FLAG "phase shift flag for reconstruction"
/*
	�e�����ɑ΂���ʑ��V�t�g�̓K�p�̉ۂ�SE:PE:RO(1:2:4)�̃r�b�g
	OR�ŋL�q�B
*/

#define SVN_PRC_PHASE_SHIFT_VECTOR (SVN_BASE_PROTOCOL_PROCESS + 8)
#define SVT_PRC_PHASE_SHIFT_VECTOR VFT_FLT32
#define SVS_PRC_PHASE_SHIFT_VECTOR "phase shift vector(shift in pixcels for each axis)"
/*
	�e�����ɑ΂���V�t�g�ʂ��s�N�Z���P�ʂŋL�q�B�V�[�P���X����
	�č\���ɑ΂������I�ɉ摜�V�t�g���s�킹�������Ɏw��B
*/

#define SVN_PRC_RO_INTERPOL_METHOD	(SVN_BASE_PROTOCOL_PROCESS + 9)
#define SVT_PRC_RO_INTERPOL_METHOD	VFT_UINT32
#define SVS_PRC_RO_INTERPOL_METHOD	"RO interpolation method"
/*	sin�g�쓮��EPI�̃f�[�^��Ԃ̓���ؑւ�
	0x01->��Ԃ��Ȃ��^����iI�A�pch��Ɨ��ɂR���̃X�v���C����ԁj
	0x02->�V�t�g���Ȃ��^����
 */

#define SVN_PRC_RO_INTERPOL_DATA	(SVN_BASE_PROTOCOL_PROCESS + 10)
#define SVT_PRC_RO_INTERPOL_DATA	VFT_FLT32
#define SVS_PRC_RO_INTERPOL_DATA	"RO interpolation data"
/*	sin�g�쓮��EPI�̃f�[�^��ԗp�̃f�[�^
	SVN_GEN_DATA_DIM(RO�����j�̌��̃T���v�����O�f�[�^�̒��ŁA
	��m�Ԗڂ̃f�[�^�́A��Ԍ�̑�SVN_PRC_RO_INTERPOL_DATA[m]�Ԗڂ�
	�f�[�^�ɑ�������B�i�����ł���A�K�����������ł͂Ȃ��B�j
	�l�͈̔́FSVN_GEN_DATA_DIM(RO�����j/SVN_EXE_NUM_OVERSMPL��N�Ƃ���ƁA
	0 �` N-1�̎���
	�v�f�� SVN_GEN_DATA_DIM�iRO�����j�̂P�����z��
 */

#define SVN_PRC_AUTO_MIP_PREVIEW	(SVN_BASE_PROTOCOL_PROCESS + 11)
#define SVT_PRC_AUTO_MIP_PREVIEW	VFT_INT32
#define SVS_PRC_AUTO_MIP_PREVIEW	"Auto MIP Preview"
/*	�v�f���R�̔z��Ƃ���B�e�v�f�̈Ӗ������͈ȉ��̂Ƃ���B
	�t�B�[���h���̂����݂��Ȃ��ꍇ�͎���MIP Preview���s��Ȃ��B
	val[0] : action
	   = 0 : �������Ȃ�
	   = 1 : �ő�l���e
	   = 2 : Composite MIP 
	   = 3 : �ő�l���e(�R����)
	   = 4 : �ő�l���e(rotate)
	   = 5 : STAMD(Shift)
	val[1] : rate (�番���\�L)
	   = 0 �` 1000 (����l�F1000)
	val[2] : filter
	   = filter key (����l�F-1)
*/

#define SVN_PRC_REAL_DC_CORRECTION	(SVN_BASE_PROTOCOL_PROCESS + 12)
#define SVT_PRC_REAL_DC_CORRECTION	VFT_INT32
#define SVS_PRC_REAL_DC_CORRECTION	"real time dc correciton flag"
/*	���̃V�[�P���X��Real Time DC�␳�\���ǂ����������t���O
	�\�Ȃ�΁A	1
	�s�Ȃ�΁A	0
	*/

#define SVN_PRC_REAL_DC_CORR_FLAG	(SVN_BASE_PROTOCOL_PROCESS + 13)
#define SVT_PRC_REAL_DC_CORR_FLAG	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_FLAG	"real time dc correciton execution flag"
/*	Real Time DC�␳�����s���邩�ǂ����������t���O
	���s����Ȃ�΁A	1
	���s���Ȃ��Ȃ�΁A	0
	*/

#define SVN_PRC_REAL_DC_CORR_PITCH	(SVN_BASE_PROTOCOL_PROCESS + 14)
#define SVT_PRC_REAL_DC_CORR_PITCH	VFT_FLT32
#define SVS_PRC_REAL_DC_CORR_PITCH	"real time dc correciton - pitch"
/*	Real Time DC�␳�p�T���v�����O�s�b�`���w�肷��B
 */

#define SVN_PRC_REAL_DC_CORR_POINT	(SVN_BASE_PROTOCOL_PROCESS + 15)
#define SVT_PRC_REAL_DC_CORR_POINT	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_POINT	"real time dc correciton - point"
/*	Real Time DC�␳�p�T���v�����O�|�C���g���w�肷��B
 */

#define SVN_PRC_REAL_DC_CORR_DELAY	(SVN_BASE_PROTOCOL_PROCESS + 16)
#define SVT_PRC_REAL_DC_CORR_DELAY	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_DELAY	"real time dc correciton - delay"
/*	Real Time DC�␳�p�T���v�����O�f�B���C���w�肷��B
 */

#define SVN_PRC_REMAIN_CORR_FLAG	(SVN_BASE_PROTOCOL_PROCESS + 17)
#define SVT_PRC_REMAIN_CORR_FLAG	VFT_INT32
#define SVS_PRC_REMAIN_CORR_FLAG	"remain correciton flag"
/*	�c���␳���s����t���O�B
	���s����Ȃ�΁A	1
	���s���Ȃ��Ȃ�΁A	0
	*/

#define SVN_PRC_B_TABLE	(SVN_BASE_PROTOCOL_PROCESS + 18)
#define SVT_PRC_B_TABLE	VFT_INT32
#define SVS_PRC_B_TABLE	"b value tables of the every contrast images in a simultaneous scan."
/*	�g�U�����C���[�W���O�ɂ����čč\���̍ۂɁA�쐬���ꂽ�摜�ɂ���b�l
	(�g�U�����̒��x��\���p�����[�^)�̒l���A�Q�Ƃ��邽�߂̃e�[�u���B
	�摜1�ɑ΂��ẮA1�`3�Ԗڂ̃e�[�u���̒l��SVN_PRC_B_VALUE�ɃR�s�[����B
	�摜2�ɑ΂��ẮA4�`6�Ԗڂ̃e�[�u���̒l��SVN_PRC_B_VALUE�ɃR�s�[����B
	�摜3�ɑ΂��ẮA7�`9�Ԗڂ̃e�[�u���̒l��SVN_PRC_B_VALUE�ɃR�s�[����B
	�摜4�ɑ΂��ẮA10�`12�Ԗڂ̃e�[�u���̒l��SVN_PRC_B_VALUE��
	�R�s�[����B
	�ݒ��:	seqgen
	At the diffusion imaging reconstruction, appropriate part of
	this table are copied to SVN_PRC_B_VALUE.
	*/

#define SVN_PRC_AUTO_DIFFUSION_ADC 		(SVN_BASE_PROTOCOL_PROCESS + 19)
#define SVT_PRC_AUTO_DIFFUSION_ADC 		VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ADC		"Auto Diffusion ADC Flag"
/* ����Diffusion�㏈��ADC�摜�쐬 ���s�t���O
 *	(a) proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb��
 *          �w�肳��Ă���l���Z�b�g����B
 *		Diffusion ADC ��ON		: True(1)
 *		Diffusion ADC ��OFF		: False(0)
 *	(b) msRecon,msPostProc��True(1)�̏ꍇADC�摜���쐬����B
*/

#define SVN_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	(SVN_BASE_PROTOCOL_PROCESS + 20)
#define SVT_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	VFT_FLT32
#define SVS_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	"Auto Diffusion ADC Threshold"
/* ����Diffusion�㏈��ADC�摜�쐬 臒l	
 *	(a) proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb��
 *	    �w�肳��Ă���臒l(�����l)���Z�b�g����B
 *	(b) msRecon,msPostProc��ADC�摜�쐬�����̒l��p����臒l��������B
 */

#define SVN_PRC_AUTO_DIFFUSION_ISOTROPIC 	(SVN_BASE_PROTOCOL_PROCESS + 21)
#define SVT_PRC_AUTO_DIFFUSION_ISOTROPIC 	VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ISOTROPIC	"Auto Diffusion ADC Flag"
/* ����Diffusion�㏈��ISOTROPIC�摜�쐬 ���s�t���O
 *	(a) proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb��
 *	    �w�肳��Ă���l���Z�b�g����B
 *		Diffusion Isotropic ��ON	: True(1)
 *		Diffusion Isotropic ��OFF	: False(0)
 *	(b) msRecon,msPostProc��True(1)�̏ꍇIsotropic�摜���쐬����B
 */

#define SVN_PRC_AUTO_PERFUSION	 	(SVN_BASE_PROTOCOL_PROCESS + 22)
#define SVT_PRC_AUTO_PERFUSION	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION		"Auto Dynamic Purfusion Flag"
/* ����Perfusion�㏈�� ���s�t���O	
 *	(a) proEdit,proPerfusion�̓p�t���[�W�����㏈�������ݒ�Θb��
 *	    �w�肳��Ă���l���Z�b�g����B
 *		Dynamic delta R2* ��ON		: True(1)
 *		Dynamic delta R2* ��OFF		: False(0)
 *	(b) msRecon,msPostProc��True(1)�̏ꍇDynamic delta R2*�摜���쐬����B
 */

#define SVN_PRC_AUTO_PERFUSION_THRESHOLD	(SVN_BASE_PROTOCOL_PROCESS + 23)
#define SVT_PRC_AUTO_PERFUSION_THRESHOLD	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_THRESHOLD	"Auto Dynamic Perfusion Threshold"
/* ����Perfusion�㏈�� 臒l	
 *	(a) �傫���Q�̔z��ŁA
 *		�z��v�f�P��	�F	�����l
 *		�z��v�f�Q��	�F	����l
 *	(b) proEdit,proPerfusion�̓p�t���[�W�����㏈�������ݒ�Θb��
 *	    臒l�Ƃ��Ďw�肳��Ă��鉺���l�A����l�����ꂼ��Z�b�g����B
 *	(c) msRecon,msPostProc�͂��̒l��delta R2*�摜�쐬����臒l�Ƃ��ėp����B
 */

#define SVN_PRC_AUTO_PERFUSION_BASE	 	(SVN_BASE_PROTOCOL_PROCESS + 24)
#define SVT_PRC_AUTO_PERFUSION_BASE	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION_BASE		"Auto Dynamic Purfusion Base Images"
/* ����Perfusion�㏈�� ��摜
 *	(a) �傫���Q�̔z��ŁA
 *		�z��v�f�P��	�F	��摜�̊J�n����
 *		�z��v�f�Q��	�F	��摜�̉摜��
 *	(b) proEdit,proPerfusion�̓p�t���[�W�����㏈�������ݒ�Θb��
 *	    ��摜�Ƃ��Ďw�肳��Ă���J�n�����Ɖ摜�������ꂼ��Z�b�g����B
 *	(c) msRecon,msPostProc�͂��̒l�����Ƃɕ��ω摜���쐬��delta R2*�摜
 *	    �쐬���̊�摜�Ƃ���B
 */

#define SVN_PRC_AUTO_PERFUSION_ZOOM	 	(SVN_BASE_PROTOCOL_PROCESS + 25)
#define SVT_PRC_AUTO_PERFUSION_ZOOM	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION_ZOOM		"Auto Dynamic Purfusion Zoom"
/* ����Perfusion�㏈�� ���ԕ�����Ԕ{��
 *	(a) proEdit,proPerfusion�͎����p�t���[�W�����㏈�������ݒ�Θb��
 *	    ���ԕ�����Ԕ{���Ƃ��Ďw�肳��Ă���l(1�`4)���Z�b�g����B
 *	(b) msRecon,msPostProc�͂��̒l�����ƂɎ��ԕ�����Ԃ��s����
 *	    delta R2*�摜���쐬����B
 */

#define SVN_PRC_AUTO_PERFUSION_MAPPING	 	(SVN_BASE_PROTOCOL_PROCESS + 26)
#define SVT_PRC_AUTO_PERFUSION_MAPPING	 	VFT_BITFIELD
#define SVS_PRC_AUTO_PERFUSION_MAPPING		"Auto Dynamic Purfusion Map Images"
/* ����Perfusion�㏈�� �}�b�v�摜
 *	(a) appcode�Ƃ��Ĉȉ������B
 *		VFC_PERFUSION_PH     (1L<<0)  Peak Height
 *		VFC_PERFUSION_PT     (1L<<1)  Peak Time
 *		VFC_PERFUSION_AT     (1L<<2)  Appearance Time
 *		VFC_PERFUSION_DT     (1L<<3)  Disappearance Time
 *		VFC_PERFUSION_AC     (1L<<4)  Area Under Curve (rCBV����)
 *		VFC_PERFUSION_rMT1   (1L<<5)  relative Mean Transit Time
 *		VFC_PERFUSION_MT2    (1L<<6)  2'nd Moment
 *		VFC_PERFUSION_TT     (1L<<7)  Transit Time
 *		VFC_PERFUSION_rFlow  (1L<<8)  relative Flow�irCBF�����j
 *		VFC_PERFUSION_US     (1L<<9)  Up Slope
 *		VFC_PERFUSION_DS     (1L<<10) Down Slope
 *		VFC_PERFUSION_PTe    (1L<<11) effective Peak Time
 *		VFC_PERFUSION_rMT1e  (1L<<12) effective Mean Transit Time
 *		VFC_PERFUSION_rFlowe (1L<<13) effective relative Flow
 *	(b) proEdit,proPerfusion�̓p�t���[�W�����㏈�������ݒ�Θb��
 *	    �w�肳��Ă���}�b�v�摜����Lappcode�̃r�b�g���̘_���a��
 *	    �Z�b�g����B
 *	(c) msRecon,msPostProc�͎w�肳�ꂽ���}�b�v�摜���쐬����B
 */

#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION	(SVN_BASE_PROTOCOL_PROCESS + 27)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION	VFT_BITFIELD
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION	"Auto Dynamic Subtraction Flag"
/* �_�C�i�~�b�N�������� ���s�t���O
 *	(a) proEdit,proMui�̓_�C�i�~�b�N�����ݒ�Θb�Ŏw�肳��Ă���l��
 *	    �Z�b�g����B
 *		�_�C�i�~�b�N������ON	: True(1)
 *		�_�C�i�~�b�N������OFF	: False(0)
 *	(b) msRecon,msPostProc��True(1)�̏ꍇ�ASVN_PRC_AUTO_DYNAMIC_
 *	    SUBTRACTION_IMAGES�Ŏw�肳��Ă���摜���쐬����BFalse(0)��
 *	    �ꍇ�͒ʏ�̃_�C�i�~�b�N�č\�����s���B
 */

#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	(SVN_BASE_PROTOCOL_PROCESS + 28)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	VFT_BITFIELD
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	"Auto Dynamic Subtraction Images"
/* �_�C�i�~�b�N�������� �摜��
 *	(a) appcode�Ƃ��Ĉȉ������B
 *		VFC_SUBTRACTION_NONE	 (1L << 0) �����Ȃ��摜�쐬
 *		VFC_SUBTRACTION_ABSOLUTE (1L << 1) ��Βl�����摜�쐬 
 *		VFC_SUBTRACTION_COMPLEX	 (1L << 2) ���f�����摜�쐬 
 *	(b) proEdit,proMui�̓_�C�i�~�b�N�����ݒ�Θb�Ŏw�肳��Ă���
 *	    �摜�����Lappcode�̃r�b�g���̘_���a�ŃZ�b�g����B
 *	(c) msRecon�͎w�肳�ꂽ�摜�핪�̍č\�����s���BVFC_SUBTRACTION_
 *	    ABSOLUTE���w�肳��Ă���ꍇ�́AmsPostProc��p���Đ�Βl
 *	    �����������s���B������0���w�肳��Ă���ꍇ�͒ʏ��
 *	    �_�C�i�~�b�N�摜���쐬����B
 */
#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE 	(SVN_BASE_PROTOCOL_PROCESS + 29)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE 	VFT_INT32
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE	"Dynamic Subtraction Base Phase"
/* �_�C�i�~�b�N�������� �����
 *	(a) proEdit,proMui�͎����_�C�i�~�b�N�����ݒ�Θb�Ŏw�肳���
 *	    �����������Z�b�g����B
 *	(b) msRecon,msPostProc�͂��̎����Ƃ���ȍ~�̎����Ƃ̍������s���B
 */

#define SVN_PRC_RECON_COUNTER		(SVN_BASE_PROTOCOL_PROCESS + 30)
#define SVT_PRC_RECON_COUNTER		VFT_INT32
#define SVS_PRC_RECON_COUNTER		"Recon Processing Counter"
/* �č\���ċA�Ăяo���J�E���^
 *	(a) msRecon�͂��̃J�E���^���č\���������狁�߂��ċA�Ăяo��
 *	    �񐔂ɒB����܂ōč\���������J��Ԃ��B�č\�����N������
 *	    �ꍇ�ɂP�C���N�������g����B�t�B�[���h���Ȃ��ꍇ�͂O
 *	    �Ƃ��Ĉ����B
 */

#define SVN_PRC_RECON_ENGINE	(SVN_BASE_PROTOCOL_PROCESS + 31)
#define SVT_PRC_RECON_ENGINE	VFT_INT32
#define SVS_PRC_RECON_ENGINE	"Reconstruction Processing Engine Type"
/*	�č\�����Z��̃^�C�v�B
	appcode
	VFC_RECON_HOST : 0 (HOST)
	VFC_RECON_RACE : 1 (RACE)
	VFC_RECON_VAP  : 2 (VAP)
	*/

#define SVN_PRC_RECON_AVERAGE_METHOD	(SVN_BASE_PROTOCOL_PROCESS + 32)
#define SVT_PRC_RECON_AVERAGE_METHOD	VFT_UINT32
#define SVS_PRC_RECON_AVERAGE_METHOD	"Recon Average Method"
/*	�č\���̃A�x���[�W���O���@���w�肷��B
  (a)appcode�Ƃ��Ĉȉ������B
   #define VFC_RECON_AVERAGE_NONE		0	
	   �č\�����ł̃A�x���[�W���O�Ȃ��B
   #define VFC_RECON_AVERAGE_SPLICE		1	
	   diffusion�B�e��SPLICE���[�h�Ŏg�p�B
	   �摜�f�[�^�ł̕��ω��Z�B
	   ���Z���̃f�[�^�`��	�́APDATA_FORM�ɂ��B
   #define VFC_RECON_AVERAGE_FISP_CISS		2	
	   FISP�B�e��CISS���[�h�Ŏg�p�B
   #define VFC_RECON_AVERAGE_FISP_SIMCAST	3
	   FISP�B�e��SIMCAST���[�h�Ŏg�p�B

*/

#define SVN_PRC_RECON_AVERAGE_CLUSTER_NUM	(SVN_BASE_PROTOCOL_PROCESS + 33)
#define SVT_PRC_RECON_AVERAGE_CLUSTER_NUM	VFT_INT32
#define SVS_PRC_RECON_AVERAGE_CLUSTER_NUM	"Number of Recon Average Cluster"
/*	�č\���ŃA�x���[�W���O�����̐����w�肷��B
 */

#define SVN_PRC_AUTO_PERFUSION_AAT	(SVN_BASE_PROTOCOL_PROCESS + 34)
#define SVT_PRC_AUTO_PERFUSION_AAT	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_AAT	"Auto Dynamic Perfusion AT Ratio"
/*	AT�}�b�v�摜�����߂邽�߂�PH(�ő�l)�ɑ΂��銄��(0.0�`1.0)�B
 */

#define SVN_PRC_AUTO_PERFUSION_ADT	(SVN_BASE_PROTOCOL_PROCESS + 35)
#define SVT_PRC_AUTO_PERFUSION_ADT	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_ADT	"Auto Dynamic Perfusion DT Ratio"
/*	DT�}�b�v�摜�����߂邽�߂�PH(�ő�l)�ɑ΂��銄��(0.0�`1.0)�B
 */

#define SVN_PRC_AUTO_PERFUSION_ACMIN	(SVN_BASE_PROTOCOL_PROCESS + 36)
#define SVT_PRC_AUTO_PERFUSION_ACMIN	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_ACMIN	"Auto Dynamic Perfusion AC min"
/*	AC�}�b�v�摜�����߂邽�߂̍ŏ��l(0.0�`10000.0)�B
 */


#define SVN_PRC_PROMPTER_SCRIPT (SVN_BASE_PROTOCOL_PROCESS + 37)
#define SVT_PRC_PROMPTER_SCRIPT VFT_STRING
#define SVS_PRC_PROMPTER_SCRIPT "Prompter script"
/*      ���������ŏ��������X�N���v�g�BPQM���������Aprompter �ɂ���ĉ��߂����B
*/

#define SVN_PRC_GATE_VIEWSHARE_RATIO	(SVN_BASE_PROTOCOL_PROCESS + 38)
#define SVT_PRC_GATE_VIEWSHARE_RATIO	VFT_FLT32
#define SVS_PRC_GATE_VIEWSHARE_RATIO	"Gate View Share Ratio"
/*
  (a) proMui/Mui���Z�b�g�B�L���l��1.0, 2.0, 4.0�B
  (b) reconstruction���Q�Ƃ����Ԏ����摜���쐬����B
*/

#define SVN_PRC_DRKS_FLAG       (SVN_BASE_PROTOCOL_PROCESS + 39)
#define SVT_PRC_DRKS_FLAG       VFT_INT32
#define SVS_PRC_DRKS_FLAG       "DRKS flag"
/*      DRKS�B�e���s�t���O
		0: DRKS Off�A 1: DRKS On
*/

#define SVN_PRC_PARENT_INDEX_FOR_PLANNING       (SVN_BASE_PROTOCOL_PROCESS + 41)
#define SVT_PRC_PARENT_INDEX_FOR_PLANNING       VFT_INT32
#define SVS_PRC_PARENT_INDEX_FOR_PLANNING       "Available Parent Image Index for Planning"
/*      �e�摜�\�v���Z�X�h�C���f�b�N�X�ԍ�
(a)�ʒu���ߎ��̐e�Ƃ��Ďg�p�\�ȉ摜�̃v���Z�X�h�m�[�h�̃C���f�b�N�X�ԍ�
(b)�č\�����Z�b�g���A�ʒu���ߏ������Q�Ƃ���B

*/

#define SVN_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   (SVN_BASE_PROTOCOL_PROCESS + 42)
#define SVT_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   "Auto Diffusion Alignment Flag"
/*      ����Diffusion�㏈���A���C�������g�摜�쐬 ���s�t���O
(a)proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb�Ŏw�肳��Ă���l���Z�b�g����B
        Alignment ��ON  : True(1)
        Alignment ��OFF : False(0)
(b)msRecon,msPostProc��True(1)�̏ꍇAlignment�摜���쐬���A��������摜�Ƃ��Ċediffusion�}�b�v�摜���쐬����B


*/

#define SVN_PRC_AUTO_DIFFUSION_MAPPING  (SVN_BASE_PROTOCOL_PROCESS + 43)
#define SVT_PRC_AUTO_DIFFUSION_MAPPING  VFT_BITFIELD
#define SVS_PRC_AUTO_DIFFUSION_MAPPING  "Auto Diffusion Map Images"
/*      (a)appcode
#define VFC_DIFFUSION_FA           1L<<0   
#define VFC_DIFFUSION_L1           1L<<1   
#define VFC_DIFFUSION_L2           1L<<2   
#define VFC_DIFFUSION_L3           1L<<3   
#define VFC_DIFFUSION_V1X          1L<<4   
#define VFC_DIFFUSION_V1Y          1L<<5   
#define VFC_DIFFUSION_V1Z          1L<<6   
(b)proEdit,proDiffusion�̓f�B�t���[�W�����㏈�������ݒ�Θb�Ŏw�肳��Ă���l��PROTOCOL�m�[�h�փZ�b�g����B
  FA��ON           : VFC_DIFFUSION_FA
  L1,L2,L3��ON     : VFC_DIFFUSION_L1 | VFC_DIFFUSION_L2 | VFC_DIFFUSION_L3
  V1x,V1y,V1z��ON  : VFC_DIFFUSION_V1X | VFC_DIFFUSION_V1Y | VFC_DIFFUSION_V1Z
(c)msRecon,msPostProc�͊e�r�b�g�ɏ]���Adiffusion�}�b�v�摜���쐬����B

*/

#define SVN_PRC_ALIGN_SHIFT_IN_PE       (SVN_BASE_PROTOCOL_PROCESS + 44)
#define SVT_PRC_ALIGN_SHIFT_IN_PE       VFT_FLT32
#define SVS_PRC_ALIGN_SHIFT_IN_PE       "Shift in PE direction at Alignment"
/*      diffusion-alignment�����ɂ�PE�����ɃV�t�g����_��
(a)alignment�����������Ɏg�p�����␳�l���AmsPostProc���o�^����B
(b)�X���C�X�������̔z��B


*/

#define SVN_PRC_ALIGN_ORIENT_KEY        (SVN_BASE_PROTOCOL_PROCESS + 45)
#define SVT_PRC_ALIGN_ORIENT_KEY        VFT_INT32
#define SVS_PRC_ALIGN_ORIENT_KEY        "Image Orient Key at Alignment"
/*      diffusion-alignment�����f�[�^�x�[�X�摜�����Ɋւ���L�[
(a)�A���C�������g�����̃f�[�^�x�[�X����������L�[�̂ЂƂB�V�[�P���X���ݒ肵�ARefer to parameter���Ɍ㏈�����Q�Ƃ���B
*/

#define SVN_PRC_AUTO_MIP_BASE   (SVN_BASE_PROTOCOL_PROCESS + 46)
#define SVT_PRC_AUTO_MIP_BASE   VFT_FLTVEC3
#define SVS_PRC_AUTO_MIP_BASE   "Base angle for auto MIP "
/*      �����l�h�o��p
*/

#define SVN_PRC_AUTO_MIP_SLICES (SVN_BASE_PROTOCOL_PROCESS + 47)
#define SVT_PRC_AUTO_MIP_SLICES VFT_INT32
#define SVS_PRC_AUTO_MIP_SLICES "Number of slices for auto MIP "
/*      �����l�h�o�X���C�X����
*/

#define SVN_PRC_AUTO_MIP_ANGLE  (SVN_BASE_PROTOCOL_PROCESS + 48)
#define SVT_PRC_AUTO_MIP_ANGLE  VFT_FLT32
#define SVS_PRC_AUTO_MIP_ANGLE  "Step of projective angle for auto MIP "
/*      �����l�h�o���e�p�X�e�b�v
*/

#define SVN_PRC_INTENSITY_MAP_TYPE  (SVN_BASE_PROTOCOL_PROCESS + 49)
#define SVT_PRC_INTENSITY_MAP_TYPE  VFT_BITFIELD
#define SVS_PRC_INTENSITY_MAP_TYPE  "Intensity Map Type"
/*      �P�x�␳�v���X�L�����ł��邱�Ƃ��������
�@SVN_PLN_SPEEDER_TYPE=1�̏ꍇ�ɃR�C��DB������PQM���������ށB
�@�@SPEEDER�R�C���̏ꍇ��3(1|2)
    �P�x�␳�K�p�݂̂̃R�C���̏ꍇ��1
    �P�x�␳�^SPEEDER���x�␳�}�b�v�̍쐬�^�C�v�B
        �������Ȃ�       VFC_INTENSITY_MAP_TYPE_NONE         0
        �P�x�␳�}�b�v   VFC_INTENSITY_MAP_TYPE_INTENSITY    (1<<0)
        ���x�␳�}�b�v   VFC_INTENSITY_MAP_TYPE_SPEEDER      (1<<1)

	�S�}�b�v(�P�x�␳�}�b�v�A���x�␳�}�b�v�AWB���x�␳�}�b�v) 3(1|2)
*/

#define SVN_PRC_INTENSITY_CORR_FLAG  (SVN_BASE_PROTOCOL_PROCESS + 50)
#define SVT_PRC_INTENSITY_CORR_FLAG  VFT_INT32
#define SVS_PRC_INTENSITY_CORR_FLAG  "Intensity Correction Flag"
/* 
  �P�x�␳������K�p���邵�Ȃ��̃t���O
�@prolim�̂�������ŁAOn/Off�t���O���K�v�B
�@�t�я��Ƃ���DISPLAY�ɕ\�������B�i�W����\�����邩�͖���j
�@PQM�͎B�e���O�ɋP�x�␳�}�b�v�̗L���m�F���A�}�b�v�Ȃ���Όx���\����Off����B
      �P�x�␳��OFF/ON�B
        �P�x�␳OFF VFC_INTENSITY_CORR_OFF    0
        �P�x�␳ON  VFC_INTENSITY_CORR_ON     1
*/

#define SVN_PRC_INTENSITY_CORR_RATIO  (SVN_BASE_PROTOCOL_PROCESS + 51)
#define SVT_PRC_INTENSITY_CORR_RATIO  VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_RATIO  "Intensity Correction Ratio"
/* 
   �P�x�␳�W���B
   �␳�W���̃t�B�[���h�F�l��float�BproMui/mui�ł�0-100%�̎w��B

*/

#define SVN_PRC_PAC_CORRECT_ACTION  (SVN_BASE_PROTOCOL_PROCESS + 52)
#define SVT_PRC_PAC_CORRECT_ACTION  VFT_INT32
#define SVS_PRC_PAC_CORRECT_ACTION  "PAC Correction Action"
/* 
   ch�Ԉʑ���,�Q�C�����␳���s�Ȃ����̃t���O�B
   �r�b�g���Ɏw��B
   0x01 SVN_EXE_PAC_PHASE_CORRECT_PARAMS �ɂ��␳ 0:OFF,1:ON
   0x02 SVN_EXE_PAC_GAIN_CORRECT_PARAMS �ɂ��␳  0:OFF,1:ON

    #define VFC_PAC_PHASE_CORRECT  1
    #define VFC_PAC_GAIN_CORRECT   (1<<1)
*/

#define SVN_PRC_DIRECT_QUEUE  (SVN_BASE_PROTOCOL_PROCESS + 53)
#define SVT_PRC_DIRECT_QUEUE  VFT_INT32
#define SVS_PRC_DIRECT_QUEUE  "Reconstruction Direct Queue Flag"
/* 
   2D/3D/postProc�ȊO�̍č\���L���[(Direct Queue)�ɂ��̃v���Z�X��
   �ςނ��ǂ���PQM���w�肷��B�č\���͂��̃t���O���P�̏ꍇ��
   Direct Queue�ɐςށB
   0 normal
   1 Direct Queue

    #define VFC_SET_NORMAL_QUEUE 0
    #define VFC_SET_DIRECT_QUEUE 1
*/

#define SVN_PRC_RECON_START_AFTER_REFVIEW  (SVN_BASE_PROTOCOL_PROCESS + 54)
#define SVT_PRC_RECON_START_AFTER_REFVIEW  VFT_INT32
#define SVS_PRC_RECON_START_AFTER_REFVIEW  "Reconstruction Start after RefView Flag"
/* 
   DRKS��RefView�č\���̎���3D�̖{�č\���̊J�n�^�C�~���O�����߂�t���O

   #define VFC_AFTER_REFVIEW_AUTO	0
   #define VFC_AFTER_REFVIEW_MANUAL	1

   VFC_AFTER_REFVIEW_AUTO�̎�
       RefView�č\���I���シ���ɖ{�č\�����J�n����B
    
   VFC_AFTER_REFVIEW_MANUAL�̎�
       RefView�č\���I����č\�����N�����Ȃ��B�āX�\���ɂ��{�č\�����s���B
*/

#define SVN_PRC_SCRIPT_STATUS  (SVN_BASE_PROTOCOL_PROCESS + 55)
#define SVT_PRC_SCRIPT_STATUS  VFT_INT32
#define SVS_PRC_SCRIPT_STATUS  "Prompter Script Status"
/*  �����]���I����ɓ]�����ꂽ�摜��processedID�̃��X�g���i�[�����B
*/

#define SVN_PRC_AUTO_GDC_ACTION	  (SVN_BASE_PROTOCOL_PROCESS + 56)
#define SVT_PRC_AUTO_GDC_ACTION	  VFT_INT32
#define SVS_PRC_AUTO_GDC_ACTION	  "Auto Geometric Distortion Correction - Action"
/* 
   ��������c�ݕ␳�����t���O
   #define VFC_GDC_ACTION_OFF   0
   #define VFC_GDC_ACTION_ON    1
*/

#define SVN_PRC_AUTO_GDC_DIM	  (SVN_BASE_PROTOCOL_PROCESS + 57)
#define SVT_PRC_AUTO_GDC_DIM	  VFT_INT32
#define SVS_PRC_AUTO_GDC_DIM	  "Auto Geometric Distortion Correction - Dimension"
/* 
   ��������c�ݕ␳Dimension
   #define VFC_GDC_DIM_2D   2
   #define VFC_GDC_DIM_3D   3
*/

#define SVN_PRC_AUTO_GDC_FILTER	  (SVN_BASE_PROTOCOL_PROCESS + 58)
#define SVT_PRC_AUTO_GDC_FILTER	  VFT_INT32
#define SVS_PRC_AUTO_GDC_FILTER	  "Auto Geometric Distortion Correction - Filter"
/* 
   ��������c�ݕ␳�t�B���^�[
*/

#define SVN_PRC_AUTO_ALT_SUBTRACTION	(SVN_BASE_PROTOCOL_PROCESS + 59)
#define SVT_PRC_AUTO_ALT_SUBTRACTION	VFT_INT32
#define SVS_PRC_AUTO_ALT_SUBTRACTION	"Auto Alternated Subtraction Flag"
/* tSLIT�������� ���s�t���O
   ALTERNATED_ACQ_FLAG��ON�̂Ƃ��̂�proMui�Ŏw��ł���B
 */

#define SVN_PRC_SUBTRACTION_POLARITY	(SVN_BASE_PROTOCOL_PROCESS + 60)
#define SVT_PRC_SUBTRACTION_POLARITY	VFT_INT32
#define SVS_PRC_SUBTRACTION_POLARITY	"tSLIT Subtraction Polarity"
/* tSLIT�����������̋ɐ�
   seqgen���ݒ肷��B
 */


#define SVN_PRC_AMB_SCAN_LOOP_ID	(SVN_BASE_PROTOCOL_PROCESS + 61)
#define SVT_PRC_AMB_SCAN_LOOP_ID	VFT_INT32
#define SVS_PRC_AMB_SCAN_LOOP_ID	"AMB scan loop ID"
/* SVN_PLN_AMB_MODE_FLG ��AMB�B���ł̎��L��
   PQM���Z�b�g����Display���\���������
   val[0]:�J�����gSEG
   val[1]:�J�����g�J��Ԃ���
   val[2]:��SEG
 */

#define SVN_PRC_GDC_FLAG                (SVN_BASE_PROTOCOL_PROCESS + 62)
#define SVT_PRC_GDC_FLAG                VFT_INT32
#define SVS_PRC_GDC_FLAG                "GDC flag"
/* 
   ����c�ݕ␳�����t���O
   0 : ����c�ݕ␳��������
   1 : ����c�ݕ␳�����L��

   #define VFC_GDC_FLAG_OFF    0
   #define VFC_GDC_FLAG_ON     1
 */

#define SVN_PRC_GDC_DIM                 (SVN_BASE_PROTOCOL_PROCESS + 63)
#define SVT_PRC_GDC_DIM                 VFT_INT32
#define SVS_PRC_GDC_DIM                 "GDC Dimension"
/* 
   ����c�ݕ␳����Dimension
   2 : 2D����
   3 : 3D����

   #define VFC_GDC_DIMENSION_2D    2
   #define VFC_GDC_DIMENSION_3D    3   
 */

#define SVN_PRC_GDC_LOCATOR_IMAGE       (SVN_BASE_PROTOCOL_PROCESS + 64)
#define SVT_PRC_GDC_LOCATOR_IMAGE       VFT_INT32
#define SVS_PRC_GDC_LOCATOR_IMAGE       "GDC locator image flag"
/* 
   ����c�ݕ␳�����ʒu���߉摜�t���O
   0 : �ʒu���߉摜�o�^����
   1 : �ʒu���߉摜�o�^�L��

   #define VFC_GDC_LOCATOR_IMAGE_OFF   0
   #define VFC_GDC_LOCATOR_IMAGE_ON    1
 */

#define SVN_PRC_GDC_LOCATOR_MASK        (SVN_BASE_PROTOCOL_PROCESS + 65)
#define SVT_PRC_GDC_LOCATOR_MASK        VFT_INT32
#define SVS_PRC_GDC_LOCATOR_MASK        "GDC locator mask flag"
/* 
   ����c�ݕ␳�����ʒu���߉摜�t���O
   0 : �ʒu���߉摜Mask����
   1 : �ʒu���߉摜Mask�L��

   #define VFC_GDC_LOCATOR_MASK_OFF   0
   #define VFC_GDC_LOCATOR_MASK_ON    1
 */

#define SVN_PRC_INTENSITY_CORR_VIVID1        (SVN_BASE_PROTOCOL_PROCESS + 66)
#define SVT_PRC_INTENSITY_CORR_VIVID1        VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_VIVID1        "Intensity Correction vivid1"
/* 
   �m�C�Y�̕s�ψꐫ�␳�̒��x��\���W��

   �m�C�Y�̕s�ψꐫ�␳�̒��x��\���v���C�}���ȃp�����[�^�B
   0�ł͕␳�Ȃ��A�l���グ��̂ɏ]���ĕs�ψꐫ�␳�̒��x�������Ȃ�B
   �ݒ�\�͈͂́A0%�`100%
 */

#define SVN_PRC_INTENSITY_CORR_VIVID2        (SVN_BASE_PROTOCOL_PROCESS + 67)
#define SVT_PRC_INTENSITY_CORR_VIVID2        VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_VIVID2        "Intensity Correction vivid2"
/* 
   �m�C�Y�̕s�ψꐫ�␳�̒��x��\���W��

   �␳��̉掿��\���B0%���W���I�Ȋ�ƂȂ�B������̒��S�Ō���ɋ߂��Ȃ�B
   0%�𒴂���ƃX���[�W���O�����߂ŁA0%��菬�����Ȃ�ƃG�b�W�����C���̉摜��
   ������B
   �ݒ�\�͈͂́A-50%�`50%
 */

#define SVN_PRC_XYRATIO_FOR_JET             (SVN_BASE_PROTOCOL_PROCESS + 68)
#define SVT_PRC_XYRATIO_FOR_JET             VFT_FLT32
#define SVS_PRC_XYRATIO_FOR_JET             "XYratio for JET"
/* 
   JET�č\���Ɏg�p����RO������PE������K��ԏ�ł̕���\�̔䗦dKx/dKy

   �V�[�P���X���v�Z���A�č\�����g�p����B

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_MOTION_CORR_FLAG_FOR_JET    (SVN_BASE_PROTOCOL_PROCESS + 69)
#define SVT_PRC_MOTION_CORR_FLAG_FOR_JET    VFT_BITFIELD
#define SVS_PRC_MOTION_CORR_FLAG_FOR_JET    "Motion correction flag for JET"
/* 
   JET�̓����␳�t���O

   JET�㏈���n�̃t�B�[���h�B
   ��](rotation) ���i(translation) �d�ݕt��(rejection)��BIT�B

   �ݒ�� PROLIM,recon,sequence
   �g�p�� recon

   #define VFC_MOTION_CORR_FLAG_FOR_JET_ROTATION    (1L << 0)
   #define VFC_MOTION_CORR_FLAG_FOR_JET_TRANSLATION (1L << 1)
   #define VFC_MOTION_CORR_FLAG_FOR_JET_REJECTION   (1L << 2)
 */

#define SVN_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      (SVN_BASE_PROTOCOL_PROCESS + 70)
#define SVT_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      VFT_INT32
#define SVS_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      "Motion correction option method for JET"
/* 
   �����␳�p�����[�^�̍œK���A���S���Y���̎w��

   �ݒ�� PROLIM,recon,sequence 
   �g�p�� recon

   #define VFC_MOTION_CORR_OPT_METHOD_FOR_JET_NONE          0
 */

#define SVN_PRC_REGRIDDING_METHOD_FOR_JET   (SVN_BASE_PROTOCOL_PROCESS + 71)
#define SVT_PRC_REGRIDDING_METHOD_FOR_JET   VFT_INT32
#define SVS_PRC_REGRIDDING_METHOD_FOR_JET   "Regridding method for JET"
/* 
   JET��Cartesian���W�n�ւ�Regridding���@���w�肷��t���O�B

   JET�㏈���n�̃t�B�[���h�B

   �ݒ�� PROLIM,recon,sequence
   �g�p�� recon

   #define VFC_REGRIDDING_METHOD_FOR_JET_NONE              0
   #define VFC_REGRIDDING_METHOD_FOR_JET_JACKSON           1
   #define VFC_REGRIDDING_METHOD_FOR_JET_JACKSON_DECONV    2
   #define VFC_REGRIDDING_METHOD_FOR_JET_ENEN              3
   #define VFC_REGRIDDING_METHOD_FOR_JET_ENEN_DECONV       4
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_METHOD  (SVN_BASE_PROTOCOL_PROCESS + 72)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_METHOD  VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_METHOD  "K space trajectory sort method"
/* 
   K��Ԃ̑|���O���̃\�[�g���@

   SVN_PLN_K_SPACE_TRAJECTORY_TYPE != NONE�̏ꍇ�ɋN�������sorting�v���Z�X�ɂ�����
   sorting��SVN_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD�ɉ������ʑ��␳���s����
   �ǂ����̃t���O�B
   USE_DEFAULT�ł́A((DATA)shot)Ec �̃��[�v�� ((DATA)Ec)Shot�ƕ��ׂ�B
   SORT_FREE���w�肷��ƁASVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE�ɏ]���ă\�[�g����B

   �ݒ�� sequence
   �g�p�� recon

   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_NONE         0
   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_USE_DEFAULT  1
   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_SORT_FREE    2
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    (SVN_BASE_PROTOCOL_PROCESS + 73)
#define SVT_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    "K space trajectory phase correction method"
/* 
   K��Ԃ̑|���O���̈ʑ��␳���@

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE���g�p�����ʑ��␳���@���w�肷��B
   V9.20�ł͈ʑ��␳�͍s��Ȃ��B

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  (SVN_BASE_PROTOCOL_PROCESS + 74)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  "K space trajectory sort table bits"
/* 
   K��Ԃ̑|���O���\�[�g�e�[�u���̃r�b�g��

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE��index(�s����)���w�肷�邽�߂Ɏg�p�����BIT���B
   ���BIT�͂��̃f�[�^�̑�����\������B

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       (SVN_BASE_PROTOCOL_PROCESS + 75)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       "Sort table of k space trajectory"
/* 
   K��Ԃ̑|���O���\�[�g�e�[�u��

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_METHOD��SORT_FREE�̏ꍇ�Ɏg�p����sort table�B

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  (SVN_BASE_PROTOCOL_PROCESS + 76)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  "Sort table size of k space trajectory"
/*
   K��Ԃ̑|���O���\�[�g�e�[�u���̃T�C�Y

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   (SVN_BASE_PROTOCOL_PROCESS + 77)
#define SVT_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   "T2 correction of k space trajectory"
/*
   K��Ԃ̑|���O���̐U���␳���@

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE���g�p�����U���␳���@���w�肷��B
   V9.20�ł͐U���␳�͍s��Ȃ��B

   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_DC_CORR_METHOD_FOR_JET              (SVN_BASE_PROTOCOL_PROCESS + 78)
#define SVT_PRC_DC_CORR_METHOD_FOR_JET              VFT_INT32
#define SVS_PRC_DC_CORR_METHOD_FOR_JET              "DC correction method for JET"
/*
   JET�p�č\���v���Z�X�O�i��DC�␳���s�����߂̃t���O

   #define VFC_DC_CORR_METHOD_FOR_JET_NONE              0
   V9.20�ł�DC�␳�͍s��Ȃ��B


   �ݒ�� sequence
   �g�p�� recon
 */

#define SVN_PRC_DUAL_ECHO_COMBINING_TYPE            (SVN_BASE_PROTOCOL_PROCESS + 79)
#define SVT_PRC_DUAL_ECHO_COMBINING_TYPE            VFT_INT32
#define SVS_PRC_DUAL_ECHO_COMBINING_TYPE            "operation type for combined dual echo"
/*
   2�G�R�[�摜�ɂ����鎩�������摜������On/Off����щ��Z�^�C�v�̎w��

   #define VFC_DUAL_ECHO_COMBINING_NONE                 0
   #define VFC_DUAL_ECHO_COMBINING_ABS_ADD              1
   #define VFC_DUAL_ECHO_COMBINING_ABS_SUB1             2
   #define VFC_DUAL_ECHO_COMBINING_ABS_SUB2             3
   #define VFC_DUAL_ECHO_COMBINING_FWS_WEAK             4
   #define VFC_DUAL_ECHO_COMBINING_FWS_MEDIUM           5
   #define VFC_DUAL_ECHO_COMBINING_FWS_STRONG           6

   �ݒ�� PROLIM
   �g�p�� recon
 */

#define SVN_PRC_DUAL_ECHO_COMBINING_RATIO            (SVN_BASE_PROTOCOL_PROCESS + 80)
#define SVT_PRC_DUAL_ECHO_COMBINING_RATIO            VFT_FLT32
#define SVS_PRC_DUAL_ECHO_COMBINING_RATIO            "weighting factors for combined dual echo"
/*
   2�G�R�[�摜�ɂ����鎩�������摜�����ɂĎg�p����e�G�R�[�̏d�݌W��

   ratio[0]:1�G�R�[�ڂ̏d�݌W��
   ratio[1]:2�G�R�[�ڂ̏d�݌W��

   �ݒ�� PROLIM
   �g�p�� recon
 */

#define SVN_PRC_DUAL_ECHO_DISPLAY_FLAG              (SVN_BASE_PROTOCOL_PROCESS + 81)
#define SVT_PRC_DUAL_ECHO_DISPLAY_FLAG              VFT_INT32
#define SVS_PRC_DUAL_ECHO_DISPLAY_FLAG              "show/hide images for each echo"
/*
   �e�G�R�[�摜�𒆊ԉ摜�Ƃ��邩�ǂ����̃t���O

   VFC_DUAL_ECHO_HIDE        0
   VFC_DUAL_ECHO_SHOW        1

   �ݒ�� PROLIM
   �g�p�� recon
 */

#define SVN_PRC_AFI_ALGORITHM               (SVN_BASE_PROTOCOL_PROCESS + 82)
#define SVT_PRC_AFI_ALGORITHM               VFT_INT32
#define SVS_PRC_AFI_ALGORITHM               "AFI algorithm"
/*
    AFI�������@���

    VFC_AFI_ALGORITHM_MARGOSIAN         0
    VFC_AFI_ALGORITHM_POCS              1
    VFC_AFI_ALGORITHM_FIR               2
    VFC_AFI_ALGORITHM_FIR_POCS          3
    VFC_AFI_ALGORITHM_MOFIR             4
    VFC_AFI_ALGORITHM_MOFIR_POCS        5
    VFC_AFI_ALGORITHM_HYBRID            8
    VFC_AFI_ALGORITHM_POCS_HYBRID       9
    VFC_AFI_ALGORITHM_FIR_HYBRID        10
    VFC_AFI_ALGORITHM_FIR_POCS_HYBRID   11
    VFC_AFI_ALGORITHM_MOFIR_HYBRID      12
    VFC_AFI_ALGORITHM_MOFIR_POCS_HYBRID 13

    �ݒ�� PROLIM,seqgen
    �g�p�� recon
 */

#define SVN_PRC_AFI_POCS_TIMES              (SVN_BASE_PROTOCOL_PROCESS + 83)
#define SVT_PRC_AFI_POCS_TIMES              VFT_INT32
#define SVS_PRC_AFI_POCS_TIMES              "AFI POCS times"
/*
    POCS�������s���ꍇ�̌J��Ԃ���

    �ݒ�� PROLIM,seqgen
    �g�p�� recon
 */

#define SVN_PRC_IMAGE_COMBINATION_RERECON_FLAG  (SVN_BASE_PROTOCOL_PROCESS + 84)
#define SVT_PRC_IMAGE_COMBINATION_RERECON_FLAG  VFT_INT32
#define SVS_PRC_IMAGE_COMBINATION_RERECON_FLAG  "image combination display flag on Mui"
/*
    image combination display flag on Mui

    0 - Image Combination�̃O���[�v�{�b�N�X��Mui�ŕ\�����Ȃ�
    1 - FWS�֘A�����Insensitive�A���̉��Z���[�h�̂ݕ\��
    2 - �S�Ẳ��Z���[�h��\��

    �ݒ�� PROLIM
    �g�p�� RECON
 */

#define SVN_PRC_AUTO_STAMD_MODE         (SVN_BASE_PROTOCOL_PROCESS + 85)
#define SVT_PRC_AUTO_STAMD_MODE         VFT_INT32
#define SVS_PRC_AUTO_STAMD_MODE         "MIP or minIP for auto STAMD(Shift)"
/*
    MIP or minIP for auto STAMD(Shift)

    #define VFC_AUTO_STAMD_MINIP	0 (�ŏ��l���e)
    #define VFC_AUTO_STAMD_MIP		1 (�ő�l���e)

    �ݒ�� PROLIM
    �g�p�� RECON
 */

#define SVN_PRC_AUTO_STAMD_SLICES       (SVN_BASE_PROTOCOL_PROCESS + 86)
#define SVT_PRC_AUTO_STAMD_SLICES       VFT_INT32
#define SVS_PRC_AUTO_STAMD_SLICES       "Units of STAMD slices"
/*
    Units of STAMD slices

    �ݒ�� PROLIM
    �g�p�� RECON
 */

#define SVN_PRC_AUTO_STAMD_NUM_SHIFT   (SVN_BASE_PROTOCOL_PROCESS + 87)
#define SVT_PRC_AUTO_STAMD_NUM_SHIFT   VFT_INT32
#define SVS_PRC_AUTO_STAMD_NUM_SHIFT   "Number of shifted slices in STAMD"
/*
    Number of shifted slices in STAMD

    �ݒ�� PROLIM
    �g�p�� RECON
 */

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_PROCESS	SVN_MIN_SUBPROTOCOL + SVN_OFF_PROCESS

#define SVN_PRC_RETRO_VIEWTBL_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 0)
#define SVT_PRC_RETRO_VIEWTBL_SIZE	VFT_INT32
#define SVS_PRC_RETRO_VIEWTBL_SIZE	"Number of entries in view table"
/*	  Retrospctive Gating�pview�e�[�u���̗v�f���B
	*/

#define SVN_PRC_RETRO_VIEWTBL_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 1)
#define SVT_PRC_RETRO_VIEWTBL_TYPE	VFT_INT32
#define SVS_PRC_RETRO_VIEWTBL_TYPE	"Type of view table"
/*	  Retrospctive Gating�pview�e�[�u���̃f�[�^�^�C�v�B
	*/

#define SVN_PRC_RETRO_VIEW_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 2)
#define SVT_PRC_RETRO_VIEW_TABLE	VFT_INT32
#define SVS_PRC_RETRO_VIEW_TABLE	"View table"
/*	  Retrospctive Gating�pview�e�[�u���B
	  �ʑ��G���R�[�h�APS�G���R�[�h�A�A�x���[�W���O�A�X���C�X�A�X���u��
	  �f�[�^�Z�b�g�����W���Ɋi�[����B
	*/

#define SVN_PRC_RETRO_TIMING_PLAN_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 3)
#define SVT_PRC_RETRO_TIMING_PLAN_SIZE	VFT_INT32
#define SVS_PRC_RETRO_TIMING_PLAN_SIZE	"Number of entires of planed ADC timing table"
/*	  Retrospctive Gating�p�v�掞���W�^�C�~���O�e�[�u���̗v�f���B
	*/

#define SVN_PRC_RETRO_TIMING_PLAN_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 4)
#define SVT_PRC_RETRO_TIMING_PLAN_TYPE	VFT_INT32
#define SVS_PRC_RETRO_TIMING_PLAN_TYPE	"Type of timing table"
/*	 Retrospctive Gating�p�v�掞���W�^�C�~���O�e�[�u���̃f�[�^�^�C�v�B 
	*/

#define SVN_PRC_RETRO_TIMING_PLAN	(SVN_BASE_SUBPROTOCOL_PROCESS + 5)
#define SVT_PRC_RETRO_TIMING_PLAN	VFT_FLT32
#define SVS_PRC_RETRO_TIMING_PLAN	"Planed ADC timing table"
/*	 Retrospctive Gating�p�v�掞���W�^�C�~���O�e�[�u���B
	 �G�R�[���W�������A�V�[�P���X�J�n�������[���Ƃ���msec�P�ʂŔz�񂷂�B 
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 6)
#define SVT_PRC_RETRO_AVERAGING_TABLE_SIZE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE_SIZE	"Number of entries of averaging phase and read-out reversal table"
/*	 Retrospctive Gating�p�A�x���[�W���O�e�[�u���̗v�f���B 
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 7)
#define SVT_PRC_RETRO_AVERAGING_TABLE_TYPE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE_TYPE	"Type of averaging phase and read-out reversal table"
/*	  Retrospctive Gating�p�A�x���[�W���O�e�[�u���̃^�C�v�B
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 8)
#define SVT_PRC_RETRO_AVERAGING_TABLE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE	"Table of averaging phase and read-out reversal"
/*	  Retrospctive Gating�p�A�x���[�W���O�e�[�u���B
	  ���Z�����̈ʑ����]�����EPI�̃G�R�[���]�Ɋւ������z�񂷂�B
�@�@�@�@�l�F�@�O�@�@�@�@�ʑ����]�Ȃ��A�G�R�[���]�Ȃ�
�@�@�@�@�l�F�@�Q�@�@�@�@�ʑ����]����A�G�R�[���]�Ȃ�
�@�@�@�@�l�F�@�P�U�@�@�@�ʑ����]�Ȃ��A�G�R�[���]����
�@�@�@�@�l�F�@�P�W�@�@�@�ʑ����]����A�G�R�[���]����

	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 9)
#define SVT_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	"Number of entries in ALL_ADCCOUNT_TABLE"
/*	  Retrospctive Gating(MODE_2)�pSteadyLoop�����W�񐔃e�[�u���̗v�f���B
	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 10)
#define SVT_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	"Type of ALL_ADCCOUNT_TABLE"
/*	  Retrospctive Gating(MODE_2)�pSteadyLoop�����W�񐔃e�[�u���̃f�[�^�^�C�v�B
	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNT_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 11)
#define SVT_PRC_RETRO_ALL_ADCCOUNT_TABLE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNT_TABLE	"Table of adc counts in a steady loop"
/*	  Retrospctive Gating(MODE_2)�pSteadyLoop�����W�񐔃e�[�u���B
	  SteadyLoop�����W�񐔂����W���Ɋi�[����B
	*/

#define SVN_PRC_MOTION_ROTATION_RESULTS_FOR_JET		(SVN_BASE_SUBPROTOCOL_PROCESS + 12)
#define SVT_PRC_MOTION_ROTATION_RESULTS_FOR_JET		VFT_FLTVEC2
#define SVS_PRC_MOTION_ROTATION_RESULTS_FOR_JET		"Motion ratation results for JET"
/*	  JET�̉�]�␳(rotation correction)�̌��ʂ��L�^����t�B�[���h�B
	  2D�̏ꍇ(0,��)�ƂȂ�AnumBlade �~ numSlice���L�^�����B

	  �ݒ�� recon
	  �g�p�� recon
	*/

#define SVN_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	(SVN_BASE_SUBPROTOCOL_PROCESS + 13)
#define SVT_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	VFT_FLTVEC3
#define SVS_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	"Motion translation results for JET"
/*	  JET�̕��i�␳(translation correction)�̌��ʂ��L�^����t�B�[���h�B
	  3D�̏ꍇ(ro,pe,0)�ƂȂ�AnumBlade �~ numSlice���L�^�����B

	  �ݒ�� recon
	  �g�p�� recon
	*/

#define SVN_PRC_MOTION_REJECTION_RESULTS_FOR_JET	(SVN_BASE_SUBPROTOCOL_PROCESS + 14)
#define SVT_PRC_MOTION_REJECTION_RESULTS_FOR_JET	VFT_FLT32
#define SVS_PRC_MOTION_REJECTION_RESULTS_FOR_JET	"Motion rejection results for JET"
/*	  JET�̏d�ݕt��(rejection correction)�̌��ʂ��L�^����t�B�[���h�B
	  V9.20�ł�rejection correction�͍s��Ȃ����߁A���1�Ƃ���B
	  numBlade �~ numSlice���L�^�����B

	  �ݒ�� recon
	  �g�p�� recon
	*/

/*****************************************************************************
  PROCESSED name space
*/

#define SVN_BASE_PROCESSED_PROCESS	(SVN_MIN_PROCESSED + SVN_OFF_PROCESS)

#define SVN_PRC_DATA_KIND		(SVN_BASE_PROCESSED_PROCESS + 0)
#define SVT_PRC_DATA_KIND	 	VFT_INT32
#define SVS_PRC_DATA_KIND	 	"Data kind"
/*	Indicates the kind of the data and which calculation process was
	used to produce the data:
	processed     - standard reconstruction, 
	intermediate  - partial  reconstruction,
	mra_projected - postprocessed by MRA-projection,
	resliced      - postprocessed by oblique reformatting,
	... 
	*/

#define SVN_PRC_ANC_SBP_INDX		(SVN_BASE_PROCESSED_PROCESS + 1)
#define SVT_PRC_ANC_SBP_INDX		VFT_INT32
#define SVS_PRC_ANC_SBP_INDX		"Ancestral subprotocol index"
/* 	The index of SUBPROTOCOL branch which had been used to produce the
	current  SVD_ST_PROCESSED branch
	*/

#define SVN_PRC_ANC_SLG_INDX		(SVN_BASE_PROCESSED_PROCESS + 2)
#define SVT_PRC_ANC_SLG_INDX		VFT_INT32
#define SVS_PRC_ANC_SLG_INDX		"Ancestral slice group index"
/* 	The index of SLICEGROUP branch which had been used to produce the
	current  SVD_ST_PROCESSED branch
	*/

#define	SVN_PRC_ROI_PROC_INDX		(SVN_BASE_PROCESSED_PROCESS + 3)
#define SVT_PRC_ROI_PROC_INDX		VFT_INT32
#define SVS_PRC_ROI_PROC_INDX   	"ROI-map SVD_ST_PROCESSED index"
/*	The index of SVD_ST_PROCESSED branch which contains the region
	of interest bit-map which has been used to produce the current
	SVD_ST_PROCESSED branch (postprocessing)
	*/

#define	SVN_PRC_ANC_PROC_INDX		(SVN_BASE_PROCESSED_PROCESS + 4)
#define	SVT_PRC_ANC_PROC_INDX		VFT_INT32
#define	SVS_PRC_ANC_PROC_INDX		"Ancestral SVD_ST_PROCESSED index"
/*	The index of SVD_ST_PROCESSED branch which  has been used to
	produce the current SVD_ST_PROCESSED branch (postprocessing)
	*/

#define	SVN_PRC_ANC_ECHO_INDX		(SVN_BASE_PROCESSED_PROCESS + 5)
#define	SVT_PRC_ANC_ECHO_INDX		VFT_INT32
#define	SVS_PRC_ANC_ECHO_INDX		"Ancestral SVD_ST_ECHO index"
/*	The index of SVD_ST_ECHO branch which  has been used to
	produce the current SVD_ST_PROCESSED branch (postprocessing)
	*/

#define	SVN_PRC_ANC_POS_INDX		(SVN_BASE_PROCESSED_PROCESS + 6)
#define	SVT_PRC_ANC_POS_INDX		VFT_INT32
#define	SVS_PRC_ANC_POS_INDX		"Ancestral SVD_ST_POSITION index"
/*	The index of SVD_ST_POSITION branch which  has been used to
	produce the current SVD_ST_PROCESSED branch (postprocessing)
	*/

#define	SVN_PRC_ANC_POS_RANGE		(SVN_BASE_PROCESSED_PROCESS + 7)
#define	SVT_PRC_ANC_POS_RANGE		VFT_INT32
#define	SVS_PRC_ANC_POS_RANGE		"Range of ancestral SVD_ST_POSITION indicies"
/*	The range (first--last) of the slice SVD_ST_POSITION indicies which
	have been used to produce the current SVD_ST_PROCESSED branch
	(postprocessing)
	*/

#define	SVN_PRC_ANC_PROT_INDX		(SVN_BASE_PROCESSED_PROCESS + 8)
#define	SVT_PRC_ANC_PROT_INDX		VFT_INT32
#define	SVS_PRC_ANC_PROT_INDX		"Ancestral SVD_ST_PROTOCOL index"
/*	The index of SVD_ST_PROTOCOL subtree which  has been used to
	produce the current SVD_ST_PROCESSED branch (postprocessing)
	*/

/* Added by K.Kokubun	Jan-26-1994 */
#define SVN_PRC_CALC			(SVN_BASE_PROCESSED_PROCESS + 9)
#define SVT_PRC_CALC			VFT_INT32
#define SVS_PRC_CALC			"Calculate images"

#define SVN_PRC_DYNAMIC_SEG		(SVN_BASE_PROCESSED_PROCESS + 10)
#define SVT_PRC_DYNAMIC_SEG		VFT_INT32
#define SVS_PRC_DYNAMIC_SEG		"Segment number of dynamic scan"
/*	�_�C�i�~�b�N�X�L������iselector���\������ہA����Z�O�����g�ԍ���
	�O���[�v������ׂɎg�p����B
	*/

#define SVN_PRC_STATUS			(SVN_BASE_PROCESSED_PROCESS + 11)
#define SVT_PRC_STATUS			VFT_INT32
#define SVS_PRC_STATUS			"Finish creating new node"
/*	�m�[�h����������ʒm����B
	���̃t�B�[���h������΃m�[�h�����������Ƃ݂Ȃ��B
	*/

#define SVN_PRC_ROW_CONTROL_FLAG	(SVN_BASE_PROCESSED_PROCESS + 12)
#define SVT_PRC_ROW_CONTROL_FLAG	VFT_INT32
#define SVS_PRC_ROW_CONTROL_FLAG	"The flag to controll image selector"
/*	SVN_PRC_ROW_ID,SVN_PRC_GROUP_ID �ɂ��I-selector��
	�̕��ѕ��𐧌䂷�邱�Ƃ������t���O�B
	Processed Node �ɓo�^����B
	SVN_PRC_ROW_CONTROL_FLAG�̒l(�^�^�U�A�g�����̂���BOOL
	�ł͂Ȃ�INT���g�p)�ɂ���� row-id,group-id �ɂ�����
	�ς����s�Ȃ��B�������A���̃t�B�[���h�����݂��Ȃ��ꍇ
	�U���ݒ肳�ꂽ�ꍇ�Ɠ��l�̏������s�Ȃ��B
	���̃t���O���^�ƂȂ� Processed �͑S�Ẳ摜�ɂ���
	row-id/group-id ����`����Ă��Ȃ��Ă͂Ȃ�Ȃ��B
	��O�Ƃ��� row �܂��� group ��������̏ꍇ�A
	�ȗ����\�B
	*/

#define SVN_PRC_TOTAL_SLAB_NUM		(SVN_BASE_PROCESSED_PROCESS + 13)
#define SVT_PRC_TOTAL_SLAB_NUM		VFT_INT32
#define SVS_PRC_TOTAL_SLAB_NUM		"Total number of slabs"
/*	���X���u���B
	Processed Node �ɓo�^����B
	*/

#define SVN_PRC_IMG_INFO		(SVN_BASE_PROCESSED_PROCESS + 14)
#define SVT_PRC_IMG_INFO		VFT_UINT8
#define SVS_PRC_IMG_INFO		"Image Infomation"
/*	�摜��ʎ��ʏ�� 
	���̃t�B�[���h�͉ϒ��ɂȂ��Ă���̂Ń^�C�v��VFT_UINT8�Ƃ���B
	�A�������͈ȉ��̗l�ȍ\�������Ă���B
	typedef	struct {
	    size_t	infoType_depth		;  �摜��ʐ� 
	    unit32_t	infoType[infoType_depth];  �摜��� 
	    unit32_t	infoSubType		;  �摜��ʏڍ� 
	} ImgInfo_t ;
	�擪�ɂ͉摜��ʐ�(infoType_depth)���i�[����A���̌��
	�����̉摜���(infoType)���i�[����A�Ō�ɃJ�����g��
	�摜��ʏڍ�(infoSubType)���i�[�����B
	*/

#define SVN_PRC_PAC_RECON_IMAGE		(SVN_BASE_PROCESSED_PROCESS + 15)
#define SVT_PRC_PAC_RECON_IMAGE		VFT_BITFIELD
#define SVS_PRC_PAC_RECON_IMAGE		"Image type of PAC"
/*	PAC�̍č\���摜�� �����摜�Ȃ̂��e�`���l���̉摜�Ȃ̂���
	��ʂ��邽�߂ɗp����B
	*/

#define SVN_PRC_POST_PROC_DESC		(SVN_BASE_PROCESSED_PROCESS + 16)
#define SVT_PRC_POST_PROC_DESC		VFT_STRING
#define SVS_PRC_POST_PROC_DESC		"Post Processing Description"
/* �㏈�����
 *	(a) msPostProc�͏��������|�X�g�v���b�N�t�@�C�����e���i�[����B
 */

#define SVN_PRC_PROC_TIME	(SVN_BASE_PROCESSED_PROCESS + 17)
#define SVT_PRC_PROC_TIME	VFT_INT32
#define SVS_PRC_PROC_TIME	"Image Processing Start and End Time"
/*	�傫���Q�̔z��
	array[0] : �摜�����̊J�n����
	array[1] : �摜�����̏I������
	*/

#define SVN_PRC_FILTERING_CODE	(SVN_BASE_PROCESSED_PROCESS + 18)
#define SVT_PRC_FILTERING_CODE	VFT_STRING
#define SVS_PRC_FILTERING_CODE	"Filtering Code for Reconstruction"
/*	�t�B���^�[�i�����R�[�h�\�L�j�́A�t�B���^�[�f�[�^�x�[�X��k-space
	�t�B���^�[��Refine�t�B���^�̓����R�[�h�Ƃ���B�i�������A-1�́���
	�\������B�j
	*/

#define SVN_PRC_DELTA_R2_BASE_TIME	(SVN_BASE_PROCESSED_PROCESS + 19)
#define SVT_PRC_DELTA_R2_BASE_TIME	VFT_INT32
#define SVS_PRC_DELTA_R2_BASE_TIME	"Delta R2* Base Time"
/*	�f���^R2���̊����
	�č\�����f���^R2�摜�쐬���Ƀv���Z�X�h�m�[�h�Ɋi�[����B
	�P�ʂ�msec�B
	*/

#define SVN_PRC_PREP_SCAN_TIME	(SVN_BASE_PROCESSED_PROCESS + 20)
#define SVT_PRC_PREP_SCAN_TIME	VFT_INT32
#define SVS_PRC_PREP_SCAN_TIME	"Prep Scan Time"
/*	�v���b�v�X�L�����J�n��{�X�L�������J�n����܂ł̎���
	msRecon�n�͖{�X�L�����̎B�e�B�e���Ԃƃv���b�v�X�L�����̎B�e
	�J�n���Ԃ̍������߃Z�b�g����B�P�ʂ̓~���b�B
	*/

#define SVN_PRC_MAP_EXIST_FLAG	(SVN_BASE_PROCESSED_PROCESS + 21)
#define SVT_PRC_MAP_EXIST_FLAG	VFT_BOOL
#define SVS_PRC_MAP_EXIST_FLAG	"Exist Sensitivity data"
/*
SPEEDER�̊��x�}�b�v ���ł�����������A�č\���͊��x����p�X�L������
�v���Z�X�h�m�[�h�ɏ������ށB
  True : SVN_PLN_MAP_PATH�ɂ��銴�x�}�b�v�f�[�^�����݂���B
  False: ���݂��Ȃ��B
*/

#define SVN_PRC_MAP_DATA	(SVN_BASE_PROCESSED_PROCESS + 22)
#define SVS_PRC_MAP_DATA	"Sensitivity Map"
/* SPEEDER�̊��x�}�b�v
 ���ҍ��W�n�ɂāA�{�����[���f�[�^�̌`�ł��B
 �I�u���[�N�֎~�Ȃ̂ŁA�傫���́AxDim,yDim,zDim�̂R�̗v�f�ŕ\���ł���B
�@�v���Z�X�h�m�[�h�ɏ������ށB
�@���f�[�^�Ɠ����t�@�C���Ɋi�[����B
�i�������ݎ��ɁA�O���[�vID���w�肷�邱�Ƃŉ\�j
�@��F64x64x64�Ȃ�΁A
      int16_t,complex�̏ꍇ�A�PMB�@�ƂȂ�A���ꂪ�`���l�����ł���B
*/

#define SVN_PRC_MAP_DATA1	(SVN_BASE_PROCESSED_PROCESS + 23)
#define SVS_PRC_MAP_DATA1	"Sensitivity Map"

#define SVN_PRC_MAP_DATA2	(SVN_BASE_PROCESSED_PROCESS + 24)
#define SVS_PRC_MAP_DATA2	"Sensitivity Map"

#define SVN_PRC_MAP_DATA3	(SVN_BASE_PROCESSED_PROCESS + 25)
#define SVS_PRC_MAP_DATA3	"Sensitivity Map"

#define SVN_PRC_MAP_DATA4	(SVN_BASE_PROCESSED_PROCESS + 26)
#define SVS_PRC_MAP_DATA4	"Sensitivity Map"

#define SVN_PRC_MAP_DATA5	(SVN_BASE_PROCESSED_PROCESS + 27)
#define SVS_PRC_MAP_DATA5	"Sensitivity Map"

#define SVN_PRC_MAP_DATA6	(SVN_BASE_PROCESSED_PROCESS + 28)
#define SVS_PRC_MAP_DATA6	"Sensitivity Map"

#define SVN_PRC_MAP_DATA7	(SVN_BASE_PROCESSED_PROCESS + 29)
#define SVS_PRC_MAP_DATA7	"Sensitivity Map"
/* SPEEDER�̊��x�}�b�v (�W�`���l���Ή��j
*/

#define SVN_PRC_MAP_DATA_TYPE	(SVN_BASE_PROCESSED_PROCESS + 30)
#define SVT_PRC_MAP_DATA_TYPE	VFT_INT32
#define SVS_PRC_MAP_DATA_TYPE	"Sensitivity data type"
/* SPEEDER�̊��x�}�b�v �̃^�C�v
�@�v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_DATA_FORM	(SVN_BASE_PROCESSED_PROCESS + 31)
#define SVT_PRC_MAP_DATA_FORM	VFT_INT32
#define SVS_PRC_MAP_DATA_FORM	"Sensitivity data form"
/* SPEEDER�̊��x�}�b�v �̃t�H�[��
   �v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_DATA_DIM	(SVN_BASE_PROCESSED_PROCESS + 32)
#define SVT_PRC_MAP_DATA_DIM	VFT_INT32
#define SVS_PRC_MAP_DATA_DIM	"Sensitivity data dimensions"
/* SPEEDER�̊��x�}�b�v �̑傫��
 �v���Z�X�h�m�[�h�ɏ������ށB
   ���ҍ��W�n�ɂāA�{�����[���f�[�^�̌`�ł��B
 �I�u���[�N�֎~�Ȃ̂ŁA�傫���́AxDim,yDim,zDim�̂R�̗v�f�ŕ\���ł���B
 �i��{�I�ɂ́Ax,y,z�̏��ł���B�j
*/

#define SVN_PRC_MAP_DATA_DESC	(SVN_BASE_PROCESSED_PROCESS + 33)
#define SVT_PRC_MAP_DATA_DESC	VFT_INT32
#define SVS_PRC_MAP_DATA_DESC	"Sensitivity data dimension descriptions"
/* SVN_PRC_MAP_DATA_DIM ���ǂ̏��Ԃœ����Ă��邩���L�q�B
   ���ҍ��W�n�ł�X���W�AY���W�AZ���W�̂R�̗v�f�����B
�@�v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_START_POSITION	(SVN_BASE_PROCESSED_PROCESS + 34)
#define SVT_PRC_MAP_START_POSITION	VFT_FLTVEC3
#define SVS_PRC_MAP_START_POSITION	"Sensitivity data Start Position"
/* SVN_PRC_MAP_DATA �̃X�^�[�g�|�W�V����
 ���ҍ��W�n�ł�X���W�AY���W�AZ���W�ɂ�����ŏ����W���Z�b�g�����B
 �v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_END_POSITION	(SVN_BASE_PROCESSED_PROCESS + 35)
#define SVT_PRC_MAP_END_POSITION	VFT_FLTVEC3
#define SVS_PRC_MAP_END_POSITION	"Sensitivity data End Position"
/* SVN_PRC_MAP_DATA �̃G���h�|�W�V����
�@���ҍ��W�n�ł�X���W�AY���W�AZ���W�ɂ�����ő���W���Z�b�g�����B
 �v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_STEP	(SVN_BASE_PROCESSED_PROCESS + 36)
#define SVT_PRC_MAP_STEP	VFT_FLTVEC3
#define SVS_PRC_MAP_STEP	"Sensitivity data Step"
/* SVN_PRC_MAP_DATA �̍��ݕ�
 �v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_MAP_D_INV_TABLE (SVN_BASE_PROCESSED_PROCESS + 37)
#define SVT_PRC_MAP_D_INV_TABLE VFT_FLT32
#define SVS_PRC_MAP_D_INV_TABLE "Sensitivity data Displacements in each dimension"
/*      (1) ���x�}�b�v�̒u���x�N�g��(ro,pe,se) ->  (x,y,z)��ݒ肷��B
        VFT_INT32 �~ �X �̔z���
      Dinvmap[0] : x.ro
      Dinvmap[1] : x.pe
      Dinvmap[2] : x.se
      Dinvmap[3] : y.ro
      Dinvmap[4] : y.pe
      Dinvmap[5] : y.se
      Dinvmap[6] : z.ro
      Dinvmap[7] : z.pe
      Dinvmap[8] : z.se
     (2) SPEEDER�v���X�L�����̍č\�����A���x�}�b�v�쐬���ɐݒ肷��B
     (3) SPEEDER�{�X�L�����̍č\�����A���x�}�b�v�؂�o���� �܂��́AUNFOLD��������
         �g�p����B
*/

#define SVN_PRC_MAP_DATA_SCALE  (SVN_BASE_PROCESSED_PROCESS + 38)
#define SVT_PRC_MAP_DATA_SCALE  VFT_FLT32
#define SVS_PRC_MAP_DATA_SCALE  "Sensitivity data scale factor"
/*      ���x�}�b�v�f�[�^�̃X�P�[��

The scale factor that was used to normalize data in an SVN_PRC_MAP_DATA* varfield.  To restore the data to its original range, divide the data by this scale fac
*/

#define SVN_PRC_INTENSITY_MAP_DATA	(SVN_BASE_PROCESSED_PROCESS + 39)
#define SVS_PRC_INTENSITY_MAP_DATA      "Intensity Correction Map"
/* �P�x�␳�}�b�v
  �`���́ASVN_PRC_MAP_DATA �Ɠ��l�B
�@�␳�p�f�[�^���W�p�č\���̏ꍇ�ɁA�iSOS/WB�j�����ɍ쐬����}�b�v�B
  ���ҍ��W�n�ɂāA�{�����[���f�[�^�̌`�ł��B
  �I�u���[�N�֎~�Ȃ̂ŁA�傫���́AxDim,yDim,zDim�̂R�̗v�f�ŕ\���ł���B
�@�v���Z�X�h�m�[�h�ɏ������ށB
�@���f�[�^�Ɠ����t�@�C���Ɋi�[����B�i�������ݎ��ɁA�O���[�vID���w�肷�邱�Ƃŉ\�j
�@type,form,dim,desc,scale���́A���x�}�b�v�Ɠ��l�̂��̂��g�p����B
�@��F64x64x64�Ȃ�΁A
      int16_t,complex�̏ꍇ�A�PMB�@�ƂȂ�A���ꂪ�v���Z�X�h�ɂP�ł���B*/

#define SVN_PRC_MAP_MASK_DATA	(SVN_BASE_PROCESSED_PROCESS + 40)
#define SVS_PRC_MAP_MASK_DATA   "Sensitivity Map Mask"
/* �}�b�v�̃}�X�N�f�[�^
  �`���́ASVN_PRC_MAP_DATA �Ɠ��l�B
�@�␳�p�f�[�^���W�p�č\���̏ꍇ�ɁAWB�f�[�^�����ɍ쐬����}�b�v�̃}�X�N�f�[�^�B
  ���ҍ��W�n�ɂāA�{�����[���f�[�^�̌`�ł��B
  �I�u���[�N�֎~�Ȃ̂ŁA�傫���́AxDim,yDim,zDim�̂R�̗v�f�ŕ\���ł���B
�@�v���Z�X�h�m�[�h�ɏ������ށB
�@���f�[�^�Ɠ����t�@�C���Ɋi�[����B�i�������ݎ��ɁA�O���[�vID���w�肷�邱�Ƃŉ\�j
�@type,form,dim,desc,scale���́A���x�}�b�v�Ɠ��l�̂��̂��g�p����B
�@��F64x64x64�Ȃ�΁A
      int16_t,complex�̏ꍇ�A�PMB�@�ƂȂ�A���ꂪ�v���Z�X�h�ɂP�ł���B*/

#define SVN_PRC_GDC_IMAGE_KIND          (SVN_BASE_PROCESSED_PROCESS + 41)
#define SVT_PRC_GDC_IMAGE_KIND          VFT_INT32
#define SVS_PRC_GDC_IMAGE_KIND          "GDC image kind"
/* 
   ����c�ݕ␳�����摜���
   0 : �ʏ�摜
   1 : ����c�ݕ␳�摜
   2 : ����c�ݕ␳�����ʒu���߉摜
   3 : ����c�ݕ␳�����ʒu���߉摜(�}�X�N�L��)
 */

#define SVN_PRC_NUMBER_OF_SLICES_IN_SLAB    (SVN_BASE_PROCESSED_PROCESS + 42)
#define SVT_PRC_NUMBER_OF_SLICES_IN_SLAB    VFT_INT32
#define SVS_PRC_NUMBER_OF_SLICES_IN_SLAB    "Number of slices in slab"
/* 
   (�}���`)�X���u���ɑ��݂���X���C�X����
*/

#define SVN_PRC_ADC_EXEC    (SVN_BASE_PROCESSED_PROCESS + 43)
#define SVT_PRC_ADC_EXEC    VFT_INT32
#define SVS_PRC_ADC_EXEC    "ADC executing flag"
/*
   ADC�������s�t���O�FADC���������{���ꂽ�o�������邩���f����t���O
   0 : ADC�����Ȃ�
   1 : ADC�������{�ς�
*/

#define SVN_PRC_ISO_REF_PATH    (SVN_BASE_PROCESSED_PROCESS + 44)
#define SVT_PRC_ISO_REF_PATH    VFT_INT32
#define SVS_PRC_ISO_REF_PATH    "Processed Index of the Parent image of Isotropic image"
/*
  Isotropic�摜�̊�摜�̃v���Z�X�C���f�b�N�X�ԍ��FIsotropic�摜�̃X�P�[�����O�����{
  ����ꍇ�Ɋ�ƂȂ�摜�Q�̃p�X���`����B
*/

#define SVN_PRC_MAP_AVERAGE    (SVN_BASE_PROCESSED_PROCESS + 45)
#define SVT_PRC_MAP_AVERAGE    VFT_FLT32
#define SVS_PRC_MAP_AVERAGE    "Brightness mean in the central part of the map"
/* 
   �}�b�v�̒��S���̋P�x���ϒl
   V8.00 SPEEDER/�P�x�␳�m�C�Y�⏞�t�B���^�Œǉ�
   �v���Z�X�h�m�[�h�ɏ������ށB
*/

#define SVN_PRC_PERFUSION_MAP   (SVN_BASE_PROCESSED_PROCESS + 46)
#define SVT_PRC_PERFUSION_MAP   VFT_INT32
#define SVS_PRC_PERFUSION_MAP   "The detail of Image kind"
/*
    ADC�����̎��s�����t���O�B
*/

#define SVN_PRC_MAP_DATA8	(SVN_BASE_PROCESSED_PROCESS + 47)
#define SVS_PRC_MAP_DATA8	"Sensitivity Map"

#define SVN_PRC_MAP_DATA9	(SVN_BASE_PROCESSED_PROCESS + 48)
#define SVS_PRC_MAP_DATA9	"Sensitivity Map"

#define SVN_PRC_MAP_DATA10	(SVN_BASE_PROCESSED_PROCESS + 49)
#define SVS_PRC_MAP_DATA10	"Sensitivity Map"

#define SVN_PRC_MAP_DATA11	(SVN_BASE_PROCESSED_PROCESS + 50)
#define SVS_PRC_MAP_DATA11	"Sensitivity Map"

#define SVN_PRC_MAP_DATA12	(SVN_BASE_PROCESSED_PROCESS + 51)
#define SVS_PRC_MAP_DATA12	"Sensitivity Map"

#define SVN_PRC_MAP_DATA13	(SVN_BASE_PROCESSED_PROCESS + 52)
#define SVS_PRC_MAP_DATA13	"Sensitivity Map"

#define SVN_PRC_MAP_DATA14	(SVN_BASE_PROCESSED_PROCESS + 53)
#define SVS_PRC_MAP_DATA14	"Sensitivity Map"

#define SVN_PRC_MAP_DATA15	(SVN_BASE_PROCESSED_PROCESS + 54)
#define SVS_PRC_MAP_DATA15	"Sensitivity Map"

#define SVN_PRC_MAP_DATA16	(SVN_BASE_PROCESSED_PROCESS + 55)
#define SVS_PRC_MAP_DATA16	"Sensitivity Map"

#define SVN_PRC_MAP_DATA17	(SVN_BASE_PROCESSED_PROCESS + 56)
#define SVS_PRC_MAP_DATA17	"Sensitivity Map"

#define SVN_PRC_MAP_DATA18	(SVN_BASE_PROCESSED_PROCESS + 57)
#define SVS_PRC_MAP_DATA18	"Sensitivity Map"

#define SVN_PRC_MAP_DATA19	(SVN_BASE_PROCESSED_PROCESS + 58)
#define SVS_PRC_MAP_DATA19	"Sensitivity Map"

#define SVN_PRC_MAP_DATA20	(SVN_BASE_PROCESSED_PROCESS + 59)
#define SVS_PRC_MAP_DATA20	"Sensitivity Map"

#define SVN_PRC_MAP_DATA21	(SVN_BASE_PROCESSED_PROCESS + 60)
#define SVS_PRC_MAP_DATA21	"Sensitivity Map"

#define SVN_PRC_MAP_DATA22	(SVN_BASE_PROCESSED_PROCESS + 61)
#define SVS_PRC_MAP_DATA22	"Sensitivity Map"

#define SVN_PRC_MAP_DATA23	(SVN_BASE_PROCESSED_PROCESS + 62)
#define SVS_PRC_MAP_DATA23	"Sensitivity Map"

#define SVN_PRC_MAP_DATA24	(SVN_BASE_PROCESSED_PROCESS + 63)
#define SVS_PRC_MAP_DATA24	"Sensitivity Map"

#define SVN_PRC_MAP_DATA25	(SVN_BASE_PROCESSED_PROCESS + 64)
#define SVS_PRC_MAP_DATA25	"Sensitivity Map"

#define SVN_PRC_MAP_DATA26	(SVN_BASE_PROCESSED_PROCESS + 65)
#define SVS_PRC_MAP_DATA26	"Sensitivity Map"

#define SVN_PRC_MAP_DATA27	(SVN_BASE_PROCESSED_PROCESS + 66)
#define SVS_PRC_MAP_DATA27	"Sensitivity Map"

#define SVN_PRC_MAP_DATA28	(SVN_BASE_PROCESSED_PROCESS + 67)
#define SVS_PRC_MAP_DATA28	"Sensitivity Map"

#define SVN_PRC_MAP_DATA29	(SVN_BASE_PROCESSED_PROCESS + 68)
#define SVS_PRC_MAP_DATA29	"Sensitivity Map"

#define SVN_PRC_MAP_DATA30	(SVN_BASE_PROCESSED_PROCESS + 69)
#define SVS_PRC_MAP_DATA30	"Sensitivity Map"

#define SVN_PRC_MAP_DATA31	(SVN_BASE_PROCESSED_PROCESS + 70)
#define SVS_PRC_MAP_DATA31	"Sensitivity Map"

#define SVN_PRC_MAP_DATA32	(SVN_BASE_PROCESSED_PROCESS + 71)
#define SVS_PRC_MAP_DATA32	"Sensitivity Map"

#define SVN_PRC_MAP_DATA33	(SVN_BASE_PROCESSED_PROCESS + 72)
#define SVS_PRC_MAP_DATA33	"Sensitivity Map"

#define SVN_PRC_MAP_DATA34	(SVN_BASE_PROCESSED_PROCESS + 73)
#define SVS_PRC_MAP_DATA34	"Sensitivity Map"

#define SVN_PRC_MAP_DATA35	(SVN_BASE_PROCESSED_PROCESS + 74)
#define SVS_PRC_MAP_DATA35	"Sensitivity Map"

#define SVN_PRC_MAP_DATA36	(SVN_BASE_PROCESSED_PROCESS + 75)
#define SVS_PRC_MAP_DATA36	"Sensitivity Map"

#define SVN_PRC_MAP_DATA37	(SVN_BASE_PROCESSED_PROCESS + 76)
#define SVS_PRC_MAP_DATA37	"Sensitivity Map"

#define SVN_PRC_MAP_DATA38	(SVN_BASE_PROCESSED_PROCESS + 77)
#define SVS_PRC_MAP_DATA38	"Sensitivity Map"

#define SVN_PRC_MAP_DATA39	(SVN_BASE_PROCESSED_PROCESS + 78)
#define SVS_PRC_MAP_DATA39	"Sensitivity Map"

#define SVN_PRC_MAP_DATA40	(SVN_BASE_PROCESSED_PROCESS + 79)
#define SVS_PRC_MAP_DATA40	"Sensitivity Map"

#define SVN_PRC_MAP_DATA41	(SVN_BASE_PROCESSED_PROCESS + 80)
#define SVS_PRC_MAP_DATA41	"Sensitivity Map"

#define SVN_PRC_MAP_DATA42	(SVN_BASE_PROCESSED_PROCESS + 81)
#define SVS_PRC_MAP_DATA42	"Sensitivity Map"

#define SVN_PRC_MAP_DATA43	(SVN_BASE_PROCESSED_PROCESS + 82)
#define SVS_PRC_MAP_DATA43	"Sensitivity Map"

#define SVN_PRC_MAP_DATA44	(SVN_BASE_PROCESSED_PROCESS + 83)
#define SVS_PRC_MAP_DATA44	"Sensitivity Map"

#define SVN_PRC_MAP_DATA45	(SVN_BASE_PROCESSED_PROCESS + 84)
#define SVS_PRC_MAP_DATA45	"Sensitivity Map"

#define SVN_PRC_MAP_DATA46	(SVN_BASE_PROCESSED_PROCESS + 85)
#define SVS_PRC_MAP_DATA46	"Sensitivity Map"

#define SVN_PRC_MAP_DATA47	(SVN_BASE_PROCESSED_PROCESS + 86)
#define SVS_PRC_MAP_DATA47	"Sensitivity Map"

#define SVN_PRC_MAP_DATA48	(SVN_BASE_PROCESSED_PROCESS + 87)
#define SVS_PRC_MAP_DATA48	"Sensitivity Map"

#define SVN_PRC_MAP_DATA49	(SVN_BASE_PROCESSED_PROCESS + 88)
#define SVS_PRC_MAP_DATA49	"Sensitivity Map"

#define SVN_PRC_MAP_DATA50	(SVN_BASE_PROCESSED_PROCESS + 89)
#define SVS_PRC_MAP_DATA50	"Sensitivity Map"

#define SVN_PRC_MAP_DATA51	(SVN_BASE_PROCESSED_PROCESS + 90)
#define SVS_PRC_MAP_DATA51	"Sensitivity Map"

#define SVN_PRC_MAP_DATA52	(SVN_BASE_PROCESSED_PROCESS + 91)
#define SVS_PRC_MAP_DATA52	"Sensitivity Map"

#define SVN_PRC_MAP_DATA53	(SVN_BASE_PROCESSED_PROCESS + 92)
#define SVS_PRC_MAP_DATA53	"Sensitivity Map"

#define SVN_PRC_MAP_DATA54	(SVN_BASE_PROCESSED_PROCESS + 93)
#define SVS_PRC_MAP_DATA54	"Sensitivity Map"

#define SVN_PRC_MAP_DATA55	(SVN_BASE_PROCESSED_PROCESS + 94)
#define SVS_PRC_MAP_DATA55	"Sensitivity Map"

#define SVN_PRC_MAP_DATA56	(SVN_BASE_PROCESSED_PROCESS + 95)
#define SVS_PRC_MAP_DATA56	"Sensitivity Map"

#define SVN_PRC_MAP_DATA57	(SVN_BASE_PROCESSED_PROCESS + 96)
#define SVS_PRC_MAP_DATA57	"Sensitivity Map"

#define SVN_PRC_MAP_DATA58	(SVN_BASE_PROCESSED_PROCESS + 97)
#define SVS_PRC_MAP_DATA58	"Sensitivity Map"

#define SVN_PRC_MAP_DATA59	(SVN_BASE_PROCESSED_PROCESS + 98)
#define SVS_PRC_MAP_DATA59	"Sensitivity Map"

#define SVN_PRC_MAP_DATA60	(SVN_BASE_PROCESSED_PROCESS + 99)
#define SVS_PRC_MAP_DATA60	"Sensitivity Map"

#define SVN_PRC_MAP_DATA61	(SVN_BASE_PROCESSED_PROCESS + 100)
#define SVS_PRC_MAP_DATA61	"Sensitivity Map"

#define SVN_PRC_MAP_DATA62	(SVN_BASE_PROCESSED_PROCESS + 101)
#define SVS_PRC_MAP_DATA62	"Sensitivity Map"

#define SVN_PRC_MAP_DATA63	(SVN_BASE_PROCESSED_PROCESS + 102)
#define SVS_PRC_MAP_DATA63	"Sensitivity Map"

#define SVN_PRC_MAP_DATA64	(SVN_BASE_PROCESSED_PROCESS + 103)
#define SVS_PRC_MAP_DATA64	"Sensitivity Map"

#define SVN_PRC_MAP_DATA65	(SVN_BASE_PROCESSED_PROCESS + 104)
#define SVS_PRC_MAP_DATA65	"Sensitivity Map"

#define SVN_PRC_MAP_DATA66	(SVN_BASE_PROCESSED_PROCESS + 105)
#define SVS_PRC_MAP_DATA66	"Sensitivity Map"

#define SVN_PRC_MAP_DATA67	(SVN_BASE_PROCESSED_PROCESS + 106)
#define SVS_PRC_MAP_DATA67	"Sensitivity Map"

#define SVN_PRC_MAP_DATA68	(SVN_BASE_PROCESSED_PROCESS + 107)
#define SVS_PRC_MAP_DATA68	"Sensitivity Map"

#define SVN_PRC_MAP_DATA69	(SVN_BASE_PROCESSED_PROCESS + 108)
#define SVS_PRC_MAP_DATA69	"Sensitivity Map"

#define SVN_PRC_MAP_DATA70	(SVN_BASE_PROCESSED_PROCESS + 109)
#define SVS_PRC_MAP_DATA70	"Sensitivity Map"

#define SVN_PRC_MAP_DATA71	(SVN_BASE_PROCESSED_PROCESS + 110)
#define SVS_PRC_MAP_DATA71	"Sensitivity Map"

#define SVN_PRC_MAP_DATA72	(SVN_BASE_PROCESSED_PROCESS + 111)
#define SVS_PRC_MAP_DATA72	"Sensitivity Map"

#define SVN_PRC_MAP_DATA73	(SVN_BASE_PROCESSED_PROCESS + 112)
#define SVS_PRC_MAP_DATA73	"Sensitivity Map"

#define SVN_PRC_MAP_DATA74	(SVN_BASE_PROCESSED_PROCESS + 113)
#define SVS_PRC_MAP_DATA74	"Sensitivity Map"

#define SVN_PRC_MAP_DATA75	(SVN_BASE_PROCESSED_PROCESS + 114)
#define SVS_PRC_MAP_DATA75	"Sensitivity Map"

#define SVN_PRC_MAP_DATA76	(SVN_BASE_PROCESSED_PROCESS + 115)
#define SVS_PRC_MAP_DATA76	"Sensitivity Map"

#define SVN_PRC_MAP_DATA77	(SVN_BASE_PROCESSED_PROCESS + 116)
#define SVS_PRC_MAP_DATA77	"Sensitivity Map"

#define SVN_PRC_MAP_DATA78	(SVN_BASE_PROCESSED_PROCESS + 117)
#define SVS_PRC_MAP_DATA78	"Sensitivity Map"

#define SVN_PRC_MAP_DATA79	(SVN_BASE_PROCESSED_PROCESS + 118)
#define SVS_PRC_MAP_DATA79	"Sensitivity Map"

#define SVN_PRC_MAP_DATA80	(SVN_BASE_PROCESSED_PROCESS + 119)
#define SVS_PRC_MAP_DATA80	"Sensitivity Map"

#define SVN_PRC_MAP_DATA81	(SVN_BASE_PROCESSED_PROCESS + 120)
#define SVS_PRC_MAP_DATA81	"Sensitivity Map"

#define SVN_PRC_MAP_DATA82	(SVN_BASE_PROCESSED_PROCESS + 121)
#define SVS_PRC_MAP_DATA82	"Sensitivity Map"

#define SVN_PRC_MAP_DATA83	(SVN_BASE_PROCESSED_PROCESS + 122)
#define SVS_PRC_MAP_DATA83	"Sensitivity Map"

#define SVN_PRC_MAP_DATA84	(SVN_BASE_PROCESSED_PROCESS + 123)
#define SVS_PRC_MAP_DATA84	"Sensitivity Map"

#define SVN_PRC_MAP_DATA85	(SVN_BASE_PROCESSED_PROCESS + 124)
#define SVS_PRC_MAP_DATA85	"Sensitivity Map"

#define SVN_PRC_MAP_DATA86	(SVN_BASE_PROCESSED_PROCESS + 125)
#define SVS_PRC_MAP_DATA86	"Sensitivity Map"

#define SVN_PRC_MAP_DATA87	(SVN_BASE_PROCESSED_PROCESS + 126)
#define SVS_PRC_MAP_DATA87	"Sensitivity Map"

#define SVN_PRC_MAP_DATA88	(SVN_BASE_PROCESSED_PROCESS + 127)
#define SVS_PRC_MAP_DATA88	"Sensitivity Map"

#define SVN_PRC_MAP_DATA89	(SVN_BASE_PROCESSED_PROCESS + 128)
#define SVS_PRC_MAP_DATA89	"Sensitivity Map"

#define SVN_PRC_MAP_DATA90	(SVN_BASE_PROCESSED_PROCESS + 129)
#define SVS_PRC_MAP_DATA90	"Sensitivity Map"

#define SVN_PRC_MAP_DATA91	(SVN_BASE_PROCESSED_PROCESS + 130)
#define SVS_PRC_MAP_DATA91	"Sensitivity Map"

#define SVN_PRC_MAP_DATA92	(SVN_BASE_PROCESSED_PROCESS + 131)
#define SVS_PRC_MAP_DATA92	"Sensitivity Map"

#define SVN_PRC_MAP_DATA93	(SVN_BASE_PROCESSED_PROCESS + 132)
#define SVS_PRC_MAP_DATA93	"Sensitivity Map"

#define SVN_PRC_MAP_DATA94	(SVN_BASE_PROCESSED_PROCESS + 133)
#define SVS_PRC_MAP_DATA94	"Sensitivity Map"

#define SVN_PRC_MAP_DATA95	(SVN_BASE_PROCESSED_PROCESS + 134)
#define SVS_PRC_MAP_DATA95	"Sensitivity Map"

#define SVN_PRC_MAP_DATA96	(SVN_BASE_PROCESSED_PROCESS + 135)
#define SVS_PRC_MAP_DATA96	"Sensitivity Map"

#define SVN_PRC_MAP_DATA97	(SVN_BASE_PROCESSED_PROCESS + 136)
#define SVS_PRC_MAP_DATA97	"Sensitivity Map"

#define SVN_PRC_MAP_DATA98	(SVN_BASE_PROCESSED_PROCESS + 137)
#define SVS_PRC_MAP_DATA98	"Sensitivity Map"

#define SVN_PRC_MAP_DATA99	(SVN_BASE_PROCESSED_PROCESS + 138)
#define SVS_PRC_MAP_DATA99	"Sensitivity Map"

#define SVN_PRC_MAP_DATA100	(SVN_BASE_PROCESSED_PROCESS + 139)
#define SVS_PRC_MAP_DATA100	"Sensitivity Map"

#define SVN_PRC_MAP_DATA101	(SVN_BASE_PROCESSED_PROCESS + 140)
#define SVS_PRC_MAP_DATA101	"Sensitivity Map"

#define SVN_PRC_MAP_DATA102	(SVN_BASE_PROCESSED_PROCESS + 141)
#define SVS_PRC_MAP_DATA102	"Sensitivity Map"

#define SVN_PRC_MAP_DATA103	(SVN_BASE_PROCESSED_PROCESS + 142)
#define SVS_PRC_MAP_DATA103	"Sensitivity Map"

#define SVN_PRC_MAP_DATA104	(SVN_BASE_PROCESSED_PROCESS + 143)
#define SVS_PRC_MAP_DATA104	"Sensitivity Map"

#define SVN_PRC_MAP_DATA105	(SVN_BASE_PROCESSED_PROCESS + 144)
#define SVS_PRC_MAP_DATA105	"Sensitivity Map"

#define SVN_PRC_MAP_DATA106	(SVN_BASE_PROCESSED_PROCESS + 145)
#define SVS_PRC_MAP_DATA106	"Sensitivity Map"

#define SVN_PRC_MAP_DATA107	(SVN_BASE_PROCESSED_PROCESS + 146)
#define SVS_PRC_MAP_DATA107	"Sensitivity Map"

#define SVN_PRC_MAP_DATA108	(SVN_BASE_PROCESSED_PROCESS + 147)
#define SVS_PRC_MAP_DATA108	"Sensitivity Map"

#define SVN_PRC_MAP_DATA109	(SVN_BASE_PROCESSED_PROCESS + 148)
#define SVS_PRC_MAP_DATA109	"Sensitivity Map"

#define SVN_PRC_MAP_DATA110	(SVN_BASE_PROCESSED_PROCESS + 149)
#define SVS_PRC_MAP_DATA110	"Sensitivity Map"

#define SVN_PRC_MAP_DATA111	(SVN_BASE_PROCESSED_PROCESS + 150)
#define SVS_PRC_MAP_DATA111	"Sensitivity Map"

#define SVN_PRC_MAP_DATA112	(SVN_BASE_PROCESSED_PROCESS + 151)
#define SVS_PRC_MAP_DATA112	"Sensitivity Map"

#define SVN_PRC_MAP_DATA113	(SVN_BASE_PROCESSED_PROCESS + 152)
#define SVS_PRC_MAP_DATA113	"Sensitivity Map"

#define SVN_PRC_MAP_DATA114	(SVN_BASE_PROCESSED_PROCESS + 153)
#define SVS_PRC_MAP_DATA114	"Sensitivity Map"

#define SVN_PRC_MAP_DATA115	(SVN_BASE_PROCESSED_PROCESS + 154)
#define SVS_PRC_MAP_DATA115	"Sensitivity Map"

#define SVN_PRC_MAP_DATA116	(SVN_BASE_PROCESSED_PROCESS + 155)
#define SVS_PRC_MAP_DATA116	"Sensitivity Map"

#define SVN_PRC_MAP_DATA117	(SVN_BASE_PROCESSED_PROCESS + 156)
#define SVS_PRC_MAP_DATA117	"Sensitivity Map"

#define SVN_PRC_MAP_DATA118	(SVN_BASE_PROCESSED_PROCESS + 157)
#define SVS_PRC_MAP_DATA118	"Sensitivity Map"

#define SVN_PRC_MAP_DATA119	(SVN_BASE_PROCESSED_PROCESS + 158)
#define SVS_PRC_MAP_DATA119	"Sensitivity Map"

#define SVN_PRC_MAP_DATA120	(SVN_BASE_PROCESSED_PROCESS + 159)
#define SVS_PRC_MAP_DATA120	"Sensitivity Map"

#define SVN_PRC_MAP_DATA121	(SVN_BASE_PROCESSED_PROCESS + 160)
#define SVS_PRC_MAP_DATA121	"Sensitivity Map"

#define SVN_PRC_MAP_DATA122	(SVN_BASE_PROCESSED_PROCESS + 161)
#define SVS_PRC_MAP_DATA122	"Sensitivity Map"

#define SVN_PRC_MAP_DATA123	(SVN_BASE_PROCESSED_PROCESS + 162)
#define SVS_PRC_MAP_DATA123	"Sensitivity Map"

#define SVN_PRC_MAP_DATA124	(SVN_BASE_PROCESSED_PROCESS + 163)
#define SVS_PRC_MAP_DATA124	"Sensitivity Map"

#define SVN_PRC_MAP_DATA125	(SVN_BASE_PROCESSED_PROCESS + 164)
#define SVS_PRC_MAP_DATA125	"Sensitivity Map"

#define SVN_PRC_MAP_DATA126	(SVN_BASE_PROCESSED_PROCESS + 165)
#define SVS_PRC_MAP_DATA126	"Sensitivity Map"

#define SVN_PRC_MAP_DATA127	(SVN_BASE_PROCESSED_PROCESS + 166)
#define SVS_PRC_MAP_DATA127	"Sensitivity Map"
/* SPEEDER�̊��x�}�b�v (�P�Q�W�`���l���Ή��j
*/

#define SVN_PRC_NOISE_GAIN_TABLE   (SVN_BASE_PROCESSED_PROCESS + 167)
#define SVT_PRC_NOISE_GAIN_TABLE   VFT_FLT32
#define SVS_PRC_NOISE_GAIN_TABLE   "noise gain ratio among coils"
/*
   �`���l���R�C���ԃm�C�Y�Q�C���␳�p�̃e�[�u��
*/

#define SVN_PRC_NOISE_MATRIX_TABLE   (SVN_BASE_PROCESSED_PROCESS + 168)
#define SVT_PRC_NOISE_MATRIX_TABLE   VFT_FLT32
#define SVS_PRC_NOISE_MATRIX_TABLE   "noise correction between coils"
/*
   Speeder�W�J�p�`���l���R�C���ԃm�C�Y���֌W���}�g���N�X
*/

#define SVN_PRC_NOISE_GAIN_TABLE_FLAG        (SVN_BASE_PROCESSED_PROCESS + 169)
#define SVT_PRC_NOISE_GAIN_TABLE_FLAG        VFT_INT32
#define SVS_PRC_NOISE_GAIN_TABLE_FLAG        "noise gain table flag"
/* 
   �`���l���R�C���ԃm�C�Y�Q�C���␳�t���O

   0 : �␳��������
   1 : �␳�����L��

   #define VFC_NOISE_GAIN_TABLE_OFF   0
   #define VFC_NOISE_GAIN_TABLE_ON    1
 */

#define SVN_PRC_NOISE_MATRIX_TABLE_FLAG      (SVN_BASE_PROCESSED_PROCESS + 170)
#define SVT_PRC_NOISE_MATRIX_TABLE_FLAG      VFT_INT32
#define SVS_PRC_NOISE_MATRIX_TABLE_FLAG      "noise matrix table flag"
/* 
   Speeder�`���l���R�C���ԑ��փ}�g���N�X�␳�t���O

   0 : �␳��������
   1 : �␳�����L��

   #define VFC_NOISE_MATRIX_TABLE_OFF   0
   #define VFC_NOISE_MATRIX_TABLE_ON    1
 */

#define SVN_PRC_SERIES_DESCRIPTION      (SVN_BASE_PROCESSED_PROCESS + 171)
#define SVT_PRC_SERIES_DESCRIPTION      VFT_STRING
#define SVS_PRC_SERIES_DESCRIPTION      "Series Description"
/* 
    Series description

    setting     Iselector
    reference   DICOM
 */

/*****************************************************************************
  ECHO name space
*/
#define SVN_BASE_ECHO_PROCESS		SVN_MIN_ECHO + SVN_OFF_PROCESS

#define SVN_PRC_ECHO_CODE		(SVN_BASE_ECHO_PROCESS + 0)
#define SVT_PRC_ECHO_CODE		VFT_INT32
#define SVS_PRC_ECHO_CODE		"Echo code : reconed or synthed"
/* 	This is to indicate whether this particular echo was acquired and
	reconstructed or it is synthesised by postprocessing
	*/

#define SVN_PRC_ANC_SLC_INDX		(SVN_BASE_ECHO_PROCESS + 1)
#define SVT_PRC_ANC_SLC_INDX		VFT_INT32
#define SVS_PRC_ANC_SLC_INDX		"Ancestral slice index"
/* 	The index of SVD_ST_SLICE branch which had been used to produce the
	current  SVD_ST_ECHO branch
	*/
#define SVN_PRC_ANC_IMG_INDX		(SVN_BASE_ECHO_PROCESS + 2)
#define SVT_PRC_ANC_IMG_INDX		VFT_INT32
#define SVS_PRC_ANC_IMG_INDX		"Ancestral image index"
/* 	The index of SVD_ST_IMAGE branch which had been used to produce the
	current SVD_ST_ECHO branch
	*/

#define SVN_PRC_CEN_TO_CEN_DIST 	(SVN_BASE_ECHO_PROCESS + 3)
#define SVT_PRC_CEN_TO_CEN_DIST		VFT_FLT32
#define SVS_PRC_CEN_TO_CEN_DIST		"Abs. distance between slice centers"
/*	The absolute (unsigned) distance between the centers of corresponding
	voxels of neighboring slices
	*/

#define SVN_PRC_ROW_DIRECTION		(SVN_BASE_ECHO_PROCESS + 4)
/*#define SVT_PRC_ROW_DIRECTION		VFT_PATFLTVEC3*/
#define SVT_PRC_ROW_DIRECTION		VFT_FLTVEC3
#define SVS_PRC_ROW_DIRECTION		"Row direction vector (pat. coords.)"
/*	Unit vector indicating the direction of the rows of the image
	in patient coordinates. Used by display to label the image.
	*/

#define SVN_PRC_COL_DIRECTION		(SVN_BASE_ECHO_PROCESS + 5)
/*#define SVT_PRC_COL_DIRECTION		VFT_PATFLTVEC3*/
#define SVT_PRC_COL_DIRECTION		VFT_FLTVEC3
#define SVS_PRC_COL_DIRECTION		"Col. direction vector (pat. coords.)"
/*	Unit vector indicating the direction of the columns of the image
	in patient coordinates. Used by display to label the image.
	*/

#define SVN_PRC_SOURCE_PATH		(SVN_BASE_ECHO_PROCESS + 6)
#define SVT_PRC_SOURCE_PATH		VFT_PATHELEM
#define SVS_PRC_SOURCE_PATH		"Source data for this calculation"
/*	Varfields path that indicates the source of the data used for this
	calculation (including reconstruction). e.g. for 2d recon this
	will point to the slicegroup in the td side of the tree that
	was used to produce this data. for MIP this will point to the
	processed tree used for input.
	*/

#define SVN_PRC_FOV			(SVN_BASE_ECHO_PROCESS + 7)
#define SVT_PRC_FOV			VFT_FLT32
#define SVS_PRC_FOV			"Field of view"
/*	A 2 element array defining the field of view of all images beneath
	this node. The order of the elements is minor axis (column direction),
	major axis (row direction). The unit is metres.
	This field is written by reconstruction, reconstruction derives the
	information from the SVN_PLN_FOV field stored at the protocol
	level.
	*/

/* Added by K.Kokubun	Jan-26-1994 */
#define SVN_PRC_ECHO_NO			(SVN_BASE_ECHO_PROCESS + 8)
#define SVT_PRC_ECHO_NO			VFT_INT32
#define SVS_PRC_ECHO_NO			"Echo number"
/*	Echo number
	*/

#define SVN_PRC_SLAB_NO			(SVN_BASE_ECHO_PROCESS + 9)
#define SVT_PRC_SLAB_NO			VFT_INT32
#define SVS_PRC_SLAB_NO			"Slab No"
/*	�X���u�ԍ��B
	Echo Node �ɓo�^����B
	*/

#define SVN_PRC_IMAGE_FLIP_FLAG		(SVN_BASE_ECHO_PROCESS + 10)
#define SVT_PRC_IMAGE_FLIP_FLAG		VFT_BOOL
#define SVS_PRC_IMAGE_FLIP_FLAG		"image flip flag for AX"
/*	�X���C�X�f�ʂ��`�wlike�ł��摜�ώ@������ViewFromFeet
	�̏ꍇ�ɁA�摜�̍��E�𔽓]���ĕ\�����邽�߂̃t���O�B
	FALSE�F���]���Ȃ��BTRUE�F���]����B
	�o�[�W�����̌Â��摜�ł��̃t�B�[���h���Ȃ����͔��]����
	���B
	*/

#define SVN_PRC_MPG_APPLY_DIRECT	(SVN_BASE_ECHO_PROCESS + 11)
#define SVT_PRC_MPG_APPLY_DIRECT	VFT_INT32
#define SVS_PRC_MPG_APPLY_DIRECT	"The direction which MPG pulse is applied along."
/*	  diffusion�摜�ɂ����āAReference�y��Phase�ARead�ASlice�̂����A
	  MPG���ǂ̕����Ɉ�����ē���ꂽ�摜�ł��邩�����������B������
	  �ȉ��Ɏ������l�Ŏw�肳���B
	  VFC_IVIM_ACQUIRE_REF    1
	  VFC_IVIM_ACQUIRE_PE     2
	  VFC_IVIM_ACQUIRE_RO     4
	  VFC_IVIM_ACQUIRE_SS     8
	  
	  �ݒ��:	recon
	  
	  This field show the direction of the MPG pulse at
	  the diffusion weighted image.
	  There are four directions which are reference(=1), phase(=2),
	  read(=4), and slice(=8). 
	  */

#define SVN_PRC_DYNAMIC_NO		(SVN_BASE_ECHO_PROCESS + 12)	
#define SVT_PRC_DYNAMIC_NO		VFT_INT32
#define SVS_PRC_DYNAMIC_NO		"Dynamic No"
/*	�����̔ԍ��B
	Echo Node �ɓo�^����B
	*/

#define SVN_PRC_PERFUSION_MAP_TYPE   (SVN_BASE_ECHO_PROCESS + 13)
#define SVT_PRC_PERFUSION_MAP_TYPE   VFT_INT32
#define SVS_PRC_PERFUSION_MAP_TYPE   "The detail of Image kind"
/*
    Perfusion MAP�����̎�ʂ��`����B
*/

/*****************************************************************************
  POSITION name space
*/
#define SVN_BASE_POSITION_PROCESS	SVN_MIN_POSITION+SVN_OFF_PROCESS

#define SVN_PRC_ANGULAR_POSITION  	(SVN_BASE_POSITION_PROCESS + 0)
#define SVT_PRC_ANGULAR_POSITION 	VFT_FLT32
#define SVS_PRC_ANGULAR_POSITION 	"Angular position of slice"
/* 	Angular position (in degrees, RHP) of a slice for radial sliced or
	projected images
        */

#define SVN_PRC_SLICE_POSITION 		(SVN_BASE_POSITION_PROCESS + 1)
#define SVT_PRC_SLICE_POSITION 		VFT_FLT32
#define SVS_PRC_SLICE_POSITION  	"Slice position relative to the center"
/*	The distance (signed) from the current slice to the central slice
	(which center coincides with the SVN_PLN_OFFSET_VECTOR)
	*/

#define SVN_PRC_IMAGE_ID		(SVN_BASE_POSITION_PROCESS + 2)
#define SVT_PRC_IMAGE_ID		VFT_INT32
#define SVS_PRC_IMAGE_ID		"Image ID on image selector"
/*	�摜�ԍ��B
	Position Node �ɓo�^����B
	group ���� unique �Ȓl���Ƃ�B
	group ���ł̃\�[�g�ɗp����B�ȗ��s�B
	*/

#define SVN_PRC_AFTER_INJECT_TIME	(SVN_BASE_POSITION_PROCESS + 3)
#define SVT_PRC_AFTER_INJECT_TIME      	VFT_INT32
#define SVS_PRC_AFTER_INJECT_TIME      	"After Inject time"
/*      ���e�ܒ�������̌o�ߎ��� (�P�ʂ�ms�j
	Dynamic���ő��e�ܒ����O�̎����̏ꍇ�̓[��������B
        �i�[���̏ꍇ�͔�\���ɂ���B�j
	*/

#define SVN_PRC_RO_DEPHASE_AREA_RATIO	(SVN_BASE_POSITION_PROCESS + 4)
#define SVT_PRC_RO_DEPHASE_AREA_RATIO   	VFT_STRING
#define SVS_PRC_RO_DEPHASE_AREA_RATIO   	"Write dephase area ratio"
/*	Flow-Spoied FBI�̖{�X�L�������邢��FlowPrep�X�L�����ŁA����
	�摜�����W�����ۂ́ARO���邢��PE�����Ɉ�����ꂽDephasing Pulse
	�̋��x��\��������B�p���X���x�́A�Ǐo���p�̌X�Ύ���̗���肩��G�R�[
	���S�܂ł̖ʐς��P�Ƃ������́ADephase�p���X�̖ʐϔ�ŕ\���B
	�������FD�̌�ɋ��x�����\���������́i��FFD+15%,FD-10% �Ȃ�)
*/

#define SVN_PRC_SLICE_NUMBER    (SVN_BASE_POSITION_PROCESS + 5)
#define SVT_PRC_SLICE_NUMBER    VFT_INT32
#define SVS_PRC_SLICE_NUMBER    "Slice number"
/*	�X���C�X�ԍ�
        �X���u�Ԃł̃V�[�P���V�����Ȕԍ����ݒ肳���B
*/

#define SVN_PRC_SLICE_NUMBER_FOR_DICOM	(SVN_BASE_POSITION_PROCESS + 6)
#define SVT_PRC_SLICE_NUMBER_FOR_DICOM	VFT_INT32
#define SVS_PRC_SLICE_NUMBER_FOR_DICOM	"Slice number for DICOM"
/*	DICOM�̃X���C�X�ԍ�(�摜�ԍ�)
	�X���u�Ԃł̃V�[�P���V�����Ȕԍ����ݒ肳���B
        �}���`�G�R�[�B���ł͂Q�G�R�[�ڂ���
        �_�C�i�~�b�N�B���ł͂Q�����ڂ���I�t�Z�b�g���t������B
        ����ɂ�蓯��V���[�Y���Ń��j�[�N�Ȕԍ��ƂȂ�B
*/

/*****************************************************************************
  IMCALC name space
*/



#endif /* tami_vfStudy_vfs_process_H__*/
