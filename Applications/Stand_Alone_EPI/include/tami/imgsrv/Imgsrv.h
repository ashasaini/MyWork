/*****************************************************************************
 *	Imgsrv.h
 *	2002/05/01 y.morita	V0.01 P1.18 IMGSRV_LIMIT_MAXSLICE(65536)
 *	2002/01/17 y.morita	V0.01 P1.11 timeout(read/write).
 *	2002/01/15 y.morita	V0.01 P1.10 limit.
 *	2002/01/11 y.morita	V0.01 P1.10 timeout(IMGSRV_REQ_KILL)
 *	2001/12/27 y.morita	V0.01 P1.09 extra-buffer(put/get)
 *	2001/12/20 y.morita	V0.01 P1.08 imgsrv_req.
 *	2001/12/10 y.morita	V0.01 P1.06 log(mask/output).
 *	2001/12/10 y.morita	V0.01 P1.05 jobs.
 *	2001/12/09 y.morita	V0.01 P1.04 imgsrv_log.
 *	2001/10/12 y.morita	V0.01 P1.02 DEBUG_IMGSRV
 *	2001/07/18 y.morita	V0.01 P1.01 ImgsrvErrno(Imgsrv.h)
 *	2001/07/17 y.morita	V0.01 P1.01 limit
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __Imgsrv_h__
#define __Imgsrv_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __WIN_NATIVE_PORTING__
#include <sys/param.h>
#endif /* __WIN_NATIVE_PORTING__ */


/*===========================================================================*
 *	定数：デフォルト値
 *===========================================================================*/
#define IMGSRV_SERVICE_NAME	"imgsrv"

#ifndef __NUTC__
#define IMGSRV_FIFO_NAME        "/gp/tmp/imgsrv.fifo"
#else
#define IMGSRV_FIFO_NAME "imgsrv.fifo"
#endif
#define	IMGSRV_FLOCK_EMPTY_NAME	"/gp/tmp/imgsrv.lock.empty"
#define	IMGSRV_FLOCK_MUTEX_NAME	"/gp/tmp/imgsrv.lock.mutex"

#define	IMGSRV_GP_ETC_PATH	"GP_ETC_PATH"
#define	IMGSRV_CONFIG_PATH	"/gp/etc"
#define	IMGSRV_CONFIG_NAME	"imageServer.cf"

#define IMGSRV_KEY_LIMIT_MAXROW		"LIMIT_MAX_ROW"
#define IMGSRV_KEY_LIMIT_MAXCOL		"LIMIT_MAX_COL"
#define IMGSRV_KEY_LIMIT_MAXSLICE	"LIMIT_MAX_SLICE"
#define IMGSRV_KEY_LIMIT_MAXSHMNUM	"LIMIT_MAX_SHM_NUM"
#define IMGSRV_KEY_LIMIT_MAXSHMSIZE	"LIMIT_MAX_SHM_SIZE"
#define IMGSRV_KEY_LIMIT_MAXSHMTOTAL	"LIMIT_MAX_SHM_TOTAL"
#define IMGSRV_KEY_LIMIT_JOBLOW		"LIMIT_JOB_LOW"
#define IMGSRV_KEY_LIMIT_JOBHIGH	"LIMIT_JOB_HIGH"
#define	IMGSRV_KEY_LIMIT_MAXREQTOTAL	"LIMIT_MAX_REQ_TOTAL"
#define	IMGSRV_KEY_LIMIT_MAXREQ2DFT	"LIMIT_MAX_REQ_2DFT"
#define	IMGSRV_KEY_LIMIT_MAXREQ3DFT	"LIMIT_MAX_REQ_3DFT"
#define	IMGSRV_KEY_LIMIT_MAXREQINTR	"LIMIT_MAX_REQ_INTR"
#define	IMGSRV_KEY_LIMIT_TIMEOUT	"LIMIT_TIMEOUT"
#define	IMGSRV_KEY_LIMIT_RETRY		"LIMIT_RETRY"

#define	IMGSRV_LOG_MASKLEN		64
#define	IMGSRV_LOG_OUTPUTLEN		128

#define IMGSRV_USER_REQUEST_MAX		1024

#define IMGSRV_TIMEOUT		(imgsrv_limit()->Timeout)
#define IMGSRV_RD_TIMEOUT	(imgsrv_limit()->Timeout + 10)
#define IMGSRV_WR_TIMEOUT	(imgsrv_limit()->Timeout)


enum {
  IMGSRV_STRBUFLEN = 512		/* ファイル名生成用バッファサイズ */
};

