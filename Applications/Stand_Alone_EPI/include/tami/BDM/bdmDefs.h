/*******************************************************************************
 *  Last Modified:      $Date: 2012/04/20 06:43:36 $
 *  Revision Log:       $Log: bdmDefs.h,v $
 *  Revision Log:       Revision 1.2  2012/04/20 06:43:36  ohmure
 *  Revision Log:       MCM0300-00373 [AM]Add Couch BTN LED OFF func.  MVC011993
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:       no message
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1.28.1.2.4  2003/05/23 05:46:10  pc_koku
 *  Revision Log:       Bug fixed
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1.28.1.2.4  2003/04/21 10:41:38  pc_koku
 *  Revision Log:       USIF Implementation for PC
 *  Revision Log:       BDM Porting for PC
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1.28.1.2.3  2003/04/01 22:24:46  ohba
 *  Revision Log:       from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1  2002/04/16 07:19:52  cvsadmin
 *  Revision Log:       Imported
 *  Revision Log:
 *  Revision Log:       Revision 1.1.1.1  2000/04/21 07:43:25  seki
 *  Revision Log:       from persia:V4.04
 *  Revision Log:
 * Revision 1.1  93/05/18  13:39:35  srinivas
 * Initial version
 * 
 
 *    File Overview:
 *      This file defines constants for buttons on the Gantry and the Console.
 *      acqman and bdm should use these constants while communicating
 *      with each other.
 *
 *********************************************************************************/
/* Revision History:
 * yy-mm-dd     modified-by     modifications-done
 * 93-04-16     ST              Created 
 * End of History */


#ifndef BDMDEFS_H__
#define BDMDEFS_H__

typedef enum {
  LED_OFF          = 0,
  LED_ON           = 1,
  LED_BLINK        = 2,
  LED_BITS         = 0xC0000000,       /* 110.....0 (32 bits) */
  LED_BIT_POSITION = 30,
  BUTTON_BITS	   = 0x3FFFFFFF       /* 00111....11 (32 bits) */
  } led_bits;
    
typedef enum {
  START_ON_CONSOLE_BUTTON = 2,
  PAUSE_ON_CONSOLE_BUTTON = 3,
  ABORT_ON_CONSOLE_BUTTON = 4,
  START_ON_GANTRY_BUTTON  = 18,
  PAUSE_ON_GANTRY_BUTTON  = 19,
  ABORT_ON_GANTRY_BUTTON  = 20
  } button_numbers;
    
typedef enum {
  START_ON_CONSOLE_MASK   = 0x2,
  PAUSE_ON_CONSOLE_MASK   = 0x4,
  ABORT_ON_CONSOLE_MASK   = 0x8,
  COUCH_ON_CONSOLE_MASK   = 0x10,
  START_ON_GANTRY_MASK    = 0x20000,
  PAUSE_ON_GANTRY_MASK    = 0x40000,
  ABORT_ON_GANTRY_MASK    = 0x80000,
  READY_ON_GANTRY_MASK    = 0x100000
  } button_masks;
#ifdef __NUTC__
typedef enum {
  START_USIF_CONSOLE_BUTTON   = 0x31,
  PAUSE_USIF_CONSOLE_BUTTON   = 0x32,
  ABORT_USIF_CONSOLE_BUTTON   = 0x34,
  COUCH_USIF_CONSOLE_BUTTON   = 0x38
  } USIF_button_numbers;

typedef enum {
  START_USIF_CONSOLE_BUTTON_MASK   = 0x31,
  PAUSE_USIF_CONSOLE_BUTTON_MASK   = 0x32,
  ABORT_USIF_CONSOLE_BUTTON_MASK   = 0x34,
  COUCH_USIF_CONSOLE_BUTTON_MASK   = 0x38
  } USIF_button_mask;

typedef enum {
  START_USIF_LED_CONSOLE_MASK   = 0x1,
  PAUSE_USIF_LED_CONSOLE_MASK   = 0x2,
  ABORT_USIF_LED_CONSOLE_MASK  	= 0x4,
  COUCH_USIF_LED_CONSOLE_MASK  	= 0x8
  } USIF_LED_button_mask;
  
#endif	/* __NUTC__ */

#endif  /* BDMDEFS_H__ */
