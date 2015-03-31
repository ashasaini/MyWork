/**********************************************************
**
**
**		librb.h
**
**********************************************************/

/*
**	FUNCTION PROTO TYPE
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef PROTO_DEF
extern	int		RBopen( char *, int );
extern	int		RBclose( int );
extern	int		RBioctl(int, int, int);
extern	int		RBread(int, void *);
#else
extern	int		RBopen();
extern	int		RBclose();
extern	int		RBioctl();
extern	int		RBread();
#endif

#ifdef __cplusplus
} // extern "C"
#endif
