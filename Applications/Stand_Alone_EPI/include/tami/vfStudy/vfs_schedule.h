/******************************************************************************
 *  
 *    File Overview:
 *	This file contains the varfield names for the SCHEDULE subspace
 *	of the study file hierarchy.  For more information on the study
 *	hierarchy, read "VarFields Implementation of Patient Data" by
 *	George Mattinger.
 *
 *	The SCHEDULE subspace contains fields associated with scheduling a
 *	patient for scanning.
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
 * 92-08-06     donb            Defined most patient info node values.
 * 92-06-09	scottm		Created
 * End of History */



#ifndef tami_vfStudy_vfs_schedule_H__
#define tami_vfStudy_vfs_schedule_H__

#ifndef tami_vfStudy_vfs_structure_H__
#include <tami/vfStudy/vfs_structure.h>
#endif /* tami_vfStudy_vfs_structure_H__ */



/*****************************************************************************
  INDEPENDENT name space
*/




/*****************************************************************************
  STUDY name space
*/
/* These correspond to parameters written through the Patient Data Entry
   Tool.  Unless otherwise commented, the field is written by PDET. */
   
#define SVN_SCH_STUDY_ID	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 0)
#define SVT_SCH_STUDY_ID	VFT_SCHED_STUDY_ID
#define SVS_SCH_STUDY_ID	"Study ID"
/*	スタディＩＤ。Run No. とサイトＩＤから構成される。スタディファイル名も
	このスタディＩＤから作成される。
		"Run%d.%d",run_num,site_id
	ＰＤＥＴが SVN_ST_STUDY に記述する。

	This is the name by which the study will be identified in the
	Patient Database and study file.  A structure defined by two numbers,
	it is assigned by PDET (based on last assigned run number and site ID)
	and may be used by anybody.  Written by PDET.
	*/
	
#define SVN_SCH_PATIENT_ID      (SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  1)
#define SVT_SCH_PATIENT_ID      VFT_XMSTRING
#define SVS_SCH_PATIENT_ID      "Patient ID"
/*	患者ＩＤ。システムとしてこのパラメータには何ら規定を設けていない。
	サイトの運用による。ＰＤＥＴが SVN_ST_STUDY に記述する。

	This is a unique identifier for each patient in the Patient
	Database.  Its use may vary from site to site but a typical
	US site might use Social Security Number.  Entered by Operator
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_LAST_NAME       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  2)
#define SVT_SCH_LAST_NAME       VFT_XMSTRING
#define SVS_SCH_LAST_NAME       "Last Name"
/*	患者姓。日本語版では漢字名がはいる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

	Patient's last name.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	 */

#define SVN_SCH_FIRST_NAME      (SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  3)
#define SVT_SCH_FIRST_NAME      VFT_XMSTRING
#define SVS_SCH_FIRST_NAME      "First Name"
/*	患者名。日本語版では読み（Ascii 大文字、ヘボン式ローマ字）が入る。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

	Patient's first name.  Typically assigned by parents.  Entered by
	Operator and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_ADDRESS		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  4)
#define SVT_SCH_ADDRESS		VFT_XMSTRING
#define SVS_SCH_ADDRESS         "Address"
/*	患者住所。日本語版（V2.0 以降）では使用せず。

	Patient's address.  Entered by Operator and written to the Patient
	Database and study file by PDET.
	*/


