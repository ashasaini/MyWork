/******************************************************************************
 *    File Overview:
 *	This file contains the varfield names for the EXECUTION subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The EXECUTION subspace contains fields associated with the execution
 *	of an MRI sequence.
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



#ifndef tami_vfStudy_vfs_execution_H__
#define tami_vfStudy_vfs_execution_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/
#define SVN_EXE_SHIM_FSY_VALUE	(SVN_MIN_INDEPENDENT+SVN_OFF_EXECUTION+0)
#define SVT_EXE_SHIM_FSY_VALUE	VFT_FLT64
#define SVS_EXE_SHIM_FSY_VALUE	"center frequency for shimming"
/*	シミング後の中心周波数設定値。周波数値は絶対量をセットす
        る。
	PQM が現在の中心周波数設定値を読み出し、root node に記述
        する。シミングスキャンの直前に PQM が protocol node にこ
        の値をコピーする。
	msShimがシミング後の設定値を算出し、processed node にセッ
        トする。
	msShim の調整処理終了後、processed node の値を shimming 
        が root node にコピーする。シミング処理終了後、PQM は
        Acqman にこの設定値に中心周波数を設定する要求を出す。
        Acqman は要求を受けて、root node の中心周波数値を設定
        する。
	*/

/*****************************************************************************
  STUDY name space
*/




/*****************************************************************************
  PROTOCOL name space
*/

#define SVN_BASE_PROTOCOL_EXECUTION SVN_MIN_PROTOCOL+SVN_OFF_EXECUTION


#define SVN_EXE_PRESCAN_FSY_ACTION 	(SVN_BASE_PROTOCOL_EXECUTION + 0)
#define SVT_EXE_PRESCAN_FSY_ACTION	VFT_INT32
#define SVS_EXE_PRESCAN_FSY_ACTION	"Center Frequency prescan action"

#define SVN_EXE_PRESCAN_FSY_VALUE 	(SVN_BASE_PROTOCOL_EXECUTION + 1)
#define SVT_EXE_PRESCAN_FSY_VALUE 	VFT_FLT64
#define SVS_EXE_PRESCAN_FSY_VALUE 	"Center Frequency value (Hz)"

#define SVN_EXE_PRESCAN_RFL_ACTION	(SVN_BASE_PROTOCOL_EXECUTION + 2)
#define SVT_EXE_PRESCAN_RFL_ACTION	VFT_INT32
#define SVS_EXE_PRESCAN_RFL_ACTION	"RF Level prescan action"

#define SVN_EXE_PRESCAN_RFL_GAIN_TABLE 	(SVN_BASE_PROTOCOL_EXECUTION + 3)
#define SVT_EXE_PRESCAN_RFL_GAIN_TABLE 	VFT_STRING
#define SVS_EXE_PRESCAN_RFL_GAIN_TABLE	"RF Level; RF Tx gain table name"

#define SVN_EXE_PRESCAN_RFL_GAIN_VALUE 	(SVN_BASE_PROTOCOL_EXECUTION + 4)
#define SVT_EXE_PRESCAN_RFL_GAIN_VALUE 	VFT_FLT32
#define SVS_EXE_PRESCAN_RFL_GAIN_VALUE	"RF Level; RF Tx gain value (dB)"

#define SVN_EXE_PRESCAN_RFL_SCALE_TABLE	(SVN_BASE_PROTOCOL_EXECUTION + 5)
#define SVT_EXE_PRESCAN_RFL_SCALE_TABLE	VFT_STRING
#define SVS_EXE_PRESCAN_RFL_SCALE_TABLE	"RF Level; WG scaling table name"

#define SVN_EXE_PRESCAN_RFL_SCALE_VALUE	(SVN_BASE_PROTOCOL_EXECUTION + 6)
#define SVT_EXE_PRESCAN_RFL_SCALE_VALUE	VFT_FLT32
#define SVS_EXE_PRESCAN_RFL_SCALE_VALUE	"RF Level; WG scaling value"

#define SVN_EXE_PRESCAN_RGN_ACTION 	(SVN_BASE_PROTOCOL_EXECUTION + 7)
#define SVT_EXE_PRESCAN_RGN_ACTION 	VFT_INT32
#define SVS_EXE_PRESCAN_RGN_ACTION	"Receiver Gain prescan action"

#define SVN_EXE_PRESCAN_RGN_TABLE 	(SVN_BASE_PROTOCOL_EXECUTION + 8)
#define SVT_EXE_PRESCAN_RGN_TABLE 	VFT_STRING
#define SVS_EXE_PRESCAN_RGN_TABLE	"Receiver Gain table name"

#define SVN_EXE_PRESCAN_RGN_VALUE 	(SVN_BASE_PROTOCOL_EXECUTION + 9)
#define SVT_EXE_PRESCAN_RGN_VALUE 	VFT_FLT32
#define SVS_EXE_PRESCAN_RGN_VALUE	"Receiver Gain value (dB)"

#define SVN_EXE_COUCH			(SVN_BASE_PROTOCOL_EXECUTION + 10)
#define SVT_EXE_COUCH			VFT_FLT32
#define SVS_EXE_COUCH			"Couch position"
/*	寝台位置。単位 mm。

	Couch position in millimeters.
	*/

#define SVN_EXE_VERBOSITY		(SVN_BASE_PROTOCOL_EXECUTION + 11)
#define SVT_EXE_VERBOSITY		VFT_INT32
#define SVS_EXE_VERBOSITY		"Verbosity"
/*
	*/

#define SVN_EXE_SHOW_SEQGEN_MESSAGES	(SVN_BASE_PROTOCOL_EXECUTION + 12)
#define SVT_EXE_SHOW_SEQGEN_MESSAGES	VFT_INT32
#define SVS_EXE_SHOW_SEQGEN_MESSAGES	"Show messages from SeqGen to operator"
/*
	*/

#define SVN_EXE_PROTOCOL_ACQ_STATUS	(SVN_BASE_PROTOCOL_EXECUTION + 13)
#define SVT_EXE_PROTOCOL_ACQ_STATUS	VFT_INT32
#define SVS_EXE_PROTOCOL_ACQ_STATUS	"Acquisition Status for a protocol"
/*      This field indicates, if it exists, whether the entire protocol
	was acquired successfully, or it failed because of some error. The
	contents of this field are written by PQM.
	*/

#define SVN_EXE_ACQ_CONDITION		(SVN_BASE_PROTOCOL_EXECUTION + 14)
#define SVT_EXE_ACQ_CONDITION		VFT_INT32
#define SVS_EXE_ACQ_CONDITION		"Acquisition Condition Key"
/*	同一収集条件（寝台位置・コイル・部位等のＰＱＭで設定する条件）を判定
	するために使用するキー。ＰＱＭは、同一の収集条件に対して同一のキーを
	発行する。従って、本フィールドの値を比較することにより収集上条件の変
	更を検出することが可能である。
	グラフィックロケータは本フィールドの検定により親画像として使用可能か
	どうかの判定をする。

	注）本フィールドは他のプロトコルの値と比較することにより初めて意味を
	    もつもので、単一では何の意味もない。
	*/

