/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: stringdefines.h
 *  Overview: Structure of string defines used in PQM
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPCLIB_STRING_DEFINES_H__
#define __SM_IPCLIB_STRING_DEFINES_H__
#define PQM_WARN_BASE                        10000
#define PQM_WARN_PRESCAN_NUMBER              PQM_WARN_BASE + 0
#define PQM_WARN_PRESCAN_COUCH               PQM_WARN_BASE + 1
#define PQM_WARN_PRESCAN_ANATOMY             PQM_WARN_BASE + 2
#define PQM_WARN_PRESCAN_COIL                PQM_WARN_BASE + 3
#define PQM_WARN_PRESCAN_DIFF_COIL           PQM_WARN_BASE + 4
#define PQM_WARN_PRESCAN_MOVING_COUCH        PQM_WARN_BASE + 5
#define PQM_WARN_PRESCAN_PUMP                PQM_WARN_BASE + 6
#define PQM_WARN_PRESCAN_FREEZER             PQM_WARN_BASE + 7
#define PQM_WARN_PRESCAN_PUMP_AND_FREEZER    PQM_WARN_BASE + 8
#define PQM_WARN_PRESCAN_SPEEDER_COUCH       PQM_WARN_BASE + 9
#define PQM_WARN_PRESCAN_SPEEDER_COIL        PQM_WARN_BASE + 10
#define PQM_WARN_PRESCAN_INTENSITY_COUCH     PQM_WARN_BASE + 11
#define PQM_WARN_PRESCAN_PVC_COUCH_POSI      PQM_WARN_BASE + 12
#define PQM_WARN_PRESCAN_HOME_POS            PQM_WARN_BASE + 13
#define PQM_WARN_PRESCAN_PROJECTION          PQM_WARN_BASE + 14
#define PQM_ACQ_BIT_WARNING                  0x2000

#define PQM_ERROR_BASE                       15000
#define ERR_ACQMAN_ACQINFO                   PQM_ERROR_BASE + 0
#define ERR_ACQMAN_ACQINFO_DONE              PQM_ERROR_BASE + 1
#define ERR_ACQMAN_RFINTERLOCK               PQM_ERROR_BASE + 2
#define ERR_ACQMAN_RFINTERLOCK_DONE          PQM_ERROR_BASE + 3
#define ERR_ACQMAN_PRESCAN                   PQM_ERROR_BASE + 4
#define ERR_ACQMAN_PRESCAN_DONE              PQM_ERROR_BASE + 5
#define ERR_ACQMAN_PRESCAN_CONTINUE          PQM_ERROR_BASE + 6
#define ERR_ACQMAN_PRESCAN_CONTINUE_DONE     PQM_ERROR_BASE + 7
#define ERR_ACQMAN_ACQUISITION               PQM_ERROR_BASE + 8
#define ERR_ACQMAN_ACQUISITION_DONE          PQM_ERROR_BASE + 9
#define ERR_ACQMAN_ACQUISITION_CONTINUE      PQM_ERROR_BASE + 10
#define ERR_ACQMAN_ACQUISITION_CONTINUE_DONE PQM_ERROR_BASE + 11

#define NAK_STR                             _T("NAK")
#define COUCH_STR                           _T("COUCH")
#define TXCOIL_STR                          _T("TXCOIL")
#define RXCOIL_STR                          _T("RXCOIL")
#define GATEMETHOD_STR                      _T("GATEMETHOD")
#define SEQGEN_STR                          _T("SEQGEN")
#define SAR_STR                             _T("SAR")
#define SAR_COIL_STR                        _T("SAR_COIL")

#endif  // #ifndef __SM_IPCLIB_STRING_DEFINES_H__