#define SVN_SCH_PHONE_NUM	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  5)
#define SVT_SCH_PHONE_NUM	VFT_XMSTRING
#define SVS_SCH_PHONE_NUM       "Phone #"
/*	患者電話番号。日本語版（V2.0 以降）では使用せず。

        Patient's phone number.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_DOB		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  6)
#define SVT_SCH_DOB		VFT_TM
#define SVS_SCH_DOB		"Date of Birth"
/*	患者生年月日。ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's date of birth.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_GENDER		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  7)
#define SVT_SCH_GENDER		VFT_INT
#define SVS_SCH_GENDER		"Gender"
/*	患者性別。０：男性、１：女性。ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's gender.  Chosen by Operator from an option menu and
	written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_HEIGHT		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  8)
#define SVT_SCH_HEIGHT		VFT_FLOAT
#define SVS_SCH_HEIGHT		"Height"
/*	患者身長。日本語版（V2.0 以降）では使用せず。

        Patient's height, measured in meters.  Entered by Operator and
	written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_WEIGHT		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE +  9)
#define SVT_SCH_WEIGHT		VFT_FLOAT
#define SVS_SCH_WEIGHT		"Weight"
/*	患者体重。単位は kg。ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's weight in kilograms.  Entered by Operator and
	written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_PAT_COMMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 10)
#define SVT_SCH_PAT_COMMENT	VFT_XMSTRING
#define SVS_SCH_PAT_COMMENT	"Patient Comment"
/*	患者コメント。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient comment.  Entered by Operator and written to the
	written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_RADIOLOGIST	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 11)
#define SVT_SCH_RADIOLOGIST	VFT_XMSTRING
#define SVS_SCH_RADIOLOGIST	"Radiologist"
/*	放射線科医師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's radiologist.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_PHYSICIAN	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 12)
#define SVT_SCH_PHYSICIAN	VFT_XMSTRING
#define SVS_SCH_PHYSICIAN       "Referring Physician"
/*	依頼科医師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's physician.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_DEPARTMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 13)
#define SVT_SCH_DEPARTMENT	VFT_XMSTRING
#define SVS_SCH_DEPARTMENT	"Department"
/*	依頼科名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Imaging department.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_OPERATOR	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 14)
#define SVT_SCH_OPERATOR	VFT_XMSTRING
#define SVS_SCH_OPERATOR	"Operator"
/*	放射線科技師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Scan operator.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_SCAN_DATE_TIME	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 15)
#define SVT_SCH_SCAN_DATE_TIME	VFT_TIME
#define SVS_SCH_SCAN_DATE_TIME	"Scan Date/Time"
/*	予約収集時刻。収集後は収集時刻となる。予約はＰＤＥＴが
	 SVN_ST_STUDY に記述し、収集時刻はＰＱＭが記述する。

	Scan date and time.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_CONTRAST	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 16)
#define SVT_SCH_CONTRAST	VFT_XMSTRING
#define SVS_SCH_CONTRAST	"Contrast Agent"
/*	造影剤種。V2.0 以降では造影剤種は SVN_ST_PROTOCOL ノードに
	ＰＱＭが記述する。

	Scan contrast agent.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_STUDY_COMMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 17)
#define SVT_SCH_STUDY_COMMENT	VFT_XMSTRING
#define SVS_SCH_STUDY_COMMENT	"Study Comment"
/*	検査コメント。日本語版では使用せず。

	Study comment.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_ARCHIVE_VOL	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 18)
#define SVT_SCH_ARCHIVE_VOL	VFT_XMSTRING
#define SVS_SCH_ARCHIVE_VOL	"Archival Volume"
/*	保存媒体。ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's archival volume.  Written to the Patient Database
	and study file by Archive.
	*/

#define SVN_SCH_ARCHIVE_DATE	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 19)
#define SVT_SCH_ARCHIVE_DATE	VFT_TIME
#define SVS_SCH_ARCHIVE_DATE	"Archival Date"
/*	保存日。ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's archival date.  Written to the Patient Database and
	study file by Archive.
	*/

#define SVN_SCH_LANGUAGE        (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 20)
#define SVT_SCH_LANGUAGE        VFT_INT
#define SVS_SCH_LANGUAGE        "Language"
/*	言語コード。ＰＤＥＴが SVN_ST_STUDY に記述する。

	Language of the site where the scan took place.  Written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_SYSTEM_ID       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 21)
#define SVT_SCH_SYSTEM_ID       VFT_INT
#define SVS_SCH_SYSTEM_ID       "System ID"
/*	システムＩＤ。ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's original system ID.  Written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_SITE_NAME       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 22)
#define SVT_SCH_SITE_NAME       VFT_XMSTRING
#define SVS_SCH_SITE_NAME       "Site Name"
/*	サイト名。 ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's original site name.  Written to the study file
	by PDET.
	*/

