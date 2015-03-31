/*********************************************************************
 *
 *  (c) Copyright 1997 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *	This file contains the varfield names for the SEQUENCE subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The SEQUENCE subspace contains fields associated with the detailed
 *	physics parameters in an MRI sequence.
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



#ifndef tami_vfStudy_vfs_sequence_H__
#define tami_vfStudy_vfs_sequence_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/
#define SVN_SEQ_SHIM_OFFSET	(SVN_MIN_INDEPENDENT+SVN_OFF_SEQUENCE+0)
#define SVT_SEQ_SHIM_OFFSET	VFT_INT32
#define SVS_SEQ_SHIM_OFFSET	"Shimming offset for shim-amp."
/*	PQMがシステムのデフォルト値を読み出し、root nodeに
	記述する。シミングスキャンの直前にPQMがprotocol
	nodeにこの値をコピーする。シミングスキャンの直前に
	PQMはAcqmanにこの値をRmに送る要求を出す。
	要求を受けてAcqmanはroot nodeの値を読みだし、Rmへ送る。
	protocol nodeの値を用いてmsShimが調整量を
	算出し、processed nodeにセットする。
	msShimの調整処理終了後、processed nodeの値をshimmingが
	root nodeにコピーする。シミング処理終了後、PQMはAcqman
	にこの調整量をRmへ送る要求を出す。Acqmanは要求を受けて、
	root nodeの値をRmに送る。
	*/

#define SVN_SEQ_SHIM_GRAD_OFFSET	(SVN_MIN_INDEPENDENT+SVN_OFF_SEQUENCE+1)
#define SVT_SEQ_SHIM_GRAD_OFFSET 	VFT_INT32
#define SVS_SEQ_SHIM_GRAD_OFFSET 	"SHIM GRAD OFFSET"
/*	PQMがシステムのデフォルト値を読み出し、root nodeに
	セットする。シミングスキャンの直前にPQMがprotocol
	nodeにこの値をコピーする。protocol nodeの
	値を用いてmsShimが算出し、processed nodeに
	セットする。msShimが算出した値をshimmingが
	root nodeにコピーする。libgtで、processed nodeの値
	をseqgenが読みだし、本収集のオフセットにのせる。
	*/

#define SVN_SEQ_SSFP_PRE_PULSE_PITCH    (SVN_MIN_INDEPENDENT+SVN_OFF_SEQUENCE + 2)
#define SVT_SEQ_SSFP_PRE_PULSE_PITCH    VFT_FLT32
#define SVS_SEQ_SSFP_PRE_PULSE_PITCH    "SSFP preFlip pulse pitch"
/*      (1)SSFP用preFlipパルスでのパルスピッチ
(2)prolimがPROTOCOLノードへセットし、seqgenが参照しSUBPROTOCOLノードへセットする。
*/

#define SVN_SEQ_SSFP_POST_PULSE_PITCH   (SVN_MIN_INDEPENDENT+SVN_OFF_SEQUENCE + 3)
#define SVT_SEQ_SSFP_POST_PULSE_PITCH   VFT_FLT32
#define SVS_SEQ_SSFP_POST_PULSE_PITCH   "SSFP postFlip pulse pitch"
/*      (1)SSFP用postFlipパルスでのパルスピッチ
(2)prolimがPROTOCOLノードへセットし、seqgenが参照しSUBPROTOCOLノードへセットする。
*/

/*****************************************************************************
  STUDY name space
*/



/*****************************************************************************
  PROTOCOL name space
*/
#define SVN_SEQ_LINK_RFL	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 0)
#define SVT_SEQ_LINK_RFL	VFT_STRING
#define SVS_SEQ_LINK_RFL	"Name of link command for RF Level adjustment"
/*	This field is the name of the link command to be used for RF
	level adjustment for this protocol.  It is determined by the
	support equations.
	*/

#define	SVN_SEQ_FEP_RFL		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 1)
#define	SVT_SEQ_FEP_RFL		VFT_STRING
#define	SVS_SEQ_FEP_RFL		"Name of FEP data for RF Level adjustment"
/*	This field is the name of the FEP data to be used for RF
	level adjustment for this protocol.  It is determined by the
	support equations.
	*/

#define SVN_SEQ_SEQUENCE_RFL	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 2)
#define SVT_SEQ_SEQUENCE_RFL	VFT_STRING
#define SVS_SEQ_SEQUENCE_RFL	"Sequence for RF Level including path"
/*	This field is the complete path to the seqgenned RF Level sequence
	that is to be executed by AcqMan.  It is determined by the sequence.
	*/

#define SVN_SEQ_RFL_WAVE_FAMILY	 (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 3)
#define SVT_SEQ_RFL_WAVE_FAMILY	 VFT_INT32
#define SVS_SEQ_RFL_WAVE_FAMILY	 "Family of the waveform used for RFL"
/*	This field indicates the family of the waveform used for RF Level
	adjustment.  It is written to study file by the RFL sequence or by
	Acqman.  The values for this field are defined in vf_appcodes.h.
	*/

#define SVN_SEQ_RFL_WAVE_SUM	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 4)
#define SVT_SEQ_RFL_WAVE_SUM	VFT_FLT32
#define SVS_SEQ_RFL_WAVE_SUM	"Sum of waveform values used for RFL"
/*	This field is the sum of values of all waveform points used for RFL
	adjustment for this protocol.  It is written to study file by the RFL
	sequence or by Acqman.
	*/

#define SVN_SEQ_LINK_GAINSRCH	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 5)
#define SVT_SEQ_LINK_GAINSRCH	VFT_STRING
#define SVS_SEQ_LINK_GAINSRCH	"Name of link command for Gain Search"
/*	This field is the name of the link command to be used for Gain
	Search for this protocol.  It is determined by the support equations.
	*/

#define	SVN_SEQ_FEP_GAINSRCH	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 6)
#define	SVT_SEQ_FEP_GAINSRCH	VFT_STRING
#define	SVS_SEQ_FEP_GAINSRCH	"Name of FEP data for Gain Search"
/*	This field is the name of the FEP data to be used for Gain Search
	for this protocol.  It is determined by the support equations.
	*/

#define SVN_SEQ_SEQUENCE_GAINSRCH    (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 7)
#define SVT_SEQ_SEQUENCE_GAINSRCH    VFT_STRING
#define SVS_SEQ_SEQUENCE_GAINSRCH    "Sequence for Gain Search, including path"
/*	This field is the complete path to the seqgenned Gain Search sequence
	that is to be executed by AcqMan.  It is determined by the sequence.
	*/

#define SVN_SEQ_LINK_RCVRGAIN	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 8)
#define SVT_SEQ_LINK_RCVRGAIN	VFT_STRING
#define SVS_SEQ_LINK_RCVRGAIN	"Name of link command for Receiver Gain adjust"
/*	This field is the name of the link command to be used for Receiver
	Gain adjustment for this protocol.  It is determined by the support
	equations.
	*/

#define	SVN_SEQ_FEP_RCVRGAIN	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 9)
#define	SVT_SEQ_FEP_RCVRGAIN	VFT_STRING
#define	SVS_SEQ_FEP_RCVRGAIN	"Name of FEP data for Receiver Gain adjust"
/*	This field is the name of the FEP data to be used for Receiver Gain
	adjustment for this protocol.  It is determined by the support
	equations.
	*/

#define SVN_SEQ_SEQUENCE_RCVRGAIN  (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 10)
#define SVT_SEQ_SEQUENCE_RCVRGAIN  VFT_STRING
#define SVS_SEQ_SEQUENCE_RCVRGAIN  "Sequence for Receiver Gain, including path"
/*	This field is the complete path to the seqgenned Receiver Gain
	sequence that is to be executed by AcqMan.  It is determined by the
	sequence.
	*/

#define SVN_SEQ_DETAIL_TR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 11)
#define SVT_SEQ_DETAIL_TR	VFT_FLT32
#define SVS_SEQ_DETAIL_TR	"Detailed TR for each echo"
/*	This field is the detailed TR in seconds for each echo in an image
	that is collected by the host.
	*/

#define SVN_SEQ_RFL_WAVE_PITCH	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 12)
#define SVT_SEQ_RFL_WAVE_PITCH	VFT_FLT32
#define SVS_SEQ_RFL_WAVE_PITCH	"Pitch of waveform values used for RFL"
/*	This field is the excitation waveform pitch used for RFL adjustment
	for this protocol.  It is written to study file by the RFL sequence or
	by Acqman.
	*/

#define SVN_SEQ_RFL_EST_GAIN_VALUE   (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 13)
#define SVT_SEQ_RFL_EST_GAIN_VALUE   VFT_FLT32
#define SVS_SEQ_RFL_EST_GAIN_VALUE   "Estimated Tx gain value for RFL"
/*	An estimated Tx gain value for RF Level measurement.  It is determined
	by the imaging sequence. It overwrites any system default initial gain
	value.
	*/

#define SVN_SEQ_IMG_WAVE_FAMILY	 (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 14)
#define SVT_SEQ_IMG_WAVE_FAMILY	 VFT_INT32
#define SVS_SEQ_IMG_WAVE_FAMILY	 "Family of the waveform used for imaging"
/*	This field indicates the family of the waveforms used for imaging.  It
	is normally the family of the excitation waveform.  It is written by
	the imaging sequence (prolim).  The values for this field are defined
 	in vf_appcodes.h.
	*/

#define SVN_SEQ_LINK_CFA	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 15)
#define SVT_SEQ_LINK_CFA        VFT_STRING
#define SVS_SEQ_LINK_CFA        "Name of link command for CFA"
/*	This field is the name of the link command to be used for 
	Center Frequency Adjustment for this protocol.
	It is determined by the support equations.
	*/

#define SVN_SEQ_FEP_CFA         (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 16)
#define SVT_SEQ_FEP_CFA         VFT_STRING
#define SVS_SEQ_FEP_CFA         "Name of FEP data for CFA"
/*	This field is the name of the FEP data to be used for 
	Center Frequency Adjustment for this protocol.
	It is determined by the support equations.
	*/

#define SVN_SEQ_SEQUENCE_CFA    (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 17)
#define SVT_SEQ_SEQUENCE_CFA    VFT_STRING 
#define SVS_SEQ_SEQUENCE_CFA    "Sequence for CFA including path"
/*	This field is the complete path to the seqgenned CFA sequence
	that is to be executed by AcqMan.  It is determined by the sequence.
	*/

#define SVN_SEQ_RFL_WAVE_ANGLE	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 18)
#define SVT_SEQ_RFL_WAVE_ANGLE	VFT_INT32
#define SVS_SEQ_RFL_WAVE_ANGLE	"Angle of the waveform for RFL"
/*	This field indicates the flip angle of the waveform used for RF Level
	measurement.  It is written to study file by the RFL sequence or by
	Acqman.
	*/

#define SVN_SEQ_ECHO_TYPE	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 19)
#define SVT_SEQ_ECHO_TYPE	VFT_INT32
#define SVS_SEQ_ECHO_TYPE	"Echo Type of each echo"
/*	This field identifies the echo type--spin echo, field echo, etc. of
	each echo in the protocol.  This information originates with the
	sequence.
	*/

#define SVN_SEQ_DETAIL_NEX	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 20)
#define SVT_SEQ_DETAIL_NEX	VFT_INT32
#define SVS_SEQ_DETAIL_NEX	"Detailed NEX for each echo"
/*	This field is the detailed number of averages for each echo in the
	acquired data set.  It is used to describe thing like NEX as a function
	of PE for selective averaging.
	*/

