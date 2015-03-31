/*********************************************************************
 *
 *  (c) Copyright 1997 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Nov. 4.97	古舘直幸	Created
 * 2007-11-05	sr.Hong     2エコー画像における自動合成後処理対応
 *                          PROTOCOL name spaceに追加
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
/*	T2補正用のフラグ値、画像コントラストなどによりプリセット値を設定
	する。現在有効な値として、
	VFC_T2_CORR_NONE        0    補正なし
	VFC_T2_CORR_FOR_T2W     1    T２W用
	VFC_T2_CORR_FOR_T1W     2    T１W用
	VFC_T2_CORR_FOR_PD      3    PD用
	VFC_T2_CORR_FREE        4    Wiener filter による補正用
	要素２の１次元配列とし、デュアルコントラストに対応する。
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
/*	フェーズエンコード方向の並べ変えルールを記述する。
		VFC_SORT_NONE		0
		VFC_SORT_FREE		1
		VFC_SORT_CENTRIC	2
	VFC_SORT_FREE の場合はソートテーブルを使用する。
	*/

#define SVN_PRC_PE_SRTTBL_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 20)
#define SVT_PRC_PE_SRTTBL_TYPE		VFT_INT32
#define SVS_PRC_PE_SRTTBL_TYPE		"Sort table type (PE)"
/*	フェーズエンコードの並べ変えテーブルのタイプ(VFT_UINT16,VFTUINT32)
	を記述する。
	*/

#define SVN_PRC_PE_SRTTBL_BITS		(SVN_BASE_INDEPENDENT_PROCESS + 21)
#define SVT_PRC_PE_SRTTBL_BITS		VFT_INT32
#define SVS_PRC_PE_SRTTBL_BITS		"The number of bits for index part"
/*	並べ替えテーブルのインデックス部のビット数を記述する。
	*/


#define SVN_PRC_PE_SRTTBL_SIZE		(SVN_BASE_INDEPENDENT_PROCESS + 22)
#define SVT_PRC_PE_SRTTBL_SIZE		VFT_INT32
#define SVS_PRC_PE_SRTTBL_SIZE		"The number of entries in sort table"
/*	並べ替えテーブルのエントリー数を記述する。
	*/

#define SVN_PRC_SE_SORT_METHOD		(SVN_BASE_INDEPENDENT_PROCESS + 23)
#define SVT_PRC_SE_SORT_METHOD		VFT_INT32
#define SVS_PRC_SE_SORT_METHOD		"Sort method (SE)"
/*	スライスエンコード方向の並べ変えルールを記述する。
		VFC_SORT_NONE		0
		VFC_SORT_FREE		1
		VFC_SORT_CENTRIC	2
	VFC_SORT_FREE の場合はソートテーブルを使用する。
	従来スキャンとの互換性を保つため、RBC でのソートは
	Interleave Flag に係わらず、VFC_SORT_CENTRIC の場合のみ
	に行ない、RECON でのソートはVFC_SORT_CENTRIC の場合には
	行なわない。
	*/

#define SVN_PRC_SE_SRTTBL_TYPE		(SVN_BASE_INDEPENDENT_PROCESS + 24)
#define SVT_PRC_SE_SRTTBL_TYPE		VFT_INT32
#define SVS_PRC_SE_SRTTBL_TYPE		"Sort table type (SE)"
/*	スライスエンコードの並べ変えテーブルのタイプ(VFT_UINT16,VFTUINT32)
	を記述する。
	*/

#define SVN_PRC_SE_SRTTBL_BITS		(SVN_BASE_INDEPENDENT_PROCESS + 25)
#define SVT_PRC_SE_SRTTBL_BITS		VFT_INT32
#define SVS_PRC_SE_SRTTBL_BITS		"The number of bits for index part"
/*	並べ替えテーブルのインデックス部のビット数を記述する。
	*/

#define SVN_PRC_SE_SRTTBL_SIZE		(SVN_BASE_INDEPENDENT_PROCESS + 26)
#define SVT_PRC_SE_SRTTBL_SIZE		VFT_INT32
#define SVS_PRC_SE_SRTTBL_SIZE		"The number of entries in sort table"
/*	並べ替えテーブルのエントリー数を記述する。
	*/

#define SVN_PRC_PROCESSING_CODE		(SVN_BASE_INDEPENDENT_PROCESS + 27)
#define SVT_PRC_PROCESSING_CODE		VFT_STRING
#define SVS_PRC_PROCESSING_CODE		"Process code for reconstruction"
/*	再構成で使用したフィルターを識別するための文字列。
	１文字が１処理に相当する。フィルターの他ゼロ詰め処理などの
	識別にも使用する。表示系はこの文字列をそのまま表示する。
	再構成によってプロセスドノードにセットされる。
	*/

#define SVN_PRC_PROTECT_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 28)
#define SVT_PRC_PROTECT_FLAG		VFT_INT32
#define SVS_PRC_PROTECT_FLAG		"Protect flag"
/*	SVN_ST_PROTOCOL,SVN_ST_PROCESSED に記述されるフィールド。
        それぞれ、RawData,Imageのプロテクトの有無を表す。ただし、互換性維持
	のため、コードが存在しない場合には、プロテクトされているとみなす。
  	*/

#define SVN_PRC_DC_SHAVE		(SVN_BASE_INDEPENDENT_PROCESS + 29)
#define SVT_PRC_DC_SHAVE		VFT_BOOL
#define SVS_PRC_DC_SHAVE		"The flag for shaving"
/*	DCアーチファクト除去処理（通称鬚剃り）行なうか否かを指定する。
	SVN_ST_PROTOCOL,SVN_ST_PROCESSED に記述される。
	*/

#define SVN_PRC_PE_SRTTBL		(SVN_BASE_INDEPENDENT_PROCESS + 30)
#define SVS_PRC_PE_SRTTBL		"PE sorting table"
/*	並べ替えテーブル。型は SVN_PRC_SE_SRTTBL_TYPE によって決まる。
	テーブルのエントリーはスラブ番号（上位ビット）とスライス番号（下位
	ビット）を持つ。スライス部のビット数は SVN_PRC_SE_SRTTBL_BITS
	によって決まりスラブ部のビット数は SVN_PRC_SE_SRTTBL_TYPE と
	SVN_PRC_SE_SRTTBL_BITS によって計算される。
	テーブル順が励起順（時刻）であり、それぞれのスライス位置が
	スラブ番号：スライス番号
	により定義される。
	*/

#define SVN_PRC_SE_SRTTBL		(SVN_BASE_INDEPENDENT_PROCESS + 31)
#define SVS_PRC_SE_SRTTBL		"SE sorting table"
/*	SVN_PRC_PE_SRTTBL 参照。
	*/

#define SVN_PRC_REFINE_FILTER		(SVN_BASE_INDEPENDENT_PROCESS + 32)
#define SVT_PRC_REFINE_FILTER		VFT_INT32
#define SVS_PRC_REFINE_FILTER		"Refine filter ID"
/*	リファインフィルターＩＤ。
	上位１６ビットをフィルター種、下位１６ビットをインデックス
	とする。フィルター係数・表示情報はフィルターＤＢに置かれる。
	設定者によりプロトコルノードにフィールドを作成し情報をセット
	する。再構成はプロトコルノードからプロセスドノードにコピーす
	る。フィルターＩＤ＝０はフィルターを掛けないことを意味する。
	SVN_ST_PROTOCOL,SVN_ST_PROCESSED に設定する。
	*/

#define SVN_PRC_K_SPACE_FILTER		(SVN_BASE_INDEPENDENT_PROCESS + 33)
#define SVT_PRC_K_SPACE_FILTER 		VFT_INT32
#define SVS_PRC_K_SPACE_FILTER		"K Space filter ID"
/*	Ｋ空間フィルターＩＤ。
	上位１６ビットをフィルター種、下位１６ビットをインデックス
	とする。フィルター係数・表示情報はフィルターＤＢに置かれる。
	設定者によりプロトコルノードにフィールドを作成し情報をセット
	する。再構成はプロトコルノードからプロセスドノードにコピーす
	る。フィルターＩＤ＝０はフィルターを掛けないことを意味する。
	*/

#define SVN_PRC_BARIQ_MODE		(SVN_BASE_INDEPENDENT_PROCESS + 34)
#define SVT_PRC_BARIQ_MODE		VFT_INT32
#define SVS_PRC_BARIQ_MODE		"BariQ Mode"
/*	追加：パルスシーケンスでのRF/ADC 位相のコントロールにより、DC 
	アーチファクトが画像上に現れる位置が中心／位相エンコード方向の
	画像端と変化するため、再構成側での補正をそれに合わせて変更する。
	ショット数を偶数にとれるシーケンス（現行のほとんどのシーケンス）
	では現在の処理からの変更なし。

	VFC_BARIQ_NONE		0
	VFC_BARIQ_ON		1
	*/

#define SVN_PRC_WIENER_CORR_METHOD	(SVN_BASE_INDEPENDENT_PROCESS + 35)
#define SVT_PRC_WIENER_CORR_METHOD	VFT_BITFIELD
#define SVS_PRC_WIENER_CORR_METHOD	"Weiner filter method"
/*	Weiner filter の有無と、適用される軸を指定するフラグ。軸については 
	Weiner filter の有無と、適用される軸を指定するフラグ。軸については 
	Bit Or で選択する。
	VFC_WIENER_CORR_NONE		0x0000	補正なし
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
/*	Weiner filter の強度を決定するパラメータ。詳しくはソフトウェア
	システム仕様書SAM45-0101 を参照のこと。定義されない場合のデフォルト
	値は1.0 とする。
*/

#define SVN_PRC_T2_ESTIMATE_TIME	(SVN_BASE_INDEPENDENT_PROCESS + 37)
#define SVT_PRC_T2_ESTIMATE_TIME	VFT_FLT32
#define SVS_PRC_T2_ESTIMATE_TIME	"T2 Estimate Time"
/*	T2 補正法の拡張に伴い、テンプレート収集が不可能なワンショット
	シーケンス、RO 方向などにもT2 補正を可能とする。ただし、テンプレート
	により減衰率を算出できないため、フィルター係数などと同様にユーザーが
	入力可能なパラメータとする。テンプレートデータとの優先順位はT2 補正
	モードにより決定する。他の時間パラメータと同様、秒単位での設定とする。
*/

#define SVN_PRC_PS_RECON_IMAGE		(SVN_BASE_INDEPENDENT_PROCESS + 38)
#define SVT_PRC_PS_RECON_IMAGE		VFT_BITFIELD
#define SVS_PRC_PS_RECON_IMAGE		"Phase Shift reconstruction images"
/*	PSーMRA再構成で再構成する画像種の種類

	○スピード画像
	VFC_PS_RECON_SPEED_PE		0x0001
	VFC_PS_RECON_SPEED_RO		0x0002
	VFC_PS_RECON_SPEED_SE		0x0004
	○フロー画像
	VFC_PS_RECON_FLOW_PE		0x0010
	VFC_PS_RECON_FLOW_RO		0x0020
	VFC_PS_RECON_FLOW_SE		0x0040
	○ベッセル画像
	VFC_PS_RECON_VESSEL		0x0100
	○プロトン画像
	VFC_PS_RECON_PROTON		0x1000
*/

#define SVN_PRC_PS_COMBINATION		(SVN_BASE_INDEPENDENT_PROCESS + 40)
#define SVT_PRC_PS_COMBINATION		VFT_BITFIELD
#define SVS_PRC_PS_COMBINATION		"PS Combination"
/*	各流速方向(PE,RO,SL)に関する、収集データの加減算の符合
	PE,RO,SE の順に配列として格納する。

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
				 | | | |  --> 第１収集	00:None
				 | | |  --> 第２収集	01:Addition
				 | |  --> 第３収集	10:Subtraction
				 |  --> 第４収集

	最大６点法までサポートする。（２点法もある）

	Default:v[0] = -+-+  --> 102	(PE)
	        v[1] = -++-  --> 150	(RO)
		v[2] = --++  -->  90	(SE)
*/

#define SVN_PRC_PS_THRESHOLD		(SVN_BASE_INDEPENDENT_PROCESS + 41)
#define SVT_PRC_PS_THRESHOLD		VFT_FLT32
#define SVS_PRC_PS_THRESHOLD		"Phase shift threshold"
/*	PS-MRA再構成での閾値処理で使用する
	値はCOEFICIENT,LOW,HIGHの順に配列で格納する

	v[0],COEFICIENT:閾値処理の係数を表す
	v[1],LOW       :平均画像値のいくつ以下を閾値を処理するかを表す
	v[2],HIGH      :平均画像値のいくつ以上を閾値を処理するかを表す
	
	Default: COEFFICIENT = 0.0
	         LOW	     = 1.0
		 HIGH	     = 1.5
*/

#define SVN_PRC_PS_LPF_KC		(SVN_BASE_INDEPENDENT_PROCESS + 42)
#define SVT_PRC_PS_LPF_KC		VFT_FLT32
#define SVS_PRC_PS_LPF_KC		"PS LPF KC"
/*	PS-MRA再構成での低周波フィルター処理で使用する
	各項目はAFIで用いられるパラメータと同じ意味を持つ

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_PS_LPF_K1		(SVN_BASE_INDEPENDENT_PROCESS + 43)
#define SVT_PRC_PS_LPF_K1		VFT_FLT32
#define SVS_PRC_PS_LPF_K1		"PS LPF K1"
/*	PS-MRA再構成での低周波フィルター処理で使用する
	各項目はAFIで用いられるパラメータと同じ意味を持つ

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_PS_LPF_K2		(SVN_BASE_INDEPENDENT_PROCESS + 44)
#define SVT_PRC_PS_LPF_K2		VFT_FLT32
#define SVS_PRC_PS_LPF_K2		"PS LPF K2"
/*	PS-MRA再構成での低周波フィルター処理で使用する
	各項目はAFIで用いられるパラメータと同じ意味を持つ

	Default:SVN_PRC_PS_LPF_KC	= 7.0
		SVN_PRC_PS_LPF_K1	= 5.0
		SVN_PRC_PS_LPF_K2	= 2.0
*/

