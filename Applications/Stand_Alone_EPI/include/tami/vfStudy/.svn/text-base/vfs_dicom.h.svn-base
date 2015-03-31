/*********************************************************************
 *
 *  (c) Copyright 1997 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Oct.30.97	古舘直幸	Created
 * End of History */



#ifndef tami_vfStudy_vfs_dicom_H__
#define tami_vfStudy_vfs_dicom_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */


/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_BASE_INDEPENDENT_DICOM	SVN_MIN_INDEPENDENT+SVN_OFF_DICOM

#define SVN_DCM_COMMIT_PROTECT   	(SVN_BASE_INDEPENDENT_DICOM + 1)
#define SVT_DCM_COMMIT_PROTECT   	VFT_INT32
#define SVS_DCM_COMMIT_PROTECT   	"protect flag for DICOM Commitment"
/*	保存委託中の画像データが削除されないようにプロテクトするためのフラグ。
 */


/*****************************************************************************
  STUDY name space
*/

#define SVN_BASE_STUDY_DICOM	SVN_MIN_STUDY+SVN_OFF_DICOM

#define SVN_DCM_ACCESSION_NUM	(SVN_BASE_STUDY_DICOM + 1)
#define SVT_DCM_ACCESSION_NUM	VFT_STRING
#define SVS_DCM_ACCESSION_NUM	"Acception Number"
/*	受付番号。
	検査についてのオーダーを識別するＲＩＳ発行の番号。
	*/

#define SVN_DCM_CODE_VALUE	(SVN_BASE_STUDY_DICOM + 2)
#define SVT_DCM_CODE_VALUE	VFT_STRING
#define SVS_DCM_CODE_VALUE	"Code Value"
/*	コード値
	依頼された処置のタイプを表すコード値。コード化体系によって定義される。
	*/


#define SVN_DCM_DESIGNATOR	(SVN_BASE_STUDY_DICOM + 3)
#define SVT_DCM_DESIGNATOR	VFT_STRING
#define SVS_DCM_DESIGNATOR	"Coding Scheme Designator"
/*	コード化体系指定子。
	コード値をコード意味へ写像するコード化体系を指定するコード値。
	*/


#define SVN_DCM_CODE_MEANING	(SVN_BASE_STUDY_DICOM + 4)
#define SVT_DCM_CODE_MEANING	VFT_STRING
#define SVS_DCM_CODE_MEANING	"Code Meaning"
/*	コード意味。
	コード値によって表現される依頼された処置。
	*/


#define SVN_DCM_STUDY_INSTANCE_UID	(SVN_BASE_STUDY_DICOM + 5)
#define SVT_DCM_STUDY_INSTANCE_UID	VFT_STRING
#define SVS_DCM_STUDY_INSTANCE_UID	"Study Instance UID"
/*	検査インスタンスＵＩＤ。
	検査を確認するために使用される固有識別子
	*/


#define SVN_DCM_STATION_AE	(SVN_BASE_STUDY_DICOM + 6)
#define SVT_DCM_STATION_AE	VFT_STRING
#define SVS_DCM_STATION_AE	"Scheduled Station AE Title"
/*	予約されたステーションＡＥ名称。
	予約された手続きステップが実行されることが予約されたモダリティの
	ＡＥ名称。
	*/


#define SVN_DCM_DESCRIPTION	(SVN_BASE_STUDY_DICOM + 7)
#define SVT_DCM_DESCRIPTION	VFT_STRING
#define SVS_DCM_DESCRIPTION	"Scheduled Procedure Step Description"
/*	予約された手続きステップ記述。
	実行される予約された手続きステップの施設生成の記述または分類。
	Institution-generated description or classification of the Scheduled
	Procedure Step to be performed.
	*/


#define SVN_DCM_PROCEDURE_ID	(SVN_BASE_STUDY_DICOM + 8)
#define SVT_DCM_PROCEDURE_ID	VFT_STRING
#define SVS_DCM_PROCEDURE_ID	"Scheduled Procedure Step ID"
/*	予約された手続きステップID。
	予約された手続きステップを識別する識別子。
	*/


