/*********************************************************************
 *
 *  (c) Copyright 1996 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libvfToUi/vfToUiproto.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2013/03/06 06:07:58 $
 *  Revision Log:    $Log: vfToUiproto.h,v $
 *  Revision Log:    Revision 1.2  2013/03/06 06:07:58  tatsuya
 *  Revision Log:    MCM0310-00083 Redmine #5582 [SM] Fixed to not delete vfc parameters when scratchpad is created.
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.6.12.1.2.3.64.1  2007/06/01 07:05:37  hanaki
 *  Revision Log:    MCM0200-00094 add vuiCoil
 *  Revision Log:
 *  Revision Log:    Revision 1.6.12.1.2.3  2003/03/31 07:25:41  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2003/03/22 11:29:24  vseries
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *  Revision Log:    Revision 1.6  2002/03/18 03:32:11  yutaka
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *  Revision Log:    Revision 1.5  2002/03/16 08:12:53  yutaka
 *  Revision Log:    added VuiCopyNode()
 *  Revision Log:
 *  Revision Log:    Revision 1.4  2002/02/13 08:45:33  yagisawa
 *  Revision Log:    added VuiCopyProtocol2()
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2000/08/24 07:14:15  full
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2000/07/25 11:32:53  full
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/06/08 11:01:58  full
 *  Revision Log:    Change Include Path
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/04/21 08:59:19  ohba
 *  Revision Log:    From Persia
 *  Revision Log:
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Feb.22.96	Naoyuki FURUDATE	Created
 * End of History */



#ifndef vfToUiproto_H__
#define vfToUiproto_H__

#include <tami/libvfToUi/vfToUiDef.h>

/*******************************************
 * 	File : vuiInit.c
 *******************************************/
int		VuiProcessInit		(char *,char *,VuiInfo_t **,int *);
void		VuiAddConvert		(char *,addConvertfunc_t);

/*******************************************
 * 	File : vuiFile.c
 *******************************************/
VfDBHandle_t	VuiOpen			(char *,int);
status_t	VuiOpen_blank		(char *);
status_t	VuiClose		(VfDBHandle_t);

/*******************************************
 * 	File : vuiBlock.c
 *******************************************/
status_t	VuiGetBlockListLabel	(int,char **,int *);
status_t	VuiGetBlockListData	(VfDBHandle_t,int,int,char **,
					 int *);
status_t	VuiMakeBlockListData	(VfDBHandle_t,int,int,char **,
					 int *);
status_t	VuiReloadBlockListData	(VfDBHandle_t,int,int);
int		VuiGetRelationalBlock	(VfName_t);
status_t	VuiPackBlockListData	(VfDBHandle_t,int,int,char *,int *);

/*******************************************
 * 	File : vuiTree.c
 *******************************************/
status_t	VuiCopyProt		(VfDBHandle_t,bitfield_t *,int *,
					 char *,int,bool_t,bool_t,
					 void (*)(int));
status_t	VuiCopyProtNew		(VfDBHandle_t,bitfield_t *,int *,
					 char *,int,bool_t,void (*)(int));
status_t	VuiCopyProtSubtree	(VfDBHandle_t,int,int);
status_t	VuiCopyProtSubtreeNew	(VfDBHandle_t,int,int);


/*******************************************
 * 	File : vuiCopy.c
 *******************************************/
status_t	VuiCopyProtocol		(VfDBHandle_t,int,int,int,int *);

status_t	VuiCopyProtocol2	(VfDBHandle_t,int,VfDBHandle_t,int,int,int *);

status_t	VuiCopyProtocolToFile	(VfDBHandle_t,int,char *,int,
					 int,int *);
status_t	VuiCopyProtocolFromFile	(VfDBHandle_t,int,char *,int,
					 int,int *);
status_t	VuiCopyProtocolsToFile	(VfDBHandle_t,int *,int,
					 char *,int,
					 status_t (*)(VfDBHandle_t,int,void *),
					 void *);
status_t	VuiCopyProtocolsFromFile(VfDBHandle_t,char *,
					 int *,int,int,
					 status_t (*)(VfDBHandle_t,int,void *),
					 void *);
status_t	VuiCopyNode		(VfDBHandle_t, VfPathElem_t *, int,
					 VfDBHandle_t, VfPathElem_t *, int,
                                         int ) ;

status_t VuiCopyProtocolToPrivateProlimScratchPad(VfDBHandle_t,int,char *,int,int,int *);
status_t VuiCopyProtocolFromPrivateProlimScratchPad( VfDBHandle_t,int,char*,int,int,int* );


/*******************************************
 * 	File : vuiVf.c
 *******************************************/
status_t	VuiGetAllProt		(VfDBHandle_t,bitfield_t *,int *);
status_t	VuiUpdateAcqOrder	(VfDBHandle_t,VuiAcqOrder_t);
status_t	VuiAddToAcqOrder	(VfDBHandle_t,int);
status_t	VuiSetAcqOrder		(VfDBHandle_t,int,int);
status_t	VuiDeleteAcqNum		(VfDBHandle_t,int);
status_t	VuiDeleteProtcol	(VfDBHandle_t,int);
status_t	VuiDeleteProtcols	(VfDBHandle_t,bitfield_t *,int,
					 void (*)(int));
status_t	VuiGetProtocolImgGr	(VfDBHandle_t,int,int *,int);
status_t	VuiDelProtocolImgGr	(VfDBHandle_t,int,int *,int);
status_t	VuiDelProtocolImgGrField(VfDBHandle_t,int);
status_t	VuiGetAllImgGr		(VfDBHandle_t,int **,int *);
int		VuiProtNum		(VfDBHandle_t,int);
int		VuiAcqNum		(VfDBHandle_t,int);
status_t	VuiGetAcqOrder		(VfDBHandle_t,VuiAcqOrder_t *);
status_t	VuiSetNewAcqOrder	(VfDBHandle_t,int,int);
status_t	VuiGetVfAcqOrder	(VfDBHandle_t,VuiAcqOrder_t *);

/*******************************************
 * 	File : vuiVec.c
 *******************************************/
status_t	VuiGetOffsetVector	(VfDBHandle_t,int,f32vec3_t *);
status_t	VuiGetSliceVector	(VfDBHandle_t,int,f32vec3_t *);
status_t	VuiGetPhaseVector	(VfDBHandle_t,int,f32vec3_t *);
status_t	VuiShiftOffsetVector	(VfDBHandle_t,int,int,int,float);
status_t	VuiSetOffsetVector	(VfDBHandle_t,int,f32vec3_t *);
status_t	VuiSetSliceVector	(VfDBHandle_t,int,f32vec3_t *);
status_t	VuiSetPhaseVector	(VfDBHandle_t,int,f32vec3_t *);

/*******************************************
 * 	File : vuiCoil.c
 *******************************************/
status_t VuiSaturnCoilName		(int, char *, VfDBHandle_t);
status_t VuiVantageCoilName		(void *, int , char *);

#endif /* vfToUiproto_H__ */