#define SVN_PRC_WIENER_SE_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 45)
#define SVT_PRC_WIENER_SE_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_SE_DECAY_TABLE	"Wiener SE Decay table"
/*	  Wiener filter の減衰補正を表すテーブル。SVN_PRC_WIENER_CORR_METHOD
	にてエコーデータ収集間隔テーブル(VFC_WEINER_SE_ESTIMATE)が定義されている
	場合に参照される。
	  テンプレート収集から補正する方式(VFC_WEINER_SE_TEMPLATE)については
	従来のT2 補正と同様テンプレートデータから計算した減衰率を利用する。
*/

#define SVN_PRC_WIENER_PE_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 46)
#define SVT_PRC_WIENER_PE_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_PE_DECAY_TABLE	"Wiener PE Decay table"
/*	  Wiener filter の減衰補正を表すテーブル。SVN_PRC_WIENER_CORR_METHOD
	にてエコーデータ収集間隔テーブル(VFC_WEINER_PE_ESTIMATE)が定義されている
	場合に参照される。
	  テンプレート収集から補正する方式(VFC_WEINER_PE_TEMPLATE)については
	従来のT2 補正と同様テンプレートデータから計算した減衰率を利用する。
*/

#define SVN_PRC_WIENER_RO_DECAY_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 47)
#define SVT_PRC_WIENER_RO_DECAY_TABLE	VFT_FLT32
#define SVS_PRC_WIENER_RO_DECAY_TABLE	"Wiener RO Decay table"
/*	  Wiener filter の減衰補正を表すパラメータ。SVN_PRC_WIENER_CORR_METHOD
	にてエコーデータ収集間隔テーブル(VFC_WEINER_RO_STEP)が定義されている
	場合に参照される。メンバーとしては、スタート値とステップとなる。
	ESTIMATE_T2 とあわせ、内部的に減衰率を計算するために用いられる。
	(table で持たせるとサンプリング点数分用意する必要があり、効率が悪い）
*/

#define SVN_PRC_PHASE_CORR_DATA		(SVN_BASE_INDEPENDENT_PROCESS + 48)
#define SVT_PRC_PHASE_CORR_DATA		VFT_FLT32
#define SVS_PRC_PHASE_CORR_DATA		"Phase correction parameter for zero shot phase correction"
/*
	  Multi-Shot EPI / Hybrid EPI において、位相補正が０
	ショットモードの場合に補正に使用する値（リード方向の
	位相補正量）現状では、1次までの係数を設定する。
	要素数５の配列に係数を持たせる。
	[単位]
	０次の位相補正量。(単位：度)
	１次の位相補正量。(単位：度／points)
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
/*	行を特定する 1 以上の整数。
	Echo Node または Position Node に登録する。
	同じ row-id を持つ画像は同じ行に置かれる。
	row-id は Processed 内で unique な値となる。
	*/

#define SVN_PRC_GROUP_ID		(SVN_BASE_INDEPENDENT_PROCESS + 53)
#define SVT_PRC_GROUP_ID		VFT_INT32
#define SVS_PRC_GROUP_ID		"Group ID on image selector"
/*	行の中のグループを特定する 1 以上の整数。
	Echo Node または Position Node に登録する。
	同じ row-id, group-id を持つ画像は同じ行の一まとまりとして
	配置される。異なる group-id 間には空白が挿入される。
	(従来のマルチスラブと同様)
	group-id は行内で unique な値となる。
	*/

#define SVN_PRC_IMG_INFO_DATA		(SVN_BASE_INDEPENDENT_PROCESS + 54)
#define SVT_PRC_IMG_INFO_DATA		VFT_UINT8
#define SVS_PRC_IMG_INFO_DATA		"Image Infomation Data"
/*	画像種別に異なる固有情報(画像種別固有情報)
	データ長及びデータ個数が共に可変長になっているので
	タイプはVFT_UINT8とする。
	但し内部は以下の様な構造をしている。
	typedef struct {
	    size_t	   	n		;  データ数 
	    struct {
	 	unit32_t   	infoTag		;  データ名 
	 	size_t		length		;  データ長 
	 	char	   	infoData[length];  実データ 
	    }		infoDatas[n] ;
	}ImgInfodata_t ;
	先頭にデータ件数(n)が格納され、それ以降には各データの
	タグ(infoTag)、サイズ(length)、実データ(infoData)が
	セットで格納される。これがデータ件数(n)分格納される。

	処理したプロセスは必要に応じて processed, echo または 
	position ノードに格納する。
	例）インセット情報（MIP、MPR）、係数（画像間演算、フィルタ）
	*/
  
#define SVN_PRC_SNAP_CORR_DATA (SVN_BASE_INDEPENDENT_PROCESS + 55)
#define SVS_PRC_SNAP_CORR_DATA "Copied Image data for Snap mode data correction"
/*	位相補正のため、テンプレートのみを含んだスキャンとして実行
	されたプロトコルもしくはプレスキャンのスタディファイルから
	SVN_GEN_DATAをコピーすることにより作成する。
	作成者は、プレスキャンのコントロールを行うプロセス
	(pqm or acqman)
	使用者は、再構成
	This data is copied from SVN_GEN_DATA for phase correction
	scan.
	Creater: pqm or acqman who control prescan process.
	User: recon
	
	*/

#define SVN_PRC_SNAP_CORR_DATA_TYPE (SVN_BASE_INDEPENDENT_PROCESS + 56)
#define SVT_PRC_SNAP_CORR_DATA_TYPE VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_TYPE "Data type for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA のデータタイプ
	Data type for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_DIM (SVN_BASE_INDEPENDENT_PROCESS + 57)
#define SVT_PRC_SNAP_CORR_DATA_DIM VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_DIM "Data dimension for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA のデータサイズ
	Data dimension for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_FORM (SVN_BASE_INDEPENDENT_PROCESS + 58)
#define SVT_PRC_SNAP_CORR_DATA_FORM VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_FORM "Data form for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA のデータ型
	Data form for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_DATA_DESC (SVN_BASE_INDEPENDENT_PROCESS + 59)
#define SVT_PRC_SNAP_CORR_DATA_DESC VFT_INT32
#define SVS_PRC_SNAP_CORR_DATA_DESC "Data dimension discription for SNAP_CORR_DATA"
/*	SVN_PRC_SNAP_CORR_DATA のデータディスクリプション
	Data dimension discription for SVN_PRC_SNAP_CORR_DATA
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL (SVN_BASE_INDEPENDENT_PROCESS + 60)
#define SVS_PRC_SNAP_CORR_PE_SRTTBL "PE sorting table"
/*	位相補正用データ収集により、作成される並べ替えテーブル。
	型は SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE によって決まる。
	テーブルのエントリーはアトリビュート（上位ビット）と
	位相エンコード位置（下位ビット）を持つ。位相エンコード
	位置部のビット数は SVN_PRC_SNAP_CORR_PE_SRTTBL_BITSに
	よって決まりアトリビュート部のビット数は SVN_PRC_SNAP_
	CORR_SRTTBL_TYPE とSVN_PRC_SNAP_CORR_PE_SRTTBL_BITS 
	によって計算される。
	テーブル順が励起順（時刻）であり、それぞれの位相エンコ
	ード量（ｋ空間配置）、テンプレート情報などのアトリビュ
	ート情報が再構成に伝えられる。
	
	seqgenによって作成されたSVN_PRC_PE_SRTTBLから位相補正
	のコントロールプロセス(qpm or acqman)によりコピーされ
	る。
	使用者は、再構成
	
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
/*	SVN_PRC_SNAP_CORR_PE_SRTTBLのpe order部の長さ
	seqgenによって作成されたSVN_PRC_PE_SRTTBL_BITSから位相補正
	のコントロールプロセス(qpm or acqman)によりコピーされる。
	使用者は、再構成
	
	seqgen generate SVN_PRC_PE_SRTTBL_BITS and prescan 
	control process copies SVN_PRC_PE_SRTTBL_BITS to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_BITS.
	User: Recon
	
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE (SVN_BASE_INDEPENDENT_PROCESS + 62)
#define SVT_PRC_SNAP_CORR_PE_SRTTBL_TYPE VFT_INT32
#define SVS_PRC_SNAP_CORR_PE_SRTTBL_TYPE "Sort table type (PE)"
/*	SVN_PRC_SNAP_CORR_PE_SRTTBLのデータ型
	seqgenによって作成されたSVN_PRC_PE_SRTTBL_TYPEから位相補正
	のコントロールプロセス(qpm or acqman)によりコピーされる。
	使用者は、再構成
	
	seqgen generate SVN_PRC_PE_SRTTBL_TYPE and prescan 
	control process copies SVN_PRC_PE_SRTTBL_TYPE to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_TYPE.
	User: Recon
	
	*/

#define SVN_PRC_SNAP_CORR_PE_SRTTBL_SIZE (SVN_BASE_INDEPENDENT_PROCESS + 63)
#define SVT_PRC_SNAP_CORR_PE_SRTTBL_SIZE VFT_INT32
#define SVS_PRC_SNAP_CORR_PE_SRTTBL_SIZE "The number of entries in sort table"
/*	SVN_PRC_SNAP_CORR_PE_SRTTBLのエントリー数
	seqgenによって作成されたSVN_PRC_PE_SRTTBL_SIZEから位相補正
	のコントロールプロセス(qpm or acqman)によりコピーされる。
	使用者は、再構成
	
	seqgen generate SVN_PRC_PE_SRTTBL_SIZE and prescan 
	control process copies SVN_PRC_PE_SRTTBL_SIZE to 
	SVN_PRC_SNAP_CORR_PE_SRTTBL_SIZE.
	User: Recon
	
	*/

#define SVN_PRC_CARDIAC_OUTLINE		(SVN_BASE_INDEPENDENT_PROCESS + 64)
#define SVT_PRC_CARDIAC_OUTLINE		VFT_CARDIAC_OUTLINE
#define SVS_PRC_CARDIAC_OUTLINE		"Outline point of the cardiac muscle."
/*	画像上の心筋の輪郭点データ
	CFA(Cardiac Functional Analysis)で使用
	Outline point of the cardiac muscle.
	This data is used by CFA(Cardiac Functional Analysis).
	
	*/

#define SVN_PRC_VIEW_SORT_TABLE	(SVN_BASE_INDEPENDENT_PROCESS + 65)
#define SVT_PRC_VIEW_SORT_TABLE	VFT_INT32
#define SVS_PRC_VIEW_SORT_TABLE	"View Sort Table"
/*	ダイナミックで収集された3Dローデータから、任意の収集時相と位置の
	エコーを抽出して再構成するためのテーブル。（１次元配列）

	第 i 番目の抽出後のデータは、抽出前（収集時）の第 j 番目のエコー
	データに対応するとすると、
	j = SVN_PRC_VIEW_SORT_TABLE[i]
	という対応になる。抽出後のデータの総数は、抽出後のデータの総数と
	必ずしも一致しない。また、同一のエコーデータが複数回抽出される場
	合もある。
	*/

#define SVN_PRC_VIEW_SRTTBL_SIZE	(SVN_BASE_INDEPENDENT_PROCESS + 66)
#define SVT_PRC_VIEW_SRTTBL_SIZE	VFT_INT32
#define SVS_PRC_VIEW_SRTTBL_SIZE	"Size of View Sort Table"
/*	VIEW_SORT_TABLE（１次元配列）の要素数を示す変数。
	値の範囲：1からpe方向の最大matrixSize＊スライス方向の最大matrixSize＊
	ダイナミックの最大収集時相数まで
	*/

#define SVN_PRC_VIEW_SRTTBL_TYPE	(SVN_BASE_INDEPENDENT_PROCESS + 67)
#define SVT_PRC_VIEW_SRTTBL_TYPE	VFT_INT32
#define SVS_PRC_VIEW_SRTTBL_TYPE	"Type of View Sort Table"
/*	3D DRKSにおいて、並び変える順序を示すテーブルVIEW_SORT_TABLE
	のタイプ(VFT_UINT16,VFTUINT32)	を記述する。
	*/

#define SVN_PRC_VIEW_SORT_METHOD	(SVN_BASE_INDEPENDENT_PROCESS + 68)
#define SVT_PRC_VIEW_SORT_METHOD	VFT_INT32
#define SVS_PRC_VIEW_SORT_METHOD	"View Sort Method"
/*	ダイナミックで収集された3Dローデータから、任意の収集時相と位置の
	エコーを抽出して再構成する、いわゆるviewSortingをするルールを記述。
	VFC_VIEW_NONE	 0  ：なにもしない。
	VFC_VIEW_FREE	 1  ：VIEW_SORT_TABLEを使用する。
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
  SVN_PLN_SNAP_SHOT_MODE == 1の場合に利用される補正データ。
  V6.50では、SVN_PRC_SNAP_CORR_DATA1からSVN_PRC_SNAP_CORR_DATA7まで
  フィールドだけ追加する。追加理由は、補正データもPAC対応必要なため、
  位相補正のため、テンプレートのみを含んだスキャンとして実行
  されたプロトコルもしくはプレスキャンのスタディファイルから
  SVN_GEN_DATAをコピーすることにより作成する。
  作成者は、プレスキャンのコントロールを行うプロセス
  (pqm or acqman)
  使用者は、再構成
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
/*	再構成時にレシーバーゲイン分の画像値補正を行うかどうかを
	指定するフラグ
	*/