#define SVN_DCM_REQUESTED_ID 	(SVN_BASE_STUDY_DICOM + 9)
#define SVT_DCM_REQUESTED_ID	VFT_STRING
#define SVS_DCM_REQUESTED_ID	"Requested Procedure ID"
/*	依頼された手続きＩＤ。
	画像サービス要求の中で依頼された手続きを確認する識別子。
	*/


#define SVN_DCM_EXAM_START_DATE_TIME	(SVN_BASE_STUDY_DICOM + 10)
#define SVT_DCM_EXAM_START_DATE_TIME	VFT_TIME
#define SVS_DCM_EXAM_START_DATE_TIME	"Date and Time at which the PPS started"
/*	オペレータにより検査の開始が指定された日時。
*/

#define SVN_DCM_EXAM_END_DATE_TIME	(SVN_BASE_STUDY_DICOM + 11)
#define SVT_DCM_EXAM_END_DATE_TIME	VFT_TIME
#define SVS_DCM_EXAM_END_DATE_TIME	"Date and Time at which the PPS ended"
/*	オペレータにより検査の終了が指定された日時。
*/

#define SVN_DCM_PPS_ID	(SVN_BASE_STUDY_DICOM + 12)
#define SVT_DCM_PPS_ID	VFT_STRING
#define SVS_DCM_PPS_ID	"User or equipment generated identifier"
/*	実行された手続きのユーザーあるいは装置が発行した識別子
*/

#define SVN_DCM_PPS_STEP_DESCRIPTION	(SVN_BASE_STUDY_DICOM + 13)
#define SVT_DCM_PPS_STEP_DESCRIPTION	VFT_STRING
#define SVS_DCM_PPS_STEP_DESCRIPTION	"Institution-generated description or classification of the PPS"
/*	実施済手続きステップの施設生成の記述あるいは分類
*/

#define SVN_DCM_PPS_TYPE_DESCRIPTION	(SVN_BASE_STUDY_DICOM + 14)
#define SVT_DCM_PPS_TYPE_DESCRIPTION	VFT_STRING
#define SVS_DCM_PPS_TYPE_DESCRIPTION	"a description of the type of the PPS"
/*	実施済手続きのタイプ記述
*/

#define SVN_DCM_PPS_STEP_COMMENT	(SVN_BASE_STUDY_DICOM + 15)
#define SVT_DCM_PPS_STEP_COMMENT	VFT_STRING
#define SVS_DCM_PPS_STEP_COMMENT	"User-defined comments on the PPS"
/*	実施済手続きステップについてのユーザー定義コメント
*/

#define SVN_DCM_PPS_STATUS	(SVN_BASE_STUDY_DICOM + 16)
#define SVT_DCM_PPS_STATUS	VFT_INT32
#define SVS_DCM_PPS_STATUS	"the state of the PPS"
/*	実施済手続きステップの状態
	値の定義
	VFC_MPPS_NONE		1
	VFC_MPPS_START		2
	VFC_MPPS_STOP		3
	VFC_MPPS_DONE		4
	VFC_MPPS_ERROR		5
*/


#define SVN_DCM_VERSION		(SVN_BASE_STUDY_DICOM + 17)
#define SVT_DCM_VERSION		VFT_STRING
#define SVS_DCM_VERSION		"Scheduled Coding Scheme Version"

#define SVN_DCM_REQ_CODE_VALUE	(SVN_BASE_STUDY_DICOM + 18)
#define SVT_DCM_REQ_CODE_VALUE	VFT_STRING
#define SVS_DCM_REQ_CODE_VALUE	"Requested Code Value"

#define SVN_DCM_REQ_DESIGNATOR	(SVN_BASE_STUDY_DICOM + 19)
#define SVT_DCM_REQ_DESIGNATOR	VFT_STRING
#define SVS_DCM_REQ_DESIGNATOR	"Requested Coding Scheme Designator"

