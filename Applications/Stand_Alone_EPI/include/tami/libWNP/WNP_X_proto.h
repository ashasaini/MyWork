#ifndef _X_MY_PROTO_H
#define _X_MY_PROTO_H

/* 
　　Windows Native Porting に必要なコードのコンパイルに必要な定義をMKS-includeから抜粋
　　実際は未使用。
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN_NATIVE_PORTING__

/* copy from NuTCROOT/include/X11(Xm,Mrm)/*.h */


#define XtOffset(p_type,field) ((unsigned int)&(((p_type)NULL)->field))

typedef unsigned long		Atom;
typedef unsigned long		XID;
typedef XID			Window;
typedef XID			Pixmap;
typedef char			Boolean;
typedef char			*caddr_t;
typedef char			*XPointer;
typedef void			*XtPointer;
typedef unsigned long		XtInputId;
typedef unsigned char		*XmString;
typedef struct _XtAppStruct	*XtAppContext;
typedef struct _WidgetRec	*Widget;
typedef struct _XDisplay	Display;
typedef unsigned short	Dimension;  /* Size in pixels			*/
typedef struct _XmFontListRec       *XmFontList;       /* opaque to outside */

#define NeedFunctionPrototypes 1

typedef void (*XtInputCallbackProc)(
#if NeedFunctionPrototypes
    XtPointer 		/* closure */,
    int*		/* source */,
    XtInputId*		/* id */
#endif
);

typedef struct _XImage {
    int width, height;		/* size of image */
    int xoffset;		/* number of pixels offset in X direction */
    int format;			/* XYBitmap, XYPixmap, ZPixmap */
    char *data;			/* pointer to image data */
    int byte_order;		/* data byte order, LSBFirst, MSBFirst */
    int bitmap_unit;		/* quant. of scanline 8, 16, 32 */
    int bitmap_bit_order;	/* LSBFirst, MSBFirst */
    int bitmap_pad;		/* 8, 16, 32 either XY or ZPixmap */
    int depth;			/* depth of image */
    int bytes_per_line;		/* accelarator to next line */
    int bits_per_pixel;		/* bits per pixel (ZPixmap) */
    unsigned long red_mask;	/* bits in z arrangment */
    unsigned long green_mask;
    unsigned long blue_mask;
    XPointer obdata;		/* hook for the object routines to hang on */
    struct funcs {		/* image manipulation routines */
	struct _XImage *(*create_image)();
#if NeedFunctionPrototypes
	int (*destroy_image)        (struct _XImage *);
	unsigned long (*get_pixel)  (struct _XImage *, int, int);
	int (*put_pixel)            (struct _XImage *, int, int, unsigned long);
	struct _XImage *(*sub_image)(struct _XImage *, int, int, unsigned int, unsigned int);
	int (*add_pixel)            (struct _XImage *, long);
#else
	int (*destroy_image)();
	unsigned long (*get_pixel)();
	int (*put_pixel)();
	struct _XImage *(*sub_image)();
	int (*add_pixel)();
#endif
	} f;
} XImage;


typedef union _XEvent {
        int type;		/* must not be changed; first element */
	long pad[24];
} XEvent;

typedef struct
{
    int     reason;
    XEvent  *event;
} XmAnyCallbackStruct;

#define	uint	unsigned int

#define True 1

#endif /* __WIN_NATIVE_PORTING__ */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _X_MY_PROTO_H */

