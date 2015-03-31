#ifndef	NGP_ERRORS_H__
#define	NGP_ERRORS_H__

/*
 * status_t is the standard return type for many of the NGP functions.
 * It allows us to have a richer return diagnostic than the ubiquitous
 * UNIX "-1".  See tami/errors.h for macros for dealing with this:
 */

typedef	int		status_t;

#define	if_no_error(status)	if ((status) == E_NO_ERROR)
#define	if_error(status)	if ((status) != E_NO_ERROR)

#define	E_NO_ERROR		0
#define	E_ERROR			1
/* The rest of the integer range is free for the libraries to define,
 * each in its own namespace.
 */

#endif /* !NGP_ERRORS_H__ */



