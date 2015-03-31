// PlanUpdate.h: interface for the CPlanUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANUPDATE_H__94FA6B88_221D_4369_9F9F_2F825EACF599__INCLUDED_)
#define AFX_PLANUPDATE_H__94FA6B88_221D_4369_9F9F_2F825EACF599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dataTypes.h"
#include "PDCoordinates.h"

#include <winsock2.h>
#include <tami/libWNP/WinNativePorting.h>
#include <tami/libpltree/Plsftree.h>
#include <tami/libpl/plParDefs.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/vfStudy/vf_appcodes.h>

#include "PDLogHelper.h"
/************************************************************************/
/* Base class used for planning the sequence 
/************************************************************************/
class CPlanUpdate  
{
public:
	CPlanUpdate();
	virtual ~CPlanUpdate();

    int UpdatePlanStatus(/*VfPathElem_t  f_path[]*/);/*PLN_STATUS update.*/
    int UpdateLocator(/*VfPathElem_t  f_path[],*/ int f_slice_number);
    int UpdateSelectedSliceAttribute(CPDCoordinates *f_Coordinates,int f_attribute_selection);
    int UpdateFOV(CPDCoordinates *f_Coordinates);
    
    virtual int UpdateVector(CPDCoordinates *f_VOI_Coordinates,VfPathElem_t f_path[], int f_sliceplane_selection) = 0;
    virtual int UpdateSliceAttributes(CPDCoordinates *f_Coordinates,int f_attribute_selection, int f_sliceplane_selection) = 0;
    
protected:

};

typedef CPlanUpdate* PLUPDATEHANDLE;
#endif // !defined(AFX_PLANUPDATE_H__94FA6B88_221D_4369_9F9F_2F825EACF599__INCLUDED_)