enum {
  IMGSRV_LIMIT_MAXROW		=   8192,
  IMGSRV_LIMIT_MAXCOL		=   8192,
  IMGSRV_LIMIT_MAXSLICE		=  65536,	/* 256 Slice x 256 Phase */
  IMGSRV_LIMIT_MAXSHMNUM	=     64,	/* 使用最大共有メモリ数*/
  IMGSRV_LIMIT_MAXSHMSIZE	= 524288,	/* K Byte 単位 */
  IMGSRV_LIMIT_MAXSHMTOTAL	= 524288,	/* K Byte 単位 */
  IMGSRV_LIMIT_JOBLOW		=      1,
  IMGSRV_LIMIT_JOBHIGH		=  10000,
  IMGSRV_LIMIT_MAXREQ2DFT	=      5,	/* モード2DFTの最大起動子プロセス数 */
  IMGSRV_LIMIT_MAXREQ3DFT	=      5,	/* モード3DFTの最大起動子プロセス数 */
  IMGSRV_LIMIT_MAXREQINTR	=      1,	/* モードINTRの最大起動子プロセス数 */
  IMGSRV_LIMIT_MAXREQTOTAL	=     64,	/* モード2DFTの最大起動子プロセス数 */
  IMGSRV_LIMIT_TIMEOUT		=     10,	/* デフォルトタイムアウト値(秒) */
  IMGSRV_LIMIT_RETRY		=      5	/* タイムアウト時のリトライ回数 */
};

enum {
  IMGSRV_SHM_KEY_2DFT	= 9000,	/* Shared Memmory Key */
  IMGSRV_SHM_KEY_3DFT	= 9001,	/* Shared Memmory Key */
  IMGSRV_SHM_KEY_INTR	= 9002	/* Shared Memmory Key */
};


/*===========================================================================*
 *	処理要求画像格納エリア処理種
 *===========================================================================*/
typedef enum imgsrv_const_kind_e {
  IMGSRV_KIND_2DFT,
  IMGSRV_KIND_3DFT,
  IMGSRV_KIND_INTR
} imgsrv_const_kind_t;


/*===========================================================================*
 *	処理要求画像モード
 *===========================================================================*/
typedef enum imgsrv_const_mode_e {
  IMGSRV_MODE_STABLE,
  IMGSRV_MODE_CYCLIC
} imgsrv_const_mode_t;


/*===========================================================================*
 *	処理要求画像フォーム
 *===========================================================================*/
typedef enum imgsrv_const_form_e {
  IMGSRV_FORM_UNKNOWN	= -1,
  IMGSRV_FORM_MAGNITUDE,
  IMGSRV_FORM_PHASE,
  IMGSRV_FORM_REAL_ONLY,
  IMGSRV_FORM_IMGN_ONLY,
  IMGSRV_FORM_COMPLEX,
  IMGSRV_FORM_POLAR
} imgsrv_const_form_t;


/*===========================================================================*
 *	処理要求画像タイプ
 *===========================================================================*/
typedef enum imgsrv_const_type_e {
  IMGSRV_TYPE_UNKNOWN	= -1,
  IMGSRV_TYPE_INT,
  IMGSRV_TYPE_FLOAT,
  IMGSRV_TYPE_INT8,
  IMGSRV_TYPE_UINT8,
  IMGSRV_TYPE_INT16,
  IMGSRV_TYPE_UINT16,
  IMGSRV_TYPE_INT32,
  IMGSRV_TYPE_UINT32,
  IMGSRV_TYPE_FLT32,
  IMGSRV_TYPE_FLT64
} imgsrv_const_type_t;


/*===========================================================================*
 *	特殊スライス番号
 *===========================================================================*/
typedef enum imgsrv_const_slice_e {
  IMGSRV_SLICE_NEXT = -1,
  IMGSRV_SLICE_LAST = -2
} imgsrv_const_slice_t;


/*===========================================================================*
 *	ロック
 *===========================================================================*/
typedef enum imgsrv_const_lock_e {
  IMGSRV_LOCK_READ,
  IMGSRV_LOCK_WRITE
} imgsrv_const_lock_t;


/*===========================================================================*
 *	その他
 *===========================================================================*/
#define imgsrv_slice_forw(i, j, ns) \
			(((i+j)%ns <= 0) ? ((i+j)%ns)+ns : ((i+j)%ns))
#define imgsrv_slice_back(i, j, ns) \
			(((i-j)%ns <= 0) ? ((i-j)%ns)+ns : ((i-j)%ns))
#define imgsrv_slice_next(i, ns)	imgsrv_slice_forw(i, 1, ns)
#define imgsrv_slice_prev(i, ns)	imgsrv_slice_back(i, 1, ns)
#define imgsrv_slice_sub(i, j, ns) \
			(((i-j)%ns < 0) ? ((i-j)%ns)+ns : ((i-j)%ns))


typedef struct imgsrv_label_s {
  char	*lbl;
#ifndef __FREE_BSD__
  char	*BSD_lbl;
#endif
  long	val;
#ifndef __FREE_BSD__
  long	BSD_val;
#endif
} imgsrv_label_t;


/*===========================================================================*
 *	JOB管理情報
 *===========================================================================*/
typedef struct imgsrv_job_res_s {
  imgsrv_const_kind_t	kind;		/* リソース種			*/
  imgsrv_const_mode_t	mode;		/* 画像格納エリア使用モード	*/
  long			max;		/* IMGSRV_MODE_CYCLIC時の
					   領域確保スライス数		*/
#ifndef __FREE_BSD__
  long			BSD_max;
#endif
  imgsrv_const_type_t	type;		/* 画像タイプ			*/
  imgsrv_const_form_t	form;		/* 画像フォーム			*/
  long			row;		/* 画像横方向サイズ		*/
#ifndef __FREE_BSD__
  long			BSD_row;
#endif
  long			col;		/* 画像縦方向サイズ		*/
#ifndef __FREE_BSD__
  long			BSD_col;
#endif
  long			slice;		/* 画像スライス枚数		*/
#ifndef __FREE_BSD__
  long			BSD_slice;
#endif
  long			extlen;		/* 付加情報サイズ		*/
#ifndef __FREE_BSD__
  long			BSD_extlen;
#endif
} imgsrv_job_res_t;

