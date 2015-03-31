// PlanUpdateMPR.h: interface for the CPlanUpdateMPR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANUPDATEMPR_H__478007F3_ECE9_4EAD_8D76_D7EA7D6694A2__INCLUDED_)
#define AFX_PLANUPDATEMPR_H__478007F3_ECE9_4EAD_8D76_D7EA7D6694A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "C:\MRMPlus\include\tami\common\basicdefs.h"
#include "PlanUpdate.h"

class CPlanUpdateMPR : public CPlanUpdate  
{
public:
	CPlanUpdateMPR();
	virtual ~CPlanUpdateMPR();

    int UpdateVector(CPDCoordinates* f_MPR_Coordinates,VfPathElem_t f_path[], int f_sliceplane_selection = 0);
    int UpdateSliceAttributes(CPDCoordinates* f_Coordinates,int f_attribute_selection = 0, int f_sliceplane_selection = 0);
    
private:
};

#endif // !defined(AFX_PLANUPDATEMPR_H__478007F3_ECE9_4EAD_8D76_D7EA7D6694A2__INCLUDED_)