#define SVN_PRC_GATE_VIEWSHARE_FLAG		(SVN_BASE_INDEPENDENT_PROCESS + 140)
#define SVT_PRC_GATE_VIEWSHARE_FLAG		VFT_INT32
#define SVS_PRC_GATE_VIEWSHARE_FLAG		"Gate View Share Image Flag"
/*
  (a) proMui/muiがセットする。０または１。
  (b) reconstructionが参照し、１の場合に中間時相画像を作成する。
	*/

#define SVN_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   (SVN_BASE_INDEPENDENT_PROCESS + 141)
#define SVT_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   VFT_BITFIELD
#define SVS_PRC_AUTO_DIFFUSION_ALIGNMENT_MODE   "Auto Diffusion Alignment Mode"
/*      自動Diffusion後処理アラインメント補正モード
(a)appcode
#define VFC_DIFFUSION_ALIGNMENT_REFER   1L << 0
#define VFC_DIFFUSION_ALIGNMENT_CALC            1L << 1
#define VFC_DIFFUSION_ALIGNMENT_CALC_SAVE       1L << 2
(b)proEdit,proDiffusionはディフュージョン後処理条件設定対話で指定されている値をPROTOCOLノードへセットする。
  Refer to parameters       : VFC_DIFFUSION_ALIGNMENT_REFER
  Calculate parameters      : VFC_DIFFUSION_ALIGNMENT_CALC
  Calculate/Save parameters : VFC_DIFFUSION_ALIGNMENT_CALC_SAVE
(c)msRecon, msPostProcはVFC_DIFFUSION_ALIGNMENT_REFERの場合には補正値データベースを参照して画像を補正し、VFC_DIFFUSION_ALIGNMENT_CALCの場合には補正値を算出して画像を補正する。また、VFC_DIFFUSION_ALIGNMENT_CALC_SAVEの場合は補正値を算出して画像を補正し補正値をデータベースへ登録する。
また、実際の補正値が再利用か算出したものかをPROCESSEDノードへセットする。

*/

#define SVN_PRC_RECON_NR_FLAG	(SVN_BASE_INDEPENDENT_PROCESS + 142)
#define SVT_PRC_RECON_NR_FLAG	VFT_INT32
#define SVS_PRC_RECON_NR_FLAG	"Noise Reduction flag for Reconstruction"
/* 再構成ノイズ リダクションフラグ 
0 : 再構成ノイズ リダクション処理無し
1 : 再構成ノイズ リダクション処理有り

フィールド自体が存在しない場合は再構成ノイズ リダクション処理無しとする。
*/

#define SVN_PRC_RECON_NR_SET	(SVN_BASE_INDEPENDENT_PROCESS + 143)
#define SVT_PRC_RECON_NR_SET	VFT_INT32
#define SVS_PRC_RECON_NR_SET	"Noise Reduction set ID for Reconstruction"
/* 再構成ノイズ リダクションセットパラメータID 
再構成ノイズ リダクション処理有りの場合、選択されたセットパラメータ番号をセットする。
再構成ノイズ リダクション処理無しの場合、データのセットを行わない。
対話表示時にこの値をデフォルトとして表示する。
値が-1またはフィールド自体が存在しない場合は再構成ノイズ リダクション処理無しとする。
*/
 
#define SVN_PRC_GOP_FILTER           (SVN_BASE_INDEPENDENT_PROCESS + 144)
#define SVT_PRC_GOP_FILTER           VFT_INT32
#define SVS_PRC_GOP_FILTER           "GOP filter ID"
/* GOPフィルタID
再構成ノイズ リダクション処理有りの場合、選択されたGOPパラメータ番号をセットする。
再構成ノイズ リダクション処理無しの場合、データのセットを行わない。
値が-1またはフィールド自体が存在しない場合はGOPフィルタ処理を行わない。
*/

#define SVN_PRC_K_SPACE_FILTER_NR          (SVN_BASE_INDEPENDENT_PROCESS + 145)
#define SVT_PRC_K_SPACE_FILTER_NR          VFT_INT32
#define SVS_PRC_K_SPACE_FILTER_NR          "K Space filter ID for Noise Reduction"
/* 再構成ノイズ リダクションk-spaceフィルタID
再構成ノイズ リダクション処理有りの場合、選択されたk-space filter番号をセットする。
再構成ノイズ リダクション処理無しの場合、データのセットを行わない。
*/
 
#define SVN_PRC_AUTO_MIP_FILTER_ID   (SVN_BASE_INDEPENDENT_PROCESS + 146)
#define SVT_PRC_AUTO_MIP_FILTER_ID   VFT_INT32
#define SVS_PRC_AUTO_MIP_FILTER_ID   "filter ID for auto MIP"
/* 自動MIP用フィルタID
自動MIPプレビューでノイズ リダクション処理有りの場合、選択されたMIP filter IDをセットする。
自動MIPプレビューでノイズ リダクション処理無しの場合、データのセットを行わない。
*/

#define SVN_PRC_AUTO_MIP_GOP_FILTER   (SVN_BASE_INDEPENDENT_PROCESS + 147)
#define SVT_PRC_AUTO_MIP_GOP_FILTER   VFT_INT32
#define SVS_PRC_AUTO_MIP_GOP_FILTER   "GOP filter ID for auto MIP"
/* 自動MIP用GOPフィルタID
自動MIPプレビューでノイズ リダクション処理有りの場合、選択されたGOPパラメータ番号をセットする。
自動MIPプレビューでノイズ リダクション処理無しの場合、データのセットを行わない。
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
/*	  Multi-Shot EPI / Hybrid EPI において、
	SVN_PRC_T2_CORR_METHODがVFC_T2_CORR_FOR_EPI_2SHOT_MODE
	の場合に使用するT2補正を行なう／行なわないを決定するしきい値
	*/

#define SVN_PRC_BLOCK_SORT_SIZE	(SVN_BASE_PROTOCOL_PROCESS + 1)
#define SVT_PRC_BLOCK_SORT_SIZE	VFT_INT32
#define SVS_PRC_BLOCK_SORT_SIZE	"Block sort table for 3D Hybrid EPI"
/*	３Dの Hybrid EPI において、ローデータを３DのFSEあるいは
	３DのFASEの並び方に並び変える順序を示すテーブル
	BLOCK_SORT table（１次元配列）の要素数を示す変数。
	値の範囲：1からnumEchoTrainの最大＊スライス方向の
	マトリクスの最大値まで 
	*/

#define SVN_PRC_BLOCK_SORT_TABLE	(SVN_BASE_PROTOCOL_PROCESS + 2)
#define SVT_PRC_BLOCK_SORT_TABLE	VFT_INT32
#define SVS_PRC_BLOCK_SORT_TABLE	"Block sort table"
/*	３Dの Hybrid EPI において、ローデータを３DのFSE
	あるいは３DのFASEの並び方に並び変える順序を示すテーブル
	BLOCK_SORT table（１次元配列、要素数はSVN_PRC_BLOCK_SORT_SIZE）
	並べ変え後のデータで第 i 番目のエコーブロックのデータは、
	並べ変え前の（収集時の）エコーデータの第 j 番目のエコー
	ブロックに相当するとすると、
	  j = SVN_PRC_BLOCK_SORT_TABLE[i]
	という対応になる。
	*/

#define SVN_PRC_PE_SRTTBL2	(SVN_BASE_PROTOCOL_PROCESS + 3)
#define SVS_PRC_PE_SRTTBL2	"Phase encode sort table for dual contrast scan"
/*	内容についてはSVN_PRC_PE_SRTTBLと同じ。
	*/

#define SVN_PRC_B_VALUE (SVN_BASE_PROTOCOL_PROCESS + 4)
#define SVT_PRC_B_VALUE VFT_INTVEC3
#define SVS_PRC_B_VALUE "B value: gradient factor"
/*
	動きによる位相シフトの度合を表すパラメータで、[s/m^2]の単位
	で表したもの。X,Y,Z各チャンネルの値を設定する。
*/

#define SVN_PRC_DIFF_CORR_METHOD (SVN_BASE_PROTOCOL_PROCESS + 5)
#define SVT_PRC_DIFF_CORR_METHOD VFT_INT32
#define SVS_PRC_DIFF_CORR_METHOD "navigator(diffusion)correction method"
/*
	diffusionにおけるナビゲータエコーによる補正方法を指定する。
	２要素の配列とし、
	[0]補正用データの制御方法
		(1)同じスキャンのデータを使用。
		(2)B=0のスキャンのデータを使用。
	[1]補正方法の種類としては、
		(1)ナビゲータデータをFTしたものをラインごとに乗算する。
		(2)なびげーたより0次、１次の補正量をフィッティングに
		   より求め、補正する。
		(3)ナビゲータより０次、１次の補正量をヒストグラムに
		   より求め、補正する。
	などがある。	
*/

#define SVN_PRC_ADC_IMAGE_PNO (SVN_BASE_PROTOCOL_PROCESS + 6)
#define SVT_PRC_ADC_IMAGE_PNO VFT_INT32
#define SVS_PRC_ADC_IMAGE_PNO "protocol No. utilized for ADC image"
/*
	ADC画像（拡散係数画像）を計算するのに使用した画像のプロトコル
	番号。複数設定される。
*/

#define SVN_PRC_PHASE_SHIFT_FLAG (SVN_BASE_PROTOCOL_PROCESS + 7)
#define SVT_PRC_PHASE_SHIFT_FLAG VFT_INT32
#define SVS_PRC_PHASE_SHIFT_FLAG "phase shift flag for reconstruction"
/*
	各方向に対する位相シフトの適用の可否をSE:PE:RO(1:2:4)のビット
	ORで記述。
*/

#define SVN_PRC_PHASE_SHIFT_VECTOR (SVN_BASE_PROTOCOL_PROCESS + 8)
#define SVT_PRC_PHASE_SHIFT_VECTOR VFT_FLT32
#define SVS_PRC_PHASE_SHIFT_VECTOR "phase shift vector(shift in pixcels for each axis)"
/*
	各方向に対するシフト量をピクセル単位で記述。シーケンス側で
	再構成に対し明示的に画像シフトを行わせたい時に指定。
*/

#define SVN_PRC_RO_INTERPOL_METHOD	(SVN_BASE_PROTOCOL_PROCESS + 9)
#define SVT_PRC_RO_INTERPOL_METHOD	VFT_UINT32
#define SVS_PRC_RO_INTERPOL_METHOD	"RO interpolation method"
/*	sin波駆動時EPIのデータ補間の動作切替え
	0x01->補間しない／する（I、Ｑchを独立に３次のスプライン補間）
	0x02->シフトしない／する
 */

#define SVN_PRC_RO_INTERPOL_DATA	(SVN_BASE_PROTOCOL_PROCESS + 10)
#define SVT_PRC_RO_INTERPOL_DATA	VFT_FLT32
#define SVS_PRC_RO_INTERPOL_DATA	"RO interpolation data"
/*	sin波駆動時EPIのデータ補間用のデータ
	SVN_GEN_DATA_DIM(RO方向）の個数のサンプリングデータの中で、
	第m番目のデータは、補間後の第SVN_PRC_RO_INTERPOL_DATA[m]番目の
	データに相当する。（実数であり、必ずしも整数ではない。）
	値の範囲：SVN_GEN_DATA_DIM(RO方向）/SVN_EXE_NUM_OVERSMPLをNとすると、
	0 ～ N-1の実数
	要素数 SVN_GEN_DATA_DIM（RO方向）の１次元配列
 */

#define SVN_PRC_AUTO_MIP_PREVIEW	(SVN_BASE_PROTOCOL_PROCESS + 11)
#define SVT_PRC_AUTO_MIP_PREVIEW	VFT_INT32
#define SVS_PRC_AUTO_MIP_PREVIEW	"Auto MIP Preview"
/*	要素数３の配列とする。各要素の意味合いは以下のとおり。
	フィールド自体が存在しない場合は自動MIP Previewを行わない。
	val[0] : action
	   = 0 : 何もしない
	   = 1 : 最大値投影
	   = 2 : Composite MIP 
	   = 3 : 最大値投影(３方向)
	   = 4 : 最大値投影(rotate)
	   = 5 : STAMD(Shift)
	val[1] : rate (千分率表記)
	   = 0 ～ 1000 (既定値：1000)
	val[2] : filter
	   = filter key (既定値：-1)
*/

#define SVN_PRC_REAL_DC_CORRECTION	(SVN_BASE_PROTOCOL_PROCESS + 12)
#define SVT_PRC_REAL_DC_CORRECTION	VFT_INT32
#define SVS_PRC_REAL_DC_CORRECTION	"real time dc correciton flag"
/*	このシーケンスがReal Time DC補正可能かどうかを示すフラグ
	可能ならば、	1
	不可ならば、	0
	*/

#define SVN_PRC_REAL_DC_CORR_FLAG	(SVN_BASE_PROTOCOL_PROCESS + 13)
#define SVT_PRC_REAL_DC_CORR_FLAG	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_FLAG	"real time dc correciton execution flag"
/*	Real Time DC補正を実行するかどうかを示すフラグ
	実行するならば、	1
	実行しないならば、	0
	*/

#define SVN_PRC_REAL_DC_CORR_PITCH	(SVN_BASE_PROTOCOL_PROCESS + 14)
#define SVT_PRC_REAL_DC_CORR_PITCH	VFT_FLT32
#define SVS_PRC_REAL_DC_CORR_PITCH	"real time dc correciton - pitch"
/*	Real Time DC補正用サンプリングピッチを指定する。
 */

#define SVN_PRC_REAL_DC_CORR_POINT	(SVN_BASE_PROTOCOL_PROCESS + 15)
#define SVT_PRC_REAL_DC_CORR_POINT	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_POINT	"real time dc correciton - point"
/*	Real Time DC補正用サンプリングポイントを指定する。
 */

