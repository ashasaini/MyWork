#ifndef __SM_IPCLIB_PQM_SHIM_STRUCT_H__
#define __SM_IPCLIB_PQM_SHIM_STRUCT_H__

#include <tami/libvf/varfields.h>       /* Varfield definitions         */
#include <tami/vfStudy/vf_appcodes.h>   /* Values for varfield defs     */
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vfstudy.h>       /* Study hierarchy defs         */
#include <tami/vfStudy/vf_apptypes.h>
#include "ShimmingRequestStructures.h"

class CVarFieldHandle;

#define GRAD_CHANNEL	3
#define SHIM_COIL	8

#define MAX_STRING_SHIM 256
#define	SHIM_CALC_INDEX	0
#define SHIM_ADJUST_INDEX 1
#define SHIM_WATER 0
#define SHIM_FAT 1

#define STUDY_NAME_LENGTH 128

#define CENTERPPM     4.644

#ifndef MAX
#define MAX(A,B) 	(A>B)?A:B
#endif

#define MRS_PROC_START 0
#define MRS_PROC_LOCAL 1
#define MRS_PROC_F0    2
#define MRS_PROC_REF   3
#define MRS_PROC_WSAT  4
#define MRS_PROC_OTHER 5
#define MRS_PROC_F0_END 6

#define MRS_STATE_WAIT 1
#define MRS_STATE_SKIP 2
#define MRS_STATE_CURR 3
#define MRS_STATE_DONE 4
#define MRS_STATE_FAIL 5

#define MRS_EXE_AUTO 0
#define MRS_EXE_MANUAL 1


typedef struct {
    int  local;
    int  f0;
    int spec_graph ;			//Patni-Ravindra Acharya/02Dec2010/Added/IR-168
    int  ref;
    int  wsat;
} mrsState_t;

/* Local Definitions for shim scratchpad */

#define SVN_SHIM_PATH		SVN_SEQ_HR_SHIM_PATH
#define SVT_SHIM_PATH		SVT_SEQ_HR_SHIM_PATH
#define SHIM_PROCESSED_INDEX	2
#define VF_PATH_DEPTH 8

typedef struct {
    int			calc_processed;
    int			adjust_processed;
    int			adjust_protocol;
    int                 hr_shim_processed;
    int			totalSlice;
    //XtInputId		receiveMessageId;
    int		grad_default[GRAD_CHANNEL];
    int		shim_default[SHIM_COIL];
    double		b0_default;
    float*		axical_offsetP;
    float*		sagital_offsetP;
    float*		coronal_offsetP;
    float*		hist_valueP;
    int		grad_offset[GRAD_CHANNEL];
    int		shim_offset[SHIM_COIL];
    int             localShimLimit;
} shimInfo_t, *shimInfoPtr;

typedef struct {
    f32vec3_t           offsetVec;
    f32vec3_t           sliceVec;
    f32vec3_t           peVec;
    f32vec3_t           roVec;
    f32vec2_t           fovVec;
    int             sliceNum;
    float             thickness;
    float             sliceGap;
    float             wholeThickness;
    int             tx_coil;
    int             rx_coil;
    int             matrixPE;
    int             matrixRO;
    int             matrixSO;
    f32vec2_t           seq_acq_matrix;
    int             PacFlag;
    VfPathElem_t        locImages[4];
    float             voxelSO;
    f32vec3_t           voxelOffsetVec;
} mrsInfo_t, *mrsInfoPtr;

typedef struct {
    f32vec3_t           offsetVec;
    f32vec3_t           sliceVec;
    f32vec3_t           peVec;
    f32vec3_t           roVec;
    f32vec2_t           fovVec;
    int             sliceNum;
    float             thickness;
    float             sliceGap;
    float             wholeThickness;
    f32vec3_t		startPosi;
    f32vec3_t		endPosi;
    f32vec2_t           acq_matrix;
} hr_shimInfo_t, *hr_shimInfoPtr;

typedef struct {
    f32vec3_t           offsetVec;
    f32vec3_t           sliceVec;
    f32vec3_t           peVec;
    f32vec3_t           roVec;
    f32vec3_t           fovVec;
} mrsSelected_t, *mrsSelectedPtr;

typedef struct {
    float             fwhm_hz;
    float             fwhm_ppm;
    float             cf_init_hz;
    float             cf_calc_hz;
    float             cf_gap_ppm;
} f0Info_t, *f0InfoPtr;

typedef struct {
    float             pln_nex;
    f32vec2_t           seq_acq_matrix;
    f32vec2_t           pln_displayed_matrix;
    int             dummy_shot;
    float             tr;
    int iRefMode;
} refInfoParam_t;

typedef struct {
    refInfoParam_t      refInfo_single;
    refInfoParam_t      refInfo_multi;
    int             iValueAA;  /* V6.20 Added in PAC */
    float             fFr_from;  /* V6.20 Added in PAC */
    float             fFr_to;    /* V6.20 Added in PAC */
    bool_t              show_graph;
    bool_t              show_spect;
    int             num_point;
    double             exe_fsy;
    char*                seq_link_image;
    char*                seq_link_rcvrgain;
    f32vec2_t           fovVec;
    float             thickness;
    int             ratio;
    f32vec2_t           main_acq_matrix;
    int             ref_matrix[3];
    int*             fsData;
    bool_t              refdata_1D; /* Added for identifying 1D scan */
    int	                rawdim;     /* ÉfÅ[É^éüå≥êî */
} refInfo_t, *refInfoPtr;