#define SVN_SEQ_SKIP_GAINSRCH   (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 21)
#define SVT_SEQ_SKIP_GAINSRCH	VFT_BOOL
#define SVS_SEQ_SKIP_GAINSRCH	"Gainsearch is not necessary"
/*	This flag indicates gainsearch is needed.
	When set, gainsearch prescan is not performed.
	*/

#define SVN_SEQ_MAX_GAINSRCH    (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 22)
#define SVT_SEQ_MAX_GAINSRCH    VFT_INT32
#define SVS_SEQ_MAX_GAINSRCH    "Index of gainsearch data with biggest signal"
/*      This is the index (starting with 0) of the data acquired
	during gainsearch prescan,  with the greatest signal magnitude.
	It is determined when acqman performs prescan.
	*/

#define SVN_SEQ_NUM_TEMPLATE_SHOT	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 23)
#define SVT_SEQ_NUM_TEMPLATE_SHOT	VFT_INT32
#define SVS_SEQ_NUM_TEMPLATE_SHOT	"The number of template shot"
/*	テンプレートショットの本数。アベレージングループの中に入る。
	シーケンス毎に固定の値をとり、プロリムにおいてスキャン時間計算に
	使用される。主として ＦＳＥ:T1W,PD にて定義される。
	*/

#define SVN_SEQ_NUM_PHASE_CORR_K1	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 24)
#define SVT_SEQ_NUM_PHASE_CORR_K1	VFT_INT32
#define SVS_SEQ_NUM_PHASE_CORR_K1	"K1 of window for template"
/*	テンプレート用窓関数のK1。
	ＦＳＥ：T1W,PD においてテンプレートショットを使用して位相補正を行ない
	ます。この、位相補正に際し、テンプレートデータに対し、LowPass Filter 
	を適用する必要があります。このフィルターを AFI と同様な窓関数として、
	規定します。詳細は、AFI についての木村さんの技報を参照して下さい。
	葛西
	*/

#define SVN_SEQ_NUM_PHASE_CORR_KC	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 25)
#define SVT_SEQ_NUM_PHASE_CORR_KC	VFT_INT32
#define SVS_SEQ_NUM_PHASE_CORR_KC	"KC of window for template"
/*	テンプレート用窓関数のKC。
	ＦＳＥ：T1W,PD においてテンプレートショットを使用して位相補正を行ない
	ます。この、位相補正に際し、テンプレートデータに対し、LowPass Filter 
	を適用する必要があります。このフィルターを AFI と同様な窓関数として、
	規定します。詳細は、AFI についての木村さんの技報を参照して下さい。
	葛西
	*/

#define SVN_SEQ_NUM_PHASE_CORR_K2	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 26)
#define SVT_SEQ_NUM_PHASE_CORR_K2	VFT_INT32
#define SVS_SEQ_NUM_PHASE_CORR_K2	"K2 of window for template"
/*	テンプレート用窓関数のK2。
	ＦＳＥ：T1W,PD においてテンプレートショットを使用して位相補正を行ない
	ます。この、位相補正に際し、テンプレートデータに対し、LowPass Filter 
	を適用する必要があります。このフィルターを AFI と同様な窓関数として、
	規定します。詳細は、AFI についての木村さんの技報を参照して下さい。
	葛西
	*/

#define SVN_SEQ_NUM_T2_AMP_CORR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 27)
#define SVT_SEQ_NUM_T2_AMP_CORR		VFT_INT32
#define SVS_SEQ_NUM_T2_AMP_CORR		"number of points for T2 decay correction"
/*	テンプレートデータ中でT２補正に使用するポイント数。
	要素２の１次元配列とし、デュアルコントラストに対応する。
  	*/

#define SVN_SEQ_NUM_AFI_CORR_KC		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 28)
#define SVT_SEQ_NUM_AFI_CORR_KC		VFT_INT32
#define SVS_SEQ_NUM_AFI_CORR_KC		"AFI primary points for half fouier mergin."
/*	AFI 再構成を行なうための、低周波位相を検出するためのデータ点数。
        配列要素は、それぞれ0: PE, 1:RO, 2:SE
	要素２の１次元配列とし、デュアルコントラストに対応する。
	*/

#define SVN_SEQ_NUM_AFI_CORR_K1		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 29)
#define SVT_SEQ_NUM_AFI_CORR_K1		VFT_INT32
#define SVS_SEQ_NUM_AFI_CORR_K1		"AFI roll-off 1 points for half fouier mergin."
/*	AFI 再構成を行なうための、低周波位相を検出するためのロールオフ点数。
        配列要素は、それぞれ0: PE, 1:RO, 2:SE
	要素２の１次元配列とし、デュアルコントラストに対応する。
	*/

#define SVN_SEQ_NUM_AFI_CORR_K2		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 30)
#define SVT_SEQ_NUM_AFI_CORR_K2		VFT_INT32
#define SVS_SEQ_NUM_AFI_CORR_K2		"AFI roll-off 2 points for half fouier mergin."
/*	AFI 再構成を行なうための、低周波位相を検出するためのロールオフ関数
	パラメータ。配列要素は、それぞれ0: PE, 1:RO, 2:SE
	要素２の１次元配列とし、デュアルコントラストに対応する。
	*/

#define SVN_SEQ_TEMPLATE_MODE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 31)
#define SVT_SEQ_TEMPLATE_MODE		VFT_FLT32
#define SVS_SEQ_TEMPLATE_MODE		"Template mode"
/*	テンプレートスキャンをどのように行なうか
	Hybrid EPI, 3D FastSE などに対応した拡張機能。
	詳細なモードについては未定で、再構成担当者との協議が必要。
	*/

#define SVN_SEQ_GRAD_ECHO_SPACE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 32)
#define SVT_SEQ_GRAD_ECHO_SPACE		VFT_FLT32
#define SVS_SEQ_GRAD_ECHO_SPACE		"GRAD echo space"
/*	(MultiShot)EPIスキャンにおけるエコー間隔
	*/

#define SVN_SEQ_RF_ECHO_SPACE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 33)
#define SVT_SEQ_RF_ECHO_SPACE		VFT_FLT32
#define SVS_SEQ_RF_ECHO_SPACE		"RF echo space"
/*	Hybrid EPI, FastSEスキャンにおけるエコー間隔
	*/

#define SVN_SEQ_EFFECTIVE_MASS		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 34)
#define SVT_SEQ_EFFECTIVE_MASS		VFT_FLT32
#define SVS_SEQ_EFFECTIVE_MASS		"Effective mass of imaging region"
/*	ＳＡＲ計算に使用する関心領域の実効質量。(kg)
	PQM が患者の体重に部位係数を乗じて算出する。
	*/

#define SVN_SEQ_SAR_LIMIT	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 35)
#define SVT_SEQ_SAR_LIMIT	VFT_FLT32
#define SVS_SEQ_SAR_LIMIT	"SAR limit"
/*	This is the maximum SAR allowed, in Watts/kg.
	*/

#define SVN_SEQ_PE_SCROLL_BY_SEQ  (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 36)
#define SVT_SEQ_PE_SCROLL_BY_SEQ  VFT_BOOL
#define SVS_SEQ_PE_SCROLL_BY_SEQ  "PE direction scrolling by sequence"
/*	If present and true, this field indicates sequence will do scrolling
	in PE direction for FID/DC correction. Recon can then skip that step.
	*/

#define SVN_SEQ_SLAB_SCROLL_BY_SEQ  (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 37)
#define SVT_SEQ_SLAB_SCROLL_BY_SEQ  VFT_BOOL
#define SVS_SEQ_SLAB_SCROLL_BY_SEQ  "Slice direction scrolling by 3D sequence"
/*	If present and true, this field indicates a 3D sequence will do
	scrolling in slice direction for FID/DC correction. Recon can then skip
	that step.
	*/

#define SVN_SEQ_TIME_BTWN_EXCTN_PULSES	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE+ 38)
#define SVT_SEQ_TIME_BTWN_EXCTN_PULSES	VFT_FLT32
#define SVS_SEQ_TIME_BTWN_EXCTN_PULSES	"Time between successive first echos"
/*      This is the time between the RF pulses for successive image
        slices, this is set by seqgen
	*/
#define SVN_SEQ_TIME_BTWN_FS_PULSES (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 39)
#define SVT_SEQ_TIME_BTWN_FS_PULSES	VFT_FLT32
#define SVS_SEQ_TIME_BTWN_FS_PULSES	"Time between successive field stab RF pulses"
/*      This is the time between the RF pulses for successive field stability
        slices, this is set by seqgen
	*/
#define SVN_SEQ_TIME_FS_TO_NEXT_RF (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 40)
#define SVT_SEQ_TIME_FS_TO_NEXT_RF VFT_FLT32
#define SVS_SEQ_TIME_FS_TO_NEXT_RF "Time between FS RF pulse and the next imaging RF pulse"
/*      This is the time between the RF pulses for the field stability
	slice and the first image slice, this is set by seqgen
	*/

#define SVN_SEQ_SHIM_FSY_FLAG		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 41)
#define SVT_SEQ_SHIM_FSY_FLAG		VFT_INT32
#define SVS_SEQ_SHIM_FSY_FLAG		"SHIM FSY FLAG"
/*	中心周波数が水／脂肪に合っているかのフラグ。
	水：０、脂肪：１
	libgt でshimmingInfoファイルより読みだし
	seqgen が本収集のプロトコルノードに記述する。
	*/

#define SVN_SEQ_SHIM_FLAG		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 42)
#define SVT_SEQ_SHIM_FLAG		VFT_BOOL
#define SVS_SEQ_SHIM_FLAG		"SHIMMING SEQUENCE FLAG"
/*	シミング処理用のシーケンスかどうかを判別するためのフラグ。
	True  :  シミング処理用シーケンス。
	False :  その他。
	*/

#define SVN_SEQ_ACQ_MATRIX	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 43)
#define SVT_SEQ_ACQ_MATRIX	VFT_FLTVEC2
#define SVS_SEQ_ACQ_MATRIX	"acquire matrix size preset by prolim"
/*	AFI スキャンのための拡張。
	AFIによるエコーカット(RO)、位相エンコードステップの削減量
	を、seqgenにインターフェイスするためのパラメータ。
	パルスシーケンス内部では、vf.roAcq, vf.peAcq （既存）にコピー
	されて使われる。
	位相エンコード方向については実際に、FEPで収集されるデータ量に
	対応する。
	高速スキャンについても、SVN_PLN_NUM_ECHO_FACTOR_SHOTS*
	SVN_PLN_NUM_ECHO_FACTOR にて誘導される
	２次的なパラメータとして設定される。
	*/

#define SVN_SEQ_SHIM_PATH		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 44)
#define SVT_SEQ_SHIM_PATH		VFT_PATHELEM
#define SVS_SEQ_SHIM_PATH		"Shiiming data path"
/*	調整処理が存在している場合、
	磁場分布調整データパスをPQMがprotocol nodeに
	記述する。そのパスに格納されている情報を
	脂肪抑制を行なう本収集の際は、Seqgenが読み出し
	オフセットをのせる。
	*/

#define SVN_SEQ_FLUORO_FLAG		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 45)
#define SVT_SEQ_FLUORO_FLAG		VFT_BOOL
#define SVS_SEQ_FLUORO_FLAG		"Fluoro sequence flag"
/*	Fluoro用のシーケンスかどうかを判別するためのフラグ。
	True  :  Fluroro用シーケンス。
	False :  その他。
	*/

