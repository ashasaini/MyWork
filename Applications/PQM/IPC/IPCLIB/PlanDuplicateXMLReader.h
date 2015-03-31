// PlanDuplicateXMLReader.h: interface for the PlanDuplicateXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PLANDUPLICATE_XMLREADER_H__
#define __SM_IPCLIB_PLANDUPLICATE_XMLREADER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PQMXMLParser.h"

class CPlanDuplicateXMLReader  : public CPQMXMLParser
{
public:
    CPlanDuplicateXMLReader();
    virtual ~CPlanDuplicateXMLReader();
    int GetPresetItem(VARIANT** f_preset_item_data);

};
#endif // #ifndef __SM_IPCLIB_PLANDUPLICATE_XMLREADER_H__
