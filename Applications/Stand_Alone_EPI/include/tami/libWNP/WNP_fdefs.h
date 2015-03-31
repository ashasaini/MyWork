#ifndef _WNP_FDEFS_H
#define _WNP_FDEFS_H

/* 
　　Windows Native Porting ライブラリ群の関数マクロ宣言

　　注１）
　　　　可変引数を取る関数の場合、下記の様に宣言するとコンパイルエラーとなる。
　　　　　#define func(a, b, c, ...)          WNP_func(a, b, c, ...)
　　　　その為、下記の様に宣言する。
　　　　　#define func                        WNP_func

　　注２）
　　　　mmapは固定引数を取るが、ソースコード上の多数のマクロによってコンパイルエラーとなる。
　　　　その為、下記の様に宣言する。
　　　　　#define mmap                        WNP_mmap
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN_NATIVE_PORTING__

/* =========================================================================== *
 * libWNPcore
 * =========================================================================== */

/* --------------------------------------------------------------------------- *
 * ファイル入出力
 * --------------------------------------------------------------------------- */
#define open                            WNP_open
#define close(a)                        WNP_close(a)
#define dup(a)                          WNP_dup(a)
#define dup2(a, b)                      WNP_dup2(a, b)
#define lseek(a, b, c)                  WNP_lseek(a, b, c)
#define read(a, b, c)                   WNP_read(a, b, c)
#define write(a, b, c)                  WNP_write(a, b, c)
#define fstat(a, b)                     WNP_fstat(a, b)
#define fsync(a)                        WNP_fsync(a)
#define fcntl                           WNP_fcntl
#define fopen(a, b)                     WNP_fopen(a, b)
#define fclose(a)                       WNP_fclose(a)
#define creat(a, b)                     WNP_open(a, O_CREAT|O_WRONLY|O_TRUNC, b)

/* --------------------------------------------------------------------------- *
 * ファイル・ディレクトリ制御
 * --------------------------------------------------------------------------- */
#define access(a, b)                    WNP_access(a, b)
#define chmod(a, b)                     WNP_chmod(a, b)
#define stat(a, b)                      WNP_stat(a, b)
#define lstat(a, b)                     WNP_lstat(a, b)
#define umask(a)                        WNP_umask(a)
#define link(a, b)                      WNP_link(a, b)
#define unlink(a)                       WNP_unlink(a)
#define chdir(a)                        WNP_chdir(a)
#define opendir(a)                      WNP_opendir(a)
#define readdir(a)                      WNP_readdir(a)
#define rewinddir(a)                    WNP_rewinddir(a)
#define closedir(a)                     WNP_closedir(a)
#define basename(a)                     WNP_basename(a)
#define statfs(a, b)                    WNP_statfs(a, b)

/* --------------------------------------------------------------------------- *
 * メモリアクセス
 * --------------------------------------------------------------------------- */
#define bzero(s, n)                     memset(s, 0, n)
#define bcopy(s1, s2, n)                memcpy(s2, s1, n)

/* --------------------------------------------------------------------------- *
 * 時間
 * --------------------------------------------------------------------------- */
#define gettimeofday(a, b)              WNP_gettimeofday(a, b)
#define times(a)                        WNP_times(a)
#define utime(a, b)                     WNP_utime(a, b)
#define strptime(a, b, c)               WNP_strptime(a, b, c)

/* --------------------------------------------------------------------------- *
 * プロセス
 * --------------------------------------------------------------------------- */
#define getpid()                        WNP_getpid()
#define popen(a, b)                     WNP_popen(a, b)
#define pclose(a)                       WNP_pclose(a)
#define _NutQueryProcessName(a)         WNP_NutQueryProcessName(a)
#define _NutQueryRootDir(a)             WNP_NutQueryRootDir(a)
#define _NutFdToHandle(a)               WNP_NutFdToHandle(a)

/* #define mmap(a, b, c, d, e, f)        WNP_mmap(a, b, c, d, e, f) */
#define mmap                            WNP_mmap
#define munmap(a, b)                    WNP_munmap(a, b)
#define msync(a, b, c)                  WNP_msync(a, b, c)

/* --------------------------------------------------------------------------- *
 * その他
 * --------------------------------------------------------------------------- */
#define getpagesize()                   WNP_getpagesize()
#define finite(a)                       WNP_finite(a)
#define cbrt(a)                         WNP_cbrt(a)

/* c++の場合はmath.hに宣言されている */
#ifndef __cplusplus
#define fabsf(a)                        WNP_fabsf(a)
#define powf(a, b)                      WNP_powf(a, b)
#define sqrtf(a)                        WNP_sqrtf(a)
#define expf(a)                         WNP_expf(a)
#define sinf(a)                         WNP_sinf(a)
#define cosf(a)                         WNP_cosf(a)
#define tanf(a)                         WNP_tanf(a)
#define atan2f(a, b)                    WNP_atan2f(a, b)
#define acosf(a)                        WNP_acosf(a)
#define ceilf(a)                        WNP_ceilf(a)
#define floorf(a)                       WNP_floorf(a)
#endif // __cplusplus

#define rint(a)                         WNP_rint(a)
#define rintf(a)                        WNP_rintf(a)
#define memalign(a, b)                  WNP_memalign(a, b)
#define regcomp(a, b, c)                WNP_regcomp(a, b, c)
#define regexec(a, b, c, d, e)          WNP_regexec(a, b, c, d, e)
#define regerror(a, b, c, d)            WNP_regerror(a, b, c, d)
#define regfree(a)                      WNP_regfree(a)
#define vsnprintf(a, b, c, d)           WNP_vsnprintf(a, b, c, d)
#define pipe(a)                         WNP_pipe(a)
#define getenv(a)                       WNP_getenv(a)
#define putenv(a)                       WNP_putenv(a)