#define SVN_SEQ_SAR_TOTAL_TIME		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 46)
#define SVT_SEQ_SAR_TOTAL_TIME		VFT_FLT32
#define SVS_SEQ_SAR_TOTAL_TIME		"SAR Total Time"
/*	SARの計算対象時間。通常シーケンスではTRであり、FFEではTI+SegmentTime
	となる。
	*/

#define SVN_SEQ_SAR_FLIP_FACTOR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 47)
#define SVT_SEQ_SAR_FLIP_FACTOR		VFT_INT32
#define SVS_SEQ_SAR_FLIP_FACTOR		"SAR Flip Pulse Factor"
/*	SAR計算のための Flip Pulse Power Factor */

#define SVN_SEQ_SAR_FLOP_FACTOR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 48)
#define SVT_SEQ_SAR_FLOP_FACTOR		VFT_INT32
#define SVS_SEQ_SAR_FLOP_FACTOR		"SAR Flop Pulse Factor"
/*	SAR計算のための Flip Pulse Power Factor */

#define SVN_SEQ_SAR_PSAT_FACTOR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 49)
#define SVT_SEQ_SAR_PSAT_FACTOR		VFT_INT32
#define SVS_SEQ_SAR_PSAT_FACTOR		"SAR Presat Pulse Factor"
/*	SAR計算のための Flop Pulse Power Factor */

#define SVN_SEQ_SAR_SORS_FACTOR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 50)
#define SVT_SEQ_SAR_SORS_FACTOR		VFT_INT32
#define SVS_SEQ_SAR_SORS_FACTOR		"SAR SORS Pulse Factor"
/*	SAR計算のための SORS Pulse Power Factor */

#define SVN_SEQ_SAR_INV_FACTOR		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 51)
#define SVT_SEQ_SAR_INV_FACTOR		VFT_INT32
#define SVS_SEQ_SAR_INV_FACTOR		"SAR Inversion Pulse Factor"
/*	SAR計算のための Inversion Pulse Power Factor */

#define SVN_SEQ_SAR_FATSAT_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 52)
#define SVT_SEQ_SAR_FATSAT_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_FATSAT_FACTOR	"SAR Fatsat Pulse Factor"
/*	SAR計算のための Fat saturation Pulse Power Factor */

#define SVN_SEQ_SAR_MTCSAT_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 53)
#define SVT_SEQ_SAR_MTCSAT_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_MTCSAT_FACTOR	"SAR MTC Pulse Factor"
/*	SAR計算のための MTC Pulse Power Factor */

#define SVN_SEQ_SAR_FH_TAG_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 54)
#define SVT_SEQ_SAR_FH_TAG_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_FH_TAG_FACTOR	"SAR FH Tag Pulse Factor"
/*	SAR計算のための Freehand Tag Pulse Power Factor */

#define SVN_SEQ_SAR_GRID_TAG_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 55)
#define SVT_SEQ_SAR_GRID_TAG_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_GRID_TAG_FACTOR	"SAR Grid Tag Pulse Factor"
/*	SAR計算のための Grid Tag Pulse Power Factor */

#define SVN_SEQ_SAR_WATERSAT_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 56)
#define SVT_SEQ_SAR_WATERSAT_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_WATERSAT_FACTOR	"SAR WATERSAT Pulse Factor"
/*	SAR計算のための WATERSAT Pulse Power Factor */


#define SVN_SEQ_HR_SHIM_PATH		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 57)
#define SVT_SEQ_HR_SHIM_PATH		VFT_PATHELEM
#define SVS_SEQ_HR_SHIM_PATH		"High Resolution Shiiming data path"
/*	高分解能磁場分布調整データパスをPQMがprotocol nodeに
	記述する。
	*/

#define SVN_SEQ_SAR_BBINV_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 58)
#define SVT_SEQ_SAR_BBINV_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_BBINV_FACTOR	"SAR BB Inversion Pulse Factor"
/*	SAR計算のための Black Blood Inversion Pulse Power Factor */


#define SVN_SEQ_FLY_FLAG	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 59)
#define SVT_SEQ_FLY_FLAG	VFT_INT32
#define SVS_SEQ_FLY_FLAG	"on the fly flag"
/*	このシーケンスがon the fly可能かどうかを示すフラグ
	on the fly可能ならば、 1
	on the fly不可ならば、 0
*/

#define SVN_SEQ_MPG_APPLY_ORDER	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 60)
#define SVT_SEQ_MPG_APPLY_ORDER	VFT_INT32
#define SVS_SEQ_MPG_APPLY_ORDER	"MPG pulese appling order at the diffusion 3channel simultaneous acquirsition"
/*	3軸同時収集において、Reference及びPhase、Read、Sliceの各方向に
	MPGを印加した画像が、どの順番で収集されたかを示すテーブル。
	このテーブルには、収集した順番にMPGを印加した方向が、以下に示す
	数値で指定されているものとする。
	VFC_IVIM_ACQUIRE_REF    1
	VFC_IVIM_ACQUIRE_PE     2
	VFC_IVIM_ACQUIRE_RO     4
	VFC_IVIM_ACQUIRE_SS     8
	
	設定者:	seqgen
	
	This field indicates the MPG pulese appling order at
	the diffusion weighted 3channel simultaneous acquirsition
	*/

#define SVN_SEQ_SAR_PREC_FLIP_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 61)
#define SVT_SEQ_SAR_PREC_FLIP_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_PREC_FLIP_FACTOR	"SAR PreC Flip Pulse Factor"
/*	SAR計算のための PreC Flip Pulse Power Factor */

#define SVN_SEQ_SAR_PREC_FLOP_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 62)
#define SVT_SEQ_SAR_PREC_FLOP_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_PREC_FLOP_FACTOR	"SAR PreC Flop Pulse Factor"
/*	SAR計算のための PreC Flip Pulse Power Factor */

#define SVN_SEQ_NUM_DRKS	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 63)
#define SVT_SEQ_NUM_DRKS	VFT_INT32
#define SVS_SEQ_NUM_DRKS	"Number of DRKS"
/*	DRKS(=differential-rate K-space sampling)撮影(ダイナミック撮影で、K空
	間中心付近の収集間隔をそれ以外部分より短くすることによって、擬似的に
	時間分解能を上げる方法)において、収集間隔を短くする部分のエンコード数
	の全体に占める割合の逆数。

	たとえば、SVN_SEQ_NUM_DRKS=4,SE方向のマトリクス数=32の時、32*1/
	4にあたる8スライスエンコードは、ほかの部分に比べ収集間隔が短い。
*/

#define SVN_SEQ_NSSIR_PULSE_NAME	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 64)
#define SVT_SEQ_NSSIR_PULSE_NAME	VFT_I32COMPLEX
#define SVS_SEQ_NSSIR_PULSE_NAME	"nssIR pulse waveform name"
/*	nssIRパルスで使用するSEP波形名。

*/

#define SVN_SEQ_NSSIR_PULSE_PITCH	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 65)
#define SVT_SEQ_NSSIR_PULSE_PITCH	VFT_FLT32
#define SVS_SEQ_NSSIR_PULSE_PITCH	"nssIR pulse pitch"
/*	nssIRパルスでのパルスピッチ

*/

#define SVN_SEQ_LOOP_ORDER      (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 66)
#define SVT_SEQ_LOOP_ORDER      VFT_INT32
#define SVS_SEQ_LOOP_ORDER      "Acquisition loop order for FISP3D"
/*      FISP3Dにおける収集順序
#define VFC_LOOP_PE_SE 0 (PE->SE)
#define VFC_LOOP_SE_PE 1 (SE->PE)


*/

#define SVN_SEQ_SNR_PREDICTOR   (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 67)
#define SVT_SEQ_SNR_PREDICTOR   VFT_FLT32
#define SVS_SEQ_SNR_PREDICTOR   "Signal to Noise Ratio predictor"
/*      rSNR予測値
*/

#define SVN_SEQ_BAND_WIDTH      (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 68)
#define SVT_SEQ_BAND_WIDTH      VFT_FLT32
#define SVS_SEQ_BAND_WIDTH      "Band width"
/*      バンド幅（単位 Hz）
*/

#define SVN_SEQ_SSFP_PRE_PULSE_FLAG     (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 69)
#define SVT_SEQ_SSFP_PRE_PULSE_FLAG     VFT_INT32
#define SVS_SEQ_SSFP_PRE_PULSE_FLAG     "SSFP preFlip pulse On/Off Flag"
/*      (1)SSFP用preFlipパルスで使用フラグ
(2)prolimがセットし、seqgenが参照する。
*/

#define SVN_SEQ_SSFP_PRE_PULSE_NAME     (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 70)
#define SVT_SEQ_SSFP_PRE_PULSE_NAME     VFT_I32COMPLEX
#define SVS_SEQ_SSFP_PRE_PULSE_NAME     "SSFP preFlip pulse waveform name"
/*      (1)SSFP用preFlipパルスで使用するＳＥＰ波形名
(2)prolimがセットし、seqgenが参照する。
*/


#define SVN_SEQ_SSFP_POST_PULSE_FLAG    (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 71)
#define SVT_SEQ_SSFP_POST_PULSE_FLAG    VFT_INT32
#define SVS_SEQ_SSFP_POST_PULSE_FLAG    "SSFP postFlip pulse On/Off Flag"
/*      (1)SSFP用preFlipパルスで使用フラグ
(2)prolimがセットし、seqgenが参照する。
*/

#define SVN_SEQ_SSFP_POST_PULSE_NAME    (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 72)
#define SVT_SEQ_SSFP_POST_PULSE_NAME    VFT_I32COMPLEX
#define SVS_SEQ_SSFP_POST_PULSE_NAME    "SSFP postFlip pulse waveform name"
/*      (1)SSFP用postFlipパルスで使用するＳＥＰ波形名
(2)prolimがセットし、seqgenが参照する。
*/

#define SVN_SEQ_DEFAULT_RGN_VALUE       (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 73)
#define SVT_SEQ_DEFAULT_RGN_VALUE       VFT_FLT32
#define SVS_SEQ_DEFAULT_RGN_VALUE       "Default Receiver Gain value (dB)"
/*  prolimがセットして pqm or acqmanが使う。
    RGNしないときに使うRGN値/RGN収集する時の開始RGN値
*/

#define SVN_SEQ_DBDT_MODE               (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 74)
#define SVT_SEQ_DBDT_MODE               VFT_INT32
#define SVS_SEQ_DBDT_MODE               "dBdt mode of each sequences"
/*  (1) dBdtに関するシーケンス毎の動作モードを設定
        高dBdtシーケンスモード,自主規制モード,通常モードがある。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_DBDT_IEC_OPERATING_MODE (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 75)
#define SVT_SEQ_DBDT_IEC_OPERATING_MODE VFT_INT32
#define SVS_SEQ_DBDT_IEC_OPERATING_MODE               "dBdt IEC operating mode"
/*  (1) IECの規格で規定されるdBdt管理操作モード。
       a.通常操作モード           (VFT_DBDT_IEC_OPERATING_NORMAL)
       b.第１次水準管理操作モード (VFT_DBDT_IEC_OPERATING_1ST_CONTROLLED)
       c.第２次水準管理操作モード (VFT_DBDT_IEC_OPERATING_2ND_CONTROLLED)
    (2) pqm, seqgenがセットする。
*/

