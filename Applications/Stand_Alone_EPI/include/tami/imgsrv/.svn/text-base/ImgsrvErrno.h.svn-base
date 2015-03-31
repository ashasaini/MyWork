/*****************************************************************************
 *	ImgsrvErrno.h
 *	2002/01/18 y.morita	V0.01 P1.12 ImgsrvApl.
 *	2002/01/17 y.morita	V0.01 P1.11 timeout(read/write).
 *	2002/01/15 y.morita	V0.01 P1.10 timeout(connect).
 *	2002/01/11 y.morita	V0.01 P1.10 timeout.
 *	2001/12/20 y.morita	V0.01 P1.08 imgsrv_req.
 *	2001/12/10 y.morita	V0.01 P1.06 log(mask/output).
 *	2001/12/10 y.morita	V0.01 P1.05 jobs.
 *	2001/12/09 y.morita	V0.01 P1.04 imgsrv_log.
 *	2001/10/12 y.morita	V0.01 P1.02 DEBUG_IMGSRV
 *	2001/07/18 y.morita	V0.01 P1.01 ImgsrvErrno(Imgsrv.h)
 *	2001/07/12 y.morita	V0.01 P1.00 Initial Version.
 *****************************************************************************/
#ifndef __ImgsrvErrno_H__
#define __ImgsrvErrno_H__


#include <tami/imgsrv/imgsrv_log.h>


enum {
  /* 正常終了 */
  IMGSRV_E_NO_ERROR		=      0,


  /* 一般的なエラー */
  IMGSRV_E_ERROR		=      1, /* エラー発生                      */
  IMGSRV_E_ILLARGS		=      2, /* 引数不正                        */


  /* システムコール,標準関数でのエラー */
  IMGSRV_E_GETHOSTBYNAME	=     11, /* 接続先IPアドレス取得に失敗      */
  IMGSRV_E_GETSERVBYNAME	=     12, /* 接続先ポート番号取得に失敗      */
  IMGSRV_E_SOCKET 		=     13, /* socket()でエラー発生            */
  IMGSRV_E_SETSOCKOPT 		=     14, /* setsockopt()でエラー発生        */
  IMGSRV_E_BIND	 		=     15, /* bind()でエラー発生              */
  IMGSRV_E_LISTEN	 	=     16, /* listen()でエラー発生            */
  IMGSRV_E_CONNECT 		=     17, /* connect()でエラー発生           */
  IMGSRV_E_OPEN			=     18, /* open()でエラー発生              */
  IMGSRV_E_CLOSE		=     19, /* open()でエラー発生              */
  IMGSRV_E_FCNTL 		=     20, /* fcntl()でエラー発生             */
  IMGSRV_E_WRITEWLOCK		=     21, /* fcntl()でエラー発生             */
  IMGSRV_E_READWLOCK		=     22, /* fcntl()でエラー発生             */
  IMGSRV_E_UNLOCK		=     23, /* fcntl()でエラー発生             */
  IMGSRV_E_UNLINK 		=     24, /* unlink()でエラー発生            */
  IMGSRV_E_SHMGET		=     25, /* shmget()でエラー発生            */
  IMGSRV_E_SHMCTL		=     26, /* shmctl()でエラー発生            */
  IMGSRV_E_SHMAT		=     27, /* shmat()でエラー発生             */
  IMGSRV_E_SEMGET		=     28, /* semget()でエラー発生            */
  IMGSRV_E_SEMCTL		=     29, /* semctl()でエラー発生            */
  IMGSRV_E_SEMOP		=     30, /* semop()でエラー発生             */
  IMGSRV_E_SEMVMX		=     31, /* SEMVMXを超えた値                */
  IMGSRV_E_MALLOC		=     32, /* malloc()でエラー発生            */
  IMGSRV_E_WRITE		=     33, /* write()でエラー発生             */
  IMGSRV_E_READ			=     34, /* read()でエラー発生              */
  IMGSRV_E_SELECT		=     35, /* select()でエラー発生            */

  /* タイムアウト */
  IMGSRV_E_CONNECT_TIMEOUT	=     51, /* connect()でタイムアウト発生     */
  IMGSRV_E_READ_TIMEOUT		=     52, /* read()でタイムアウト発生        */
  IMGSRV_E_WRITE_TIMEOUT	=     53, /* write()でタイムアウト発生       */


