/***********************************************************************
 *
 *  (c) Copyright 2001 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 ***********************************************************************/
/* Revision History:
 * May.17.01	古舘 直幸	Created
 * End of History */



#ifndef tami_vfStudy_vfs_mrs_H__
#define tami_vfStudy_vfs_mrs_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */


/*****************************************************************************
  INDEPENDENT name space
*/

#define SVN_BASE_INDEPENDENT_MRS	SVN_MIN_INDEPENDENT+SVN_OFF_MRS



/*****************************************************************************
  STUDY name space
*/

#define SVN_BASE_STUDY_MRS	SVN_MIN_STUDY+SVN_OFF_MRS


/*****************************************************************************
  PROTOCOL name space
*/

#define SVN_BASE_PROTOCOL_MRS	SVN_MIN_PROTOCOL+SVN_OFF_MRS

#define SVN_MRS_VERSION	(SVN_BASE_PROTOCOL_MRS + 0)
#define SVT_MRS_VERSION	VFT_STRING
#define SVS_MRS_VERSION	"MRS system version"
/*	ＭＲＳ後処理バージョン
*/

#define SVN_MRS_MODEL	(SVN_BASE_PROTOCOL_MRS + 1)
#define SVT_MRS_MODEL	VFT_STRING
#define SVS_MRS_MODEL	"MRS data model name"
/*	ＭＲＳデータ構造名称
*/

#define SVN_MRS_NUCLEUS	(SVN_BASE_PROTOCOL_MRS + 2)
#define SVT_MRS_NUCLEUS	VFT_STRING
#define SVS_MRS_NUCLEUS	"MRS nucleus"
/*	ＭＲＳ核種
*/

#define SVN_MRS_REF_MATRIX	(SVN_BASE_PROTOCOL_MRS + 3)
#define SVT_MRS_REF_MATRIX	VFT_INT32
#define SVS_MRS_REF_MATRIX	"MRS Reference Matrix"
/*
	MRSのREF収集マトリクス(x, y, z)
*/

#define SVN_MRS_WSAT_FLAG	(SVN_BASE_PROTOCOL_MRS + 4)
#define SVT_MRS_WSAT_FLAG	VFT_INT32
#define SVS_MRS_WSAT_FLAG	"MRS Water Sat Flag"
/*
	MRSの水抑制収集フラグ
*/

#define SVN_MRS_WSAT_SWITCH	(SVN_BASE_PROTOCOL_MRS + 5)
#define SVT_MRS_WSAT_SWITCH	VFT_INT32
#define SVS_MRS_WSAT_SWITCH	"MRS Water Sat Switch"
/*
	MRSの水抑制収集フラグスイッチ
	プレスキャンでは０
	本スキャンでは１
*/

#define SVN_MRS_REF_MODE	(SVN_BASE_PROTOCOL_MRS + 6)
#define SVT_MRS_REF_MODE	VFT_INT32
#define SVS_MRS_REF_MODE	"MRS Ref Scan Mode"
/* 
    MRS Ref scan mode.
    VFC_MRS_REF_NONE    0    No Ref scan
    VFC_MRS_REF_NORMAL  1    Normal Ref scan
    VFC_MRS_REF_FAST    2    Fast Ref scan
*/

#define SVN_MRS_CHGAIN		(SVN_BASE_PROTOCOL_MRS + 7)
#define SVT_MRS_CHGAIN		VFT_FLT32
#define SVS_MRS_CHGAIN		"MRS Channel Gain Value"
/* 
    MRS Channel Gain Value.
    Channel数分配列でセット
*/

#define SVN_MRS_CHPHASE		(SVN_BASE_PROTOCOL_MRS + 8)
#define SVT_MRS_CHPHASE		VFT_FLT32
#define SVS_MRS_CHPHASE		"MRS Channel Phase Value"
/* 
    MRS Channel Phase Value.
    Channel数分配列でセット
*/

#define SVN_MRS_REF_CHGAIN	(SVN_BASE_PROTOCOL_MRS + 9)
#define SVT_MRS_REF_CHGAIN	VFT_FLT32
#define SVS_MRS_REF_CHGAIN	"MRS Ref Channel Gain Value"
/* 
    MRS Ref Channel Gain Value.
    Channel数分配列でセット
*/

#define SVN_MRS_REF_CHPHASE	(SVN_BASE_PROTOCOL_MRS + 10)
#define SVT_MRS_REF_CHPHASE	VFT_FLT32
#define SVS_MRS_REF_CHPHASE	"MRS Ref Channel Phase Value"
/* 
    MRS Ref Channel Phase Value.
    Channel数分配列でセット
*/

#define SVN_MRS_F0_GEN_DATA (SVN_BASE_PROTOCOL_MRS + 11)
#define SVS_MRS_F0_GEN_DATA "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 1 Data.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA1 (SVN_BASE_PROTOCOL_MRS + 12)
#define SVS_MRS_F0_GEN_DATA1 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 2 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA2 (SVN_BASE_PROTOCOL_MRS + 13)
#define SVS_MRS_F0_GEN_DATA2 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 3 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA3 (SVN_BASE_PROTOCOL_MRS + 14)
#define SVS_MRS_F0_GEN_DATA3 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 4 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA4 (SVN_BASE_PROTOCOL_MRS + 15)
#define SVS_MRS_F0_GEN_DATA4 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 5 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA5 (SVN_BASE_PROTOCOL_MRS + 16)
#define SVS_MRS_F0_GEN_DATA5 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 6 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA6 (SVN_BASE_PROTOCOL_MRS + 17)
#define SVS_MRS_F0_GEN_DATA6 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 7 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA7 (SVN_BASE_PROTOCOL_MRS + 18)
#define SVS_MRS_F0_GEN_DATA7 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 8 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA8 (SVN_BASE_PROTOCOL_MRS + 19)
#define SVS_MRS_F0_GEN_DATA8 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 9 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA9 (SVN_BASE_PROTOCOL_MRS + 20)
#define SVS_MRS_F0_GEN_DATA9 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 10 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA10 (SVN_BASE_PROTOCOL_MRS + 21)
#define SVS_MRS_F0_GEN_DATA10 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 11 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA11 (SVN_BASE_PROTOCOL_MRS + 22)
#define SVS_MRS_F0_GEN_DATA11 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 12 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA12 (SVN_BASE_PROTOCOL_MRS + 23)
#define SVS_MRS_F0_GEN_DATA12 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 13 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA13 (SVN_BASE_PROTOCOL_MRS + 24)
#define SVS_MRS_F0_GEN_DATA13 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel14 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA14 (SVN_BASE_PROTOCOL_MRS + 25)
#define SVS_MRS_F0_GEN_DATA14 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 15 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA15 (SVN_BASE_PROTOCOL_MRS + 26)
#define SVS_MRS_F0_GEN_DATA15 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 16 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA16 (SVN_BASE_PROTOCOL_MRS + 27)
#define SVS_MRS_F0_GEN_DATA16 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 17 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA17 (SVN_BASE_PROTOCOL_MRS + 28)
#define SVS_MRS_F0_GEN_DATA17 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 18 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA18 (SVN_BASE_PROTOCOL_MRS + 29)
#define SVS_MRS_F0_GEN_DATA18 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 19 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA19 (SVN_BASE_PROTOCOL_MRS + 30)
#define SVS_MRS_F0_GEN_DATA19 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 20 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA20 (SVN_BASE_PROTOCOL_MRS + 31)
#define SVS_MRS_F0_GEN_DATA20 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 21 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA21 (SVN_BASE_PROTOCOL_MRS + 32)
#define SVS_MRS_F0_GEN_DATA21 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 22 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA22 (SVN_BASE_PROTOCOL_MRS + 33)
#define SVS_MRS_F0_GEN_DATA22 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 23 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA23 (SVN_BASE_PROTOCOL_MRS + 34)
#define SVS_MRS_F0_GEN_DATA23 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 24 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA24 (SVN_BASE_PROTOCOL_MRS + 35)
#define SVS_MRS_F0_GEN_DATA24 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 25 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA25 (SVN_BASE_PROTOCOL_MRS + 36)
#define SVS_MRS_F0_GEN_DATA25 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 26 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA26 (SVN_BASE_PROTOCOL_MRS + 37)
#define SVS_MRS_F0_GEN_DATA26 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 27 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA27 (SVN_BASE_PROTOCOL_MRS + 38)
#define SVS_MRS_F0_GEN_DATA27 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 28 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA28 (SVN_BASE_PROTOCOL_MRS + 39)
#define SVS_MRS_F0_GEN_DATA28 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 29 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA29 (SVN_BASE_PROTOCOL_MRS + 40)
#define SVS_MRS_F0_GEN_DATA29 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 30 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA30 (SVN_BASE_PROTOCOL_MRS + 41)
#define SVS_MRS_F0_GEN_DATA30 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 31 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA31 (SVN_BASE_PROTOCOL_MRS + 42)
#define SVS_MRS_F0_GEN_DATA31 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 32 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA32 (SVN_BASE_PROTOCOL_MRS + 43)
#define SVS_MRS_F0_GEN_DATA32 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 33 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA33 (SVN_BASE_PROTOCOL_MRS + 44)
#define SVS_MRS_F0_GEN_DATA33 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 34 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA34 (SVN_BASE_PROTOCOL_MRS + 45)
#define SVS_MRS_F0_GEN_DATA34 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 35 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA35 (SVN_BASE_PROTOCOL_MRS + 46)
#define SVS_MRS_F0_GEN_DATA35 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 36 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA36 (SVN_BASE_PROTOCOL_MRS + 47)
#define SVS_MRS_F0_GEN_DATA36 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 37 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA37 (SVN_BASE_PROTOCOL_MRS + 48)
#define SVS_MRS_F0_GEN_DATA37 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 38 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA38 (SVN_BASE_PROTOCOL_MRS + 49)
#define SVS_MRS_F0_GEN_DATA38 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 39 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA39 (SVN_BASE_PROTOCOL_MRS + 50)
#define SVS_MRS_F0_GEN_DATA39 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 40 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA40 (SVN_BASE_PROTOCOL_MRS + 51)
#define SVS_MRS_F0_GEN_DATA40 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 41 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA41 (SVN_BASE_PROTOCOL_MRS + 52)
#define SVS_MRS_F0_GEN_DATA41 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 42 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA42 (SVN_BASE_PROTOCOL_MRS + 53)
#define SVS_MRS_F0_GEN_DATA42 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 43 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA43 (SVN_BASE_PROTOCOL_MRS + 54)
#define SVS_MRS_F0_GEN_DATA43 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 44 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA44 (SVN_BASE_PROTOCOL_MRS + 55)
#define SVS_MRS_F0_GEN_DATA44 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 45 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA45 (SVN_BASE_PROTOCOL_MRS + 56)
#define SVS_MRS_F0_GEN_DATA45 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 46 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA46 (SVN_BASE_PROTOCOL_MRS + 57)
#define SVS_MRS_F0_GEN_DATA46 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 47 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA47 (SVN_BASE_PROTOCOL_MRS + 58)
#define SVS_MRS_F0_GEN_DATA47 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 48 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA48 (SVN_BASE_PROTOCOL_MRS + 59)
#define SVS_MRS_F0_GEN_DATA48 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 49 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA49 (SVN_BASE_PROTOCOL_MRS + 60)
#define SVS_MRS_F0_GEN_DATA49 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 50 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA50 (SVN_BASE_PROTOCOL_MRS + 61)
#define SVS_MRS_F0_GEN_DATA50 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 51 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA51 (SVN_BASE_PROTOCOL_MRS + 62)
#define SVS_MRS_F0_GEN_DATA51 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 52 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA52 (SVN_BASE_PROTOCOL_MRS + 63)
#define SVS_MRS_F0_GEN_DATA52 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 53 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA53 (SVN_BASE_PROTOCOL_MRS + 64)
#define SVS_MRS_F0_GEN_DATA53 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 54 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA54 (SVN_BASE_PROTOCOL_MRS + 65)
#define SVS_MRS_F0_GEN_DATA54 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 55 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA55 (SVN_BASE_PROTOCOL_MRS + 66)
#define SVS_MRS_F0_GEN_DATA55 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 56 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA56 (SVN_BASE_PROTOCOL_MRS + 67)
#define SVS_MRS_F0_GEN_DATA56 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 57 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA57 (SVN_BASE_PROTOCOL_MRS + 68)
#define SVS_MRS_F0_GEN_DATA57 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 58 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA58 (SVN_BASE_PROTOCOL_MRS + 69)
#define SVS_MRS_F0_GEN_DATA58 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 59 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA59 (SVN_BASE_PROTOCOL_MRS + 70)
#define SVS_MRS_F0_GEN_DATA59 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 60 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA60 (SVN_BASE_PROTOCOL_MRS + 71)
#define SVS_MRS_F0_GEN_DATA60 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 61 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA61 (SVN_BASE_PROTOCOL_MRS + 72)
#define SVS_MRS_F0_GEN_DATA61 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 62 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA62 (SVN_BASE_PROTOCOL_MRS + 73)
#define SVS_MRS_F0_GEN_DATA62 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 63 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA63 (SVN_BASE_PROTOCOL_MRS + 74)
#define SVS_MRS_F0_GEN_DATA63 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 64 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA64 (SVN_BASE_PROTOCOL_MRS + 75)
#define SVS_MRS_F0_GEN_DATA64 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 65 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA65 (SVN_BASE_PROTOCOL_MRS + 76)
#define SVS_MRS_F0_GEN_DATA65 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 66 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA66 (SVN_BASE_PROTOCOL_MRS + 77)
#define SVS_MRS_F0_GEN_DATA66 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 67 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA67 (SVN_BASE_PROTOCOL_MRS + 78)
#define SVS_MRS_F0_GEN_DATA67 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 68 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA68 (SVN_BASE_PROTOCOL_MRS + 79)
#define SVS_MRS_F0_GEN_DATA68 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 69 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA69 (SVN_BASE_PROTOCOL_MRS + 80)
#define SVS_MRS_F0_GEN_DATA69 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 70 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA70 (SVN_BASE_PROTOCOL_MRS + 81)
#define SVS_MRS_F0_GEN_DATA70 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 71 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA71 (SVN_BASE_PROTOCOL_MRS + 82)
#define SVS_MRS_F0_GEN_DATA71 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 72 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA72 (SVN_BASE_PROTOCOL_MRS + 83)
#define SVS_MRS_F0_GEN_DATA72 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 73 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA73 (SVN_BASE_PROTOCOL_MRS + 84)
#define SVS_MRS_F0_GEN_DATA73 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 74 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA74 (SVN_BASE_PROTOCOL_MRS + 85)
#define SVS_MRS_F0_GEN_DATA74 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 75 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA75 (SVN_BASE_PROTOCOL_MRS + 86)
#define SVS_MRS_F0_GEN_DATA75 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 76 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA76 (SVN_BASE_PROTOCOL_MRS + 87)
#define SVS_MRS_F0_GEN_DATA76 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 77 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA77 (SVN_BASE_PROTOCOL_MRS + 88)
#define SVS_MRS_F0_GEN_DATA77 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 78 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA78 (SVN_BASE_PROTOCOL_MRS + 89)
#define SVS_MRS_F0_GEN_DATA78 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 79 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA79 (SVN_BASE_PROTOCOL_MRS + 90)
#define SVS_MRS_F0_GEN_DATA79 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 80 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA80 (SVN_BASE_PROTOCOL_MRS + 91)
#define SVS_MRS_F0_GEN_DATA80 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 81 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA81 (SVN_BASE_PROTOCOL_MRS + 92)
#define SVS_MRS_F0_GEN_DATA81 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 82 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA82 (SVN_BASE_PROTOCOL_MRS + 93)
#define SVS_MRS_F0_GEN_DATA82 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 83 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA83 (SVN_BASE_PROTOCOL_MRS + 94)
#define SVS_MRS_F0_GEN_DATA83 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 84 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA84 (SVN_BASE_PROTOCOL_MRS + 95)
#define SVS_MRS_F0_GEN_DATA84 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 85 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA85 (SVN_BASE_PROTOCOL_MRS + 96)
#define SVS_MRS_F0_GEN_DATA85 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 86 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA86 (SVN_BASE_PROTOCOL_MRS + 97)
#define SVS_MRS_F0_GEN_DATA86 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 87 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA87 (SVN_BASE_PROTOCOL_MRS + 98)
#define SVS_MRS_F0_GEN_DATA87 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 88 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA88 (SVN_BASE_PROTOCOL_MRS + 99)
#define SVS_MRS_F0_GEN_DATA88 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 89 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA89 (SVN_BASE_PROTOCOL_MRS + 100)
#define SVS_MRS_F0_GEN_DATA89 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 90 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA90 (SVN_BASE_PROTOCOL_MRS + 101)
#define SVS_MRS_F0_GEN_DATA90 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 91 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA91 (SVN_BASE_PROTOCOL_MRS + 102)
#define SVS_MRS_F0_GEN_DATA91 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 92 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA92 (SVN_BASE_PROTOCOL_MRS + 103)
#define SVS_MRS_F0_GEN_DATA92 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 93 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA93 (SVN_BASE_PROTOCOL_MRS + 104)
#define SVS_MRS_F0_GEN_DATA93 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 94 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA94 (SVN_BASE_PROTOCOL_MRS + 105)
#define SVS_MRS_F0_GEN_DATA94 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 95 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA95 (SVN_BASE_PROTOCOL_MRS + 106)
#define SVS_MRS_F0_GEN_DATA95 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 96 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA96 (SVN_BASE_PROTOCOL_MRS + 107)
#define SVS_MRS_F0_GEN_DATA96 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 97 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA97 (SVN_BASE_PROTOCOL_MRS + 108)
#define SVS_MRS_F0_GEN_DATA97 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 98 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA98 (SVN_BASE_PROTOCOL_MRS + 109)
#define SVS_MRS_F0_GEN_DATA98 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 99 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA99 (SVN_BASE_PROTOCOL_MRS + 110)
#define SVS_MRS_F0_GEN_DATA99 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 100 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA100 (SVN_BASE_PROTOCOL_MRS + 111)
#define SVS_MRS_F0_GEN_DATA100 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 101 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA101 (SVN_BASE_PROTOCOL_MRS + 112)
#define SVS_MRS_F0_GEN_DATA101 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 102 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA102 (SVN_BASE_PROTOCOL_MRS + 113)
#define SVS_MRS_F0_GEN_DATA102 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 103 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA103 (SVN_BASE_PROTOCOL_MRS + 114)
#define SVS_MRS_F0_GEN_DATA103 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 104 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA104 (SVN_BASE_PROTOCOL_MRS + 115)
#define SVS_MRS_F0_GEN_DATA104 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 105 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA105 (SVN_BASE_PROTOCOL_MRS + 116)
#define SVS_MRS_F0_GEN_DATA105 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 106 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA106 (SVN_BASE_PROTOCOL_MRS + 117)
#define SVS_MRS_F0_GEN_DATA106 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 107 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA107 (SVN_BASE_PROTOCOL_MRS + 118)
#define SVS_MRS_F0_GEN_DATA107 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 108 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA108 (SVN_BASE_PROTOCOL_MRS + 119)
#define SVS_MRS_F0_GEN_DATA108 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 109 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA109 (SVN_BASE_PROTOCOL_MRS + 120)
#define SVS_MRS_F0_GEN_DATA109 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 110 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA110 (SVN_BASE_PROTOCOL_MRS + 121)
#define SVS_MRS_F0_GEN_DATA110 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 111 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA111 (SVN_BASE_PROTOCOL_MRS + 122)
#define SVS_MRS_F0_GEN_DATA111 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 112 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA112 (SVN_BASE_PROTOCOL_MRS + 123)
#define SVS_MRS_F0_GEN_DATA112 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 113 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA113 (SVN_BASE_PROTOCOL_MRS + 124)
#define SVS_MRS_F0_GEN_DATA113 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 114 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA114 (SVN_BASE_PROTOCOL_MRS + 125)
#define SVS_MRS_F0_GEN_DATA114 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 115 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA115 (SVN_BASE_PROTOCOL_MRS + 126)
#define SVS_MRS_F0_GEN_DATA115 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 116 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA116 (SVN_BASE_PROTOCOL_MRS + 127)
#define SVS_MRS_F0_GEN_DATA116 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 117 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA117 (SVN_BASE_PROTOCOL_MRS + 128)
#define SVS_MRS_F0_GEN_DATA117 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 118 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA118 (SVN_BASE_PROTOCOL_MRS + 129)
#define SVS_MRS_F0_GEN_DATA118 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 119 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA119 (SVN_BASE_PROTOCOL_MRS + 130)
#define SVS_MRS_F0_GEN_DATA119 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 120 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA120 (SVN_BASE_PROTOCOL_MRS + 131)
#define SVS_MRS_F0_GEN_DATA120 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 121 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA121 (SVN_BASE_PROTOCOL_MRS + 132)
#define SVS_MRS_F0_GEN_DATA121 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 122 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA122 (SVN_BASE_PROTOCOL_MRS + 133)
#define SVS_MRS_F0_GEN_DATA122 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 123 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA123 (SVN_BASE_PROTOCOL_MRS + 134)
#define SVS_MRS_F0_GEN_DATA123 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 124 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA124 (SVN_BASE_PROTOCOL_MRS + 135)
#define SVS_MRS_F0_GEN_DATA124 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 125 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA125 (SVN_BASE_PROTOCOL_MRS + 136)
#define SVS_MRS_F0_GEN_DATA125 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 126 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA126 (SVN_BASE_PROTOCOL_MRS + 137)
#define SVS_MRS_F0_GEN_DATA126 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 127 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA127 (SVN_BASE_PROTOCOL_MRS + 138)
#define SVS_MRS_F0_GEN_DATA127 "MRS F0 Raw Data"
/*
    MRS F0 Raw Data.
    Channel 128 Data in PAC scan.
    F0プレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_F0_IMAGE_SIZE (SVN_BASE_PROTOCOL_MRS + 139)
#define SVT_MRS_F0_IMAGE_SIZE VFT_INT32
#define SVS_MRS_F0_IMAGE_SIZE "MRS F0 Prescan Raw Data Size"
/*
    MRS F0 Raw Data Size.
    F0プレスキャンのSVN_GEN_DATAのデータサイズを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_PRESCAN_FSY (SVN_BASE_PROTOCOL_MRS + 140)
#define SVT_MRS_F0_PRESCAN_FSY VFT_FLT64
#define SVS_MRS_F0_PRESCAN_FSY "MRS F0 Prescan Center Frequency value (Hz)"
/*
    MRS F0 Prescan Center Frequency value.
    F0プレスキャンの中心周波数を設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_ACQ_MATRIX (SVN_BASE_PROTOCOL_MRS + 141)
#define SVT_MRS_F0_ACQ_MATRIX VFT_FLTVEC2
#define SVS_MRS_F0_ACQ_MATRIX "MRS F0 Prescan Acquire Matrix Size"
/*
    MRS F0 Acquire Matrix Size.
    F0プレスキャンのacquire matrixを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_RGN (SVN_BASE_PROTOCOL_MRS + 142)
#define SVT_MRS_F0_RGN VFT_FLT32
#define SVS_MRS_F0_RGN "MRS F0 Prescan Receiver Gain value (dB)"
/*
    MRS F0 Prescan Receiver Gain value.
    F0プレスキャンの受信ゲインを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA_TYPE (SVN_BASE_PROTOCOL_MRS + 143)
#define SVT_MRS_F0_GEN_DATA_TYPE VFT_INT32
#define SVS_MRS_F0_GEN_DATA_TYPE "MRS F0 Prescan Image data type"
/*
    MRS F0 Prescan Image data type.
    F0プレスキャンSVN_GEN_DATAデータのタイプを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_GEN_DATA_FORM (SVN_BASE_PROTOCOL_MRS + 144)
#define SVT_MRS_F0_GEN_DATA_FORM VFT_INT32
#define SVS_MRS_F0_GEN_DATA_FORM "MRS F0 Prescan Image data form"
/*
    MRS F0 Prescan Image data form.
    F0プレスキャンSVN_GEN_DATAデータのフォームを以下の値で設定する。
     VFC_FORM_MAGNITUDE 0
     VFC_FORM_PHASE     1
     VFC_FORM_REAL_ONLY 2
     VFC_FORM_IMGN_ONLY 3
     VFC_FORM_COMPLEX   4
     VFC_FORM_POLAR     5
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_ADC_PITCH (SVN_BASE_PROTOCOL_MRS + 145)
#define SVT_MRS_F0_ADC_PITCH VFT_FLT32
#define SVS_MRS_F0_ADC_PITCH "MRS F0 Prescan ADC Pitch"
/*
    MRS F0 Prescan ADC Pitch.
    F0プレスキャンのADCpitchを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_F0_MAX_MAG (SVN_BASE_PROTOCOL_MRS + 146)
#define SVT_MRS_F0_MAX_MAG VFT_FLT32
#define SVS_MRS_F0_MAX_MAG "MRS F0 Prescan Max Magnitude"
/*
    MRS F0 Prescan Max Magnitude.
    F0プレスキャンデータの中心周波数での信号強度を設定する。
    設定者：acqMan
    使用者：acqMan
*/

