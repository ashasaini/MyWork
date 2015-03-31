// WFDAImplementer.h: interface for the CWFDAImplementer class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __SM_WFDA_IMPLEMENTER_H__
#define __SM_WFDA_IMPLEMENTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WFDAConstants.h"


class CWFDAHandler;
class CWFDAHandlerInterface ;



class CWFDAImplementer : private MR::ACQ::SM::COMMON::NonCopyable<CWFDAImplementer>
{

public:
    CWFDAImplementer();
    CWFDAImplementer(CPQMView* f_pqm_view);

    virtual ~CWFDAImplementer();

    CWFDAHandlerInterface* GetWFDAHandlerInterface() const ;

    bool NewSequencesAdded(const int f_count) const ;

    bool DuplicateProtocol(const int f_duplicate_index) const ;
    bool ScanStatusChanged(const int f_acq_order, const  WFDA_Protocol_Status f_status) const ;

    bool IsProtocolInscan(const int f_acq_order)const ;
    bool OnScanEditStatusChangedForProtocol(const int f_acq_order,
                                            const WFDA_Protocol_Status f_wfda_protocol_status
                                           ) const ;
private :

    CWFDAHandler* m_wfda_handler;
    CPQMView* m_pqm_view;
};

#endif // #ifndef __SM_WFDA_IMPLEMENTER_H__