#define SVN_EXE_PRESCAN_RFL_VALUES_ARRAY (SVN_BASE_PROTOCOL_EXECUTION + 15)
#define SVT_EXE_PRESCAN_RFL_VALUES_ARRAY VFT_FLT32
#define SVS_EXE_PRESCAN_RFL_VALUES_ARRAY "RF Level; Array of values measured"
/* This field contains an array of TxGain values that are written by
   acqman and read by seqgen for the RF Level sequence. These values are
   used by the sequence used to measure RF Level.

   It is the responsibility of AcqMan to make sure that this array is
   correct prior to seqgen and after RF Level is completed. AcqMan can
   request that seqgen create a seq to measure 5 points and then make
   another request for a single point.  After RFL measurement is
   completed AcqMan will write the complete array of 6 points to
   varfields.
   */

#define SVN_EXE_PAC_SCAN_FLAG	(SVN_BASE_PROTOCOL_EXECUTION + 16)
#define SVT_EXE_PAC_SCAN_FLAG	VFT_BOOL
#define SVS_EXE_PAC_SCAN_FLAG	"P.A.C multiple channels scan flag"
/*	ＰＡＣ多チャネル同時収集を行なう時 TRUE(1) にする。
	このフィールドが存在しない時、または、FALSE(0) が設定されている
	時は、通常収集である。
	尚V3.5 では、受信コイルのチャネルが、'W','X','Y','Z'の時ＰＡＣ
	収集を行なう。
	*/

#define SVN_EXE_NUM_PAC_SCAN	(SVN_BASE_PROTOCOL_EXECUTION + 17)
#define SVT_EXE_NUM_PAC_SCAN	VFT_INT16
#define SVS_EXE_NUM_PAC_SCAN	"number of channels for P.A.C scan"
/*	SVN_EXE_PAC_SCAN_FLAG が TRUE(1) の時有効であり、ＰＡＣ収集のチャ
	ネル数を設定する。
	尚V3.5 では、ＰＡＣ収集のチャネル数は ２ である。
	*/

#define SVN_EXE_NUM_OVERSMPL	(SVN_BASE_PROTOCOL_EXECUTION + 18)
#define SVT_EXE_NUM_OVERSMPL	VFT_FLT32
#define SVS_EXE_NUM_OVERSMPL	"The number of over sample"
/*	データ補間実施時のオーバーサンプリングの倍率（補間前後のデータ数の比）
	1->オーバーサンプリングしない。
	n->n倍のオーバーサンプリングする。
 */

#define SVN_EXE_CMD_BEFORE_PRESCAN	(SVN_BASE_PROTOCOL_EXECUTION + 19)
#define SVT_EXE_CMD_BEFORE_PRESCAN	VFT_STRING
#define SVS_EXE_CMD_BEFORE_PRESCAN	"Command before prescan"
/*
 */

#define SVN_EXE_CMD_BEFORE_SCAN		(SVN_BASE_PROTOCOL_EXECUTION + 20)
#define SVT_EXE_CMD_BEFORE_SCAN		VFT_STRING
#define SVS_EXE_CMD_BEFORE_SCAN		"Command before scan"
/*
 */

#define SVN_EXE_CMD_AFTER_SCAN		(SVN_BASE_PROTOCOL_EXECUTION + 21)
#define SVT_EXE_CMD_AFTER_SCAN		VFT_STRING
#define SVS_EXE_CMD_AFTER_SCAN		"Command after scan"
/*
 */

#define SVN_EXE_AUTO_VOICE	(SVN_BASE_PROTOCOL_EXECUTION + 22)
#define SVT_EXE_AUTO_VOICE	VFT_INT32
#define SVS_EXE_AUTO_VOICE	"The flag for auto voice."
/*	撮影の前後でのオートボイス制御フラグ。PLN のオートボイスとは
	異なる。
	２個の音声種からなる配列。第1要素がプリスキャン時オートボイス、
	第2要素が本スキャン時オートボイス。
	
	0:オートボイスOFF
	1:息止め音声１		「息を吸って止めてください」
	2:息止め音声２		「息を吐いて止めてください」
	3:息止め音声３		「息を吸って吐いて止めてください」
	4:体動防止音声		「動かないでください」
	*/

#define SVN_EXE_PRESCAN_RGN_OFFSET 	(SVN_BASE_PROTOCOL_EXECUTION + 23)
#define SVT_EXE_PRESCAN_RGN_OFFSET 	VFT_FLT32
#define SVS_EXE_PRESCAN_RGN_OFFSET	"Receiver Gain offset (dB)"
/*	CLINICAL,USE_VARFIELD,USE_PREVIOUS で与えられるレシーバゲインに
	対してオフセットを加える。
	*/

#define SVN_EXE_SCAN_AGENT	(SVN_BASE_PROTOCOL_EXECUTION + 24)
#define SVT_EXE_SCAN_AGENT	VFT_STRING
#define SVS_EXE_SCAN_AGENT	"Scan Agent"
/*	プロトコルに対して予約される種々の処理を記述する。
 */

#define SVN_EXE_CMD_INDEX	(SVN_BASE_PROTOCOL_EXECUTION + 25)
#define SVT_EXE_CMD_INDEX	VFT_INT32
#define SVS_EXE_CMD_INDEX	"Command index for prescan/scan"
/*	CMD_BEFORE_PRESCAN, CMD_BEFORE_SCAN, CMD_AFTER_SCAN の
	置き換えを制御する。
	v[0] : CMD_BEFORE_PRESCAN
	v[1] : CMD_BEFORE_SCAN
	v[2] : CMD_AFTER_SCAN
	pqm が v[n] > 0 の場合に EXE_CMD を実施する前に Command Table
	に応じてフィールドの置換を行う。
	*/

#define SVN_EXE_PRESCAN_TGC_RFOUT_RATIO	(SVN_BASE_PROTOCOL_EXECUTION + 26)
#define SVT_EXE_PRESCAN_TGC_RFOUT_RATIO	VFT_FLT32
#define SVS_EXE_PRESCAN_TGC_RFOUT_RATIO	"RF output power loss ratio. (output / input)"
/*	TGC処理で測定したRFアンプの出力値と入力値の比率。
	SAR計算で使用する。
	*/

#define SVN_EXE_PREP_SCAN_DELAY_TIME	(SVN_BASE_PROTOCOL_EXECUTION + 27)
#define SVT_EXE_PREP_SCAN_DELAY_TIME	VFT_INT32
#define SVS_EXE_PREP_SCAN_DELAY_TIME	"Delay Time for PrepScan"
/*	モニタスキャンから本スキャンに移行する際にスタートボタンを
	押してから実際にシーケンスが起動するまでの時間を msec 単位で
	記述する。
	*/

#define SVN_EXE_PAC_PATTERN	(SVN_BASE_PROTOCOL_EXECUTION + 28)
#define SVT_EXE_PAC_PATTERN	VFT_INT32
#define SVS_EXE_PAC_PATTERN	"Channel conection pattern for P.A.C scan"
/*	PACの各チャネルとVAPボードとの対応付けを行う。
	4ビット毎をVAPの各ボードに対応させPACのチャネル番号を格納する。
	下位ビットからVAPボード A,B,C,D とする。
	     COIL
            +-----+
            |  1  |       VAP
            +-----+     +-----+
            |  2  |     |  A  |--+   左のような接続の場合、
            +-----+     +-----+  |          3 --> C
            |  3  |---+ |  B  |  |          4 --> D
            +-----+   | +-----+  |          5 --> A
            |  4  |-+ +-|  C  |  |   A に対応するチャネル番号5を下位4ビットに
            +-----+ |   +-----+  |   する 0x4305 を格納することになる。
          +-|  5  | +---|  D  |  |
          | +-----+     +-----+  |
          | |  6  |              |
          | +-----+              |
          +----------------------+
	*/

