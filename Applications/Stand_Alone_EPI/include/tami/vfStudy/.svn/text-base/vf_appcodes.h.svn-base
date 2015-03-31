/*********************************************************************
 *
 *  (c) Copyright 1997 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *	This include file defines varfield codes, i.e. sets of values
 *	for a particular varfield, that are of universal interest.
 *
 *********************************************************************/
/* Revision History:
 * Aug.11.97	古舘直幸	Modified
 * End of History */

#ifndef tami_vfStudy_vf_appcodes_H__
#define tami_vfStudy_vf_appcodes_H__

/*****************************************************************************
  GENERAL name subspace
  */

/* VarField:	SVN_GEN_DATA_FORM
 */
#define VFC_FORM_MAGNITUDE	0
#define VFC_FORM_PHASE		1
#define VFC_FORM_REAL_ONLY	2
#define VFC_FORM_IMGN_ONLY	3
#define VFC_FORM_COMPLEX	4
#define VFC_FORM_POLAR		5

/* VarField:	SVN_GEN_DATA_DESC
 */
#define VFC_DATA_NONE	0	/* No meaning is attached to this dimension */
#define VFC_DATA_RO	1	/* Readout */
#define VFC_DATA_PE	2	/* Phase Encode */
#define VFC_DATA_SE	3	/* Slice Encode */

/* VarField:	SVN_PLN_AUTO_VOICE
		SVN_EXE_AUTO_VOICE
 */
#define VFC_AUTO_VOICE_NONE	0	/* オートボイスOFF */
#define VFC_AUTO_VOICE_BREATH1	1	/* 息止めオートボイス */
#define VFC_AUTO_VOICE_BREATH2	2	/* 息止めオートボイス */
#define VFC_AUTO_VOICE_BREATH3	3	/* 息止めオートボイス */
#define VFC_AUTO_VOICE_MOTION	4	/* 体動防止オートボイス */


/*****************************************************************************
  NMRTOOLS name subspace
  */



/*****************************************************************************
  SCHEDULE name subspace
  */

/* VarField:	SVN_SCH_GENDER
 */
#define VFC_GENDER_FEMALE		0x00	/* 女性      000 */
#define VFC_GENDER_MALE			0x01	/* 男性      001 */
#define VFC_GENDER_FEMALE_INFANT	0x02	/* 女性|幼児 010 */
#define VFC_GENDER_MALE_INFANT		0x03	/* 男性|幼児 011 */
#define VFC_GENDER_FEMALE_PREGNANT	0x04	/* 女性|妊婦 100 */

#define VFC_GENDER_INFANT		0x02	/* 幼児      010 */
#define VFC_GENDER_PREGNANT		0x04	/* 妊婦      100 */

#define VFC_GENDER_OTHERS		0x1000

/* VarField:	SVN_SCH_LANGUAGE
 */
#define VFC_LANG_ENGLISH     0
#define VFC_LANG_JAPANESE    1


/*****************************************************************************
  PLAN name subspace
  */

/* VarField:	SVN_PLN_IMAGING_MODE
 */
#define VFC_IMAGING_MODE_2D		0	/* Data acquired in 2D mode */
#define VFC_IMAGING_MODE_3D		1	/* Data acquired in 3D mode */

/* VarField:	SVN_PLN_APP_OPTION
 */
#define VFC_APP_OPTION_TOF		1	/* Time of Flight Angio */
#define VFC_APP_OPTION_PC		2	/* Phase Contrast Angio */
#define VFC_APP_OPTION_AC		3	/* Amplitude Contrast Angio */
#define VFC_APP_OPTION_SORS		4	/* SORS */
#define VFC_APP_OPTION_WFOP		5	/* Water-Fat Out of Phase */
#define VFC_APP_OPTION_LCS		6	/* Low Chemical Shift */
#define VFC_APP_OPTION_FINE_SLICE	7	/* Fine Slice */
#define VFC_APP_OPTION_ISCE		8	/* ISCE */
#define VFC_APP_OPTION_PASTA		9	/* PASTA */
#define VFC_APP_OPTION_FLUORO		10	/* Fluoro */
#define VFC_APP_OPTION_MBW		11	/* Matched Band Width */
#define VFC_APP_OPTION_DIET		12	/* DIET */
#define VFC_APP_OPTION_MRSCSI_3D	46	/* 3DMRS */

#define VFC_APP_OPTION_ISCE1		101	/* ISCE option */
#define VFC_APP_OPTION_ISCE2		102	/* ISCE option */
#define VFC_APP_OPTION_ISCE3		103	/* ISCE option */

#define VFC_APP_OPTION_FASE_SS		201	/* FASE Single Shot */
#define VFC_APP_OPTION_FASE_TMP		202	/* FASE with template */

/* VarField:	SVN_PLN_ANGIO_OPTION
 */
#define VFC_ANGIO_OPTION_TOF		1	/* Time of Flight Angio */
#define VFC_ANGIO_OPTION_PC		2	/* Phase Contrast Angio */
#define VFC_ANGIO_OPTION_AC		3	/* Amplitude Contrast Angio */
/* Added by Nao.Furudate */
#define VFC_ANGIO_OPTION_SORS		4	/* SORS */

/* VarField:	SVN_PLN_SE_OPTION
 */
#define VFC_SEOPT_BIT_WFOP		1	/* Water-Fat Out of Phase */
#define VFC_SEOPT_BIT_EXCITATION_ENCODE	(1<<1)	/* Excitation Encoding */
/* Added by Nao.Furudate */
#define VFC_SEOPT_BIT_LCS		(1<<2)	/* Low Chemical Shift */
#define VFC_SEOPT_BIT_FINE_SLICE	(1<<3)	/* Fine Slice */

/* VarField:	SVN_PLN_MOTION_SUPPRESSION
 */
/* Note: first bit was left out by mistake. */
#define VFC_MSUP_BIT_FLOW_COMP		(1<<1)	/* Flow Compensation */
#define VFC_MSUP_BIT_BREATH_HOLD	(1<<2)	/* Breath hold */

/* VarField:	SVN_PLN_SLICE_ORIENTATION
 */
#define VFC_ORI_TRANSAXIAL		0	/* Transaxial */
#define VFC_ORI_SAGITTAL		1	/* Sagittal */
#define VFC_ORI_CORONAL			2	/* Coronal */
#define VFC_ORI_OTHER			3	/* Other */
#define VFC_ORI_AXI_OBLIQUE		4	/* Transaxial oblique */
#define VFC_ORI_SAG_OBLIQUE		5	/* Sagittal oblique */
#define VFC_ORI_COR_OBLIQUE		6	/* Coronal oblique */

/* VarField:	SVN_PLN_PHASE_DIRECTION
 */
#define VFC_PE_DIR_HEAD_FEET		0	/* PE dir from head  to feet */
#define VFC_PE_DIR_RIGHT_LEFT		1	/* PE dir from right to left */
#define VFC_PE_DIR_FRONT_BACK		2	/* PE dir from front to back */
#define VFC_PE_DIR_OTHER		3	/* PE dir other */
#define VFC_PE_DIR_AXI_PLANE		4	/* PE dir axial plane */
#define VFC_PE_DIR_SAG_PLANE		5	/* PE dir sagittal plane */
#define VFC_PE_DIR_COR_PLANE		6	/* PE dir coronal plane */

/* VarField:	SVN_PLN_WRAPAROUND_REDUCT
 */
#define VFC_WRAPAROUND_REDUCT_BIT_PE	1
#define VFC_WRAPAROUND_REDUCT_BIT_RO	(1<<1)
#define VFC_WRAPAROUND_REDUCT_BIT_SE	(1<<2)

/* VarField:	SVN_PLN_GATING_METHOD
 */
#define VFC_GAT_METHOD_BIT_ECG		1	/* ECG */
#define VFC_GAT_METHOD_BIT_PERIPHERAL	(1<<1)	/* Peripheral */
#define VFC_GAT_METHOD_BIT_RESPIRATORY	(1<<2)	/* Respiratory */
#define VFC_GAT_METHOD_BIT_RESP_EDGE	(1<<2)	/* Respiratory Edge Mode */
#define VFC_GAT_METHOD_BIT_RESP_LEVEL	(1<<3)	/* Respiratory Level Mode */
#define VFC_GAT_METHOD_BIT_RESP_ECG	(1<<4)	/* Respiratory + ECG */
#define VFC_GAT_METHOD_BIT_RESP_PERP	(1<<5)	/* Respiratory + Peripheral */

