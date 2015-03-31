//Patni-MP/2009June07/Added/ACE-2/Post Processing
#define	PROMPTERSCRIPT_MAXSIZE	4096

#define	RECON_SCRIPT		"1 RECON"
#define	POSTPROC_SCRIPT		"2 POSTPROC 1"
#define	BACKGROUND_SCRIPT2	"2 BACKGROUND 1"
#define	BACKGROUND_SCRIPT3	"3 BACKGROUND 1 2"
#define	BACKGROUND_SCRIPT4	"4 BACKGROUND 1 2"
#define	EZSCAN_END		"99 EZSCAN"
#define	SCRIPT_END		" END"

#define	SCANAUTO_FILE		"fileman/ScanAuto.preset"
/* #define	SCANAUTO_FILE_FORM	"BG-%s"	*/
#define	SCANAUTO_FILE_FORM	"%s"

/*#define	NORMAL_MODE		"BG-MODE: normal"
#define	EZSCAN_MODE		"BG-MODE: EZscan"
#define	AUTO_FILM1		"2 FILM 1"
#define	AUTO_FILM2		"3 FILM 1 2"
#define	FILM_MODE		"FILM-MODE:AUTO"
/*
#define	AUTO_FILM		"BG-FILM: AUTO"
#define	MANUAL_FILM		"BG-FILM: MANUAL"
*/
//Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
#define	AUTOFILM		(PqmGlobal.autoFilm == TRUE)


/* 再構成で自動的に連動処理を行うかチェックするプロセス
   Usage: checkPostProc <studyName> <protocol>
   シーケンス依存の自動処理がある場合TRUEを返します。
   現状ではTOF3Dの首つなぎとFISP (V5.00)
 */
#define	MS_POSTPROC_CHECK	"checkPostProc"