#define SVN_EXE_HOME_POSITION	(SVN_BASE_PROTOCOL_EXECUTION + 29)
#define SVT_EXE_HOME_POSITION	VFT_FLT32
#define SVS_EXE_HOME_POSITION	"Home Couch Position"
/*	寝台ホームポジション
	患者セッティング後、最初のスキャン開始時の寝台位置
	*/

#define SVN_EXE_COUCH_OFFSET	(SVN_BASE_PROTOCOL_EXECUTION + 30)
#define SVT_EXE_COUCH_OFFSET	VFT_FLT32
#define SVS_EXE_COUCH_OFFSET	"Offset for coordinate system conversion"
/*	寝台移動時の座標系変換における、
	オフセットベクタ（ｚ成分）の変換量（offset）。
	ホームポジション座標系オフセット（ＨＰ）
	磁場中心座標系オフセット（ＣＰ）としたとき、
	ＨＰからＣＰへの変換：ＣＰ＝ＨＰ＋ offset
	ＣＰからＨＰへの変換：ＨＰ＝ＣＰ－ offset
	*/

#define SVN_EXE_PARENT_COUCH_OFFSET	(SVN_BASE_PROTOCOL_EXECUTION + 31)
#define SVT_EXE_PARENT_COUCH_OFFSET	VFT_FLT32
#define SVS_EXE_PARENT_COUCH_OFFSET	"Offset for coordinate system conversion"
/*	寝台移動時の座標系変換における、親画像の 
	オフセットベクタ（ｚ成分）の変換量（offset）。
	ホームポジション座標系オフセット（ＨＰ）
	磁場中心座標系オフセット（ＣＰ）としたとき、
	ＨＰからＣＰへの変換：ＣＰ＝ＨＰ＋ offset
	ＣＰからＨＰへの変換：ＨＰ＝ＣＰ－ offset
	*/

#define SVN_EXE_CHANNEL_GAIN_RATIO	(SVN_BASE_PROTOCOL_EXECUTION + 32)
#define SVT_EXE_CHANNEL_GAIN_RATIO	VFT_FLT32
#define SVS_EXE_CHANNEL_GAIN_RATIO	"Channel gain ratio"
/* 	コイルエレメント毎のゲイン補正係数をVFEPの各チャネルに再配分した
	もの。(SDM45-0498にて新規定義)
	VFT_FLT32 * 8 の配列とするが、使用するのは４要素のみ。
	SVN_EXE_PAC_PATTERN が有効な場合にのみ機能する。
	SVN_EXE_PAC_PATTERN が無効で、本フィールドが存在する場合は無視
	されなければならない。
	（例）Path=0x000043221 PAC_PATTERN=0x4321 (V4.20)
	        ┏━┳━┳━┳━┳━┳━┳━┳━┓
	コイル  ┃0 ┃1 ┃2 ┃3 ┃4 ┃5 ┃6 ┃7 ┃
	        ┗┯┻┯┻┯┻┯┻┯┻━┻━┻━┛
		  │  ├─┘  │  │
		  │  │  ┌─┘  │
		  │  │  │  ┌─┘
		┏┷┳┷┳┷┳┷┓
	VFEP	┃0 ┃1 ┃2 ┃3 ┃
		┗━┻━┻━┻━┛
	v[0] = p[0]
	v[1] =(p[1] + p[2]) / 2
	v[2] = p[3]
	v[3] = p[4]
 */

#define SVN_EXE_CHANNEL_PER_VFEP	(SVN_BASE_PROTOCOL_EXECUTION + 33)
#define SVT_EXE_CHANNEL_PER_VFEP	VFT_INT32
#define SVS_EXE_CHANNEL_PER_VFEP	"Channel Per VFEP"
/*	そのプロトコルの撮影で、単一のデータ収集ユニットで処理するチャネル数。
	以下の３つの情報から決める。
	 (1)選択されているコイルのチャネル数(ユーザ選択 & coilDB)
	 (2)実装しているデータ収集ユニット数(/gp/etc/sm.cf)
	 (3)単一のデータ収集ユニットで処理可能なチャネル数(/gp/etc/sm.cf)
 */

#define SVN_EXE_PAC_PHASE_CORRECT_PARAMS	(SVN_BASE_PROTOCOL_EXECUTION + 34)
#define SVT_EXE_PAC_PHASE_CORRECT_PARAMS	VFT_FLT32
#define SVS_EXE_PAC_PHASE_CORRECT_PARAMS	"PAC Phase Correction Params"     
/*	ch間位相差測定ツール用のフィールド
	SVN_PRC_PAC_CORRECT_ACTION のVFC_PAC_PHASE_CORRECTのビットがたっている場合、
	再構成にて利用され、ch間の位相差の補正が行なわれる。
	要素数 SVN_EXE_NUM_PAC_SCAN 分セットされている。

 */

#define SVN_EXE_PAC_GAIN_CORRECT_PARAMS        (SVN_BASE_PROTOCOL_EXECUTION + 35)
#define SVT_EXE_PAC_GAIN_CORRECT_PARAMS        VFT_FLT32                         
#define SVS_EXE_PAC_GAIN_CORRECT_PARAMS        "PAC Gain Correction Params"     
/*	ch間位相差測定ツール用のフィールド
	SVN_PRC_PAC_CORRECT_ACTION のVFC_PAC_GAIN_CORRECTのビットがたっている場合、
	再構成にて利用され、ch間のゲイン差の補正が行なわれる。
	要素数 SVN_EXE_NUM_PAC_SCAN 分セットされている。

 */

#define SVN_EXE_PROBE_THRESHOLD_MAX        (SVN_BASE_PROTOCOL_EXECUTION + 36)
#define SVT_EXE_PROBE_THRESHOLD_MAX        VFT_FLT32                         
#define SVS_EXE_PROBE_THRESHOLD_MAX        "Threshold max value for probing"     
/*	proProbeがprobeDataに閾値の最大値をポイント数でセットする。
        probing scan時(SVN_PLN_PROBE_CORRECTIONがOn)、pqmがこのファイルと
        Matrix,FOV情報から距離に変換し、mmの単位でセットする。
        acqman,RMが参照する。
 */

#define SVN_EXE_PROBE_THRESHOLD_MIN        (SVN_BASE_PROTOCOL_EXECUTION + 37)
#define SVT_EXE_PROBE_THRESHOLD_MIN        VFT_FLT32                         
#define SVS_EXE_PROBE_THRESHOLD_MIN        "Threshold min value for probing"     
/*	proProbeがprobeDataに閾値の最小値をポイント数でセットする。
        probing scan時(SVN_PLN_PROBE_CORRECTIONがOn)、pqmがこのファイルと
        Matrix,FOV情報から距離に変換し、mmの単位でセットする。
        acqman,RMが参照する。
 */

#define SVN_EXE_IMAGING_SCAN_START		(SVN_BASE_PROTOCOL_EXECUTION + 38)
#define SVT_EXE_IMAGING_SCAN_START		VFT_TIME
#define SVS_EXE_IMAGING_SCAN_START		"Imaging Scan time"
/*      VisualPrep撮影等での実際の本撮影開始時刻（ディレイ含）をセットする。
        acqmanが RMに対して最初の本撮影開始要求を出した時刻。
        SVN_EXE_SCAN_TIME より遅い時間になる。
        しかし先頭にPrepが無い場合はほとんど変らない時刻になる。
        reconが造影剤注入時間計算するのに参照する。
*/