#define SVN_SCH_RELEASE_NUM     (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 23)
#define SVT_SCH_RELEASE_NUM     VFT_STRING
#define SVS_SCH_RELEASE_NUM     "Release Number"
/*	リリース番号。ＰＤＥＴが SVN_ST_STUDY に記述する。

	The release number for the study origin site's system.  Written
	to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_SITE_ID       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 24)
#define SVT_SCH_SITE_ID       VFT_STRING
#define SVS_SCH_SITE_ID       "Site ID"
/*      The study's original site ID.  Written to the
	Patient Database and study file by PDET.
        */

#define SVN_SCH_FIELD_STRENGTH		(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 25)
#define SVT_SCH_FIELD_STRENGTH		VFT_STRING
#define SVS_SCH_FIELD_STRENGTH		"Magnet field strength"
/*      Field strength of the magnet
 */

#define SVN_SCH_SCAN_AGE	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 26)
#define SVT_SCH_SCAN_AGE	VFT_STRING
#define SVS_SCH_SCAN_AGE	"Scan Age"
/*	撮影時年齢
	１歳以上は年齢のみで１歳未満は月齢、日齢を表す。
	１カ月～１１カ月	1m ～ 11m	(月齢)
	１カ月未満		1d ～ 30d	(日齢)
	*/

#define SVN_SCH_N_PATIENT_ID      (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 27)
#define SVT_SCH_N_PATIENT_ID      VFT_STRING
#define SVS_SCH_N_PATIENT_ID      "Patient ID"
/*	患者ＩＤ。システムとしてこのパラメータには何ら規定を設けていない。
	サイトの運用による。ＰＤＥＴが SVN_ST_STUDY に記述する。

	This is a unique identifier for each patient in the Patient
	Database.  Its use may vary from site to site but a typical
	US site might use Social Security Number.  Entered by Operator
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_LAST_NAME       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 28)
#define SVT_SCH_N_LAST_NAME       VFT_STRING
#define SVS_SCH_N_LAST_NAME       "Last Name"
/*	患者姓。日本語版では漢字名がはいる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

	Patient's last name.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	 */

#define SVN_SCH_N_FIRST_NAME      (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 29)
#define SVT_SCH_N_FIRST_NAME      VFT_STRING
#define SVS_SCH_N_FIRST_NAME      "First Name"
/*	患者名。日本語版では読み（Ascii 大文字、ヘボン式ローマ字）が入る。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

	Patient's first name.  Typically assigned by parents.  Entered by
	Operator and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_ADDRESS	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 30)
#define SVT_SCH_N_ADDRESS	VFT_STRING
#define SVS_SCH_N_ADDRESS	"Address"
/*	患者住所。日本語版（V2.0 以降）では使用せず。

	Patient's address.  Entered by Operator and written to the Patient
	Database and study file by PDET.
	*/


#define SVN_SCH_N_PHONE_NUM	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 31)
#define SVT_SCH_N_PHONE_NUM	VFT_STRING
#define SVS_SCH_N_PHONE_NUM       "Phone #"
/*	患者電話番号。日本語版（V2.0 以降）では使用せず。

        Patient's phone number.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_PAT_COMMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 32)
#define SVT_SCH_N_PAT_COMMENT	VFT_STRING
#define SVS_SCH_N_PAT_COMMENT	"Patient Comment"
/*	患者コメント。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient comment.  Entered by Operator and written to the
	written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_RADIOLOGIST	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 33)
#define SVT_SCH_N_RADIOLOGIST	VFT_STRING
#define SVS_SCH_N_RADIOLOGIST	"Radiologist"
/*	放射線科医師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's radiologist.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_PHYSICIAN	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 34)
#define SVT_SCH_N_PHYSICIAN	VFT_STRING
#define SVS_SCH_N_PHYSICIAN       "Referring Physician"
/*	依頼科医師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Patient's physician.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_DEPARTMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 35)
#define SVT_SCH_N_DEPARTMENT	VFT_STRING
#define SVS_SCH_N_DEPARTMENT	"Department"
/*	依頼科名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Imaging department.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_OPERATOR	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 36)
#define SVT_SCH_N_OPERATOR	VFT_STRING
#define SVS_SCH_N_OPERATOR	"Operator"
/*	放射線科技師名。オプションメニューからの選択となる。
	ＰＤＥＴが SVN_ST_STUDY に記述する。

        Scan operator.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_CONTRAST	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 37)
#define SVT_SCH_N_CONTRAST	VFT_STRING
#define SVS_SCH_N_CONTRAST	"Contrast Agent"
/*	造影剤種。V2.0 以降では造影剤種は SVN_ST_PROTOCOL ノードに
	ＰＱＭが記述する。

	Scan contrast agent.  Chosen by Operator from an option menu
	and written to the Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_STUDY_COMMENT	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 38)
#define SVT_SCH_N_STUDY_COMMENT	VFT_STRING
#define SVS_SCH_N_STUDY_COMMENT	"Study Comment"
/*	検査コメント。日本語版では使用せず。

	Study comment.  Entered by Operator and written to the
	Patient Database and study file by PDET.
	*/