typedef struct imgsrv_job_sync_stable_s {
  long	fd_mutex;
#ifndef __FREE_BSD__
  long	BSD_fd_mutex;
#endif
  long	fd_empty;
#ifndef __FREE_BSD__
  long	BSD_fd_empty;
#endif
} imgsrv_job_sync_stable_t;

typedef struct imgsrv_job_sync_cyclic_s {
  long	fd_mutex;
#ifndef __FREE_BSD__
  long	BSD_fd_mutex;
#endif
  long	se_nempty;
#ifndef __FREE_BSD__
  long	BSD_se_nempty;
#endif
  long	se_nstored;
#ifndef __FREE_BSD__
  long	BSD_se_nstored;
#endif
} imgsrv_job_sync_cyclic_t;

typedef union imgsrv_job_sync_mode_u {
  imgsrv_job_sync_stable_t	stable;
  imgsrv_job_sync_cyclic_t	cyclic;
} imgsrv_job_sync_mode_t;

typedef struct imgsrv_job_sync_s {
  imgsrv_job_sync_mode_t	mode;
  long				r_next;
#ifndef __FREE_BSD__
  long				BSD_r_next;
#endif
  long				r_done;
#ifndef __FREE_BSD__
  long				BSD_r_done;
#endif
  long				w_next;
#ifndef __FREE_BSD__
  long				BSD_w_next;
#endif
  long				w_done;
#ifndef __FREE_BSD__
  long				BSD_w_done;
#endif
} imgsrv_job_sync_t;

typedef struct imgsrv_job_ds_s {
  long			jobid;	/* JOB ID				*/
#ifndef __FREE_BSD__
  long			BSD_jobid;
#endif
  long			shmid;	/* 共有メモリキー			*/
#ifndef __FREE_BSD__
  long			BSD_shmid;
#endif
  long			nbytes;	/* １スライス分のバイト数(extbufも含む)	*/
#ifndef __FREE_BSD__
  long			BSD_nbytes;
#endif
  long			nslice;	/* スライス枚数				*/
#ifndef __FREE_BSD__
  long			BSD_nslice;
#endif
  imgsrv_job_res_t	res;	/* リソース				*/
  imgsrv_job_sync_t	sync;	/* 同期情報				*/
} imgsrv_job_ds_t;

typedef struct imgsrv_job_node_s {
  struct imgsrv_job_node_s	*prev;
#ifndef __FREE_BSD__
  struct imgsrv_job_node_s	*BSD_prev;
#endif
  struct imgsrv_job_node_s	*next;
#ifndef __FREE_BSD__
  struct imgsrv_job_node_s	*BSD_next;
#endif
  imgsrv_job_ds_t		jobds;
} imgsrv_job_node_t;


/* ATTACH/DETACH用 */
typedef struct imgsrv_shm_node_s {
  struct imgsrv_shm_node_s	*prev;
#ifndef __FREE_BSD__
  struct imgsrv_shm_node_s	*BSD_prev;
#endif
  struct imgsrv_shm_node_s	*next;
#ifndef __FREE_BSD__
  struct imgsrv_shm_node_s	*BSD_next;
#endif
  imgsrv_job_ds_t		jobds;
  long				count;
#ifndef __FREE_BSD__
  long				BSD_count;
#endif
  void				*addrp;
#ifndef __FREE_BSD__
  void				*BSD_addrp;
#endif
} imgsrv_shm_node_t;

typedef struct imgsrv_shm_s {
  imgsrv_shm_node_t	*top;
#ifndef __FREE_BSD__
  imgsrv_shm_node_t	*BSD_top;
#endif
  imgsrv_shm_node_t	*free;
#ifndef __FREE_BSD__
  imgsrv_shm_node_t	*BSD_free;
#endif
} imgsrv_shm_t;


/* LOCK/UNLOCK用 */
typedef struct imgsrv_lock_node_s {
  struct imgsrv_lock_node_s	*prev;
#ifndef __FREE_BSD__
  struct imgsrv_lock_node_s	*BSD_prev;
#endif
  struct imgsrv_lock_node_s	*next;
#ifndef __FREE_BSD__
  struct imgsrv_lock_node_s	*BSD_next;
#endif
  imgsrv_job_ds_t		jobds;
  long				count;
#ifndef __FREE_BSD__
  long				BSD_count;
#endif
} imgsrv_lock_node_t;

typedef struct imgsrv_lock_s {
  imgsrv_lock_node_t	*top;
#ifndef __FREE_BSD__
  imgsrv_lock_node_t	*BSD_top;
#endif
  imgsrv_lock_node_t	*free;
#ifndef __FREE_BSD__
  imgsrv_lock_node_t	*BSD_free;
#endif
} imgsrv_lock_t;