#define SVN_DCM_REQ_CODE_MEANING	(SVN_BASE_STUDY_DICOM + 20)
#define SVT_DCM_REQ_CODE_MEANING	VFT_STRING
#define SVS_DCM_REQ_CODE_MEANING	"Requested Code Meaning"

#define SVN_DCM_REQ_VERSION	(SVN_BASE_STUDY_DICOM + 21)
#define SVT_DCM_REQ_VERSION	VFT_STRING
#define SVS_DCM_REQ_VERSION	"Requested Coding Scheme Version"

#define SVN_DCM_REQ_DESCRIPTION	(SVN_BASE_STUDY_DICOM + 22)
#define SVT_DCM_REQ_DESCRIPTION	VFT_STRING
#define SVS_DCM_REQ_DESCRIPTION	"Requested Procedure Step Description"

#define SVN_DCM_PHYSICIAN	(SVN_BASE_STUDY_DICOM + 23)
#define SVT_DCM_PHYSICIAN      	VFT_STRING
#define SVS_DCM_PHYSICIAN      	"Referenced Study Sequence"

#define SVN_DCM_REF_CLASS_UID  	(SVN_BASE_STUDY_DICOM + 24)
#define SVT_DCM_REF_CLASS_UID	VFT_STRING
#define SVS_DCM_REF_CLASS_UID   "Referenced SOP Class UID"

#define SVN_DCM_REF_INST_UID    (SVN_BASE_STUDY_DICOM + 25)
#define SVT_DCM_REF_INST_UID   	VFT_STRING
#define SVS_DCM_REF_INST_UID	"Referenced SOP Instance UID"

#define SVN_DCM_MPPS_CLASS_UID  (SVN_BASE_STUDY_DICOM + 26)
#define SVT_DCM_MPPS_CLASS_UID	VFT_STRING
#define SVS_DCM_MPPS_CLASS_UID  "MPPS SOP Class UID"

#define SVN_DCM_MPPS_INST_UID   (SVN_BASE_STUDY_DICOM + 27)
#define SVT_DCM_MPPS_INST_UID   VFT_STRING
#define SVS_DCM_MPPS_INST_UID	"MPPS SOP Instance UID"

#define SVN_DCM_PPS_CODE_VALUE	(SVN_BASE_STUDY_DICOM + 28)
#define SVT_DCM_PPS_CODE_VALUE	VFT_STRING
#define SVS_DCM_PPS_CODE_VALUE	"Requested Code Value"

#define SVN_DCM_PPS_DESIGNATOR	(SVN_BASE_STUDY_DICOM + 29)
#define SVT_DCM_PPS_DESIGNATOR	VFT_STRING
#define SVS_DCM_PPS_DESIGNATOR	"Requested Coding Scheme Designator"

#define SVN_DCM_PPS_CODE_MEANING	(SVN_BASE_STUDY_DICOM + 30)
#define SVT_DCM_PPS_CODE_MEANING	VFT_STRING
#define SVS_DCM_PPS_CODE_MEANING	"Requested Code Meaning"

#define SVN_DCM_PPS_VERSION	(SVN_BASE_STUDY_DICOM + 31)
#define SVT_DCM_PPS_VERSION	VFT_STRING
#define SVS_DCM_PPS_VERSION	"Requested Coding Scheme Version"

#define SVN_DCM_WORKLIST			(SVN_BASE_STUDY_DICOM + 32)
#define SVT_DCM_WORKLIST			VFT_DCM_WORKLIST_STRUCT
#define SVS_DCM_WORKLIST			"Modality WorkList"

#define SVN_DCM_REQ_CODE_SEQUENCE		(SVN_BASE_STUDY_DICOM + 33)
#define SVT_DCM_REQ_CODE_SEQUENCE		VFT_DCM_CODE_SEQ_STRUCT
#define SVS_DCM_REQ_CODE_SEQUENCE		"Requested Procedure Code Sequence"

#define SVN_DCM_REF_STUDY_SEQUENCE		(SVN_BASE_STUDY_DICOM + 34)
#define SVT_DCM_REF_STUDY_SEQUENCE		VFT_DCM_REF_STUDY_SEQ_STRUCT
#define SVS_DCM_REF_STUDY_SEQUENCE		"Referenced Study Sequence"