  /* サーバーJOB制御関連でのエラー */
  IMGSRV_E_JOBMAXROW		=   1001, /* 画像横方向サイズオーバー        */
  IMGSRV_E_JOBMAXCOL		=   1002, /* 画像縦方向サイズオーバー        */
  IMGSRV_E_JOBMAXSLICE		=   1003, /* 画像スライス数オーバー          */
  IMGSRV_E_JOBMAXSHMSIZE	=   1004, /* 共有メモリサイズオーバー        */
  IMGSRV_E_JOBMAXSHMTOTAL	=   1005, /* 共有メモリ合計サイズオーバー    */
  IMGSRV_E_JOBMAXSHMNUM		=   1006, /* 共有メモリ総数オーバー          */
  IMGSRV_E_JOBNODEALLOC		=   1007, /* malloc()でエラー発生            */
  IMGSRV_E_JOBNEXT		=   1008, /* JOBの発番に失敗                 */
  IMGSRV_E_JOBNOTFOUND		=   1009, /* JOB番号が存在しない             */


  /* サーバー親/子間FIFO関連でのエラー */
  IMGSRV_E_FIFOMAKE		=   1101, /* mkfifo()でエラー発生            */
  IMGSRV_E_FIFOOPEN		=   1101, /* open()でエラー発生              */
  IMGSRV_E_FIFOMAKEOPEN		=   1103, /* FIFO作成に失敗                  */
  IMGSRV_E_FIFOWRITE		=   1104, /* write()でエラー発生             */
  IMGSRV_E_FIFOREAD		=   1105, /* read()でエラー発生              */
  IMGSRV_E_FIFOWRITENOTENOUGH	=   1106, /* write()で書き足りない           */
  IMGSRV_E_FIFOREADNOTENOUGH	=   1107, /* read()で読み足りない            */
  IMGSRV_E_FIFOMISMATCH		=   1108, /* 期待した要求でない              */
  IMGSRV_E_FIFOREADEOF		=   1109, /* 書き手がクローズされた          */
  IMGSRV_E_FIFOCTOP		=   1110, /* 子から親のFIFO送信でエラー発生  */
  IMGSRV_E_FIFOPTOC		=   1111, /* 親から子のFIFO送信でエラー発生  */
  IMGSRV_E_FIFOJOBNEW		=   1112, /* 子から親のFIFO送信でエラー発生  */
  IMGSRV_E_FIFOJOBDEL		=   1113, /* 子から親のFIFO送信でエラー発生  */
  IMGSRV_E_FIFOJOBINFO		=   1114, /* 子から親のFIFO送信でエラー発生  */
  IMGSRV_E_FIFOJOBNEWREPLY	=   1115, /* 親から子のFIFO受信でエラー発生  */
  IMGSRV_E_FIFOWRITEREAD	=   1116, /* 子から親のFIFO送受信エラー発生  */


  /* サーバー(子)/アプリ間socket通信関連でのエラー */
  IMGSRV_E_MSGWRITE		=   1201, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_MSGREAD		=   1202, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_MSG_H_WRITE		=   1203, /* へッダー送信でエラーが発生      */
  IMGSRV_E_MSG_H_READ		=   1204, /* ヘッダー受信でエラーが発生      */
  IMGSRV_E_MSG_H_WRITENOTENOUGH	=   1205, /* ヘッダー送信で書き足りない      */
  IMGSRV_E_MSG_H_READNOTENOUGH	=   1206, /* へッダー受信で読み足りない      */
  IMGSRV_E_MSG_P_WRITE		=   1207, /* パラメータ送信でエラーが発生    */
  IMGSRV_E_MSG_P_READ		=   1208, /* パラメータ受信でエラーが発生    */
  IMGSRV_E_MSG_P_WRITENOTENOUGH	=   1209, /* パラメータ送信で書き足りない    */
  IMGSRV_E_MSG_P_READNOTENOUGH	=   1210, /* パラメータ受信で読み足りない    */