/*===========================================================================*
 *	管理用構造体
 *===========================================================================*/
typedef struct imgsrv_limit_s {
  unsigned long MaxRow;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxRow;
#endif
  unsigned long MaxCol;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxCol;
#endif
  unsigned long MaxSlice;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxSlice;
#endif
  unsigned long MaxShmNum;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxShmNum;
#endif
  unsigned long MaxShmSize;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxShmSize;
#endif
  unsigned long MaxShmTotal;
#ifndef __FREE_BSD__
  unsigned long BSD_MaxShmTotal;
#endif
  unsigned long JobLow;
#ifndef __FREE_BSD__
  unsigned long BSD_JobLow;
#endif
  unsigned long JobHigh;
#ifndef __FREE_BSD__
  unsigned long BSD_JobHigh;
#endif
  unsigned long	MaxReq2dft;	/* モード2DFTの最大起動子プロセス数 */
#ifndef __FREE_BSD__
  unsigned long	BSD_MaxReq2dft;
#endif
  unsigned long	MaxReq3dft;	/* モード3DFTの最大起動子プロセス数 */
#ifndef __FREE_BSD__
  unsigned long	BSD_MaxReq3dft;
#endif
  unsigned long	MaxReqIntr;	/* モードINTRの最大起動子プロセス数 */
#ifndef __FREE_BSD__
  unsigned long	BSD_MaxReqIntr;
#endif
  unsigned long	MaxReqTotal;	/* 最大受付要求数 */
#ifndef __FREE_BSD__
  unsigned long	BSD_MaxReqTotal;
#endif
  unsigned long Timeout;	/* デフォルトタイムアウト値(秒) */
#ifndef __FREE_BSD__
  unsigned long BSD_Timeout;
#endif
  unsigned long Retry;		/* タイムアウト時のリトライ回数 */
#ifndef __FREE_BSD__
  unsigned long BSD_Retry;
#endif

#if 0
  unsigned long CountShmNum;	/* 現在使用中の共有メモリ数 */
  unsigned long CountShmTotal;	/* 現在使用中の共有メモリ総サイズ */
  unsigned long CountReq2dft;	/* 現在起動中のモード2DFTの子プロセス数 */
  unsigned long CountReq3dft;	/* 現在起動中のモード3DFTの子プロセス数 */
  unsigned long CountReqIntr;	/* 現在起動中のモードINTRの子プロセス数 */
  unsigned long CountReqTotal;	/* 現在起動中の子プロセス数 */
#endif
} imgsrv_limit_t;


/*===========================================================================*
 *	クライアントサーバー間メッセージフォーマット
 *===========================================================================*/
/* 処理要求コマンド */
typedef enum imgsrv_msg_req_e {
  /* クライアント/サーバー間 */
  IMGSRV_REQ_NOTUSED	= -1,
  IMGSRV_REQ_REPLY	= 0,
  IMGSRV_REQ_CONNECT,
  IMGSRV_REQ_CLOSE,
  IMGSRV_REQ_START,
  IMGSRV_REQ_FINISH,
  IMGSRV_REQ_PUT,
  IMGSRV_REQ_GET,
  IMGSRV_REQ_ATTACH,
  IMGSRV_REQ_DETACH,
  IMGSRV_REQ_LOCK,
  IMGSRV_REQ_UNLOCK,
  IMGSRV_REQ_INFO,
  IMGSRV_REQ_JOBS,
  IMGSRV_REQ_LOGMASK,
  IMGSRV_REQ_LOGOUTPUT,
  IMGSRV_REQ_LOGERROUT,
  IMGSRV_REQ_KILL
} imgsrv_msg_req_t;


/* へッダー */
typedef struct imgsrv_msg_header_s {
  char	hostname[MAXHOSTNAMELEN];	/* 発信元ホスト名	*/
  long	pid;				/* 発信元プロセス ID	*/
#ifndef __FREE_BSD__
  long	BSD_pid;
#endif
  long	request;			/* 要求コマンド		*/
#ifndef __FREE_BSD__
  long	BSD_request;
#endif
  long	nbytes;				/* パラメータ長		*/
#ifndef __FREE_BSD__
  long	BSD_nbytes;
#endif
} imgsrv_msg_header_t;


/* クライアント/サーバー間 各要求コマンドパラメータ */
/*   START パラメータ */
typedef imgsrv_job_res_t	imgsrv_prm_start_t;

/*   FINISH パラメータ */
typedef struct imgsrv_prm_finish_s {
  long	jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long	BSD_jobid;
#endif
} imgsrv_prm_finish_t;

/*   PUT パラメータ */
typedef struct imgsrv_prm_put_s {
  long	jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long	BSD_jobid;
#endif
  long	sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long	BSD_sliceno;
#endif
  long	extra;		/* 0:なし, 1:あり	*/
#ifndef __FREE_BSD__
  long	BSD_extra;
#endif
} imgsrv_prm_put_t;

/*   GET パラメータ */
typedef struct imgsrv_prm_get_s {
  long	jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long	BSD_jobid;
#endif
  long	sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long	BSD_sliceno;
#endif
  long	extra;		/* 0:なし, 1:あり	*/
#ifndef __FREE_BSD__
  long	BSD_extra;
#endif
} imgsrv_prm_get_t;

