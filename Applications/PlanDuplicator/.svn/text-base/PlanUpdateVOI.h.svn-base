// PlanUpdateVOI.h: interface for the CPlanUpdateVOI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANUPDATEVOI_H__206BDCAE_F2F8_4527_BA9E_51B57E72789C__INCLUDED_)
#define AFX_PLANUPDATEVOI_H__206BDCAE_F2F8_4527_BA9E_51B57E72789C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlanUpdate.h"

class CPlanUpdateVOI : public CPlanUpdate  
{
public:
	CPlanUpdateVOI();
	virtual ~CPlanUpdateVOI();

    int UpdateVector(CPDCoordinates *f_VOI_Coordinates,VfPathElem_t f_path[], int f_sliceplane_selection);
    int UpdateSliceAttributes(CPDCoordinates *f_Coordinates,int f_attribute_selection, int f_sliceplane_selection);

};

#endif // !defined(AFX_PLANUPDATEVOI_H__206BDCAE_F2F8_4527_BA9E_51B57E72789C__INCLUDED_)