#define SVN_PRC_REAL_DC_CORR_DELAY	(SVN_BASE_PROTOCOL_PROCESS + 16)
#define SVT_PRC_REAL_DC_CORR_DELAY	VFT_INT32
#define SVS_PRC_REAL_DC_CORR_DELAY	"real time dc correciton - delay"
/*	Real Time DC補正用サンプリングディレイを指定する。
 */

#define SVN_PRC_REMAIN_CORR_FLAG	(SVN_BASE_PROTOCOL_PROCESS + 17)
#define SVT_PRC_REMAIN_CORR_FLAG	VFT_INT32
#define SVS_PRC_REMAIN_CORR_FLAG	"remain correciton flag"
/*	残差補正実行制御フラグ。
	実行するならば、	1
	実行しないならば、	0
	*/

#define SVN_PRC_B_TABLE	(SVN_BASE_PROTOCOL_PROCESS + 18)
#define SVT_PRC_B_TABLE	VFT_INT32
#define SVS_PRC_B_TABLE	"b value tables of the every contrast images in a simultaneous scan."
/*	拡散強調イメージングにおいて再構成の際に、作成された画像につけるb値
	(拡散強調の程度を表すパラメータ)の値を、参照するためのテーブル。
	画像1に対しては、1～3番目のテーブルの値をSVN_PRC_B_VALUEにコピーする。
	画像2に対しては、4～6番目のテーブルの値をSVN_PRC_B_VALUEにコピーする。
	画像3に対しては、7～9番目のテーブルの値をSVN_PRC_B_VALUEにコピーする。
	画像4に対しては、10～12番目のテーブルの値をSVN_PRC_B_VALUEに
	コピーする。
	設定者:	seqgen
	At the diffusion imaging reconstruction, appropriate part of
	this table are copied to SVN_PRC_B_VALUE.
	*/

#define SVN_PRC_AUTO_DIFFUSION_ADC 		(SVN_BASE_PROTOCOL_PROCESS + 19)
#define SVT_PRC_AUTO_DIFFUSION_ADC 		VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ADC		"Auto Diffusion ADC Flag"
/* 自動Diffusion後処理ADC画像作成 実行フラグ
 *	(a) proEdit,proDiffusionはディフュージョン後処理条件設定対話で
 *          指定されている値をセットする。
 *		Diffusion ADC がON		: True(1)
 *		Diffusion ADC がOFF		: False(0)
 *	(b) msRecon,msPostProcはTrue(1)の場合ADC画像を作成する。
*/

#define SVN_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	(SVN_BASE_PROTOCOL_PROCESS + 20)
#define SVT_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	VFT_FLT32
#define SVS_PRC_AUTO_DIFFUSION_ADC_THRESHOLD	"Auto Diffusion ADC Threshold"
/* 自動Diffusion後処理ADC画像作成 閾値	
 *	(a) proEdit,proDiffusionはディフュージョン後処理条件設定対話で
 *	    指定されている閾値(下限値)をセットする。
 *	(b) msRecon,msPostProcはADC画像作成時この値を用いて閾値処理する。
 */

#define SVN_PRC_AUTO_DIFFUSION_ISOTROPIC 	(SVN_BASE_PROTOCOL_PROCESS + 21)
#define SVT_PRC_AUTO_DIFFUSION_ISOTROPIC 	VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ISOTROPIC	"Auto Diffusion ADC Flag"
/* 自動Diffusion後処理ISOTROPIC画像作成 実行フラグ
 *	(a) proEdit,proDiffusionはディフュージョン後処理条件設定対話で
 *	    指定されている値をセットする。
 *		Diffusion Isotropic がON	: True(1)
 *		Diffusion Isotropic がOFF	: False(0)
 *	(b) msRecon,msPostProcはTrue(1)の場合Isotropic画像を作成する。
 */

#define SVN_PRC_AUTO_PERFUSION	 	(SVN_BASE_PROTOCOL_PROCESS + 22)
#define SVT_PRC_AUTO_PERFUSION	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION		"Auto Dynamic Purfusion Flag"
/* 自動Perfusion後処理 実行フラグ	
 *	(a) proEdit,proPerfusionはパフュージョン後処理条件設定対話で
 *	    指定されている値をセットする。
 *		Dynamic delta R2* がON		: True(1)
 *		Dynamic delta R2* がOFF		: False(0)
 *	(b) msRecon,msPostProcはTrue(1)の場合Dynamic delta R2*画像を作成する。
 */

#define SVN_PRC_AUTO_PERFUSION_THRESHOLD	(SVN_BASE_PROTOCOL_PROCESS + 23)
#define SVT_PRC_AUTO_PERFUSION_THRESHOLD	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_THRESHOLD	"Auto Dynamic Perfusion Threshold"
/* 自動Perfusion後処理 閾値	
 *	(a) 大きさ２の配列で、
 *		配列要素１つめ	：	下限値
 *		配列要素２つめ	：	上限値
 *	(b) proEdit,proPerfusionはパフュージョン後処理条件設定対話で
 *	    閾値として指定されている下限値、上限値をそれぞれセットする。
 *	(c) msRecon,msPostProcはこの値をdelta R2*画像作成時の閾値として用いる。
 */

#define SVN_PRC_AUTO_PERFUSION_BASE	 	(SVN_BASE_PROTOCOL_PROCESS + 24)
#define SVT_PRC_AUTO_PERFUSION_BASE	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION_BASE		"Auto Dynamic Purfusion Base Images"
/* 自動Perfusion後処理 基準画像
 *	(a) 大きさ２の配列で、
 *		配列要素１つめ	：	基準画像の開始時相
 *		配列要素２つめ	：	基準画像の画像数
 *	(b) proEdit,proPerfusionはパフュージョン後処理条件設定対話で
 *	    基準画像として指定されている開始時相と画像数をそれぞれセットする。
 *	(c) msRecon,msPostProcはこの値をもとに平均画像を作成しdelta R2*画像
 *	    作成時の基準画像とする。
 */

#define SVN_PRC_AUTO_PERFUSION_ZOOM	 	(SVN_BASE_PROTOCOL_PROCESS + 25)
#define SVT_PRC_AUTO_PERFUSION_ZOOM	 	VFT_INT32
#define SVS_PRC_AUTO_PERFUSION_ZOOM		"Auto Dynamic Purfusion Zoom"
/* 自動Perfusion後処理 時間方向補間倍率
 *	(a) proEdit,proPerfusionは自動パフュージョン後処理条件設定対話で
 *	    時間方向補間倍率として指定されている値(1～4)をセットする。
 *	(b) msRecon,msPostProcはこの値をもとに時間方向補間を行って
 *	    delta R2*画像を作成する。
 */

#define SVN_PRC_AUTO_PERFUSION_MAPPING	 	(SVN_BASE_PROTOCOL_PROCESS + 26)
#define SVT_PRC_AUTO_PERFUSION_MAPPING	 	VFT_BITFIELD
#define SVS_PRC_AUTO_PERFUSION_MAPPING		"Auto Dynamic Purfusion Map Images"
/* 自動Perfusion後処理 マップ画像
 *	(a) appcodeとして以下を持つ。
 *		VFC_PERFUSION_PH     (1L<<0)  Peak Height
 *		VFC_PERFUSION_PT     (1L<<1)  Peak Time
 *		VFC_PERFUSION_AT     (1L<<2)  Appearance Time
 *		VFC_PERFUSION_DT     (1L<<3)  Disappearance Time
 *		VFC_PERFUSION_AC     (1L<<4)  Area Under Curve (rCBV相当)
 *		VFC_PERFUSION_rMT1   (1L<<5)  relative Mean Transit Time
 *		VFC_PERFUSION_MT2    (1L<<6)  2'nd Moment
 *		VFC_PERFUSION_TT     (1L<<7)  Transit Time
 *		VFC_PERFUSION_rFlow  (1L<<8)  relative Flow（rCBF相当）
 *		VFC_PERFUSION_US     (1L<<9)  Up Slope
 *		VFC_PERFUSION_DS     (1L<<10) Down Slope
 *		VFC_PERFUSION_PTe    (1L<<11) effective Peak Time
 *		VFC_PERFUSION_rMT1e  (1L<<12) effective Mean Transit Time
 *		VFC_PERFUSION_rFlowe (1L<<13) effective relative Flow
 *	(b) proEdit,proPerfusionはパフュージョン後処理条件設定対話で
 *	    指定されているマップ画像を上記appcodeのビット毎の論理和で
 *	    セットする。
 *	(c) msRecon,msPostProcは指定された分マップ画像を作成する。
 */

#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION	(SVN_BASE_PROTOCOL_PROCESS + 27)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION	VFT_BITFIELD
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION	"Auto Dynamic Subtraction Flag"
/* ダイナミック差分処理 実行フラグ
 *	(a) proEdit,proMuiはダイナミック差分設定対話で指定されている値を
 *	    セットする。
 *		ダイナミック差分がON	: True(1)
 *		ダイナミック差分がOFF	: False(0)
 *	(b) msRecon,msPostProcはTrue(1)の場合、SVN_PRC_AUTO_DYNAMIC_
 *	    SUBTRACTION_IMAGESで指定されている画像を作成する。False(0)の
 *	    場合は通常のダイナミック再構成を行う。
 */

#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	(SVN_BASE_PROTOCOL_PROCESS + 28)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	VFT_BITFIELD
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES	"Auto Dynamic Subtraction Images"
/* ダイナミック差分処理 画像種
 *	(a) appcodeとして以下を持つ。
 *		VFC_SUBTRACTION_NONE	 (1L << 0) 差分なし画像作成
 *		VFC_SUBTRACTION_ABSOLUTE (1L << 1) 絶対値差分画像作成 
 *		VFC_SUBTRACTION_COMPLEX	 (1L << 2) 複素差分画像作成 
 *	(b) proEdit,proMuiはダイナミック差分設定対話で指定されている
 *	    画像種を上記appcodeのビット毎の論理和でセットする。
 *	(c) msReconは指定された画像種分の再構成を行う。VFC_SUBTRACTION_
 *	    ABSOLUTEが指定されている場合は、msPostProcを用いて絶対値
 *	    差分処理を行う。ただし0が指定されている場合は通常の
 *	    ダイナミック画像を作成する。
 */
#define SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE 	(SVN_BASE_PROTOCOL_PROCESS + 29)
#define SVT_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE 	VFT_INT32
#define SVS_PRC_AUTO_DYNAMIC_SUBTRACTION_BASE	"Dynamic Subtraction Base Phase"
/* ダイナミック差分処理 基準時相
 *	(a) proEdit,proMuiは自動ダイナミック差分設定対話で指定されて
 *	    いる基準時相をセットする。
 *	(b) msRecon,msPostProcはこの時相とそれ以降の時相との差分を行う。
 */

#define SVN_PRC_RECON_COUNTER		(SVN_BASE_PROTOCOL_PROCESS + 30)
#define SVT_PRC_RECON_COUNTER		VFT_INT32
#define SVS_PRC_RECON_COUNTER		"Recon Processing Counter"
/* 再構成再帰呼び出しカウンタ
 *	(a) msReconはこのカウンタが再構成条件から求めた再帰呼び出し
 *	    回数に達するまで再構成処理を繰り返す。再構成を起動した
 *	    場合に１インクリメントする。フィールドがない場合は０
 *	    として扱う。
 */

#define SVN_PRC_RECON_ENGINE	(SVN_BASE_PROTOCOL_PROCESS + 31)
#define SVT_PRC_RECON_ENGINE	VFT_INT32
#define SVS_PRC_RECON_ENGINE	"Reconstruction Processing Engine Type"
/*	再構成演算器のタイプ。
	appcode
	VFC_RECON_HOST : 0 (HOST)
	VFC_RECON_RACE : 1 (RACE)
	VFC_RECON_VAP  : 2 (VAP)
	*/

#define SVN_PRC_RECON_AVERAGE_METHOD	(SVN_BASE_PROTOCOL_PROCESS + 32)
#define SVT_PRC_RECON_AVERAGE_METHOD	VFT_UINT32
#define SVS_PRC_RECON_AVERAGE_METHOD	"Recon Average Method"
/*	再構成のアベレージング方法を指定する。
  (a)appcodeとして以下をもつ。
   #define VFC_RECON_AVERAGE_NONE		0	
	   再構成側でのアベレージングなし。
   #define VFC_RECON_AVERAGE_SPLICE		1	
	   diffusion撮影のSPLICEモードで使用。
	   画像データでの平均加算。
	   加算時のデータ形式	は、PDATA_FORMによる。
   #define VFC_RECON_AVERAGE_FISP_CISS		2	
	   FISP撮影のCISSモードで使用。
   #define VFC_RECON_AVERAGE_FISP_SIMCAST	3
	   FISP撮影のSIMCASTモードで使用。

*/

#define SVN_PRC_RECON_AVERAGE_CLUSTER_NUM	(SVN_BASE_PROTOCOL_PROCESS + 33)
#define SVT_PRC_RECON_AVERAGE_CLUSTER_NUM	VFT_INT32
#define SVS_PRC_RECON_AVERAGE_CLUSTER_NUM	"Number of Recon Average Cluster"
/*	再構成でアベレージングする塊の数を指定する。
 */

#define SVN_PRC_AUTO_PERFUSION_AAT	(SVN_BASE_PROTOCOL_PROCESS + 34)
#define SVT_PRC_AUTO_PERFUSION_AAT	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_AAT	"Auto Dynamic Perfusion AT Ratio"
/*	ATマップ画像を求めるためのPH(最大値)に対する割合(0.0～1.0)。
 */