/*   ATTACH パラメータ */
typedef struct imgsrv_prm_attach_s {
  long	jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long	BSD_jobid;
#endif
  long	sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long	BSD_sliceno;
#endif
} imgsrv_prm_attach_t;

/*   DETACH パラメータ */
typedef struct imgsrv_prm_detach_s {
  long	jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long	BSD_jobid;
#endif
  long	sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long	BSD_sliceno;
#endif
} imgsrv_prm_detach_t;

/*   LOCK パラメータ */
typedef struct imgsrv_prm_lock_s {
  long			jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long			BSD_jobid;
#endif
  long			sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long			BSD_sliceno;
#endif
  imgsrv_const_lock_t	flag;		/* READ/WRITE		*/
#ifndef __FREE_BSD__
  int           BSD_termboundary;
#endif
} imgsrv_prm_lock_t;

/*   UNLOCK パラメータ */
typedef struct imgsrv_prm_unlock_s {
  long			jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long			BSD_jobid;
#endif
  long			sliceno;	/* スライス番号		*/
#ifndef __FREE_BSD__
  long			BSD_sliceno;
#endif
  imgsrv_const_lock_t	flag;		/* READ/WRITE		*/
#ifndef __FREE_BSD__
  int           BSD_termboundary;
#endif
} imgsrv_prm_unlock_t;

/*   INFO パラメータ */
typedef struct imgsrv_prm_info_s {
  long			jobid;		/* JOB ID		*/
#ifndef __FREE_BSD__
  long			BSD_jobid;
#endif
} imgsrv_prm_info_t;

/*   KILL パラメータ */
typedef struct imgsrv_prm_kill_s {
  long			pid;		/* サーバ(子)PID	*/
#ifndef __FREE_BSD__
  long			BSD_pid;
#endif
  long			signo;		/* signal		*/
#ifndef __FREE_BSD__
  long			BSD_signo;
#endif
} imgsrv_prm_kill_t;

/*   リプライステータス */
typedef struct imgsrv_prm_reply_s {
  long			status;		/* ステータス	*/
#ifndef __FREE_BSD__
  long			BSD_status;
#endif
  imgsrv_job_ds_t	jobds;
} imgsrv_prm_reply_t;

/*   JOBS リプライステータス */
typedef struct imgsrv_prm_rjobs_s {
  long			status;		/* ステータス	*/
#ifndef __FREE_BSD__
  long			BSD_status;
#endif
  int			jobnum;
#ifndef __FREE_BSD__
  int			BSD_termboundary;
#endif
} imgsrv_prm_rjobs_t;

/* 要求コマンドパラメータ */
typedef union imgsrv_msg_param_u {
  /* クライアント/サーバー間コマンドパラメータ */
  imgsrv_prm_start_t	start;
  imgsrv_prm_finish_t	finish;
  imgsrv_prm_put_t	put;
  imgsrv_prm_get_t	get;
  imgsrv_prm_attach_t	attach;
  imgsrv_prm_detach_t	detach;
  imgsrv_prm_lock_t	lock;
  imgsrv_prm_unlock_t	unlock;
  imgsrv_prm_info_t	info;
  imgsrv_prm_kill_t	kill;
  imgsrv_prm_reply_t	reply;	/* リプライ用 */
  imgsrv_prm_rjobs_t	rjobs;	/* JOBSのリプライ用 */
} imgsrv_msg_param_t;


/* メッセージフォーマット */
typedef struct imgsrv_message_s {
  imgsrv_msg_header_t    header;
  imgsrv_msg_param_t     param;
} imgsrv_message_t;


/*===========================================================================*
 *	サーバー親子間通信フォーマット
 *	FIFOの場合,複数の読み書きが混じり合わないように,アトミックな読み書き
 *	になるように注意すること
 *===========================================================================*/
/* サーバー親子間処理要求コマンド */
typedef enum imgsrv_pnc_req_e {
  IMGSRV_REQ_FIFO      = 100,
  IMGSRV_REQ_FIFOREPLY,
  IMGSRV_REQ_JOBNEW,
  IMGSRV_REQ_JOBINFO,
  IMGSRV_REQ_JOBDEL,
  IMGSRV_REQ_ALLJOBS,
  
  IMGSRV_REQ_SYNCPOST,
  IMGSRV_REQ_SYNCWRITENEXT,
  IMGSRV_REQ_SYNCWRITEDONE,
  IMGSRV_REQ_SYNCREADNEXT,
  IMGSRV_REQ_SYNCREADDONE,

  IMGSRV_REQ_MASK,
  IMGSRV_REQ_OUTPUT,
  IMGSRV_REQ_ERROUT,

  IMGSRV_REQ_END
} imgsrv_pnc_req_t;