#define SVN_MRS_REF_GEN_DATA (SVN_BASE_PROTOCOL_MRS + 147)
#define SVS_MRS_REF_GEN_DATA "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 1 Data.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA1 (SVN_BASE_PROTOCOL_MRS + 148)
#define SVS_MRS_REF_GEN_DATA1 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 2 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA2 (SVN_BASE_PROTOCOL_MRS + 149)
#define SVS_MRS_REF_GEN_DATA2 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 3 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA3 (SVN_BASE_PROTOCOL_MRS + 150)
#define SVS_MRS_REF_GEN_DATA3 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 4 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA4 (SVN_BASE_PROTOCOL_MRS + 151)
#define SVS_MRS_REF_GEN_DATA4 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 5 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA5 (SVN_BASE_PROTOCOL_MRS + 152)
#define SVS_MRS_REF_GEN_DATA5 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 6 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA6 (SVN_BASE_PROTOCOL_MRS + 153)
#define SVS_MRS_REF_GEN_DATA6 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 7 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA7 (SVN_BASE_PROTOCOL_MRS + 154)
#define SVS_MRS_REF_GEN_DATA7 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 8 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA8 (SVN_BASE_PROTOCOL_MRS + 155)
#define SVS_MRS_REF_GEN_DATA8 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 9 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA9 (SVN_BASE_PROTOCOL_MRS + 156)
#define SVS_MRS_REF_GEN_DATA9 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 10 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA10 (SVN_BASE_PROTOCOL_MRS + 157)
#define SVS_MRS_REF_GEN_DATA10 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 11 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA11 (SVN_BASE_PROTOCOL_MRS + 158)
#define SVS_MRS_REF_GEN_DATA11 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 12 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA12 (SVN_BASE_PROTOCOL_MRS + 159)
#define SVS_MRS_REF_GEN_DATA12 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 13 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA13 (SVN_BASE_PROTOCOL_MRS + 160)
#define SVS_MRS_REF_GEN_DATA13 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel14 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA14 (SVN_BASE_PROTOCOL_MRS + 161)
#define SVS_MRS_REF_GEN_DATA14 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 15 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA15 (SVN_BASE_PROTOCOL_MRS + 162)
#define SVS_MRS_REF_GEN_DATA15 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 16 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA16 (SVN_BASE_PROTOCOL_MRS + 163)
#define SVS_MRS_REF_GEN_DATA16 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 17 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA17 (SVN_BASE_PROTOCOL_MRS + 164)
#define SVS_MRS_REF_GEN_DATA17 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 18 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA18 (SVN_BASE_PROTOCOL_MRS + 165)
#define SVS_MRS_REF_GEN_DATA18 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 19 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA19 (SVN_BASE_PROTOCOL_MRS + 166)
#define SVS_MRS_REF_GEN_DATA19 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 20 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA20 (SVN_BASE_PROTOCOL_MRS + 167)
#define SVS_MRS_REF_GEN_DATA20 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 21 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA21 (SVN_BASE_PROTOCOL_MRS + 168)
#define SVS_MRS_REF_GEN_DATA21 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 22 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA22 (SVN_BASE_PROTOCOL_MRS + 169)
#define SVS_MRS_REF_GEN_DATA22 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 23 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA23 (SVN_BASE_PROTOCOL_MRS + 170)
#define SVS_MRS_REF_GEN_DATA23 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 24 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA24 (SVN_BASE_PROTOCOL_MRS + 171)
#define SVS_MRS_REF_GEN_DATA24 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 25 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA25 (SVN_BASE_PROTOCOL_MRS + 172)
#define SVS_MRS_REF_GEN_DATA25 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 26 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA26 (SVN_BASE_PROTOCOL_MRS + 173)
#define SVS_MRS_REF_GEN_DATA26 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 27 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA27 (SVN_BASE_PROTOCOL_MRS + 174)
#define SVS_MRS_REF_GEN_DATA27 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 28 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA28 (SVN_BASE_PROTOCOL_MRS + 175)
#define SVS_MRS_REF_GEN_DATA28 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 29 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA29 (SVN_BASE_PROTOCOL_MRS + 176)
#define SVS_MRS_REF_GEN_DATA29 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 30 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA30 (SVN_BASE_PROTOCOL_MRS + 177)
#define SVS_MRS_REF_GEN_DATA30 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 31 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA31 (SVN_BASE_PROTOCOL_MRS + 178)
#define SVS_MRS_REF_GEN_DATA31 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 32 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA32 (SVN_BASE_PROTOCOL_MRS + 179)
#define SVS_MRS_REF_GEN_DATA32 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 33 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA33 (SVN_BASE_PROTOCOL_MRS + 180)
#define SVS_MRS_REF_GEN_DATA33 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 34 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA34 (SVN_BASE_PROTOCOL_MRS + 181)
#define SVS_MRS_REF_GEN_DATA34 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 35 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA35 (SVN_BASE_PROTOCOL_MRS + 182)
#define SVS_MRS_REF_GEN_DATA35 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 36 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA36 (SVN_BASE_PROTOCOL_MRS + 183)
#define SVS_MRS_REF_GEN_DATA36 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 37 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA37 (SVN_BASE_PROTOCOL_MRS + 184)
#define SVS_MRS_REF_GEN_DATA37 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 38 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA38 (SVN_BASE_PROTOCOL_MRS + 185)
#define SVS_MRS_REF_GEN_DATA38 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 39 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA39 (SVN_BASE_PROTOCOL_MRS + 186)
#define SVS_MRS_REF_GEN_DATA39 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 40 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA40 (SVN_BASE_PROTOCOL_MRS + 187)
#define SVS_MRS_REF_GEN_DATA40 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 41 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA41 (SVN_BASE_PROTOCOL_MRS + 188)
#define SVS_MRS_REF_GEN_DATA41 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 42 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA42 (SVN_BASE_PROTOCOL_MRS + 189)
#define SVS_MRS_REF_GEN_DATA42 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 43 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA43 (SVN_BASE_PROTOCOL_MRS + 190)
#define SVS_MRS_REF_GEN_DATA43 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 44 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA44 (SVN_BASE_PROTOCOL_MRS + 191)
#define SVS_MRS_REF_GEN_DATA44 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 45 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA45 (SVN_BASE_PROTOCOL_MRS + 192)
#define SVS_MRS_REF_GEN_DATA45 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 46 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA46 (SVN_BASE_PROTOCOL_MRS + 193)
#define SVS_MRS_REF_GEN_DATA46 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 47 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA47 (SVN_BASE_PROTOCOL_MRS + 194)
#define SVS_MRS_REF_GEN_DATA47 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 48 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA48 (SVN_BASE_PROTOCOL_MRS + 195)
#define SVS_MRS_REF_GEN_DATA48 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 49 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA49 (SVN_BASE_PROTOCOL_MRS + 196)
#define SVS_MRS_REF_GEN_DATA49 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 50 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA50 (SVN_BASE_PROTOCOL_MRS + 197)
#define SVS_MRS_REF_GEN_DATA50 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 51 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA51 (SVN_BASE_PROTOCOL_MRS + 198)
#define SVS_MRS_REF_GEN_DATA51 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 52 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA52 (SVN_BASE_PROTOCOL_MRS + 199)
#define SVS_MRS_REF_GEN_DATA52 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 53 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA53 (SVN_BASE_PROTOCOL_MRS + 200)
#define SVS_MRS_REF_GEN_DATA53 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 54 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA54 (SVN_BASE_PROTOCOL_MRS + 201)
#define SVS_MRS_REF_GEN_DATA54 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 55 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA55 (SVN_BASE_PROTOCOL_MRS + 202)
#define SVS_MRS_REF_GEN_DATA55 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 56 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA56 (SVN_BASE_PROTOCOL_MRS + 203)
#define SVS_MRS_REF_GEN_DATA56 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 57 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA57 (SVN_BASE_PROTOCOL_MRS + 204)
#define SVS_MRS_REF_GEN_DATA57 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 58 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA58 (SVN_BASE_PROTOCOL_MRS + 205)
#define SVS_MRS_REF_GEN_DATA58 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 59 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA59 (SVN_BASE_PROTOCOL_MRS + 206)
#define SVS_MRS_REF_GEN_DATA59 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 60 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA60 (SVN_BASE_PROTOCOL_MRS + 207)
#define SVS_MRS_REF_GEN_DATA60 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 61 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA61 (SVN_BASE_PROTOCOL_MRS + 208)
#define SVS_MRS_REF_GEN_DATA61 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 62 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA62 (SVN_BASE_PROTOCOL_MRS + 209)
#define SVS_MRS_REF_GEN_DATA62 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 63 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA63 (SVN_BASE_PROTOCOL_MRS + 210)
#define SVS_MRS_REF_GEN_DATA63 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 64 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA64 (SVN_BASE_PROTOCOL_MRS + 211)
#define SVS_MRS_REF_GEN_DATA64 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 65 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA65 (SVN_BASE_PROTOCOL_MRS + 212)
#define SVS_MRS_REF_GEN_DATA65 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 66 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA66 (SVN_BASE_PROTOCOL_MRS + 213)
#define SVS_MRS_REF_GEN_DATA66 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 67 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA67 (SVN_BASE_PROTOCOL_MRS + 214)
#define SVS_MRS_REF_GEN_DATA67 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 68 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA68 (SVN_BASE_PROTOCOL_MRS + 215)
#define SVS_MRS_REF_GEN_DATA68 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 69 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA69 (SVN_BASE_PROTOCOL_MRS + 216)
#define SVS_MRS_REF_GEN_DATA69 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 70 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA70 (SVN_BASE_PROTOCOL_MRS + 217)
#define SVS_MRS_REF_GEN_DATA70 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 71 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA71 (SVN_BASE_PROTOCOL_MRS + 218)
#define SVS_MRS_REF_GEN_DATA71 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 72 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA72 (SVN_BASE_PROTOCOL_MRS + 219)
#define SVS_MRS_REF_GEN_DATA72 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 73 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA73 (SVN_BASE_PROTOCOL_MRS + 220)
#define SVS_MRS_REF_GEN_DATA73 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 74 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA74 (SVN_BASE_PROTOCOL_MRS + 221)
#define SVS_MRS_REF_GEN_DATA74 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 75 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA75 (SVN_BASE_PROTOCOL_MRS + 222)
#define SVS_MRS_REF_GEN_DATA75 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 76 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA76 (SVN_BASE_PROTOCOL_MRS + 223)
#define SVS_MRS_REF_GEN_DATA76 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 77 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA77 (SVN_BASE_PROTOCOL_MRS + 224)
#define SVS_MRS_REF_GEN_DATA77 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 78 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA78 (SVN_BASE_PROTOCOL_MRS + 225)
#define SVS_MRS_REF_GEN_DATA78 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 79 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA79 (SVN_BASE_PROTOCOL_MRS + 226)
#define SVS_MRS_REF_GEN_DATA79 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 80 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA80 (SVN_BASE_PROTOCOL_MRS + 227)
#define SVS_MRS_REF_GEN_DATA80 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 81 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA81 (SVN_BASE_PROTOCOL_MRS + 228)
#define SVS_MRS_REF_GEN_DATA81 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 82 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA82 (SVN_BASE_PROTOCOL_MRS + 229)
#define SVS_MRS_REF_GEN_DATA82 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 83 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA83 (SVN_BASE_PROTOCOL_MRS + 230)
#define SVS_MRS_REF_GEN_DATA83 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 84 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA84 (SVN_BASE_PROTOCOL_MRS + 231)
#define SVS_MRS_REF_GEN_DATA84 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 85 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA85 (SVN_BASE_PROTOCOL_MRS + 232)
#define SVS_MRS_REF_GEN_DATA85 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 86 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA86 (SVN_BASE_PROTOCOL_MRS + 233)
#define SVS_MRS_REF_GEN_DATA86 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 87 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA87 (SVN_BASE_PROTOCOL_MRS + 234)
#define SVS_MRS_REF_GEN_DATA87 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 88 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA88 (SVN_BASE_PROTOCOL_MRS + 235)
#define SVS_MRS_REF_GEN_DATA88 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 89 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA89 (SVN_BASE_PROTOCOL_MRS + 236)
#define SVS_MRS_REF_GEN_DATA89 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 90 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA90 (SVN_BASE_PROTOCOL_MRS + 237)
#define SVS_MRS_REF_GEN_DATA90 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 91 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA91 (SVN_BASE_PROTOCOL_MRS + 238)
#define SVS_MRS_REF_GEN_DATA91 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 92 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA92 (SVN_BASE_PROTOCOL_MRS + 239)
#define SVS_MRS_REF_GEN_DATA92 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 93 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA93 (SVN_BASE_PROTOCOL_MRS + 240)
#define SVS_MRS_REF_GEN_DATA93 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 94 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA94 (SVN_BASE_PROTOCOL_MRS + 241)
#define SVS_MRS_REF_GEN_DATA94 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 95 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA95 (SVN_BASE_PROTOCOL_MRS + 242)
#define SVS_MRS_REF_GEN_DATA95 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 96 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA96 (SVN_BASE_PROTOCOL_MRS + 243)
#define SVS_MRS_REF_GEN_DATA96 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 97 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA97 (SVN_BASE_PROTOCOL_MRS + 244)
#define SVS_MRS_REF_GEN_DATA97 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 98 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA98 (SVN_BASE_PROTOCOL_MRS + 245)
#define SVS_MRS_REF_GEN_DATA98 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 99 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA99 (SVN_BASE_PROTOCOL_MRS + 246)
#define SVS_MRS_REF_GEN_DATA99 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 100 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA100 (SVN_BASE_PROTOCOL_MRS + 247)
#define SVS_MRS_REF_GEN_DATA100 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 101 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA101 (SVN_BASE_PROTOCOL_MRS + 248)
#define SVS_MRS_REF_GEN_DATA101 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 102 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA102 (SVN_BASE_PROTOCOL_MRS + 249)
#define SVS_MRS_REF_GEN_DATA102 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 103 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA103 (SVN_BASE_PROTOCOL_MRS + 250)
#define SVS_MRS_REF_GEN_DATA103 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 104 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA104 (SVN_BASE_PROTOCOL_MRS + 251)
#define SVS_MRS_REF_GEN_DATA104 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 105 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA105 (SVN_BASE_PROTOCOL_MRS + 252)
#define SVS_MRS_REF_GEN_DATA105 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 106 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA106 (SVN_BASE_PROTOCOL_MRS + 253)
#define SVS_MRS_REF_GEN_DATA106 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 107 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA107 (SVN_BASE_PROTOCOL_MRS + 254)
#define SVS_MRS_REF_GEN_DATA107 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 108 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA108 (SVN_BASE_PROTOCOL_MRS + 255)
#define SVS_MRS_REF_GEN_DATA108 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 109 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA109 (SVN_BASE_PROTOCOL_MRS + 256)
#define SVS_MRS_REF_GEN_DATA109 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 110 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA110 (SVN_BASE_PROTOCOL_MRS + 257)
#define SVS_MRS_REF_GEN_DATA110 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 111 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA111 (SVN_BASE_PROTOCOL_MRS + 258)
#define SVS_MRS_REF_GEN_DATA111 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 112 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA112 (SVN_BASE_PROTOCOL_MRS + 259)
#define SVS_MRS_REF_GEN_DATA112 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 113 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA113 (SVN_BASE_PROTOCOL_MRS + 260)
#define SVS_MRS_REF_GEN_DATA113 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 114 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA114 (SVN_BASE_PROTOCOL_MRS + 261)
#define SVS_MRS_REF_GEN_DATA114 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 115 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA115 (SVN_BASE_PROTOCOL_MRS + 262)
#define SVS_MRS_REF_GEN_DATA115 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 116 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA116 (SVN_BASE_PROTOCOL_MRS + 263)
#define SVS_MRS_REF_GEN_DATA116 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 117 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA117 (SVN_BASE_PROTOCOL_MRS + 264)
#define SVS_MRS_REF_GEN_DATA117 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 118 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA118 (SVN_BASE_PROTOCOL_MRS + 265)
#define SVS_MRS_REF_GEN_DATA118 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 119 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA119 (SVN_BASE_PROTOCOL_MRS + 266)
#define SVS_MRS_REF_GEN_DATA119 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 120 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA120 (SVN_BASE_PROTOCOL_MRS + 267)
#define SVS_MRS_REF_GEN_DATA120 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 121 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA121 (SVN_BASE_PROTOCOL_MRS + 268)
#define SVS_MRS_REF_GEN_DATA121 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 122 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA122 (SVN_BASE_PROTOCOL_MRS + 269)
#define SVS_MRS_REF_GEN_DATA122 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 123 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA123 (SVN_BASE_PROTOCOL_MRS + 270)
#define SVS_MRS_REF_GEN_DATA123 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 124 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA124 (SVN_BASE_PROTOCOL_MRS + 271)
#define SVS_MRS_REF_GEN_DATA124 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 125 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA125 (SVN_BASE_PROTOCOL_MRS + 272)
#define SVS_MRS_REF_GEN_DATA125 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 126 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA126 (SVN_BASE_PROTOCOL_MRS + 273)
#define SVS_MRS_REF_GEN_DATA126 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 127 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA127 (SVN_BASE_PROTOCOL_MRS + 274)
#define SVS_MRS_REF_GEN_DATA127 "MRS Ref Raw Data"
/*
    MRS Ref Raw Data.
    Channel 128 Data in PAC scan.
    RefプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_REF_IMAGE_SIZE (SVN_BASE_PROTOCOL_MRS + 275)
#define SVT_MRS_REF_IMAGE_SIZE VFT_INT32
#define SVS_MRS_REF_IMAGE_SIZE "MRS Ref Prescan Raw Data Size"
/*
    MRS Ref Raw Data Size.
    RefプレスキャンのSVN_GEN_DATAのデータサイズを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_PRESCAN_FSY (SVN_BASE_PROTOCOL_MRS + 276)
#define SVT_MRS_REF_PRESCAN_FSY VFT_FLT64
#define SVS_MRS_REF_PRESCAN_FSY "MRS Ref Prescan Center Frequency value (Hz)"
/*
    MRS Ref Prescan Center Frequency value.
    Refプレスキャンの中心周波数を設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_ACQ_MATRIX (SVN_BASE_PROTOCOL_MRS + 277)
#define SVT_MRS_REF_ACQ_MATRIX VFT_FLTVEC2
#define SVS_MRS_REF_ACQ_MATRIX "MRS Ref Prescan Acquire Matrix Size"
/*
    MRS Ref Acquire Matrix Size.
    Refプレスキャンのacquire matrixを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_RGN (SVN_BASE_PROTOCOL_MRS + 278)
#define SVT_MRS_REF_RGN VFT_FLT32
#define SVS_MRS_REF_RGN "MRS Ref Prescan Receiver Gain value (dB)"
/*
    MRS Ref Prescan Receiver Gain value.
    Refプレスキャンの受信ゲインを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_GEN_DATA_TYPE (SVN_BASE_PROTOCOL_MRS + 279)
#define SVT_MRS_REF_GEN_DATA_TYPE VFT_INT32
#define SVS_MRS_REF_GEN_DATA_TYPE "MRS Ref Prescan Image data type"
/*
    MRS Ref Prescan Image data type.
    RefプレスキャンSVN_GEN_DATAデータのタイプを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/


#define SVN_MRS_REF_GEN_DATA_FORM (SVN_BASE_PROTOCOL_MRS + 280)
#define SVT_MRS_REF_GEN_DATA_FORM VFT_INT32
#define SVS_MRS_REF_GEN_DATA_FORM "MRS Ref Prescan Image data form"
/*
    MRS Ref Prescan Image data form.
    RefプレスキャンSVN_GEN_DATAデータのフォームを以下の値で設定する。
     VFC_FORM_MAGNITUDE 0
     VFC_FORM_PHASE     1
     VFC_FORM_REAL_ONLY 2
     VFC_FORM_IMGN_ONLY 3
     VFC_FORM_COMPLEX   4
     VFC_FORM_POLAR     5
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_ADC_PITCH (SVN_BASE_PROTOCOL_MRS + 281)
#define SVT_MRS_REF_ADC_PITCH VFT_FLT32
#define SVS_MRS_REF_ADC_PITCH "MRS Ref Prescan ADC Pitch"
/*
    MRS Ref Prescan ADC Pitch.
    RefプレスキャンのADCpitchを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_NUM_POINT (SVN_BASE_PROTOCOL_MRS + 282)
#define SVT_MRS_REF_NUM_POINT VFT_INT32
#define SVS_MRS_REF_NUM_POINT "MRS Ref Prescan Number of Points"
/*
    MRS Ref Prescan Number of Points.
    Refプレスキャンの計算用VoxelのVoxel数を設定する。
    SVN_MRS_REF_MATRIXの３要素を掛け合わせた数
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_REF_RAW_DIM (SVN_BASE_PROTOCOL_MRS + 283)
#define SVT_MRS_REF_RAW_DIM VFT_INT32
#define SVS_MRS_REF_RAW_DIM "MRS Ref Prescan Voxel Dimension"
/*
    MRS Ref Prescan Voxel Dimension.
    Refプレスキャンの計算用Voxelの次元数を設定する。
    SVN_MRS_REF_MATRIXの１より大きい次元数を設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA (SVN_BASE_PROTOCOL_MRS + 284)
#define SVS_MRS_WSAT_GEN_DATA "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 1 Data.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA1 (SVN_BASE_PROTOCOL_MRS + 285)
#define SVS_MRS_WSAT_GEN_DATA1 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 2 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA2 (SVN_BASE_PROTOCOL_MRS + 286)
#define SVS_MRS_WSAT_GEN_DATA2 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 3 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA3 (SVN_BASE_PROTOCOL_MRS + 287)
#define SVS_MRS_WSAT_GEN_DATA3 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 4 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA4 (SVN_BASE_PROTOCOL_MRS + 288)
#define SVS_MRS_WSAT_GEN_DATA4 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 5 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA5 (SVN_BASE_PROTOCOL_MRS + 289)
#define SVS_MRS_WSAT_GEN_DATA5 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 6 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA6 (SVN_BASE_PROTOCOL_MRS + 290)
#define SVS_MRS_WSAT_GEN_DATA6 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 7 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA7 (SVN_BASE_PROTOCOL_MRS + 291)
#define SVS_MRS_WSAT_GEN_DATA7 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 8 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA8 (SVN_BASE_PROTOCOL_MRS + 292)
#define SVS_MRS_WSAT_GEN_DATA8 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 9 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA9 (SVN_BASE_PROTOCOL_MRS + 293)
#define SVS_MRS_WSAT_GEN_DATA9 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 10 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA10 (SVN_BASE_PROTOCOL_MRS + 294)
#define SVS_MRS_WSAT_GEN_DATA10 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 11 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA11 (SVN_BASE_PROTOCOL_MRS + 295)
#define SVS_MRS_WSAT_GEN_DATA11 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 12 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA12 (SVN_BASE_PROTOCOL_MRS + 296)
#define SVS_MRS_WSAT_GEN_DATA12 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 13 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA13 (SVN_BASE_PROTOCOL_MRS + 297)
#define SVS_MRS_WSAT_GEN_DATA13 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel14 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA14 (SVN_BASE_PROTOCOL_MRS + 298)
#define SVS_MRS_WSAT_GEN_DATA14 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 15 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA15 (SVN_BASE_PROTOCOL_MRS + 299)
#define SVS_MRS_WSAT_GEN_DATA15 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 16 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA16 (SVN_BASE_PROTOCOL_MRS + 300)
#define SVS_MRS_WSAT_GEN_DATA16 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 17 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA17 (SVN_BASE_PROTOCOL_MRS + 301)
#define SVS_MRS_WSAT_GEN_DATA17 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 18 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA18 (SVN_BASE_PROTOCOL_MRS + 302)
#define SVS_MRS_WSAT_GEN_DATA18 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 19 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA19 (SVN_BASE_PROTOCOL_MRS + 303)
#define SVS_MRS_WSAT_GEN_DATA19 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 20 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA20 (SVN_BASE_PROTOCOL_MRS + 304)
#define SVS_MRS_WSAT_GEN_DATA20 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 21 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA21 (SVN_BASE_PROTOCOL_MRS + 305)
#define SVS_MRS_WSAT_GEN_DATA21 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 22 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA22 (SVN_BASE_PROTOCOL_MRS + 306)
#define SVS_MRS_WSAT_GEN_DATA22 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 23 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA23 (SVN_BASE_PROTOCOL_MRS + 307)
#define SVS_MRS_WSAT_GEN_DATA23 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 24 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA24 (SVN_BASE_PROTOCOL_MRS + 308)
#define SVS_MRS_WSAT_GEN_DATA24 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 25 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA25 (SVN_BASE_PROTOCOL_MRS + 309)
#define SVS_MRS_WSAT_GEN_DATA25 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 26 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA26 (SVN_BASE_PROTOCOL_MRS + 310)
#define SVS_MRS_WSAT_GEN_DATA26 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 27 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA27 (SVN_BASE_PROTOCOL_MRS + 311)
#define SVS_MRS_WSAT_GEN_DATA27 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 28 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA28 (SVN_BASE_PROTOCOL_MRS + 312)
#define SVS_MRS_WSAT_GEN_DATA28 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 29 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA29 (SVN_BASE_PROTOCOL_MRS + 313)
#define SVS_MRS_WSAT_GEN_DATA29 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 30 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA30 (SVN_BASE_PROTOCOL_MRS + 314)
#define SVS_MRS_WSAT_GEN_DATA30 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 31 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA31 (SVN_BASE_PROTOCOL_MRS + 315)
#define SVS_MRS_WSAT_GEN_DATA31 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 32 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA32 (SVN_BASE_PROTOCOL_MRS + 316)
#define SVS_MRS_WSAT_GEN_DATA32 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 33 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA33 (SVN_BASE_PROTOCOL_MRS + 317)
#define SVS_MRS_WSAT_GEN_DATA33 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 34 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA34 (SVN_BASE_PROTOCOL_MRS + 318)
#define SVS_MRS_WSAT_GEN_DATA34 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 35 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA35 (SVN_BASE_PROTOCOL_MRS + 319)
#define SVS_MRS_WSAT_GEN_DATA35 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 36 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA36 (SVN_BASE_PROTOCOL_MRS + 320)
#define SVS_MRS_WSAT_GEN_DATA36 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 37 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA37 (SVN_BASE_PROTOCOL_MRS + 321)
#define SVS_MRS_WSAT_GEN_DATA37 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 38 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA38 (SVN_BASE_PROTOCOL_MRS + 322)
#define SVS_MRS_WSAT_GEN_DATA38 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 39 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA39 (SVN_BASE_PROTOCOL_MRS + 323)
#define SVS_MRS_WSAT_GEN_DATA39 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 40 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA40 (SVN_BASE_PROTOCOL_MRS + 324)
#define SVS_MRS_WSAT_GEN_DATA40 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 41 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA41 (SVN_BASE_PROTOCOL_MRS + 325)
#define SVS_MRS_WSAT_GEN_DATA41 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 42 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA42 (SVN_BASE_PROTOCOL_MRS + 326)
#define SVS_MRS_WSAT_GEN_DATA42 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 43 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA43 (SVN_BASE_PROTOCOL_MRS + 327)
#define SVS_MRS_WSAT_GEN_DATA43 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 44 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA44 (SVN_BASE_PROTOCOL_MRS + 328)
#define SVS_MRS_WSAT_GEN_DATA44 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 45 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA45 (SVN_BASE_PROTOCOL_MRS + 329)
#define SVS_MRS_WSAT_GEN_DATA45 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 46 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA46 (SVN_BASE_PROTOCOL_MRS + 330)
#define SVS_MRS_WSAT_GEN_DATA46 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 47 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA47 (SVN_BASE_PROTOCOL_MRS + 331)
#define SVS_MRS_WSAT_GEN_DATA47 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 48 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA48 (SVN_BASE_PROTOCOL_MRS + 332)
#define SVS_MRS_WSAT_GEN_DATA48 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 49 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA49 (SVN_BASE_PROTOCOL_MRS + 333)
#define SVS_MRS_WSAT_GEN_DATA49 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 50 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA50 (SVN_BASE_PROTOCOL_MRS + 334)
#define SVS_MRS_WSAT_GEN_DATA50 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 51 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA51 (SVN_BASE_PROTOCOL_MRS + 335)
#define SVS_MRS_WSAT_GEN_DATA51 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 52 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA52 (SVN_BASE_PROTOCOL_MRS + 336)
#define SVS_MRS_WSAT_GEN_DATA52 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 53 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA53 (SVN_BASE_PROTOCOL_MRS + 337)
#define SVS_MRS_WSAT_GEN_DATA53 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 54 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA54 (SVN_BASE_PROTOCOL_MRS + 338)
#define SVS_MRS_WSAT_GEN_DATA54 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 55 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA55 (SVN_BASE_PROTOCOL_MRS + 339)
#define SVS_MRS_WSAT_GEN_DATA55 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 56 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA56 (SVN_BASE_PROTOCOL_MRS + 340)
#define SVS_MRS_WSAT_GEN_DATA56 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 57 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA57 (SVN_BASE_PROTOCOL_MRS + 341)
#define SVS_MRS_WSAT_GEN_DATA57 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 58 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA58 (SVN_BASE_PROTOCOL_MRS + 342)
#define SVS_MRS_WSAT_GEN_DATA58 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 59 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA59 (SVN_BASE_PROTOCOL_MRS + 343)
#define SVS_MRS_WSAT_GEN_DATA59 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 60 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA60 (SVN_BASE_PROTOCOL_MRS + 344)
#define SVS_MRS_WSAT_GEN_DATA60 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 61 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA61 (SVN_BASE_PROTOCOL_MRS + 345)
#define SVS_MRS_WSAT_GEN_DATA61 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 62 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA62 (SVN_BASE_PROTOCOL_MRS + 346)
#define SVS_MRS_WSAT_GEN_DATA62 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 63 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA63 (SVN_BASE_PROTOCOL_MRS + 347)
#define SVS_MRS_WSAT_GEN_DATA63 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 64 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA64 (SVN_BASE_PROTOCOL_MRS + 348)
#define SVS_MRS_WSAT_GEN_DATA64 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 65 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA65 (SVN_BASE_PROTOCOL_MRS + 349)
#define SVS_MRS_WSAT_GEN_DATA65 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 66 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA66 (SVN_BASE_PROTOCOL_MRS + 350)
#define SVS_MRS_WSAT_GEN_DATA66 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 67 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA67 (SVN_BASE_PROTOCOL_MRS + 351)
#define SVS_MRS_WSAT_GEN_DATA67 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 68 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA68 (SVN_BASE_PROTOCOL_MRS + 352)
#define SVS_MRS_WSAT_GEN_DATA68 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 69 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA69 (SVN_BASE_PROTOCOL_MRS + 353)
#define SVS_MRS_WSAT_GEN_DATA69 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 70 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA70 (SVN_BASE_PROTOCOL_MRS + 354)
#define SVS_MRS_WSAT_GEN_DATA70 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 71 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA71 (SVN_BASE_PROTOCOL_MRS + 355)
#define SVS_MRS_WSAT_GEN_DATA71 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 72 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA72 (SVN_BASE_PROTOCOL_MRS + 356)
#define SVS_MRS_WSAT_GEN_DATA72 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 73 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA73 (SVN_BASE_PROTOCOL_MRS + 357)
#define SVS_MRS_WSAT_GEN_DATA73 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 74 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA74 (SVN_BASE_PROTOCOL_MRS + 358)
#define SVS_MRS_WSAT_GEN_DATA74 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 75 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA75 (SVN_BASE_PROTOCOL_MRS + 359)
#define SVS_MRS_WSAT_GEN_DATA75 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 76 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA76 (SVN_BASE_PROTOCOL_MRS + 360)
#define SVS_MRS_WSAT_GEN_DATA76 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 77 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA77 (SVN_BASE_PROTOCOL_MRS + 361)
#define SVS_MRS_WSAT_GEN_DATA77 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 78 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA78 (SVN_BASE_PROTOCOL_MRS + 362)
#define SVS_MRS_WSAT_GEN_DATA78 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 79 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA79 (SVN_BASE_PROTOCOL_MRS + 363)
#define SVS_MRS_WSAT_GEN_DATA79 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 80 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA80 (SVN_BASE_PROTOCOL_MRS + 364)
#define SVS_MRS_WSAT_GEN_DATA80 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 81 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA81 (SVN_BASE_PROTOCOL_MRS + 365)
#define SVS_MRS_WSAT_GEN_DATA81 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 82 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA82 (SVN_BASE_PROTOCOL_MRS + 366)
#define SVS_MRS_WSAT_GEN_DATA82 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 83 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA83 (SVN_BASE_PROTOCOL_MRS + 367)
#define SVS_MRS_WSAT_GEN_DATA83 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 84 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA84 (SVN_BASE_PROTOCOL_MRS + 368)
#define SVS_MRS_WSAT_GEN_DATA84 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 85 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA85 (SVN_BASE_PROTOCOL_MRS + 369)
#define SVS_MRS_WSAT_GEN_DATA85 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 86 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA86 (SVN_BASE_PROTOCOL_MRS + 370)
#define SVS_MRS_WSAT_GEN_DATA86 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 87 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA87 (SVN_BASE_PROTOCOL_MRS + 371)
#define SVS_MRS_WSAT_GEN_DATA87 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 88 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA88 (SVN_BASE_PROTOCOL_MRS + 372)
#define SVS_MRS_WSAT_GEN_DATA88 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 89 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA89 (SVN_BASE_PROTOCOL_MRS + 373)
#define SVS_MRS_WSAT_GEN_DATA89 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 90 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA90 (SVN_BASE_PROTOCOL_MRS + 374)
#define SVS_MRS_WSAT_GEN_DATA90 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 91 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA91 (SVN_BASE_PROTOCOL_MRS + 375)
#define SVS_MRS_WSAT_GEN_DATA91 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 92 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA92 (SVN_BASE_PROTOCOL_MRS + 376)
#define SVS_MRS_WSAT_GEN_DATA92 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 93 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA93 (SVN_BASE_PROTOCOL_MRS + 377)
#define SVS_MRS_WSAT_GEN_DATA93 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 94 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA94 (SVN_BASE_PROTOCOL_MRS + 378)
#define SVS_MRS_WSAT_GEN_DATA94 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 95 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA95 (SVN_BASE_PROTOCOL_MRS + 379)
#define SVS_MRS_WSAT_GEN_DATA95 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 96 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA96 (SVN_BASE_PROTOCOL_MRS + 380)
#define SVS_MRS_WSAT_GEN_DATA96 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 97 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA97 (SVN_BASE_PROTOCOL_MRS + 381)
#define SVS_MRS_WSAT_GEN_DATA97 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 98 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA98 (SVN_BASE_PROTOCOL_MRS + 382)
#define SVS_MRS_WSAT_GEN_DATA98 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 99 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA99 (SVN_BASE_PROTOCOL_MRS + 383)
#define SVS_MRS_WSAT_GEN_DATA99 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 100 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA100 (SVN_BASE_PROTOCOL_MRS + 384)
#define SVS_MRS_WSAT_GEN_DATA100 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 101 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA101 (SVN_BASE_PROTOCOL_MRS + 385)
#define SVS_MRS_WSAT_GEN_DATA101 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 102 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA102 (SVN_BASE_PROTOCOL_MRS + 386)
#define SVS_MRS_WSAT_GEN_DATA102 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 103 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA103 (SVN_BASE_PROTOCOL_MRS + 387)
#define SVS_MRS_WSAT_GEN_DATA103 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 104 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA104 (SVN_BASE_PROTOCOL_MRS + 388)
#define SVS_MRS_WSAT_GEN_DATA104 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 105 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA105 (SVN_BASE_PROTOCOL_MRS + 389)
#define SVS_MRS_WSAT_GEN_DATA105 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 106 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA106 (SVN_BASE_PROTOCOL_MRS + 390)
#define SVS_MRS_WSAT_GEN_DATA106 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 107 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA107 (SVN_BASE_PROTOCOL_MRS + 391)
#define SVS_MRS_WSAT_GEN_DATA107 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 108 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA108 (SVN_BASE_PROTOCOL_MRS + 392)
#define SVS_MRS_WSAT_GEN_DATA108 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 109 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA109 (SVN_BASE_PROTOCOL_MRS + 393)
#define SVS_MRS_WSAT_GEN_DATA109 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 110 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA110 (SVN_BASE_PROTOCOL_MRS + 394)
#define SVS_MRS_WSAT_GEN_DATA110 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 111 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA111 (SVN_BASE_PROTOCOL_MRS + 395)
#define SVS_MRS_WSAT_GEN_DATA111 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 112 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA112 (SVN_BASE_PROTOCOL_MRS + 396)
#define SVS_MRS_WSAT_GEN_DATA112 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 113 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA113 (SVN_BASE_PROTOCOL_MRS + 397)
#define SVS_MRS_WSAT_GEN_DATA113 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 114 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA114 (SVN_BASE_PROTOCOL_MRS + 398)
#define SVS_MRS_WSAT_GEN_DATA114 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 115 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA115 (SVN_BASE_PROTOCOL_MRS + 399)
#define SVS_MRS_WSAT_GEN_DATA115 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 116 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA116 (SVN_BASE_PROTOCOL_MRS + 400)
#define SVS_MRS_WSAT_GEN_DATA116 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 117 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA117 (SVN_BASE_PROTOCOL_MRS + 401)
#define SVS_MRS_WSAT_GEN_DATA117 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 118 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA118 (SVN_BASE_PROTOCOL_MRS + 402)
#define SVS_MRS_WSAT_GEN_DATA118 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 119 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA119 (SVN_BASE_PROTOCOL_MRS + 403)
#define SVS_MRS_WSAT_GEN_DATA119 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 120 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA120 (SVN_BASE_PROTOCOL_MRS + 404)
#define SVS_MRS_WSAT_GEN_DATA120 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 121 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA121 (SVN_BASE_PROTOCOL_MRS + 405)
#define SVS_MRS_WSAT_GEN_DATA121 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 122 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA122 (SVN_BASE_PROTOCOL_MRS + 406)
#define SVS_MRS_WSAT_GEN_DATA122 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 123 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA123 (SVN_BASE_PROTOCOL_MRS + 407)
#define SVS_MRS_WSAT_GEN_DATA123 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 124 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA124 (SVN_BASE_PROTOCOL_MRS + 408)
#define SVS_MRS_WSAT_GEN_DATA124 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 125 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA125 (SVN_BASE_PROTOCOL_MRS + 409)
#define SVS_MRS_WSAT_GEN_DATA125 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 126 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA126 (SVN_BASE_PROTOCOL_MRS + 410)
#define SVS_MRS_WSAT_GEN_DATA126 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 127 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA127 (SVN_BASE_PROTOCOL_MRS + 411)
#define SVS_MRS_WSAT_GEN_DATA127 "MRS Wsat Raw Data"
/*
    MRS Wsat Raw Data.
    Channel 128 Data in PAC scan.
    WsatプレスキャンのSVN_GEN_DATAを保持する。
    設定者：acqMan
    使用者：MRS後処理、acqMan
*/