#define SVN_EXE_INJECT_TIME		(SVN_BASE_PROTOCOL_EXECUTION + 39)
#define SVT_EXE_INJECT_TIME		VFT_TIME
#define SVS_EXE_INJECT_TIME		"Prep Inject time"
/*      PQMがVisualPrep撮影時に造影剤注入時刻をセットする。
        造影剤注入からの時間計算するの使用する。
        recon が参照する。
*/
#define SVN_EXE_CHANGED_RECEIVE_COIL     (SVN_BASE_PROTOCOL_EXECUTION + 40)
#define SVT_EXE_CHANGED_RECEIVE_COIL     VFT_INT32
#define SVS_EXE_CHANGED_RECEIVE_COIL     "Changed Receiver coil"
/*      動的に受信コイル切り替えの有無
        TRUE (1): 切り替え有り
	FALSE(0): 切り替え無し
	このフィールドが無い場合、FALSE:切り替え無しとする。
	pqmがセットして acqman/recon等が参照する。
        */

#define SVN_EXE_RECVR_COIL_TBL          (SVN_BASE_PROTOCOL_EXECUTION + 41)
#define SVT_EXE_RECVR_COIL_TBL          VFT_INT32
#define SVS_EXE_RECVR_COIL_TBL          "Receiver coil Database Key table"
/*      動的に受信コイルが変る時の受信コイルKeyテーブル。
	SVS_EXE_CHANGED_RECEIVE_COILがTRUEの場合、
        総撮像回数分(SVN_PLN_REPETITIONS)存在する。
        pqmがセットして acqman/recon等が参照する。
        */

#define SVN_EXE_RECVR_COILNAME_TBL          (SVN_BASE_PROTOCOL_EXECUTION + 42)
#define SVT_EXE_RECVR_COILNAME_TBL          VFT_STRING
#define SVS_EXE_RECVR_COILNAME_TBL          "Receiver Coil name table"
/*      動的に受信コイルが変る時の受信コイル名のテーブル。
	コイル名称はセミコロン';' で区切られて入る。
	SVS_EXE_CHANGED_RECEIVE_COILがTRUEの場合、
        総撮像回数分(SVN_PLN_REPETITIONS)存在する。
	pqmがセットして reconが参照する。
        */

#define SVN_EXE_PAC_PATTERN_TBL	(SVN_BASE_PROTOCOL_EXECUTION + 43)
#define SVT_EXE_PAC_PATTERN_TBL	VFT_INT32
#define SVS_EXE_PAC_PATTERN_TBL	"Channel conection pattern for P.A.C scan table"
/*	動的に受信コイルが変る時のPACの各チャネルとVAPボードとの対応付け
        を行うテーブル。
	SVS_EXE_CHANGED_RECEIVE_COILがTRUEの場合、
        総撮像回数分(SVN_PLN_REPETITIONS)存在する。
	pqmがセットして reconが参照する。

	4ビット毎をVAPの各ボードに対応させPACのチャネル番号を格納する。
	下位ビットからVAPボード A,B,C,D とする。
	     COIL
            +-----+
            |  1  |       VAP
            +-----+     +-----+
            |  2  |     |  A  |--+   左のような接続の場合、
            +-----+     +-----+  |          3 --> C
            |  3  |---+ |  B  |  |          4 --> D
            +-----+   | +-----+  |          5 --> A
            |  4  |-+ +-|  C  |  |   A に対応するチャネル番号5を下位4ビットに
            +-----+ |   +-----+  |   する 0x4305 を格納することになる。
          +-|  5  | +---|  D  |  |
          | +-----+     +-----+  |
          | |  6  |              |
          | +-----+              |
          +----------------------+
	*/

#define SVN_EXE_CHANNEL_GAIN_RATIO_TBL	(SVN_BASE_PROTOCOL_EXECUTION + 44)
#define SVT_EXE_CHANNEL_GAIN_RATIO_TBL	VFT_FLT32
#define SVS_EXE_CHANNEL_GAIN_RATIO_TBL	"Channel gain ratio table"
/* 	コイルエレメント毎のゲイン補正係数をVFEPの各チャネルに再配分した
	もの。(SDM45-XXXXにて新規定義)
	VFT_FLT32 * 8 の配列とする。
	SVS_EXE_CHANGED_RECEIVE_COIL が有効な場合にのみ機能する。
	SVS_EXE_CHANGED_RECEIVE_COIL が無効で、本フィールドが存在する場合は無視
	されなければならない。
	（例）Path=0x000043221 PAC_PATTERN=0x4321 (V5.30)
	        ┏━┳━┳━┳━┳━┳━┳━┳━┓
	コイル  ┃0 ┃1 ┃2 ┃3 ┃4 ┃5 ┃6 ┃7 ┃
	        ┗┯┻┯┻┯┻┯┻┯┻━┻━┻━┛
		  │  ├─┘  │  │
		  │  │  ┌─┘  │
		  │  │  │  ┌─┘
		┏┷┳┷┳┷┳┷┳━┳━┳━┳━┓
	VFEP	┃0 ┃1 ┃2 ┃3 ┃4 ┃5 ┃6 ┃7 ┃
		┗━┻━┻━┻━┻━┻━┻━┻━┛
	v[0] = p[0]
	v[1] =(p[1] + p[2]) / 2
	v[2] = p[3]
	v[3] = p[4]
 */
#define SVN_EXE_PAC_PHASE_CORRECT_PARAMS_TBL	(SVN_BASE_PROTOCOL_EXECUTION + 45)
#define SVT_EXE_PAC_PHASE_CORRECT_PARAMS_TBL	VFT_FLT32
#define SVS_EXE_PAC_PHASE_CORRECT_PARAMS_TBL	"PAC Phase Correction Params Table"     
/*	ch間位相差測定ツール用のフィールド
	SVN_PRC_PAC_CORRECT_ACTION のVFC_PAC_PHASE_CORRECTのビットがたっている場合、
	再構成にて利用され、ch間の位相差の補正が行なわれる。
	要素数 SVN_EXE_NUM_PAC_SCAN 分セットされている。
	ただし、サイズとしては、８ch分xMAXの場合16個分ある。
 */

#define SVN_EXE_PAC_GAIN_CORRECT_PARAMS_TBL    (SVN_BASE_PROTOCOL_EXECUTION + 46)
#define SVT_EXE_PAC_GAIN_CORRECT_PARAMS_TBL    VFT_FLT32                         
#define SVS_EXE_PAC_GAIN_CORRECT_PARAMS_TBL    "PAC Gain Correction Params Table"     
/*	ch間位相差測定ツール用のフィールド
	SVN_PRC_PAC_CORRECT_ACTION のVFC_PAC_GAIN_CORRECTのビットがたっている場合、
	再構成にて利用され、ch間のゲイン差の補正が行なわれる。
	要素数 SVN_EXE_NUM_PAC_SCAN 分セットされている。
	ただし、サイズとしては、８ch分xMAXの場合16個分ある。
 */

#define SVN_EXE_RGN_COVERAGE_TYPE		(SVN_BASE_PROTOCOL_EXECUTION + 47)
#define SVT_EXE_RGN_COVERAGE_TYPE		VFT_INT32
#define SVS_EXE_RGN_COVERAGE_TYPE		"RGN coverage acquisition type"
/*	マルチカバレージの時のRGN(プレスキャン)の収集タイプ。
　      中心カバレージだけで収集してRGN算出するか、
        全カバレージで収集してRGN算出するか、のモード。
	pqm     -> acqman
	(セット)　 (参照)
	*/

