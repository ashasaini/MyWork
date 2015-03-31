#ifndef malloc_debug_h__
#define malloc_debug_h__

#ifdef MALLOC_DEBUG

#define MALLOC(n)	Malloc(n)
#define CALLOC(n,s)	Calloc(n,s)
#define REALLOC(p,n)	Realloc(p,n)
#define FREE(p)		Free(p)

#else

#define MALLOC(n)	malloc(n)
#define CALLOC(n,s)	calloc(n,s)
#define REALLOC(p,n)	realloc(p,n)
#define FREE(p)		free(p)

#endif

#endif