#define SVN_PRC_AUTO_PERFUSION_ADT	(SVN_BASE_PROTOCOL_PROCESS + 35)
#define SVT_PRC_AUTO_PERFUSION_ADT	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_ADT	"Auto Dynamic Perfusion DT Ratio"
/*	DTマップ画像を求めるためのPH(最大値)に対する割合(0.0～1.0)。
 */

#define SVN_PRC_AUTO_PERFUSION_ACMIN	(SVN_BASE_PROTOCOL_PROCESS + 36)
#define SVT_PRC_AUTO_PERFUSION_ACMIN	VFT_FLT32
#define SVS_PRC_AUTO_PERFUSION_ACMIN	"Auto Dynamic Perfusion AC min"
/*	ACマップ画像を求めるための最小値(0.0～10000.0)。
 */


#define SVN_PRC_PROMPTER_SCRIPT (SVN_BASE_PROTOCOL_PROCESS + 37)
#define SVT_PRC_PROMPTER_SCRIPT VFT_STRING
#define SVS_PRC_PROMPTER_SCRIPT "Prompter script"
/*      自動処理で処理されるスクリプト。PQMが生成し、prompter によって解釈される。
*/

#define SVN_PRC_GATE_VIEWSHARE_RATIO	(SVN_BASE_PROTOCOL_PROCESS + 38)
#define SVT_PRC_GATE_VIEWSHARE_RATIO	VFT_FLT32
#define SVS_PRC_GATE_VIEWSHARE_RATIO	"Gate View Share Ratio"
/*
  (a) proMui/Muiがセット。有効値は1.0, 2.0, 4.0。
  (b) reconstructionが参照し中間時相画像を作成する。
*/

#define SVN_PRC_DRKS_FLAG       (SVN_BASE_PROTOCOL_PROCESS + 39)
#define SVT_PRC_DRKS_FLAG       VFT_INT32
#define SVS_PRC_DRKS_FLAG       "DRKS flag"
/*      DRKS撮影実行フラグ
		0: DRKS Off、 1: DRKS On
*/

#define SVN_PRC_PARENT_INDEX_FOR_PLANNING       (SVN_BASE_PROTOCOL_PROCESS + 41)
#define SVT_PRC_PARENT_INDEX_FOR_PLANNING       VFT_INT32
#define SVS_PRC_PARENT_INDEX_FOR_PLANNING       "Available Parent Image Index for Planning"
/*      親画像可能プロセスドインデックス番号
(a)位置決め時の親として使用可能な画像のプロセスドノードのインデックス番号
(b)再構成がセットし、位置決め処理が参照する。

*/

#define SVN_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   (SVN_BASE_PROTOCOL_PROCESS + 42)
#define SVT_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   VFT_INT32
#define SVS_PRC_AUTO_DIFFUSION_ALIGNMENT_FLAG   "Auto Diffusion Alignment Flag"
/*      自動Diffusion後処理アラインメント画像作成 実行フラグ
(a)proEdit,proDiffusionはディフュージョン後処理条件設定対話で指定されている値をセットする。
        Alignment がON  : True(1)
        Alignment がOFF : False(0)
(b)msRecon,msPostProcはTrue(1)の場合Alignment画像を作成し、これを原画像として各diffusionマップ画像を作成する。


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
(b)proEdit,proDiffusionはディフュージョン後処理条件設定対話で指定されている値をPROTOCOLノードへセットする。
  FAがON           : VFC_DIFFUSION_FA
  L1,L2,L3がON     : VFC_DIFFUSION_L1 | VFC_DIFFUSION_L2 | VFC_DIFFUSION_L3
  V1x,V1y,V1zがON  : VFC_DIFFUSION_V1X | VFC_DIFFUSION_V1Y | VFC_DIFFUSION_V1Z
(c)msRecon,msPostProcは各ビットに従い、diffusionマップ画像を作成する。

*/

#define SVN_PRC_ALIGN_SHIFT_IN_PE       (SVN_BASE_PROTOCOL_PROCESS + 44)
#define SVT_PRC_ALIGN_SHIFT_IN_PE       VFT_FLT32
#define SVS_PRC_ALIGN_SHIFT_IN_PE       "Shift in PE direction at Alignment"
/*      diffusion-alignment処理にてPE方向にシフトする点数
(a)alignment処理した時に使用した補正値を、msPostProcが登録する。
(b)スライス枚数分の配列。


*/

#define SVN_PRC_ALIGN_ORIENT_KEY        (SVN_BASE_PROTOCOL_PROCESS + 45)
#define SVT_PRC_ALIGN_ORIENT_KEY        VFT_INT32
#define SVS_PRC_ALIGN_ORIENT_KEY        "Image Orient Key at Alignment"
/*      diffusion-alignment処理データベース画像方向に関するキー
(a)アラインメント処理のデータベースを検索するキーのひとつ。シーケンスが設定し、Refer to parameter時に後処理が参照する。
*/

#define SVN_PRC_AUTO_MIP_BASE   (SVN_BASE_PROTOCOL_PROCESS + 46)
#define SVT_PRC_AUTO_MIP_BASE   VFT_FLTVEC3
#define SVS_PRC_AUTO_MIP_BASE   "Base angle for auto MIP "
/*      自動ＭＩＰ基準角
*/

#define SVN_PRC_AUTO_MIP_SLICES (SVN_BASE_PROTOCOL_PROCESS + 47)
#define SVT_PRC_AUTO_MIP_SLICES VFT_INT32
#define SVS_PRC_AUTO_MIP_SLICES "Number of slices for auto MIP "
/*      自動ＭＩＰスライス枚数
*/

#define SVN_PRC_AUTO_MIP_ANGLE  (SVN_BASE_PROTOCOL_PROCESS + 48)
#define SVT_PRC_AUTO_MIP_ANGLE  VFT_FLT32
#define SVS_PRC_AUTO_MIP_ANGLE  "Step of projective angle for auto MIP "
/*      自動ＭＩＰ投影角ステップ
*/

#define SVN_PRC_INTENSITY_MAP_TYPE  (SVN_BASE_PROTOCOL_PROCESS + 49)
#define SVT_PRC_INTENSITY_MAP_TYPE  VFT_BITFIELD
#define SVS_PRC_INTENSITY_MAP_TYPE  "Intensity Map Type"
/*      輝度補正プレスキャンであることを示す情報
　SVN_PLN_SPEEDER_TYPE=1の場合にコイルDBを見てPQMが書き込む。
　　SPEEDERコイルの場合→3(1|2)
    輝度補正適用のみのコイルの場合→1
    輝度補正／SPEEDER感度補正マップの作成タイプ。
        何もしない       VFC_INTENSITY_MAP_TYPE_NONE         0
        輝度補正マップ   VFC_INTENSITY_MAP_TYPE_INTENSITY    (1<<0)
        感度補正マップ   VFC_INTENSITY_MAP_TYPE_SPEEDER      (1<<1)

	全マップ(輝度補正マップ、感度補正マップ、WB感度補正マップ) 3(1|2)
*/

#define SVN_PRC_INTENSITY_CORR_FLAG  (SVN_BASE_PROTOCOL_PROCESS + 50)
#define SVT_PRC_INTENSITY_CORR_FLAG  VFT_INT32
#define SVS_PRC_INTENSITY_CORR_FLAG  "Intensity Correction Flag"
/* 
  輝度補正処理を適用するしないのフラグ
　prolimのしきたりで、On/Offフラグが必要。
　付帯情報としてDISPLAYに表示される。（係数を表示するかは未定）
　PQMは撮影直前に輝度補正マップの有無確認し、マップなければ警告表示後Offする。
      輝度補正のOFF/ON。
        輝度補正OFF VFC_INTENSITY_CORR_OFF    0
        輝度補正ON  VFC_INTENSITY_CORR_ON     1
*/

#define SVN_PRC_INTENSITY_CORR_RATIO  (SVN_BASE_PROTOCOL_PROCESS + 51)
#define SVT_PRC_INTENSITY_CORR_RATIO  VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_RATIO  "Intensity Correction Ratio"
/* 
   輝度補正係数。
   補正係数のフィールド：値はfloat。proMui/muiでは0-100%の指定。

*/

#define SVN_PRC_PAC_CORRECT_ACTION  (SVN_BASE_PROTOCOL_PROCESS + 52)
#define SVT_PRC_PAC_CORRECT_ACTION  VFT_INT32
#define SVS_PRC_PAC_CORRECT_ACTION  "PAC Correction Action"
/* 
   ch間位相差,ゲイン差補正を行なうかのフラグ。
   ビット毎に指定。
   0x01 SVN_EXE_PAC_PHASE_CORRECT_PARAMS による補正 0:OFF,1:ON
   0x02 SVN_EXE_PAC_GAIN_CORRECT_PARAMS による補正  0:OFF,1:ON

    #define VFC_PAC_PHASE_CORRECT  1
    #define VFC_PAC_GAIN_CORRECT   (1<<1)
*/

#define SVN_PRC_DIRECT_QUEUE  (SVN_BASE_PROTOCOL_PROCESS + 53)
#define SVT_PRC_DIRECT_QUEUE  VFT_INT32
#define SVS_PRC_DIRECT_QUEUE  "Reconstruction Direct Queue Flag"
/* 
   2D/3D/postProc以外の再構成キュー(Direct Queue)にこのプロセスを
   積むかどうかPQMが指定する。再構成はこのフラグが１の場合に
   Direct Queueに積む。
   0 normal
   1 Direct Queue

    #define VFC_SET_NORMAL_QUEUE 0
    #define VFC_SET_DIRECT_QUEUE 1
*/

#define SVN_PRC_RECON_START_AFTER_REFVIEW  (SVN_BASE_PROTOCOL_PROCESS + 54)
#define SVT_PRC_RECON_START_AFTER_REFVIEW  VFT_INT32
#define SVS_PRC_RECON_START_AFTER_REFVIEW  "Reconstruction Start after RefView Flag"
/* 
   DRKSのRefView再構成の時に3Dの本再構成の開始タイミングを決めるフラグ

   #define VFC_AFTER_REFVIEW_AUTO	0
   #define VFC_AFTER_REFVIEW_MANUAL	1

   VFC_AFTER_REFVIEW_AUTOの時
       RefView再構成終了後すぐに本再構成を開始する。
    
   VFC_AFTER_REFVIEW_MANUALの時
       RefView再構成終了後再構成を起動しない。再々構成により本再構成を行う。
*/

#define SVN_PRC_SCRIPT_STATUS  (SVN_BASE_PROTOCOL_PROCESS + 55)
#define SVT_PRC_SCRIPT_STATUS  VFT_INT32
#define SVS_PRC_SCRIPT_STATUS  "Prompter Script Status"
/*  自動転送終了後に転送された画像のprocessedIDのリストが格納される。
*/

#define SVN_PRC_AUTO_GDC_ACTION	  (SVN_BASE_PROTOCOL_PROCESS + 56)
#define SVT_PRC_AUTO_GDC_ACTION	  VFT_INT32
#define SVS_PRC_AUTO_GDC_ACTION	  "Auto Geometric Distortion Correction - Action"
/* 
   自動磁場歪み補正処理フラグ
   #define VFC_GDC_ACTION_OFF   0
   #define VFC_GDC_ACTION_ON    1
*/

#define SVN_PRC_AUTO_GDC_DIM	  (SVN_BASE_PROTOCOL_PROCESS + 57)
#define SVT_PRC_AUTO_GDC_DIM	  VFT_INT32
#define SVS_PRC_AUTO_GDC_DIM	  "Auto Geometric Distortion Correction - Dimension"
/* 
   自動磁場歪み補正Dimension
   #define VFC_GDC_DIM_2D   2
   #define VFC_GDC_DIM_3D   3
*/

#define SVN_PRC_AUTO_GDC_FILTER	  (SVN_BASE_PROTOCOL_PROCESS + 58)
#define SVT_PRC_AUTO_GDC_FILTER	  VFT_INT32
#define SVS_PRC_AUTO_GDC_FILTER	  "Auto Geometric Distortion Correction - Filter"
/* 
   磁自動場歪み補正フィルター
*/

#define SVN_PRC_AUTO_ALT_SUBTRACTION	(SVN_BASE_PROTOCOL_PROCESS + 59)
#define SVT_PRC_AUTO_ALT_SUBTRACTION	VFT_INT32
#define SVS_PRC_AUTO_ALT_SUBTRACTION	"Auto Alternated Subtraction Flag"
/* tSLIT差分処理 実行フラグ
   ALTERNATED_ACQ_FLAGがONのときのみproMuiで指定できる。
 */

#define SVN_PRC_SUBTRACTION_POLARITY	(SVN_BASE_PROTOCOL_PROCESS + 60)
#define SVT_PRC_SUBTRACTION_POLARITY	VFT_INT32
#define SVS_PRC_SUBTRACTION_POLARITY	"tSLIT Subtraction Polarity"
/* tSLIT差分処理時の極性
   seqgenが設定する。
 */


#define SVN_PRC_AMB_SCAN_LOOP_ID	(SVN_BASE_PROTOCOL_PROCESS + 61)
#define SVT_PRC_AMB_SCAN_LOOP_ID	VFT_INT32
#define SVS_PRC_AMB_SCAN_LOOP_ID	"AMB scan loop ID"
/* SVN_PLN_AMB_MODE_FLG がAMB撮像での時有効
   PQMがセットしてDisplayが表示される情報
   val[0]:カレントSEG
   val[1]:カレント繰り返し回数
   val[2]:総SEG
 */

