//Redmine-777
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASCOM_COMM_Def.h
 *  Overview		:
 *  Last Modified	: [6/6/2008]
 *******************************************************************************
 *                       Revision History
 *******************************************************************************
 *  Revision	Author		Date & Time			Changes
 *==============================================================================
 *   1.0		Patni       [6/6/2008]			Added file header
 *
 ******************************************************************************/
#ifndef __SM_PASCOM_COMMUNICATION_DEF_H__
#define __SM_PASCOM_COMMUNICATION_DEF_H__


/// Enumeration for the messages to be passed
typedef enum tagMSG_TYPE_e {

    PASCOM_SEQ_SENDING = 1,
    PASCOM_SEQ_RECEIVING,
    PASCOM_SEQ_SELECTED,
    PASCOM_REQ_PASPATH,
    PASCOM_REGIST_FINISH,
    PASCOM_PQM_SEQ_RECEIVED

} MSG_TYPE;


// Data structure to be exchanged
typedef struct tagPASCOM_COMM_MSG_s {

    MSG_TYPE	msg;
    int         no_of_sequence_selected;
    int         no_of_sequence_sent;
    _TCHAR*		seq_data;
    int mode;
    _TCHAR* sequence_path;
    int acquisition_order;
    //WFDA Nitin
    //CString m_study_loid;
    _TCHAR* m_study_loid;

} PASCOM_COMM_MSG;



///MEENAKSHI N
typedef struct channelstruct {
    CString channel1, channel2;
    int size;
    int curchannel;
} channels;
///MEENAKSHI N

namespace PASCOM_STORE
{
typedef enum {
    TRANSFER_MEMORY = 0,
    COPY_MEMORY,
    PROLIM_MEMORY
} SeqInsertType;
}


#endif  // #ifndef __SM_PASCOM_COMMUNICATION_DEF_H__