#define	SVN_EXE_AMB_ACTUAL_COUCH	(SVN_BASE_PROTOCOL_EXECUTION + 48)
#define	SVT_EXE_AMB_ACTUAL_COUCH	VFT_INT32
#define	SVS_EXE_AMB_ACTUAL_COUCH	"Advanced MovingBed Actual Couch position"
/*	Advanced MovingBed 撮像時の実寝台位置
　  AMB撮像時にacqmanがセットする。
 */

#define SVN_EXE_MRS_REF_RGN_VALUE 	(SVN_BASE_PROTOCOL_EXECUTION + 49)
#define SVT_EXE_MRS_REF_RGN_VALUE 	VFT_FLT32
#define SVS_EXE_MRS_REF_RGN_VALUE	"MRS Ref Receiver Gain value (dB)"
/*  MRS Ref収集におけるRGN値
 */

#define SVN_EXE_PAC_MATRIX 		(SVN_BASE_PROTOCOL_EXECUTION + 50)
#define SVT_EXE_PAC_MATRIX 		VFT_UINT32
#define SVS_EXE_PAC_MATRIX		"Channel conection matrix for P.A.C scan"
/*	従来のPAC_PATTERNでは、8chまでしか対応できない。今回、新たに
	PAC_PATTERNの代わりにSVN_EXE_PAC_MATRIXを設けるものとする。
	512byte(1～128ch)固定を指定するものとする。上位byteから順に
	4byte単位で1ch、2ch、3ch・・・・と定義する。内容は、使用する
	コイルエレメント番号が入っているものとする。(未使用の場合０を設定する。)
 */

#define SVN_EXE_PAC_MATRIX_TBL 		(SVN_BASE_PROTOCOL_EXECUTION + 51)
#define SVT_EXE_PAC_MATRIX_TBL 		VFT_UINT32
#define SVS_EXE_PAC_MATRIX_TBL		"Channel connection matrix for P.A.C scan table"
/*	動的に受信コイルが変る時のPACの各チャネルとVAPボードとの対応付けを行うテーブル。
	今回、新たにPAC_PATTERNの代わりにSVN_EXE_PAC_MATRIX を設けたので、PAC_MATRIXに
	対応するテーブルを新規追加する。
	SVN_EXE_CHANGED_RECEIVE_COILがTRUEの場合、
	総撮像回数分(SVN_PLN_REPETITIONS)存在する。
	pqmがセットして reconが参照する。
 */

#define	SVN_EXE_CONNECTED_COIL		(SVN_BASE_PROTOCOL_EXECUTION + 52)
#define	SVT_EXE_CONNECTED_COIL		VFT_INT32
#define	SVS_EXE_CONNECTED_COIL		"Connected coil Database Key"
/*	ポートに接続されているコイル。WBコイルを含む
	*/

#define	SVN_EXE_MOVE_COUCH_METHOD	(SVN_BASE_PROTOCOL_EXECUTION + 53)
#define	SVT_EXE_MOVE_COUCH_METHOD	VFT_INT32
#define	SVS_EXE_MOVE_COUCH_METHOD	"Move Couch Method"
/*	磁場中心への移動指定(C)時に、PQM設定により位置決め中心へ移動するモードと
	親画像中心へ移動するモードがあるが、どちらのモードで動作したかフィールドに
	保存されていないため、あとでスタディファイルから判断することができません。
	そのために寝台移動方法をどのモードで行ったのかをフィールドに保存する。
	*/

#define SVN_EXE_DEGENERACY_PAC_MATRIX	(SVN_BASE_PROTOCOL_EXECUTION + 54)
#define SVT_EXE_DEGENERACY_PAC_MATRIX	VFT_UINT32
#define SVS_EXE_DEGENERACY_PAC_MATRIX	"Degeneracy Channel Table"
/*	sectionDBに定義されているMinCh(Sectionの最小出力数)、InsPri(Section内の優先度)、
	Sig(Signal番号)から使用する生データのPAC_MATRIXテーブルの作成を行う。
	SVN_PLN_DEGENERACY_FLAGがTRUEの場合、作成する。

	設定者 acqman
	使用者 recon
	*/

#define SVN_EXE_NUM_PAC_DEGENERACY	(SVN_BASE_PROTOCOL_EXECUTION + 55)
#define SVT_EXE_NUM_PAC_DEGENERACY	VFT_INT16
#define SVS_EXE_NUM_PAC_DEGENERACY	"No. of Degeneracy"
/*	縮退後のPAC数。
	受信データ(生データ)の縮退を行う場合、縮退後のPAC数を設定する。
	縮退しない場合、0を設定する。

	設定者 pqm
	使用者 acqman,recon
	*/

#define SVN_EXE_N_IMAGING_SCAN_START	(SVN_BASE_PROTOCOL_EXECUTION + 56)
#define SVT_EXE_N_IMAGING_SCAN_START	VFT_LOCAL_TIME
#define SVS_EXE_N_IMAGING_SCAN_START	"Imaging Scan time"
/*      VisualPrep撮影等での実際の本撮影開始時刻（ディレイ含）をセットする。
        acqmanが RMに対して最初の本撮影開始要求を出した時刻。
        SVN_EXE_SCAN_TIME より遅い時間になる。
        しかし先頭にPrepが無い場合はほとんど変らない時刻になる。
        reconが造影剤注入時間計算するのに参照する。
*/

#define SVN_EXE_N_INJECT_TIME		(SVN_BASE_PROTOCOL_EXECUTION + 57)
#define SVT_EXE_N_INJECT_TIME		VFT_LOCAL_TIME
#define SVS_EXE_N_INJECT_TIME		"Prep Inject time"
/*      PQMがVisualPrep撮影時に造影剤注入時刻をセットする。
        造影剤注入からの時間計算するの使用する。
        recon が参照する。

        設定者：PQM
        使用者：RECON
*/

#define SVN_EXE_NUM_MAP			(SVN_BASE_PROTOCOL_EXECUTION + 58)
#define SVT_EXE_NUM_MAP			VFT_INT32
#define SVS_EXE_NUM_MAP			"The number of map"
/*      map撮影で縮退Onの場合に作成する。(収集chのmapデータ作成後、縮退mapデータを作成。)
        このフィールドはPAS更新対象外である。

        設定者：RECON
        使用者：RECON
*/

#define SVN_EXE_PROCESSED_NUM_MAP	(SVN_BASE_PROTOCOL_EXECUTION + 59)
#define SVT_EXE_PROCESSED_NUM_MAP	VFT_INT32
#define SVS_EXE_PROCESSED_NUM_MAP	"Sensitivity data processed index for the degeneracy"
/*      map撮影のprocessed番号を記述するフィールド。processed番号の配列。
        map撮影で縮退Onの場合に作成する。(収集chのmapデータprocessed番号、
        縮退mapデータprocessed番号)
        このフィールドはPAS更新対象外である。

        設定者：RECON
        使用者：RECON
*/

#define SVN_EXE_PRESCAN_NUM_MTGC		(SVN_BASE_PROTOCOL_EXECUTION + 60)
#define SVT_EXE_PRESCAN_NUM_MTGC		VFT_INT32
#define SVS_EXE_PRESCAN_NUM_MTGC		"The number of repetitions of mTGC"
/*	mTGCのパルス数。
	mTGCがスキップした場合0が入る。

	設定者：sequence
	使用者：acqman
	*/

