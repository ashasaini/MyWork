// ShimmingRequestStructures.h: interface for the CShimmingRequestStructures class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SHIMMINGREQUEST_STRUCTURES_H__
#define __SM_IPCLIB_SHIMMINGREQUEST_STRUCTURES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tami/vfStudy/vfstudy.h>
#include <tami/libvf/varfields.h>       /* Varfield definitions         */

class CVarFieldHandle;

//const int COIL_PORT_MASK = 0xFF000000;
//const int BREAST_SPDR_COIL = 0x4600003b;
//const int COIL_PHYSICALID_MASK = 0x0000FFFF;
//const int WBCOIL_PHYSICAL_ID = 0x0000;
//const int MAXIMUM_COIL_IN_PORT = 16;

const int VF_STUDY_NAME_LENGTH = 128;
const int SHIM_FATAL_ERROR = 1;
const int SHIM_POSITION_ERROR = 2;
const int SHIM_ARG_ERROR = 3;
const int SHIM_MS_ERROR = 4;
const int SHIM_CANCEL = 5;
const int SHIM_TO_PQM_ERROR = 6;
const int SHIM_FM_PQM_ERROR = 7;
const int SHIM_RESULT_ERROR = 8;
const int SHIM_SLICE_ORI_ERROR = 9;
const int MAX_STRING_SHIM = 256;
const int GRAD_CHANNEL = 3;
const int SHIM_COIL = 8;
const int WAIT_TIME_MAX = 9;
const int B0_SHIFT_LIMIT = 702;
const int VF_PATH_DEPTH = 8;

const int SHIM_AUTO_MODE = 0;
const int SHIM_MANUAL_MODE 	= 1;
const int ERROR_MODE = 1;
const int NORMAL_MODE = 0;
const int SHIM_NORMAL_END = 0;
typedef struct {
    int			calc_processed;
    int			adjust_processed;
    int			adjust_protocol;
    int			totalSlice;
    int 		grad_default[GRAD_CHANNEL];
    int 		shim_default[SHIM_COIL];
    float		b0_default;
    float*		axical_offsetP;
    float*		sagital_offsetP;
    float*		coronal_offsetP;
    float*		hist_valueP;
    int 		grad_offset[GRAD_CHANNEL];
    int 		shim_offset[SHIM_COIL];
    int          shim_flag;
} SShimInfo_t, *SSimInfoPtr;

typedef struct {
    char*        	study;
    int 		protocol;
    CVarFieldHandle* vfHandle;
    VfPathElem_t*	 nodes;
    VfPathElem_t*	 imageNodes;
    int			writeFD;
    int			readFD;
    f32vec3_t		startPosi;
    f32vec3_t		endPosi;
    int			shimInfo_mode;
    int			debug_mode;
    int			F0_mode;
    int			WaitTimeMax;
    int			B0ShiftLimit;
} SShimStudy_t, *SSimStudyPtr;

typedef struct st_position {
    f32vec3_t 		offsetVec;
    f32vec3_t 		sliceVec;
    f32vec3_t		peVec;
    f32vec2_t 		fovVec;
    int     		sliceNum;
    float   		thickness;
    float   		cen_to_cen_dist;
    float   		sliceGap;
    int     		from;
    int     		to;
    float   		fovRoi;
    f32vec3_t		roiOffset;
    f32vec3_t		unitRow;
    f32vec3_t		unitCol;
    st_position() {
        memset(&offsetVec, NULL, sizeof(f32vec3_t));
        memset(&sliceVec, NULL, sizeof(f32vec3_t));
        memset(&peVec, NULL, sizeof(f32vec3_t));
        memset(&fovVec, NULL, sizeof(f32vec3_t));
        sliceNum = 0;
        thickness = 0.0;
        cen_to_cen_dist = 0.0;
        sliceGap = 0.0;
        from = 0;
        to = 0;
        fovRoi = 0.0;
        memset(&roiOffset, NULL, sizeof(f32vec3_t));
        memset(&unitRow, NULL, sizeof(f32vec3_t));
        memset(&unitCol, NULL, sizeof(f32vec3_t));
    }
} position_t;

typedef struct SShimGlobal_s {
    //pid_t 		    pid;
    SShimStudy_t*	 shimStudyP;
    SShimInfo_t*	shimInfoP;
    int			    end_flag;
    position_t*		posiDataP;
    double          wait_time;

    SShimGlobal_s() :
        shimStudyP(NULL),
        shimInfoP(NULL),
        end_flag(0),
        posiDataP(NULL),
        wait_time(0)
    { }

} SShimGlobal_t ;


typedef struct ShimReqInfo_s {
    char		    study[VF_STUDY_NAME_LENGTH ];
    int 		    protocol;
    int 		    path_len;
    VfPathElem_t	path[SVD_ST_POSITION];

    ShimReqInfo_s() :
        protocol(0),
        path_len(0) {
        memset(study, 0, sizeof(study));
        memset(path, 0, sizeof(path));
    }

} SShimReqInfo_t;


#define CHANNEL_X   	0
#define CHANNEL_Y   	1
#define CHANNEL_Z   	2
#define CHANNEL_ZX 	3
#define CHANNEL_XY 	4
#define CHANNEL_ZY 	5
#define CHANNEL_Z2 	6
#define CHANNEL_X2Y2 	7

#define SHIMAXICAL  0
#define SHIMSAGITAL 1
#define SHIMCORONAL 2
#define SHIM_SURFACE_ERROR -1
#define E_SHIM_SLICE_ORI_ERROR -2

#define OFFSET_COPY 0
#define OFFSET_CLEAR 1
#define OFFSET_DEFAULT 2
#define OFFSET_BEFORE 3
#define OFFSET_CANCEL 4

#define SHIM_AUTO_MODE 		0
#define SHIM_MANUAL_MODE 	1
#define ERROR_MODE		1
#define NORMAL_MODE		0

#define SHIM_NORMAL_END         0
/*
#define SHIM_DEFAULT_END        -1
#define SHIM_CLEAR_END          -2
*/
#define SHIM_FATAL_ERROR        1
#define SHIM_POSITION_ERROR     2
#define SHIM_ARG_ERROR          3
#define SHIM_MS_ERROR           4
#define SHIM_CANCEL             5
#define SHIM_TO_PQM_ERROR       6
#define SHIM_FM_PQM_ERROR       7
#define SHIM_RESULT_ERROR       8
#define SHIM_SLICE_ORI_ERROR    9

#define SHIM_WAIT               10
#endif // #ifndef __SM_IPCLIB_SHIMMINGREQUEST_STRUCTURES_H__