#define SVN_SEQ_DBDT_VALUE              (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 76)
#define SVT_SEQ_DBDT_VALUE              VFT_FLT32
#define SVS_SEQ_DBDT_VALUE              "dBdt Value"
/*  (1) 計算されたdBdt値。
        [0] : seqgenで計算した値
        [1] : GRCで計算された値
    (2) acqman, seqgenがセットする。
*/

#define SVN_SEQ_DBDT_RO_RES_LIMIT1      (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 77)
#define SVT_SEQ_DBDT_RO_RES_LIMIT1      VFT_FLT32
#define SVS_SEQ_DBDT_RO_RES_LIMIT1      "dBdt self restricted mode1 RO_resolution limit"
/*  (1) 自己規制モード１において, RO方向の許容限界分解能を[m]単位で指定する。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_DBDT_RO_DIR_LIMIT2     (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 78)
#define SVT_SEQ_DBDT_RO_DIR_LIMIT2     VFT_INT32
#define SVS_SEQ_DBDT_RO_DIR_LIMIT2     "dBdt self restricted mode2 RO_direction limit"
/*  (1) 自己規制モード２において, 撮影可能なRO方向を指定する。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_LOCAL_SYSPARAMS         (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 79)
#define SVT_SEQ_LOCAL_SYSPARAMS         VFT_FLTVEC3
#define SVS_SEQ_LOCAL_SYSPARAMS         "local slew rate"
/*  (1) 傾斜磁場情報。
        [0]:s+(x,y,z) [1]:s-(x,y,z) [2]:dt(x,y,z)
	s:そのchの1msあたりの傾斜磁場値変化量[h@1ms]
	dt:delay time 傾斜磁場の遅れ[us]
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_SAR_SSFP_POST_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 80)
#define SVT_SEQ_SAR_SSFP_POST_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_SSFP_POST_FACTOR	"SAR SSFP Post Pulse Factor"
/*	SAR計算のための SSFP Post Pulse Power Factor */

#define SVN_SEQ_SAR_SSFP_PRE_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 81)
#define SVT_SEQ_SAR_SSFP_PRE_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_SSFP_PRE_FACTOR	"SAR SSFP Pre Pulse Factor"
/*	SAR計算のための SSFP Pre Pulse Power Factor */

#define SVN_SEQ_SAR_NSSIR_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 82)
#define SVT_SEQ_SAR_NSSIR_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_NSSIR_FACTOR	"SAR Nss IR Pulse Factor"
/*	SAR計算のための Nss IR Pulse Power Factor */

#define SVN_SEQ_SAR_PROBE_FLIP_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 83)
#define SVT_SEQ_SAR_PROBE_FLIP_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_PROBE_FLIP_FACTOR	"SAR Probe Flip Pulse Factor"
/*	SAR計算のための Probe Flip Pulse Power Factor */

#define SVN_SEQ_SAR_PROBE_FLOP_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 84)
#define SVT_SEQ_SAR_PROBE_FLOP_FACTOR	VFT_INT32
#define SVS_SEQ_SAR_PROBE_FLOP_FACTOR	"SAR Probe Flop Pulse Factor"
/*	SAR計算のための Probe Flop Pulse Power Factor */

#define SVN_SEQ_RGN_PERCENT_ADC		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 85)
#define SVT_SEQ_RGN_PERCENT_ADC		VFT_FLT32
#define SVS_SEQ_RGN_PERCENT_ADC		"RGN coefficient"
/*	RGN収集にてRGN値を決定する際にacqManが用いる計数。
	有効範囲0.0以上,1.0以下。
	値が0.0もしくは有効範囲外の場合は,従来通り 
	/gp/lang/english/app-defaults/AcqMan から読み込む */

#define SVN_SEQ_RAMP_SAMPL_METHOD	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 86)
#define SVT_SEQ_RAMP_SAMPL_METHOD	VFT_INT32
#define SVS_SEQ_RAMP_SAMPL_METHOD	"ramp sampling method"
/*  (1) 読出し傾斜磁場の立上り立下りでサンプリングを行うためのフラグ。
        [0]:0 ramp samplingしない       1 不等間隔samplにより実現。
            2 等間隔とregridにより実現(分解能優先)  
            4 等間隔とregridにより実現(SNR重視)
	[1]:ramp samplingにより追加された収集点数
    (2) seqgenがセットする。*/


#define SVN_SEQ_DBDT_MODE_2002               (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 87)
#define SVT_SEQ_DBDT_MODE_2002               VFT_INT32
#define SVS_SEQ_DBDT_MODE_2002               "2nd dBdt mode of each sequences"
/*  (1) 2nd dBdtに関するシーケンス毎の動作モードを設定
        高dBdtシーケンスモード,自主規制モード,通常モードがある。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_DBDT_RO_RES_LIMIT1_2002      (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 88)
#define SVT_SEQ_DBDT_RO_RES_LIMIT1_2002      VFT_FLT32
#define SVS_SEQ_DBDT_RO_RES_LIMIT1_2002      "2nd dBdt self restricted mode1 RO_resolution limit"
/*  (1) 2nd dBdt 自己規制モード１において, RO方向の許容限界分解能を[m]単位で指定する。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_DBDT_RO_DIR_LIMIT2_2002     (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 89)
#define SVT_SEQ_DBDT_RO_DIR_LIMIT2_2002     VFT_INT32
#define SVS_SEQ_DBDT_RO_DIR_LIMIT2_2002     "2nd dBdt self restricted mode2 RO_direction limit"
/*  (1) 2nd dBdt 自己規制モード２において, 撮影可能なRO方向を指定する。
    (2) prolimがセットし、pqmが参照する。
*/

#define SVN_SEQ_DBDT_IEC_EDITION	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 90)
#define SVT_SEQ_DBDT_IEC_EDITION	VFT_INT32
#define SVS_SEQ_DBDT_IEC_EDITION	"dBdt IEC Editio"
/*  (1) IEC Edition
       a.1st Edition             (VFT_DBDT_IEC_EDITION_1995)
       b.2nd Edition             (VFT_DBDT_IEC_EDITION_2002)
    (2) acqmanがセットして、seqgenが参照する。
*/

#define SVN_SEQ_DBDT_CALC_MODE (SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 91)
#define SVT_SEQ_DBDT_CALC_MODE VFT_INT32
#define SVS_SEQ_DBDT_CALC_MODE "2nd dBdt IEC operating mode"
/*  (1) dBdt計算モード
       a.CSR方式(1st/2nd両方）     (VFT_DBDT_CALC_MODE_CSR)
       b.MIN_DT付きCSR(2nd）       (VFT_DBDT_CALC_MODE_CSR_MIN_DT)
       c.CRT方式(2nd）             (VFT_DBDT_CALC_MODE_CRT)
    (2) prolimがセットして pqm, seqgen,prolimが参照する。
*/

#define SVN_SEQ_TEMPB0_CORR_TE	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 92)
#define SVT_SEQ_TEMPB0_CORR_TE	VFT_FLT32
#define SVS_SEQ_TEMPB0_CORR_TE	"Echo time for inline echo (second)"
/*  インラインエコーのためのエコー時間（秒）
*/

#define SVN_SEQ_TEMPB0_CORR_ECHOES	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 93)
#define SVT_SEQ_TEMPB0_CORR_ECHOES	VFT_INT32
#define SVS_SEQ_TEMPB0_CORR_ECHOES	"number of inline echo for B0 shift correction (arbitrary)"
/*  B0シフトのためのインラインエコー数（任意）
*/

#define SVN_SEQ_TEMPB0_CORR_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 94)
#define SVT_SEQ_TEMPB0_CORR_FACTOR	VFT_FLT32
#define SVS_SEQ_TEMPB0_CORR_FACTOR	"B0 shift correction coefficient (On/Off) (arbitrary)"
/*  B0シフト係数
*/

#define SVN_SEQ_TEMPB0_CORR_ENABLE_FLAG		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 95)
#define SVT_SEQ_TEMPB0_CORR_ENABLE_FLAG		VFT_INT32
#define SVS_SEQ_TEMPB0_CORR_ENABLE_FLAG		"B0 shift correction enable for this sequence(0/1)"
/*  	シーケンスがB0シフト補正対象かどうかのFlag
	0:B0シフト補正対象ではない
	1:B0シフト補正対象

	コメント
		int　Commonで０にしておく　PROLIM変数と連動
		書くProcess：Prolimだけ　Commonでは0、特定のFE_EPIは1
		読むProcess：ProMui、Mui
*/

#define SVN_SEQ_TEMPB0_CORR_FLAG		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 96)
#define SVT_SEQ_TEMPB0_CORR_FLAG		VFT_INT32
#define SVS_SEQ_TEMPB0_CORR_FLAG		"B0 shift correction execute flag in this protocol(0/1)"
/*  	B0シフト補正を行うかどうかのFlag
	0:B0シフト補正を実行しない
	1:B0シフト補正を実行する

	コメント
		B0シフト処理をするかどうか　0:Off、1:On
		int　seqgenが書き込む
*/

#define SVN_SEQ_TEMPB0_CORR_PHASE_BASE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 97)
#define SVT_SEQ_TEMPB0_CORR_PHASE_BASE		VFT_INT32
#define SVS_SEQ_TEMPB0_CORR_PHASE_BASE		"B0 shift correction Base Dynamic Phase(1-origin)"
/*  	B0シフト補正を行う時の基準時相(1-origin)	

	コメント
		B0シフト処理の基準時相　1-origin(現状仕様の踏襲)
		int　seqgenが書き込む　デフォルト0
*/

#define SVN_SEQ_WATEREXCIT_FLIP_PULSE_PITCH	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 98)
#define SVT_SEQ_WATEREXCIT_FLIP_PULSE_PITCH	VFT_FLT32
#define SVS_SEQ_WATEREXCIT_FLIP_PULSE_PITCH	"Water Excitation Pulse Flip Pitch"
/*      Water Excitation Pulse Flip Pitch
*/

#define SVN_SEQ_PHASE_CORRECTION_PRESCAN_METHOD	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 99)
#define SVT_SEQ_PHASE_CORRECTION_PRESCAN_METHOD	VFT_INT32
#define SVS_SEQ_PHASE_CORRECTION_PRESCAN_METHOD	"Phase correction prescan method"
/*	phase correction prescan method
	1)PROLIMが位相補正プリスキャン時に0を書き込む
	2)seqgenが上書きして設定する(3エコーモード、5エコーモード)

	3エコーモード	(VFC_PHASE_CORRECTION_PRESCAN_FSE_THREE_ECHO_MODE)
	5エコーモード	(VFC_PHASE_CORRECTION_PRESCAN_FSE_FIVE_ECHO_MODE)
*/

#define SVN_SEQ_FEP_PHASE_CORRECTION_PRESCAN	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 100)
#define SVT_SEQ_FEP_PHASE_CORRECTION_PRESCAN	VFT_STRING
#define SVS_SEQ_FEP_PHASE_CORRECTION_PRESCAN	"Name of FEP data for phase correction prescan"
/*	Name of FEP data for phase correction prescan
*/

#define SVN_SEQ_SAR_AVERAGE_FACTOR	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 101)
#define SVT_SEQ_SAR_AVERAGE_FACTOR	VFT_FLT32
#define SVS_SEQ_SAR_AVERAGE_FACTOR	"SAR Average Factor"
/*	SAR average factor is calculated by using default APC.
	Actual SAR is calculated by multiplying by the ratio of Acquired APC to
	Default APC.
	*/

