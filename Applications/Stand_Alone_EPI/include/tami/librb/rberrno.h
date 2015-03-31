/*******************************************************************
**
**				rberrno.h
**
*******************************************************************/

extern	int		errno;
extern	int		rberrno;

/* Library Error */
#define	ERB_ENXIO		0x10001	/* No such device or address 		*/
#define	ERB_EINVAL		0x10002	/* Invalid argument 			*/
#define	ERB_ENOSPC		0x10003	/* No space left on device 		*/
#define	ERB_ENODEV		0x10004	/* No such device 			*/
#define	ERB_EBUSY		0x10005	/* Device or resource busy 		*/
#define ERB_ENOENT		0x10006	/* No such file or directory 		*/

#define	ERB_BAD_FORMAT		0x20001	/* Bad format RB config file		*/
#define	ERB_EXIST_RB_BOARD	0x20002	/* Exist RB Board 			*/
#define	ERB_EXIST_VMEBUF_BOARD	0x20003	/* Exist VME Buffer Memory Board	*/
#define	ERB_FAIL_WAIT_INTR	0x20004	/* Fail wait interrupt		 	*/
#define	ERB_FAIL_GV_OPEN 	0x20005	/* Fail GVopen() 			*/
#define	ERB_FAIL_GV_READ 	0x20006	/* Fail GVread() 			*/

/* RB hardware error */
#define	ERB_RBSTAT_MASK		0x30000	/* RB Status mask 			*/
#define	ERB_RBSTAT_NODONE	0x30001	/* stray intr w/o DONE set -- not reenabling intrs  */
#define	ERB_RBSTAT_NOERRBIT	0x30002	/* err w/o ERROR bit??? (continuing...) */
#define	ERB_RBSTAT_BERR		0x30004	/* VME bus err 				*/
#define	ERB_RBSTAT_NFE		0x30008	/* NFE err 				*/
#define	ERB_RBSTAT_CERR		0x30010	/* real CERR cond? 			*/
#define	ERB_RBSTAT_BARERR	0x30020	/* BAR err 				*/
#define	ERB_RBSTAT_ERRPROB	0x30040	/* unknown err cond 			*/
#define	ERB_RBSTAT_STILLARM	0x30080	/* err chan still armed? 		*/
#define	ERB_RBSTAT_STRAY	0x30100	/* DONE on idle chan? 			*/
#define	ERB_RBSTAT_WCNZERO	0x30200	/* wcnt > 0 on DONE chan? 		*/
#define	ERB_RBSTAT_FEPRACE	0x30400	/* FEP race on DONE chan 		*/
#define	ERB_RBSTAT_EIO		0x30800	/* I/O error		 		*/
#define ERB_RBSTAT_CANCELIO	0x31000	/* Cancel IO 	 			*/
