#ifndef _WNP_TYPES_H
#define _WNP_TYPES_H

/* 
　　Windows Native Porting に必要な型定義をMKS-includeから抜粋
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN_NATIVE_PORTING__

#include <windows.h>    /* include windows.h */

#include "WNP_X_proto.h"

#include <tami/common/basicdefs.h>

/* for windows.h (tmp)*/
#ifndef __WNP_PATNI__
#undef SendMessage
#endif

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/size_t.h */
typedef unsigned int		size_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/intptr_t.h */
#ifdef  _WIN64
typedef unsigned __int64	uintptr_t;
typedef __int64			intptr_t;
#else
typedef int			intptr_t;
typedef unsigned int		uintptr_t;
#endif

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/inttypes.h */
#if 0 // it copied to include/tami/common/machdefs.h
typedef char			int8_t;
typedef short			int16_t;
typedef long			int32_t;
typedef __int64			int64_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
#ifndef uint32_t
typedef unsigned long		uint32_t;
#endif
typedef unsigned __int64	uint64_t;

#define _SYS_TYPES_H
#endif
/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/key_t.h */
typedef long			key_t;

/* --------------------------------------------------------------------------*/
/* copy form /Program Files/NuTCROOT/include/nutc/unsigned.h */
typedef unsigned long		ulong;
typedef unsigned short		ushort;
typedef unsigned char		unchar;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/pid_t.h */
typedef long			pid_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/uid_t.h */
typedef unsigned long		uid_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/gid_t.h */
typedef unsigned long		gid_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/off_t.h */
typedef long			off_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/caddr_t.h */
typedef char			*caddr_t;

/* --------------------------------------------------------------------------*/
/* /Program Files/NuTCROOT/include/nutc/quad_t.h */
typedef __int64			quad_t;

/* --------------------------------------------------------------------------*/
/* copy from develop/src/mathServer/imgReg/include/msmanDebug.h */
#define VREG_TTY        "/tmp/VReg.tty"

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/path_max.h */
#define PATH_MAX	256		/* max path name length */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/un.h */
typedef short		sa_family_t;

