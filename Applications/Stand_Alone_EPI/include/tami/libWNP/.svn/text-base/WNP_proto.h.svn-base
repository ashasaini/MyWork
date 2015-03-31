#ifndef _WNP_PROTO_H
#define _WNP_PROTO_H

/* 
　　Windows Native Porting ライブラリ群のプロトタイプ宣言
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN_NATIVE_PORTING__
#include <stdio.h>

/* --------------------------------------------------------------------------- *
 * 内部シグナル機構 初期・終了処理
 * --------------------------------------------------------------------------- */
extern void  WNP_ProcInit(void);
extern void  WNP_ProcTerm();
/* --------------------------------------------------------------------------- *
 * ソケット 初期：終了処理
 * --------------------------------------------------------------------------- */
extern void  WNP_ProcInit_SocketInit(void);
extern void  WNP_ProcTerm_SocketTerm(void);

/* --------------------------------------------------------------------------- *
 * ソケット
 * --------------------------------------------------------------------------- */
extern int   WNP_socket(int af, int type, int protocol);
extern int   WNP_bind(int s, const struct sockaddr *name, socklen_t namelen);
extern int   WNP_listen(int s, int backlog);
extern int   WNP_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
extern int   WNP_connect(int s, const struct sockaddr *name, socklen_t namelen);
extern int   WNP_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
extern int   WNP_recv(int s, void *buf, size_t len, int flags);
extern int   WNP_send(int s, const void *msg, size_t len, int flags);
extern int   WNP_getsockname(int s, struct sockaddr *name, socklen_t *namelen);
extern int   WNP_getpeername(int s, struct sockaddr *name, socklen_t *namelen);
extern int   WNP_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
extern int   WNP_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);

extern void  WNP_FD_ZERO(fd_set FAR *set);
extern void  WNP_FD_CLR(int fd, fd_set FAR *set);
extern void  WNP_FD_SET(int fd, fd_set FAR *set);
extern int   WNP_FD_ISSET(int fd, fd_set FAR *set);

extern int   WNP_WaitForActiveRecvSocket(int fd, int msec);
extern int   WNP_WaitForStandByRecvSocket(int fd, int size, int msec);
extern int   WNP_WaitForStandByRecvPipe(int fd, int size, int msec);

/* --------------------------------------------------------------------------- *
 * ネットワークアドレス変換
 * --------------------------------------------------------------------------- */
extern struct hostent *WNP_gethostbyname(const char *name);
extern struct servent *WNP_getservbyname(const char *name, const char *proto);
extern unsigned long   WNP_htonl(unsigned long hostlong);
extern unsigned short  WNP_htons(unsigned short hostshort);
extern unsigned long   WNP_ntohl(unsigned long netlong);
extern in_addr_t       WNP_inet_addr(const char *cp);
extern const char      *WNP_inet_ntop(int __af, const void *__src, char *__dst, size_t __size);

/* --------------------------------------------------------------------------- *
 * ファイル入出力
 * --------------------------------------------------------------------------- */
extern int     WNP_open(const char *pathname, int oflag,...);
extern int     WNP_close(int fildes);
extern int     WNP_dup(int fildes);
extern int     WNP_dup2(int fildes, int fildes2);
extern off_t   WNP_lseek(int fildes, off_t offset, int whence);
extern ssize_t WNP_read(int fildes, void *buf, size_t nbyte);
extern ssize_t WNP_write(int fildes, const void *buf, size_t nbyte);
extern int     WNP_fstat(int fildes, struct stat *buf);
extern int     WNP_fsync(int fildes);
extern int     WNP_fcntl(int fildes, int cmd,...);
extern FILE   *WNP_fopen(const char *pathname, const char *mode);
extern int     WNP_fclose(FILE *stream);

/* --------------------------------------------------------------------------- *
 * ファイル・ディレクトリ制御
 * --------------------------------------------------------------------------- */
