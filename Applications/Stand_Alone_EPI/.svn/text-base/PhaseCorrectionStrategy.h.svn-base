// PhaseCorrectionStrategy.h: interface for the CPhaseCorrectionStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHASECORRECTIONSTRATEGY_H__4C4E1FD9_E1E7_4F8C_AC48_391E547785E1__INCLUDED_)
#define AFX_PHASECORRECTIONSTRATEGY_H__4C4E1FD9_E1E7_4F8C_AC48_391E547785E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef V35M
extern "C" {
#include    <tami/vfStudy/vfstudy.h>
#include    <tami/vfStudy/vf_appcodes.h>
#include    <tami/libvf/vf_errors.h>
#include    <tami/common/study_id.h>
#include    <tami/common/errors.h>
#include <tami/libvf/varfields.h>
};
#endif
class CPhaseCorrectionStrategy  
{
public:
	CPhaseCorrectionStrategy();
	virtual ~CPhaseCorrectionStrategy();
    int performPhaseCorrection(f64complex_t *f_dTempDataBuff,/*inP*/
                               int f_nHeight,/*inP*/
                               int f_nWidth,/*inP*/
                               int f_nPhaseMethodSelectionFlag,/*inP*/
                               f64complex_t *f_dKspaceDataBuff /*oP*/);

};

#endif // !defined(AFX_PHASECORRECTIONSTRATEGY_H__4C4E1FD9_E1E7_4F8C_AC48_391E547785E1__INCLUDED_)