struct sockaddr_un
{
	sa_family_t	sun_family;
	char		sun_path[PATH_MAX];
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/time_t.h */
typedef long		time_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/clock_t.h */
typedef long		clock_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/times.h */
struct tms
{
	clock_t	tms_utime;
	clock_t	tms_stime;
	clock_t	tms_cutime;
	clock_t	tms_cstime;
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/Microsoft Visual Studio/VC98/Include/SYS/UTIME.H */
#ifndef _UTIMBUF_DEFINED

struct _utimbuf {
        time_t actime;          /* access time */
        time_t modtime;         /* modification time */
        };

#if     !__STDC__
/* Non-ANSI name for compatibility */
struct utimbuf {
        time_t actime;          /* access time */
        time_t modtime;         /* modification time */
        };
#endif

#define _UTIMBUF_DEFINED
#endif

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/tzname.h */
extern int	___NutDaylight;
extern long	___NutTimezone;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/signal.h */
#undef SIG_DFL
#undef SIG_IGN
#undef SIG_ERR
#undef SIGABRT

typedef void		(*__handler)(int);

#define SIG_DFL		((__handler)0)
#define SIG_ERR		((__handler)-1)
#define SIG_IGN		((__handler)1)
#define SIG_HOLD	((__handler)2)

#define SIGHUP		1
#define SIGINT		2
#define SIGQUIT		3
#define SIGILL		4
#define SIGTRAP		5
#define SIGABRT		6
#define SIGIOT		SIGABRT
#define SIGEMT		7
#define SIGFPE		8
#define SIGKILL		9
#define SIGBUS		10
#define SIGSEGV		11
#define SIGSYS		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGUSR1		16
#define SIGUSR2		17
#define SIGCHLD		18
#define SIGCLD		SIGCHLD
#define SIGPWR		19
#define SIGWINCH	20
#define SIGURG		21
#define SIGIO		22
#define SIGPOLL		SIGIO
#if 1				/* __NC_INTERNAL__ */
# define SIGSTOP	23
# define SIGTSTP	24
# define SIGCONT	25
# define SIGTTIN	26
# define SIGTTOU	27
#endif
#define SIGVTALRM	28
#define SIGPROF		29
#define SIGXCPU		30
#define SIGXFSZ		31

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/mman.h */
#define PROT_READ	0x04		/* pages can be read */
#define PROT_WRITE	0x02		/* pages can be written */
#define PROT_EXEC	0x01		/* pages can be executed */
#define PROT_NONE	0x00		/* pages can't be anything */

#define MAP_SHARED	0x0001		/* changes are visible to all proc */
#define MAP_PRIVATE	0x0002		/* changes are private to process */
#define MAP_FIXED	0x0010		/* interpret 'addr' exactly */

/*
 * Flags for msync()
 */
#define MS_ASYNC	0x0001		/* perform async writes */
#define MS_INVALIDATE	0x0002		/* invalidate mappings */
#define MS_SYNC		0x0004		/* perform sync writes */

/* mmap failure value */
#define MAP_FAILED      ((void *) -1)  

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/param.h */
#define MAXHOSTNAMELEN	256		/* max hostname size */
#define NOFILE		2048		/* max open files per process */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/netinet/in.h */
#define IPPROTO_TCP	6		/* tcp */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/netinet/tcp.h */
#if 0
#define TCP_NODELAY	0x01		/* don't delay send to coalesce packets */
#endif

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/socket.h */
#define AF_INET6	23		/* Inetnetwork version 6 */
#define AF_UNIX		1		/* local to host (pipes, portals) */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/mode_t.h */
typedef unsigned long	mode_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/errno.h */
#define EIO		5		/* I/O error */
#define ENXIO		6		/* No such device or address */
#define EWOULDBLOCK	64		/* Operation would block */
#define EINPROGRESS	65		/* Operation now in progress */
#define ESOCKTNOSUPPORT 73		/* Socket type not supported */
#define EHOSTDOWN	93		/* Host is down */

#define EZERO		0	/* Error 0 */
#define EPERM		1	/* Operation not permitted */
#define ENOENT		2	/* No such file or directory */
#define ESRCH		3	/* No such process */
#define EINTR		4	/* Interrupted function */
#define EIO		5	/* I/O error */
#define ENXIO		6	/* No such device or address */
#define E2BIG		7	/* Argument list too long */
#define ENOEXEC		8	/* Executable file format error */
#define EBADF		9	/* Bad file number */
#define ECHILD		10	/* No child processes */
#define EAGAIN		11	/* Resource unavailable; try again */
#define ENOMEM		12	/* Not enough space */
#define EACCES		13	/* Permission denied */
#define EFAULT		14	/* Bad address */
#define ENOTBLK		15	/* Block device required */
#define EBUSY		16	/* Device or resource busy */
#define EEXIST		17	/* File exists */
#define EXDEV		18	/* Cross-device link */
#define ENODEV		19	/* No such device */
#define ENOTDIR		20	/* Not a directory */
#define EISDIR		21	/* Is a directory */
#define EINVAL		22	/* Invalid argument */
#define ENFILE		23	/* File table overflow */
#define EMFILE		24	/* Too many open files */
#define ENOTTY		25	/* Inappropriate I/O control operation */
#define ETXTBSY		26	/* Text file busy */
#define EFBIG		27	/* File too large */
#define ENOSPC		28	/* No space left on device */
#define ESPIPE		29	/* Invalid seek */
#define EROFS		30	/* Read-only file system */
#define EMLINK		31	/* Too many links */
#define EPIPE		32	/* Broken pipe */
#define EDOM		33	/* Math argument out of domain of function */
#define ERANGE		34	/* Result too large */
#define EDEADLK		36	/* Resource deadlock would occur */
#define ENAMETOOLONG	38	/* Filename too long */
#define ENOLCK		39	/* No locks available */
#define ENOSYS		40	/* Function not implemented */
#define ENOTEMPTY	41	/* Directory not empty */
#define EILSEQ		42	/* Illegal byte sequence */

#define EWOULDBLOCK	64	/* Operation would block */
#define EINPROGRESS	65	/* Operation now in progress */
#define EALREADY	66	/* Operation already in progress */
#define ENOTSOCK	67	/* Socket operation on non-socket */
#define EDESTADDRREQ	68	/* Destination address required */
#define EMSGSIZE	69	/* Message too long */
#define EPROTOTYPE	70	/* Protocol wrong type for socket */
#define ENOPROTOOPT	71	/* Protocol not available */
#define EPROTONOSUPPORT 72	/* Protocol not supported */
#define ESOCKTNOSUPPORT 73	/* Socket type not supported */
#define EOPNOTSUPP	74	/* Operation not supported on socket */
#define EPFNOSUPPORT	75	/* Protocol family not supported */
#define EAFNOSUPPORT	76	/* Addr family not supported by proto family */
#define EADDRINUSE	77	/* Address already in use */
#define EADDRNOTAVAIL	78	/* Can't assign requested address */
#define ENETDOWN	79	/* Network is down */
#define ENETUNREACH	80	/* Network is unreachable */
#define ENETRESET	81	/* Network dropped connection on reset */
#define ECONNABORTED	82	/* Software caused connection abort */
#define ECONNRESET	83	/* Connection reset by peer */
#define ENOBUFS		84	/* No buffer space available */
#define EISCONN		85	/* Socket is already connected */
#define ENOTCONN	86	/* Socket is not connected */
#define ESHUTDOWN	87	/* Can't send after socket shutdown */
#define ETOOMANYREFS	88	/* Too many references: can't splice */
#define ETIMEDOUT	89	/* Connection timed out */
#define ECONNREFUSED	90	/* Connection refused */
#define ELOOP		91	/* Too many levels of symbolic links */
#define EHOSTDOWN	93	/* Host is down */
#define EHOSTUNREACH	94	/* No route to host */
#define EPROCLIM	96	/* Too many processes */
#define EUSERS		97	/* Too many users */
#define EDQUOT		98	/* Disc quota exceeded */
#define ESTALE		99	/* Stale NFS file handle */
#define EREMOTE		100	/* Too many levels of remote in path */

#define ENOSTR		130	/* Device is not a stream */
#define ETIME		131	/* Timer expired */
#define ENOSR		132	/* Out of streams resources */
#define ENOMSG		133	/* No message of desired type */
#define EBADMSG		134	/* Trying to read unreadable message */
#define EIDRM		135	/* Identifier removed */
#define ENONET		136	/* Machine is not on the network */
#define ERREMOTE	137	/* Object is remote */
#define ENOLINK		138	/* Link has been severed */
#define EADV		139	/* Advertise error */
#define ESRMNT		140	/* srmount error */
#define ECOMM		141	/* Communication error on send */
#define EPROTO		142	/* Protocol error */
#define EMULTIHOP	143	/* Multihop attempted */
#define EDOTDOT		144	/* Cross mount point (not an error) */
#define EREMCHG		145	/* Remote address changed */
#define EDISCON		146	/* Circuit gracefully terminated */
#define ECANCELED	147	/* Operation canceled */
#define ENOTSUP		148	/* Not supported */
#define EFTYPE		149	/* Inappropriate file type or format */

/* original errno of libWNP */
#define EIPCDDEAD   150 /* WNP_ipcd died */
#define EIPCDNOTIFY 151 /* failed to notify IPC info to WNP_ipcd */
#define EIPCDNOTIFYDEAD 152 /* failed to notify IPC info to WNP_ipcd and terminated WNP_ipcd */
#define EIPCDUPDATE 153 /* WNP_ipcd failed to update IPC info list and terminated WNP_ipcd */
#define ELOCKCNFLCT 154 /* LockFileEx()/UnlockFileEx failed in WNP_fcntl and restored lock info */
#define ELOCKBREAK  155 /* LockFileEx()/UnlockFileEx failed in WNP_fcntl, failed to restore lock info and terminated WNP_ipcd */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/stat.h */
/* MKS の S_IXUSR, S_IWUSR, S_IRUSR と Windows の S_IEXEC, S_IWRITE, S_IREAD */
/* は同じ値                                                                  */
#define S_IXOTH		0000000001
#define S_IWOTH		0000000002
#define S_IROTH		0000000004
#define S_IXGRP		0000000010
#define S_IWGRP		0000000020
#define S_IRGRP		0000000040
#define S_IXUSR		0000000100
#define S_IWUSR		0000000200
#define S_IRUSR		0000000400
#if defined(__USE_MISC)
# define S_ISVTX	0000001000
#endif /* __USE_MISC */
#define S_ISGID		0000002000
#define S_ISUID		0000004000

/*
 * Access mode helper macros defined in POSIX.1 (1988)
 */
#define S_IRWXO		(S_IROTH|S_IWOTH|S_IXOTH)
#define S_IRWXG		(S_IRGRP|S_IWGRP|S_IXGRP)
#define S_IRWXU		(S_IRUSR|S_IWUSR|S_IXUSR)


#define __S_IFMT	0000170000
#define __S_IAMB	0177607777

#define __S_IFIFO	0000010000
#define __S_IFCHR	0000020000
#define __S_IFDIR	0000040000
#define __S_IFBLK	0000060000
#define __S_IFREG	0000100000
#define __S_IFCTG	0000110000
#define __S_IFLNK	0000120000
#define __S_IFSOCK	0000140000

# define S_IFLNK	__S_IFLNK
# define S_IFSOCK	__S_IFSOCK
# define S_IFDIR	__S_IFDIR

#define S_ISDIR(_m)	(((_m) & __S_IFMT) == __S_IFDIR)

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/include/fcntl.h */
struct flock
{
	short	l_type;
	short	l_whence;
	off_t	l_start;
	off_t	l_len;
	long	l_sysid;
	pid_t	l_pid;
	long	l_pad[4];
};

#define F_DUPFD		0
#define F_GETFD		1
#define F_SETFD		2
#define F_GETFL		3
#define F_SETFL		4
#define F_GETLK		5
#define F_SETLK		6
#define F_SETLKW	7

#define FD_CLOEXEC	0x00000001 /* flag for F_SETFD */

#define __O_NONBLOCK	0x00000000	// #define __O_NONBLOCK	0x00000080
#define O_NONBLOCK	__O_NONBLOCK

#define __O_NDELAY	0x00000000	// #define __O_NDELAY	0x00000004
#define O_NDELAY	__O_NDELAY

#define __O_NOCTTY	0x00000000
#define O_NOCTTY	__O_NOCTTY

#define FAPPEND     O_APPEND
#define FASYNC      O_ASYNC
#define FCREAT      O_CREAT
#define FEXCL       O_EXCL
#define FNDELAY     O_NDELAY
#define FTRUNC      O_TRUNC

#define F_RDLCK		1
#define F_UNLCK		2
#define F_WRLCK		3
//#define F_RDLCK		1
//#define F_WRLCK		2
//#define F_UNLCK		3

/* copy from /Program Files/Microsoft Visual Studio/VC98/Include/FCNTL.H */
#ifndef _INC_FCNTL
#define _INC_FCNTL

#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at eof */

#define _O_CREAT        0x0100  /* create and open file */
#define _O_TRUNC        0x0200  /* open and truncate */
#define _O_EXCL         0x0400  /* open only if file doesn't already exist */

/* O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
** and <lf> sequences translated to <cr><lf> on write()'s
*/

#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */

/* macro to translate the C 2.0 name used to force binary mode for files */

#define _O_RAW  _O_BINARY

/* Open handle inherit bit */

#define _O_NOINHERIT    0x0080  /* child process doesn't inherit file */

/* Temporary file bit - file is deleted when last handle is closed */

#define _O_TEMPORARY    0x0040  /* temporary file bit */

/* temporary access hint */

#define _O_SHORT_LIVED  0x1000  /* temporary storage file, try not to flush */

/* sequential/random access hints */

#define _O_SEQUENTIAL   0x0020  /* file access is primarily sequential */
#define _O_RANDOM       0x0010  /* file access is primarily random */

#if     !__STDC__ || defined(_POSIX_)
/* Non-ANSI names for compatibility */
#define O_RDONLY        _O_RDONLY
#define O_WRONLY        _O_WRONLY
#define O_RDWR          _O_RDWR
#define O_APPEND        _O_APPEND
#define O_CREAT         _O_CREAT
#define O_TRUNC         _O_TRUNC
#define O_EXCL          _O_EXCL
#define O_TEXT          _O_TEXT
#define O_BINARY        _O_BINARY
#define O_RAW           _O_BINARY
#define O_TEMPORARY     _O_TEMPORARY
#define O_SHORT_LIVED   _O_SHORT_LIVED
#define O_NOINHERIT     _O_NOINHERIT
#define O_SEQUENTIAL    _O_SEQUENTIAL
#define O_RANDOM        _O_RANDOM
#endif  /* __STDC__ */

/*
 * Mask to separate the access mode for the file from the various open-mode
 * flags.
 */
#define O_ACCMODE	(O_RDONLY|O_WRONLY|O_RDWR)

#endif  /* _INC_FCNTL */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/Microsoft Visual Studio/VC98/Include/SYS/TYPES.H */

typedef unsigned int _dev_t;
typedef unsigned short _ino_t;          /* i-node number (not used on DOS) */
typedef long _off_t;                    /* file offset value */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/Microsoft Visual Studio/VC98/Include/SYS/STAT.H */

#ifndef _STAT_DEFINED

struct _stat {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        _off_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#if     !__STDC__

/* Non-ANSI names for compatibility */

struct stat {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        _off_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#endif  /* __STDC__ */

#if     _INTEGRAL_MAX_BITS >= 64
struct _stati64 {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        __int64 st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };
#endif

#define _STAT_DEFINED

#endif /* _STAT_DEFINED */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/Microsoft Visual Studio/VC98/Include/SYS/PROCESS.H */
#define _P_WAIT         0
#define _P_NOWAIT       1
#define _OLD_P_OVERLAY  2
#define _P_NOWAITO      3
#define _P_DETACH       4

#define P_WAIT          _P_WAIT
#define P_NOWAIT        _P_NOWAIT

#define OLD_P_OVERLAY   _OLD_P_OVERLAY
#define P_NOWAITO       _P_NOWAITO
#define P_DETACH        _P_DETACH

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/math.h */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_E		2.7182818284590452354	/* e */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/unsigned.h */
typedef	unsigned int	u_int;
typedef unsigned long	u_long;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/unistd.h */
#define F_OK		0x0000
#define X_OK		0x0001
#define W_OK		0x0002
#define R_OK		0x0004

extern char *optarg;
extern int optind, opterr, optopt;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/ipc.h */
#define IPC_CREAT	0001000
#define IPC_EXCL	0002000
#define IPC_NOWAIT	0004000

#define IPC_ALLOC	0100000

#define IPC_PRIVATE	(key_t)0

#define IPC_RMID	0
#define IPC_SET		1
#define IPC_STAT	2

#define IPC_W		0000200
#define IPC_R		0000400


struct ipc_perm
{
	uid_t		uid;
	gid_t		gid;
	uid_t		cuid;
	gid_t		cgid;
	mode_t		mode;
	unsigned long	seq;
	key_t		key;
	char		*ipc_secp; /* should be (struct ipc_sec *) */
	unsigned long	pad[3];
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/off_t.h */
typedef long		off_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/resource.h */
struct  rusage {
	struct timeval ru_utime;        /* user time used */
	struct timeval ru_stime;        /* system time used */
	long    ru_spare[14];		/* Remaining fields. */
};                              

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/time.h */
struct timezone
{
	int		dontuse_tz_minuteswest;
	int		dontuse_tz_dsttime;
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/wait.h */
#define WNOHANG		0x01

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/regex.h */

/* regcomp() flags */
#define REG_BASIC	0000
#define REG_EXTENDED	0001
#define REG_ICASE	0002
#define REG_NOSUB	0004
#define REG_NEWLINE	0010
#define REG_NOSPEC	0020
#define REG_PEND	0040
#define REG_DUMP	0200

/* regexec() flags */
#define REG_NOTBOL	00001
#define REG_NOTEOL	00002
#define REG_STARTEND	00004
#define REG_TRACE	00400
#define REG_LARGE	01000
#define REG_BACKR	02000

#define REG_NOMATCH	1
#define REG_BADPAT	2
#define REG_ECOLLATE	3
#define REG_ECTYPE	4
#define REG_EESCAPE	5
#define REG_ESUBREG	6
#define REG_EBRACK	7
#define REG_EPAREN	8
#define REG_EBRACE	9
#define REG_BADBR 	10
#define REG_ERANGE	11
#define REG_ESPACE	12
#define REG_BADRPT	13
#define REG_EMPTY	14
#define REG_ASSERT	15
#define REG_INVARG	16

#define REG_ATOI	255
#define REG_ITOA	0400


typedef off_t		regoff_t;

typedef struct
{
	int		re_magic;
	size_t		re_nsub;
	const char	*re_endp;
	struct re_guts	*re_g;
} regex_t;

typedef struct
{
	regoff_t	rm_so;
	regoff_t	rm_eo;
} regmatch_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/statvfs.h */
#define FSTYPSZ		16

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/mount.h */
typedef quad_t		fsid_t;
#define MFSNAMELEN	FSTYPSZ	/* length of fs type name, including null */
#define MNAMELEN	90	/* length of buffer for returned name */
struct statfs
{
	short	f_type;			/* type of filesystem (see below) */
	short	f_flags;		/* copy of mount flags */
	long	f_bsize;		/* fundamental file system block size */
	long	f_iosize;		/* optimal transfer block size */
	long	f_blocks;		/* total data blocks in file system */
	long	f_bfree;		/* free blocks in fs */
	long	f_bavail;		/* free blocks avail to non-superuser */
	long	f_files;		/* total file nodes in file system */
	long	f_ffree;		/* free file nodes in fs */
	fsid_t	f_fsid;			/* file system id */
	long	f_maxlen;		/* maximum length of filename in FS */
	uid_t	f_owner;		/* user that mounted the filesystem */
	long	f_spare[3];		/* spare for later */
	char	f_fstypename[MFSNAMELEN]; /* fs type name */
	char	f_mntonname[MNAMELEN];	/* directory on which mounted */
	char	f_mntfromname[MNAMELEN];/* mounted filesystem */
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/sem.h */

struct sem
{
	unsigned short	semval;  /* セマフォ値 */
	pid_t		sempid;  /* 最後に操作を行ったプロセスID */
	unsigned short	semncnt; /* 増加を待つプロセス数 */
	unsigned short	semzcnt; /* ゼロを待つプロセス数 */
};

struct semid_ds
{
	struct ipc_perm	sem_perm;  /* 所有権と許可 */
	struct sem	*sem_base; /* セマフォ集合の配列へのポインタ */
	unsigned short	sem_nsems; /* 集合内のセマフォの数 */
	time_t		sem_otime; /* 最後のsemopの時間 */
	unsigned long	sem_pad1;
	time_t		sem_ctime; /* 作成した時間 */
	unsigned long	sem_pad2;
	unsigned long	sem_pad3[4];
};

struct sembuf
{
	unsigned short	sem_num; /* セマフォ番号 */
	short		sem_op;  /* セマフォ操作 */
	short		sem_flg; /* 操作フラグ */
};

union semun
{
  int val;                      /* SETVALの値 */
  struct semid_ds *buf;         /* IPC_STAT, IPC_SET用のバッファ */
  unsigned short *array;        /* GETALL, SETALL用のバッファ */
//} arg;
} ; // エラー起きたのでargを削除。argがなぜあったか不明。

#define SEM_UNDO	010000

#define GETNCNT		3
#define GETPID		4
#define GETVAL		5
#define GETALL		6
#define GETZCNT		7
#define SETVAL		8
#define SETALL		9

# define SEM_A		IPC_W
# define SEM_R		IPC_R

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/shm.h */
# define SHM_R		IPC_R
# define SHM_W		IPC_W
# define SHM_RDONLY     IPC_R
typedef unsigned short	shmatt_t;

struct shmid_ds
{
	struct ipc_perm	shm_perm;   /* 所有権と許可 */
	size_t		shm_segsz;  /* 共有メモリ・セグメントのサイズ */
	pid_t		shm_lpid;   /* 最後にshmat/shmdtしたプロセス */
	pid_t		shm_cpid;   /* 作成者のプロセス */
	shmatt_t	shm_nattch; /* 現在アタッチしている数 */
	time_t		shm_atime;  /* 最後にアタッチした時間 */
	time_t		shm_dtime;  /* 最後にデタッチした時間 */
	time_t		shm_ctime;  /* 作成した時間 */
	void		*shm_handle;
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/tcflag_t.h */
typedef unsigned long	tcflag_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/speed_t.h */
typedef unsigned long	speed_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/cc_t.h */
#define NCCS		20

typedef unsigned char	cc_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/termios.h */
struct termios
{
	tcflag_t	c_iflag;	/* input flags */
	tcflag_t	c_oflag;	/* output flags */
	tcflag_t	c_cflag;	/* control flags */
	tcflag_t	c_lflag;	/* local flags */
	cc_t		c_cc[NCCS];	/* control chars */
	speed_t		c_ispeed;	/* input speed */
	speed_t		c_ospeed;	/* output speed */
};

#define BRKINT		0x00000002	/* map BREAK to SIGINTR */
#define INLCR		0x00000040	/* map NL into CR */
#define ICRNL		0x00000100	/* map CR to NL (ala CRMOD) */
# define IUCLC		0x00000200	/* map upper-case to lower-case */
#define IXON		0x00000400	/* enable output flow control */

#define OPOST		0x00000001	/* enable following output processing */
# define ONLCR		0x00000004	/* map NL to CR-NL (ala CRMOD) */

#define B9600		13
# define CBAUD		0x0000000F	/* ??? */

#define TCSANOW		0		/* make change immediate */

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/machine/param.h */
#define NBPG		4096		/* bytes/page */
#define DEV_BSIZE	512

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/ioccom.h */

#define IOCPARM_MAX	NBPG		/* max size of ioctl args */
#define IOC_INOUT	(IOC_IN|IOC_OUT)/* copy paramters in and out */

#define _IOC(inout,group,num,len) \
	(inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))
#define _IOWR(g,n,t)	_IOC(IOC_INOUT,	(g), (n), sizeof(t))

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/sys/msg.h */
#define MSG_NOERROR	010000
# define MSGPOOL	8		/* size in kilobytes of message pool. */
# define MSGMAP		100		/* # of entries in msg map. */
# define MSGMAX		(MSGPOOL * 1024) /* max message size in bytes. */
# define MSGMNB		MSGMAX		/* default max # of bytes on queue. */

typedef unsigned long	msgqnum_t;
typedef unsigned long	msglen_t;

struct msqid_ds
{
	struct ipc_perm	msg_perm;   /* 所有権と許可 */
	void		*msg_first;
	void		*msg_last;
	msglen_t	msg_cbytes; /* メッセージキューに現在入っているバイト数 */
	msgqnum_t	msg_qnum;   /* メッセージキューに現在入っているメッセージの数 */
	msglen_t	msg_qbytes; /* メッセージキューに許可されるバイト数 */
	pid_t		msg_lspid;  /* 最後に msgsnd() したプロセス */
	pid_t		msg_lrpid;  /* 最後に msgrcv() したプロセス */
	time_t		msg_stime;  /* 最後の msgsnd() の時間 */
	unsigned long	msg_spare0;
	time_t		msg_rtime;  /* 最後の msgrcv() の時間 */
	unsigned long	msg_spare1;
	time_t		msg_ctime;  /* 作成した時間 */
	unsigned long	msg_spare2;
	int		msg_rcnt;
	int		msg_wcnt;
	unsigned long	msg_spare3[2];
};

struct msgbuf
{
	long		mtype;
	char		mtext[1];
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/size_t.h */
typedef unsigned int	size_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/ssize_t.h */
typedef signed int	ssize_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/mode_t.h */
typedef unsigned long	mode_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/wait.h */
/* modified for WNP */
/*
pid_t wait(int *stat_loc);

stat_loc format:
  31 - 24 | exit status
  25 - 16 | (not used)
  15 - 08 | exit value
  07 - 00 | signal value
*/

#define _WNP_SIGNALED           0x01000000
#define _WNP_CORED              0x02000000
#define _WNP_STOPPED            0x04000000
#define _WNP_CONTINUED          0x08000000
#define _WNP_MASK               0x0F000000
#define _WNP_EXITCODE           0x10000000

#define WEXITSTATUS(_s)         (((int)(_s) >> 8) & 0xFF)
#define WIFEXITED(_s)           (((int)(_s) & _WNP_MASK) == 0x00)
#define WIFSIGNALED(_s)         (((int)(_s) & _WNP_SIGNALED) == _WNP_SIGNALED)
#define WIFSTOPPED(_s)          (((int)(_s) & _WNP_STOPPED) == _WNP_STOPPED)
#define WSTOPSIG(_s)            ((int)(_s) & 0xFF)
#define WTERMSIG(_s)            ((int)(_s) & 0xFF)

#define WIFCONTINUED(_s)        (((int)(_s) & _WNP_CONTINUED) == _WNP_CONTINUED)
#define WCOREDUMP(_s)           (((int)(_s) & _WNP_CORED) == _WNP_CORED)
#define WCORESIG(_s)            (WTERMSIG(_s))
#define WIFCORED(_s)            (WCOREDUMP(_s))

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/in_addr_t.h */
typedef unsigned long	in_addr_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/socklen_t.h */
typedef int 		socklen_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/ino_t.h */
/* ino_t は VC にも存在するため、型名を変更した。 */
typedef unsigned long	ino2_t;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/name_max.h */
#define NAME_MAX	255		/* max file name */
#define dirent direct

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/dirent.h */
/* ino_t は VC にも存在するため、型名を変更した。 */
struct dirent
{
	ino2_t	d_ino;
	u_short	d_reclen;
	u_short	d_namlen;
	char	d_name[NAME_MAX + 1];
	mode_t	d_attr;
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/dir.h */
/* NutC の構造体に一部追加修正した。 */
#define __UNUSED3	sizeof(struct dirent)

typedef struct
{
	int		dd_fd;
	unsigned long	_unused1[2];
	void		*dd_buf;
	unsigned long	_unused2[2];
	unsigned char	_unused3[__UNUSED3];
	char		dd_path[PATH_MAX+1];

#if 1   /* Add for internal use */
	long dirhandle;
	struct dirent currentdir;
	void *tmp_buf;
#endif
} DIR;

/* --------------------------------------------------------------------------*/
/* original types */
typedef struct WNP_CmdProc *WNP_CmdProc_ptr;

/* --------------------------------------------------------------------------*/
/* copy from /Program Files\NuTCROOT\include\limits.h */
#define CHILD_MAX		32

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NutCROOT/include/sys/uio.h */
struct iovec
{
	void	*iov_base;
	int		iov_len;
};

/* --------------------------------------------------------------------------*/
/* copy from /Program Files/NuTCROOT/include/nutc/msghdr.h */
struct msghdr
{
	void			*msg_name;	/* optional address */
	socklen_t		msg_namelen;	/* size of address */
	struct	iovec	*msg_iov;	/* scatter/gather array */
	int				msg_iovlen;	/* # elements in msg_iov */
	void			*msg_control;	/* ancillary data, see below */
	socklen_t		msg_controllen;	/* ancillary data buffer len */
	int				msg_flags;	/* flags on received message */
};

/* --------------------------------------------------------------------------*/
/* copy from PROGRAM FILES\NuTCROOT\include\nutc\sysconf.h => */
#define _SC_CLK_TCK						2


#define sigjmp_buf  jmp_buf

#endif /* __WIN_NATIVE_PORTING__ */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _WNP_TYPES_H */