#define SVN_PRC_GDC_FLAG                (SVN_BASE_PROTOCOL_PROCESS + 62)
#define SVT_PRC_GDC_FLAG                VFT_INT32
#define SVS_PRC_GDC_FLAG                "GDC flag"
/* 
   磁場歪み補正処理フラグ
   0 : 磁場歪み補正処理無し
   1 : 磁場歪み補正処理有り

   #define VFC_GDC_FLAG_OFF    0
   #define VFC_GDC_FLAG_ON     1
 */

#define SVN_PRC_GDC_DIM                 (SVN_BASE_PROTOCOL_PROCESS + 63)
#define SVT_PRC_GDC_DIM                 VFT_INT32
#define SVS_PRC_GDC_DIM                 "GDC Dimension"
/* 
   磁場歪み補正処理Dimension
   2 : 2D処理
   3 : 3D処理

   #define VFC_GDC_DIMENSION_2D    2
   #define VFC_GDC_DIMENSION_3D    3   
 */

#define SVN_PRC_GDC_LOCATOR_IMAGE       (SVN_BASE_PROTOCOL_PROCESS + 64)
#define SVT_PRC_GDC_LOCATOR_IMAGE       VFT_INT32
#define SVS_PRC_GDC_LOCATOR_IMAGE       "GDC locator image flag"
/* 
   磁場歪み補正処理位置決め画像フラグ
   0 : 位置決め画像登録無し
   1 : 位置決め画像登録有り

   #define VFC_GDC_LOCATOR_IMAGE_OFF   0
   #define VFC_GDC_LOCATOR_IMAGE_ON    1
 */

#define SVN_PRC_GDC_LOCATOR_MASK        (SVN_BASE_PROTOCOL_PROCESS + 65)
#define SVT_PRC_GDC_LOCATOR_MASK        VFT_INT32
#define SVS_PRC_GDC_LOCATOR_MASK        "GDC locator mask flag"
/* 
   磁場歪み補正処理位置決め画像フラグ
   0 : 位置決め画像Mask無し
   1 : 位置決め画像Mask有り

   #define VFC_GDC_LOCATOR_MASK_OFF   0
   #define VFC_GDC_LOCATOR_MASK_ON    1
 */

#define SVN_PRC_INTENSITY_CORR_VIVID1        (SVN_BASE_PROTOCOL_PROCESS + 66)
#define SVT_PRC_INTENSITY_CORR_VIVID1        VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_VIVID1        "Intensity Correction vivid1"
/* 
   ノイズの不均一性補正の程度を表す係数

   ノイズの不均一性補正の程度を表すプライマリなパラメータ。
   0では補正なし、値を上げるのに従って不均一性補正の程度が強くなる。
   設定可能範囲は、0%～100%
 */

#define SVN_PRC_INTENSITY_CORR_VIVID2        (SVN_BASE_PROTOCOL_PROCESS + 67)
#define SVT_PRC_INTENSITY_CORR_VIVID2        VFT_FLT32
#define SVS_PRC_INTENSITY_CORR_VIVID2        "Intensity Correction vivid2"
/* 
   ノイズの不均一性補正の程度を表す係数

   補正後の画質を表す。0%が標準的な基準となる撮像視野の中心で原画に近くなる。
   0%を超えるとスムージングが強めで、0%より小さくなるとエッジ強調気味の画像が
   得られる。
   設定可能範囲は、-50%～50%
 */

#define SVN_PRC_XYRATIO_FOR_JET             (SVN_BASE_PROTOCOL_PROCESS + 68)
#define SVT_PRC_XYRATIO_FOR_JET             VFT_FLT32
#define SVS_PRC_XYRATIO_FOR_JET             "XYratio for JET"
/* 
   JET再構成に使用するRO方向とPE方向のK空間上での分解能の比率dKx/dKy

   シーケンスが計算し、再構成が使用する。

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_MOTION_CORR_FLAG_FOR_JET    (SVN_BASE_PROTOCOL_PROCESS + 69)
#define SVT_PRC_MOTION_CORR_FLAG_FOR_JET    VFT_BITFIELD
#define SVS_PRC_MOTION_CORR_FLAG_FOR_JET    "Motion correction flag for JET"
/* 
   JETの動き補正フラグ

   JET後処理系のフィールド。
   回転(rotation) 並進(translation) 重み付け(rejection)のBIT。

   設定者 PROLIM,recon,sequence
   使用者 recon

   #define VFC_MOTION_CORR_FLAG_FOR_JET_ROTATION    (1L << 0)
   #define VFC_MOTION_CORR_FLAG_FOR_JET_TRANSLATION (1L << 1)
   #define VFC_MOTION_CORR_FLAG_FOR_JET_REJECTION   (1L << 2)
 */

#define SVN_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      (SVN_BASE_PROTOCOL_PROCESS + 70)
#define SVT_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      VFT_INT32
#define SVS_PRC_MOTION_CORR_OPT_METHOD_FOR_JET      "Motion correction option method for JET"
/* 
   動き補正パラメータの最適化アルゴリズムの指定

   設定者 PROLIM,recon,sequence 
   使用者 recon

   #define VFC_MOTION_CORR_OPT_METHOD_FOR_JET_NONE          0
 */

#define SVN_PRC_REGRIDDING_METHOD_FOR_JET   (SVN_BASE_PROTOCOL_PROCESS + 71)
#define SVT_PRC_REGRIDDING_METHOD_FOR_JET   VFT_INT32
#define SVS_PRC_REGRIDDING_METHOD_FOR_JET   "Regridding method for JET"
/* 
   JETのCartesian座標系へのRegridding方法を指定するフラグ。

   JET後処理系のフィールド。

   設定者 PROLIM,recon,sequence
   使用者 recon

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
   K空間の掃引軌道のソート方法

   SVN_PLN_K_SPACE_TRAJECTORY_TYPE != NONEの場合に起動されるsortingプロセスにおいて
   sortingやSVN_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHODに応じた位相補正を行うか
   どうかのフラグ。
   USE_DEFAULTでは、((DATA)shot)Ec のループを ((DATA)Ec)Shotと並べる。
   SORT_FREEを指定すると、SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLEに従ってソートする。

   設定者 sequence
   使用者 recon

   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_NONE         0
   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_USE_DEFAULT  1
   #define VFC_K_SPACE_TRAJECTORY_SORT_METHOD_SORT_FREE    2
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    (SVN_BASE_PROTOCOL_PROCESS + 73)
#define SVT_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_PHASE_CORR_METHOD    "K space trajectory phase correction method"
/* 
   K空間の掃引軌道の位相補正方法

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLEを使用した位相補正方法を指定する。
   V9.20では位相補正は行わない。

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  (SVN_BASE_PROTOCOL_PROCESS + 74)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_BITS  "K space trajectory sort table bits"
/* 
   K空間の掃引軌道ソートテーブルのビット数

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLEのindex(行き先)を指定するために使用されるBIT数。
   上位BITはそのデータの属性を表現する。

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       (SVN_BASE_PROTOCOL_PROCESS + 75)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE       "Sort table of k space trajectory"
/* 
   K空間の掃引軌道ソートテーブル

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_METHODがSORT_FREEの場合に使用するsort table。

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  (SVN_BASE_PROTOCOL_PROCESS + 76)
#define SVT_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_SORT_TABLE_SIZE  "Sort table size of k space trajectory"
/*
   K空間の掃引軌道ソートテーブルのサイズ

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   (SVN_BASE_PROTOCOL_PROCESS + 77)
#define SVT_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   VFT_INT32
#define SVS_PRC_K_SPACE_TRAJECTORY_T2_CORR_METHOD   "T2 correction of k space trajectory"
/*
   K空間の掃引軌道の振幅補正方法

   SVN_PRC_K_SPACE_TRAJECTORY_SORT_TABLEを使用した振幅補正方法を指定する。
   V9.20では振幅補正は行わない。

   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_DC_CORR_METHOD_FOR_JET              (SVN_BASE_PROTOCOL_PROCESS + 78)
#define SVT_PRC_DC_CORR_METHOD_FOR_JET              VFT_INT32
#define SVS_PRC_DC_CORR_METHOD_FOR_JET              "DC correction method for JET"
/*
   JET用再構成プロセス前段でDC補正を行うためのフラグ

   #define VFC_DC_CORR_METHOD_FOR_JET_NONE              0
   V9.20ではDC補正は行わない。


   設定者 sequence
   使用者 recon
 */

#define SVN_PRC_DUAL_ECHO_COMBINING_TYPE            (SVN_BASE_PROTOCOL_PROCESS + 79)
#define SVT_PRC_DUAL_ECHO_COMBINING_TYPE            VFT_INT32
#define SVS_PRC_DUAL_ECHO_COMBINING_TYPE            "operation type for combined dual echo"
/*
   2エコー画像における自動合成画像処理のOn/Offおよび演算タイプの指定

   #define VFC_DUAL_ECHO_COMBINING_NONE                 0
   #define VFC_DUAL_ECHO_COMBINING_ABS_ADD              1
   #define VFC_DUAL_ECHO_COMBINING_ABS_SUB1             2
   #define VFC_DUAL_ECHO_COMBINING_ABS_SUB2             3
   #define VFC_DUAL_ECHO_COMBINING_FWS_WEAK             4
   #define VFC_DUAL_ECHO_COMBINING_FWS_MEDIUM           5
   #define VFC_DUAL_ECHO_COMBINING_FWS_STRONG           6

   設定者 PROLIM
   使用者 recon
 */

#define SVN_PRC_DUAL_ECHO_COMBINING_RATIO            (SVN_BASE_PROTOCOL_PROCESS + 80)
#define SVT_PRC_DUAL_ECHO_COMBINING_RATIO            VFT_FLT32
#define SVS_PRC_DUAL_ECHO_COMBINING_RATIO            "weighting factors for combined dual echo"
/*
   2エコー画像における自動合成画像処理にて使用する各エコーの重み係数

   ratio[0]:1エコー目の重み係数
   ratio[1]:2エコー目の重み係数

   設定者 PROLIM
   使用者 recon
 */

#define SVN_PRC_DUAL_ECHO_DISPLAY_FLAG              (SVN_BASE_PROTOCOL_PROCESS + 81)
#define SVT_PRC_DUAL_ECHO_DISPLAY_FLAG              VFT_INT32
#define SVS_PRC_DUAL_ECHO_DISPLAY_FLAG              "show/hide images for each echo"
/*
   各エコー画像を中間画像とするかどうかのフラグ

   VFC_DUAL_ECHO_HIDE        0
   VFC_DUAL_ECHO_SHOW        1

   設定者 PROLIM
   使用者 recon
 */

#define SVN_PRC_AFI_ALGORITHM               (SVN_BASE_PROTOCOL_PROCESS + 82)
#define SVT_PRC_AFI_ALGORITHM               VFT_INT32
#define SVS_PRC_AFI_ALGORITHM               "AFI algorithm"
/*
    AFI処理方法種別

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

    設定者 PROLIM,seqgen
    使用者 recon
 */

#define SVN_PRC_AFI_POCS_TIMES              (SVN_BASE_PROTOCOL_PROCESS + 83)
#define SVT_PRC_AFI_POCS_TIMES              VFT_INT32
#define SVS_PRC_AFI_POCS_TIMES              "AFI POCS times"
/*
    POCS処理を行う場合の繰り返し回数

    設定者 PROLIM,seqgen
    使用者 recon
 */

#define SVN_PRC_IMAGE_COMBINATION_RERECON_FLAG  (SVN_BASE_PROTOCOL_PROCESS + 84)
#define SVT_PRC_IMAGE_COMBINATION_RERECON_FLAG  VFT_INT32
#define SVS_PRC_IMAGE_COMBINATION_RERECON_FLAG  "image combination display flag on Mui"
/*
    image combination display flag on Mui

    0 - Image CombinationのグループボックスをMuiで表示しない
    1 - FWS関連が常にInsensitive、他の演算モードのみ表示
    2 - 全ての演算モードを表示

    設定者 PROLIM
    使用者 RECON
 */

#define SVN_PRC_AUTO_STAMD_MODE         (SVN_BASE_PROTOCOL_PROCESS + 85)
#define SVT_PRC_AUTO_STAMD_MODE         VFT_INT32
#define SVS_PRC_AUTO_STAMD_MODE         "MIP or minIP for auto STAMD(Shift)"
/*
    MIP or minIP for auto STAMD(Shift)

    #define VFC_AUTO_STAMD_MINIP	0 (最小値投影)
    #define VFC_AUTO_STAMD_MIP		1 (最大値投影)

    設定者 PROLIM
    使用者 RECON
 */

#define SVN_PRC_AUTO_STAMD_SLICES       (SVN_BASE_PROTOCOL_PROCESS + 86)
#define SVT_PRC_AUTO_STAMD_SLICES       VFT_INT32
#define SVS_PRC_AUTO_STAMD_SLICES       "Units of STAMD slices"
/*
    Units of STAMD slices

    設定者 PROLIM
    使用者 RECON
 */

#define SVN_PRC_AUTO_STAMD_NUM_SHIFT   (SVN_BASE_PROTOCOL_PROCESS + 87)
#define SVT_PRC_AUTO_STAMD_NUM_SHIFT   VFT_INT32
#define SVS_PRC_AUTO_STAMD_NUM_SHIFT   "Number of shifted slices in STAMD"
/*
    Number of shifted slices in STAMD

    設定者 PROLIM
    使用者 RECON
 */

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_PROCESS	SVN_MIN_SUBPROTOCOL + SVN_OFF_PROCESS

#define SVN_PRC_RETRO_VIEWTBL_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 0)
#define SVT_PRC_RETRO_VIEWTBL_SIZE	VFT_INT32
#define SVS_PRC_RETRO_VIEWTBL_SIZE	"Number of entries in view table"
/*	  Retrospctive Gating用viewテーブルの要素数。
	*/

