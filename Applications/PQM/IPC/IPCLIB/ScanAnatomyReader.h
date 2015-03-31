//Redmine-781_CPP_TestTool
//Redmine-780
// ScanAnatomyReader.h: interface for the CScanAnatomyReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SCANANATOMY_READER_H__
#define __SM_IPCLIB_SCANANATOMY_READER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMXMLParser.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"


class CScanAnatomyReader : public CPQMXMLParser
{
public:
    //Constructor / Destructor
    CScanAnatomyReader();
    virtual ~CScanAnatomyReader();

    bool GetScanAnatomyData(VARIANT** f_scan_anatomy_data, const long f_max_saranatomy_limit, const long f_max_scananatomy_limit);

private:

    int  GetSarAnatomyIndex(CPTString f_sar_anatomy);

};
#endif // #ifndef __SM_IPCLIB_SCANANATOMY_READER_H__ 