extern int            WNP_access(const char *pathname, int amode);
extern int            WNP_chmod(const char *pathname, mode_t mode);
extern int            WNP_stat(const char *pathname, struct stat *buf);
extern int            WNP_lstat(const char *pathname, struct stat *buf);
extern mode_t         WNP_umask(mode_t cmask);
extern int            WNP_link(const char *pathname1, const char *pathname2);
extern int            WNP_unlink(const char *pathname);
extern int            WNP_chdir(const char *pathname);
extern DIR           *WNP_opendir(const char *dirname);
extern struct dirent *WNP_readdir(DIR *dirp);
extern void           WNP_rewinddir(DIR *dirp);
extern int            WNP_closedir(DIR *dirp);
extern char          *WNP_basename(char *path);
extern int            WNP_statfs(const char *path, struct statfs *buf);

/* --------------------------------------------------------------------------- *
 * 時間
 * --------------------------------------------------------------------------- */
extern unsigned int WNP_sleep(unsigned int period);
extern int          WNP_gettimeofday(struct timeval *tp, void *tzp);
extern clock_t      WNP_times(struct tms *__buffer);
extern int          WNP_utime(const char *__path, struct utimbuf *__times);
extern char        *WNP_strptime(const char *s, const char *format, struct tm *tm);

/* --------------------------------------------------------------------------- *
 * シグナル
 * --------------------------------------------------------------------------- */
extern int          WNP_kill(pid_t pid, int sig);
extern void         (*WNP_signal(int sig, void (*disp)(int)))(int);
extern unsigned int WNP_alarm(unsigned int seconds);
extern int          WNP_sighold(int sig);
extern int          WNP_sigpause(int sig);

/* --------------------------------------------------------------------------- *
 * プロセス
 * --------------------------------------------------------------------------- */
extern int   WNP_system(const char *command);
extern pid_t WNP_fork(void);
extern pid_t WNP_execve(const char *path, char *const argv[], char *const envp[]);
extern pid_t WNP_execvp(const char *file, char *const argv[]);
extern pid_t WNP_wait(int *stat_loc);
extern pid_t WNP_waitpid(pid_t pid, int *stat_loc, int options);
extern int   WNP_getpid(void);
extern FILE *WNP_popen(const char *command, const char *mode);
extern int   WNP_pclose(FILE *stream);
extern void  WNP_exit(int exitcode);
extern char *WNP_NutQueryProcessName(pid_t __pid);
extern int   WNP_NutForkExeclp(const char *file, const char *arg0, ...); /* tmp */
extern char *WNP_NutQueryRootDir(int fmt);
extern HANDLE WNP_NutFdToHandle(int fd);

/* --------------------------------------------------------------------------- *
 * ＩＰＣ（共用メモリ・セマフォ・メッセージキュー）
 * --------------------------------------------------------------------------- */
extern void *WNP_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
extern int   WNP_munmap(void *addr, size_t len);
extern int   WNP_msync(void *addr, size_t len, int flags);

extern int   WNP_shmget(key_t __key, size_t __size, int __shmflg);
extern void *WNP_shmat(int __shmid, const void *__shmaddr, int __shmflg);
extern int   WNP_shmdt(const void *__shmaddr);
extern int   WNP_shmctl(int __shmid, int __cmd, struct shmid_ds *__buf);

extern int   WNP_semget(key_t key, int nsems, int semflg);
extern int   WNP_semop(int semid, struct sembuf *sops, size_t nsops);
extern int   WNP_semctl(int semid, int semnum, int cmd,...);

extern int   WNP_msgget(key_t key, int msgflg);
extern int   WNP_msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
extern int   WNP_msgrcv(int msqid, void *msgp, int msgsz, long msgtyp, int msgflg);
extern int   WNP_msgctl(int msqid, int cmd, struct msqid_ds *buf);

extern BOOL  WNP_IPC_IsIpcdAlive();

/* --------------------------------------------------------------------------- *
 * プロセス間通信（オリジナル仕様）
 * --------------------------------------------------------------------------- */