/* VarField:	SVN_PLN_PLN_GATE_PARAM
 */
#define VFC_GAT_MODE_STANDARD		0	/* Multi Slice Single Phase */
#define VFC_GAT_MODE_S_SLICE_M_PHASE	1	/* Single Slice Multi Phase */
#define VFC_GAT_MODE_SEQUENCIAL_M_SLICE	2	/* Sequential Multi Slice */
#define VFC_GAT_MODE_SEQUENTIAL_M_SLICE	2	/* Sequential Multi Slice */
#define VFC_GAT_MODE_SEGMENTATION	3	/* Segmented SE and FE */
#define VFC_GAT_MODE_ECG_PREP		4	/* ECG Prep */
#define VFC_GAT_MODE_SEQUENTIAL_PACKING 5 /* Sequential Packing */
#define VFC_GAT_MODE_RETRO 	 	6 /* Retro Mode  */
#define VFC_GAT_MODE_FLOW_PREP 	 	7 /* Flow Prep */
#define VFC_GAT_MODE_BBTI_PREP 	 	8 /* BBTI Prep */
#define VFC_GAT_MODE_TI_PREP 	 	9 /* TI Prep */

/* VarField:	SVN_PLN_GATING_ROTAT
 */
#define VFC_GAT_ROTAT_FORWARD		0	/* Gating cycles acquired 1-N */
#define VFC_GAT_ROTAT_REVERSE		1	/* Gating cycles acquired N-1 */

/* VarField:	SNV_PLN_RAW_AVERAG_MODE
 */
#define VFC_RAW_AVERAG_MODE_NORMAL	1
#define VFC_RAW_AVERAG_MODE_AFI		2
#define VFC_RAW_AVERAG_MODE_SEL		3

/* VarField:	SVN_PLN_AUTOVOICE
 */

/* VarField:	SVN_PLN_POST_ACQ_MODE
 */
#define VFC_MODE_NONE	                 0    /* No post ACQ actions */
#define VFC_MODE_AUTO_RECON       (1L << 0)   /* Do Automatic Reconstruction */
#define VFC_MODE_AUTO_VIEW        (1L << 1)   /* Do Autoview */
#define VFC_MODE_AUTO_VIEW_CS     (1L << 2)   /* Display Center Slice via Auto View */
#define VFC_MODE_RTL              (1L << 3)   /* Do Real Time Locator */
#define VFC_MODE_RTL_EXTENDED     (1L << 4)   /* Do Extended Locator */
#define VFC_MODE_EVOLVING_IMAGE   (1L << 5)   /* Do Evolving Image */
#define VFC_MODE_CONCURRENT_RECON (1L << 6)   /* Do Concurrent Recon */
#define VFC_MODE_DYNAMIC_RECON	  (1L << 7)   /* Not Dynamic Reconstruction */

/* VarField:	SVN_PLN_VIEWING_DIR
 */
#define VFC_VIEW_FROM_HEAD	0	/* View images from head as default */
#define VFC_VIEW_FROM_FEET	1 	/* 	from feet as default */

/* VarField:	SVN_PLN_PATIENT_ORIENT -- These codes are used in VFT_PATORI
 */
/* end_in codes: */
#define VFC_PATORI_HEAD_FIRST	0	/* Patient entered magnet head first */
#define VFC_PATORI_FEET_FIRST	1	/* Patient entered magnet feet first */
/* side_up codes: */
#define VFC_PATORI_PRONE	0	/* Patient has back side up in magnet */
#define VFC_PATORI_SUPINE	1	/* Patient has front side up in magnet */
#define VFC_PATORI_LEFT_UP	2	/* Patient has left side up in magnet */
#define VFC_PATORI_RIGHT_UP	3	/* Patient has right side up in magnet */


/* VarField:	SVN_PLN_APP_CODE
 */
#define VFC_APP_CODE_SPIN_ECHO		 0 /* Spin Echo Application */
#define VFC_APP_CODE_FAST_SPIN_ECHO	 1 /* Fast Field Echo Application */
#define VFC_APP_CODE_FIELD_ECHO		 2 /* Field Echo Application */
#define VFC_APP_CODE_INVERSION_RECOVERY	 4 /* Inversion Recovery Application */
#define VFC_APP_CODE_FAST_FIELD_ECHO	 5 /* Fast Spin Echo Application */
#define VFC_APP_CODE_FAST_IR		 6 /* Fast Spin Echo + Inversoin Pulse */
#define VFC_APP_CODE_ANGIO		 7 /* Angiography Application */
#define VFC_APP_CODE_PHASE_SHIFT	 8 /* Phase-Shift MR-Angiography */
#define VFC_APP_CODE_QUAD_SE		10 /* Quad SE Application */
#define VFC_APP_CODE_QUAD_SPIN_ECHO	10 /* TAMI's definition */
#define VFC_APP_CODE_MULTI_EXCIT_SE	10 /* Multiple Excitation SE Application */
#define VFC_APP_CODE_QUAD_FE		11 /* Quad FE Application */
#define VFC_APP_CODE_QUAD_FIELD_ECHO	11 /* TAMI's definition */
#define VFC_APP_CODE_MULTI_EXCIT_FE	11 /* Multiple Excitation FE Application */
#define VFC_APP_CODE_FASE		12 /* Fast Asymmetric Spin Echo Application */
#define VFC_APP_CODE_HYEPI		13 /* Hybrid EPI */
#define VFC_APP_CODE_SSEPI_SE		15 /* Shingle Shot EPI (SE) */
#define VFC_APP_CODE_SSEPI_FE		16 /* Shingle Shot EPI (FE) */
#define VFC_APP_CODE_MSEPI_SE		17 /* Multi Shot EPI (SE) */
#define VFC_APP_CODE_MSEPI_FE		18 /* Multi Shot EPI (FE) */
#define VFC_APP_CODE_MRS		19 /* MR Spectroscopy */
#define VFC_APP_CODE_MRSCSI		20 /* MR Spectroscopy */
#define VFC_APP_CODE_FFE_EPI	21 /* Segmented FE_EPI */

#define VFC_APP_CODE_PREP	       100 /* Visual Prep */
#define VFC_APP_CODE_PROBE	       110 /* Probing */

/* VarField:	SVN_PLN_SPATIAL_PRESAT
 */
#define VFC_SPATPSAT_BIT_SLICE		1
#define VFC_SPATPSAT_BIT_PE		(1<<1)
#define VFC_SPATPSAT_BIT_RO		(1<<2)
#define VFC_SPATPSAT_BIT_COVERAGE	(1<<3)
#define VFC_SPATPSAT_BIT_UPSTREAM	(1<<4)
#define VFC_SPATPSAT_BIT_DOWNSTREAM	(1<<5)
#define VFC_SPATPSAT_BIT_SORS		(1<<6)

/* VarField:	SVN_PLN_PRESAT_TYPE
 */
#define VFC_PRESAT_TYPE_SLICE		1
#define VFC_PRESAT_TYPE_PE		2
#define VFC_PRESAT_TYPE_RO		3
#define VFC_PRESAT_TYPE_COVERAGE	4
#define VFC_PRESAT_TYPE_UPSTREAM	5
#define VFC_PRESAT_TYPE_DOWNSTREAM	6
#define VFC_PRESAT_TYPE_FREEHAND	7
#define VFC_PRESAT_TYPE_SORS		8
/* 1997.08.06 */
#define VFC_PRESAT_TYPE_FH_TAG		10
#define VFC_PRESAT_TYPE_HORIZON_TAG	11
#define VFC_PRESAT_TYPE_VERTICAL_TAG	12
/* 1999.04.20 */
#define VFC_PRESAT_TYPE_MRS_SELECTED	13
/* 1999.07.08 */
#define VFC_PRESAT_TYPE_TAG_IR		14
#define VFC_PRESAT_TYPE_CONTROL_IR	15
/* 2001.09.27 */
#define VFC_PRESAT_TYPE_TEC		16
/* 2001.11.30 */
#define VFC_PRESAT_TYPE_FREE_TAG_IR	17
#define VFC_PRESAT_TYPE_FREE_CONTROL_IR	18
#define VFC_PRESAT_TYPE_FREE_TEC	19
/* 2002.04.30 */
#define VFC_PRESAT_TYPE_PROBE		20
/* 2003.12.02 */
#define VFC_PRESAT_TYPE_TSLIT		21