#define SVN_DCM_SPS_SEQUENCE			(SVN_BASE_STUDY_DICOM + 35)
#define SVT_DCM_SPS_SEQUENCE			VFT_DCM_SPS_SEQ_STRUCT
#define SVS_DCM_SPS_SEQUENCE			"Scheduled Procedure Step Sequence"

#define SVN_DCM_SAI_CODE_SEQUENCE		(SVN_BASE_STUDY_DICOM + 36)
#define SVT_DCM_SAI_CODE_SEQUENCE		VFT_DCM_CODE_SEQ_STRUCT
#define SVS_DCM_SAI_CODE_SEQUENCE		"Scheduled Action Item Code Sequence"

#define SVN_DCM_FILM_CONSUMPTION_SEQUENCE	(SVN_BASE_STUDY_DICOM + 37)
#define SVT_DCM_FILM_CONSUMPTION_SEQUENCE	VFT_DCM_FILM_CONSUMPTION_SEQ_STRUCT
#define SVS_DCM_FILM_CONSUMPTION_SEQUENCE	"Film Consumption Sequence"

#define SVN_DCM_PPS_INCREMENTAL_ID		(SVN_BASE_STUDY_DICOM + 38)
#define SVT_DCM_PPS_INCREMENTAL_ID		VFT_INT32
#define SVS_DCM_PPS_INCREMENTAL_ID		"PPS Incremental ID"

#define SVN_DCM_GROUPING_KIND			(SVN_BASE_STUDY_DICOM + 39)
#define SVT_DCM_GROUPING_KIND			VFT_INT32
#define SVS_DCM_GROUPING_KIND			"MWM Grouping Kind"

/* Value of SVN_DCM_GROUPING_KIND */
#define DCM_GROUPING_UNSCHEDULED	0	/* Unscheduled Case                  */
#define DCM_GROUPING_SIMPLE		1	/* Simple Case                       */
#define DCM_GROUPING_MULTI_REQUEST	2	/* Multi Request, Same Accession No  */
#define DCM_GROUPING_MULTI_ACCESSION	3	/* Multi Request, Multi Accession No */

#define SVN_DCM_PROTOCOL_PPS_SENT_MASK		(SVN_BASE_STUDY_DICOM + 40)
#define SVT_DCM_PROTOCOL_PPS_SENT_MASK		VFT_UINT32
#define SVS_DCM_PROTOCOL_PPS_SENT_MASK		"PPS Sent Protocols"

#define SVN_DCM_DISCONT_REASON_CODE_SEQUENCE	(SVN_BASE_STUDY_DICOM + 41)
#define SVT_DCM_DISCONT_REASON_CODE_SEQUENCE	VFT_DCM_CODE_SEQ_STRUCT
#define SVS_DCM_DISCONT_REASON_CODE_SEQUENCE	"PPS Discontinuation Reason Code Sequence"

#define SVN_DCM_ORIGINAL_PATIENT_NAME		(SVN_BASE_STUDY_DICOM + 42)
#define SVT_DCM_ORIGINAL_PATIENT_NAME		VFT_DCM_ORIGINAL_PATIENT_NAME
#define SVS_DCM_ORIGINAL_PATIENT_NAME		"Original Patient Name"
/*	患者名。
	PDET, dicom_wm_findが SVN_ST_STUDY に記述する。
	患者名にスペースまたはミドルネーム以降が設定されていた場合のみ記述される。
	*/

#define SVN_DCM_SJIS_FLAG	(SVN_BASE_STUDY_DICOM + 43)
#define SVT_DCM_SJIS_FLAG	VFT_INT32
#define SVS_DCM_SJIS_FLAG	"DICOM Information Charactor Code Flag"
/*	DICOM情報文字コードフラグ。
	PDET, dicom_wm_findが SVN_ST_STUDY に記述する。
	DICOM情報の日本語をSJISコードで保存する場合にのみ記述される。
	*/