/* サーバー親子間コマンドパラメータ */
typedef struct imgsrv_pnc_fifo_s {
  long			req;
#ifndef __FREE_BSD__
  long			BSD_req;
#endif
  long			pid;
#ifndef __FREE_BSD__
  long			BSD_pid;
#endif
  char			tstamp[32];	/* タイムスタンプ保存 */
  long			status;
#ifndef __FREE_BSD__
  long			BSD_status;
#endif
  long			value;
#ifndef __FREE_BSD__
  long			BSD_value;
#endif
  int			socket;
#ifndef __FREE_BSD__
  int			BSD_socket;
#endif
  imgsrv_job_ds_t	jobds;
} imgsrv_pnc_fifo_t;


/*===========================================================================*
 *	デバッグ用プリント文
 *===========================================================================*/

#if 0

#ifdef DEBUG_IMGSRV
#include <time.h>

static char prtbuf[512]; \

#define IMGSRV_PRINT(fmt) \
	{ \
	  time_t t; \
	  struct tm *tm; \
	  t = time(NULL); \
	  tm = localtime(&t); \
	  strftime(prtbuf, 512, "%c", tm); \
    	  fprintf(stdout, "%s imgsrv(%d) ",prtbuf,getpid()); \
    	  fprintf(stdout, fmt); \
    	  fprintf(stdout, "\t%s:%d ",__FILE__,__LINE__); \
    	  fprintf(stdout, "\n"); \
	  fflush(stdout); \
	}

#define IMGSRV_PRINT2(fmt,prm1) \
	{ \
	  time_t t; \
	  struct tm *tm; \
	  t = time(NULL); \
	  tm = localtime(&t); \
	  strftime(prtbuf, 512, "%c", tm); \
    	  fprintf(stdout, "%s imgsrv(%d) ",prtbuf,getpid()); \
    	  fprintf(stdout, fmt, prm1); \
    	  fprintf(stdout, "\t%s:%d ",__FILE__,__LINE__); \
    	  fprintf(stdout, "\n"); \
	  fflush(stdout); \
	}

#else	/* #define IMGSRV_DB_PRINT(str) */

#define IMGSRV_DB_PRINT(str)	;
#define IMGSRV_PRINT(fmt)	;
#define IMGSRV_PRINT2(fmt,prm1)	;

#endif	/* #define IMGSRV_DB_PRINT(str) */

#endif


#ifdef IMGSRV_DEFINE_EXTERN_VARIABLES
/*===========================================================================*
 *	外部変数定義
 *===========================================================================*/
long	ImgsrvErrno;			/* エラー番号 */

char	ImgsrvTimeStampP[IMGSRV_STRBUFLEN];	/* 親のタイムスタンプ文字列 */
char	ImgsrvTimeStampC[IMGSRV_STRBUFLEN];	/* 子のタイムスタンプ文字列 */

char	ImgsrvStrBuf[IMGSRV_STRBUFLEN];	/* ファイル名生成用バッファ。
					 * ただしIMGSRV_STRBUFLENバイト以上の
					 * 文字数についてはチェック(保証)され
					 * ない */


imgsrv_label_t ImgsrvLabelMsgReq[] =
{
#ifdef __FREE_BSD__
  "IMGSRV_REQ_REPLY",		IMGSRV_REQ_REPLY,
  "IMGSRV_REQ_CONNECT",		IMGSRV_REQ_CONNECT,
  "IMGSRV_REQ_CLOSE",		IMGSRV_REQ_CLOSE,
  "IMGSRV_REQ_START",		IMGSRV_REQ_START,
  "IMGSRV_REQ_FINISH",		IMGSRV_REQ_FINISH,
  "IMGSRV_REQ_PUT",		IMGSRV_REQ_PUT,
  "IMGSRV_REQ_GET",		IMGSRV_REQ_GET,
  "IMGSRV_REQ_ATTACH",		IMGSRV_REQ_ATTACH,
  "IMGSRV_REQ_DETACH",		IMGSRV_REQ_DETACH,
  "IMGSRV_REQ_LOCK",		IMGSRV_REQ_LOCK,
  "IMGSRV_REQ_UNLOCK",		IMGSRV_REQ_UNLOCK,
  "IMGSRV_REQ_INFO",		IMGSRV_REQ_INFO,
  "IMGSRV_REQ_JOBS",		IMGSRV_REQ_JOBS,
  "IMGSRV_REQ_LOGMASK",		IMGSRV_REQ_LOGMASK,
  "IMGSRV_REQ_LOGOUTPUT",	IMGSRV_REQ_LOGOUTPUT,
  "IMGSRV_REQ_LOGERROUT",	IMGSRV_REQ_LOGERROUT,
  "IMGSRV_REQ_KILL",		IMGSRV_REQ_KILL,
#else
  "IMGSRV_REQ_REPLY",		"", IMGSRV_REQ_REPLY,		0L,
  "IMGSRV_REQ_CONNECT",		"", IMGSRV_REQ_CONNECT,		0L,
  "IMGSRV_REQ_CLOSE",		"", IMGSRV_REQ_CLOSE,		0L,
  "IMGSRV_REQ_START",		"", IMGSRV_REQ_START,		0L,
  "IMGSRV_REQ_FINISH",		"", IMGSRV_REQ_FINISH,		0L,
  "IMGSRV_REQ_PUT",		"", IMGSRV_REQ_PUT,		0L,
  "IMGSRV_REQ_GET",		"", IMGSRV_REQ_GET,		0L,
  "IMGSRV_REQ_ATTACH",		"", IMGSRV_REQ_ATTACH,		0L,
  "IMGSRV_REQ_DETACH",		"", IMGSRV_REQ_DETACH,		0L,
  "IMGSRV_REQ_LOCK",		"", IMGSRV_REQ_LOCK,		0L,
  "IMGSRV_REQ_UNLOCK",		"", IMGSRV_REQ_UNLOCK,		0L,
  "IMGSRV_REQ_INFO",		"", IMGSRV_REQ_INFO,		0L,
  "IMGSRV_REQ_JOBS",		"", IMGSRV_REQ_JOBS,		0L,
  "IMGSRV_REQ_LOGMASK",		"", IMGSRV_REQ_LOGMASK,		0L,
  "IMGSRV_REQ_LOGOUTPUT",	"", IMGSRV_REQ_LOGOUTPUT,	0L,
  "IMGSRV_REQ_LOGERROUT",	"", IMGSRV_REQ_LOGERROUT,	0L,
  "IMGSRV_REQ_KILL",		"", IMGSRV_REQ_KILL,		0L,
#endif
  NULL
};