/* VarField:	SVN_PLN_VG_TYPE
 */
#define VFC_VG_TYPE_PARALLEL		1
#define VFC_VG_TYPE_RADIAL		2
#define VFC_VG_TYPE_DOUBLE_RADIAL	3

/* VarField:	SVN_PLN_TYPE_DELAY_BETWEEN_REPS
 */
#define VFC_TYPE_DELAY_BETWEEN_REPS_SPECIFY	1
#define VFC_TYPE_DELAY_BETWEEN_REPS_MANUAL	2

/* VarField:	SVN_PLN_SCAN_ANATOMY
 */
#define VFC_ANATOMY_ABDOMEN 	1
#define VFC_ANATOMY_CHEST 	2
#define VFC_ANATOMY_PELVIS 	3
#define VFC_ANATOMY_TL_SPINE 	4
#define VFC_ANATOMY_SHOULDER	5
#define VFC_ANATOMY_HEAD	6
#define VFC_ANATOMY_C_SPINE	7
#define VFC_ANATOMY_EXTRA	8
#define VFC_ANATOMY_EXTREMITY	8
#define VFC_ANATOMY_HAND	9
#define VFC_ANATOMY_ANKLE	10

/* VarField:	SVN_PLN_IS_ACQ_FORWARD
 */
#define VFC_ACQ_ORDER_BACKWARD  0
#define VFC_ACQ_ORDER_FORWARD   1
#define VFC_ACQ_ORDER_CENTRIC   2

/* VarField:	SVN_PLN_INTERL_FLAG
 */
#define VFC_INTRL_NONE          0
#define VFC_INTRL_ALTERNATE     1
#define VFC_INTRL_COVERAGE	2

/* VarField:	SVN_PLN_AFI_MODE
 */
#define VFC_AFI_PE_HEAD		1
#define VFC_AFI_PE_TAIL		2
#define VFC_AFI_RO_HEAD		4
#define VFC_AFI_RO_TAIL 	8
#define VFC_AFI_SE_HEAD 	16
#define VFC_AFI_SE_TAIL 	32

/* VarField:	SVN_PLN_AFI_DIRECTION
 */
#define VFC_AFI_PE		0x03
#define VFC_AFI_RO		0x0C
#define VFC_AFI_SE		0x30

/* VarField:	SVN_PLN_IR_PULSE
 */
#define VFC_IR_PULSE_NONE	0
/*
 * Frequency Multiplexed Method (TAMI方式)
 */
#define VFC_IR_PULSE_FMM	1
/*
 * Time Multiplexed Method
 */
#define VFC_IR_PULSE_STIR	2
#define VFC_IR_PULSE_FLAIR	3
#define VFC_IR_PULSE_IR2	4

/*  VarField:	SVN_PLN_INV_PULSE_TYPE
 */
#define VFC_INV_PULSE_TYPE_SINC		0
#define VFC_INV_PULSE_TYPE_ADIABATIC	1
#define VFC_INV_PULSE_TYPE_ASTAR	2
#define VFC_INV_PULSE_TYPE_NOSELECT	3
#define VFC_INV_PULSE_TYPE_ASTAR2	4
#define VFC_INV_PULSE_TYPE_DOUBLEIRSTAR	5
#define VFC_INV_PULSE_TYPE_SSATURATION	6

/* VarField: 	SVN_PLN_NUM_EXCITATIONS		* Number of Excitations in
 *						* the multiple excitation
 *						* sequence.  At the time
 *						* this was added, quad
 *						* dissapears as an independent
 *						* sequence
 */
#define VFC_DUO_SCAN_NUMBER_EXCIT	2
#define VFC_TRI_SCAN_NUMBER_EXCIT	3
#define VFC_QUAD_SCAN_NUMBER_EXCIT	4
#define VFC_APP_CODE_MULTI_EXCIT_FFE	8 /* Multiple Excitation Fast FE */

/* VarField:	SVN_PLN_VENC_DIR
   */
#define VFC_VENC_DIR_HEAD_FEET		0
#define VFC_VENC_DIR_ALL		1


/* VarField:	SVN_PLN_SAR_ANATOMY
   */
#define VFC_SAR_ANATOMY_HEAD		1
#define VFC_SAR_ANATOMY_ABDOMEN	        2
#define VFC_SAR_ANATOMY_CHEST           3
#define VFC_SAR_ANATOMY_CSPINE		4
#define VFC_SAR_ANATOMY_TLSPINE		5
#define VFC_SAR_ANATOMY_PELVIS		6
#define VFC_SAR_ANATOMY_LOWER_EXT	7
#define VFC_SAR_ANATOMY_UPPER_EXT	8
#define VFC_SAR_ANATOMY_MISC		9

/* VarField:	SVN_PLN_STATUS
   */
#define VFC_PLN_STATUS_PARENT		(1<<0)
#define VFC_PLN_STATUS_PLAN2		(1<<1)

#define VFC_PLN_STATUS_PROEDIT		(1<<8)
#define VFC_PLN_STATUS_GLOC		(1<<9)
#define VFC_PLN_STATUS_GATING		(1<<10)
#define VFC_PLN_STATUS_DYNAMIC		(1<<11)
#define VFC_PLN_STATUS_VGCOPY		(1<<12)
#define VFC_PLN_STATUS_PROMUI		(1<<13)
#define VFC_PLN_STATUS_SPCPLAN		(1<<14)
#define VFC_PLN_STATUS_MOVINGBED	(1<<15)
#define VFC_PLN_STATUS_DIFFUSION	(1<<16)
#define VFC_PLN_STATUS_PERFUSION	(1<<17)
#define VFC_PLN_STATUS_ASL		(1<<18)
#define VFC_PLN_STATUS_PROBE		(1<<19)

#define VFC_PLN_STATUS_PROMON		(1<<24)
#define VFC_PLN_STATUS_PRODEBUG		(1<<25)

/* VarField:	SVN_PLN_ISCE_MODE
   */
#define VFC_ISCE_MODE_NORMAL		0
#define VFC_ISCE_MODE_1_2		1
#define VFC_ISCE_MODE_1_3		2


/* VarField:	SVN_PLN_HOLD
  |<---  Pre Scan -------------->|<-Prep---->|<--- Scan ---------------->
  _______________________________________________________________________
  | Coil Tune | APC | CFA | RGN ∥Prep Scan ∥  Main Scan  |  SUB ......|
  ↑~~~~~~~~~~↑~~~~↑~~~~↑~~~~~↑~~~~~~~~~~↑~~~~~~~~~~~~~↑~~~~~
  PRE_SCAN    APC   CFA   RGN    MAIN_SCAN   COVERAGE       COVERAGE
                                 COVERAGE    (PREP)
				 
定義
	  VFC_HOLD_PRE_SCAN	: 撮影処理の最初で止まる。（通常の撮影処理）
				  これがオフになっている時は自動撮影モード。
	  VFC_HOLD_MAIN_SCAN	: プレスキャン終了後、本スキャンの前で一時停止。
				”息止め撮影”用。
	  VFC_HOLD_COVERAGE	: カバレージごとに止まる。
	  VFC_HOLD_PREP		: プレップスキャンと本スキャンの間での停止を
	  			  意味する。通知用の情報であり制御には使用しない。
	  
	  VFC_HOLD_APC	:  ＡＰＣ直前で一時停止。”息止めＡＰＣ”用。
	  VFC_HOLD_CFA	:  ＣＦＡ直前で一時停止。
	  VFC_HOLD_RGN	:  ＲＧＮ直前で一時停止。
	  VFC_HOLD_CDS	:  ＣＤＳ直前で一時停止。

	注）
	  VFC_HOLD_MAIN_SCAN か VFC_HOLD_COVERAGE のどちらかが指定されて
	いた場合、本スキャンの前で一時停止する。
	
   */