#define SVN_MRS_WSAT_IMAGE_SIZE (SVN_BASE_PROTOCOL_MRS + 412)
#define SVT_MRS_WSAT_IMAGE_SIZE VFT_INT32
#define SVS_MRS_WSAT_IMAGE_SIZE "MRS Wsat Prescan Raw Data Size"
/*
    MRS Wsat Raw Data Size.
    WsatプレスキャンのSVN_GEN_DATAのデータサイズを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_PRESCAN_FSY (SVN_BASE_PROTOCOL_MRS + 413)
#define SVT_MRS_WSAT_PRESCAN_FSY VFT_FLT64
#define SVS_MRS_WSAT_PRESCAN_FSY "MRS Wsat Prescan Center Frequency value (Hz)"
/*
    MRS Wsat Prescan Center Frequency value.
    Wsatプレスキャンの中心周波数を設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_ACQ_MATRIX (SVN_BASE_PROTOCOL_MRS + 414)
#define SVT_MRS_WSAT_ACQ_MATRIX VFT_FLTVEC2
#define SVS_MRS_WSAT_ACQ_MATRIX "MRS Wsat Prescan Acquire Matrix Size"
/*
    MRS Wsat Acquire Matrix Size.
    Wsatプレスキャンのacquire matrixを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_RGN (SVN_BASE_PROTOCOL_MRS + 415)
#define SVT_MRS_WSAT_RGN VFT_FLT32
#define SVS_MRS_WSAT_RGN "MRS Wsat Prescan Receiver Gain value (dB)"
/*
    MRS Wsat Prescan Receiver Gain value.
    Wsatプレスキャンの受信ゲインを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA_TYPE (SVN_BASE_PROTOCOL_MRS + 416)
#define SVT_MRS_WSAT_GEN_DATA_TYPE VFT_INT32
#define SVS_MRS_WSAT_GEN_DATA_TYPE "MRS Wsat Prescan Image data type"
/*
    MRS Wsat Prescan Image data type.
    WsatプレスキャンSVN_GEN_DATAデータのタイプを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_GEN_DATA_FORM (SVN_BASE_PROTOCOL_MRS + 417)
#define SVT_MRS_WSAT_GEN_DATA_FORM VFT_INT32
#define SVS_MRS_WSAT_GEN_DATA_FORM "MRS Wsat Prescan Image data form"
/*
    MRS Wsat Prescan Image data form.
    WsatプレスキャンSVN_GEN_DATAデータのフォームを以下の値で設定する。
     VFC_FORM_MAGNITUDE 0
     VFC_FORM_PHASE     1
     VFC_FORM_REAL_ONLY 2
     VFC_FORM_IMGN_ONLY 3
     VFC_FORM_COMPLEX   4
     VFC_FORM_POLAR     5
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_ADC_PITCH (SVN_BASE_PROTOCOL_MRS + 418)
#define SVT_MRS_WSAT_ADC_PITCH VFT_FLT32
#define SVS_MRS_WSAT_ADC_PITCH "MRS Wsat Prescan ADC Pitch"
/*
    MRS Wsat Prescan ADC Pitch.
    WsatプレスキャンのADCpitchを設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/

#define SVN_MRS_WSAT_NUM_RO_SMPL (SVN_BASE_PROTOCOL_MRS + 419)
#define SVT_MRS_WSAT_NUM_RO_SMPL VFT_INT32
#define SVS_MRS_WSAT_NUM_RO_SMPL "MRS Wsat Prescan Number of Readout Samples Digitized"
/*
    MRS Wsat Prescan Number of Readout Samples Digitized.
    WsatプレスキャンSVN_GEN_DATAデータのRO方向サンプル数を設定する。
    設定者：acqMan
    使用者：MRS本スキャン後処理、acqMan
*/