#define SVN_EXE_PRESCAN_MTGC_INDEX		(SVN_BASE_PROTOCOL_EXECUTION + 61)
#define SVT_EXE_PRESCAN_MTGC_INDEX		VFT_INT32
#define SVS_EXE_PRESCAN_MTGC_INDEX		"Index of RFL in mTGC"
/*	mTGCのパルス数のうち、RFLを実行した順番。

	設定者：sequence
	使用者：acqman
	*/

#define SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO	(SVN_BASE_PROTOCOL_EXECUTION + 62)
#define SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO	VFT_FLTVEC2
#define SVS_EXE_PRESCAN_MTGC_RFOUT_RATIO	"Multi pulse RF output power loss ratio. ((W), (out / in))"
/*	mTGCの実行結果(X(W),Y(out/in))。
	mTGCがスキップした場合0が入る。

	設定者：X:sequence,Y:acqman
	使用者：acqman,sequence
	*/

#define SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX	(SVN_BASE_PROTOCOL_EXECUTION + 63)
#define SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX	VFT_FLT32
#define SVS_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX	"Maximum value of mTGC (output / input)"
/*	mTGCの実行結果(out/in)の最大値。

	設定者：acqman
	使用者：PROLIM
	*/

#define SVN_EXE_PRESCAN_STGC_RFOUT_RATIO	(SVN_BASE_PROTOCOL_EXECUTION + 64)
#define SVT_EXE_PRESCAN_STGC_RFOUT_RATIO	VFT_FLT32
#define SVS_EXE_PRESCAN_STGC_RFOUT_RATIO	"Single pulse RF output power loss ratio. (output / input)"
/*	sTGCの実行結果(out/in)。

	設定者：acqman
	使用者：PROLIM
	*/

#define SVN_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT	(SVN_BASE_PROTOCOL_EXECUTION + 65)
#define SVT_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT	VFT_FLT32
#define SVS_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT	"RF Level; RF Tx gain value (dB) limit"
/*	RFL実行時のTXgainの上限(dB)。

	設定者：acqman
	使用者：sequence,PROLIM
	*/

#define SVN_EXE_PRESCAN_RF_AMP_MODE_MAX		(SVN_BASE_PROTOCOL_EXECUTION + 66)
#define SVT_EXE_PRESCAN_RF_AMP_MODE_MAX		VFT_FLT32
#define SVS_EXE_PRESCAN_RF_AMP_MODE_MAX		"RFamp mode max (W)"
/*	RFampモードの最大値(W)。

	設定者：acqman
	使用者：sequence,PROLIM
	*/

#define SVN_EXE_PRESCAN_MRS_F0_ACTION (SVN_BASE_PROTOCOL_EXECUTION + 67)
#define SVT_EXE_PRESCAN_MRS_F0_ACTION VFT_INT32
#define SVS_EXE_PRESCAN_MRS_F0_ACTION "MRS F0 Prescan Action"
/*
    MRS F0 Prescan Action.
    MRSのF0プレスキャンを実行するかスキップするかを設定する。
    0:Skip      (VFC_PRESCAN_MRS_SKIP)
    1:Execute   (VFC_PRESCAN_MRS_EXE)
    設定者：PQM
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_EXE_PRESCAN_MRS_REF_ACTION (SVN_BASE_PROTOCOL_EXECUTION + 68)
#define SVT_EXE_PRESCAN_MRS_REF_ACTION VFT_INT32
#define SVS_EXE_PRESCAN_MRS_REF_ACTION "MRS Ref Prescan Action"
/*
    MRS Ref Prescan Action.
    MRSのRefプレスキャンを実行するかスキップするかを設定する。
    0:Skip      (VFC_PRESCAN_MRS_SKIP)
    1:Execute   (VFC_PRESCAN_MRS_EXE)
    設定者：PQM
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_EXE_PRESCAN_MRS_WSAT_ACTION (SVN_BASE_PROTOCOL_EXECUTION + 69)
#define SVT_EXE_PRESCAN_MRS_WSAT_ACTION VFT_INT32
#define SVS_EXE_PRESCAN_MRS_WSAT_ACTION "MRS Wsat Prescan Action"
/*
    MRS Wsat Prescan Action.
    MRSのWsatプレスキャンを実行するかスキップするかを設定する。
    0:Skip      (VFC_PRESCAN_MRS_SKIP)
    1:Execute   (VFC_PRESCAN_MRS_EXE)
    設定者：PQM
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_EXE_PRESCAN_RFL_SLICE_ORI (SVN_BASE_PROTOCOL_EXECUTION + 70)
#define SVT_EXE_PRESCAN_RFL_SLICE_ORI VFT_INT32
#define SVS_EXE_PRESCAN_RFL_SLICE_ORI "RF Level prescan slice orientation"
/*
    RFL実行時にrflDBから該当するコイル・部位で参照した断面指定の値を
    Acqmanが設定しシーケンスが参照する。
    1:SG    (VFC_RFL_SLICE_ORI_SG)
    2:CO    (VFC_RFL_SLICE_ORI_CO)
    3:AX    (VFC_RFL_SLICE_ORI_AX)
    設定者 : acqman
    使用者 : seqgen
*/

#define SVN_EXE_PROCESSED_FWS   (SVN_BASE_PROTOCOL_EXECUTION + 71)
#define SVT_EXE_PROCESSED_FWS   VFT_INT32
#define SVS_EXE_PROCESSED_FWS   "Processed index for FWS"
/*
    Processed index for FWS
    設定者 : RECON
    使用者 : RECON
*/

#define SVN_EXE_CDS_COIL_LOCATION  (SVN_BASE_PROTOCOL_EXECUTION + 72)
#define SVT_EXE_CDS_COIL_LOCATION  VFT_CDS_COIL_LOCATION
#define SVS_EXE_CDS_COIL_LOCATION  "CDS coil location"
/*
    磁場中心からのコイル中心位置。Z軸の座標符号は寝台位置と同じ。
    PAS更新対象外
    設定者：acqman
    使用者：pqm
    {CoilID + detectedFlag + コイル位置}の配列。
    配列順序はacqmanからpqmへの接続コイル情報と同じで、
    コイルポートの順番である.
     = {A/空/空/D1/D2/F/G1/--/G7}（１３個）
    SVN_PLN_CDS_COIL_COORDINATEと同じフォーマット
*/

#define SVN_EXE_RFL_COUCH			(SVN_BASE_PROTOCOL_EXECUTION + 73)
#define SVT_EXE_RFL_COUCH			VFT_FLT32
#define SVS_EXE_RFL_COUCH			"RFL Couch position"
/*
	撮像時に使用されたRFL情報が決定されたときのプレスキャン寝台位置。
	設定者：PQM
	使用者：PQM
*/

#define SVN_EXE_CFA_COUCH			(SVN_BASE_PROTOCOL_EXECUTION + 74)
#define SVT_EXE_CFA_COUCH			VFT_FLT32
#define SVS_EXE_CFA_COUCH			"CFA Couch position"
/*
	撮像時に使用されたCFA情報が決定されたときのプレスキャン寝台位置。
	設定者：PQM
	使用者：PQM
*/

#define SVN_EXE_SHIM_AFTER_FSY_VALUE	(SVN_BASE_PROTOCOL_EXECUTION + 75)
#define SVT_EXE_SHIM_AFTER_FSY_VALUE	VFT_FLT64
#define SVS_EXE_SHIM_AFTER_FSY_VALUE	"After shimmng Center Frequency value (Hz)"
/*
	シミング処理後のCFAで得られた中心周波数。シミングプロトコルのみに設定する。
	設定者：PQM
	使用者：PQM
*/

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_EXECUTION SVN_MIN_SUBPROTOCOL+SVN_OFF_EXECUTION