#define VFC_HOLD_PRE_SCAN	(1L<<0)		/* 制御＆通知 */
#define VFC_HOLD_MAIN_SCAN	(1L<<1)		/* 制御＆通知 */
#define VFC_HOLD_COVERAGE	(1L<<2)		/* 制御＆通知 */
#define VFC_HOLD_PREP		(1L<<3)		/* 通知 */
#define VFC_HOLD_MOVE_BED	(1L<<4)		/* 制御＆通知 */

#define VFC_HOLD_COUCH		(1L<<10)	/* PQMのみで使用 */

#define VFC_HOLD_APC		(1L<<16)	/* 制御＆通知 */
#define VFC_HOLD_CFA		(1L<<17)	/* 制御＆通知 */
#define VFC_HOLD_RGN		(1L<<18)	/* 制御＆通知 */
#define VFC_HOLD_CDS		(1L<<19)	/* 制御＆通知(V9.50) */


/* VarField:	SVN_PLN_GRID_TAG
 */
#define VFC_GRID_TAG_HORIZON		(1<<0)
#define VFC_GRID_TAG_VERTICAL		(1<<1)

/* VarField:	SVN_PLN_IMAGE_PATTERN
 */
#define VFC_IVIM_ACQ_REF	(1<<0)
#define VFC_IVIM_ACQ_PE		(1<<1)
#define VFC_IVIM_ACQ_RO		(1<<2)
#define VFC_IVIM_ACQ_SS		(1<<3)
#define VFC_IVIM_ACQ_PEpRO	(1<<4)
#define VFC_IVIM_ACQ_PEnRO	(1<<5)
#define VFC_IVIM_ACQ_ROpSS	(1<<6)
#define VFC_IVIM_ACQ_ROnSS	(1<<7)
#define VFC_IVIM_ACQ_SSpPE	(1<<8)
#define VFC_IVIM_ACQ_SSnPE	(1<<9)

#define VFC_IVIM_ACQ3(n1,n2,n3)	(VFC_IVIM_ACQ_##n1|\
				 VFC_IVIM_ACQ_##n2|\
				 VFC_IVIM_ACQ_##n3)
#define VFC_IVIM_IMAGE2(n1,n2)	(VFC_IVIM_IMAGE_##n1|\
				 VFC_IVIM_IMAGE_##n2)

#define VFC_IVIM_IMAGE_REF		VFC_IVIM_ACQ_REF
#define VFC_IVIM_IMAGE_PE		VFC_IVIM_ACQ_PE	
#define VFC_IVIM_IMAGE_RO		VFC_IVIM_ACQ_RO
#define VFC_IVIM_IMAGE_SS		VFC_IVIM_ACQ_SS	
#define VFC_IVIM_IMAGE_PE_RO		VFC_IVIM_ACQ_PEpRO
#define VFC_IVIM_IMAGE_RO_SS		VFC_IVIM_ACQ_ROpSS
#define VFC_IVIM_IMAGE_SS_PE		VFC_IVIM_ACQ_SSpPE
#define VFC_IVIM_IMAGE_3_AXIS		VFC_IVIM_ACQ3(PE,RO,SS)
#define VFC_IVIM_IMAGE_3_AXIS_PLUS	VFC_IVIM_ACQ3(PEpRO,ROpSS,SSpPE)
#define VFC_IVIM_IMAGE_3_AXIS_MINUS	VFC_IVIM_ACQ3(PEnRO,ROnSS,SSnPE)
#define VFC_IVIM_IMAGE_TENSOR_PLUS	VFC_IVIM_IMAGE2(3_AXIS,3_AXIS_PLUS)
#define VFC_IVIM_IMAGE_TENSOR_MINUS	VFC_IVIM_IMAGE2(3_AXIS,3_AXIS_MINUS)
#define VFC_IVIM_IMAGE_TENSOR		VFC_IVIM_IMAGE2(3_AXIS_PLUS,3_AXIS_MINUS)

/* VarField:	SVN_PLN_MRS_BASE_INFO
 */
#define VFC_MRS_BASE_LTL		0
#define VFC_MRS_BASE_LTR		1
#define VFC_MRS_BASE_LBL		2
#define VFC_MRS_BASE_LBR		3
#define VFC_MRS_BASE_RTL		4
#define VFC_MRS_BASE_RTR		5
#define VFC_MRS_BASE_RBL		6
#define VFC_MRS_BASE_RBR		7

/* VarField:	SVN_PLN_SEGMENTATION_TYPE
 */
#define VFC_SEGMENT_TYPE_MOSAIC			0
#define VFC_SEGMENT_TYPE_INTERLEAVE		1
#define VFC_SEGMENT_TYPE_SLICE_CENTRIC		2
#define VFC_SEGMENT_TYPE_SEQUENTIAL		3
#define VFC_SEGMENT_TYPE_SWIRL			4
#define VFC_SEGMENT_TYPE_VIEWSHARING		5
#define VFC_SEGMENT_TYPE_ETL_T1			6
#define VFC_SEGMENT_TYPE_ETL_T2			7
#define VFC_SEGMENT_TYPE_EQUAL_PE		8
#define VFC_SEGMENT_TYPE_REVERSE_CENTRIC	9

/* VarField:	SVN_PLN_PREC_PULSE
 */
#define VFC_PREC_PULSE_NONE		0
#define VFC_PREC_PULSE_DWI		1
#define VFC_PRECONTRAST_PULSE_T2	2
#define VFC_PREC_PULSE_T2	2

/* VarField:	SVN_PLN_MOVING_COUCH_FLG
 */
#define VFC_MOVING_COUCH_NONE		0
#define VFC_MOVING_COUCH_CONTINUOUSLY	1
#define VFC_MOVING_COUCH_MANUAL		2
#define VFC_MOVING_COUCH_AUTOMATIC	3


/* VarField:	SVN_PLN_SPEEDER_FLAG
 */
#define       VFC_SPEEDER_FLAG_BIT_PE     1
#define       VFC_SPEEDER_FLAG_BIT_RO     (1<<1)
#define       VFC_SPEEDER_FLAG_BIT_SE     (1<<2)

/* VarField:	SVN_PLN_SPEEDER_TYPE
 */

#define     VFC_SPEEDER_TYPE_NONE         0
#define     VFC_SPEEDER_TYPE_SENSITIVITY  1
#define     VFC_SPEEDER_TYPE_SPEEDER      2

/* VarField:	SVN_PLN_SKIP_PREPULSE_CONTRAST
 */
#define VFC_SKIP_PREPULSE_CONTRAST_NORMAL	0
#define VFC_SKIP_PREPULSE_CONTRAST_HEAVY	1
#define VFC_SKIP_PREPULSE_CONTRAST_MIDDLE	2
#define VFC_SKIP_PREPULSE_CONTRAST_LIGHT	3

/* VarField:	SVN_PLN_PREP_IMAGE_TYPE
 */
#define VFC_PREP_IMAGE_TYPE_IMAGE       0
#define VFC_PREP_IMAGE_TYPE_MIP 		1

/* VarField:	SVN_PLN_ACQ_SPLIT_MODE
 */
#define VFC_ACQ_SPLIT_MODE_NONE    0
#define VFC_ACQ_SPLIT_MODE_AUTOPAUSE    1
#define VFC_ACQ_SPLIT_MODE_PREP			2

/* VarField:	SVN_PLN_AUTO_MOVINGBED_SUBTRACTION_IMAGES
   		SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES
 */
#define VFC_SUBTRACTION_NONE		(1L << 0)	/* 差分なし画像作成 */
#define VFC_SUBTRACTION_ABSOLUTE	(1L << 1)	/* 絶対値差分画像作成 */
#define VFC_SUBTRACTION_COMPLEX		(1L << 2)	/* 複素差分画像作成 */

/* VarField:	SVN_PLN_PROEDIT_MODE
 */
#define VFC_PROEDIT_MODE_NORMAL 0
#define VFC_PROEDIT_MODE_USER1  1
#define VFC_PROEDIT_MODE_USER2  2

/* VarField:	SVN_PLN_FATSAT_PULSE_TYPE
 */