/*****************************************************************************
  SUBPROTOCOL name space
*/

#define SVN_BASE_SUBPROTOCOL_MRS	SVN_MIN_SUBPROTOCOL+SVN_OFF_MRS

/*****************************************************************************
  SLICEGROUP/PRESAT name space
*/
    
#define SVN_BASE_SLICEGROUP_MRS	SVN_MIN_SLICEGROUP+SVN_OFF_MRS

/*****************************************************************************
  SLICE name space
*/

#define SVN_BASE_SLICE_MRS		SVN_MIN_SLICE+SVN_OFF_MRS

/*****************************************************************************
  IMAGE name space
*/

#define SVN_BASE_IMAGE_MRS		SVN_MIN_IMAGE+SVN_OFF_MRS

/*****************************************************************************
  PROCESSED name space
*/

#define SVN_BASE_PROCESSED_MRS	SVN_MIN_PROCESSED+SVN_OFF_MRS

#define SVN_MRS_SELECTED_START	(SVN_BASE_PROCESSED_MRS + 0)
#define SVT_MRS_SELECTED_START	VFT_FLTVEC3
#define SVS_MRS_SELECTED_START	"MRS selected voxel number(start)"
/*	ＭＲＳ関心領域スタートボクセル
*/

#define SVN_MRS_SELECTED_END	(SVN_BASE_PROCESSED_MRS + 1)
#define SVT_MRS_SELECTED_END	VFT_FLTVEC3
#define SVS_MRS_SELECTED_END	"MRS selected voxel number(end)"
/*	ＭＲＳ関心領域エンドボクセル
*/