#define SVN_PRC_RETRO_VIEWTBL_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 1)
#define SVT_PRC_RETRO_VIEWTBL_TYPE	VFT_INT32
#define SVS_PRC_RETRO_VIEWTBL_TYPE	"Type of view table"
/*	  Retrospctive Gating用viewテーブルのデータタイプ。
	*/

#define SVN_PRC_RETRO_VIEW_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 2)
#define SVT_PRC_RETRO_VIEW_TABLE	VFT_INT32
#define SVS_PRC_RETRO_VIEW_TABLE	"View table"
/*	  Retrospctive Gating用viewテーブル。
	  位相エンコード、PSエンコード、アベレージング、スライス、スラブの
	  データセットを収集順に格納する。
	*/

#define SVN_PRC_RETRO_TIMING_PLAN_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 3)
#define SVT_PRC_RETRO_TIMING_PLAN_SIZE	VFT_INT32
#define SVS_PRC_RETRO_TIMING_PLAN_SIZE	"Number of entires of planed ADC timing table"
/*	  Retrospctive Gating用計画時収集タイミングテーブルの要素数。
	*/

#define SVN_PRC_RETRO_TIMING_PLAN_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 4)
#define SVT_PRC_RETRO_TIMING_PLAN_TYPE	VFT_INT32
#define SVS_PRC_RETRO_TIMING_PLAN_TYPE	"Type of timing table"
/*	 Retrospctive Gating用計画時収集タイミングテーブルのデータタイプ。 
	*/

#define SVN_PRC_RETRO_TIMING_PLAN	(SVN_BASE_SUBPROTOCOL_PROCESS + 5)
#define SVT_PRC_RETRO_TIMING_PLAN	VFT_FLT32
#define SVS_PRC_RETRO_TIMING_PLAN	"Planed ADC timing table"
/*	 Retrospctive Gating用計画時収集タイミングテーブル。
	 エコー収集時刻を、シーケンス開始時刻をゼロとしてmsec単位で配列する。 
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 6)
#define SVT_PRC_RETRO_AVERAGING_TABLE_SIZE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE_SIZE	"Number of entries of averaging phase and read-out reversal table"
/*	 Retrospctive Gating用アベレージングテーブルの要素数。 
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 7)
#define SVT_PRC_RETRO_AVERAGING_TABLE_TYPE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE_TYPE	"Type of averaging phase and read-out reversal table"
/*	  Retrospctive Gating用アベレージングテーブルのタイプ。
	*/

#define SVN_PRC_RETRO_AVERAGING_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 8)
#define SVT_PRC_RETRO_AVERAGING_TABLE	VFT_INT32
#define SVS_PRC_RETRO_AVERAGING_TABLE	"Table of averaging phase and read-out reversal"
/*	  Retrospctive Gating用アベレージングテーブル。
	  加算処理の位相反転およびEPIのエコー反転に関する情報を配列する。
　　　　値：　０　　　　位相反転なし、エコー反転なし
　　　　値：　２　　　　位相反転あり、エコー反転なし
　　　　値：　１６　　　位相反転なし、エコー反転あり
　　　　値：　１８　　　位相反転あり、エコー反転あり

	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	(SVN_BASE_SUBPROTOCOL_PROCESS + 9)
#define SVT_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNTTBL_SIZE	"Number of entries in ALL_ADCCOUNT_TABLE"
/*	  Retrospctive Gating(MODE_2)用SteadyLoop内収集回数テーブルの要素数。
	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	(SVN_BASE_SUBPROTOCOL_PROCESS + 10)
#define SVT_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNTTBL_TYPE	"Type of ALL_ADCCOUNT_TABLE"
/*	  Retrospctive Gating(MODE_2)用SteadyLoop内収集回数テーブルのデータタイプ。
	*/

#define SVN_PRC_RETRO_ALL_ADCCOUNT_TABLE	(SVN_BASE_SUBPROTOCOL_PROCESS + 11)
#define SVT_PRC_RETRO_ALL_ADCCOUNT_TABLE	VFT_INT32
#define SVS_PRC_RETRO_ALL_ADCCOUNT_TABLE	"Table of adc counts in a steady loop"
/*	  Retrospctive Gating(MODE_2)用SteadyLoop内収集回数テーブル。
	  SteadyLoop内収集回数を収集順に格納する。
	*/

#define SVN_PRC_MOTION_ROTATION_RESULTS_FOR_JET		(SVN_BASE_SUBPROTOCOL_PROCESS + 12)
#define SVT_PRC_MOTION_ROTATION_RESULTS_FOR_JET		VFT_FLTVEC2
#define SVS_PRC_MOTION_ROTATION_RESULTS_FOR_JET		"Motion ratation results for JET"
/*	  JETの回転補正(rotation correction)の結果を記録するフィールド。
	  2Dの場合(0,φ)となり、numBlade × numSlice分記録される。

	  設定者 recon
	  使用者 recon
	*/

#define SVN_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	(SVN_BASE_SUBPROTOCOL_PROCESS + 13)
#define SVT_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	VFT_FLTVEC3
#define SVS_PRC_MOTION_TRANSLATION_RESULTS_FOR_JET	"Motion translation results for JET"
/*	  JETの並進補正(translation correction)の結果を記録するフィールド。
	  3Dの場合(ro,pe,0)となり、numBlade × numSlice分記録される。

	  設定者 recon
	  使用者 recon
	*/

#define SVN_PRC_MOTION_REJECTION_RESULTS_FOR_JET	(SVN_BASE_SUBPROTOCOL_PROCESS + 14)
#define SVT_PRC_MOTION_REJECTION_RESULTS_FOR_JET	VFT_FLT32
#define SVS_PRC_MOTION_REJECTION_RESULTS_FOR_JET	"Motion rejection results for JET"
/*	  JETの重み付け(rejection correction)の結果を記録するフィールド。
	  V9.20ではrejection correctionは行わないため、常に1とする。
	  numBlade × numSlice分記録される。

	  設定者 recon
	  使用者 recon
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
/*	ダイナミックスキャンをiselectorが表示する際、同一セグメント番号を
	グループ化する為に使用する。
	*/

#define SVN_PRC_STATUS			(SVN_BASE_PROCESSED_PROCESS + 11)
#define SVT_PRC_STATUS			VFT_INT32
#define SVS_PRC_STATUS			"Finish creating new node"
/*	ノード生成完了を通知する。
	このフィールドがあればノードが完成したとみなす。
	*/

#define SVN_PRC_ROW_CONTROL_FLAG	(SVN_BASE_PROCESSED_PROCESS + 12)
#define SVT_PRC_ROW_CONTROL_FLAG	VFT_INT32
#define SVS_PRC_ROW_CONTROL_FLAG	"The flag to controll image selector"
/*	SVN_PRC_ROW_ID,SVN_PRC_GROUP_ID によりI-selector上
	の並び方を制御することを示すフラグ。
	Processed Node に登録する。
	SVN_PRC_ROW_CONTROL_FLAGの値(真／偽、拡張性のためBOOL
	ではなくINTを使用)によって row-id,group-id による並べ
	変えを行なう。ただし、このフィールドが存在しない場合
	偽が設定された場合と同様の処理を行なう。
	このフラグが真となる Processed は全ての画像について
	row-id/group-id が定義されていなくてはならない。
	例外として row または group がただ一つの場合、
	省略が可能。
	*/

#define SVN_PRC_TOTAL_SLAB_NUM		(SVN_BASE_PROCESSED_PROCESS + 13)
#define SVT_PRC_TOTAL_SLAB_NUM		VFT_INT32
#define SVS_PRC_TOTAL_SLAB_NUM		"Total number of slabs"
/*	総スラブ数。
	Processed Node に登録する。
	*/

#define SVN_PRC_IMG_INFO		(SVN_BASE_PROCESSED_PROCESS + 14)
#define SVT_PRC_IMG_INFO		VFT_UINT8
#define SVS_PRC_IMG_INFO		"Image Infomation"
/*	画像種別識別情報 
	このフィールドは可変長になっているのでタイプはVFT_UINT8とする。
	但し内部は以下の様な構造をしている。
	typedef	struct {
	    size_t	infoType_depth		;  画像種別数 
	    unit32_t	infoType[infoType_depth];  画像種別 
	    unit32_t	infoSubType		;  画像種別詳細 
	} ImgInfo_t ;
	先頭には画像種別数(infoType_depth)が格納され、その後に
	個数分の画像種別(infoType)が格納され、最後にカレントの
	画像種別詳細(infoSubType)が格納される。
	*/

#define SVN_PRC_PAC_RECON_IMAGE		(SVN_BASE_PROCESSED_PROCESS + 15)
#define SVT_PRC_PAC_RECON_IMAGE		VFT_BITFIELD
#define SVS_PRC_PAC_RECON_IMAGE		"Image type of PAC"
/*	PACの再構成画像が 合成画像なのか各チャネルの画像なのかを
	区別するために用いる。
	*/

#define SVN_PRC_POST_PROC_DESC		(SVN_BASE_PROCESSED_PROCESS + 16)
#define SVT_PRC_POST_PROC_DESC		VFT_STRING
#define SVS_PRC_POST_PROC_DESC		"Post Processing Description"
/* 後処理情報
 *	(a) msPostProcは処理したポストプロックファイル内容を格納する。
 */

#define SVN_PRC_PROC_TIME	(SVN_BASE_PROCESSED_PROCESS + 17)
#define SVT_PRC_PROC_TIME	VFT_INT32
#define SVS_PRC_PROC_TIME	"Image Processing Start and End Time"
/*	大きさ２の配列
	array[0] : 画像処理の開始時刻
	array[1] : 画像処理の終了時刻
	*/

#define SVN_PRC_FILTERING_CODE	(SVN_BASE_PROCESSED_PROCESS + 18)
#define SVT_PRC_FILTERING_CODE	VFT_STRING
#define SVS_PRC_FILTERING_CODE	"Filtering Code for Reconstruction"
/*	フィルター（内部コード表記）は、フィルターデータベースのk-space
	フィルターとRefineフィルタの内部コードとする。（ただし、-1は＊と
	表示する。）
	*/

#define SVN_PRC_DELTA_R2_BASE_TIME	(SVN_BASE_PROCESSED_PROCESS + 19)
#define SVT_PRC_DELTA_R2_BASE_TIME	VFT_INT32
#define SVS_PRC_DELTA_R2_BASE_TIME	"Delta R2* Base Time"
/*	デルタR2＊の基準時間
	再構成がデルタR2画像作成時にプロセスドノードに格納する。
	単位はmsec。
	*/

#define SVN_PRC_PREP_SCAN_TIME	(SVN_BASE_PROCESSED_PROCESS + 20)
#define SVT_PRC_PREP_SCAN_TIME	VFT_INT32
#define SVS_PRC_PREP_SCAN_TIME	"Prep Scan Time"
/*	プレップスキャン開始後本スキャンが開始するまでの時間
	msRecon系は本スキャンの撮影撮影時間とプレップスキャンの撮影
	開始時間の差を求めセットする。単位はミリ秒。
	*/

#define SVN_PRC_MAP_EXIST_FLAG	(SVN_BASE_PROCESSED_PROCESS + 21)
#define SVT_PRC_MAP_EXIST_FLAG	VFT_BOOL
#define SVS_PRC_MAP_EXIST_FLAG	"Exist Sensitivity data"
/*
SPEEDERの感度マップ ができあがったら、再構成は感度推定用スキャンの
プロセスドノードに書き込む。
  True : SVN_PLN_MAP_PATHにある感度マップデータが存在する。
  False: 存在しない。
*/

#define SVN_PRC_MAP_DATA	(SVN_BASE_PROCESSED_PROCESS + 22)
#define SVS_PRC_MAP_DATA	"Sensitivity Map"
/* SPEEDERの感度マップ
 患者座標系にて、ボリュームデータの形でもつ。
 オブリーク禁止なので、大きさは、xDim,yDim,zDimの３つの要素で表現できる。
　プロセスドノードに書き込む。
　生データと同じファイルに格納する。
（書き込み時に、グループIDを指定することで可能）
　例：64x64x64ならば、
      int16_t,complexの場合、１MB　となり、これがチャネル分できる。
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
/* SPEEDERの感度マップ (８チャネル対応）
*/

#define SVN_PRC_MAP_DATA_TYPE	(SVN_BASE_PROCESSED_PROCESS + 30)
#define SVT_PRC_MAP_DATA_TYPE	VFT_INT32
#define SVS_PRC_MAP_DATA_TYPE	"Sensitivity data type"
/* SPEEDERの感度マップ のタイプ
　プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_DATA_FORM	(SVN_BASE_PROCESSED_PROCESS + 31)
#define SVT_PRC_MAP_DATA_FORM	VFT_INT32
#define SVS_PRC_MAP_DATA_FORM	"Sensitivity data form"
/* SPEEDERの感度マップ のフォーム
   プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_DATA_DIM	(SVN_BASE_PROCESSED_PROCESS + 32)
#define SVT_PRC_MAP_DATA_DIM	VFT_INT32
#define SVS_PRC_MAP_DATA_DIM	"Sensitivity data dimensions"
/* SPEEDERの感度マップ の大きさ
 プロセスドノードに書き込む。
   患者座標系にて、ボリュームデータの形でもつ。
 オブリーク禁止なので、大きさは、xDim,yDim,zDimの３つの要素で表現できる。
 （基本的には、x,y,zの順である。）
*/