#define VFC_FATSAT_PULSE_TYPE_STANDARD 0
#define VFC_FATSAT_PULSE_TYPE_ROBUST   1
#define VFC_FATSAT_PULSE_TYPE_LIGHT    2
#define VFC_FATSAT_PULSE_TYPE_DFS      3
#define VFC_FATSAT_PULSE_TYPE_TFS      4
#define VFC_FATSAT_PULSE_TYPE_SPAIR    5

/* VarField:	SVN_PLN_ENCODE_ORDER
 */
#define VFC_ENCODE_ORDER_SEQUENTIAL 0
#define VFC_ENCODE_ORDER_CENTRIC    1

/* VarField:	SVN_PLN_PROBE_FLAG
 */
#define VFC_PROBE_NONE 0
#define VFC_PROBE_RTMC 1 /* Real Time Motion Correction */
#define VFC_PROBE_RTMP 2 /* Real Time Motion Probing */

/* VarField:	SVN_PLN_FLUORO_MODE
 */
#define VFC_FLUORO_NONE    0
#define VFC_FLUORO_DIRECT  1 /* Direct Mode */
#define VFC_FLUORO_SEQ_RM  2 /* Seqgen Mode */

/* VarField:	SVN_PLN_RETRO_MODE
 */
#define VFC_RETRO_MODE_NONE     0    
#define VFC_RETRO_MODE_1        1         
#define VFC_RETRO_MODE_2        2

/* VarField:	SVN_PLN_AUTO_PROEDIT_FLAG
 */
#define VFC_AUTO_PROEDIT_NONE     0    
#define VFC_AUTO_PROEDIT_3DLOCATOR        1         

/* VarField:	SVN_PLN_EPI_SORT_TYPE
 */
#define VFC_SORT_TYPE_NONE     0    
#define VFC_SORT_TYPE_T1       1         
#define VFC_SORT_TYPE_T2       2         

/* VarField:	SVN_PLN_LOCATOR_ROI_STYLE
 */
#define VFC_LOCATOR_ROI_STYLE_SLICES     0    
#define VFC_LOCATOR_ROI_STYLE_GROUP      1    
#define VFC_LOCATOR_ROI_STYLE_CENTER     2    

/* VarField:	SVN_PLN_ALTERNATED_ACQ_FLAG
 */
#define VFC_ALTERNATED_ACQ_NONE 0
#define VFC_ALTERNATED_ACQ_ON   1
#define VFC_ALTERNATED_ACQ_DLY2 2

/* VarField:	SVN_PLN_NON_SELECTIVE_IR_PULSE
 */
#define VFC_NON_SELECTIVE_IR_PULSE_ON  0
#define VFC_NON_SELECTIVE_IR_PULSE_OFF 1

/* VarField:	SVN_PLN_BBIR_PULSE
 */
#define VFC_BBIR_PULSE_NONE   0
#define VFC_BBIR_PULSE_DOUBLE 1
#define VFC_BBIR_PULSE_TSLIT  2

/* VarField:	SVN_PLN_LOCAL_CF_FLAG
 */
#define VFC_LOCAL_CF_NONE   0
#define VFC_LOCAL_CF_ON 1

/* VarField:	SVN_PLN_FLOW_COMP
 */
#define VFC_FLOW_COMP_OFF    0
#define VFC_FLOW_COMP_RO     1
#define VFC_FLOW_COMP_PE     2
#define VFC_FLOW_COMP_SS     4
#define VFC_FLOW_COMP_RO_SS  5
#define VFC_FLOW_COMP_3AXIS  7

/* VarField:	SVN_PLN_GATE_TRIG_TYPE
 */
#define VFC_GATE_TRIG_TYPE_NORMAL    0
#define VFC_GATE_TRIG_TYPE_ACROSS    1


/* VarField:	SVN_PLN_AMB_MODE_FLG
   このフィールドな無い場合はVFC_AMB_NONEと同じ意味とする。
 */
#define	VFC_AMB_MODE_NONE	0	/*　通常撮像 */
#define	VFC_AMB_MODE_MASTER	1	/* AMB Mater protocol */
#define	VFC_AMB_MODE_SLAVE	2	/* AMB SLAVE protocol */  

/* VarField:	SVN_PLN_FREQ_SUPPRESSION_PASTA
 */
#define VFC_SUPPRESSION_NONE      0
#define VFC_SUPPRESSION_FAT       1
#define VFC_SUPPRESSION_SI        2

/* VarField:	SVN_PLN_SLICE_NUMBER_METHOD
 */
#define VFC_SLICE_NUMBER_METHOD_SLICES      0   /* スラブ間でのシーケンシャルな
                                                   スライス番号表示 */
#define VFC_SLICE_NUMBER_METHOD_SLAB        1   /* スラブ毎のスライス番号表示 */

/* VarField:	SVN_PLN_GATE_INTERVAL_SEGMENTATION_TYPE
 */
#define VFC_GATE_INTERVAL_SEGMENT_TYPE_SEQUENTIAL   1
#define VFC_GATE_INTERVAL_SEGMENT_TYPE_REVERSE      2

/* VarField:	SVN_PLN_PHASE_CORRECTION_PRESCAN_FLAG
 */
#define VFC_PHASE_CORRECTION_PRESCAN_OFF   0
#define VFC_PHASE_CORRECTION_PRESCAN_ON    1

/* VarField:	SVN_PLN_K_SPACE_TRAJECTORY_TYPE
 */
#define VFC_K_SPACE_TRAJECTORY_TYPE_NONE        0
#define VFC_K_SPACE_TRAJECTORY_TYPE_2DJET       1
#define VFC_K_SPACE_TRAJECTORY_TYPE_2DRADIAL    2
#define VFC_K_SPACE_TRAJECTORY_TYPE_3DRADIAL    3
#define VFC_K_SPACE_TRAJECTORY_TYPE_SPIRAL      4

/* VarField:	SVN_PLN_SWITCH_COIL_TYPE
 */
#define VFC_SWITCH_COIL_TYPE_NONE       0
#define VFC_SWITCH_COIL_TYPE_MAP        1 /* PAC/WB切り替え */

/* VarField:	SVN_PLN_VARI_FLOP_FLAG
 */
#define VFC_VFA_FOR_NONE            0
#define VFC_VFA_FOR_2D              1
#define VFC_VFA_FOR_3DBRAIN_T2      2
#define VFC_VFA_FOR_3DBRAIN_PD      3
#define VFC_VFA_FOR_3DOTHER_T2      4
#define VFC_VFA_FOR_3DOTHER_PD      5

/* VarField:	SVN_PLN_GATE_CTRL_TYPE
 */
#define VFC_GATE_CTRL_TYPE_BYRECON      0
#define VFC_GATE_CTRL_TYPE_BYPROLIM     1

/* VarField:	SVN_PLN_SAR_CALC_MODE
 */
#define VFC_SAR_CALC_MODE_MODEL     0
#define VFC_SAR_CALC_MODE_SEQ       1

/* VarField:	SVN_PLN_CDS_FLAG
 */
#define VFC_CDS_FLAG_OFF     0
#define VFC_CDS_FLAG_ON      1

/* VarField:	SVN_PLN_CDS_STATUS
 */
#define VFC_CDS_STATUS_AUTO_SELECT      0x0001
#define VFC_CDS_STATUS_SCAN             0x0002
#define VFC_CDS_STATUS_USE_PREVIOUS     0x0004




/*****************************************************************************
  SEQUENCE name subspace
  */

/* VarField:	SVN_SEQ_ECHO_TYPE
 */
#define VFC_ECHO_TYPE_SPIN_ECHO		0	/* Spin Echo */
#define VFC_ECHO_TYPE_FIELD_ECHO	1	/* Field Echo */

/* VarField:	SVN_SEQ_RFL_WAVE_FAMILY
		SVT_SEQ_IMG_WAVE_FAMILY
 */
#define VFC_SEQ_WAVE_FAMILY_REGULAR	0


/* VarField:	SVN_SEQ_RO_CHANNEL_POLARITY
		SVN_SEQ_PE_CHANNEL_POLARITY
		SVN_SEQ_ECHO_POLARITY
		SVN_SEQ_PE_STEP_POLARITY
		SVN_SEQ_READ_POLARITY
 */
#define VFC_POLARITY_POSITIVE	  1
#define VFC_POLARITY_NEGATIVE	(-1)

/* VarField:	SVN_SEQ_DATA_ALTERNATION
 */