#define SVN_SEQ_DEFAULT_RFL_GAIN_VALUE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 102)
#define SVT_SEQ_DEFAULT_RFL_GAIN_VALUE		VFT_FLT32
#define SVS_SEQ_DEFAULT_RFL_GAIN_VALUE		"Default RFL Gain"
/*	Default APC Gain value
	*/

#define SVN_SEQ_DEFAULT_RFL_SCALE_VALUE		(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 103)
#define SVT_SEQ_DEFAULT_RFL_SCALE_VALUE		VFT_FLT32
#define SVS_SEQ_DEFAULT_RFL_SCALE_VALUE		"Default RFL Scaler"
/*	Default APC Scale value
	*/

#define SVN_SEQ_NUM_SHOTS_PHASE_CORRECTION_PRESCAN	(SVN_MIN_PROTOCOL+SVN_OFF_SEQUENCE + 104)
#define SVT_SEQ_NUM_SHOTS_PHASE_CORRECTION_PRESCAN	VFT_INT32
#define SVS_SEQ_NUM_SHOTS_PHASE_CORRECTION_PRESCAN	"Number of shots for phase correction prescan"
/*	Number of shots for phase correction prescan
	setting     seqgen
	reference   acqman
	*/

/*****************************************************************************
  Subprotocol name space
*/

#define SVN_SEQ_REC_SUBCOIL	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 0)
#define SVT_SEQ_REC_SUBCOIL	VFT_INT32
#define SVS_SEQ_REC_SUBCOIL	"Element of Rec. Coil used in this subprotocol"
/*	This field is the number of the element of the receiver coil used
	to acquire this subprotocol.  This field is determined by the
	sequences and depends on the relative locations of the patient,
	specified slices and the coil.
	*/

#define SVN_SEQ_FEP_DATA	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 1)
#define SVT_SEQ_FEP_DATA	VFT_FEP
#define SVS_SEQ_FEP_DATA	"FEP data for imaging"
/*	This field is the information about how the FEP massages the data.
	It is needed by RBControl for sorting purposes.  It is written by
	the sequence.
	*/

#define	SVN_SEQ_LINK_IMAGE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 2)
#define SVT_SEQ_LINK_IMAGE	VFT_STRING
#define SVS_SEQ_LINK_IMAGE	"Name of link command for imaging"
/*	This field is the name of the link command to be used for imaging
	for this protocol.  It is determined by the support equations.
	*/

#define	SVN_SEQ_FEP_IMAGE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 3)
#define	SVT_SEQ_FEP_IMAGE	VFT_STRING
#define	SVS_SEQ_FEP_IMAGE	"Name of FEP data for imaging"
/*	This field is the name of the FEP data to be used for imaging
	for this protocol.  It is determined by the support equations.
	*/

#define	SVN_SEQ_SEQUENCE_IMAGE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 4)
#define SVT_SEQ_SEQUENCE_IMAGE 	VFT_STRING
#define SVS_SEQ_SEQUENCE_IMAGE 	"Sequence for imaging, including path"
/*	This field is the complete path to the seqgenned imaging sequence
	that is to be executed by AcqMan.  It is determined by the sequence.
	*/

#define SVN_SEQ_SAR		(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 5)
#define SVT_SEQ_SAR		VFT_FLT32
#define SVS_SEQ_SAR		"Specific Absorption Rate (SAR)"
/*	This field is the SAR for this protocol in Watts/kg.
	*/

#define SVN_SEQ_RO_CHANNEL_POLARITY  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 6)
#define SVT_SEQ_RO_CHANNEL_POLARITY  VFT_INT32
#define SVS_SEQ_RO_CHANNEL_POLARITY  "Readout channel overall polarity"
/*	This is an overall polarity applied to the entire readout channel for
	the purpose of reducing gradient overloading. The image flip in the
	read direction is determined by the combination of this field and the
	read polarity defined at the image level.  This field is computed by
	ProSel support equations. The values for this field are defined in
	vf_appcodes.h.
	*/

#define SVN_SEQ_PE_CHANNEL_POLARITY  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 7)
#define SVT_SEQ_PE_CHANNEL_POLARITY  VFT_INT32
#define SVS_SEQ_PE_CHANNEL_POLARITY  "Phase encode channel overall polarity"
/*	This is an overall polarity applied to the entire phase encode
	channel, excluding the phase encode pulse,  for the purpose of
	reducing gradient overloading. This field is computed by ProSel
	support equations. The values for this field are defined in
	vf_appcodes.h.
	*/

#define SVN_SEQ_FLIP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 8)
#define SVT_SEQ_FLIP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_FLIP_PULSE_SCALER  "Excitation pulse scaler"
/*	The excitation pulse scaler computed by seqgen from prescan data.
	*/

#define SVN_SEQ_FLIP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 9)
#define SVT_SEQ_FLIP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_FLIP_PULSE_PITCH  "Excitation pulse pitch"
/*	The excitation pulse pitch, in hardware units, computed by seqgen.
	*/

#define SVN_SEQ_FLOP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 10)
#define SVT_SEQ_FLOP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_FLOP_PULSE_SCALER  "Refocusing pulse scaler"
/*	The refocusing pulse scaler computed by seqgen from prescan data.
	*/

#define SVN_SEQ_FLOP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 11)
#define SVT_SEQ_FLOP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_FLOP_PULSE_PITCH  "Refocusing pulse pitch"
/*	The refocusing pulse pitch, in hardware units, computed by seqgen.
	*/

#define SVN_SEQ_SAT_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 12)
#define SVT_SEQ_SAT_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_SAT_PULSE_SCALER  "Saturation pulse scaler"
/*	The saturation pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_SAT_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 13)
#define SVT_SEQ_SAT_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_SAT_PULSE_PITCH  "Saturation pulse pitch"
/*	The saturation pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_IMG_GAIN_VALUE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 14)
#define SVT_SEQ_IMG_GAIN_VALUE 	VFT_FLT32
#define SVS_SEQ_IMG_GAIN_VALUE	"Imaging RF Tx gain value (dB)"
/* 	Tx gain value for imaging. It is computed by seqgen from RF Level
 	measurements. */

#define SVN_SEQ_SAT_GAIN_VALUE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 15)
#define SVT_SEQ_SAT_GAIN_VALUE 	VFT_FLT32
#define SVS_SEQ_SAT_GAIN_VALUE	"Saturation RF Tx gain value (dB)"
/* 	Tx gain value for saturation pulse. It is computed by seqgen from RF
	Level measurements. */

#define SVN_SEQ_INV_GAIN_VALUE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 16)
#define SVT_SEQ_INV_GAIN_VALUE 	VFT_FLT32
#define SVS_SEQ_INV_GAIN_VALUE	"Inversion RF Tx gain value (dB)"
/* 	Tx gain value for inversion pulse. It is computed by seqgen from RF
	Level measurements. */

#define	SVN_SEQ_ANONYMOUS_DATA (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 17)
#define	SVT_SEQ_ANONYMOUS_DATA	VFT_FLT32
#define	SVS_SEQ_ANONYMOUS_DATA	"Anonymous sequence data"
/*	This is an array of floating point data used privately by the sequence.
 	The meaning of the data is interpreted entirely by the sequence. 
	*/

#define SVN_SEQ_INV_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 18)
#define SVT_SEQ_INV_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_INV_PULSE_SCALER  "Inversion pulse scaler"
/*	The inversion pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_INV_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 19)
#define SVT_SEQ_INV_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_INV_PULSE_PITCH  "Inversion pulse pitch"
/*	The inversion pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_FLIP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 20)
#define SVT_SEQ_FLIP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_FLIP_PULSE_NAME  "Excitation pulse waveform name"
/*	フリップパルスで使用するＳＥＰ波形名。
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_FLOP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 21)
#define SVT_SEQ_FLOP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_FLOP_PULSE_NAME  "Refocusing pulse waveform name"
/*	フロップパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_SAT_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 22)
#define SVT_SEQ_SAT_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_SAT_PULSE_NAME  "Saturation pulse waveform name"
/*      サチュレーションパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_INV_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 23)
#define SVT_SEQ_INV_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_INV_PULSE_NAME  "Inversion pulse waveform name"
/*	インバージョンパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_FATSAT_GAIN_VALUE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 24)
#define SVT_SEQ_FATSAT_GAIN_VALUE 	VFT_FLT32
#define SVS_SEQ_FATSAT_GAIN_VALUE	"Fat suppress RF Tx gain value (dB)"
/* 	Tx gain value for fat suppress pulse. It is computed by seqgen from RF
	Level measurements. */

#define SVN_SEQ_FATSAT_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 25)
#define SVT_SEQ_FATSAT_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_FATSAT_PULSE_SCALER  "Fat suppress pulse scaler"
/*	The fat suppress pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_FATSAT_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 26)
#define SVT_SEQ_FATSAT_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_FATSAT_PULSE_PITCH  "Fat suppress pulse pitch"
/*	The fat suppress pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_FATSAT_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 27)
#define SVT_SEQ_FATSAT_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_FATSAT_PULSE_NAME  "Fat suppress pulse waveform name"
/*	ファットサプレスパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_PREAL		(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 28)
#define SVT_SEQ_PREAL		VFT_FLT32
#define SVS_SEQ_PREAL		"Power dissipation of coil"
/*	コイル保護に使用するパラメータ。ＳＡＲの計算過程で同時に算出される
	ＲＦパワーのコイル損失分。
	*/

#define SVN_SEQ_ACQUIRED_SIZE	 (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 29)
#define SVT_SEQ_ACQUIRED_SIZE	 VFT_INT32
#define SVS_SEQ_ACQUIRED_SIZE	 "Acquired data size in bytes"
/* 	This field is the acquired data size as calculated by seqgen.
	AcqMan will read this field for allocating space for acquired data.
	*/

#define SVN_SEQ_RF_POWER_FACTOR	 (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 30)
#define SVT_SEQ_RF_POWER_FACTOR	 VFT_FLT32
#define SVS_SEQ_RF_POWER_FACTOR	 "RF power factor for a scan"
/* 	This is the RF power factor for a scan computed by seqgen.  It is the
	wave-squared-integal per unit time.
	*/

#define	SVN_SEQ_RF_GRADS_OFF_FLAGS (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 31)
#define	SVT_SEQ_RF_GRADS_OFF_FLAGS VFT_BITFIELD
#define	SVS_SEQ_RF_GRADS_OFF_FLAGS "RF and gradient channel off flags"
/*	This is an optional field to turn off RF and/or any gradient channels.
	It only works for sequences that read it.  This field is normally
	written by mrtools.  This is a bit field. The bits are defined in
	vf_appcodes.h.
	*/

#define SVN_SEQ_SORS_GAIN_VALUE (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 32)
#define SVT_SEQ_SORS_GAIN_VALUE VFT_FLT32
#define SVS_SEQ_SORS_GAIN_VALUE	"SORS pulse RF Tx gain value (dB)"
/* 	Tx gain value for SORS pulse. It is computed by seqgen from RF
	Level measurements. */