imgsrv_label_t ImgsrvLabelPncReq[] =
{
#ifdef __FREE_BSD__
  "IMGSRV_REQ_FIFO",		IMGSRV_REQ_FIFO,
  "IMGSRV_REQ_FIFOREPLY",	IMGSRV_REQ_FIFOREPLY,
  "IMGSRV_REQ_JOBNEW",		IMGSRV_REQ_JOBNEW,
  "IMGSRV_REQ_JOBINFO",		IMGSRV_REQ_JOBINFO,
  "IMGSRV_REQ_JOBDEL",		IMGSRV_REQ_JOBDEL,
  "IMGSRV_REQ_ALLJOBS",		IMGSRV_REQ_ALLJOBS,
  "IMGSRV_REQ_SYNCPOST",	IMGSRV_REQ_SYNCPOST,
  "IMGSRV_REQ_SYNCWRITENEXT",	IMGSRV_REQ_SYNCWRITENEXT,
  "IMGSRV_REQ_SYNCWRITEDONE",	IMGSRV_REQ_SYNCWRITEDONE,
  "IMGSRV_REQ_SYNCREADNEXT",	IMGSRV_REQ_SYNCREADNEXT,
  "IMGSRV_REQ_SYNCREADDONE",	IMGSRV_REQ_SYNCREADDONE,
  "IMGSRV_REQ_MASK",		IMGSRV_REQ_MASK,
  "IMGSRV_REQ_OUTPUT",		IMGSRV_REQ_OUTPUT,
  "IMGSRV_REQ_ERROUT",		IMGSRV_REQ_ERROUT,
  "IMGSRV_REQ_END",		IMGSRV_REQ_END,
#else
  "IMGSRV_REQ_FIFO",		"", IMGSRV_REQ_FIFO,		0L,
  "IMGSRV_REQ_FIFOREPLY",	"", IMGSRV_REQ_FIFOREPLY,	0L,
  "IMGSRV_REQ_JOBNEW",		"", IMGSRV_REQ_JOBNEW,		0L,
  "IMGSRV_REQ_JOBINFO",		"", IMGSRV_REQ_JOBINFO,		0L,
  "IMGSRV_REQ_JOBDEL",		"", IMGSRV_REQ_JOBDEL,		0L,
  "IMGSRV_REQ_ALLJOBS",		"", IMGSRV_REQ_ALLJOBS,		0L,
  "IMGSRV_REQ_SYNCPOST",	"", IMGSRV_REQ_SYNCPOST,	0L,
  "IMGSRV_REQ_SYNCWRITENEXT",	"", IMGSRV_REQ_SYNCWRITENEXT,	0L,
  "IMGSRV_REQ_SYNCWRITEDONE",	"", IMGSRV_REQ_SYNCWRITEDONE,	0L,
  "IMGSRV_REQ_SYNCREADNEXT",	"", IMGSRV_REQ_SYNCREADNEXT,	0L,
  "IMGSRV_REQ_SYNCREADDONE",	"", IMGSRV_REQ_SYNCREADDONE,	0L,
  "IMGSRV_REQ_MASK",		"", IMGSRV_REQ_MASK,		0L,
  "IMGSRV_REQ_OUTPUT",		"", IMGSRV_REQ_OUTPUT,		0L,
  "IMGSRV_REQ_ERROUT",		"", IMGSRV_REQ_ERROUT,		0L,
  "IMGSRV_REQ_END",		"", IMGSRV_REQ_END,		0L,
#endif
  NULL
};

imgsrv_label_t ImgsrvLabelKind[] =
{
#ifdef __FREE_BSD__
  "IMGSRV_KIND_2DFT",	IMGSRV_KIND_2DFT,	
  "IMGSRV_KIND_3DFT",	IMGSRV_KIND_3DFT,	
  "IMGSRV_KIND_INTR",	IMGSRV_KIND_INTR,	
#else
  "IMGSRV_KIND_2DFT",	"", IMGSRV_KIND_2DFT,			0L,
  "IMGSRV_KIND_3DFT",	"", IMGSRV_KIND_3DFT,			0L,
  "IMGSRV_KIND_INTR",	"", IMGSRV_KIND_INTR,			0L,
#endif
  NULL
};