  /* 同期(共有メモリ排他制御)関連でのエラー */
  IMGSRV_E_SYNCFLOCKCREATE	=   1301, /* ロックファイル作成失敗          */
  IMGSRV_E_SYNCFLOCKOPEN	=   1302, /* ロックファイルオープン失敗      */
  IMGSRV_E_SYNCFLOCKCLOSE	=   1303, /* ロックファイルクローズ失敗      */
  IMGSRV_E_SYNCFLOCKWAITWRITE	=   1304, /* ロックファイル書き込み待ち失敗  */
  IMGSRV_E_SYNCFLOCKWAITREAD	=   1305, /* ロックファイル読み出し待ち失敗  */
  IMGSRV_E_SYNCFLOCKPOST	=   1306, /* ロックファイル読み書き許可失敗  */
  IMGSRV_E_SYNCSEMCREATE	=   1307, /* セマフォ作成失敗                */
  IMGSRV_E_SYNCSEMWAIT		=   1308, /* セマフォ待ち失敗                */
  IMGSRV_E_SYNCSEMPOST		=   1309, /* セマフォ許可失敗                */
  IMGSRV_E_SYNCDELETESTABLE	=   1310, /* 同期機構削除失敗                */
  IMGSRV_E_SYNCDELETECYCLIC	=   1311, /* 同期機構削除失敗                */
  IMGSRV_E_SYNCILLMODE		=   1312, /* 不正な画像格納モード            */
  IMGSRV_E_SYNCILLSLICENO	=   1313, /* 不正なスライス番号              */
  IMGSRV_E_SYNCWRITENEXT	=   1314, /* カウンタ更新失敗                */
  IMGSRV_E_SYNCWRITEDONE	=   1315, /* カウンタ更新失敗                */
  IMGSRV_E_SYNCREADNEXT		=   1316, /* カウンタ更新失敗                */
  IMGSRV_E_SYNCREADDONE		=   1317, /* カウンタ更新失敗                */
  IMGSRV_E_SYNCFIFOREAD		=   1318, /* FIFO読み出し失敗                */
  IMGSRV_E_SYNCFIFOWRITE	=   1319, /* FIFO書き込み失敗                */
  IMGSRV_E_SYNCFIFOWRITEREAD	=   1320, /* FIFO読み出し失敗                */
  IMGSRV_E_SYNCCREATE		=   1321, /* 同期機構作成失敗                */
  IMGSRV_E_SYNCDELETE		=   1322, /* 同期機構削除失敗                */
  IMGSRV_E_SYNCWAITWRITE	=   1323, /* 書き出し同期待ち失敗            */
  IMGSRV_E_SYNCPOSTWRITE	=   1324, /* 書き出し同期解除失敗            */
  IMGSRV_E_SYNCWAITREAD		=   1325, /* 読み込み同期待ち失敗            */
  IMGSRV_E_SYNCPOSTREAD		=   1326, /* 読み込み同期解除失敗            */
  IMGSRV_E_SYNCOPEN		=   1327, /* 同期機構オープン失敗            */
  IMGSRV_E_SYNCCLOSE		=   1328, /* 同期解除クローズ失敗            */
  IMGSRV_E_SHMATTACH		=   1329, /* 共有メモリ接続失敗              */
  IMGSRV_E_SHMDETACH		=   1330, /* 共有メモリ切断失敗              */
  IMGSRV_E_LOCKOPEN		=   1331, /* ロックオープン失敗              */
  IMGSRV_E_LOCKCLOSE		=   1332, /* ロッククローズ失敗              */
  IMGSRV_E_LOCKGETNODE		=   1333, /* 不正なJOB指定                   */


  /* アプリケーション側でのエラー */
  IMGSRV_E_CLOSE_MSGWRITE	=   2001, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_CLOSE_MSGREAD	=   2002, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_CLOSE_MSGMISMATCH	=   2003, /* 期待したメッセージでない        */
  IMGSRV_E_CLOSE_MSGREPLY	=   2004, /* リプライステータスがエラー      */

  IMGSRV_E_START_MSGWRITE	=   2011, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_START_MSGREAD	=   2012, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_START_MSGMISMATCH	=   2013, /* 期待したメッセージでない        */

  IMGSRV_E_FINISH_MSGWRITE	=   2021, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_FINISH_MSGREAD	=   2022, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_FINISH_MSGMISMATCH	=   2023, /* 期待したメッセージでない        */

  IMGSRV_E_PUT_MSGWRITE		=   2031, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_PUT_MSGREAD		=   2032, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_PUT_MSGMISMATCH	=   2033, /* 期待したメッセージでない        */
  IMGSRV_E_PUT_DATAWRITE	=   2034, /* データ送信でエラーが発生        */
  IMGSRV_E_PUT_DATAREAD		=   2036, /* データ受信でエラーが発生        */