#define SVN_MRS_FINE_RECON	(SVN_BASE_PROCESSED_MRS + 2)
#define SVT_MRS_FINE_RECON	VFT_INT32
#define SVS_MRS_FINE_RECON	"MRS fine recon flag"
/*	ＭＲＳ高精細再構成フラグ
*/

#define SVN_MRS_DATA	(SVN_BASE_PROCESSED_MRS + 3)
#define SVT_MRS_DATA	VFT_FLT32
#define SVS_MRS_DATA	"MRS raw data"
/*	ＭＲＳ収集データ
*/

#define SVN_MRS_FFT_DATA	(SVN_BASE_PROCESSED_MRS + 4)
#define SVT_MRS_FFT_DATA	VFT_FLT32
#define SVS_MRS_FFT_DATA	"MRS FFT data"
/*	ＭＲＳ再構成データ
*/

#define SVN_MRS_PC_DATA	(SVN_BASE_PROCESSED_MRS + 5)
#define SVT_MRS_PC_DATA	VFT_FLT32
#define SVS_MRS_PC_DATA	"MRS phase correct data"
/*	ＭＲＳ位相補正後データ
*/

#define SVN_MRS_BLC_DATA	(SVN_BASE_PROCESSED_MRS + 6)
#define SVT_MRS_BLC_DATA	VFT_FLT32
#define SVS_MRS_BLC_DATA	"MRS baseline correct data"
/*	ＭＲＳベースライン補正後データ
*/

#define SVN_MRS_BL_DATA	(SVN_BASE_PROCESSED_MRS + 7)
#define SVT_MRS_BL_DATA	VFT_FLT32
#define SVS_MRS_BL_DATA	"MRS baseline data"
/*	ＭＲＳベースラインデータ
*/

#define SVN_MRS_CF_DATA	(SVN_BASE_PROCESSED_MRS + 8)
#define SVT_MRS_CF_DATA	VFT_FLT32
#define SVS_MRS_CF_DATA	"MRS curve fit data"
/*	ＭＲＳカーブフィット後データ
*/

#define SVN_MRS_FFT_MAP	(SVN_BASE_PROCESSED_MRS + 9)
#define SVT_MRS_FFT_MAP	VFT_INT32
#define SVS_MRS_FFT_MAP	"MRS FFT data map"
/*	ＭＲＳ再構成実行マップ
*/

#define SVN_MRS_PC_MAP	(SVN_BASE_PROCESSED_MRS + 10)
#define SVT_MRS_PC_MAP	VFT_INT32
#define SVS_MRS_PC_MAP	"MRS phase correct data map"
/*	ＭＲＳ位相補正実行マップ
*/

#define SVN_MRS_BLC_MAP	(SVN_BASE_PROCESSED_MRS + 11)
#define SVT_MRS_BLC_MAP	VFT_INT32
#define SVS_MRS_BLC_MAP	"MRS baseline correct data map"
/*	ＭＲＳベースライン補正実行マップ
*/

#define SVN_MRS_BL_MAP	(SVN_BASE_PROCESSED_MRS + 12)
#define SVT_MRS_BL_MAP	VFT_INT32
#define SVS_MRS_BL_MAP	"MRS baseline data map"
/*	ＭＲＳベースラインマップ
*/

#define SVN_MRS_CF_MAP	(SVN_BASE_PROCESSED_MRS + 13)
#define SVT_MRS_CF_MAP	VFT_INT32
#define SVS_MRS_CF_MAP	"MRS curve fit data map"
/*	ＭＲＳカーブフィット実行マップ
*/

#define SVN_MRS_FS_DATA	(SVN_BASE_PROCESSED_MRS + 14)
#define SVT_MRS_FS_DATA	VFT_INT32
#define SVS_MRS_FS_DATA	"MRS frequency shift data"
/*	ＭＲＳ周波数シフトデータ
*/

#define SVN_MRS_SPECTRUM_PRF	(SVN_BASE_PROCESSED_MRS + 15)
#define SVT_MRS_SPECTRUM_PRF	VFT_STRING
#define SVS_MRS_SPECTRUM_PRF	"MRS spectrum preference parameter"
/*	ＭＲＳスペクトル表示パラメータ
*/

#define SVN_MRS_CSI_DATA	(SVN_BASE_PROCESSED_MRS + 16)
#define SVT_MRS_CSI_DATA	VFT_STRING
#define SVS_MRS_CSI_DATA	"MRS CSI data"
/*	ＭＲＳＣＳＩデータ。
	キーワード(MRSCSI)のあとのスペース後、１データ分（ヘッダ＋データ）
	のサイズが入り、スペース後にヘッダー部とデータ部となる。
	ヘッダー部とデータ部の区切りはない。
	次のデータはMRSCSIから書き込まれる。
	（例）MRSCSI 512 [ヘッダー部][データ部]MRSCSI 256 ・・・・
*/

#define SVN_MRS_MPC_PRF	(SVN_BASE_PROCESSED_MRS + 17)
#define SVT_MRS_MPC_PRF	VFT_STRING
#define SVS_MRS_MPC_PRF	"MRS manual phase correct preference parameter"
/*	手動位相補正パラメータ。
	キーワード(MPCPRF)のあとのスペース後、ボクセル番号がx,y,xの順に
	スペースで区切られて入る。zのあとのスペース後、データ部分
	のサイズとなり次のスペース後からサイズ分がデータとなる。
	次のデータはMRSPRFから書き込まれる。
	（例）MRSPRF 9 11 1 60 [データ部]MRSPRF 9 9 1 80・・・・
*/

#define SVN_MRS_RAW_FILTER	(SVN_BASE_PROCESSED_MRS + 18)
#define SVT_MRS_RAW_FILTER	VFT_FLT32
#define SVS_MRS_RAW_FILTER	"MRS Rawdata Filter parameter"
/*
    MRS Rawdata Filter parameter.
*/

#define SVN_MRS_WATER_DATA	(SVN_BASE_PROCESSED_MRS + 19)
#define SVT_MRS_WATER_DATA	VFT_FLT32
#define SVS_MRS_WATER_DATA	"MRS Ref Scan Water Data"
/*
    The water data created by Ref scan.
    In PAC scan, data after PAC summation. 
*/

#define SVN_MRS_REF_RAW_DATA    (SVN_BASE_PROCESSED_MRS + 20)
#define SVT_MRS_REF_RAW_DATA     VFT_FLT32
#define SVS_MRS_REF_RAW_DATA     "MRS Ref Scan Raw Data "
/* 
    The ref data cerated by Ref scan.
    Channel 1 Data 
*/

#define SVN_MRS_REF_RAW_DATA1	(SVN_BASE_PROCESSED_MRS + 21)
#define SVT_MRS_REF_RAW_DATA1	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA1	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 2 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA2	(SVN_BASE_PROCESSED_MRS + 22)
#define SVT_MRS_REF_RAW_DATA2	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA2	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 3 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA3	(SVN_BASE_PROCESSED_MRS + 23)
#define SVT_MRS_REF_RAW_DATA3	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA3	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 4 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA4	(SVN_BASE_PROCESSED_MRS + 24)
#define SVT_MRS_REF_RAW_DATA4	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA4	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 5 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA5	(SVN_BASE_PROCESSED_MRS + 25)
#define SVT_MRS_REF_RAW_DATA5	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA5	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 6 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA6	(SVN_BASE_PROCESSED_MRS + 26)
#define SVT_MRS_REF_RAW_DATA6	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA6	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 7 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA7	(SVN_BASE_PROCESSED_MRS + 27)
#define SVT_MRS_REF_RAW_DATA7	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA7	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 8 data in PAC scan.
*/

#define SVN_MRS_REF_WSIGNAL	(SVN_BASE_PROCESSED_MRS + 28)
#define SVT_MRS_REF_WSIGNAL	VFT_INT16
#define SVS_MRS_REF_WSIGNAL	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 1 data.
*/

#define SVN_MRS_REF_WSIGNAL1	(SVN_BASE_PROCESSED_MRS + 29)
#define SVT_MRS_REF_WSIGNAL1	VFT_INT16
#define SVS_MRS_REF_WSIGNAL1	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 2 data.
*/

#define SVN_MRS_REF_WSIGNAL2	(SVN_BASE_PROCESSED_MRS + 30)
#define SVT_MRS_REF_WSIGNAL2	VFT_INT16
#define SVS_MRS_REF_WSIGNAL2	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 3 data.
*/

#define SVN_MRS_REF_WSIGNAL3	(SVN_BASE_PROCESSED_MRS + 31)
#define SVT_MRS_REF_WSIGNAL3	VFT_INT16
#define SVS_MRS_REF_WSIGNAL3	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 4 data.
*/


#define SVN_MRS_REF_WSIGNAL4	(SVN_BASE_PROCESSED_MRS + 32)
#define SVT_MRS_REF_WSIGNAL4	VFT_INT16
#define SVS_MRS_REF_WSIGNAL4	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 5 data.
*/

#define SVN_MRS_REF_WSIGNAL5	(SVN_BASE_PROCESSED_MRS + 33)
#define SVT_MRS_REF_WSIGNAL5	VFT_INT16
#define SVS_MRS_REF_WSIGNAL5	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 6 data.
*/

#define SVN_MRS_REF_WSIGNAL6	(SVN_BASE_PROCESSED_MRS + 34)
#define SVT_MRS_REF_WSIGNAL6	VFT_INT16
#define SVS_MRS_REF_WSIGNAL6	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 7 data.
*/

#define SVN_MRS_REF_WSIGNAL7	(SVN_BASE_PROCESSED_MRS + 35)
#define SVT_MRS_REF_WSIGNAL7	VFT_INT16
#define SVS_MRS_REF_WSIGNAL7	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 8 data.
*/

#define SVN_MRS_REF_SHIFT_DATA	(SVN_BASE_PROCESSED_MRS + 36)
#define SVT_MRS_REF_SHIFT_DATA	VFT_INT16
#define SVS_MRS_REF_SHIFT_DATA	"MRS Ref Shift Data"
/*   
    The shift data for frequency shift from Ref scan.
*/