#define SVN_EXE_SCAN_START		(SVN_BASE_SUBPROTOCOL_EXECUTION + 0)
#define SVT_EXE_SCAN_START		VFT_TIME
#define SVS_EXE_SCAN_START		"Time sequence actually started"

#define SVN_EXE_ACQ_STATUS		(SVN_BASE_SUBPROTOCOL_EXECUTION + 1)
#define SVT_EXE_ACQ_STATUS		VFT_STATUS
#define SVS_EXE_ACQ_STATUS		"Acquisition status after subprotocol"
/*	STUB; it is not yet clear what type is appropriate
        */

#define SVN_EXE_NUM_TIME_STAMP		(SVN_BASE_SUBPROTOCOL_EXECUTION + 2)
#define SVT_EXE_NUM_TIME_STAMP		VFT_INT32
#define SVS_EXE_NUM_TIME_STAMP		"The number of time stamps"
/*	ダイナミックスキャンテーブル（SVN_EXE_TIME_STAMP）に格納されている
        実収集時刻の有効数を格納する。
  	*/

#define SVN_EXE_TIME_STAMP		(SVN_BASE_SUBPROTOCOL_EXECUTION + 3)
#define SVT_EXE_TIME_STAMP		VFT_INT32
#define SVS_EXE_TIME_STAMP		"Time stamp table"
/*	ダイナミックスキャン実収集時刻（計画スキャン数：SVN_PLN_REPETITIONS 
	の配列）。
	ダイナミックスキャンのときスキャンが実際に開始した時刻を格納する。
	格納する時刻は、ScanStart からの相対時刻（ミリ秒単位）とする。
	
	格納領域数は、計画スキャン数（SVN_PLN_REPETITIONS）分あり実際にセット
	されている数を実収集時刻有効数（SVN_EXE_NUM_TIME_STAMP）に格納する。
	*/

#define SVN_EXE_NUM_COUCH_TABLE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 4)
#define SVT_EXE_NUM_COUCH_TABLE		VFT_INT32
#define SVS_EXE_NUM_COUCH_TABLE		"The number of couch position table"
/*	寝台位置テーブル（SVN_EXE_COUCH_TABLE）に格納されている
        実寝台位置の有効数を格納する。
	
    (1) 実寝台位置有効数	
 	寝台位置テーブル（SVN_EXE_COUCH_TABLE）に格納されている
 	実寝台位置の有効数を格納する。	
 	Moving Bedの計画に従い，実際にスキャンした回数。移動回数ではない。
    (2) acqmanがセットする。
    (3) msReconはDynamic Scanと同じように格納された生データをこの寝台位置分
 	再構成を繰り返す。
  	*/

#define SVN_EXE_COUCH_TABLE	(SVN_BASE_SUBPROTOCOL_EXECUTION + 5)
#define SVT_EXE_COUCH_TABLE	VFT_INT32
#define SVS_EXE_COUCH_TABLE	"couch position table"
/*	Moving Bed実寝台位置
	Moving Bedのとき各移動後の寝台位置を格納する。

    SUBPROTOCOL ノード
    (1)	Moving Bed撮影時実寝台位置。
 	単位 mm。絶対位置（負の値）が入る。
 	配列の大きさは，計画時移動の移動回数＋１となる。実際にスキャンした
 	回数分値が入る。
 	配列の第１要素には寝台移動前の最初のスキャンの寝台位置がセットされる。
           
 	P[0] = P
 	P[1] = P + v[1]
 	P[2] = P + v[2]
 	P[3] = P + v[3]
 	...
        ~~~~~~~~~~~~~~~
      p[0],p[1],,p[n] には実際の寝台位置が入る。

     (2) acqmanがセットする。
     (3) msReconはこのテーブルをもとに，該当する寝台位置を再構成した各ECHO
 	ノードにばらまく。

    ECHO ノード
    (1)	Moving Bed時実寝台位置。
 	単位 mm。絶対位置（負の値）が入る。
 	その画像のスキャン時の実寝台位置がセットされる。
    (2) msReconがセットする。
    (3) dss_viewは各ECHOノードにあるSVN_EXE_COUCH_TABLEからこの値を減算し，
 	単位をmmからcmに変換して付帯情報として表示する。
         dsc_insetは，この値とSVN_EXE_COUCH(PROTOCOLノード)の値が異なる時は，
 	参照表示を行わない。
	*/

#define SVN_EXE_PREP_SCAN_START	(SVN_BASE_SUBPROTOCOL_EXECUTION + 6)
#define SVT_EXE_PREP_SCAN_START	VFT_TIME
#define SVS_EXE_PREP_SCAN_START	"Prep Scan Start Time"
/*	VisualPrep撮影時のプレップスキャンの開始時間
	acqmanがSUBPROTOCOLノードにセットする。
	*/

#define SVN_EXE_NUM_TIME_STAMP_DRKS	(SVN_BASE_SUBPROTOCOL_EXECUTION + 7)
#define SVT_EXE_NUM_TIME_STAMP_DRKS	VFT_INT32
#define SVS_EXE_NUM_TIME_STAMP_DRKS	"The number of DRKS time stamps"
/*	ダイナミックスキャンテーブル（SVN_EXE_TIME_STAMP_DRKS）に格納されている
        計画スキャン時刻の有効数を格納する。
  	*/

#define SVN_EXE_TIME_STAMP_DRKS		(SVN_BASE_SUBPROTOCOL_EXECUTION + 8)
#define SVT_EXE_TIME_STAMP_DRKS		VFT_INT32
#define SVS_EXE_TIME_STAMP_DRKS		"DRKS Time stamp table"
/*	DRKS 再構成に使用する時刻（DRKS 再構成時相数の配列）
	ダイナミックスキャンの計画時の時相を基にシーケンス実行時に作成。
	実行されるシーケンスが再構成される画像の時相に合わせて作成する。
	格納する時刻は、ScanStart からの相対時刻（ミリ秒単位）とする。	
	実際にセットされている数を実収集時刻有効数
	（SVN_EXE_NUM_TIME_STAMP_DRKS）に格納する。
	*/

#define SVN_EXE_RETRO_TRIGTBL_TYPE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 9)
#define SVT_EXE_RETRO_TRIGTBL_TYPE		VFT_INT32
#define SVS_EXE_RETRO_TRIGTBL_TYPE		"Type of retro trigger table"
/*      RETRO_TRIG_TABLE のデータタイプ。
	*/

#define SVN_EXE_RETRO_TRIGTBL_SIZE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 10)
#define SVT_EXE_RETRO_TRIGTBL_SIZE		VFT_INT32
#define SVS_EXE_RETRO_TRIGTBL_SIZE		"Number of retro trigger table"
/*	RETRO_TRIG_TABLE のテーブルの要素数。
	*/

#define SVN_EXE_RETRO_TRIG_TABLE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 11)
#define SVT_EXE_RETRO_TRIG_TABLE		VFT_UINT32
#define SVS_EXE_RETRO_TRIG_TABLE		"Trigger table of retrospective gating"
/*	レトロスペクティブゲーティング撮影時の同期トリガのテーブル。
	データの単位はmsec。
	GM -> acqman -> retron と受け渡す。
	      (保存)    (使用)
	*/