#define SVN_DCM_N_EXAM_START_DATE_TIME	(SVN_BASE_STUDY_DICOM + 44)
#define SVT_DCM_N_EXAM_START_DATE_TIME	VFT_LOCAL_TIME
#define SVS_DCM_N_EXAM_START_DATE_TIME	"Date and Time at which the PPS started"
/*	設定者：DICOM
	使用者：なし
	*/

#define SVN_DCM_N_EXAM_END_DATE_TIME	(SVN_BASE_STUDY_DICOM + 45)
#define SVT_DCM_N_EXAM_END_DATE_TIME	VFT_LOCAL_TIME
#define SVS_DCM_N_EXAM_END_DATE_TIME	"Date and Time at which the PPS ended"
/*	設定者：DICOM
	使用者：なし
	*/

/*****************************************************************************
  PROTOCOL name space
*/

#define SVN_BASE_PROTOCOL_DICOM	SVN_MIN_PROTOCOL+SVN_OFF_DICOM

#define    SVN_DCM_SYNC_OBJECT        (SVN_BASE_PROTOCOL_DICOM + 1)
#define    SVS_DCM_SYNC_OBJECT        "Sync Object"
/*
     Syncオブジェクト

     Syncオブジェクト受信処理にて受信したTag(0021,xxxx)の値を保存する。
     Syncオブジェクト受信処理で作成されたPASで撮像を行い、撮像した画像の
     Syncオブジェクトを送信する際に参照される値。

     設定者：dicom_sync_receive
     使用者：StorageSyncSCU
*/

#define SVN_DCM_SYNC_REF_IMG            (SVN_BASE_PROTOCOL_DICOM + 2)
#define SVS_DCM_SYNC_REF_IMG            "Sync Reference Image"
/*
     参照画像(前回撮像時に位置決めを行った画像のハードコピー)

     Syncオブジェクト受信処理で作成されたPASでプラン撮影を行う時に
     前回の位置決めした時の画像を見ながらROIの調整を行う。

     設定者：dicom_sync_receive
     使用者：syncRefView
*/

#define SVN_DCM_SYNC_HOST               (SVN_BASE_PROTOCOL_DICOM + 3)
#define SVT_DCM_SYNC_HOST               VFT_DCM_HOST_LIST
#define SVS_DCM_SYNC_HOST               "The host who sent Sync Object"
/*
     Syncオブジェクト送信先ホスト名

     Syncオブジェクトを送信したホスト名を保存する。

     設定者：StorageSyncSCU
     使用者：StorageSyncSCU
*/

/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_DICOM	SVN_MIN_SUBPROTOCOL+SVN_OFF_DICOM

/*****************************************************************************
  SLICEGROUP/PRESAT name space
*/
    
#define SVN_BASE_SLICEGROUP_DICOM	SVN_MIN_SLICEGROUP+SVN_OFF_DICOM

/*****************************************************************************
  SLICE name space
*/

#define SVN_BASE_SLICE_DICOM		SVN_MIN_SLICE+SVN_OFF_DICOM

/*****************************************************************************
  IMAGE name space
*/

#define SVN_BASE_IMAGE_DICOM		SVN_MIN_IMAGE+SVN_OFF_DICOM

/*****************************************************************************
  PROCESSED name space
*/

#define SVN_BASE_PROCESSED_DICOM	SVN_MIN_PROCESSED+SVN_OFF_DICOM

/*****************************************************************************
  ECHO name space
*/

#define SVN_BASE_ECHO_DICOM	SVN_MIN_ECHO+SVN_OFF_DICOM



/*****************************************************************************
  POSITION name space
*/

#define SVN_BASE_POSITION_DICOM	SVN_MIN_POSITION+SVN_OFF_DICOM


/*****************************************************************************
  VISUALGROUP name space
*/

#define SVN_BASE_VISUALGROUP_DICOM	SVN_MIN_VISUALGROUP+SVN_OFF_DICOM



#endif /* tami_vfStudy_vfs_dicom_H__ */
