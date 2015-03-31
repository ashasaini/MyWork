/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/coil.h,v $  ($Revision: 1.3 $)
 *  Last Modified:   $Date: 2010/07/23 04:53:32 $
 *  Revision Log:    $Log: coil.h,v $
 *  Revision Log:    Revision 1.3  2010/07/23 04:53:32  tatsuya
 *  Revision Log:    MCM0262-00124
 *  Revision Log:    Added define for RWB
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.4.2.3  2008/12/25 08:16:55  hanaki
 *  Revision Log:    MCM0229-00054 comment
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.4.2.2  2008/12/24 04:54:05  hanaki
 *  Revision Log:    MCM0229-00054 header
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.4.2.1  2008/04/25 10:30:52  hanaki
 *  Revision Log:    MCM0213-00066 include
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.4  2006/09/15 11:59:26  koku
 *  Revision Log:    MCM0177-00110 : GPDB for V9.00
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.3  2006/08/03 11:43:20  matsuo
 *  Revision Log:    MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7.54.2  2006/07/20 10:50:55  koku
 *  Revision Log:    MCM0177-00110  :  GPDB for V9.00
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.7  2004/02/02 07:20:08  pc_yutaka
 *  Revision Log:    MCM0102-00229
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.6  2004/01/24 05:10:49  pc_yutaka
 *  Revision Log:    MCM0102-00037   Speeder 2D enable Define
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.5  2004/01/24 05:04:20  pc_yutaka
 *  Revision Log:    MCM0102-00037   Speeder 2D enable Define
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.4  2003/05/12 08:56:52  pc_yutaka
 *  Revision Log:    for INTENSITY
 *  Revision Log:
 *  Revision Log:    Revision 1.7.12.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.7  2002/08/07 07:55:58  purandar
 *  Revision Log:    Merging new code with ported code
 *  Revision Log:
 *  Revision Log:    Revision 1.7  2002/05/22 05:58:06  yutaka
 *  Revision Log:    reviced WholeBodyCoil
 *  Revision Log:
 *  Revision Log:    Revision 1.6  2002/04/30 08:51:48  yutaka
 *  Revision Log:    add. GetRxCoilInfo
 *  Revision Log:
 *  Revision Log:    Revision 1.5  2002/04/30 08:38:41  yutaka
 *  Revision Log:    Receiver Gain Overflow
 *  Revision Log:
 *  Revision Log:    Revision 1.4  2002/01/17 11:25:25  yutaka
 *  Revision Log:    for 8ch PAC
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2001/02/07 04:31:47  full
 *  Revision Log:    PathDB 用の修正。mview の上位ビットの判定を従来の８ビットから
 *  Revision Log:    ７ビットに変更。
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2000/08/22 06:33:45  full
 *  Revision Log:    Add Coil Limit for Couch
 *  Revision Log:
 *  Revision Log:    Revision 1.1  2000/05/17 09:03:15  tel
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *
 *    File Overview:
 *	This file defines the public header for the API to the Coil Database.
 *
 *********************************************************************/
/* Revision History:
 * Oct.16.92	MT			Created
 * Dec. 9.94	Naoyuki FURUDATE	Modified
 * End of History */

/* Revision History:
 * Jul.21.95	Masakatsu OGIWARA	Modified
 * Added	channels_t added & coil_tmethod_t added
 * Aug.25.95	Hisashi Tsurumaki	Modified
 * Added	added CDB_CHANNEL_W to cdb_channels_e
 * Sep.02.99	Hisashi Tsurumaki	Modified
 * Added        CDB_PAC_PATTERN, CDB_PAC_ASSIGN
 * Jan.12.2002  福島 豊                 Modified
 *              Coilエレメントの16ch対応の為にMViewのBit定義変更
 * Apr.27.2002  福島 豊                 Modified
 *              RGNのOverflow対応
 *
 * Jun.27.2006  K.Kokubun               Added for SATURN(V9.00)
 * Apr.18.2008  S.Hanaki                耐荷重情報追加(V9.26)
 * Dec.08.2008  S.Hanaki                コイルサイズ情報追加(V9.50)
 *
 * End of History */



#ifndef libgpdb_coil_H__
#define libgpdb_coil_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define CDB_DB_NAME	"coilDB"