#define SVN_EXE_SEQGEN_STATUS			(SVN_BASE_SUBPROTOCOL_EXECUTION + 12)
#define SVT_EXE_SEQGEN_STATUS			VFT_INT32
#define SVS_EXE_SEQGEN_STATUS			"Seqgen error ID"
/*	seqgen の実行時に acqMan を通した Error 処理を行うための ID
	id[2]: id[0]....Error Level, id[1]....Error Index
	*/

#define SVN_EXE_SEQGEN_MSG			(SVN_BASE_SUBPROTOCOL_EXECUTION + 13)
#define SVT_EXE_SEQGEN_MSG			VFT_STRING
#define SVS_EXE_SEQGEN_MSG			"Seqgen error message"
/*	seqgen の実行時に acqMan を通した Error 処理を行う場合の表示メッセージ
	*/


#define SVN_EXE_RETRO_ADCCOUNTTBL_TYPE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 14)
#define SVT_EXE_RETRO_ADCCOUNTTBL_TYPE		VFT_INT32
#define SVS_EXE_RETRO_ADCCOUNTTBL_TYPE		"Type of retro ADC counter table"
/*      RETRO_ADCCOUNT_TABLE のデータタイプ。
	*/

#define SVN_EXE_RETRO_ADCCOUNTTBL_SIZE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 15)
#define SVT_EXE_RETRO_ADCCOUNTTBL_SIZE		VFT_INT32
#define SVS_EXE_RETRO_ADCCOUNTTBL_SIZE		"Number of retro ADC counter table"
/*	RETRO_ADCCOUNT_TABLE のテーブルの要素数。
	*/

#define SVN_EXE_RETRO_ADCCOUNT_TABLE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 16)
#define SVT_EXE_RETRO_ADCCOUNT_TABLE		VFT_INT32
#define SVS_EXE_RETRO_ADCCOUNT_TABLE		"ADC counter table of retrospective gating"
/*	レトロスペクティブゲーティング(方式２)撮影での1R-R内で収集したADC数のテーブル。
	GM -> acqman -> retron と受け渡す。
	      (保存)    (使用)
	*/

#define SVN_EXE_RETRO_ADCTIMINGTBL_TYPE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 17)
#define SVT_EXE_RETRO_ADCTIMINGTBL_TYPE		VFT_INT32
#define SVS_EXE_RETRO_ADCTIMINGTBL_TYPE		"Type of retro ADC timing table"
/*      RETRO_ADCTIMING_TABLE のデータタイプ。
	*/

#define SVN_EXE_RETRO_ADCTIMINGTBL_SIZE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 18)
#define SVT_EXE_RETRO_ADCTIMINGTBL_SIZE		VFT_INT32
#define SVS_EXE_RETRO_ADCTIMINGTBL_SIZE		"Number of retro ADC timing table"
/*	RETRO_ADCTIMING_TABLE のテーブルの要素数。
	*/

#define SVN_EXE_RETRO_ADCTIMING_TABLE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 19)
#define SVT_EXE_RETRO_ADCTIMING_TABLE		VFT_UINT32
#define SVS_EXE_RETRO_ADCTIMING_TABLE		"ADC timing table of retrospective gating"
/*	レトロスペクティブゲーティング(方式２)撮影でのADC(収集)タイミング(時刻)のテーブル。
	データの単位は10usec。
	GM -> acqman -> retron と受け渡す。
	      (保存)    (使用)
	*/

#define SVN_EXE_SAR_WEIGHT		(SVN_BASE_SUBPROTOCOL_EXECUTION + 20)
#define SVT_EXE_SAR_WEIGHT		VFT_FLT32
#define SVS_EXE_SAR_WEIGHT		"Weight for SAR calculation"
/*	libSARで実際にSAR計算に使用した体重を、SAR構造体にセットし、
　      seqgen(libgt)がvf構造体にコピーし、スタディフィールドにセットする。
	*/

#define SVN_EXE_ANGLE_OFFSET		(SVN_BASE_SUBPROTOCOL_EXECUTION + 21)
#define SVT_EXE_ANGLE_OFFSET		VFT_FLT32
#define SVS_EXE_ANGLE_OFFSET		"Angle offset for phase correction prescan"
/*	位相補正プリスキャン時に各スライスの
	angleOffsetの配列を格納する。
	*/

#define SVN_EXE_RO_TUNE_VALUE		(SVN_BASE_SUBPROTOCOL_EXECUTION + 22)
#define SVT_EXE_RO_TUNE_VALUE		VFT_FLT32
#define SVS_EXE_RO_TUNE_VALUE		"RO tune value for phase correction prescan"
/*	位相補正プリスキャン時にROの1次量を
	スラブ毎に4つ格納する(スラブ数×4の配列)。
	*/

#define SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS	(SVN_BASE_SUBPROTOCOL_EXECUTION + 23)
#define SVT_EXE_PHASE_CORRECTION_PRESCAN_STATUS	VFT_INT32
#define SVS_EXE_PHASE_CORRECTION_PRESCAN_STATUS	"Status for phase correction prescan"
/*	位相補正プリスキャンのステータス
	0:None			(VFC_PHASE_CORRECTION_PRESCAN_NONE)
	1:OK			(VFC_PHASE_CORRECTION_PRESCAN_OK)
	2:Use Default		(VFC_PHASE_CORRECTION_PRESCAN_USE_DEFAULT)
	3:Retry	due to overflow	(VFC_PHASE_CORRECTION_PRESCAN_RETRY_DUE_TO_OVERFLOW)
	4:Retry	in algorithm	(VFC_PHASE_CORRECTION_PRESCAN_RETRY_IN_ALGORITHM)
	5:Failed		(VFC_PHASE_CORRECTION_PRESCAN_FAILED)
	*/

#define SVN_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START	(SVN_BASE_SUBPROTOCOL_EXECUTION + 24)
#define SVT_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START	VFT_TIME
#define SVS_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START	"Phase correction prescan Start time"
/*	位相補正プリスキャンの開始時間。
	acqmanは位相補正プリスキャンの収集開始時に現在時刻をセットする。
	*/

#define SVN_EXE_OVER_TEMP_PROTECTION_START_VALUE	(SVN_BASE_SUBPROTOCOL_EXECUTION + 25)
#define SVT_EXE_OVER_TEMP_PROTECTION_START_VALUE	VFT_FLT32
#define SVS_EXE_OVER_TEMP_PROTECTION_START_VALUE	"Start value of Over template protection"
/*	撮像開始時のOLP温度

	設定者 acqman
	使用者 特に無し
	*/

#define SVN_EXE_N_SCAN_START		(SVN_BASE_SUBPROTOCOL_EXECUTION + 26)
#define SVT_EXE_N_SCAN_START		VFT_LOCAL_TIME
#define SVS_EXE_N_SCAN_START		"Time sequence actually started"
/*	設定者：ACQMAN,RECON,MRS
	使用者：RECON,DISPLAY,FILEMAN,MRS
	*/

#define SVN_EXE_N_PREP_SCAN_START	(SVN_BASE_SUBPROTOCOL_EXECUTION + 27)
#define SVT_EXE_N_PREP_SCAN_START	VFT_LOCAL_TIME
#define SVS_EXE_N_PREP_SCAN_START	"Prep Scan Start Time"
/*	VisualPrep撮影時のプレップスキャンの開始時間
	acqmanがSUBPROTOCOLノードにセットする。

	設定者：ACQMAN
	使用者：RECON
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




/*****************************************************************************
  IMCALC name space
*/




#endif /* tami_vfStudy_vfs_execution_H__ */
