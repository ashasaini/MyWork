#ifndef _IPCCOMCP_H_
#define _IPCCOMCP_H_

/************************************************************************************

    Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved

    Module: PQM - IPC

    Author: PATNI- HAR

*************************************************************************************/
#include <PQM/PQMCommanMessages.h>

template <class T>
class CProxy_IPqmIpcComEvents : public IConnectionPointImpl<T, &IID__IPqmIpcComEvents, CComDynamicUnkArray>
{
    //Warning this class may be recreated by the wizard.
public:
    //+ MEITEC/2010Apr23/Added/IR86
    HRESULT Fire_SendMsgToUI(INT Msg) {
        HRESULT ret;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++) {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            _IPqmIpcComEvents* p_IPqmIpcComEvents = reinterpret_cast<_IPqmIpcComEvents*>(sp.p);

            if (p_IPqmIpcComEvents != NULL)
                ret = p_IPqmIpcComEvents->SendMsgToUI(Msg);
        }

        return ret;

    }
    HRESULT Fire_DisplayDebugData(BSTR f_displaydata) {
        HRESULT ret;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++) {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            _IPqmIpcComEvents* p_IPqmIpcComEvents = reinterpret_cast<_IPqmIpcComEvents*>(sp.p);

            if (p_IPqmIpcComEvents != NULL)
                ret = p_IPqmIpcComEvents->DisplayDebugData(f_displaydata);
        }

        return ret;

    }
    //- MEITEC/2010Apr23/Added/IR86
};
#endif  //#ifndef _IPCCOMCP_H_