#define SVN_SEQ_SORS_PULSE_SCALER (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 33)
#define SVT_SEQ_SORS_PULSE_SCALER VFT_FLT32
#define SVS_SEQ_SORS_PULSE_SCALER "SORS pulse scaler"
/*	The SORS pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_SORS_PULSE_PITCH (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 34)
#define SVT_SEQ_SORS_PULSE_PITCH VFT_FLT32
#define SVS_SEQ_SORS_PULSE_PITCH "SORS pulse pitch"
/*	The SORS pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_SORS_PULSE_NAME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 35)
#define SVT_SEQ_SORS_PULSE_NAME VFT_I32COMPLEX
#define SVS_SEQ_SORS_PULSE_NAME "SORS pulse waveform name"
/*	ソースパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_AVERAGING_TIME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 36)
#define SVT_SEQ_AVERAGING_TIME VFT_FLT32
#define SVS_SEQ_AVERAGING_TIME "Averaging Time"
/*	アベレージングループの内側の時間。フルオロ時に
	RMで使用するパラメータ。seqgen が値を計算し、スタディ
	ファイルにセットする。
	単位：sec
	*/

#define SVN_SEQ_MTCSAT_PULSE_NAME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 37)
#define SVT_SEQ_MTCSAT_PULSE_NAME VFT_I32COMPLEX
#define SVS_SEQ_MTCSAT_PULSE_NAME "Name for MTC saturation pulses"
/*	MTCパルス名。
	*/

#define SVN_SEQ_MTCSAT_PULSE_PITCH (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 38)
#define SVT_SEQ_MTCSAT_PULSE_PITCH VFT_FLT32
#define SVS_SEQ_MTCSAT_PULSE_PITCH "RF pulse pitch for MTC saturation pulses"
/*	MTCパルスのピッチ。
	*/

#define SVN_SEQ_MTCSAT_GAIN_VALUE (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 39)
#define SVT_SEQ_MTCSAT_GAIN_VALUE VFT_FLT32
#define SVS_SEQ_MTCSAT_GAIN_VALUE "RF pulse transmit gain for MTC saturation pulses"
/*	MTCパルスのトランスミッタゲイン。
	*/

#define SVN_SEQ_MTCSAT_PULSE_SCALER (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 40)
#define SVT_SEQ_MTCSAT_PULSE_SCALER VFT_FLT32
#define SVS_SEQ_MTCSAT_PULSE_SCALER "RF pulse transmit scaler for MTC saturation pulses"
/*	MTCパルスのトランスミッタScaler。
	*/

#define SVN_SEQ_GRID_TAG_PULSE_NAME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 41)
#define SVT_SEQ_GRID_TAG_PULSE_NAME VFT_I32COMPLEX
#define SVS_SEQ_GRID_TAG_PULSE_NAME "Name for Grid-Tag pulses"
/*	Grid-Tagパルス名。
	*/

#define SVN_SEQ_GRID_TAG_PULSE_PITCH (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 42)
#define SVT_SEQ_GRID_TAG_PULSE_PITCH VFT_FLT32
#define SVS_SEQ_GRID_TAG_PULSE_PITCH "RF pulse pitch for Grid-Tag pulses"
/*	Grid-Tagパルスのピッチ。
	*/

#define SVN_SEQ_GRID_TAG_GAIN_VALUE (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 43)
#define SVT_SEQ_GRID_TAG_GAIN_VALUE VFT_FLT32
#define SVS_SEQ_GRID_TAG_GAIN_VALUE "RF pulse transmit gain for Grid-Tag pulses"
/*	Grid-Tagパルスのトランスミッタゲイン。
	*/

#define SVN_SEQ_GRID_TAG_PULSE_SCALER (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 44)
#define SVT_SEQ_GRID_TAG_PULSE_SCALER VFT_FLT32
#define SVS_SEQ_GRID_TAG_PULSE_SCALER "RF pulse transmit scaler for Grid-Tag pulses"
/*	Grid-TagパルスのトランスミッタScaler。
	*/

#define SVN_SEQ_FH_TAG_PULSE_NAME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 45)
#define SVT_SEQ_FH_TAG_PULSE_NAME VFT_I32COMPLEX
#define SVS_SEQ_FH_TAG_PULSE_NAME "Name for Freehand-Tag pulses"
/*	Freehand-Tagパルス名。
	*/

#define SVN_SEQ_FH_TAG_PULSE_PITCH (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 46)
#define SVT_SEQ_FH_TAG_PULSE_PITCH VFT_FLT32
#define SVS_SEQ_FH_TAG_PULSE_PITCH "RF pulse pitch for Freehand-Tag pulses"
/*	Freehand-Tagパルスのピッチ。
	*/

#define SVN_SEQ_FH_TAG_GAIN_VALUE (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 47)
#define SVT_SEQ_FH_TAG_GAIN_VALUE VFT_FLT32
#define SVS_SEQ_FH_TAG_GAIN_VALUE "RF pulse transmit gain for Freehand-Tag pulses"
/*	Freehand-Tagパルスのトランスミッタゲイン。
	*/

#define SVN_SEQ_FH_TAG_PULSE_SCALER (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 48)
#define SVT_SEQ_FH_TAG_PULSE_SCALER VFT_FLT32
#define SVS_SEQ_FH_TAG_PULSE_SCALER "RF pulse transmit scaler for Freehand-Tag pulses"
/*	Freehand-TagパルスのトランスミッタScaler。
	*/

#define SVN_SEQ_WATERSAT_PULSE_NAME (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 49)
#define SVT_SEQ_WATERSAT_PULSE_NAME VFT_I32COMPLEX
#define SVS_SEQ_WATERSAT_PULSE_NAME "Name for WATERSAT pulses"
/*	WATERSATパルス名。
	*/

#define SVN_SEQ_WATERSAT_PULSE_PITCH (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 50)
#define SVT_SEQ_WATERSAT_PULSE_PITCH VFT_FLT32
#define SVS_SEQ_WATERSAT_PULSE_PITCH "RF pulse pitch for WATERSAT pulses"
/*	WATERSATパルスのピッチ。
	*/

#define SVN_SEQ_WATERSAT_GAIN_VALUE (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 51)
#define SVT_SEQ_WATERSAT_GAIN_VALUE VFT_FLT32
#define SVS_SEQ_WATERSAT_GAIN_VALUE "RF pulse transmit gain for WATERSAT pulses"
/*	WATERSATパルスのトランスミッタゲイン。
	*/

#define SVN_SEQ_WATERSAT_PULSE_SCALER (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 52)
#define SVT_SEQ_WATERSAT_PULSE_SCALER VFT_FLT32
#define SVS_SEQ_WATERSAT_PULSE_SCALER "RF pulse transmit scaler for WATERSAT pulses"
/*	WATERSATパルスのトランスミッタScaler。
	*/

#define SVN_SEQ_BBINV_GAIN_VALUE 	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 53)
#define SVT_SEQ_BBINV_GAIN_VALUE 	VFT_FLT32
#define SVS_SEQ_BBINV_GAIN_VALUE	"BB Inversion RF Tx gain value (dB)"
/* 	Tx gain value for inversion pulse. It is computed by seqgen from RF
	Level measurements. */

#define SVN_SEQ_BBINV_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 54)
#define SVT_SEQ_BBINV_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_BBINV_PULSE_SCALER  "BB Inversion pulse scaler"
/*	The inversion pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_BBINV_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 55)
#define SVT_SEQ_BBINV_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_BBINV_PULSE_PITCH  "BB Inversion pulse pitch"
/*	The inversion pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_BBINV_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 56)
#define SVT_SEQ_BBINV_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_BBINV_PULSE_NAME  "Inversion pulse waveform name"
/*	ブラックブラッドインバージョンパルスで使用するＳＥＰ波形名
	ＳＡＲ計算時に波形データを検索するキーとして使用する。
	波形コードは上位１バイトは波形種、下位３バイトを波形コードとする。
	sinc	= 1
	sinc11 = 0x1011
	sinc22 = 0x1022
	sinc32 = 0x1032
	*/

#define SVN_SEQ_PREC_GAIN_VALUE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 57)
#define SVT_SEQ_PREC_GAIN_VALUE	VFT_FLT32
#define SVS_SEQ_PREC_GAIN_VALUE	"Precontrast RF Tx gain value (dB)"
/* 	Tx gain value for precontrasy pulse. It is computed by seqgen from RF
	Level measurements. */

#define SVN_SEQ_PREC_FLIP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 58)
#define SVT_SEQ_PREC_FLIP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_PREC_FLIP_PULSE_SCALER  "Precontrast flip pulse scaler"
/*	The precontrasy pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_PREC_FLOP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 59)
#define SVT_SEQ_PREC_FLOP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_PREC_FLOP_PULSE_SCALER  "Precontrast flop pulse scaler"
/*	The precontrasy pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_PREC_FLIP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 60)
#define SVT_SEQ_PREC_FLIP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_PREC_FLIP_PULSE_PITCH  "Precontrast flip pulse pitch"
/*	The precontrasy pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_PREC_FLOP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 61)
#define SVT_SEQ_PREC_FLOP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_PREC_FLOP_PULSE_PITCH  "Precontrast flop pulse pitch"
/*	The precontrasy pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_PREC_FLIP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 62)
#define SVT_SEQ_PREC_FLIP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_PREC_FLIP_PULSE_NAME  "Precontrast flip pulse waveform name"
/*	プレコントラスト(PreC)パルスでで使用するＳＥＰ波形名
	*/

#define SVN_SEQ_PREC_FLOP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 63)
#define SVT_SEQ_PREC_FLOP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_PREC_FLOP_PULSE_NAME  "Precontrast flop pulse waveform name"
/*	プレコントラスト(PreC)パルスで使用するＳＥＰ波形名
	*/

#define SVN_SEQ_RGN_TABLE_NAME	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 64)
#define SVT_SEQ_RGN_TABLE_NAME	VFT_STRING
#define SVS_SEQ_RGN_TABLE_NAME	"RGN table name(except prefix) of Imaging Sequence"
/*	プレスキャン高速化の本スキャンシーケンスのRMへの先行転送処理で使用。
	seqgenが、作成したImagingSequenceのRxgain tableのテーブル名をセットする。
	ただし、テーブル名の頭のprefix（acqmanが指定する０１などの数値）は含まない。
	AcqManが参照する。
	*/

#define SVN_SEQ_RGN_TABLE_DATA_NUM	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 65)
#define SVT_SEQ_RGN_TABLE_DATA_NUM	VFT_INT32
#define SVS_SEQ_RGN_TABLE_DATA_NUM	"RGN table data number of Imaging Sequence"
/*	プレスキャン高速化の本スキャンシーケンスのRMへの先行転送処理で使用。
	seqgenが、作成したImagingSequenceのRxgain tableのRxgainデータの個数をセットする。
	AcqManが参照する。
	*/

#define SVN_SEQ_RGN_TABLE_DATA	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 66)
#define SVT_SEQ_RGN_TABLE_DATA	VFT_FLT32
#define SVS_SEQ_RGN_TABLE_DATA	"RGN table data of Imaging Sequence"
/*	プレスキャン高速化の本スキャンシーケンスのRMへの先行転送処理で使用。
	seqgenが、作成したImagingSequenceのRxgain tableのRxgainデータをセットする。
	本テーブルにセットされるGxgain値は「ダミーRxgain値」をseqgenが操作(+/-)した値である。
	AcqManが参照する。
	*/

#define SVN_SEQ_NSSIR_PULSE_SCALER	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 67)
#define SVT_SEQ_NSSIR_PULSE_SCALER	VFT_FLT32
#define SVS_SEQ_NSSIR_PULSE_SCALER	"nssIR pulse scaler"
/*	nssIRパルスでのパルススケーラ

*/