/* data-structures and data-types */

typedef void 	*cdb_handle_p;
#ifdef __FREE_BSD__
typedef uint64_t cdb_db_key_t;
#else
typedef int32_t  cdb_db_key_t;
#endif

typedef enum cdb_keys_e {
    CDB_KEY_TYPE        = 1,
    CDB_KEY_INSTALLED,
    CDB_KEY_IMPEDANCE,
    CDB_KEY_CHANNEL,
    CDB_KEY_VIEW,
    CDB_KEY_ID,
    CDB_KEY_DESCR,
    CDB_KEY_PORT,
    CDB_KEY_TUNE_METHOD,
    CDB_KEY_THRESH1,
    CDB_KEY_THRESH2,
    CDB_KEY_NEAR,
    CDB_KEY_COARSE_LIM,
    CDB_KEY_FINE_LIM,
    CDB_KEY_SORT_ORDER,

    /*---------------------
     * Added for V9.00
     */
    CDB_KEY_COMB,
    CDB_KEY_GID,
    CDB_KEY_NSC,
    CDB_KEY_PD,
    CDB_KEY_ECB,
    CDB_KEY_WPZ,
    CDB_KEY_POS4,
    CDB_KEY_CDS,
    CDB_KEY_SDB,
    CDB_KEY_CL,
    CDB_KEY_MBF,
    CDB_KEY_WEIGHT,				/* Added for V9.26 */
    CDB_KEY_SIZE,				/* Added for V9.50 */
    CDB_KEY_SNAME

    } cdb_keys_t;

typedef enum cdb_on_off_e {
    CDB_OFF,
    CDB_ON
    } cdb_on_off_t;

typedef enum cdb_channels_e {
    CDB_CHANNEL_A = 'A',
    CDB_CHANNEL_B = 'B',
    CDB_CHANNEL_C = 'C',
    CDB_CHANNEL_L = 'L',
    CDB_CHANNEL_M = 'M',
    CDB_CHANNEL_W = 'W',		/* '95.08.25. Added */
    CDB_CHANNEL_X = 'X',		/* '95.07.21. insert */
    CDB_CHANNEL_Y = 'Y',		/* '95.07.21. insert */
    CDB_CHANNEL_Z = 'Z'			/* '95.07.21. insert */
    } cdb_channels_t;

typedef enum cdb_impedance_e {
    CDB_IMPEDANCE_LOW = CDB_OFF,
    CDB_IMPEDANCE_HIGH
    } cdb_impedance_t;

typedef enum cdb_coil_type_e {
    CDB_TRANSMIT = 1,
    CDB_RECEIVE,
    CDB_TRANSMIT_N_RCV,
    CDB_TRANSMIT_R_WB_TRANSMIT,    
    } cdb_coil_type_t;

typedef enum cdb_coil_tmethod_e {	/* tuning methods */
    CDB_VARCAP = 1,
    CDB_RTU,
    CDB_INITIAL,			/* '95.07.21. insert */
    CDB_NONE				/* '95.07.21. insert */
    } cdb_coil_tmethod_t;

typedef enum cdb_coil_port_e {
    CDB_PORT_A = 'A',
    CDB_PORT_B = 'B',
    CDB_PORT_C = 'C',			/* not used       */
    CDB_PORT_D = 'D',
    CDB_PORT_E = 'E',
    CDB_PORT_F = 'F',
    CDB_PORT_G = 'G'              /* Jun.27.2006 added for SATURN */
    } cdb_coil_port_t;

typedef enum cdb_mode_e {
    CDB_MODE_INSERT = 1,
    CDB_MODE_REPLACE
    } cdb_mode_t;

typedef char *cdb_descr_t;

