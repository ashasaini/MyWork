/******************************************************************************
 *   File Name:		libIrs/Irs.h
 *
 *   File Overview:
 *	This file contains the Information Registry Service (IRS) header file.
 *
 *****************************************************************************/
/* Revision Histroy:
 * yy-mm-dd	modified-by	modification-done
 * 92-11-30	hbw		Created.
 * End of History */


#ifndef Irs_h__
#define Irs_h__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *
 *	Include files
 *
 *****************************************************************************/

#ifdef __FREE_BSD__
#include <sys/bsd_types.h>
#endif
#include <sys/types.h>

#include <tami/libdll/dll.h>		/* doubly linked list header */


/******************************************************************************
 *
 *	Define constants
 *
 *****************************************************************************/

/* Socket connection definitions */
#define IRS_UNIX_PATH	"/usr/tmp/IrsSocket"

#ifdef __NUTC__
#define IRS_PORT 18000
#endif

#define IRS_MAX_SOCKET_MSG	512	/* 512 bytes for maximum socket message
					   size */


/* Errors */
#define	IRS_ERR_NOERROR	0
#define	IRS_ERR_ERROR	1
#define	IRS_ERR_NOMEM	2
#define	IRS_ERR_MAXLEN	3

/* Doubly linked list types */
#define IRS_DLL_BASE		10000
#define IRS_DLL_TYP_FIELD	(IRS_DLL_BASE+1)


/* Void field value and length */
#define IRS_VOID_VALUE		NULL


/* Size definitions */
#define IRS_SZ_VOID		0
#define IRS_SZ_CHAR		(sizeof(char))
#define IRS_SZ_INT		(sizeof(int))
#define IRS_SZ_FLOAT		(sizeof(float))
#define IRS_SZ_LONG		(sizeof(long))
#define IRS_SZ_SHORT		(sizeof(short))


/* Wildcard flag types */
#define IRS_WC_NOWILDCARD	0
#define IRS_WC_WILDCARD		1


/* Event data types */
#define IRS_ED_VA_END		0

#define IRS_ED_SOCKID		1
#define IRS_ED_MSGTYP		2
#define IRS_ED_NOTIFTYP		3
#define IRS_ED_CLIENTTAG	4
#define IRS_ED_RESTRICTFLG	5
#define IRS_ED_NUMUSERDATA	6
#define IRS_ED_USERDATA		7
#define IRS_ED_NUMEVENTFIELDS	8
#define IRS_ED_EVENTFIELDLIST	9

#define IRS_EDF_TYPE		10
#define IRS_EDF_LENGTH		11
#define IRS_EDF_WILDCARDFLAG	12


/* Message types */
#define IRS_MSGTYP_REGISTER	0
#define IRS_MSGTYP_DELETE	1
#define IRS_MSGTYP_NOTIFY	2


/* Notification type default */
#define IRS_NOTTYP_NONE		0


/* Restriction flags */
#define IRS_RESFLG_NONE		00
#define IRS_RESFLG_TYPE_MATCH	01
#define IRS_RESFLG_SUBSET_MATCH	02
#define IRS_RESFLG_ID_MATCH	04


/******************************************************************************
 *
 *	Define structures
 *
 ******************************************************************************/

/* Tool status information */

/* NOTE:	When changing the members of this data structure also change
   _			IrsSendEvent()
   _			IrsGetSize()
   -			irsCmpEventField()
   */
typedef struct IRS_EventFieldS {
    
    ulong	type;		/* type of IRS event field */
    long	length;		/* length value stream of bytes */
    ulong	wildCardFlag;	/* wild card for matching */
    char	*value;		/* value field */
    
} IRS_EventFieldT;


/* NOTE:	When changing the members of this data structure also change
   _			IrsCreateEvent()
   _			IrsGetSize()
   -			irsCmpEvent()
   */
typedef struct IRS_EventS {
    
    int		socketID;		/* IRS socket ID number */
    unchar	messageType;		/* IRS socket message type */
    unchar	notificationType;	/* user defined notification type */
    ulong	clientTag;		/* user defined client tag */
    ulong	restrictionFlags;	/* restriction flags for
					   matching */
    
    ulong	numberUserData;		/* number of user data bytes */
    void	*userData;		/* user defined data */
    
    ulong	numberEventFields;	/* number event fields */
    DllT	*eventFieldList;	/* Doubly linked list
					   Item: IRS_EventFieldT */
    
} IRS_EventT;

typedef struct IRS_MessageHeaderS {

    ulong	mark;		/* messgae boundary mark */
    int		pid;		/* sended process id */

} IRS_MessageHeaderT;


typedef struct IRS_MessageBodyS {

    ulong	type;		/* type of IRS event field */
    long	length;		/* length value stream of bytes */
    ulong	wildCardFlag;	/* wild card for matching */
    char	value[128];	/* value field */

} IRS_MessageBodyT;

typedef struct IRS_MessageS {

    ulong	mark;		/* message boundary mark */
    int		pid;		/* sended process id */
    int		socketID;
    unchar	messageType;
    unchar	notificationType;
    ulong       clientTag;
    ulong       restrictionFlags;
    ulong       numberUserData;
    char	userData[58];
    ulong	numberEventFields;
    IRS_MessageBodyT	eventFieldList[3];

} IRS_MessageT;

/******************************************************************************
 *
 *	Function Prototypes
 *
 *****************************************************************************/

status_t IrsAddEventField
    (ulong	type,			/* event field type */
     long	length,			/* event field length */
     void	*value,			/* event field stream of bytes */
     ulong	wildCardFlag,		/* wildcard matching flag */
     DllT	**eventFieldListP);	/* pointer to doubly linke list to add
					   to */

IRS_EventT *IrsCreateEvent
    (int	socketID,	/* IRS server socket ID */
     ...);			/* variable argument list */

void IrsFreeEvent
    (void	*irsEvent);

void *IrsGetEventField
    (IRS_EventT	*eventP,	/* event to retrieve event field from */
     ulong	type);		/* type of event field to retrieve */

int IrsInit
    (void);

IRS_EventT *IrsNextEvent
    (int	socketID);	/* socket ID to IRS server */

IRS_EventT *IrsPollEvent
    (int	socketID);	/* socket ID to IRS server */

status_t IrsSendEvent
    (register int		socketID,	/* UNIX socket ID to send to */
     register IRS_EventT	*event);	/* IRS event structure to
						   send */
#ifdef __cplusplus
} // extern "C"
#endif

#endif /* Irs_h__ */
