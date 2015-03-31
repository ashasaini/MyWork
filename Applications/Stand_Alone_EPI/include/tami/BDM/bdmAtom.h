/****************************************************************************** *
 *
 *  Last Modified:      $Date: 2010/01/06 06:45:42 $
 *  Revision Log:       $Log: bdmAtom.h,v $
 *  Revision Log:       Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:       no message
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:05  ohba
 *  Revision Log:       from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:       Revision 1.2  2002/08/05 04:19:31  purandar
 *  Revision Log:       Merging new code with ported code
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1  2000/06/08 11:01:51  full
 *  Revision Log:       Change Include Path
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1  2000/04/21 08:59:18  ohba
 *  Revision Log:       From Persia
 *  Revision Log:
 * Revision 1.1  93/05/18  13:39:26  srinivas
 * Initial version
 * 
 *
 *
 *  File Overview:
 *      This file defines items needed for atoms and properties used by bdm and
 *      display.
 *
 ******************************************************************************
 * Revision History:
 * yy-mm-dd     modified-by     modifications-done
 * 93-04-29     ST              Created

 * End of History */


#ifndef BDMATOM_H__
#define BDMATOM_H__

#define BDM_To_Display_Property      "_BDM_To_Display_Property"
#define BDM_To_Display_Data_Type     "_BDM_To_Display_Data_Type"
#define Display_BDM_Property 	     "_TAMI_DisplayWindowWithFocus"  
#define BDM_Exists_Property  	     "_TAMI_BDM"
#define BDM_Exists_Data_Type         "_TAMI_BDM_Exists_Data_Type"
#define NO_WINDOW 	     	         0

typedef struct {
   XEventClass			     class;
   int				     MotionEvent;
   } BDM_To_Display_Data; 



#endif   /* end of BDMATOM_H */
