/*****************************************************************************
 *	画像サーバーインターフェースライブラリ
 *	ImgsrvLib.h
 *	2002/01/15 y.morita	V0.01 P1.10 timeout(connect)/limit.
 *	2002/01/11 y.morita	V0.01 P1.10 timeout
 *	2001/12/27 y.morita	V0.01 P1.09 extra-buffer(put/get)
 *	2001/12/20 y.morita	V0.01 P1.08 immediate return.
 *	2001/12/19 y.morita	V0.01 P1.08 extra-information
 *	2001/10/24 y.morita	V0.01 P1.02 timestamp
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __ImgsrvLib_H__
#define __ImgsrvLib_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/imgsrv/Imgsrv.h>
#include <tami/imgsrv/ImgsrvErrno.h>


/*===========================================================================*
 *	付加情報なしで処理するためのマクロ
 *===========================================================================*/
#define	ImgsrvStart(socket,kind,mode,max,type,form,row,col,slice) \
	ImgsrvStartExtra(socket,kind,mode,max,type,form,row,col,slice,0)
#define	ImgsrvPut(socket,jobid,sliceno,putbuf) \
	ImgsrvPutExtra(socket,jobid,sliceno,putbuf,NULL,0)
#define	ImgsrvGet(socket,jobid,sliceno,getbuf) \
	ImgsrvGetExtra(socket,jobid,sliceno,getbuf,NULL,0)
#define	ImgsrvGetImdt(hostname,jobid,sliceno,getbuf) \
	ImgsrvGetImdtExtra(hostname,jobid,sliceno,getbuf,NULL)

	
/*===========================================================================*
 *	プロトタイプ
 *===========================================================================*/
void	imgsrv_time_reset();
void	imgsrv_time_stamp();
void	imgsrv_time_stamp2(char			*str);

void		ImgsrvInitLimit();
void		ImgsrvSetLimit(imgsrv_limit_t *limit);
imgsrv_limit_t *ImgsrvGetLimit();

int	ImgsrvConnect	(char			*hostname);
int	ImgsrvClose	(int			socket);
int	ImgsrvStartExtra(int			socket,
			 imgsrv_const_kind_t	kind,
			 imgsrv_const_mode_t	mode,
			 long			max,
			 imgsrv_const_type_t	type,
			 imgsrv_const_form_t	form,
			 long			row,
			 long			col,
			 long			slice,
			 long			exlen);	/* extra information length */
int	ImgsrvFinish	(int			socket,
			 long			jobid);
int	ImgsrvPutExtra	(int			socket,
			 long			jobid,
			 long			sliceno,
			 void			*putbuf,
			 void			*extbuf,
			 int			timeout);
int	ImgsrvGetExtra	(int			socket,
			 long			jobid,
			 long			sliceno,
			 void			*getbuf,
			 void			*extbuf,
			 int			timeout);
int	ImgsrvGetImdtExtra	(char		*hostname,
				 long		jobid,
				 long		sliceno,
				 void		*getbuf,
				 void		*extbuf);
int	ImgsrvGetSync	(int			reqid,
			 int			timeout);
int	ImgsrvGetCancel	(int			reqid);
int	ImgsrvReqInfo	(int			reqid,
			 int			*socket,
			 long			*jobid,
			 long			*sliceno,
			 long			*nbytes,
			 void			**buf);
int	ImgsrvAttach	(int			socket,
			 long			jobid,
			 long			sliceno,
			 void			**addrp);
int	ImgsrvDetach	(int			socket,
			 long			jobid,
			 long			sliceno,
			 void			*addr);
int	ImgsrvLock	(int			socket,
			 long			jobid,
			 long			sliceno,
			 imgsrv_const_lock_t	flag);
int	ImgsrvUnlock	(int			socket,
			 long			jobid,
			 long			sliceno,
			 imgsrv_const_lock_t	flag);
int	ImgsrvInfo	(int			socket,
			 int			jobid,
			 imgsrv_job_ds_t	*jobds);
int	ImgsrvKill	(char			*hostname,
			 int			pid,
			 int			signo);

#ifdef __cplusplus
} // extern "C"
#endif
     
#endif	/* #define __ImgsrvLib_H__ */