typedef struct cdb_rec_s *cdb_rec_p;
typedef struct cdb_rec_s {           /* content of a record in the coil DB */
    int			coil_id;        /* coil id is also key to record   */
    cdb_impedance_t	impedance;      /* impedance type                  */
    cdb_on_off_t	is_installed;   /* is it installed ?               */
    int			mview;          /* 1 = single; > 1 = muiti. segment*/
    cdb_channels_t	channel;        /* channel used                    */ 
    cdb_coil_type_t	type;
    cdb_coil_port_t	port;

  /* Added for Saturn(V9.00)
   * Jun.27.2006 K.Kokubu
   */
  int                combine;           /* Combine flag        */
  int                groupId[4] ;           /* group ids           */
  int                numSCoil;          /* Number of same coil */
  int                portDef;           /* port define         */
  int                errCheckBit;       /* error check bit     */

    cdb_coil_tmethod_t	tune;           /* tuning method                   */
    int			threshold_1;
    int			threshold_2;
#ifdef __NUTC__
    int         nearer;
#else
#ifdef __WNP_NOT_NUTC__
    int         nearer;
#else
    int                 near;
#endif
#endif
    float		coarse_limit;
    float		fine_limit;
    int			retry;


    /* Added by Nao 1994.12.07 */
    int			tx_gain;	/* for F0 */
    int			scaler;		/* for F0 */

    /* Added by Nao 1994.12.16 */
    int			apc_start;	/* for APC (txGain: dB) */

    /* Added by Furudate 2000.08.21 */
    int			outSideLimit;	/* mm */
    int			inSideLimit;	/* mm */
    int			tuneLimit;	/* mm */
    int			rflLimit;	/* mm */

    /* Added by Fukushima 2001.08.07  for SPEEDER ph1(V4.5) */
    int			speeder_enable; /* for SPEEDER */

    int			receiveWB_enable;	/* Receive by WholeBody coil */

    int                 RxOverflowX;    /* RGN Overflow X */
    int                 RxOverflowY;    /* RGN Overflow Y */
    int                 RxOverflowZ;    /* RGN Overflow Z */

  /*-----------------------------
   * Added for Saturn(V9.00)
   *       Jun.27.2006 K.Kokubun
   */
  int                patient_orient;    /* Limit of patient orientation */
  int                warPosZ;           /* Warning position Z-Axis      */
  int                pos4[4];           /* position1,position2..        */
  int                coilDetect;        /* flag of Coil Detect scan     */
  int                sectionDB;         /* flag of SectionDB            */
  int                coilLocation;      /* information of Coil Location */
  int                movingBed;         /* flag of Moving Bed scan      */
  int                maxChannel;        /* Max. channel of coil         */
  int                weight;   			/* 耐荷重 V9.26 hana */
  int                size;   			/* コイルサイズ[mm] V9.50 CDS */
  char               shortName[4];      /* Short Name                   */

    int			sort_order;     /* field to sort list of coils by  */
    int			len_of_descr;
    cdb_descr_t		description;    /* uninterpreted description stream*/

} cdb_rec_t;

typedef struct cdb_site_s *cdb_site_p;  /* site specific coil parameters */
typedef struct cdb_site_s {
    int		i_offset;
    int		q_offset;
    int		i_gain;
    int		q_gain;
} cdb_site_t;

/* Error Severity Constants */

/* function prototypes */
cdb_handle_p	CdbOpen		(char *,char *,int);
/* full pasthname for coilDB.dbm
 * full pathname for coilDB.mcf
 * on of O_RDONLY, O_WRONLY or O_RDWR
 */

status_t	CdbClose	(cdb_handle_p);
status_t	CdbGet		(cdb_handle_p,void *,cdb_rec_p);
status_t	CdbGetAll	(cdb_handle_p,
				 void (*cdb_func) (void *,cdb_rec_p));
status_t	CdbSearch	(cdb_handle_p,
				 void (*cdb_func) (void *,cdb_rec_p),
				 ...);
status_t	CdbPut		(cdb_handle_p,cdb_mode_t,
				 void *,cdb_rec_p);
status_t	CdbPutSiteParm	(cdb_handle_p,cdb_site_p);
status_t	CdbGetSiteParm	(cdb_handle_p,cdb_site_p);
status_t	CdbDelete	(cdb_handle_p,void *);


cdb_handle_p CdbOpen(char *,      /* full pathname for coilDB.dbm       */
		     char *,      /* full pathname for coilDB.mcf       */
		     int );       /* on of O_RDONLY, O_WRONLY or O_RDWR */

status_t CdbClose( cdb_handle_p );