#define SVN_SEQ_NSSIR_GAIN_VALUE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 68)
#define SVT_SEQ_NSSIR_GAIN_VALUE	VFT_FLT32
#define SVS_SEQ_NSSIR_GAIN_VALUE	"nssIR pulse TX gain value (dB)"
/*	nssIRパルスでのTXゲイン値

*/

#define SVN_SEQ_SSFP_PRE_GAIN_VALUE     (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 69)
#define SVT_SEQ_SSFP_PRE_GAIN_VALUE     VFT_FLT32
#define SVS_SEQ_SSFP_PRE_GAIN_VALUE     "SSFP preFlip pulse TX gain value (dB)"
/*      (1)SSFP用preFlipパルスでのTXゲイン値
*/

#define SVN_SEQ_SSFP_POST_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 70)
#define SVT_SEQ_SSFP_POST_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_SSFP_POST_PULSE_SCALER  "SSFP postFlip pulse scaler"
/*      (1)SSFP用postFlipパルスでのパルススケーラ
(2)seqgenがセットする。
*/

#define SVN_SEQ_SSFP_POST_GAIN_VALUE    (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 71)
#define SVT_SEQ_SSFP_POST_GAIN_VALUE    VFT_FLT32
#define SVS_SEQ_SSFP_POST_GAIN_VALUE    "SSFP postFlip pulse TX gain value (dB)"
/*      (1)SSFP用postFlipパルスでのTXゲイン値


*/

#define SVN_SEQ_SSFP_PRE_PULSE_SCALER   (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 72)
#define SVT_SEQ_SSFP_PRE_PULSE_SCALER   VFT_FLT32
#define SVS_SEQ_SSFP_PRE_PULSE_SCALER   "SSFP preFlip pulse scaler"
/*      (1)SSFP用preFlipパルスでのパルススケーラ
(2)seqgenがセットする。
*/

#define SVN_SEQ_PROBE_FLIP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 73)
#define SVT_SEQ_PROBE_FLIP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_PROBE_FLIP_PULSE_SCALER  "Probe flip pulse scaler"
/*	The probe pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_PROBE_FLOP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 74)
#define SVT_SEQ_PROBE_FLOP_PULSE_SCALER  VFT_FLT32
#define SVS_SEQ_PROBE_FLOP_PULSE_SCALER  "Probe flop pulse scaler"
/*	The probe pulse scaler computed by seqgen.
	*/

#define SVN_SEQ_PROBE_FLIP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 75)
#define SVT_SEQ_PROBE_FLIP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_PROBE_FLIP_PULSE_PITCH  "Probe flip pulse pitch"
/*	The probe pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_PROBE_FLOP_PULSE_PITCH  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 76)
#define SVT_SEQ_PROBE_FLOP_PULSE_PITCH  VFT_FLT32
#define SVS_SEQ_PROBE_FLOP_PULSE_PITCH  "Probe flop pulse pitch"
/*	The probe pulse pitch, in hardware units,  computed by seqgen.
	*/

#define SVN_SEQ_PROBE_FLIP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 77)
#define SVT_SEQ_PROBE_FLIP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_PROBE_FLIP_PULSE_NAME  "Probe flip pulse waveform name"
/*	probeパルスで使用するＳＥＰ波形名
	*/

#define SVN_SEQ_PROBE_FLOP_PULSE_NAME  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 78)
#define SVT_SEQ_PROBE_FLOP_PULSE_NAME  VFT_I32COMPLEX
#define SVS_SEQ_PROBE_FLOP_PULSE_NAME  "Probe flop pulse waveform name"
/*	probeパルスで使用するＳＥＰ波形名
	*/

#define SVN_SEQ_PROBE_FLIP_GAIN_VALUE     (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 79)
#define SVT_SEQ_PROBE_FLIP_GAIN_VALUE     VFT_FLT32
#define SVS_SEQ_PROBE_FLIP_GAIN_VALUE     "Probe flip pulse TX gain value (dB)"
/*      (1)probe用FlipパルスでのTXゲイン値
*/

#define SVN_SEQ_PROBE_FLOP_GAIN_VALUE     (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 80)
#define SVT_SEQ_PROBE_FLOP_GAIN_VALUE     VFT_FLT32
#define SVS_SEQ_PROBE_FLOP_GAIN_VALUE     "Probe flop pulse TX gain value (dB)"
/*      (1)probe用FlopパルスでのTXゲイン値
*/

#define SVN_SEQ_NUM_GATE_COUNT     	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 81)
#define SVT_SEQ_NUM_GATE_COUNT     	VFT_INT32
#define SVS_SEQ_NUM_GATE_COUNT     	"Number of gate count"
/*      Gate収集する回数。
	Gate収集時の収集カウント表示に使用する。
	set   : seqgen
	refer : acqman
*/

#define SVN_SEQ_GATE_TRIG_FILTER     	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 82)
#define SVT_SEQ_GATE_TRIG_FILTER     	VFT_INT32
#define SVS_SEQ_GATE_TRIG_FILTER     	"Selected gate filter"
/*
	ＱＲＳトリガ検出に用いるモードを指定する。TrueSSFPによるCine／Retro撮影時など
	の際には、トリガの検出精度を上げるため特別な値を設定する
	値   0x00   : 通常
	値   0x40   : TrueSSFPによるCine／Retro撮影時など
*/

#define SVN_SEQ_GATE_TRIG_DELAY_MODE     (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 83)
#define SVT_SEQ_GATE_TRIG_DELAY_MODE     VFT_INT32
#define SVS_SEQ_GATE_TRIG_DELAY_MODE     "Selected gate trigger mode"
/*
	ＧＭ基板がＱＲＳトリガからのディレイ時間をカウントする方法を指定する。
	トリガ直後まで信号を収集したい場合などに、特別な値を設定する
	値   0x0   : 通常。ディレイ時間を、WAIT2信号が設定された以降のトリガパルス
	             からカウントする。
	値   0x1   : ディレイ時間を、WAIT2信号とは独立にカウントする。
*/

#define SVN_SEQ_RGN_ADJUST_TABLE_SIZE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 84)
#define SVT_SEQ_RGN_ADJUST_TABLE_SIZE	VFT_INT32
#define SVS_SEQ_RGN_ADJUST_TABLE_SIZE	"Table size of Image Intensity adjustment due to RGN"
/*	DWIなどでMPGの印加方向、強度を変更しながら複数回繰返して撮像する場合に、その
	都度Rxgainを変更して収集する際に使用。拡散係数の計算を適切にするため、Rxgain
	が異る分を画像値で補正する。seqgenが、得られる画像数分をテーブルサイズとして
	セットする。reconが参照する。
	*/

#define SVN_SEQ_RGN_ADJUST_TABLE	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 85)
#define SVT_SEQ_RGN_ADJUST_TABLE	VFT_FLT32
#define SVS_SEQ_RGN_ADJUST_TABLE	"Image Intensity adjustment due to variable RGN"
/*	DWIなどでMPGの印加方向、強度を変更しながら複数回繰返して撮像する場合に、その
	都度Rxgainを変更して収集する際に使用。拡散係数の計算を適切にするため、Rxgain
	が異る分を画像値で補正する。seqgenが、得られる画像数分だけをRxGain値の差分を
	セットする。reconが参照する。
	*/

#define   SVN_SEQ_WATEREXCIT_FLIP_GAIN_VALUE    (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 86)
#define   SVT_SEQ_WATEREXCIT_FLIP_GAIN_VALUE    VFT_FLT32
#define   SVS_SEQ_WATEREXCIT_FLIP_GAIN_VALUE    " Water Excitation Probe flip pulse TX gain value (dB) "
/*
配列にて格納　SubProtocol にて定義。
水励起での格パルスにおいてのTX-gain値を、seqgenが計算し設定する。
*/

#define   SVN_SEQ_WATEREXCIT_FLIP_PULSE_SCALER  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 87)
#define   SVT_SEQ_WATEREXCIT_FLIP_PULSE_SCALER  VFT_FLT32
#define   SVS_SEQ_WATEREXCIT_FLIP_PULSE_SCALER  "Water Excitation pulse scaler "
/*
配列にて格納　SubProtocol にて定義。
水励起での格パルスにおいてのpulse scaler値を、seqgenが計算し設定する。
*/

#define   SVN_SEQ_OVER_TEMP_PROTECTION_ACTION  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 88)
#define   SVT_SEQ_OVER_TEMP_PROTECTION_ACTION  VFT_INT32
#define   SVS_SEQ_OVER_TEMP_PROTECTION_ACTION  "Over temperature protection action "
/*	
	GCoil温度監視機能実行フラグ。
	*/

#define   SVN_SEQ_OVER_TEMP_PROTECTION_VALUE  (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 89)
#define   SVT_SEQ_OVER_TEMP_PROTECTION_VALUE  VFT_INT32
#define   SVS_SEQ_OVER_TEMP_PROTECTION_VALUE  "Over temperature protection value "
/*	
	GCoil温度監視機能でのスキャン抑制温度。
	本温度以上の場合はスキャンを抑制する。
	*/

#define SVN_SEQ_ACTIVE_SAR	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 90)
#define SVT_SEQ_ACTIVE_SAR	VFT_INT32
#define SVS_SEQ_ACTIVE_SAR	"Active SAR (Whole Body/Head/Partial)"
/*	Whole Body SAR = 0
	Head SAR = 1
	Partial SAR = 2
	*/

#define SVN_SEQ_SAR_RATIO	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 91)
#define SVT_SEQ_SAR_RATIO	VFT_FLT32
#define SVS_SEQ_SAR_RATIO	"Specific Absorption Rate Ratio(SAR ratio)"
/*	SAR Ratio = SAR / SAR Limit
	*/

#define SVN_SEQ_TEMPERATURE_PROTECTION_PARAM   (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 92)
#define SVT_SEQ_TEMPERATURE_PROTECTION_PARAM   VFT_FLT32
#define SVS_SEQ_TEMPERATURE_PROTECTION_PARAM   "Temperature protection parameters"
/*	温度管理機構用計算パラメータ

	系統１ G-Coil OLP
	Param[0]: Tmax1
	Param[1]: Te1：熱平衝温度[℃]
	Param[2]: tau1：温度変化の時定数[s]

	系統２ボア、ライナー OLP
	Param[3]: Tmax2
	Param[4]: Te2：熱平衝温度[℃]
	Param[5]: tau2：温度変化の時定数[s]
	*/

#define SVN_SEQ_RF_AMP_DUTY	(SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 93)
#define SVT_SEQ_RF_AMP_DUTY	VFT_FLT32
#define SVS_SEQ_RF_AMP_DUTY	"RFamp Duty(W/s)"
/*	RFamp Duty(W/s)

	set   : pqm
	refer : ---
	*/

#define SVN_SEQ_FATSAT_MULTI_GAIN_VALUE     (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 94)
#define SVT_SEQ_FATSAT_MULTI_GAIN_VALUE     VFT_FLT32
#define SVS_SEQ_FATSAT_MULTI_GAIN_VALUE     "Fatsat multi-pulse RF Tx gain value (dB)"
/* 	Tx gain value for fatsat multi-pulse. It is computed by seqgen from RF
	Level measurements.

	set   : seqgen
	refer : ---
	*/

#define SVN_SEQ_FATSAT_MULTI_PULSE_SCALER   (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 95)
#define SVT_SEQ_FATSAT_MULTI_PULSE_SCALER   VFT_FLT32
#define SVS_SEQ_FATSAT_MULTI_PULSE_SCALER   "Fatsat multi-pulse scaler"
/*	The fatsat multi-pulse scaler computed by seqgen.

	set   : seqgen
	refer : ---
	*/