#define SVN_PRC_MAP_DATA_DESC	(SVN_BASE_PROCESSED_PROCESS + 33)
#define SVT_PRC_MAP_DATA_DESC	VFT_INT32
#define SVS_PRC_MAP_DATA_DESC	"Sensitivity data dimension descriptions"
/* SVN_PRC_MAP_DATA_DIM がどの順番で入っているかを記述。
   患者座標系でのX座標、Y座標、Z座標の３つの要素をもつ。
　プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_START_POSITION	(SVN_BASE_PROCESSED_PROCESS + 34)
#define SVT_PRC_MAP_START_POSITION	VFT_FLTVEC3
#define SVS_PRC_MAP_START_POSITION	"Sensitivity data Start Position"
/* SVN_PRC_MAP_DATA のスタートポジション
 患者座標系でのX座標、Y座標、Z座標における最小座標がセットされる。
 プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_END_POSITION	(SVN_BASE_PROCESSED_PROCESS + 35)
#define SVT_PRC_MAP_END_POSITION	VFT_FLTVEC3
#define SVS_PRC_MAP_END_POSITION	"Sensitivity data End Position"
/* SVN_PRC_MAP_DATA のエンドポジション
　患者座標系でのX座標、Y座標、Z座標における最大座標がセットされる。
 プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_STEP	(SVN_BASE_PROCESSED_PROCESS + 36)
#define SVT_PRC_MAP_STEP	VFT_FLTVEC3
#define SVS_PRC_MAP_STEP	"Sensitivity data Step"
/* SVN_PRC_MAP_DATA の刻み幅
 プロセスドノードに書き込む。
*/

#define SVN_PRC_MAP_D_INV_TABLE (SVN_BASE_PROCESSED_PROCESS + 37)
#define SVT_PRC_MAP_D_INV_TABLE VFT_FLT32
#define SVS_PRC_MAP_D_INV_TABLE "Sensitivity data Displacements in each dimension"
/*      (1) 感度マップの置換ベクトル(ro,pe,se) ->  (x,y,z)を設定する。
        VFT_INT32 × ９ の配列で
      Dinvmap[0] : x.ro
      Dinvmap[1] : x.pe
      Dinvmap[2] : x.se
      Dinvmap[3] : y.ro
      Dinvmap[4] : y.pe
      Dinvmap[5] : y.se
      Dinvmap[6] : z.ro
      Dinvmap[7] : z.pe
      Dinvmap[8] : z.se
     (2) SPEEDERプレスキャンの再構成が、感度マップ作成時に設定する。
     (3) SPEEDER本スキャンの再構成が、感度マップ切り出し時 または、UNFOLD処理時に
         使用する。
*/

#define SVN_PRC_MAP_DATA_SCALE  (SVN_BASE_PROCESSED_PROCESS + 38)
#define SVT_PRC_MAP_DATA_SCALE  VFT_FLT32
#define SVS_PRC_MAP_DATA_SCALE  "Sensitivity data scale factor"
/*      感度マップデータのスケール

The scale factor that was used to normalize data in an SVN_PRC_MAP_DATA* varfield.  To restore the data to its original range, divide the data by this scale fac
*/

#define SVN_PRC_INTENSITY_MAP_DATA	(SVN_BASE_PROCESSED_PROCESS + 39)
#define SVS_PRC_INTENSITY_MAP_DATA      "Intensity Correction Map"
/* 輝度補正マップ
  形式は、SVN_PRC_MAP_DATA と同様。
　補正用データ収集用再構成の場合に、（SOS/WB）を元に作成するマップ。
  患者座標系にて、ボリュームデータの形でもつ。
  オブリーク禁止なので、大きさは、xDim,yDim,zDimの３つの要素で表現できる。
　プロセスドノードに書き込む。
　生データと同じファイルに格納する。（書き込み時に、グループIDを指定することで可能）
　type,form,dim,desc,scale等は、感度マップと同様のものを使用する。
　例：64x64x64ならば、
      int16_t,complexの場合、１MB　となり、これがプロセスドに１つできる。*/

#define SVN_PRC_MAP_MASK_DATA	(SVN_BASE_PROCESSED_PROCESS + 40)
#define SVS_PRC_MAP_MASK_DATA   "Sensitivity Map Mask"
/* マップのマスクデータ
  形式は、SVN_PRC_MAP_DATA と同様。
　補正用データ収集用再構成の場合に、WBデータを元に作成するマップのマスクデータ。
  患者座標系にて、ボリュームデータの形でもつ。
  オブリーク禁止なので、大きさは、xDim,yDim,zDimの３つの要素で表現できる。
　プロセスドノードに書き込む。
　生データと同じファイルに格納する。（書き込み時に、グループIDを指定することで可能）
　type,form,dim,desc,scale等は、感度マップと同様のものを使用する。
　例：64x64x64ならば、
      int16_t,complexの場合、１MB　となり、これがプロセスドに１つできる。*/

#define SVN_PRC_GDC_IMAGE_KIND          (SVN_BASE_PROCESSED_PROCESS + 41)
#define SVT_PRC_GDC_IMAGE_KIND          VFT_INT32
#define SVS_PRC_GDC_IMAGE_KIND          "GDC image kind"
/* 
   磁場歪み補正処理画像種別
   0 : 通常画像
   1 : 磁場歪み補正画像
   2 : 磁場歪み補正処理位置決め画像
   3 : 磁場歪み補正処理位置決め画像(マスク有り)
 */

#define SVN_PRC_NUMBER_OF_SLICES_IN_SLAB    (SVN_BASE_PROCESSED_PROCESS + 42)
#define SVT_PRC_NUMBER_OF_SLICES_IN_SLAB    VFT_INT32
#define SVS_PRC_NUMBER_OF_SLICES_IN_SLAB    "Number of slices in slab"
/* 
   (マルチ)スラブ内に存在するスライス枚数
*/

#define SVN_PRC_ADC_EXEC    (SVN_BASE_PROCESSED_PROCESS + 43)
#define SVT_PRC_ADC_EXEC    VFT_INT32
#define SVS_PRC_ADC_EXEC    "ADC executing flag"
/*
   ADC処理実行フラグ：ADC処理が実施された経歴があるか判断するフラグ
   0 : ADC処理なし
   1 : ADC処理実施済み
*/

#define SVN_PRC_ISO_REF_PATH    (SVN_BASE_PROCESSED_PROCESS + 44)
#define SVT_PRC_ISO_REF_PATH    VFT_INT32
#define SVS_PRC_ISO_REF_PATH    "Processed Index of the Parent image of Isotropic image"
/*
  Isotropic画像の基画像のプロセスインデックス番号：Isotropic画像のスケーリングを実施
  する場合に基準となる画像群のパスを定義する。
*/

#define SVN_PRC_MAP_AVERAGE    (SVN_BASE_PROCESSED_PROCESS + 45)
#define SVT_PRC_MAP_AVERAGE    VFT_FLT32
#define SVS_PRC_MAP_AVERAGE    "Brightness mean in the central part of the map"
/* 
   マップの中心部の輝度平均値
   V8.00 SPEEDER/輝度補正ノイズ補償フィルタで追加
   プロセスドノードに書き込む。
*/

#define SVN_PRC_PERFUSION_MAP   (SVN_BASE_PROCESSED_PROCESS + 46)
#define SVT_PRC_PERFUSION_MAP   VFT_INT32
#define SVS_PRC_PERFUSION_MAP   "The detail of Image kind"
/*
    ADC処理の実行履歴フラグ。
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
/* SPEEDERの感度マップ (１２８チャネル対応）
*/

#define SVN_PRC_NOISE_GAIN_TABLE   (SVN_BASE_PROCESSED_PROCESS + 167)
#define SVT_PRC_NOISE_GAIN_TABLE   VFT_FLT32
#define SVS_PRC_NOISE_GAIN_TABLE   "noise gain ratio among coils"
/*
   チャネルコイル間ノイズゲイン補正用のテーブル
*/

#define SVN_PRC_NOISE_MATRIX_TABLE   (SVN_BASE_PROCESSED_PROCESS + 168)
#define SVT_PRC_NOISE_MATRIX_TABLE   VFT_FLT32
#define SVS_PRC_NOISE_MATRIX_TABLE   "noise correction between coils"
/*
   Speeder展開用チャネルコイル間ノイズ相関係数マトリクス
*/

#define SVN_PRC_NOISE_GAIN_TABLE_FLAG        (SVN_BASE_PROCESSED_PROCESS + 169)
#define SVT_PRC_NOISE_GAIN_TABLE_FLAG        VFT_INT32
#define SVS_PRC_NOISE_GAIN_TABLE_FLAG        "noise gain table flag"
/* 
   チャネルコイル間ノイズゲイン補正フラグ

   0 : 補正処理無し
   1 : 補正処理有り

   #define VFC_NOISE_GAIN_TABLE_OFF   0
   #define VFC_NOISE_GAIN_TABLE_ON    1
 */

#define SVN_PRC_NOISE_MATRIX_TABLE_FLAG      (SVN_BASE_PROCESSED_PROCESS + 170)
#define SVT_PRC_NOISE_MATRIX_TABLE_FLAG      VFT_INT32
#define SVS_PRC_NOISE_MATRIX_TABLE_FLAG      "noise matrix table flag"
/* 
   Speederチャネルコイル間相関マトリクス補正フラグ

   0 : 補正処理無し
   1 : 補正処理有り

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
/*	スラブ番号。
	Echo Node に登録する。
	*/

#define SVN_PRC_IMAGE_FLIP_FLAG		(SVN_BASE_ECHO_PROCESS + 10)
#define SVT_PRC_IMAGE_FLIP_FLAG		VFT_BOOL
#define SVS_PRC_IMAGE_FLIP_FLAG		"image flip flag for AX"
/*	スライス断面がＡＸlikeでかつ画像観察方向がViewFromFeet
	の場合に、画像の左右を反転して表示するためのフラグ。
	FALSE：反転しない。TRUE：反転する。
	バージョンの古い画像でこのフィールドがない時は反転しな
	い。
	*/

#define SVN_PRC_MPG_APPLY_DIRECT	(SVN_BASE_ECHO_PROCESS + 11)
#define SVT_PRC_MPG_APPLY_DIRECT	VFT_INT32
#define SVS_PRC_MPG_APPLY_DIRECT	"The direction which MPG pulse is applied along."
/*	  diffusion画像において、Reference及びPhase、Read、Sliceのうち、
	  MPGをどの方向に印加して得られた画像であるかをを示す情報。方向は
	  以下に示す数値で指定される。
	  VFC_IVIM_ACQUIRE_REF    1
	  VFC_IVIM_ACQUIRE_PE     2
	  VFC_IVIM_ACQUIRE_RO     4
	  VFC_IVIM_ACQUIRE_SS     8
	  
	  設定者:	recon
	  
	  This field show the direction of the MPG pulse at
	  the diffusion weighted image.
	  There are four directions which are reference(=1), phase(=2),
	  read(=4), and slice(=8). 
	  */

#define SVN_PRC_DYNAMIC_NO		(SVN_BASE_ECHO_PROCESS + 12)	
#define SVT_PRC_DYNAMIC_NO		VFT_INT32
#define SVS_PRC_DYNAMIC_NO		"Dynamic No"
/*	時相の番号。
	Echo Node に登録する。
	*/

#define SVN_PRC_PERFUSION_MAP_TYPE   (SVN_BASE_ECHO_PROCESS + 13)
#define SVT_PRC_PERFUSION_MAP_TYPE   VFT_INT32
#define SVS_PRC_PERFUSION_MAP_TYPE   "The detail of Image kind"
/*
    Perfusion MAP処理の種別を定義する。
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
/*	画像番号。
	Position Node に登録する。
	group 内で unique な値をとる。
	group 内でのソートに用いる。省略不可。
	*/

#define SVN_PRC_AFTER_INJECT_TIME	(SVN_BASE_POSITION_PROCESS + 3)
#define SVT_PRC_AFTER_INJECT_TIME      	VFT_INT32
#define SVS_PRC_AFTER_INJECT_TIME      	"After Inject time"
/*      造影剤注入からの経過時間 (単位はms）
	Dynamic等で造影剤注入前の時相の場合はゼロが入る。
        （ゼロの場合は非表示にする。）
	*/

#define SVN_PRC_RO_DEPHASE_AREA_RATIO	(SVN_BASE_POSITION_PROCESS + 4)
#define SVT_PRC_RO_DEPHASE_AREA_RATIO   	VFT_STRING
#define SVS_PRC_RO_DEPHASE_AREA_RATIO   	"Write dephase area ratio"
/*	Flow-Spoied FBIの本スキャンあるいはFlowPrepスキャンで、その
	画像を収集した際の、ROあるいはPE方向に印加されたDephasing Pulse
	の強度を表す文字列。パルス強度は、読出し用の傾斜磁場の立上りからエコー
	中心までの面積を１とした時の、Dephaseパルスの面積比で表す。
	文字列はFDの後に強度を％表示したもの（例：FD+15%,FD-10% など)
*/

#define SVN_PRC_SLICE_NUMBER    (SVN_BASE_POSITION_PROCESS + 5)
#define SVT_PRC_SLICE_NUMBER    VFT_INT32
#define SVS_PRC_SLICE_NUMBER    "Slice number"
/*	スライス番号
        スラブ間でのシーケンシャルな番号が設定される。
*/

#define SVN_PRC_SLICE_NUMBER_FOR_DICOM	(SVN_BASE_POSITION_PROCESS + 6)
#define SVT_PRC_SLICE_NUMBER_FOR_DICOM	VFT_INT32
#define SVS_PRC_SLICE_NUMBER_FOR_DICOM	"Slice number for DICOM"
/*	DICOMのスライス番号(画像番号)
	スラブ間でのシーケンシャルな番号が設定される。
        マルチエコー撮像では２エコー目から
        ダイナミック撮像では２時相目からオフセットが付加する。
        これにより同一シリーズ内でユニークな番号となる。
*/

/*****************************************************************************
  IMCALC name space
*/



#endif /* tami_vfStudy_vfs_process_H__*/