#define SVN_MRS_REF_RAW_DATA8	(SVN_BASE_PROCESSED_MRS + 37)
#define SVT_MRS_REF_RAW_DATA8	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA8	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 9 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA9	(SVN_BASE_PROCESSED_MRS + 38)
#define SVT_MRS_REF_RAW_DATA9	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA9	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 10 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA10	(SVN_BASE_PROCESSED_MRS + 39)
#define SVT_MRS_REF_RAW_DATA10	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA10	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 11 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA11	(SVN_BASE_PROCESSED_MRS + 40)
#define SVT_MRS_REF_RAW_DATA11	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA11	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 12 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA12	(SVN_BASE_PROCESSED_MRS + 41)
#define SVT_MRS_REF_RAW_DATA12	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA12	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 13 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA13	(SVN_BASE_PROCESSED_MRS + 42)
#define SVT_MRS_REF_RAW_DATA13	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA13	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 14 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA14	(SVN_BASE_PROCESSED_MRS + 43)
#define SVT_MRS_REF_RAW_DATA14	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA14	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 15 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA15	(SVN_BASE_PROCESSED_MRS + 44)
#define SVT_MRS_REF_RAW_DATA15	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA15	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 16 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA16	(SVN_BASE_PROCESSED_MRS + 45)
#define SVT_MRS_REF_RAW_DATA16	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA16	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 17 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA17	(SVN_BASE_PROCESSED_MRS + 46)
#define SVT_MRS_REF_RAW_DATA17	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA17	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 18 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA18	(SVN_BASE_PROCESSED_MRS + 47)
#define SVT_MRS_REF_RAW_DATA18	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA18	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 19 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA19	(SVN_BASE_PROCESSED_MRS + 48)
#define SVT_MRS_REF_RAW_DATA19	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA19	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 20 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA20	(SVN_BASE_PROCESSED_MRS + 49)
#define SVT_MRS_REF_RAW_DATA20	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA20	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 21 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA21	(SVN_BASE_PROCESSED_MRS + 50)
#define SVT_MRS_REF_RAW_DATA21	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA21	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 22 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA22	(SVN_BASE_PROCESSED_MRS + 51)
#define SVT_MRS_REF_RAW_DATA22	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA22	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 23 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA23	(SVN_BASE_PROCESSED_MRS + 52)
#define SVT_MRS_REF_RAW_DATA23	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA23	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 24 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA24	(SVN_BASE_PROCESSED_MRS + 53)
#define SVT_MRS_REF_RAW_DATA24	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA24	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 25 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA25	(SVN_BASE_PROCESSED_MRS + 54)
#define SVT_MRS_REF_RAW_DATA25	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA25	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 26 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA26	(SVN_BASE_PROCESSED_MRS + 55)
#define SVT_MRS_REF_RAW_DATA26	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA26	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 27 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA27	(SVN_BASE_PROCESSED_MRS + 56)
#define SVT_MRS_REF_RAW_DATA27	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA27	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 28 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA28	(SVN_BASE_PROCESSED_MRS + 57)
#define SVT_MRS_REF_RAW_DATA28	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA28	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 29 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA29	(SVN_BASE_PROCESSED_MRS + 58)
#define SVT_MRS_REF_RAW_DATA29	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA29	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 30 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA30	(SVN_BASE_PROCESSED_MRS + 59)
#define SVT_MRS_REF_RAW_DATA30	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA30	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 31 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA31	(SVN_BASE_PROCESSED_MRS + 60)
#define SVT_MRS_REF_RAW_DATA31	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA31	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 32 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA32	(SVN_BASE_PROCESSED_MRS + 61)
#define SVT_MRS_REF_RAW_DATA32	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA32	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 33 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA33	(SVN_BASE_PROCESSED_MRS + 62)
#define SVT_MRS_REF_RAW_DATA33	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA33	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 34 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA34	(SVN_BASE_PROCESSED_MRS + 63)
#define SVT_MRS_REF_RAW_DATA34	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA34	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 35 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA35	(SVN_BASE_PROCESSED_MRS + 64)
#define SVT_MRS_REF_RAW_DATA35	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA35	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 36 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA36	(SVN_BASE_PROCESSED_MRS + 65)
#define SVT_MRS_REF_RAW_DATA36	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA36	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 37 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA37	(SVN_BASE_PROCESSED_MRS + 66)
#define SVT_MRS_REF_RAW_DATA37	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA37	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 38 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA38	(SVN_BASE_PROCESSED_MRS + 67)
#define SVT_MRS_REF_RAW_DATA38	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA38	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 39 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA39	(SVN_BASE_PROCESSED_MRS + 68)
#define SVT_MRS_REF_RAW_DATA39	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA39	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 40 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA40	(SVN_BASE_PROCESSED_MRS + 69)
#define SVT_MRS_REF_RAW_DATA40	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA40	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 41 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA41	(SVN_BASE_PROCESSED_MRS + 70)
#define SVT_MRS_REF_RAW_DATA41	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA41	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 42 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA42	(SVN_BASE_PROCESSED_MRS + 71)
#define SVT_MRS_REF_RAW_DATA42	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA42	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 43 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA43	(SVN_BASE_PROCESSED_MRS + 72)
#define SVT_MRS_REF_RAW_DATA43	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA43	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 44 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA44	(SVN_BASE_PROCESSED_MRS + 73)
#define SVT_MRS_REF_RAW_DATA44	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA44	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 45 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA45	(SVN_BASE_PROCESSED_MRS + 74)
#define SVT_MRS_REF_RAW_DATA45	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA45	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 46 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA46	(SVN_BASE_PROCESSED_MRS + 75)
#define SVT_MRS_REF_RAW_DATA46	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA46	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 47 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA47	(SVN_BASE_PROCESSED_MRS + 76)
#define SVT_MRS_REF_RAW_DATA47	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA47	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 48 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA48	(SVN_BASE_PROCESSED_MRS + 77)
#define SVT_MRS_REF_RAW_DATA48	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA48	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 49 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA49	(SVN_BASE_PROCESSED_MRS + 78)
#define SVT_MRS_REF_RAW_DATA49	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA49	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 50 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA50	(SVN_BASE_PROCESSED_MRS + 79)
#define SVT_MRS_REF_RAW_DATA50	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA50	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 51 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA51	(SVN_BASE_PROCESSED_MRS + 80)
#define SVT_MRS_REF_RAW_DATA51	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA51	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 52 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA52	(SVN_BASE_PROCESSED_MRS + 81)
#define SVT_MRS_REF_RAW_DATA52	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA52	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 53 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA53	(SVN_BASE_PROCESSED_MRS + 82)
#define SVT_MRS_REF_RAW_DATA53	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA53	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 54 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA54	(SVN_BASE_PROCESSED_MRS + 83)
#define SVT_MRS_REF_RAW_DATA54	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA54	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 55 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA55	(SVN_BASE_PROCESSED_MRS + 84)
#define SVT_MRS_REF_RAW_DATA55	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA55	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 56 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA56	(SVN_BASE_PROCESSED_MRS + 85)
#define SVT_MRS_REF_RAW_DATA56	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA56	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 57 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA57	(SVN_BASE_PROCESSED_MRS + 86)
#define SVT_MRS_REF_RAW_DATA57	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA57	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 58 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA58	(SVN_BASE_PROCESSED_MRS + 87)
#define SVT_MRS_REF_RAW_DATA58	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA58	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 59 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA59	(SVN_BASE_PROCESSED_MRS + 88)
#define SVT_MRS_REF_RAW_DATA59	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA59	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 60 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA60	(SVN_BASE_PROCESSED_MRS + 89)
#define SVT_MRS_REF_RAW_DATA60	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA60	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 61 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA61	(SVN_BASE_PROCESSED_MRS + 90)
#define SVT_MRS_REF_RAW_DATA61	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA61	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 62 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA62	(SVN_BASE_PROCESSED_MRS + 91)
#define SVT_MRS_REF_RAW_DATA62	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA62	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 63 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA63	(SVN_BASE_PROCESSED_MRS + 92)
#define SVT_MRS_REF_RAW_DATA63	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA63	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 64 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA64	(SVN_BASE_PROCESSED_MRS + 93)
#define SVT_MRS_REF_RAW_DATA64	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA64	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 65 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA65	(SVN_BASE_PROCESSED_MRS + 94)
#define SVT_MRS_REF_RAW_DATA65	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA65	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 66 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA66	(SVN_BASE_PROCESSED_MRS + 95)
#define SVT_MRS_REF_RAW_DATA66	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA66	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 67 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA67	(SVN_BASE_PROCESSED_MRS + 96)
#define SVT_MRS_REF_RAW_DATA67	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA67	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 68 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA68	(SVN_BASE_PROCESSED_MRS + 97)
#define SVT_MRS_REF_RAW_DATA68	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA68	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 69 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA69	(SVN_BASE_PROCESSED_MRS + 98)
#define SVT_MRS_REF_RAW_DATA69	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA69	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 70 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA70	(SVN_BASE_PROCESSED_MRS + 99)
#define SVT_MRS_REF_RAW_DATA70	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA70	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 71 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA71	(SVN_BASE_PROCESSED_MRS + 100)
#define SVT_MRS_REF_RAW_DATA71	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA71	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 72 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA72	(SVN_BASE_PROCESSED_MRS + 101)
#define SVT_MRS_REF_RAW_DATA72	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA72	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 73 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA73	(SVN_BASE_PROCESSED_MRS + 102)
#define SVT_MRS_REF_RAW_DATA73	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA73	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 74 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA74	(SVN_BASE_PROCESSED_MRS + 103)
#define SVT_MRS_REF_RAW_DATA74	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA74	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 75 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA75	(SVN_BASE_PROCESSED_MRS + 104)
#define SVT_MRS_REF_RAW_DATA75	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA75	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 76 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA76	(SVN_BASE_PROCESSED_MRS + 105)
#define SVT_MRS_REF_RAW_DATA76	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA76	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 77 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA77	(SVN_BASE_PROCESSED_MRS + 106)
#define SVT_MRS_REF_RAW_DATA77	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA77	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 78 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA78	(SVN_BASE_PROCESSED_MRS + 107)
#define SVT_MRS_REF_RAW_DATA78	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA78	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 79 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA79	(SVN_BASE_PROCESSED_MRS + 108)
#define SVT_MRS_REF_RAW_DATA79	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA79	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 80 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA80	(SVN_BASE_PROCESSED_MRS + 109)
#define SVT_MRS_REF_RAW_DATA80	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA80	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 81 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA81	(SVN_BASE_PROCESSED_MRS + 110)
#define SVT_MRS_REF_RAW_DATA81	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA81	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 82 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA82	(SVN_BASE_PROCESSED_MRS + 111)
#define SVT_MRS_REF_RAW_DATA82	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA82	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 83 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA83	(SVN_BASE_PROCESSED_MRS + 112)
#define SVT_MRS_REF_RAW_DATA83	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA83	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 84 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA84	(SVN_BASE_PROCESSED_MRS + 113)
#define SVT_MRS_REF_RAW_DATA84	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA84	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 85 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA85	(SVN_BASE_PROCESSED_MRS + 114)
#define SVT_MRS_REF_RAW_DATA85	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA85	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 86 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA86	(SVN_BASE_PROCESSED_MRS + 115)
#define SVT_MRS_REF_RAW_DATA86	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA86	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 87 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA87	(SVN_BASE_PROCESSED_MRS + 116)
#define SVT_MRS_REF_RAW_DATA87	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA87	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 88 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA88	(SVN_BASE_PROCESSED_MRS + 117)
#define SVT_MRS_REF_RAW_DATA88	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA88	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 89 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA89	(SVN_BASE_PROCESSED_MRS + 118)
#define SVT_MRS_REF_RAW_DATA89	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA89	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 90 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA90	(SVN_BASE_PROCESSED_MRS + 119)
#define SVT_MRS_REF_RAW_DATA90	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA90	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 91 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA91	(SVN_BASE_PROCESSED_MRS + 120)
#define SVT_MRS_REF_RAW_DATA91	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA91	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 92 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA92	(SVN_BASE_PROCESSED_MRS + 121)
#define SVT_MRS_REF_RAW_DATA92	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA92	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 93 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA93	(SVN_BASE_PROCESSED_MRS + 122)
#define SVT_MRS_REF_RAW_DATA93	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA93	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 94 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA94	(SVN_BASE_PROCESSED_MRS + 123)
#define SVT_MRS_REF_RAW_DATA94	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA94	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 95 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA95	(SVN_BASE_PROCESSED_MRS + 124)
#define SVT_MRS_REF_RAW_DATA95	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA95	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 96 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA96	(SVN_BASE_PROCESSED_MRS + 125)
#define SVT_MRS_REF_RAW_DATA96	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA96	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 97 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA97	(SVN_BASE_PROCESSED_MRS + 126)
#define SVT_MRS_REF_RAW_DATA97	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA97	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 98 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA98	(SVN_BASE_PROCESSED_MRS + 127)
#define SVT_MRS_REF_RAW_DATA98	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA98	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 99 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA99	(SVN_BASE_PROCESSED_MRS + 128)
#define SVT_MRS_REF_RAW_DATA99	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA99	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 100 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA100	(SVN_BASE_PROCESSED_MRS + 129)
#define SVT_MRS_REF_RAW_DATA100	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA100	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 101 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA101	(SVN_BASE_PROCESSED_MRS + 130)
#define SVT_MRS_REF_RAW_DATA101	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA101	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 102 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA102	(SVN_BASE_PROCESSED_MRS + 131)
#define SVT_MRS_REF_RAW_DATA102	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA102	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 103 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA103	(SVN_BASE_PROCESSED_MRS + 132)
#define SVT_MRS_REF_RAW_DATA103	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA103	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 104 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA104	(SVN_BASE_PROCESSED_MRS + 133)
#define SVT_MRS_REF_RAW_DATA104	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA104	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 105 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA105	(SVN_BASE_PROCESSED_MRS + 134)
#define SVT_MRS_REF_RAW_DATA105	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA105	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 106 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA106	(SVN_BASE_PROCESSED_MRS + 135)
#define SVT_MRS_REF_RAW_DATA106	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA106	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 107 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA107	(SVN_BASE_PROCESSED_MRS + 136)
#define SVT_MRS_REF_RAW_DATA107	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA107	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 108 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA108	(SVN_BASE_PROCESSED_MRS + 137)
#define SVT_MRS_REF_RAW_DATA108	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA108	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 109 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA109	(SVN_BASE_PROCESSED_MRS + 138)
#define SVT_MRS_REF_RAW_DATA109	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA109	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 110 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA110	(SVN_BASE_PROCESSED_MRS + 139)
#define SVT_MRS_REF_RAW_DATA110	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA110	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 111 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA111	(SVN_BASE_PROCESSED_MRS + 140)
#define SVT_MRS_REF_RAW_DATA111	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA111	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 112 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA112	(SVN_BASE_PROCESSED_MRS + 141)
#define SVT_MRS_REF_RAW_DATA112	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA112	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 113 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA113	(SVN_BASE_PROCESSED_MRS + 142)
#define SVT_MRS_REF_RAW_DATA113	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA113	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 114 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA114	(SVN_BASE_PROCESSED_MRS + 143)
#define SVT_MRS_REF_RAW_DATA114	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA114	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 115 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA115	(SVN_BASE_PROCESSED_MRS + 144)
#define SVT_MRS_REF_RAW_DATA115	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA115	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 116 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA116	(SVN_BASE_PROCESSED_MRS + 145)
#define SVT_MRS_REF_RAW_DATA116	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA116	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 117 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA117	(SVN_BASE_PROCESSED_MRS + 146)
#define SVT_MRS_REF_RAW_DATA117	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA117	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 118 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA118	(SVN_BASE_PROCESSED_MRS + 147)
#define SVT_MRS_REF_RAW_DATA118	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA118	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 119 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA119	(SVN_BASE_PROCESSED_MRS + 148)
#define SVT_MRS_REF_RAW_DATA119	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA119	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 120 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA120	(SVN_BASE_PROCESSED_MRS + 149)
#define SVT_MRS_REF_RAW_DATA120	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA120	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 121 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA121	(SVN_BASE_PROCESSED_MRS + 150)
#define SVT_MRS_REF_RAW_DATA121	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA121	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 122 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA122	(SVN_BASE_PROCESSED_MRS + 151)
#define SVT_MRS_REF_RAW_DATA122	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA122	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 123 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA123	(SVN_BASE_PROCESSED_MRS + 152)
#define SVT_MRS_REF_RAW_DATA123	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA123	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 124 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA124	(SVN_BASE_PROCESSED_MRS + 153)
#define SVT_MRS_REF_RAW_DATA124	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA124	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 125 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA125	(SVN_BASE_PROCESSED_MRS + 154)
#define SVT_MRS_REF_RAW_DATA125	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA125	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 126 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA126	(SVN_BASE_PROCESSED_MRS + 155)
#define SVT_MRS_REF_RAW_DATA126	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA126	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 127 data in PAC scan.
*/

#define SVN_MRS_REF_RAW_DATA127	(SVN_BASE_PROCESSED_MRS + 156)
#define SVT_MRS_REF_RAW_DATA127	VFT_FLT32
#define SVS_MRS_REF_RAW_DATA127	"MRS Ref Scan Raw Data"
/*
    The ref data created by Ref scan.
    Channel 128 data in PAC scan.
*/

#define SVN_MRS_REF_WSIGNAL8	(SVN_BASE_PROCESSED_MRS + 157)
#define SVT_MRS_REF_WSIGNAL8	VFT_INT16
#define SVS_MRS_REF_WSIGNAL8	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 9 data.
*/

#define SVN_MRS_REF_WSIGNAL9	(SVN_BASE_PROCESSED_MRS + 158)
#define SVT_MRS_REF_WSIGNAL9	VFT_INT16
#define SVS_MRS_REF_WSIGNAL9	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 10 data.
*/

#define SVN_MRS_REF_WSIGNAL10	(SVN_BASE_PROCESSED_MRS + 159)
#define SVT_MRS_REF_WSIGNAL10	VFT_INT16
#define SVS_MRS_REF_WSIGNAL10	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 11 data.
*/

#define SVN_MRS_REF_WSIGNAL11	(SVN_BASE_PROCESSED_MRS + 160)
#define SVT_MRS_REF_WSIGNAL11	VFT_INT16
#define SVS_MRS_REF_WSIGNAL11	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 12 data.
*/

