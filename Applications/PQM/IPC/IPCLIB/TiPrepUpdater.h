// TiPrepUpdater.h: interface for the CTiPrepUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIPREPUPDATER_H__F91FAED6_D06A_411B_A50B_4D1A66506A28__INCLUDED_)
#define AFX_TIPREPUPDATER_H__F91FAED6_D06A_411B_A50B_4D1A66506A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTiPrepUpdater  
{
public:
	int CreateScratchPad();
	char m_scratchpad_file[256];
	int UpdateTIinStudy(int * f_prot_idx, float * f_ti_val);
	CTiPrepUpdater();
	virtual ~CTiPrepUpdater();

};

#endif // !defined(AFX_TIPREPUPDATER_H__F91FAED6_D06A_411B_A50B_4D1A66506A28__INCLUDED_)
