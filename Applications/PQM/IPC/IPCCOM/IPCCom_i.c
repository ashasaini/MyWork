/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jan 30 13:06:48 2014
 */
/* Compiler settings for C:\MRMPlus\sources\Acquire\PQM_AN\IPC\IPCCOM\IPCCom.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IPqmIpcCom = {0xD29723F3,0xC8CC,0x453C,{0xAA,0x93,0x80,0x30,0x02,0x1B,0x73,0xD0}};


const IID LIBID_IPCCOMLib = {0xFF01EF82,0x9016,0x4B64,{0xAF,0x2A,0xBE,0xF8,0x00,0x94,0xAD,0x62}};


const IID IID__IPqmIpcComEvents = {0xBF9A6083,0x0EE0,0x4A18,{0x83,0xF0,0xF6,0xAE,0xA7,0xF9,0xD1,0x31}};


const CLSID CLSID_PqmIpcCom = {0x577F5E9A,0xB956,0x4448,{0x9F,0xE4,0x21,0x33,0x18,0x81,0x4D,0xAF}};


#ifdef __cplusplus
}
#endif