  IMGSRV_E_GET_MSGWRITE		=   2041, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_GET_MSGREAD		=   2042, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_GET_MSGMISMATCH	=   2043, /* 期待したメッセージでない        */
  IMGSRV_E_GET_DATAREAD		=   2044, /* データ送信でエラーが発生        */
  IMGSRV_E_GET_DATAWRITE	=   2045, /* データ送信でエラーが発生        */

  IMGSRV_E_ATTACH_MSGWRITE	=   2051, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_ATTACH_MSGREAD	=   2052, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_ATTACH_MSGMISMATCH	=   2053, /* 期待したメッセージでない        */
  IMGSRV_E_ATTACH_HOSTMISMATCH	=   2054, /* ネットワーク先にATTACH要求した  */
  IMGSRV_E_ATTACH_SHMGET	=   2055, /* shmget()でエラー発生            */
  IMGSRV_E_ATTACH_SHMAT		=   2056, /* shmat()でエラー発生             */
  IMGSRV_E_ATTACH_ILLSLICENO	=   2057, /* スライス番号不正                */

  IMGSRV_E_DETACH_MSGWRITE	=   2061, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_DETACH_MSGREAD	=   2062, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_DETACH_MSGMISMATCH	=   2063, /* 期待したメッセージでない        */
  IMGSRV_E_DETACH_HOSTMISMATCH	=   2064, /* ネットワーク先にATTACH要求した  */
  IMGSRV_E_DETACH_SHMGET	=   2065, /* shmget()でエラー発生            */
  IMGSRV_E_DETACH_SHMDT		=   2066, /* shmdt()でエラー発生             */
  IMGSRV_E_DETACH_ILLSLICENO	=   2067, /* スライス番号不正                */

  IMGSRV_E_LOCK_MSGWRITE	=   2071, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_LOCK_MSGREAD		=   2072, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_LOCK_MSGMISMATCH	=   2073, /* 期待したメッセージでない        */

  IMGSRV_E_UNLOCK_MSGWRITE	=   2081, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_UNLOCK_MSGREAD	=   2082, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_UNLOCK_MSGMISMATCH	=   2083, /* 期待したメッセージでない        */

  IMGSRV_E_INFO_MSGWRITE	=   2111, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_INFO_MSGREAD		=   2112, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_INFO_MSGMISMATCH	=   2113, /* 期待したメッセージでない        */

  IMGSRV_E_JOBS_MSGWRITE	=   2121, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_JOBS_MSGREAD		=   2122, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_JOBS_MSGMISMATCH	=   2123, /* 期待したメッセージでない        */
  IMGSRV_E_JOBS_REALLOC		=   2124, /* realloc()でエラー発生           */
  IMGSRV_E_JOBS_JOBMISMATCH	=   2125, /* job情報が異常                   */
  IMGSRV_E_JOBS_DATAREAD	=   2126, /* job情報データ受信でエラー発生   */
  IMGSRV_E_JOBS_DATAWRITE	=   2127, /* job情報データ送信でエラー発生   */

  IMGSRV_E_LOG_MSGWRITE		=   2131, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_LOG_MSGREAD		=   2132, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_LOG_DATAREAD		=   2133, /* データ受信でエラー発生          */
  IMGSRV_E_LOG_DATAWRITE	=   2134, /* データ送信でエラー発生          */
  IMGSRV_E_LOG_MSGMISMATCH	=   2135, /* 期待したメッセージでない        */

  IMGSRV_E_REQ_REQIDOVER	=   2141, /* req idが最大数をオーバーした    */
  IMGSRV_E_REQ_ILLEGALREQID	=   2142, /* req idが不正                    */
  IMGSRV_E_REQ_REQIDNOTUSED	=   2143, /* 使用してないreq idを指定した    */

  IMGSRV_E_KILL_MSGWRITE	=   2151, /* メッセージ送信でエラーが発生    */
  IMGSRV_E_KILL_MSGREAD		=   2152, /* メッセージ受信でエラーが発生    */
  IMGSRV_E_KILL_MSGMISMATCH	=   2153, /* 期待したメッセージでない        */

  IMGSRV_E_END
};

#endif	/* #define __ImgsrvErrno_H__ */