#ifndef MAX
#define MAX(a,b)        ((b) > (a) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a,b)        ((b) < (a) ? (b) : (a))
#endif

#define getopt(a,b,c)                  WNP_getopt(a,b,c)

#define strcasecmp(a, b)               _stricmp(a, b)

/* =========================================================================== *
 * libWNPsocket
 * =========================================================================== */

#ifdef __WNP_SOCKET__
/* --------------------------------------------------------------------------- *
 * ソケット
 * --------------------------------------------------------------------------- */
#define socket(a, b, c)                 WNP_socket(a, b, c)
#define bind(a, b, c)                   WNP_bind(a, b, c)
#define listen(a, b)                    WNP_listen(a, b)
#define accept(a, b, c)                 WNP_accept(a, b, c)
#define connect(a, b, c)                WNP_connect(a, b, c)
#define select(a, b, c, d, e)           WNP_select(a, b, c, d, e)
#define recv(a, b, c, d)                WNP_recv(a, b, c, d)
#define send(a, b, c, d)                WNP_send(a, b, c, d)
#define getsockname(a, b, c)            WNP_getsockname(a, b, c)
#define getpeername(a, b, c)            WNP_getpeername(a, b, c)
#define getsockopt(a, b, c, d, e)       WNP_getsockopt(a, b, c, d, e)
#define setsockopt(a, b, c, d, e)       WNP_setsockopt(a, b, c, d, e)

#undef  FD_ZERO
#undef  FD_CLR
#undef  FD_SET
#undef  FD_ISSET

#define FD_ZERO(a)                      WNP_FD_ZERO(a)
#define FD_CLR(a, b)                    WNP_FD_CLR(a, b)
#define FD_SET(a, b)                    WNP_FD_SET(a, b)
#define FD_ISSET(a, b)                  WNP_FD_ISSET(a, b)

/* --------------------------------------------------------------------------- *
 * ネットワークアドレス変換
 * --------------------------------------------------------------------------- */
#define gethostbyname(a)                WNP_gethostbyname(a)
#define getservbyname(a, b)             WNP_getservbyname(a, b)
#define htonl(a)                        WNP_htonl(a)
#define htons(a)                        WNP_htons(a)
#define ntohl(a)                        WNP_ntohl(a)
#define inet_addr(a)                    WNP_inet_addr(a)
#define inet_ntop(a, b, c, d)           WNP_inet_ntop(a, b, c, d)

#endif // __WNP_SOCKET__


/* =========================================================================== *
 * libWNPsignal
 * =========================================================================== */
#ifdef __WNP_SIGNAL__

/* --------------------------------------------------------------------------- *
 * 時間 SIGNAL
 * --------------------------------------------------------------------------- */
#define sleep(a)                        WNP_sleep(a)

/* --------------------------------------------------------------------------- *
 * シグナル
 * --------------------------------------------------------------------------- */
#define kill(a, b)                      WNP_kill(a, b)
#define signal(a, b)                    WNP_signal(a, b)
#define sigset(a, b)                    WNP_signal(a, b)
#define sigignore(a)                    WNP_signal(a, SIG_IGN)
#define alarm(a)                        WNP_alarm(a)
#define sighold(a)                      WNP_sighold(a)
#define sigpause(a)                     WNP_sigpause(a)

#define siglongjmp                      longjmp
#define sigsetjmp(a, b)                 setjmp(a) 

/* --------------------------------------------------------------------------- *
 * プロセス
 * --------------------------------------------------------------------------- */
#define system(a)                       WNP_system(a)
#define fork()                          WNP_fork()
#define vfork()                         WNP_fork()
#define execv(a, b)                     WNP_execvp(a, b)
#define execvp(a, b)                    WNP_execvp(a, b)
#define execve(a, b, c)                 WNP_execve(a, b, c)
#define execvpe(a, b, c)                WNP_execve(a, b, c)
#define wait(a)                         WNP_wait(a)
#define waitpid(a, b, c)                WNP_waitpid(a, b, c)
#define exit(a)                         WNP_exit(a)
#define _NutForkExeclp                  WNP_NutForkExeclp

/* --------------------------------------------------------------------------- *
 * ＩＰＣ（共用メモリ・セマフォ・メッセージキュー）
 * --------------------------------------------------------------------------- */
#define shmget(a, b, c)                 WNP_shmget(a, b, c)
#define shmat(a, b, c)                  WNP_shmat(a, b, c)
#define shmdt(a)                        WNP_shmdt(a)
#define shmctl(a, b, c)                 WNP_shmctl(a, b, c)

/* #define semget(a, b, c)               WNP_semget(a, b, c) */
#define semget                          WNP_semget
#define semop(a, b, c)                  WNP_semop(a, b, c)
/* #define semctl(a, b, c, ...)          WNP_semctl(a, b, c, ...) */
#define semctl                          WNP_semctl

#define msgget(a, b)                    WNP_msgget(a, b)
#define msgsnd(a, b, c, d)              WNP_msgsnd(a, b, c, d)
#define msgrcv(a, b, c, d, e)           WNP_msgrcv(a, b, c, d, e)
#define msgctl(a, b, c)                 WNP_msgctl(a, b, c)

#endif // __WNP_SIGNAL__

#endif /* __WIN_NATIVE_PORTING__ */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _WNP_FDEFS_H */
