/******************************************************************************
 *  
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *      
 *
 *****************************************************************************/
 
 
#ifndef msFtp_H__
#define msFtp_H__
 
int msFtpSend(int Sock, char *Buf, int Size, int Dum);

int msFtpRecv(int Sock, char *Buf, int Size, int Dum);

int msFtpRecv2(int Sock, char *Buf, int Size, int Dum);

int msFtpSendCommand(int CmdSock, char *fmt, ...);

int msFtpAnswer(int CmdSock, char *Str);

int msFtpLogin(int CmdSock);

int msFtpConnect(char *HostName, char *PortName, int PortNumber);

void msFtpClose(int CmdSock);

void msFtpPassiveSplit(char *buf, char *Host, int *PortNum);

int msFtpPassive(int CmdSock);

int msFtpSTOR(int CmdSock, char *Fname);

int msFtpRETR(int CmdSock, char *Fname);

int msFtpLIST(int CmdSock, char *Fname);

int msFtpPut(char *HostName, char *Fname);

int msFtpGet(char *HostName, char *Fname);

int msFtpMPut(char *HostName, char *Fname);

void msFtpFileDelete(char *Fname);

/* studyCopy.script -> CŒ¾Œê‰»‚Ìˆ×’Ç‰Á sr.hong  2008-03-14 */
int msFtpCWD(int CmdSock, char *Path);
 
int msFtpLCD(int CmdSock, char *Path);
 
int msFtpMGet(char *HostName, char *Fname);

int msFtpNLST(int CmdSock, char *Fname, char *listBuf);

/* studyCopy.script -> CŒ¾Œê‰»‚Ìˆ×’Ç‰Á sr.hong  2008-04-10 */
int msFtpMDTM(int CmdSock, char *Fname);

int msFtpSIZE(int CmdSock, char *Fname);

#endif  /* msFtp_H__ */

