// AutoInsertConstant.h: interface for the CAutoInsertConstant class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERTCONSTANT_H__
#define __SM_IPCLIB_AUTOINSERTCONSTANT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum {
    MAP_MODE = 1,		//Auto Map
    SHIM_MODE		//Auto Shim
} scan_mode;


typedef enum {
    SHIM_NORMAL_MODE = 1,		//Without AutoShim
    AUTOSHIM_MODE				//AutoShim Mode
} mode_autoshim;


typedef enum {
    MAP_NORMAL_MODE = 1,		/*Without AutoMap*/
    MAP_AUTO_MODE				/*AuotMap Mode*/
} mode_automap;

enum AutoInsertEnum { AutoInsertNone, AutoInsertBefore, AutoInsertAfter };

typedef enum {
    AUTO_INSERT_NOT_REQUIRED,
    AUTO_INSERT_SUCCESS,
    AUTO_INSERT_FAIL,
    AUTO_INSERT_ERROR
} AutoInsert_status;


struct AutoInsertData {

    float  m_couch_position;
    int m_couchstatusflag;
    AutoInsertData(const float f_couch_position, int f_couchstatusflag) {
        m_couch_position = f_couch_position ;
        m_couchstatusflag = f_couchstatusflag ;
    }
};


#endif // #ifndef __SM_IPCLIB_AUTOINSERTCONSTANT_H__