#define SVN_SEQ_FATSAT_MULTI_PULSE_PITCH   (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 96)
#define SVT_SEQ_FATSAT_MULTI_PULSE_PITCH   VFT_FLT32
#define SVS_SEQ_FATSAT_MULTI_PULSE_PITCH   "Fatsat multi-pulse pitch"
/*	The fatsat multi-pulse pitch computed by seqgen.

	set   : seqgen
	refer : ---
	*/


#define SVN_SEQ_DBDT_VECTOR_SUM_METHOD   (SVN_MIN_SUBPROTOCOL+SVN_OFF_SEQUENCE + 97)
#define SVT_SEQ_DBDT_VECTOR_SUM_METHOD   VFT_INT32
#define SVS_SEQ_DBDT_VECTOR_SUM_METHOD   "dBdt vector sum method"
/*	IEC2002モード時dBdtベクトル計算方法の指定の記録
	set   : seqgen
	refer : ---
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

#define SVN_SEQ_READOUT_SMPL	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 0)
#define SVT_SEQ_READOUT_SMPL	VFT_INT32
#define SVS_SEQ_READOUT_SMPL	"Number of readout samples acquired"
/*	This field is the number of readout points acquired for this image.
	*/

#define SVN_SEQ_PHASEENC_SMPL	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 1)
#define SVT_SEQ_PHASEENC_SMPL	VFT_INT32
#define SVS_SEQ_PHASEENC_SMPL	"Number of phase encode samples acquired"
/*	This field is the number of phase encodings acquired for this image.
	*/

#define SVN_SEQ_SLICEENC_SMPL	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 2)
#define SVT_SEQ_SLICEENC_SMPL	VFT_INT32
#define SVS_SEQ_SLICEENC_SMPL	"Number of slice encode samples acquired"
/*	This field is the number of slice encodings acquired for this image.
	*/

#define SVN_SEQ_DETAIL_TE	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 3)
#define SVT_SEQ_DETAIL_TE	VFT_FLT32
#define SVS_SEQ_DETAIL_TE	"Detailed TE for each echo"
/*	This field is the detailed TE in seconds for each echo in an image
	that is collected by the host.
	*/

#define SVN_SEQ_ECHO_POLARITY  	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 4)
#define SVT_SEQ_ECHO_POLARITY  	VFT_INT32
#define SVS_SEQ_ECHO_POLARITY  	"Echo polarity"
/*	Echo polarity written by seqgen. The image flip in the phase direction
	is determined by the combination of this field and the PE step polarity
	defined in the same level.  The values for this field are defined in
	vf_appcodes.h.
	*/

#define SVN_SEQ_PE_STEP_POLARITY  (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 5)
#define SVT_SEQ_PE_STEP_POLARITY  VFT_INT32
#define SVS_SEQ_PE_STEP_POLARITY  "Phase encode step polarity"
/*	Phase encode step polarity written by seqgen. The image flip in the
	phase direction is determined by the combination of this field and the
	echo polarity defined in the same level.  The values for this field
	are defined in vf_appcodes.h.
	*/

#define SVN_SEQ_READ_POLARITY  	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 6)
#define SVT_SEQ_READ_POLARITY  	VFT_INT32
#define SVS_SEQ_READ_POLARITY  	"Read gradient polarity"
/*	Polarity of read gradient written by seqgen. The image flip in the
	readout direction is determined by the combination of this field and
	the readout channel polarity defined in the subprotocol level. Read
	polarity is a fixed property of the sequence and the echo. The
	overall channel polarity is a runtime variable that depends on the
	oblique orientation selected for a scan. The values for this field
	are defined in vf_appcodes.h.
	*/

#define SVN_SEQ_LPF_BANDWIDTH  	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 7)
#define SVT_SEQ_LPF_BANDWIDTH  	VFT_FLT32
#define SVS_SEQ_LPF_BANDWIDTH  	"Low-pass filter bandwidth"
/*	Low-pass filter bandwidth, in hardware units, computed by seqgen.
	*/

#define SVN_SEQ_ADC_PITCH  	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 8)
#define SVT_SEQ_ADC_PITCH  	VFT_FLT32
#define SVS_SEQ_ADC_PITCH	"ADC pitch"
/*	ADC pitch, in hardware units, computed by seqgen.
	*/

#define SVN_SEQ_FULL_K_SPACE_DIM (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 9)
#define SVT_SEQ_FULL_K_SPACE_DIM VFT_INT32
#define SVS_SEQ_FULL_K_SPACE_DIM "Full k-space dimensions"
/*	The dimensions (in no. of samples) of k-space.
	This is an array of either 2 or 3 elements, depending on whether
	the acquisition is 2d or 3d. The order of the
	elements is either {PE, RO},  or {SE, PE, RO}.
	*/

#define SVN_SEQ_K_SPACE_CENTER  (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 10)
#define SVT_SEQ_K_SPACE_CENTER  VFT_INT32
#define SVS_SEQ_K_SPACE_CENTER  "Index of k-space center sample"
/*	For each data dimension, this is the index of the sample that
	should be placed at the center of k-space. For acquisitions
	that are truncated (MBW, AFI), this index allows the data
	to be placed correctly in k-space during reconstruction.
	This is an array of either 2 or 3 elements, depending on whether
	the acquisition is 2d or 3d. The order of the
	elements is either {PE, RO},  or {SE, PE, RO}.

	N.B. This index is relative to the first acquired sample, for PE,
	and SE this is assumed to be the first sample stored in the in
	the data field. For RO the first acquired sample is given by
	SVN_SEQ_FIRST_RO_SAMPLE (see below).
	*/

#define SVN_SEQ_NUM_RO_SMPL_DIG  (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 11)
#define SVT_SEQ_NUM_RO_SMPL_DIG  VFT_INT32
#define SVS_SEQ_NUM_RO_SMPL_DIG  "Number of readout samples digitized"
/*	Data storage for the readout dimension has some peculiarities.
	The data field may be larger than the number of samples acquired.
	This field contains the actual number of readout samples digitized.
	N.B. Be aware that other fields describing the RO dimension
	describe the data field size NOT the actual number of samples
	acquired.
	*/

#define SVN_SEQ_FIRST_RO_SAMPLE  (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 12)
#define SVT_SEQ_FIRST_RO_SAMPLE  VFT_INT32
#define SVS_SEQ_FIRST_RO_SAMPLE  "Index of first readout sample"
/*	Index of the first readout sample digitized, relative to the start
	of the data field.
	*/

#define SVN_SEQ_FULL_FOV  	(SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 13)
#define SVT_SEQ_FULL_FOV  	VFT_FLT32
#define SVS_SEQ_FULL_FOV	"FOV related to full k-space dimensions"
/*	Array of values, in meters, specifying FOV related to each full k-space
	dimension. The order of the elements is either {PE,RO}, or {SE,PE,RO}.
	This field is determined by seqgen.
	*/

#define SVN_SEQ_DATA_ALTERNATION (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 14)
#define SVT_SEQ_DATA_ALTERNATION VFT_INT32
#define SVS_SEQ_DATA_ALTERNATION "Phase alternation flags for data dimensions."
/*	Array of flags specifying the phase alternation in each data dimension.
	The order of data dimensions is described by SVN_GEN_DATA_DESC.
	The flag values are defined in vf_appcodes.h.
	*/

#define SVN_SEQ_VENC_VECTOR (SVN_MIN_IMAGE+SVN_OFF_SEQUENCE + 15)
#define SVT_SEQ_VENC_VECTOR VFT_FLTVEC3
#define SVS_SEQ_VENC_VECTOR "Velocity encoding vector of an image (m/sec)."
/*	Velocity encoding vector used for acquiring an image in phase shift
	vascular imaging.  It is specified in the imaging coordinate system.
	The order of the elements is {RO, PE, SL}. The unit is meters/sec.
	*/

/*****************************************************************************
  PROCESSED name space
*/
#define SVN_SEQ_AXI_FSY_OFFSET	(SVN_MIN_PROCESSED+SVN_OFF_SEQUENCE+0)
#define SVT_SEQ_AXI_FSY_OFFSET	VFT_FLT32
#define SVS_SEQ_AXI_FSY_OFFSET	"Transaxial FSY offset"
/*	msShimが磁場分布調整処理の際に、アキシャル方向
	の周波数の調整量とそれに対応するアキシャルスライス方向
	のポジションの値をprocessed nodeにセットする。
	この値は、スライス枚数分セットされる。
	ポジションは昇順にセットする。
	*/

#define SVN_SEQ_SAG_FSY_OFFSET	(SVN_MIN_PROCESSED+SVN_OFF_SEQUENCE+1)
#define SVT_SEQ_SAG_FSY_OFFSET	VFT_FLT32
#define SVS_SEQ_SAG_FSY_OFFSET	"Transaxial FSY offset"
/*	msShimが磁場分布調整処理の際に、サジタル方向
	の周波数の調整量とそれに対応するサジタルスライス方向
	のポジションの値をprocessed nodeにセットする。
	この値は、スライス枚数分セットされる。
	ポジションは昇順にセットする。
	*/

#define SVN_SEQ_COR_FSY_OFFSET	(SVN_MIN_PROCESSED+SVN_OFF_SEQUENCE+2)
#define SVT_SEQ_COR_FSY_OFFSET	VFT_FLT32
#define SVS_SEQ_COR_FSY_OFFSET	"Transaxial FSY offset"
/*	msShimが磁場分布調整処理の際に、コロナル方向
	の周波数の調整量とそれに対応するコロナルスライス方向
	のポジションの値をprocessed nodeにセットする。
	この値は、スライス枚数分セットされる。
	ポジションは昇順にセットする。
	*/

#define SVN_SEQ_MAP_PATH	(SVN_MIN_PROCESSED+SVN_OFF_SEQUENCE+3)
#define SVT_SEQ_MAP_PATH	VFT_PATHELEM
#define SVS_SEQ_MAP_PATH	"Sensitivity data path"
/*   SPEEDER本スキャンのプロトコルの場合、
     感度マップパスを判断しPQMがprotocol nodeに
     記述する。そのパスに格納されている情報を
     感度マップ切り出しプログラムが、読み込み
     切り出し後感度マップを作成する。
     */

/*****************************************************************************
  ECHO name space
*/



/*****************************************************************************
  POSITION name space
*/
#define SVN_SEQ_SHIM_HIST_RANGE		(SVN_MIN_POSITION+SVN_OFF_SEQUENCE+0)
#define SVT_SEQ_SHIM_HIST_RANGE		VFT_FLT32
#define SVS_SEQ_SHIM_HIST_RANGE		"Information of shimming histgram"
/*	msShimが磁場分布測定処理、及び磁場分布調整処理
	の際に、各スライスにおけるヒストグラムの最大値
	最小値、半値幅をposition nodeに記述する。
	*/

#define SVN_SEQ_TEMPB0_CORR_FREQ        (SVN_MIN_POSITION+SVN_OFF_SEQUENCE+1)
#define SVT_SEQ_TEMPB0_CORR_FREQ        VFT_FLT32
#define SVS_SEQ_TEMPB0_CORR_FREQ        "B0 shift value (to Base phase) for each slice and dynamic phase(Hz)"
/*	スライスとダイナミック時相ごとのB0シフト値（ベース時相からの）
	*/

/*****************************************************************************
  IMCALC name space
*/



#endif /* tami_vfStudy_vfs_sequence_H__ */