#define SVN_MRS_REF_WSIGNAL12	(SVN_BASE_PROCESSED_MRS + 161)
#define SVT_MRS_REF_WSIGNAL12	VFT_INT16
#define SVS_MRS_REF_WSIGNAL12	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 13 data.
*/

#define SVN_MRS_REF_WSIGNAL13	(SVN_BASE_PROCESSED_MRS + 162)
#define SVT_MRS_REF_WSIGNAL13	VFT_INT16
#define SVS_MRS_REF_WSIGNAL13	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 14 data.
*/

#define SVN_MRS_REF_WSIGNAL14	(SVN_BASE_PROCESSED_MRS + 163)
#define SVT_MRS_REF_WSIGNAL14	VFT_INT16
#define SVS_MRS_REF_WSIGNAL14	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 15 data.
*/

#define SVN_MRS_REF_WSIGNAL15	(SVN_BASE_PROCESSED_MRS + 164)
#define SVT_MRS_REF_WSIGNAL15	VFT_INT16
#define SVS_MRS_REF_WSIGNAL15	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 16 data.
*/

#define SVN_MRS_REF_WSIGNAL16	(SVN_BASE_PROCESSED_MRS + 165)
#define SVT_MRS_REF_WSIGNAL16	VFT_INT16
#define SVS_MRS_REF_WSIGNAL16	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 17 data.
*/

#define SVN_MRS_REF_WSIGNAL17	(SVN_BASE_PROCESSED_MRS + 166)
#define SVT_MRS_REF_WSIGNAL17	VFT_INT16
#define SVS_MRS_REF_WSIGNAL17	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 18 data.
*/

#define SVN_MRS_REF_WSIGNAL18	(SVN_BASE_PROCESSED_MRS + 167)
#define SVT_MRS_REF_WSIGNAL18	VFT_INT16
#define SVS_MRS_REF_WSIGNAL18	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 19 data.
*/

#define SVN_MRS_REF_WSIGNAL19	(SVN_BASE_PROCESSED_MRS + 168)
#define SVT_MRS_REF_WSIGNAL19	VFT_INT16
#define SVS_MRS_REF_WSIGNAL19	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 20 data.
*/

#define SVN_MRS_REF_WSIGNAL20	(SVN_BASE_PROCESSED_MRS + 169)
#define SVT_MRS_REF_WSIGNAL20	VFT_INT16
#define SVS_MRS_REF_WSIGNAL20	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 21 data.
*/

#define SVN_MRS_REF_WSIGNAL21	(SVN_BASE_PROCESSED_MRS + 170)
#define SVT_MRS_REF_WSIGNAL21	VFT_INT16
#define SVS_MRS_REF_WSIGNAL21	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 22 data.
*/

#define SVN_MRS_REF_WSIGNAL22	(SVN_BASE_PROCESSED_MRS + 171)
#define SVT_MRS_REF_WSIGNAL22	VFT_INT16
#define SVS_MRS_REF_WSIGNAL22	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 23 data.
*/

#define SVN_MRS_REF_WSIGNAL23	(SVN_BASE_PROCESSED_MRS + 172)
#define SVT_MRS_REF_WSIGNAL23	VFT_INT16
#define SVS_MRS_REF_WSIGNAL23	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 24 data.
*/

#define SVN_MRS_REF_WSIGNAL24	(SVN_BASE_PROCESSED_MRS + 173)
#define SVT_MRS_REF_WSIGNAL24	VFT_INT16
#define SVS_MRS_REF_WSIGNAL24	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 25 data.
*/

#define SVN_MRS_REF_WSIGNAL25	(SVN_BASE_PROCESSED_MRS + 174)
#define SVT_MRS_REF_WSIGNAL25	VFT_INT16
#define SVS_MRS_REF_WSIGNAL25	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 26 data.
*/

#define SVN_MRS_REF_WSIGNAL26	(SVN_BASE_PROCESSED_MRS + 175)
#define SVT_MRS_REF_WSIGNAL26	VFT_INT16
#define SVS_MRS_REF_WSIGNAL26	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 27 data.
*/

#define SVN_MRS_REF_WSIGNAL27	(SVN_BASE_PROCESSED_MRS + 176)
#define SVT_MRS_REF_WSIGNAL27	VFT_INT16
#define SVS_MRS_REF_WSIGNAL27	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 28 data.
*/

#define SVN_MRS_REF_WSIGNAL28	(SVN_BASE_PROCESSED_MRS + 177)
#define SVT_MRS_REF_WSIGNAL28	VFT_INT16
#define SVS_MRS_REF_WSIGNAL28	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 29 data.
*/

#define SVN_MRS_REF_WSIGNAL29	(SVN_BASE_PROCESSED_MRS + 178)
#define SVT_MRS_REF_WSIGNAL29	VFT_INT16
#define SVS_MRS_REF_WSIGNAL29	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 30 data.
*/

#define SVN_MRS_REF_WSIGNAL30	(SVN_BASE_PROCESSED_MRS + 179)
#define SVT_MRS_REF_WSIGNAL30	VFT_INT16
#define SVS_MRS_REF_WSIGNAL30	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 31 data.
*/

#define SVN_MRS_REF_WSIGNAL31	(SVN_BASE_PROCESSED_MRS + 180)
#define SVT_MRS_REF_WSIGNAL31	VFT_INT16
#define SVS_MRS_REF_WSIGNAL31	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 32 data.
*/

#define SVN_MRS_REF_WSIGNAL32	(SVN_BASE_PROCESSED_MRS + 181)
#define SVT_MRS_REF_WSIGNAL32	VFT_INT16
#define SVS_MRS_REF_WSIGNAL32	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 33 data.
*/

#define SVN_MRS_REF_WSIGNAL33	(SVN_BASE_PROCESSED_MRS + 182)
#define SVT_MRS_REF_WSIGNAL33	VFT_INT16
#define SVS_MRS_REF_WSIGNAL33	"MRS Ref Water signal intensity"
/*  
    The water signal intensity value from Ref scan.
    Channel 34 data.
*/

#define SVN_MRS_REF_WSIGNAL34	(SVN_BASE_PROCESSED_MRS + 183)
#define SVT_MRS_REF_WSIGNAL34	VFT_INT16
#define SVS_MRS_REF_WSIGNAL34	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 35 data.
*/

#define SVN_MRS_REF_WSIGNAL35	(SVN_BASE_PROCESSED_MRS + 184)
#define SVT_MRS_REF_WSIGNAL35	VFT_INT16
#define SVS_MRS_REF_WSIGNAL35	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 36 data.
*/

#define SVN_MRS_REF_WSIGNAL36	(SVN_BASE_PROCESSED_MRS + 185)
#define SVT_MRS_REF_WSIGNAL36	VFT_INT16
#define SVS_MRS_REF_WSIGNAL36	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 37 data.
*/

#define SVN_MRS_REF_WSIGNAL37	(SVN_BASE_PROCESSED_MRS + 186)
#define SVT_MRS_REF_WSIGNAL37	VFT_INT16
#define SVS_MRS_REF_WSIGNAL37	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 38 data.
*/

#define SVN_MRS_REF_WSIGNAL38	(SVN_BASE_PROCESSED_MRS + 187)
#define SVT_MRS_REF_WSIGNAL38	VFT_INT16
#define SVS_MRS_REF_WSIGNAL38	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 39 data.
*/

#define SVN_MRS_REF_WSIGNAL39	(SVN_BASE_PROCESSED_MRS + 188)
#define SVT_MRS_REF_WSIGNAL39	VFT_INT16
#define SVS_MRS_REF_WSIGNAL39	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 40 data.
*/

#define SVN_MRS_REF_WSIGNAL40	(SVN_BASE_PROCESSED_MRS + 189)
#define SVT_MRS_REF_WSIGNAL40	VFT_INT16
#define SVS_MRS_REF_WSIGNAL40	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 41 data.
*/

#define SVN_MRS_REF_WSIGNAL41	(SVN_BASE_PROCESSED_MRS + 190)
#define SVT_MRS_REF_WSIGNAL41	VFT_INT16
#define SVS_MRS_REF_WSIGNAL41	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 42 data.
*/

#define SVN_MRS_REF_WSIGNAL42	(SVN_BASE_PROCESSED_MRS + 191)
#define SVT_MRS_REF_WSIGNAL42	VFT_INT16
#define SVS_MRS_REF_WSIGNAL42	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 43 data.
*/

#define SVN_MRS_REF_WSIGNAL43	(SVN_BASE_PROCESSED_MRS + 192)
#define SVT_MRS_REF_WSIGNAL43	VFT_INT16
#define SVS_MRS_REF_WSIGNAL43	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 44 data.
*/

#define SVN_MRS_REF_WSIGNAL44	(SVN_BASE_PROCESSED_MRS + 193)
#define SVT_MRS_REF_WSIGNAL44	VFT_INT16
#define SVS_MRS_REF_WSIGNAL44	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 45 data.
*/

#define SVN_MRS_REF_WSIGNAL45	(SVN_BASE_PROCESSED_MRS + 194)
#define SVT_MRS_REF_WSIGNAL45	VFT_INT16
#define SVS_MRS_REF_WSIGNAL45	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 46 data.
*/

#define SVN_MRS_REF_WSIGNAL46	(SVN_BASE_PROCESSED_MRS + 195)
#define SVT_MRS_REF_WSIGNAL46	VFT_INT16
#define SVS_MRS_REF_WSIGNAL46	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 47 data.
*/

#define SVN_MRS_REF_WSIGNAL47	(SVN_BASE_PROCESSED_MRS + 196)
#define SVT_MRS_REF_WSIGNAL47	VFT_INT16
#define SVS_MRS_REF_WSIGNAL47	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 48 data.
*/

#define SVN_MRS_REF_WSIGNAL48	(SVN_BASE_PROCESSED_MRS + 197)
#define SVT_MRS_REF_WSIGNAL48	VFT_INT16
#define SVS_MRS_REF_WSIGNAL48	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 49 data.
*/

#define SVN_MRS_REF_WSIGNAL49	(SVN_BASE_PROCESSED_MRS + 198)
#define SVT_MRS_REF_WSIGNAL49	VFT_INT16
#define SVS_MRS_REF_WSIGNAL49	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 50 data.
*/

#define SVN_MRS_REF_WSIGNAL50	(SVN_BASE_PROCESSED_MRS + 199)
#define SVT_MRS_REF_WSIGNAL50	VFT_INT16
#define SVS_MRS_REF_WSIGNAL50	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 51 data.
*/

#define SVN_MRS_REF_WSIGNAL51	(SVN_BASE_PROCESSED_MRS + 200)
#define SVT_MRS_REF_WSIGNAL51	VFT_INT16
#define SVS_MRS_REF_WSIGNAL51	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 52 data.
*/

#define SVN_MRS_REF_WSIGNAL52	(SVN_BASE_PROCESSED_MRS + 201)
#define SVT_MRS_REF_WSIGNAL52	VFT_INT16
#define SVS_MRS_REF_WSIGNAL52	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 53 data.
*/

#define SVN_MRS_REF_WSIGNAL53	(SVN_BASE_PROCESSED_MRS + 202)
#define SVT_MRS_REF_WSIGNAL53	VFT_INT16
#define SVS_MRS_REF_WSIGNAL53	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 54 data.
*/

#define SVN_MRS_REF_WSIGNAL54	(SVN_BASE_PROCESSED_MRS + 203)
#define SVT_MRS_REF_WSIGNAL54	VFT_INT16
#define SVS_MRS_REF_WSIGNAL54	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 55 data.
*/

#define SVN_MRS_REF_WSIGNAL55	(SVN_BASE_PROCESSED_MRS + 204)
#define SVT_MRS_REF_WSIGNAL55	VFT_INT16
#define SVS_MRS_REF_WSIGNAL55	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 56 data.
*/

#define SVN_MRS_REF_WSIGNAL56	(SVN_BASE_PROCESSED_MRS + 205)
#define SVT_MRS_REF_WSIGNAL56	VFT_INT16
#define SVS_MRS_REF_WSIGNAL56	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 57 data.
*/

#define SVN_MRS_REF_WSIGNAL57	(SVN_BASE_PROCESSED_MRS + 206)
#define SVT_MRS_REF_WSIGNAL57	VFT_INT16
#define SVS_MRS_REF_WSIGNAL57	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 58 data.
*/

#define SVN_MRS_REF_WSIGNAL58	(SVN_BASE_PROCESSED_MRS + 207)
#define SVT_MRS_REF_WSIGNAL58	VFT_INT16
#define SVS_MRS_REF_WSIGNAL58	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 59 data.
*/

#define SVN_MRS_REF_WSIGNAL59	(SVN_BASE_PROCESSED_MRS + 208)
#define SVT_MRS_REF_WSIGNAL59	VFT_INT16
#define SVS_MRS_REF_WSIGNAL59	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 60 data.
*/

#define SVN_MRS_REF_WSIGNAL60	(SVN_BASE_PROCESSED_MRS + 209)
#define SVT_MRS_REF_WSIGNAL60	VFT_INT16
#define SVS_MRS_REF_WSIGNAL60	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 61 data.
*/

#define SVN_MRS_REF_WSIGNAL61	(SVN_BASE_PROCESSED_MRS + 210)
#define SVT_MRS_REF_WSIGNAL61	VFT_INT16
#define SVS_MRS_REF_WSIGNAL61	"MRS Ref Water signal intensity"
/*  
    The water signal intensity value from Ref scan.
    Channel 62 data.
*/

#define SVN_MRS_REF_WSIGNAL62	(SVN_BASE_PROCESSED_MRS + 211)
#define SVT_MRS_REF_WSIGNAL62	VFT_INT16
#define SVS_MRS_REF_WSIGNAL62	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 63 data.
*/

#define SVN_MRS_REF_WSIGNAL63	(SVN_BASE_PROCESSED_MRS + 212)
#define SVT_MRS_REF_WSIGNAL63	VFT_INT16
#define SVS_MRS_REF_WSIGNAL63	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 64 data.
*/

#define SVN_MRS_REF_WSIGNAL64	(SVN_BASE_PROCESSED_MRS + 213)
#define SVT_MRS_REF_WSIGNAL64	VFT_INT16
#define SVS_MRS_REF_WSIGNAL64	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 65 data.
*/

#define SVN_MRS_REF_WSIGNAL65	(SVN_BASE_PROCESSED_MRS + 214)
#define SVT_MRS_REF_WSIGNAL65	VFT_INT16
#define SVS_MRS_REF_WSIGNAL65	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 66 data.
*/

#define SVN_MRS_REF_WSIGNAL66	(SVN_BASE_PROCESSED_MRS + 215)
#define SVT_MRS_REF_WSIGNAL66	VFT_INT16
#define SVS_MRS_REF_WSIGNAL66	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 67 data.
*/

#define SVN_MRS_REF_WSIGNAL67	(SVN_BASE_PROCESSED_MRS + 216)
#define SVT_MRS_REF_WSIGNAL67	VFT_INT16
#define SVS_MRS_REF_WSIGNAL67	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 68 data.
*/

#define SVN_MRS_REF_WSIGNAL68	(SVN_BASE_PROCESSED_MRS + 217)
#define SVT_MRS_REF_WSIGNAL68	VFT_INT16
#define SVS_MRS_REF_WSIGNAL68	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 69 data.
*/

#define SVN_MRS_REF_WSIGNAL69	(SVN_BASE_PROCESSED_MRS + 218)
#define SVT_MRS_REF_WSIGNAL69	VFT_INT16
#define SVS_MRS_REF_WSIGNAL69	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 70 data.
*/

#define SVN_MRS_REF_WSIGNAL70	(SVN_BASE_PROCESSED_MRS + 219)
#define SVT_MRS_REF_WSIGNAL70	VFT_INT16
#define SVS_MRS_REF_WSIGNAL70	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 71 data.
*/

#define SVN_MRS_REF_WSIGNAL71	(SVN_BASE_PROCESSED_MRS + 220)
#define SVT_MRS_REF_WSIGNAL71	VFT_INT16
#define SVS_MRS_REF_WSIGNAL71	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 72 data.
*/

#define SVN_MRS_REF_WSIGNAL72	(SVN_BASE_PROCESSED_MRS + 221)
#define SVT_MRS_REF_WSIGNAL72	VFT_INT16
#define SVS_MRS_REF_WSIGNAL72	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 73 data.
*/

#define SVN_MRS_REF_WSIGNAL73	(SVN_BASE_PROCESSED_MRS + 222)
#define SVT_MRS_REF_WSIGNAL73	VFT_INT16
#define SVS_MRS_REF_WSIGNAL73	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 74 data.
*/