status_t CdbGet(cdb_handle_p,     /* opaque handle for databases     */
		void *,           /* the key to look for = coilID    */
		cdb_rec_p );      /* data returned for the given key */

status_t CdbGetSiteParm(cdb_handle_p,  /* handle for site file        */
			cdb_site_p );  /* returned site parameters    */

status_t CdbGetAll(cdb_handle_p,  /* opaque handle for databases     */
		   void (*cdb_func)(void *, cdb_rec_p)); /* function called
							    for eachh record */

status_t CdbSearch(cdb_handle_p,  /* opaque handle for databases     */
		   void (*cdb_func)(void *, cdb_rec_p),  /* function called
							    for	each record
							    matching criteria*/
		   ... );  /* (search-key,value) pair  ends with NULL */

status_t CdbPut(cdb_handle_p,     /* opaque handle for databases      */
		cdb_mode_t,       /* record insert or repalce mode    */
		void *,           /* key for record                   */
		cdb_rec_p );      /* if record with given key exists,
				     it is replaced otherwise a new 
				     record is added to databases     */

status_t CdbPutSiteParm(cdb_handle_p,  /* handle for site file        */
			cdb_site_p );  /* new site-specific coil parm */

status_t CdbDelete(cdb_handle_p, /* opqaue handle for databases       */
		   void * );     /* key to the record = coil ID       */

status_t        GetRxCoilInfo( 	int id ,	/* SVN_PLN_RECVR_COIL */
				int *val ) ;	/* RxOverflow[X,Y,Z]:Data[3] */


/* MACRO definitions */
	/* edit by 1995.07.21 */
#define CDB_REC_KEY(port, coil_id, channel) ( ((((port) << 8) | (channel)) << 16) | (coil_id) )
	/* edit by 1995.07.21 */
#define CDB_DECODE_KEY(cdb_rec_key, port, coil_id, channel) \
    (port)    = (cdb_rec_key) >> 24; \
    (channel) = (cdb_rec_key) >> 16 & 0x00ff; \
    (coil_id) = 0x0fff & (cdb_rec_key)

#define CDB_PAC_PATTERN(mview) ((mview) & 0x0000ffff) 
#define CDB_PAC_ASSIGN(mview)  (((mview) & 0x7fff0000) >> 16)

/* enable reviced WholeBodyCoil V5.00   */
#define	RWB_SPEEDER	0x00000001
#define	RWB_PREP	0x00000002
#define	RWB_SHIMMING	0x00000004
#define	RWB_CDS		0x00000008		/* V9.50 CDSスキャン用 */


#define	RWB_SATURN			0x00000010		/* Atlasコイル時使用・通常のWBコイルに切り替えて撮像する */
/* LargeKneeコイル追加に伴い、SPEEDERMAP撮像時に
   DynamicかInlineかをコイル毎に変更できるようになった。 */
#define	RWB_MAP_DYNAMIC		0x00000020		/* SPEEDERMAP撮像時、ダイナミック方式で撮像（<->インライン方式） */
#define	RWB_MAP_PAC_PAC		0x00000040		/* SPEEDERMAP撮像時、PAC-PAC方式で撮像（<->PAC-WB方式） */


/* MAP Enable details V6.00 */
#define	MAP_SPEEDER_CORR_ENABLE		(0x00000001)			/* SPeeder PE  */
#define	MAP_INTENSITY_ENABLE		((0x00000001) << 1 )	/* 輝度補正 */
#define	MAP_SPEEDER_SE_ENABLE		((0x00000001) << 2 )	/* SPeeder SE */

/* Limit of Patient orientation V9.00 */
#define    PATORI_FEET_FIRST_PRONE     0x0001
#define    PATORI_FEET_FIRST_SUPINE    0x0002
#define    PATORI_FEET_FIRST_LEFT_UP   0x0004
#define    PATORI_FEET_FIRST_RIGHT_UP  0x0008
#define    PATORI_HEAD_FIRST_PRONE     0x0100
#define    PATORI_HEAD_FIRST_SUPINE    0x0200
#define    PATORI_HEAD_FIRST_LEFT_UP   0x0400
#define    PATORI_HEAD_FIRST_RIGHT_UP  0x0800

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_coil_H__ */