imgsrv_label_t ImgsrvLabelMode[] =
{
#ifdef __FREE_BSD__
  "IMGSRV_MODE_STABLE",	IMGSRV_MODE_STABLE,
  "IMGSRV_MODE_CYCLIC",	IMGSRV_MODE_CYCLIC,
#else
  "IMGSRV_MODE_STABLE",	"", IMGSRV_MODE_STABLE,			0L,
  "IMGSRV_MODE_CYCLIC",	"", IMGSRV_MODE_CYCLIC,			0L,
#endif
  NULL
};

imgsrv_label_t ImgsrvLabelForm[] = {
#ifdef __FREE_BSD__
  "IMGSRV_FORM_MAGNITUDE",	IMGSRV_FORM_MAGNITUDE,
  "IMGSRV_FORM_PHASE",		IMGSRV_FORM_PHASE,
  "IMGSRV_FORM_REAL_ONLY",	IMGSRV_FORM_REAL_ONLY,
  "IMGSRV_FORM_IMGN_ONLY",	IMGSRV_FORM_IMGN_ONLY,
  "IMGSRV_FORM_COMPLEX",	IMGSRV_FORM_COMPLEX,
  "IMGSRV_FORM_POLAR",		IMGSRV_FORM_POLAR,
#else
  "IMGSRV_FORM_MAGNITUDE",	"", IMGSRV_FORM_MAGNITUDE,	0L,
  "IMGSRV_FORM_PHASE",		"", IMGSRV_FORM_PHASE,		0L,
  "IMGSRV_FORM_REAL_ONLY",	"", IMGSRV_FORM_REAL_ONLY,	0L,
  "IMGSRV_FORM_IMGN_ONLY",	"", IMGSRV_FORM_IMGN_ONLY,	0L,
  "IMGSRV_FORM_COMPLEX",	"", IMGSRV_FORM_COMPLEX,	0L,
  "IMGSRV_FORM_POLAR",		"", IMGSRV_FORM_POLAR,		0L,
#endif
  NULL
};

imgsrv_label_t ImgsrvLabelType[] = {
#ifdef __FREE_BSD__
  "IMGSRV_TYPE_INT",		IMGSRV_TYPE_INT,
  "IMGSRV_TYPE_FLOAT",		IMGSRV_TYPE_FLOAT,
  "IMGSRV_TYPE_INT8",		IMGSRV_TYPE_INT8,
  "IMGSRV_TYPE_UINT8",		IMGSRV_TYPE_UINT8,
  "IMGSRV_TYPE_INT16",		IMGSRV_TYPE_INT16,
  "IMGSRV_TYPE_UINT16",		IMGSRV_TYPE_UINT16,
  "IMGSRV_TYPE_INT32",		IMGSRV_TYPE_INT32,
  "IMGSRV_TYPE_UINT32",		IMGSRV_TYPE_UINT32,
  "IMGSRV_TYPE_FLT32",		IMGSRV_TYPE_FLT32,
  "IMGSRV_TYPE_FLT64",		IMGSRV_TYPE_FLT64,
#else
  "IMGSRV_TYPE_INT",		"", IMGSRV_TYPE_INT,		0L,
  "IMGSRV_TYPE_FLOAT",		"", IMGSRV_TYPE_FLOAT,		0L,
  "IMGSRV_TYPE_INT8",		"", IMGSRV_TYPE_INT8,		0L,
  "IMGSRV_TYPE_UINT8",		"", IMGSRV_TYPE_UINT8,		0L,
  "IMGSRV_TYPE_INT16",		"", IMGSRV_TYPE_INT16,		0L,
  "IMGSRV_TYPE_UINT16",		"", IMGSRV_TYPE_UINT16,		0L,
  "IMGSRV_TYPE_INT32",		"", IMGSRV_TYPE_INT32,		0L,
  "IMGSRV_TYPE_UINT32",		"", IMGSRV_TYPE_UINT32,		0L,
  "IMGSRV_TYPE_FLT32",		"", IMGSRV_TYPE_FLT32,		0L,
  "IMGSRV_TYPE_FLT64",		"", IMGSRV_TYPE_FLT64,		0L,
#endif
  NULL
};

#else	/* #ifdef IMGSRV_DEFINE_EXTERN_VARIABLES */

/*===========================================================================*
 *	外部変数宣言
 *===========================================================================*/
extern long	ImgsrvErrno;
extern char	ImgsrvStrBuf[];
extern char	ImgsrvTimeStampP[];
extern char	ImgsrvTimeStampC[];

extern imgsrv_label_t ImgsrvLabelMsgReq[];
extern imgsrv_label_t ImgsrvLabelPncReq[];
extern imgsrv_label_t ImgsrvLabelKind[];
extern imgsrv_label_t ImgsrvLabelMode[];
extern imgsrv_label_t ImgsrvLabelForm[];
extern imgsrv_label_t ImgsrvLabelType[];

#endif	/* #ifdef IMGSRV_DEFINE_EXTERN_VARIABLES */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* #ifndef__Imgsrv_h__ */
