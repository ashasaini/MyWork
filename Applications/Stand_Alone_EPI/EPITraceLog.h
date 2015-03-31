// EPITraceLog.h: interface for the EPITraceLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPITRACELOG_H__5A22B493_FC9A_4BB8_B973_6C007242E81C__INCLUDED_)
#define AFX_EPITRACELOG_H__5A22B493_FC9A_4BB8_B973_6C007242E81C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EPILogHelper.h"
#include "EPIErrorLog.h"
#include "EPITraceLog.h"
class EPITraceLog : public CEPILogHelper  
{
public:
	EPITraceLog();
	virtual ~EPITraceLog();

};

#endif // !defined(AFX_EPITRACELOG_H__5A22B493_FC9A_4BB8_B973_6C007242E81C__INCLUDED_)