#define VFC_SEQ_DATA_ALTERNATION_NONE	0
#define VFC_SEQ_DATA_ALTERNATION_POS	1	/*  1 -1  1 -1 ... */
#define VFC_SEQ_DATA_ALTERNATION_NEG	(-1)	/* -1  1 -1  1 ... */

/* VarField:	SVN_SEQ_RF_GRADS_OFF_FLAGS
 */
#define VFC_SEQ_RF_OFF		1
#define VFC_SEQ_SL_CHANNEL_OFF	(1<<1)
#define VFC_SEQ_PE_CHANNEL_OFF	(1<<2)
#define VFC_SEQ_RO_CHANNEL_OFF	(1<<3)

/* VarField:	SVN_SEQ_SHIM_FLAG
 */
#define VFC_SHIM_NONE			0
#define VFC_SHIM_STANDARD		1
#define VFC_SHIM_HIGH_RESOLUTION	2

/* VarField:	SVN_SEQ_SHIM_FSY_FLAG
 */
#define VFC_SHIM_FSY_WATER	0
#define VFC_SHIM_FSY_FAT	1

/* VarField:	SVN_SEQ_LOOP_ORDER
 */
#define VFC_LOOP_PE_SE 1 	/* (PE->SE) */
#define VFC_LOOP_SE_PE 2 	/* (SE->PE) */

/* VarField:	SVN_SEQ_DBDT_MODE
 */
#define VFC_DBDT_NORMAL       0
#define VFC_DBDT_CONTROLLED1  1
#define VFC_DBDT_CONTROLLED2  2
#define VFC_DBDT_HIGH1        3
#define VFC_DBDT_HIGH2        4
#define VFC_DBDT_HIGH1_CONTROLLED1		5
#define VFC_DBDT_HIGH1_CONTROLLED2		6
#define VFC_DBDT_HIGH2_CONTROLLED1		7
#define VFC_DBDT_HIGH2_CONTROLLED2		8

/* VarField:	SVN_SEQ_DBDT_IEC_OPERATING_MODE
 */
#define VFC_DBDT_IEC_OPERATING_NORMAL          0
#define VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED  1
#define VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED  2
#define VFC_DBDT_IEC_OPERATING_OVER_CARDIAC_THRESHOLD  3

/* VarField:	SVN_SEQ_RAMP_SAMPL_METHOD
 */
#define VFC_RAMP_SAMPL_NONE                    0
#define VFC_RAMP_SAMPL_VARIABLE_ADC            1   
#define VFC_RAMP_SAMPL_CONSTANT_ADC_RES        2
#define VFC_RAMP_SAMPL_CONSTANT_ADC_SN         4

/* VarField:	SVN_SEQ_DBDT_IEC_EDITION
 */
#define	VFC_DBDT_IEC_EDITION_1995		1
#define	VFC_DBDT_IEC_EDITION_2002		2

/* VarField:	SVN_SEQ_DBDT_CALC_MODE
 */
#define	VFC_DBDT_CALC_MODE_CSR		0
#define	VFC_DBDT_CALC_MODE_CSR_MIN_DT	1
#define	VFC_DBDT_CALC_MODE_CRT		2

/* VarField:	SVN_SEQ_PHASE_CORRECTION_PRESCAN_METHOD
 */
#define	VFC_PHASE_CORRECTION_PRESCAN_FSE_THREE_ECHO_MODE	0
#define	VFC_PHASE_CORRECTION_PRESCAN_FSE_FIVE_ECHO_MODE		1


/*****************************************************************************
  EXECUTION name subspace
  */

/* VarField:	SVN_EXE_PRESCAN_FSY_ACTION
		SVN_EXE_PRESCAN_RFL_ACTION
		SVN_EXE_PRESCAN_RGN_ACTION
   */
#define VFC_PRESCAN_CLINICAL		0
#define VFC_PRESCAN_USE_PREVIOUS	1
#define VFC_PRESCAN_USE_VARFIELD	2
#define VFC_PRESCAN_GAIN_MEASURE        0
#define VFC_PRESCAN_GAIN_CALCULATE      3
#define VFC_PRESCAN_LOAD_VARFIELD	4

/* VarField:	SVN_EXE_PROTOCOL_ACQ_STATUS
   */
#define VFC_ACQ_STATUS_WAIT		0
#define VFC_ACQ_STATUS_SUCCESS		1
#define VFC_ACQ_STATUS_FAIL		2
#define VFC_ACQ_STATUS_CURRENT		3
/*
 * The last two (VFC_PRESCAN_GAIN_MEASURE, VFC_PRESCAN_GAIN_CALCULATE)
 * have been added to more easily distinguish between two methods of
 * determining a good receiver gain value for imaging.
 *
 * Please notice that VFC_PRESCAN_GAIN_MEASURE is defined to be the
 * same as VFC_PRESCAN_CLINICAL. This re-definition is intentional.
 * The purpose is backward-compatibility.
 */

/* VarField:	SVN_EXE_VERBOSITY
 */
#define VFC_VERBOSITY_CLINICAL		0
#define VFC_VERBOSITY_VERBOSE		1
#define VFC_VERBOSITY_CONFIGURED	2

/* VarField:	SVN_EXE_RGN_COVERAGE_TYPE
 */
#define VFC_RGN_CENTER_COVERAGE         1
#define VFC_RGN_ALL_COVERAGE            2

/* VarField:	SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS
 */
#define VFC_PHASE_CORRECTION_PRESCAN_NONE			0
#define VFC_PHASE_CORRECTION_PRESCAN_OK				1
#define VFC_PHASE_CORRECTION_PRESCAN_USE_DEFAULT		2
#define VFC_PHASE_CORRECTION_PRESCAN_RETRY_DUE_TO_OVERFLOW	3
#define VFC_PHASE_CORRECTION_PRESCAN_RETRY_IN_ALGORITHM		4
#define VFC_PHASE_CORRECTION_PRESCAN_FAILED			5

/* VarField:	SVN_EXE_MOVE_COUCH_METHOD
		SVN_PLN_MOVE_COUCH_METHOD_FLAG
 */
#define VFC_MOVE_COUCH_METHOD_NONE				-1
#define VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI			0
#define VFC_MOVE_COUCH_METHOD_PARENT_POSI			1

/* VarField:	SVN_EXE_PRESCAN_RFL_SLICE_ORI
 */
#define VFC_RFL_SLICE_ORI_SG		1
#define VFC_RFL_SLICE_ORI_CO		2
#define VFC_RFL_SLICE_ORI_AX		3

/*****************************************************************************
  PROCESS name subspace
  */

/* VarField:	SVN_PRC_DATA_KIND
 */
#define VFC_PROC_RECON 	      		0
#define VFC_PROC_INTERMEDIATE 		1
#define VFC_PROC_TEMPORARY 		2	/* 中間データ */

#define VFC_PROC_MERGE_POSITION		13

#define VFC_PROC_MRA_ALONG		20
#define VFC_PROC_MRA_ACROSS		21
#define VFC_PROC_MRA_ROI		22

#define VFC_PROC_PCMRA_VEL		40
#define VFC_PROC_PCMRA_SPEED		41

#define VFC_PROC_SPIKE_FILTER		70
#define VFC_PROC_BEST_FILTER		71

#define VFC_PROC_MRS			80

#define VFC_PROC_IMCALC_AVR		100
#define VFC_PROC_RAVE_FILTER		110

#define VFC_PROC_SHIM_CALC		200	/* 磁場分布測定画像 */
#define VFC_PROC_SHIM_ADJUST		201	/* 磁場分布調整画像 */

/* VarField:	SVN_PRC_ECHO_CODE
 */
#define VFC_ECHO_RECON			VFC_PROC_RECON
#define VFC_ECHO_SYNTH			1

/* VarField:	SVN_PRC_T2_CORR_METHOD
 */
#define VFC_T2_CORR_NONE			0
#define VFC_T2_CORR_FOR_T2W			1
#define VFC_T2_CORR_FOR_T1W			2
#define VFC_T2_CORR_FOR_PD			3
#define VFC_T2_CORR_FOR_EPI_2SHOT_METHOD	4
#define VFC_T2_CORR_FOR_EPI_ECHO_BY_ECHO	5
#define VFC_T2_CORR_FOR_EPI_CENTER		6
#define VFC_GLBL_T2_CORR_TEMP			17
#define VFC_GLBL_T2_CORR_ESTM			33