typedef struct {
    double             exe_fsy;
    char*                seq_link_image;
    char*                seq_link_rcvrgain;
    float             pln_nex;
    f32vec2_t           seq_acq_matrix;
    f32vec2_t           pln_displayed_matrix;
    int             num_point;
    int             start_angle;
    int             step_angle;
    float             minVolume;
    float             volume;
    float             corVolume;
    float             F0c;
    float             d_F0c;
    bool_t              show_graph;
    bool_t              show_spect;
    int             DC_point;
    int             dummy_shot;
    f32vec2_t           fovVec;
    float             thickness;
    float             F0_max_mag;
    float             F0_rgn;
    float             exe_max_mag;
    float             exe_rgn;
} wsatInfo_t, *wsatInfoPtr;

typedef struct {
    CHAR* 		study;
    CHAR*      scratchPad;
    int 		protocol;
    CVarFieldHandle* vfHandle;
    VfPathElem_t*	 nodes;
    VfPathElem_t*	 imageNodes;
    VfPathElem_t*        hrShimNodes;
    VfPathElem_t*        shimNodes;
    VfPathElem_t*        presatNodes;
    char*		 tmp_file;
    int			writeFD;
    int			readFD;
    f32vec3_t		startPosi;
    f32vec3_t		endPosi;
    int			shimInfo_mode;
    int			debug_mode;
    int			F0_mode;
    int                 acq_hold_mode;
} shimStudy_t, *shimStudyPtr;



typedef struct {
    int shimAcqRequest_id ;
    int refAcqRequest_id ;
    int wsatAcqRequest_id ;
    //XtInputId shimAcqInput_id;
} shimCommID_t;

enum {
    k_numParam = 128
};

typedef enum {
    k_shimNowDataPB = 0,
    k_shimClear,
    k_shimFovScale,
    k_shimDefault,
    k_shimRun,
    k_shimAdjustPB,
    k_shimManualNumWidget
} shimManualWidgetIndex_t;

typedef struct {

    float		fov;
    VfPathElem_t*	processP;

    int			process_num;
} shimManuWin_t;

#define k_shimNumLineGC	4

typedef struct {
    f32complex_t*	cf_dataP;
    int			cf_dataSize;
    int			cf_dataPoint;
    int			cf_realPoint;

    float*		cf_drawP;
    int			cf_drawPoint;

    float		yMax;
    float		yMin;
    float		xMin;
    float		xMax;
    double		range;
    int 		water_fat;
    unsigned int	black;
    unsigned int	white;
    //XFontStruct		*smallFont;
    //    GC			back;
    //    GC			fore;
    //    GC			line[k_shimNumLineGC];
    shimManuWin_t*	shimManualP;
} shimWindow_t;

typedef struct {
    //    pid_t 		pid;
    //MrmHierarchy 	hierId;
    //XtAppContext 	app;
    //MrsUI_p             ui;
    shimStudy_t*	 shimStudyP;
    shimCommID_t 	shimComm;
    shimWindow_t*        shimWin;
    shimInfo_t*		shimInfoP;
    mrsInfo_t*           mrsInfoP;
    hr_shimInfo_t*       hr_shimInfoP;
    f0Info_t*            f0InfoP;
    refInfo_t*           refInfoP;
    wsatInfo_t*          wsatInfoP;
    mrsState_t*          mrsStateP;
    mrsSelected_t*       mrsSelectedP;
    int			end_flag;
    position_t*		posiDataP;
    char		tmp[MAX_STRING_SHIM];
    int             exec_mode;
    /* Begin by Nao.Furudate 1997.02.27 */
    bool_t		ready_F0;	/*F0 ready? */
    bool_t		wait_F0;	/* Ready F0 waiting for the OK button to press

    /* End */

} shimGlobal_t ;

static shimGlobal_t* globalP = NULL;
static shimStudy_t s_shimStudy;;
static position_t  s_position;

static VfPathElem_t s_shimImagePath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_SUBPROTOCOL, 0},
    {SVN_ST_SLICEGROUP, 0},
    {SVN_ST_SLICE, 0},
    {SVN_ST_IMAGE, 0}
};

static VfPathElem_t s_shimEchoPath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_PROCESSED, 0},
    {SVN_ST_ECHO, 0},
    {SVN_ST_POSITION, 0}
};
static VfPathElem_t s_hr_shimPath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_PROCESSED, 0},
};
static VfPathElem_t s_shimPath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_PROCESSED, 0},
};
static VfPathElem_t s_presatPath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_SUBPROTOCOL, 0},
    {SVN_ST_PRESAT, 0},
};

static shimInfo_t	s_shimInfo;
static mrsInfo_t  s_mrsInfo;
static hr_shimInfo_t s_hr_shimInfo;
static wsatInfo_t  s_wsatInfo;
static refInfo_t  s_refInfo;
static f0Info_t  s_f0Info;
static mrsState_t  s_mrsState;
static mrsSelected_t s_mrsSelected;
static shimManuWin_t s_shimManualWin;

#endif // #ifndef __SM_IPCLIB_PQM_SHIM_STRUCT_H__ 





