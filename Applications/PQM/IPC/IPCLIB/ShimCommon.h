#ifndef __SM_IPCLIB_SHIM_COMMON_H__
#define __SM_IPCLIB_SHIM_COMMON_H__

#define SHIM_DEFAULT_END	-1
#define SHIM_CLEAR_END		-2

#define SHIM_IMAGE_ERROR 	3
#define SHIM_DISPLAY_ERROR 	4
#define SHIM_ACQREQUEST_ERROR 	5

#define SHIM_TO_PQM_SHUTDOWN   10
#define SHIM_IMAGE_NUM_ERROR	13
#define SHIM_AREA_ERROR         14
#define SHIM_HR_ERROR           15
#define MRS_AREA_ERROR          16
#define MRS_SHIM_ERROR          17
#define MRS_F0_ERROR            18
#define MRS_WSAT_ERROR          19
#define MRS_MS_LIMIT_ERROR      20
#define MRS_REF_ERROR          21
#define MRS_PARENT_ERROR        22
#define MRS_F0_CALC_ERROR       23
/* Add V6.20 PAC processing support */
#define SVGEN(n) SVN_GEN_DATA## n
#define ON 1
#define OFF 0
#define SINGLE_VOXEL 1
#define RG_CORRECT_FILE "RGcorrect"
#define MAX_CHANNEL 32

#define TUNE_DIR "/gp/tune/"
#define GAIN_FILE "chGain.dat"
#define PHASE_FILE "chPhase.dat"
#define E_OPEN_FILE 1
#define  MRS_MATH_TSP  4.644
#define F0_WSIG_FILE "f0_wsignal"
#define WSAT_SIG_FILE "wsat_wsignal"
#define TEMP_DIR "/gp/tmp/"

#endif // #ifndef __SM_IPCLIB_SHIM_COMMON_H__