extern int WNP_CmdProc_RecvExtCmdInit(WNP_CmdProc_ptr cmdProc);
extern int WNP_CmdProc_RecvExtCmdTerm(WNP_CmdProc_ptr cmdProc);

extern int WNP_CmdProc_ExecInit(WNP_CmdProc_ptr cmdProc, int (*disp)(char *), void (*cmdExit)(void));
extern int WNP_CmdProc_ExecTerm(WNP_CmdProc_ptr cmdProc);

extern struct WNP_CmdProc *WNP_CmdProc_Create(const char *name);
extern int WNP_CmdProc_Destroy(WNP_CmdProc_ptr cmd);

extern long  WNP_CmdProc_Send(char *pipename, char *cmdbuf);
extern char *WNP_CmdProc_Recv(WNP_CmdProc_ptr cmd);

/* --------------------------------------------------------------------------- *
 * パイプ
 * --------------------------------------------------------------------------- */
extern int    WNP_pipe(int fildes[2]);

/* --------------------------------------------------------------------------- *
 * メモリアクセス
 * --------------------------------------------------------------------------- */
extern void  *WNP_memalign(size_t blocksize, size_t bytes);

/* --------------------------------------------------------------------------- *
 * 正規表現
 * --------------------------------------------------------------------------- */
extern int    WNP_regcomp(regex_t *preg, const char *pattern, int cflags);
extern int    WNP_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
extern size_t WNP_regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
extern void   WNP_regfree(regex_t *preg);

/* --------------------------------------------------------------------------- *
 * 算術演算
 * --------------------------------------------------------------------------- */
extern float  WNP_fabsf(float __x);
extern int    WNP_finite(double __x);
extern float  WNP_powf(float x, float y);
extern float  WNP_sqrtf(float x);
extern float  WNP_expf(float x);
extern double WNP_cbrt(double x);
extern float  WNP_sinf(float x);
extern float  WNP_cosf(float x);
extern float  WNP_tanf(float x);
extern float  WNP_atan2f(float y, float x);
extern float  WNP_acosf(float x);
extern float  WNP_ceilf(float x);
extern float  WNP_floorf(float x);
extern double WNP_rint(double x);
extern float  WNP_rintf(float x);

/* --------------------------------------------------------------------------- *
 * その他
 * --------------------------------------------------------------------------- */
extern int    WNP_getpagesize(void);
extern char  *WNP_getenv(const char *name);
extern int    WNP_putenv(const char *string);
extern int    WNP_vsnprintf(char *s, size_t n, const char *format, va_list ap);

extern int    WNP_getopt(int argc, char * const argv[], const char *optstring);
extern char  *WNP_getprogname();
extern void   WNP_seterrno(int err);

#define	SET_ERRNO(err)	WNP_seterrno((err))

/* --------------------------------------------------------------------------- *
 * Windows Function
 * --------------------------------------------------------------------------- */
/* Define _CRTIMP */
#ifndef _CRTIMP
#ifdef  _DLL
#define _CRTIMP __declspec(dllimport)
#else   /* ndef _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* _CRTIMP */

/* copied from /Program Files/Microsoft Visual Studio/VC98/Include/sys/stat.h */
_CRTIMP int __cdecl _fstat(int, struct _stat *);
_CRTIMP int __cdecl _stat(const char *, struct _stat *);

/* copied from /Program Files/Microsoft Visual Studio/VC98/Include/io.h */
_CRTIMP int __cdecl _umask(int);

/* copied from /Program Files/Microsoft Visual Studio/VC98/Include/process.h */
_CRTIMP int __cdecl _getpid(void);

_CRTIMP unsigned long __cdecl _beginthreadex(void *, unsigned,
        unsigned (__stdcall *) (void *), void *, unsigned, unsigned *);


#endif /* __WIN_NATIVE_PORTING__ */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _WNP_PROTO_H */