#define SVN_SCH_N_ARCHIVE_VOL	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 39)
#define SVT_SCH_N_ARCHIVE_VOL	VFT_STRING
#define SVS_SCH_N_ARCHIVE_VOL	"Archival Volume"
/*	保存媒体。ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's archival volume.  Written to the Patient Database
	and study file by Archive.
	*/

#define SVN_SCH_N_SITE_NAME       (SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 40)
#define SVT_SCH_N_SITE_NAME       VFT_STRING
#define SVS_SCH_N_SITE_NAME       "Site Name"
/*	サイト名。 ＰＤＥＴが SVN_ST_STUDY に記述する。

	The study's original site name.  Written to the study file
	by PDET.
	*/

#define SVN_SCH_PATIENT_SUB_ID	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 41)
#define SVT_SCH_PATIENT_SUB_ID	VFT_STRING
#define SVS_SCH_PATIENT_SUB_ID	"Patient Sub ID"
/*	患者サブID。ＰＤＥＴが SVN_ST_STUDY に記述する。
	サイト毎の設定で患者に付加的なIDをつけることができる。
	画像付帯情報。
	*/

#define SVN_SCH_N_SCAN_DATE_TIME	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 42)
#define SVT_SCH_N_SCAN_DATE_TIME	VFT_LOCAL_TIME
#define SVS_SCH_N_SCAN_DATE_TIME	"Scan Date/Time"
/*	Scan date and time.  Entered by Operator and written to the
	Patient Database and study file by PDET.

	設定者：PDET,PQM
	使用者：PDET,PQM,FILEMAN,DICOM,INNERVISION
	*/

#define SVN_SCH_N_ARCHIVE_DATE	(SVN_MIN_STUDY+SVN_OFF_SCHEDULE + 43)
#define SVT_SCH_N_ARCHIVE_DATE	VFT_LOCAL_TIME
#define SVS_SCH_N_ARCHIVE_DATE	"Archival Date"
/*	The study's archival date.  Written to the Patient Database and
	study file by Archive.

	設定者：PDET
	使用者：PDET,RECON,DISPLAY,FILEMAN,DICOM
	*/

/*****************************************************************************
  PROTOCOL name space
*/




/*****************************************************************************
  SUBPROTOCOL name space
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
#define SVN_SCH_NETWORK_DATE	(SVN_MIN_POSITION+SVN_OFF_SCHEDULE + 0)
#define SVT_SCH_NETWORK_DATE	VFT_TIME
#define SVS_SCH_NETWORK_DATE	"online transfer date & time"
/*	online転送された画像データに付けられる。
        オートアーカイブ時に転送済みかどうかの判断を行う。
	*/

#define SVN_SCH_N_NETWORK_DATE	(SVN_MIN_POSITION+SVN_OFF_SCHEDULE + 1)
#define SVT_SCH_N_NETWORK_DATE	VFT_LOCAL_TIME
#define SVS_SCH_N_NETWORK_DATE	"Network Date"
/*	online転送された画像データに付けられる。
        オートアーカイブ時に転送済みかどうかの判断を行う。

        設定者：FILEMAN
        使用者：RECON,DISPLAY,FILEMAN,DICOM
	*/

/*****************************************************************************
  IMCALC name space
*/




#endif /* tami_vfStudy_vfs_schedule_H__ */