/* VarField:	SVN_PRC_PHASE_CORR_METHOD
 */
#define VFC_PHASE_CORR_NONE		0
#define VFC_PHASE_CORR_FULL		1	/* Phase Rotation Full */
#define VFC_PHASE_CORR_CENTER		2	/* Phase Rotation Center */
#define VFC_PHASE_CORR_TIME		3	/* Time Domain */
#define VFC_PHASE_CORR_ONE_SHOT_METHOD	16	/* 位相補正1ショットモード */
#define VFC_PHASE_CORR_ZERO_SHOT_METHOD	32	/* 位相補正0ショットモード */
#define VFC_PHASE_CORR_TWO_SHOTS_METHOD 64	/* 位相補正2ショットモード */
#define VFC_PHASE_CORR_FOR_SFT_P_TUNE	128	/* SFT-tool(SE/STE調整用）
						   画像傾き出力 */
#define VFC_PHASE_CORR_FOR_SFT_D_TUNE	256	/* SFT-tool(磁場遅れ調整用）
						   ２ショット位相補正量出力 */

/* VarField:	SVN_PRC_PE_SORT_METHOD
		SVN_PRC_SE_SORT_METHOD
 */
#define VFC_SORT_NONE			0
#define VFC_SORT_FREE			1
#define VFC_SORT_CENTRIC		2

/* VarField:	SVN_PRC_VIEW_SORT_METHOD
 */
#define VFC_VIEW_NONE		0	/* なにもしない。*/
#define VFC_VIEW_FREE		1	/* VIEW_SORT_TABLEを使用する。*/

/* VarField:	SVN_PRC_PE_SRTTBL
		SVN_PRC_SE_SRTTBL
 */
#define VFC_SRTTBL_ATTRB_NONE		0
#define VFC_SRTTBL_ATTRB_TEMPLATE	1
#define VFC_SRTTBL_ATTRB_ZEROFILL	2
#define VFC_SRTTBL_ATTRB_CUTOUT		3
/*#define VFC_SRTTBL_ATTRB_CONJUGATE	4*/
#define VFC_SRTTBL_ATTRB_NAVI		4
#define VFC_SRTTBL_ATTRB_TEMPLATENAVI	5
#define VFC_SRTTBL_ATTRB_NOISE		6
#define VFC_SRTTBL_ATTRB_REVERSE	8

/* VarField:	SVN_PRC_BARIQ_MODE
 */
#define VFC_BARIQ_NONE			0
#define VFC_BARIQ_ON			1

/* VarField:	SVN_PRC_WIENER_CORR_METHOD
 */
#define VFC_WIENER_CORR_NONE		0x0000
#define VFC_WIENER_CORR_ON		0x0001
#define VFC_WIENER_SE_TEMPLATE		0x0010
#define VFC_WIENER_SE_ESTIMATE		0x0020
#define VFC_WIENER_SE_STEP		0x0030
#define VFC_WIENER_PE_TEMPLATE		0x0100
#define VFC_WIENER_PE_ESTIMATE		0x0200
#define VFC_WIENER_PE_STEP		0x0300
#define VFC_WIENER_RO_TEMPLATE		0x1000
#define VFC_WIENER_RO_ESTIMATE		0x2000
#define VFC_WIENER_RO_STEP		0x3000

/* VarField:	SVN_PRC_PS_RECON_IMAGE
 */
#define VFC_PS_RECON_SPEED_PE		0x0001
#define VFC_PS_RECON_SPEED_RO		0x0002
#define VFC_PS_RECON_SPEED_SE		0x0004
#define VFC_PS_RECON_FLOW_PE		0x0010
#define VFC_PS_RECON_FLOW_RO		0x0020
#define VFC_PS_RECON_FLOW_SE		0x0040
#define VFC_PS_RECON_VESSEL		0x0100
#define VFC_PS_RECON_PROTON		0x1000

/* VarField:	SVN_PRC_PS_IMAGE_KIND
 */
#define VFC_PS_RECON_SPEED_IMAGE	1
#define VFC_PS_RECON_FLOW_IMAGE		2
#define VFC_PS_RECON_VESSEL_IMAGE	3

/* VarField:	SVN_PRC_PS_COMBINATION
 */
#define VFC_PS_COMB_NONE		0
#define VFC_PS_COMB_ADD			1
#define VFC_PS_COMB_SUB			2

#define VFC_PS_COMB(n,t)		(t<<2*(n-1))

/* VarField:	SVN_PRC_AUTO_PERFUSION_MAPPING
 */
#define VFC_PERFUSION_PH	(1L << 0)	/* Peak Height */
#define VFC_PERFUSION_PT	(1L << 1)	/* Peak Time */
#define VFC_PERFUSION_AT	(1L << 2)	/* Appearance Time */
#define VFC_PERFUSION_DT	(1L << 3)	/* Disappearance Time */
#define VFC_PERFUSION_AC	(1L << 4)	/* Area Under Curve (rCBV相当) */
#define VFC_PERFUSION_rMT1	(1L << 5)	/* 1st Moment */
#define VFC_PERFUSION_MT2	(1L << 6)	/* 2nd Moment */
#define VFC_PERFUSION_TT	(1L << 7)	/* Transit Time */
#define VFC_PERFUSION_rFlow	(1L << 8)	/* relative Flow（rCBF相当）*/
#define VFC_PERFUSION_US	(1L << 9)	/* Up Slope */
#define VFC_PERFUSION_DS	(1L << 10)	/* Down Slope */
#define VFC_PERFUSION_PTe	(1L << 11)	/* effective Peak Time */
#define VFC_PERFUSION_rMT1e	(1L << 12)	/* effective 1st Moment */
#define VFC_PERFUSION_rFlowe	(1L << 13)	/* effective relative Flow */
#define VFC_PERFUSION_LSQerror	(1L << 14)	/* LSQ error */

/* VarField:	SVN_PRC_RECON_ENGINE
 */
#define VFC_RECON_HOST		0 	/* HOST */
#define VFC_RECON_RACE		1 	/* RACE */
#define VFC_RECON_VAP		2 	/* VAP */

/* VarField:	SVN_PRC_RECON_AVERAGE_METHOD
 */
#define VFC_RECON_AVERAGE_NONE		0	/* 再構成側での
						   アベレージングなし。*/
#define VFC_RECON_AVERAGE_DIFFUSION	1	/* diffusion撮影で使用。
						   画像データでの平均加算。
						   加算時のデータ形式は、
						   PDATA_FORMによる。*/
#define VFC_RECON_AVERAGE_FISP_CISS	2	/* FISP撮影のCISSモードで
						   使用。*/
#define VFC_RECON_AVERAGE_FISP_SIMCAST	3	/* FISP撮影のSIMCASTモードで
						   使用。*/

/* VarField:	SVN_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE
 */
#define VFC_DIFFUSION_ALIGNMENT_REFER           (1L<<0)
#define VFC_DIFFUSION_ALIGNMENT_CALC            (1L<<1)
#define VFC_DIFFUSION_ALIGNMENT_CALC_SAVE       (1L<<2)

/* VarField:	SVN_PRC_AUTO_DIFFUSION_MAPPING
 */
#define VFC_DIFFUSION_FA           (1L<<0)
#define VFC_DIFFUSION_L1           (1L<<1)
#define VFC_DIFFUSION_L2           (1L<<2)
#define VFC_DIFFUSION_L3           (1L<<3)
#define VFC_DIFFUSION_V1X          (1L<<4)
#define VFC_DIFFUSION_V1Y          (1L<<5)
#define VFC_DIFFUSION_V1Z          (1L<<6)

/* VarField:	SVN_PRC_INTENSITY_MAP_TYPE
 */
#define VFC_INTENSITY_MAP_TYPE_NONE         0     
#define VFC_INTENSITY_MAP_TYPE_INTENSITY    (1<<0)
#define VFC_INTENSITY_MAP_TYPE_SPEEDER      (1<<1)

/* VarField:	SVN_PRC_INTENSITY_CORR_FLAG
 */
