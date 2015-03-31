// T1Map.h: interface for the CT1Map class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_T1MAP_H__99F5498D_EE5D_47DF_AE74_AEEDCB041286__INCLUDED_)
#define AFX_T1MAP_H__99F5498D_EE5D_47DF_AE74_AEEDCB041286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapWriter.h"

class CT1Map
{
public:
    CT1Map();
    virtual ~CT1Map();
    int GetMouseXY(int f_nidx, int &mx, int &my);   
    int CreateT1Map(double * t1val, CString &f_filename);
    int GetWidhtForT1Map();
    int GetHeightForT1Map();
    const long * GetMap();
    int SetMap(byte * roi_map, int height, int width);
    int FindSelectedIndex(int x, int y, int *f_nSelectedIndex); 
    CT1Map(CT1Map& f_pmap);
    CT1Map& operator = (CT1Map& f_map);

private:
    long* m_map;
    int m_mapsize;
    int m_height;   
    int m_width;    
};
#endif // !defined(AFX_T1MAP_H__99F5498D_EE5D_47DF_AE74_AEEDCB041286__INCLUDED_)