#define SVN_MRS_REF_WSIGNAL74	(SVN_BASE_PROCESSED_MRS + 223)
#define SVT_MRS_REF_WSIGNAL74	VFT_INT16
#define SVS_MRS_REF_WSIGNAL74	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 75 data.
*/

#define SVN_MRS_REF_WSIGNAL75	(SVN_BASE_PROCESSED_MRS + 224)
#define SVT_MRS_REF_WSIGNAL75	VFT_INT16
#define SVS_MRS_REF_WSIGNAL75	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 76 data.
*/

#define SVN_MRS_REF_WSIGNAL76	(SVN_BASE_PROCESSED_MRS + 225)
#define SVT_MRS_REF_WSIGNAL76	VFT_INT16
#define SVS_MRS_REF_WSIGNAL76	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 77 data.
*/

#define SVN_MRS_REF_WSIGNAL77	(SVN_BASE_PROCESSED_MRS + 226)
#define SVT_MRS_REF_WSIGNAL77	VFT_INT16
#define SVS_MRS_REF_WSIGNAL77	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 78 data.
*/

#define SVN_MRS_REF_WSIGNAL78	(SVN_BASE_PROCESSED_MRS + 227)
#define SVT_MRS_REF_WSIGNAL78	VFT_INT16
#define SVS_MRS_REF_WSIGNAL78	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 79 data.
*/

#define SVN_MRS_REF_WSIGNAL79	(SVN_BASE_PROCESSED_MRS + 228)
#define SVT_MRS_REF_WSIGNAL79	VFT_INT16
#define SVS_MRS_REF_WSIGNAL79	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 80 data.
*/

#define SVN_MRS_REF_WSIGNAL80	(SVN_BASE_PROCESSED_MRS + 229)
#define SVT_MRS_REF_WSIGNAL80	VFT_INT16
#define SVS_MRS_REF_WSIGNAL80	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 81 data.
*/

#define SVN_MRS_REF_WSIGNAL81	(SVN_BASE_PROCESSED_MRS + 230)
#define SVT_MRS_REF_WSIGNAL81	VFT_INT16
#define SVS_MRS_REF_WSIGNAL81	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 82 data.
*/

#define SVN_MRS_REF_WSIGNAL82	(SVN_BASE_PROCESSED_MRS + 231)
#define SVT_MRS_REF_WSIGNAL82	VFT_INT16
#define SVS_MRS_REF_WSIGNAL82	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 83 data.
*/

#define SVN_MRS_REF_WSIGNAL83	(SVN_BASE_PROCESSED_MRS + 232)
#define SVT_MRS_REF_WSIGNAL83	VFT_INT16
#define SVS_MRS_REF_WSIGNAL83	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 84 data.
*/

#define SVN_MRS_REF_WSIGNAL84	(SVN_BASE_PROCESSED_MRS + 233)
#define SVT_MRS_REF_WSIGNAL84	VFT_INT16
#define SVS_MRS_REF_WSIGNAL84	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 85 data.
*/

#define SVN_MRS_REF_WSIGNAL85	(SVN_BASE_PROCESSED_MRS + 234)
#define SVT_MRS_REF_WSIGNAL85	VFT_INT16
#define SVS_MRS_REF_WSIGNAL85	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 86 data.
*/

#define SVN_MRS_REF_WSIGNAL86	(SVN_BASE_PROCESSED_MRS + 235)
#define SVT_MRS_REF_WSIGNAL86	VFT_INT16
#define SVS_MRS_REF_WSIGNAL86	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 87 data.
*/

#define SVN_MRS_REF_WSIGNAL87	(SVN_BASE_PROCESSED_MRS + 236)
#define SVT_MRS_REF_WSIGNAL87	VFT_INT16
#define SVS_MRS_REF_WSIGNAL87	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 88 data.
*/

#define SVN_MRS_REF_WSIGNAL88	(SVN_BASE_PROCESSED_MRS + 237)
#define SVT_MRS_REF_WSIGNAL88	VFT_INT16
#define SVS_MRS_REF_WSIGNAL88	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 89 data.
*/

#define SVN_MRS_REF_WSIGNAL89	(SVN_BASE_PROCESSED_MRS + 238)
#define SVT_MRS_REF_WSIGNAL89	VFT_INT16
#define SVS_MRS_REF_WSIGNAL89	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 90 data.
*/

#define SVN_MRS_REF_WSIGNAL90	(SVN_BASE_PROCESSED_MRS + 239)
#define SVT_MRS_REF_WSIGNAL90	VFT_INT16
#define SVS_MRS_REF_WSIGNAL90	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 91 data.
*/

#define SVN_MRS_REF_WSIGNAL91	(SVN_BASE_PROCESSED_MRS + 240)
#define SVT_MRS_REF_WSIGNAL91	VFT_INT16
#define SVS_MRS_REF_WSIGNAL91	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 92 data.
*/

#define SVN_MRS_REF_WSIGNAL92	(SVN_BASE_PROCESSED_MRS + 241)
#define SVT_MRS_REF_WSIGNAL92	VFT_INT16
#define SVS_MRS_REF_WSIGNAL92	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 93 data.
*/

#define SVN_MRS_REF_WSIGNAL93	(SVN_BASE_PROCESSED_MRS + 242)
#define SVT_MRS_REF_WSIGNAL93	VFT_INT16
#define SVS_MRS_REF_WSIGNAL93	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 94 data.
*/

#define SVN_MRS_REF_WSIGNAL94	(SVN_BASE_PROCESSED_MRS + 243)
#define SVT_MRS_REF_WSIGNAL94	VFT_INT16
#define SVS_MRS_REF_WSIGNAL94	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 95 data.
*/

#define SVN_MRS_REF_WSIGNAL95	(SVN_BASE_PROCESSED_MRS + 244)
#define SVT_MRS_REF_WSIGNAL95	VFT_INT16
#define SVS_MRS_REF_WSIGNAL95	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 96 data.
*/

#define SVN_MRS_REF_WSIGNAL96	(SVN_BASE_PROCESSED_MRS + 245)
#define SVT_MRS_REF_WSIGNAL96	VFT_INT16
#define SVS_MRS_REF_WSIGNAL96	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 97 data.
*/

#define SVN_MRS_REF_WSIGNAL97	(SVN_BASE_PROCESSED_MRS + 246)
#define SVT_MRS_REF_WSIGNAL97	VFT_INT16
#define SVS_MRS_REF_WSIGNAL97	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 98 data.
*/

#define SVN_MRS_REF_WSIGNAL98	(SVN_BASE_PROCESSED_MRS + 247)
#define SVT_MRS_REF_WSIGNAL98	VFT_INT16
#define SVS_MRS_REF_WSIGNAL98	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 99 data.
*/

#define SVN_MRS_REF_WSIGNAL99	(SVN_BASE_PROCESSED_MRS + 248)
#define SVT_MRS_REF_WSIGNAL99	VFT_INT16
#define SVS_MRS_REF_WSIGNAL99	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 100 data.
*/

#define SVN_MRS_REF_WSIGNAL100	(SVN_BASE_PROCESSED_MRS + 249)
#define SVT_MRS_REF_WSIGNAL100	VFT_INT16
#define SVS_MRS_REF_WSIGNAL100	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 101 data.
*/

#define SVN_MRS_REF_WSIGNAL101	(SVN_BASE_PROCESSED_MRS + 250)
#define SVT_MRS_REF_WSIGNAL101	VFT_INT16
#define SVS_MRS_REF_WSIGNAL101	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 102 data.
*/

#define SVN_MRS_REF_WSIGNAL102	(SVN_BASE_PROCESSED_MRS + 251)
#define SVT_MRS_REF_WSIGNAL102	VFT_INT16
#define SVS_MRS_REF_WSIGNAL102	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 103 data.
*/

#define SVN_MRS_REF_WSIGNAL103	(SVN_BASE_PROCESSED_MRS + 252)
#define SVT_MRS_REF_WSIGNAL103	VFT_INT16
#define SVS_MRS_REF_WSIGNAL103	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 104 data.
*/

#define SVN_MRS_REF_WSIGNAL104	(SVN_BASE_PROCESSED_MRS + 253)
#define SVT_MRS_REF_WSIGNAL104	VFT_INT16
#define SVS_MRS_REF_WSIGNAL104	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 105 data.
*/

#define SVN_MRS_REF_WSIGNAL105	(SVN_BASE_PROCESSED_MRS + 254)
#define SVT_MRS_REF_WSIGNAL105	VFT_INT16
#define SVS_MRS_REF_WSIGNAL105	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 106 data.
*/

#define SVN_MRS_REF_WSIGNAL106	(SVN_BASE_PROCESSED_MRS + 255)
#define SVT_MRS_REF_WSIGNAL106	VFT_INT16
#define SVS_MRS_REF_WSIGNAL106	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 107 data.
*/

#define SVN_MRS_REF_WSIGNAL107	(SVN_BASE_PROCESSED_MRS + 256)
#define SVT_MRS_REF_WSIGNAL107	VFT_INT16
#define SVS_MRS_REF_WSIGNAL107	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 108 data.
*/

#define SVN_MRS_REF_WSIGNAL108	(SVN_BASE_PROCESSED_MRS + 257)
#define SVT_MRS_REF_WSIGNAL108	VFT_INT16
#define SVS_MRS_REF_WSIGNAL108	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 109 data.
*/

#define SVN_MRS_REF_WSIGNAL109	(SVN_BASE_PROCESSED_MRS + 258)
#define SVT_MRS_REF_WSIGNAL109	VFT_INT16
#define SVS_MRS_REF_WSIGNAL109	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 110 data.
*/

#define SVN_MRS_REF_WSIGNAL110	(SVN_BASE_PROCESSED_MRS + 259)
#define SVT_MRS_REF_WSIGNAL110	VFT_INT16
#define SVS_MRS_REF_WSIGNAL110	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 111 data.
*/

#define SVN_MRS_REF_WSIGNAL111	(SVN_BASE_PROCESSED_MRS + 260)
#define SVT_MRS_REF_WSIGNAL111	VFT_INT16
#define SVS_MRS_REF_WSIGNAL111	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 112 data.
*/

#define SVN_MRS_REF_WSIGNAL112	(SVN_BASE_PROCESSED_MRS + 261)
#define SVT_MRS_REF_WSIGNAL112	VFT_INT16
#define SVS_MRS_REF_WSIGNAL112	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 113 data.
*/

#define SVN_MRS_REF_WSIGNAL113	(SVN_BASE_PROCESSED_MRS + 262)
#define SVT_MRS_REF_WSIGNAL113	VFT_INT16
#define SVS_MRS_REF_WSIGNAL113	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 114 data.
*/

#define SVN_MRS_REF_WSIGNAL114	(SVN_BASE_PROCESSED_MRS + 263)
#define SVT_MRS_REF_WSIGNAL114	VFT_INT16
#define SVS_MRS_REF_WSIGNAL114	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 115 data.
*/

#define SVN_MRS_REF_WSIGNAL115	(SVN_BASE_PROCESSED_MRS + 264)
#define SVT_MRS_REF_WSIGNAL115	VFT_INT16
#define SVS_MRS_REF_WSIGNAL115	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 116 data.
*/

#define SVN_MRS_REF_WSIGNAL116	(SVN_BASE_PROCESSED_MRS + 265)
#define SVT_MRS_REF_WSIGNAL116	VFT_INT16
#define SVS_MRS_REF_WSIGNAL116	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 117 data.
*/

#define SVN_MRS_REF_WSIGNAL117	(SVN_BASE_PROCESSED_MRS + 266)
#define SVT_MRS_REF_WSIGNAL117	VFT_INT16
#define SVS_MRS_REF_WSIGNAL117	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 118 data.
*/

#define SVN_MRS_REF_WSIGNAL118	(SVN_BASE_PROCESSED_MRS + 267)
#define SVT_MRS_REF_WSIGNAL118	VFT_INT16
#define SVS_MRS_REF_WSIGNAL118	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 119 data.
*/

#define SVN_MRS_REF_WSIGNAL119	(SVN_BASE_PROCESSED_MRS + 268)
#define SVT_MRS_REF_WSIGNAL119	VFT_INT16
#define SVS_MRS_REF_WSIGNAL119	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 120 data.
*/

#define SVN_MRS_REF_WSIGNAL120	(SVN_BASE_PROCESSED_MRS + 269)
#define SVT_MRS_REF_WSIGNAL120	VFT_INT16
#define SVS_MRS_REF_WSIGNAL120	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 121 data.
*/

#define SVN_MRS_REF_WSIGNAL121	(SVN_BASE_PROCESSED_MRS + 270)
#define SVT_MRS_REF_WSIGNAL121	VFT_INT16
#define SVS_MRS_REF_WSIGNAL121	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 122 data.
*/

#define SVN_MRS_REF_WSIGNAL122	(SVN_BASE_PROCESSED_MRS + 271)
#define SVT_MRS_REF_WSIGNAL122	VFT_INT16
#define SVS_MRS_REF_WSIGNAL122	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 123 data.
*/

#define SVN_MRS_REF_WSIGNAL123	(SVN_BASE_PROCESSED_MRS + 272)
#define SVT_MRS_REF_WSIGNAL123	VFT_INT16
#define SVS_MRS_REF_WSIGNAL123	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 124 data.
*/

#define SVN_MRS_REF_WSIGNAL124	(SVN_BASE_PROCESSED_MRS + 273)
#define SVT_MRS_REF_WSIGNAL124	VFT_INT16
#define SVS_MRS_REF_WSIGNAL124	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 125 data.
*/

#define SVN_MRS_REF_WSIGNAL125	(SVN_BASE_PROCESSED_MRS + 274)
#define SVT_MRS_REF_WSIGNAL125	VFT_INT16
#define SVS_MRS_REF_WSIGNAL125	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 126 data.
*/

#define SVN_MRS_REF_WSIGNAL126	(SVN_BASE_PROCESSED_MRS + 275)
#define SVT_MRS_REF_WSIGNAL126	VFT_INT16
#define SVS_MRS_REF_WSIGNAL126	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 127 data.
*/

#define SVN_MRS_REF_WSIGNAL127	(SVN_BASE_PROCESSED_MRS + 276)
#define SVT_MRS_REF_WSIGNAL127	VFT_INT16
#define SVS_MRS_REF_WSIGNAL127	"MRS Ref Water signal intensity"
/*   
    The water signal intensity value from Ref scan.
    Channel 128 data.
*/

#define SVN_MRS_GEN_DATA_TYPE	(SVN_BASE_PROCESSED_MRS + 277)
#define SVT_MRS_GEN_DATA_TYPE	VFT_INT32
#define SVS_MRS_GEN_DATA_TYPE	"MRS gen data type"
/*   
    The type of data contained in a SVN_GEN_DATA varfield.
    This is copy from SVN_GEN_DATA_TYPE of subprotocol node.
*/

/*****************************************************************************
  ECHO name space
*/

#define SVN_BASE_ECHO_MRS	SVN_MIN_ECHO+SVN_OFF_MRS



/*****************************************************************************
  POSITION name space
*/

#define SVN_BASE_POSITION_MRS	SVN_MIN_POSITION+SVN_OFF_MRS

#define SVN_MRS_SLICE_NUMBER	(SVN_BASE_POSITION_MRS + 0)
#define SVT_MRS_SLICE_NUMBER	VFT_INT32
#define SVS_MRS_SLICE_NUMBER	"MRS slice number"
/*   
    ３ＤＭＲＳスライス番号
*/

#define SVN_MRS_LOCATOR_POS_NO	(SVN_BASE_POSITION_MRS + 1)
#define SVT_MRS_LOCATOR_POS_NO	VFT_INT32
#define SVS_MRS_LOCATOR_POS_NO	"MRS locator image position number"
/*   
    ３ＤＭＲＳ参照画像positionNode番号
*/

#define SVN_MRS_METABO_NAME	(SVN_BASE_POSITION_MRS + 2)
#define SVT_MRS_METABO_NAME	VFT_STRING
#define SVS_MRS_METABO_NAME	"MRS metabo name"
/*   
    ＭＲＳＣＳＩ代謝物名称
*/

/*****************************************************************************
  VISUALGROUP name space
*/

#define SVN_BASE_VISUALGROUP_MRS	SVN_MIN_VISUALGROUP+SVN_OFF_MRS



#endif /* tami_vfStudy_vfs_mrs_H__ */
