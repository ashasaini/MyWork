/*****************************************************************************
 *	imgsrv_ipc.h
 *	2002/01/17 y.morita	V0.01 P1.17 timeout(read/write).
 *	2002/01/15 y.morita	V0.01 P1.10 timeout(connect).
 *	2002/01/10 y.morita	V0.01 P1.10 timeout.
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __imgsrv_ipc_h__
#define __imgsrv_ipc_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/imgsrv/Imgsrv.h>

int	imgsrv_open_socket(char *hostname, char *servname);
int	imgsrv_connect_socket(char *hostname, char *servname, int timeout);
void	imgsrv_close_socket(int s);
ssize_t	imgsrv_readn(int fd, void *vptr, size_t n);
ssize_t	imgsrv_writen(int fd, const void *vptr, size_t n);
ssize_t	imgsrv_read(int fd, void *vptr, size_t n, int sec);
ssize_t	imgsrv_write(int fd, const void *vptr, size_t n, int sec);
int	imgsrv_msg_read(int s, imgsrv_message_t *msg, int sec);
int	imgsrv_msg_write(int s, imgsrv_message_t *msg, int sec);
int	imgsrv_make_open_fifo(char *name, int key, char *tstamp, int flags);
int	imgsrv_open_fifo(char *name, int key, char *tstamp, int flags);
void	imgsrv_close_fifo(int fd);
void	imgsrv_close_unlink_fifo(int fd, char *name, int key, char *tstamp);
int	imgsrv_fifo_write(int fd, const void *buf, size_t size);
int	imgsrv_fifo_read(int fd, void *buf, size_t size);
int	imgsrv_pnc_fifo_write(int fd, imgsrv_pnc_fifo_t *buf);
int	imgsrv_pnc_fifo_read(int fd, imgsrv_pnc_fifo_t *buf);
int	imgsrv_pnc_fifo(int f[],
			imgsrv_pnc_fifo_t *ctop,
			imgsrv_pnc_fifo_t *ptoc);
int	imgsrv_set_fl(int fd, int flags);
int	imgsrv_clr_fl(int fd, int flags);
int	imgsrv_read_timeout(int fd, int sec);
int	imgsrv_write_timeout(int fd, int sec);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __imgsrv_ipc_h__ */