#define VFC_INTENSITY_CORR_OFF    0
#define VFC_INTENSITY_CORR_ON     1

/* VarField:	SVN_PRC_PAC_CORRECT_ACTION 
 */
#define VFC_PAC_PHASE_CORRECT  1     
#define VFC_PAC_GAIN_CORRECT   (1<<1)

/* VarField:	SVN_PRC_DIRECT_QUEUE
 */
#define VFC_SET_NORMAL_QUEUE 0
#define VFC_SET_DIRECT_QUEUE 1

/* VarField:	SVN_PRC_RECON_START_AFTER_REFVIEW
 */
#define VFC_AFTER_REFVIEW_AUTO		0
#define VFC_AFTER_REFVIEW_MANUAL	1

/* VarField:	SVN_PRC_AUTO_GDC_ACTION
 */
#define VFC_GDC_ACTION_OFF   0
#define VFC_GDC_ACTION_ON    1

/* VarField:	SVN_PRC_AUTO_GDC_DIM
 */
#define VFC_GDC_DIM_2D   2
#define VFC_GDC_DIM_3D   3

/* VarField:	SVN_PRC_AUTO_ALT_SUBTRACTION
 */
#define VFT_PRC_AUTO_ALT_SUBTRACTION_OFF  0
#define VFT_PRC_AUTO_ALT_SUBTRACTION_ON   1

/* VarField:	SVN_PRC_SUBTRACTION_POLARITY
 */
#define VFC_SUBTRACTION_POSITIVE   0
#define VFC_SUBTRACTION_NEGATIVE   1

/* VarField:	SVN_PRC_GDC_FLAG
 */
#define VFC_GDC_FLAG_OFF    0
#define VFC_GDC_FLAG_ON     1

/* VarField:	SVN_PRC_GDC_DIM
 */
#define VFC_GDC_DIMENSION_2D    2
#define VFC_GDC_DIMENSION_3D    3

/* VarField:	SVN_PRC_GDC_LOCATOR_IMAGE
 */
#define VFC_GDC_LOCATOR_IMAGE_OFF   0
#define VFC_GDC_LOCATOR_IMAGE_ON    1

/* VarField:	SVN_PRC_GDC_LOCATOR_MASK
 */
#define VFC_GDC_LOCATOR_MASK_OFF   0
#define VFC_GDC_LOCATOR_MASK_ON    1

/* VarField:	SVN_PRC_NOISE_GAIN_TABLE_FLAG
 */
#define VFC_NOISE_GAIN_TABLE_OFF   0
#define VFC_NOISE_GAIN_TABLE_ON    1

/* VarField:	SVN_PRC_NOISE_MATRIX_TABLE_FLAG
 */
#define VFC_NOISE_MATRIX_TABLE_OFF   0
#define VFC_NOISE_MATRIX_TABLE_ON    1

/* VarField:	SVN_PRC_MOTION_CORR_FLAG_FOR_JET
 */
#define VFC_MOTION_CORR_FLAG_FOR_JET_ROTATION       (1L << 0)   /* rotation */
#define VFC_MOTION_CORR_FLAG_FOR_JET_TRANSLATION    (1L << 1)   /* translation */
#define VFC_MOTION_CORR_FLAG_FOR_JET_REJECTION      (1L << 2)   /* rejection */

/* VarField:	SVN_PRC_MOTION_CORR_OPT_METHOD_FOR_JET
 */
#define VFC_MOTION_CORR_OPT_METHOD_FOR_JET_NONE     0   /* None */

/* VarField:	SVN_PRC_REGRIDDING_METHOD_FOR_JET
 */
#define VFC_REGRIDDING_METHOD_FOR_JET_NONE              0 /* None */
#define VFC_REGRIDDING_METHOD_FOR_JET_JACKSON           1 /* Jackson */
#define VFC_REGRIDDING_METHOD_FOR_JET_JACKSON_DECONV    2 /* Jackson + deconv */
#define VFC_REGRIDDING_METHOD_FOR_JET_ENEN              3 /* ENEN */
#define VFC_REGRIDDING_METHOD_FOR_JET_ENEN_DECONV       4 /* ENEN + deconv */

/* VarField:	SVN_PRC_K_SPACE_TRAJECTORY_SORT_METHOD
 */
#define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_NONE          0
#define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_USE_DEFAULT   1
#define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_SORT_FREE     2

/* VarField:	SVN_PRC_DC_CORR_METHOD_FOR_JET
 */
#define VFC_DC_CORR_METHOD_FOR_JET_NONE             0   /* None */

/* VarField:	SVN_PRC_DUAL_ECHO_COMBINING_TYPE
 */
#define VFC_DUAL_ECHO_COMBINING_NONE                0   /* None */
#define VFC_DUAL_ECHO_COMBINING_ABS_ADD             1   /* Addition(1st + 2nd) */
#define VFC_DUAL_ECHO_COMBINING_ABS_SUB1            2   /* Subtraction1(1st - 2nd) */
#define VFC_DUAL_ECHO_COMBINING_ABS_SUB2            3   /* Subtraction2(2nd - 1st) */
#define VFC_DUAL_ECHO_COMBINING_FWS_WEAK            4   /* FWS(Weak) */
#define VFC_DUAL_ECHO_COMBINING_FWS_MEDIUM          5   /* FWS(Medium) */
#define VFC_DUAL_ECHO_COMBINING_FWS_STRONG          6   /* FWS(Strong) */

/* VarField:	SVN_PRC_DUAL_ECHO_DISPLAY_FLAG
 */
#define VFC_DUAL_ECHO_HIDE                          0
#define VFC_DUAL_ECHO_SHOW                          1

/* VarField:	SVN_PRC_AFI_ALGORITHM
 */
#define VFC_AFI_ALGORITHM_MARGOSIAN         0  /* Conventional algorithm */
#define VFC_AFI_ALGORITHM_POCS              1  /* POCS */
#define VFC_AFI_ALGORITHM_FIR               2  /* FIR */
#define VFC_AFI_ALGORITHM_FIR_POCS          3  /* FIR + POCS */
#define VFC_AFI_ALGORITHM_MOFIR             4  /* MoFIR */
#define VFC_AFI_ALGORITHM_MOFIR_POCS        5  /* MoFIR + POCS */
#define VFC_AFI_ALGORITHM_HYBRID            8  /* Hybrid */
#define VFC_AFI_ALGORITHM_POCS_HYBRID       9  /* POCS + Hybrid */
#define VFC_AFI_ALGORITHM_FIR_HYBRID        10 /* FIR + Hybrid */
#define VFC_AFI_ALGORITHM_FIR_POCS_HYBRID   11 /* FIR + POCS + Hybrid */
#define VFC_AFI_ALGORITHM_MOFIR_HYBRID      12 /* MoFIR + Hybrid */
#define VFC_AFI_ALGORITHM_MOFIR_POCS_HYBRID 13 /* MoFIR + POCS + Hybrid */

/* VarField:	SVN_PRC_AUTO_STAMD_MODE
 */
#define VFC_AUTO_STAMD_MINIP        0   /* 最小値投影 */
#define VFC_AUTO_STAMD_MIP          1   /* 最大値投影 */

/*****************************************************************************
  MRS name subspace
  */
/* VarField:	SVN_MRS_REF_MODE
 */
#define VFC_MRS_REF_NONE    0
#define VFC_MRS_REF_NORMAL  1
#define VFC_MRS_REF_FAST    2
#define VFC_MRS_REF_HNORMAL 11
#define VFC_MRS_REF_HFAST   22

/* VarField: SVN_EXE_PRESCAN_MRS_F0_ACTION
             SVN_EXE_PRESCAN_MRS_REF_ACTION
             SVN_EXE_PRESCAN_MRS_WSAT_ACTION
 */
#define VFC_PRESCAN_MRS_SKIP    0
#define VFC_PRESCAN_MRS_EXE     1

/*****************************************************************************
  PRESENT name subspace
  */

/*****************************************************************************
  DICOM name subspace
  */
#define VFC_MPPS_NONE		1
#define VFC_MPPS_START		2
#define VFC_MPPS_STOP		3
#define VFC_MPPS_DONE		4
#define VFC_MPPS_ERROR		5

#endif /* !tami_vfStudy_vf_appcodes_H__ */
