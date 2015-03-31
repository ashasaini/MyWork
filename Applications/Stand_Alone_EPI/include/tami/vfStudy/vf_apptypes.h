/**********************************************************************
*  
*    File Overview:
*	This file should be the home of all varfield types in order
*	to ensure that all type values are unique.
*
*	N.B. THE TYPE VALUES DEFINED HERE MUST NOT
*	CONFLICT WITH THE SPACE RESERVED IN varfields.h (VFT_MIN_RESERVED)
*
*	As with the varfield name space, the space for type values
*	has been partitioned by functional group.
*
**********************************************************************/


/* Revision History:
yy-mm-dd	modified-by	modifications-done
92-11-16	jwu		Changed types for time in VftGatingParam_t from
                                int32_t to flt32_t.
92-08-06        donb            Added VFT_SCHED_STUDY_ID
92-07-13	scottm		Changed VFT_FLTVEC2 and VFT_FLTVEC3 to
                                use appropriate typedefs from basicdefs.h
92-06-15	scottm		Restructured, changed type spaces (for
                                consistency with new name spaces).  Removed
				VFT_MATRIX, VFT_INTVECTOR, VFT_FLTMATRIX.
				Changed VFT_FLTVECTOR to VFT_VEC2 and
				VFT_VEC3.  Removed VFT_MEASURE.
92-05-28	sbs		Added NMR (Nmrtools) needed types.
92-05-19	rjg		Added VFT_FLTMATRIX.
92-04-16	rjg		Added ProSel needed types.
92-04-13	rickm		Added VFT_PIM_OBLANG.
92-03-26	scottm		Added VftFEP_t, and established VFTC_
                                prefix for constants used in Vft definitions.
91-12-10	rickm		Changed vft_reconproc_opts.
91-11-25	rickm		Created
End of History */

#ifndef tami_vfStudy_vf_apptypes_H__
#define tami_vfStudy_vf_apptypes_H__

/* Partition the type space. */

#define         VFT_OFF_GENERAL             0   /* GEN */
#define         VFT_OFF_NMRTOOLS         1000   /* NMR */
#define         VFT_OFF_SCHEDULE         2000   /* SCH */
#define         VFT_OFF_PLAN             3000   /* PLN */
#define         VFT_OFF_SEQUENCE         4000   /* SEQ */
#define         VFT_OFF_EXECUTION        5000   /* EXE */
#define         VFT_OFF_PROCESS          6000   /* PRC */
#define         VFT_OFF_PRESENT          7000   /* PRS */
#define         VFT_OFF_DICOM            8000   /* DCM */


/*****************************************************************************
  GENERAL type space
*/

#define		VFT_STATUS		(VFT_OFF_GENERAL + 0)
/* status_t */
  
#define		VFT_TIME		(VFT_OFF_GENERAL + 1)
/* time_t */
  
#define		VFT_TM			(VFT_OFF_GENERAL + 2)
/* struct tm (cf. man ctime) */

#define		VFT_XMSTRING		(VFT_OFF_GENERAL + 3)
/* XmString */

#define		VFT_PATHELEM		(VFT_OFF_GENERAL + 4)

#define		VFT_FLTVEC2 		(VFT_OFF_GENERAL + 5)
/* use f32vec2_t from basicdefs.h */

#define		VFT_FLTVEC3 		(VFT_OFF_GENERAL + 6)
/* use f32vec3_t from basicdefs.h */

/* A type to represent a vector in patient coordinates
** - corresponds to f32patvec3_t from basicdefs.h
*/
#define		VFT_PATFLTVEC3		(VFT_OFF_GENERAL + 7)

#define		VFT_SORT_TABLE_ELEMENT	(VFT_OFF_GENERAL + 8)
/*
 * Data is collected into a single large array. For each line of data
 * to be read,
 *  offset      indicates the starting position of the line in the buffer.
 *  fepLength   indicates the length of the buffer into which the line is
 *              placed by the FEP.
 *  acqFirstVal indicates the position in the buffer of the first valid point.
 *  acqLastVal  indicates the position in the buffer of the last valid point.
 *
 *  reqLength   indicates the length of the buffer into which the line is to
 *              be read.
 *  reqFirstVal indicates the position to write the first valid point.
 *  reqLastVal  indicates the position to write the last valid point.
 *
 * At this time it is assumed
 *  -  reqLength >= FEPLength (partial data can be obtained after the reading)
 *  -  if (acqFirstVal > acqLastVal) the data is reversed when read
 *  -  if (reqFirstVal > reqLastVal) the data is reversed when written
 *     (if you make them both reversed, you haven't done much, have you?)
 *  -  (acqLastVal - acqFirstVal +1) <= (+/-)*fepLength, end points are zero
 *        padded.
 *  -  (reqLastVal - reqFirstVal +1) = (+/-)(n+1)*reqLength,
 *        where n is the number of spaces (zeros) placed between adjacent
 *        acquisition points.
 */
typedef struct VftSortTableElement_s
{
  int offset;                   /* location at which to begin reading */

  short fepLength;              /* size of data when xferred from FEP */
  short acqFirstVal;            /* first valid point to read */
  short acqLastVal;             /* last valid point to read */

  short reqLength;              /* size of data buffer to be filled */
  short reqFirstVal;            /* first buffer position to write */
  short reqLastVal;             /* last buffer position to write */
} VftSortTableElement_t;


#define		VFT_FLTVEC4 		(VFT_OFF_GENERAL + 9)
#define		VFT_INTVEC2 		(VFT_OFF_GENERAL + 10)
#define		VFT_INTVEC3 		(VFT_OFF_GENERAL + 11)
#define		VFT_INTVEC4 		(VFT_OFF_GENERAL + 12)
#define		VFT_INTARRAY 		(VFT_OFF_GENERAL + 13)
#define		VFT_LOCAL_TIME		(VFT_OFF_GENERAL + 14)

#define		VFT_CDS_COIL_LOCATION	(VFT_OFF_GENERAL + 15)

typedef struct VftCdsCoilLocation_s {
    uint32_t    coilID;         /* コイルID */
    bool_t      detectedFlag;   /* データ有効/無効フラグ */
    int32_t     location;       /* コイル位置(mm単位) */
} Vft_CdsCoilLocation_t;



/*****************************************************************************
  NMRTOOLS type space
*/

#define       VFT_NMR_FIT             (VFT_OFF_NMRTOOLS + 0)

typedef       struct  VftNmrFit_s {
    flt32_t   coefs[4];               /* fit coefficients cubic maximum */
    flt32_t   position;               /* the fit position value */
    int16_t   seqValue;               /* the current sequence value */
} VftNmrFit_t;

#define       VFT_NMR_FWHM            (VFT_OFF_NMRTOOLS + 1)

typedef       struct  VftNmrFwhm_s {
    flt32_t   fwhm[256];              /* calculated fwhm for each PE line */
    flt32_t   lineNum[256];           /* PE line # corresponding to fwhm */
    int16_t   numPoints;              /* number of calculated FWHM points */
    int16_t   seqValue;               /* the current sequence value */
} VftNmrFwhm_t;

#define       VFT_NMR_MINMAX          (VFT_OFF_NMRTOOLS + 2)

typedef       struct  VftNmrMinmax_s {
    flt32_t   data[256];              /* caluclated max for each PE line */
    int16_t   position[256];          /* calculated max position */
    flt32_t   lineNum[256];           /* PE line # correspoinding to max */
    int16_t   numPoints;              /* number of caluclated max points */
    int16_t   seqValue;               /* the current sequence value */
} VftNmrMinmax_t;

#define       VFT_NMR_SLOPE           (VFT_OFF_NMRTOOLS + 3)

typedef       struct  VftNmrSlope_s {
    flt32_t   slope[256];             /* calculated slope for each PE line */
    flt32_t   lineNum[256];           /* PE line # corresponding to slope */
    int16_t   numPoints;              /* number of calculated slope points */
    int16_t   seqValue;               /* the current sequence value */
} VftNmrSlope_t;

#define		VFT_NMR_SIGNAL		(VFT_OFF_NMRTOOLS + 4)
typedef	struct	VftNmrSignal_s {
	flt32_t	mean;			/* the signal meas for selected ROI */
	flt32_t	stdDev;			/* standard dev. for selected ROI */
} VftNmrSignal_t;

#define		VFT_NMR_UNIFORM		(VFT_OFF_NMRTOOLS + 5)

typedef	struct	VftNmrUniform_s {
	int16_t	numAreas;		/* number of ROI's in structure */
	flt32_t	mean[9];		/* signal mean for each selected ROI */
	flt32_t	stdDev[9];		/* stardard dev. for each ROI */
} VftNmrUniform_t;

#define		VFT_NMR_NOISE		(VFT_OFF_NMRTOOLS + 6)

typedef	struct	VftNmrNoise_s {
	int16_t	numAreas;		/* number of noise areas in structure */
	flt32_t	mean[4];		/* noise mean for each selected ROI */
	flt32_t	stdDev[4];		/* stardard dev. for each noise ROI */
} VftNmrNoise_t;

#define		VFT_NMR_EDGE_SPREAD	(VFT_OFF_NMRTOOLS + 7)

typedef	struct	VftNmrEdgeSp_s {
	flt32_t	xPixels;		/* edge spread in X direction */
	flt32_t	yPixels;		/* edge spread in Y direction */
} VftNmrEdgeSp_t;

#define		VFT_NMR_PARALLEL	(VFT_OFF_NMRTOOLS + 8)

typedef	struct	VftNmrPara_s {
	int16_t	xPixels;		/* parallelogram in X direction */
	int16_t	yPixels;		/* parallelogram in Y direction */
} VftNmrPara_t;

#define		VFT_NMR_RESOL		(VFT_OFF_NMRTOOLS + 9)

typedef	struct	VftNmrResol_s {
	flt32_t	xPixels;		/* resolution for X in pixels */
	flt32_t	yPixels;		/* resolution for Y in pixels */
} VftNmrResol_t;

#define		VFT_NMR_SLICE_PROF	(VFT_OFF_NMRTOOLS + 10)

typedef	struct	VftNmrSliceP_s {
	int16_t	numProfiles;		/* number of slice profiles */
	flt32_t	maximum[6];		/* maximum for each slice profile */
	flt32_t	fwhmLeft[6];		/* left FHWM edge for slice profiles */
	flt32_t	fwhmRight[6];		/* right FWHM edge for slice profs. */
} VftNmrSliceP_t;



/*****************************************************************************
  SCHEDULE type space
*/

#define       VFT_SCHED_STUDY_ID        (VFT_OFF_SCHEDULE + 0)
/* study_id_t from study_id.h */

/*****************************************************************************
  PLAN type space
*/

#define		VFT_GATING_PARAM	(VFT_OFF_PLAN + 0)

typedef struct VftGatingParam_s {
    int32_t 	gatingMode;		/* Normal, cycled or cine */
    int32_t 	cardiacRate;		/* beats per minute */
    int32_t 	taggingPattern;		/* Radial, Grid, etc. */
    int32_t 	taggingDensity;		/* Density of tagging pattern */
    int32_t	numPeriods;		/* number of gating periods */
    flt32_t	gatingPeriod;		/* periodicity in sec for the wave */
    flt32_t	gatImagPeriod;		/* Time in sec available for imaging */
    int32_t	imagPeriod;		/* Imaging during inhalation/exhalation */
    int32_t	respRate;		/* Breaths per minute */
} VftGatingParam_t;

#define		VFT_GATING_MANAGER	(VFT_OFF_PLAN + 1)

typedef struct VftGatingMgr_s {
    int32_t 	trigSource;		/* interrupts by Heart Monitor or by
					   Gate Manager (differentiated ECG wave)*/
    int32_t 	respTrigThresh;		/* Respiratory Detection Threshold */
    int32_t	ECGTrigThresh;		/* ECG Detection threshold */
    int32_t	trigBasis;		/* Trigger on rising/falling edge */
    int32_t     encodesPerCycle;	/* One/many encodes in one imaging period*/
    int32_t	delayTime;		/* delay time from trigger to imaging */
} VftGatingMgr_t;

#define		VFT_PATORI		(VFT_OFF_PLAN + 2)

typedef struct VftPatori_s {
    int32_t	end_in;		/* End of patient that entered the magnet */
    int32_t	side_up;	/* Side of patient facing up in magnet */
} VftPatori_t;			/* Orientation of patient in magnet */

#define		VFT_TIME_CTRL_PARAM	(VFT_OFF_PLAN + 3)

#define VFTC_TMCTRL_MAX_SEGMENT	5

typedef struct VftTimeCtrlParam_t {
    int32_t	totalTime;	/* Total Time (msec) */
    int32_t	numSegment;	/* The number of segment */
    struct {
	int32_t		start;	/* Offset from start point (msec) */
	int32_t		period;	/* Period (msec) */
	int32_t		repeat;
    } segment[VFTC_TMCTRL_MAX_SEGMENT];
} VftTimeCtrlParam_t;

#define		VFT_DEF_CONTRAST	(VFT_OFF_PLAN + 4)

typedef struct VftDefContrast_s {
	int32_t		processed_node ;     /* processed node */
	int32_t		echo_node ;          /* echo node */
	int32_t		position_node ;      /* position node */
	flt32_t		wl ;                 /* Window Level(%) */
	flt32_t		ww ;                 /* Window Width(%) */
	int32_t		pos_neg ;            /* 1:POSITIVE   -1:NEGATIVE */
} VftDefContrast_t;

#define		VFT_PBM		(VFT_OFF_PLAN + 5)

typedef struct VftPbm_s {
	char		type[4];     /* date type */
	int32_t		width ;       /* 横幅 */
	int32_t		height ;      /* 高さ */
	int32_t		max ;         /* 最大値 */
} VftPbm_t;

#define         VFT_AMB_PARAM   (VFT_OFF_PLAN + 6)

#define VFTC_AMB_SEGMENT_MAX	4
#define VFTC_AMB_STATION_MAX    6

typedef struct VftAmbParam_s {
    int32_t         numStation;     /* Station数 */
    int32_t         numSegment;     /* Segment数 */
    int32_t         stationOrder;   /* 撮像時の寝台移動情報 0:outside(Station順), 1:inside(逆) */
    int32_t         shimming;       /* 0:off, 1:0n */
    int32_t         map;            /* 0:off, 1:0n */
    int32_t         direction[VFTC_AMB_SEGMENT_MAX];    /* 0:outside, 1:inside */
    int32_t         mapAutoVoice[VFTC_AMB_STATION_MAX]; /* 0:off, 1:B1, 2:B2, 3:B3, 4:B4 */
} VftAmbParam_t;

#define         VFT_AMB_CELL   (VFT_OFF_PLAN + 7)

typedef struct VftAmbCell_s {
    struct {
	int32_t		prepEnable;     /* VisualPrep 指定可能有無 0:可能, 1:可能*/
	int32_t		prep;           /* 0:off, 1:0n */
	int32_t		repeat;         /* 繰り返し回数 */
    } cell[VFTC_AMB_SEGMENT_MAX][VFTC_AMB_STATION_MAX];
} VftAmbCell_t;

#define         VFT_AMB_POST_PROC   (VFT_OFF_PLAN + 8)

typedef struct VftAmbPostProc_s {
        int32_t         subtruction;    /* (Bitmap) Ox1:差分無し画像, 0x2:Absolute, 0x4:Complex */
        int32_t         base;           /* 基準時相 */
        int32_t         mipPreview;     /* 0:off, 1:Maximum Value, 3:3 Directions, 4:Run Mip */
        int32_t         gainID;         /* msGetNRMipNumber()で取得 */
        int32_t         gainFilterNo;   /* Gain Algorithmフィルター番号*/
        flt32_t         mipBase[3];     /* X,Y,Z */
        int32_t         mipAngle;
        int32_t         mipSlice;
} VftAmbPostProc_t;

#define    VFT_SECTION_RECVR_COIL   (VFT_OFF_PLAN + 9)

typedef struct VftSectionRecvrCoil_s{
    uint8_t     portID[2];      /* 文字列、左詰めで空きはスペースいれる */
    uint16_t    coilID;         /* 物理的なコイルID */
    uint8_t     SectionID;      /* Saturnコイルのみ、従来コイルの場合はゼロ */
    uint8_t     channel;        /* 従来コイルの場合だけ有効で、チャネル番号 */
} VftSectionRecvrCoil_t;

#define    VFT_SECTION_SIGNAL   (VFT_OFF_PLAN + 10)

typedef struct VftSectionSignal_s{
    int16_t     num;                /* 有効Signal数 */
    uint16_t    signal_no[16];      /* Signal 情報 */
    uint16_t    hybrid_signal[16];  /* Hybrid 情報 */
}VftSectionSignal_t;

#define    VFT_COIL_SHOWLIST   (VFT_OFF_PLAN + 11)

typedef struct VftCoilListInfo_s {
    int32_t     num;            /* コイル数 */
    struct {
        uint8_t     portID[2];  /* Port番号 */
        uint16_t    coilID;     /* CoilID 物理番号 */
        uint8_t     channel;    /* Channel情報 */
        uint8_t     coordinate; /* 表示位置 */
        uint8_t     filler[2];
    } coillist[12];
} VftCoilListInfo_t;

/*****************************************************************************
  SEQUENCE type space
*/

#define		VFT_FEP			(VFT_OFF_SEQUENCE + 0)

#define VFTC_MAX_FEP_LOOPS 	16	/* Maximum number of loops supported
					   by RM -- must be consistent with
					   tami/mrsedDefs.h */
#define VFTC_MAX_FEP_ECHOS	4	/* Maximum number of echos supported
					   by RM -- must be consistent with
					   tami/rmDefs.h */
#define VFTC_AVG_LOOP_NAME_LEN	16	/* Size of name of average loop name */

typedef struct VftFEPEcho_s
{
    int32_t count;		/* number of times to use this	*/
    /* echo definition	*/
    int32_t bufferLength;	/* Length of the echo buffer	*/
    int32_t startingPoint;	/* The starting point in the	*/
    /* buffer for the echo	*/
    int32_t samples;		/* Number of points to sample in the echo */
    flt32_t DCoffsetI;		/* The in-phase channel DC offset */
    flt32_t DCoffsetQ;		/* The quadrature channel DC off set */
    flt32_t balance;		/* The magnitude correction factor */
}  VftFEPEcho_t;

typedef struct VftFEP_s
{			
    int32_t algorithm;		/* The FEP algorithm */
    int32_t numLoops;		/* Number of loops with loopCount defined */
    int32_t loopCount[VFTC_MAX_FEP_LOOPS];	/* Loop counts */
    int32_t loopSortOrder[VFTC_MAX_FEP_LOOPS];	/* Loop sort order */
    int32_t sliceBarLoops;	/* The number of loops in loopSortOrder that
				   are INSIDE a slice (single image) */
    int32_t averageLoop;	/* Loop number of averaging loop */
    int32_t transferLoop;	/* Loop number of transfer loop */
    int32_t overflowLoop;	/* Loop number of raw data overflow loop */
    int32_t steadyStateLoop; 	/* Loop number of steady state loop */
    char averageLoopName[VFTC_AVG_LOOP_NAME_LEN];
    /* Name of the averaging loop in the sequence
       -- must match appropriate rmDefs.h
       #define */
    int32_t dataFormat;		/* Data format (short, int, or float) of the
				   MRI data */
    int32_t dblChanByteCount;	/* Byte count for 2-channel FEP-RB xfers */
    
    int32_t numEchos;		/* Number of valid echo definitions */
    VftFEPEcho_t echos[VFTC_MAX_FEP_ECHOS];	/* Echo definitions */
} VftFEP_t;




/*****************************************************************************
  EXECUTION type space
*/

#define		VFT_COIL		(VFT_OFF_EXECUTION + 0)

typedef struct VftCoil_s {
    char  	port;
    char  	channel;
    uint16_t	id;
} VftCoil_t;




/*****************************************************************************
  PROCESS type space
*/

#define		VFT_RECONPROC_OPTS	(VFT_OFF_PROCESS + 0)

typedef struct vft_reconproc_opts {
    bool_t	fftFlag;
    bool_t	swapFlag;
    bool_t	flipFlag;
    bool_t	conjFlag;
    bool_t	zoomFlag;
    bool_t	smoothFlag;
} vft_reconproc_opts_t;

#define		VFT_PIM_OBLANG		(VFT_OFF_PROCESS + 1)

typedef struct VftPimOblang_s {
    flt32_t	hor_oblang;
    flt32_t	ver_oblang;
} VftPimOblang_t;

#define		VFT_ICON_STRUCT		(VFT_OFF_PROCESS + 2)

typedef struct VftIcon_s {
    bool_t	makeF;
    int32_t	type;
    int32_t	side;
}VftIcon_t;

#define		VFT_CARDIAC_OUTLINE	(VFT_OFF_PROCESS + 3)

/*****************************************************************************
  PRESENT type space
*/
/* Two dimensial integer coordinates */
typedef struct VftIxy_s{
    int32_t     x;
    int32_t     y;
}VftIxy_t;

#define   	VFT_ZOOM_STRUCT 	(VFT_OFF_PRESENT + 0)
typedef struct VftZoom_s{
    int32_t	type;
    flt32_t	factor;
}VftZoom_t;



#define         VFT_SCROLL_STRUCT    (VFT_OFF_PRESENT + 1)
typedef struct VftScroll_s{
    int32_t	type;
    int32_t     x;
    int32_t	y;
    VftIxy_t    lowerLeft;	/* from dWin->imageP->viewPort.lowerLeft */
    VftIxy_t    upperRight;	/* from dWin->imageP->viewPort.upperRight */
}VftScroll_t;


#define		VFT_CROP_STRUCT    	(VFT_OFF_PRESENT + 2)
typedef struct VftCrop_s{
    VftIxy_t	x;
    VftIxy_t	y;
    bool_t  	xWrap;
    bool_t      yWrap;
}VftCrop_t;

#define 	VFT_GRID_STRUCT         (VFT_OFF_PRESENT + 3)
typedef struct VftGrid_s{
    bool_t	visible;
}VftGrid_t;

#define         VFT_CONTRAST_STRUCT	(VFT_OFF_PRESENT + 4)
typedef struct VftContrast_s{
    flt32_t	window;		/* normalized within 0 to 1 */
    flt32_t	level;		/* normalized within 0 to 1 */
    bool_t      reverse;	/* slope of the ramp is negative */
}VftContrast_t;

#define		VFT_TRANSFORM_STRUCT   	(VFT_OFF_PRESENT + 5)
typedef struct VftTransform_s{
    bool_t	flipLR;		/* flip left/right with the initial position */
    float	angle;	        /*angle in degrees mod with 360, measured in
				  counterclockwise derection after flipLR*/
}VftTransform_t;    
    
#define		VFT_IMAGE_TRANSFORM_STRUCT	(VFT_OFF_PRESENT + 6)
typedef struct VftImageTransform_s {
    int32_t	magic	;	/* NASU				*/
    int32_t	version	;	/* display version		*/
    bool_t	flipH	;	/* flip flag for Horizontal	*/
    bool_t	flipV	;	/* flip flag for Vertical	*/
    flt64_t	angle	;	/* rotation angle		*/
    flt64_t	zoom	;	/* zoom percent			*/
    flt64_t	x	;	/* display origin for X		*/
    flt64_t	y	;	/* display origin for Y		*/
} VftImageTransformStruct_t;    

#define		VFT_IMAGE_CONTRAST		(VFT_OFF_PRESENT + 7)
typedef struct VftImageContrast_s {
    int32_t	magic	;	/* NASU				*/
    int32_t	version	;	/* display version		*/
    uint32_t	count	;	/* number of contrast tables	*/
    int32_t	body	;	/* contast body			*/
} VftImageContrast_t ;
    
/*****************************************************************************
  DICOM type space
*/

#define DCM_VR_LEN_AE   (16)
#define DCM_VR_LEN_CS   (16)
#define DCM_VR_LEN_DA   (8)
#define DCM_VR_LEN_DS   (16)
#define DCM_VR_LEN_LO   (64)
#define DCM_VR_LEN_LT   (1024)
#define DCM_VR_LEN_PN   (64)
#define DCM_VR_LEN_SH   (16)
#define DCM_VR_LEN_TM   (16)
#define DCM_VR_LEN_UI   (64)
#define DCM_VR_LEN_US   (2)
#define DCM_VR_LEN_IS   (12)

/* (0032,1064) DICOM Requested Procedure Code Sequence */
/* (0040,0008) DICOM Scheduled Action Item Code Sequence */
#define VFT_DCM_CODE_SEQ_STRUCT		(VFT_OFF_DICOM + 0)
typedef struct {
    int32_t     idx;                             /* Serial Index                                    */
    char        code_value[DCM_VR_LEN_SH*3];     /* (0008,0100) Code Value                          */
    char        code_scheme[DCM_VR_LEN_SH*3];    /* (0008,0102) Coding Scheme Designator            */
    char        code_version[DCM_VR_LEN_SH*3];   /* (0008,0103) Code Version                        */
    char        code_mean[DCM_VR_LEN_LO*3];      /* (0008,0104) Code Meaning                        */
} DicomCodeSeq_t;

/* (0008,1110) DICOM Referenced Study Sequence */
#define VFT_DCM_REF_STUDY_SEQ_STRUCT	(VFT_OFF_DICOM + 1)
typedef struct {
    int32_t     idx;                             /* Serial Index                                    */
    char        Ref_SOP_UID[DCM_VR_LEN_UI+1];    /* (0008,1150) Referenced SOP Class UID            */
    char        Ref_Inst_UID[DCM_VR_LEN_UI+1];   /* (0008,1155) Referenced SOP Instance UID         */
} DicomRefStudySeq_t;

/* (0040,0100) DICOM Scheduled Procedure Step Sequence */
#define VFT_DCM_SPS_SEQ_STRUCT		(VFT_OFF_DICOM + 3)
typedef struct {
    int32_t     idx;                             /* Serial Index                                    */
    char        AEtitle[DCM_VR_LEN_AE+1];        /* (0040,0001) AE Title                            */
    char        sDate[DCM_VR_LEN_DA+1];          /* (0040,0002) Start Date                          */
    char        sTime[DCM_VR_LEN_TM+1];          /* (0040,0003) Start Time                          */
    char        eDate[DCM_VR_LEN_DA+1];          /* (0040,0004) End Date                            */
    char        eTime[DCM_VR_LEN_TM+1];          /* (0040,0005) End Time                            */
    char        physician[DCM_VR_LEN_PN*4];      /* (0040,0006) Physician                           */
    char        desc[DCM_VR_LEN_LO+1];           /* (0040,0007) Procedure Step Description          */
    char        stepID[DCM_VR_LEN_SH+1];         /* (0040,0009) Procedure Step ID                   */
    char        modality[DCM_VR_LEN_CS+1];       /* (0008,0060) Modality                            */
    int32_t     idxActionItem;                   /* Index  of Scheduled Action Item Code Sequence   */
    int32_t     numActionItem;                   /* Number of Scheduled Action Item Code Sequence   */
} DicomSpsSeq_t;

/* DICOM Modality WorkList */
#define VFT_DCM_WORKLIST_STRUCT		(VFT_OFF_DICOM + 4)
typedef struct {
    char        medicalAlert[DCM_VR_LEN_LO+1];   /* (0010,2000) Medical Alerts                      */
    char        accesionNo[DCM_VR_LEN_SH+5];     /* (0008,0050) Accession Number                    */
    char        refPhysician[DCM_VR_LEN_PN*4];   /* (0008,0090) Referring Physician's Name          */
    char        reqProcID[DCM_VR_LEN_SH+1];      /* (0040,1001) Requested Procedure ID              */
    char        instanceUID[DCM_VR_LEN_UI+1];    /* (0020,000D) Study Instance UID                  */
    char        reqDesc[DCM_VR_LEN_LO+1];        /* (0032,1060) Requested Procedure Description     */
    int32_t     idxReqCodeSeq;                   /* Index  of Requested Procedure Code Sequence     */
    int32_t     numReqCodeSeq;                   /* Number of Requested Procedure Code Sequence     */
    int32_t     idxRefStudySeq;                  /* Index  of Referenced Study Sequence             */
    int32_t     numRefStudySeq;                  /* Number of Referenced Study Sequence             */
    int32_t     idxSpsSeq;                       /* Index  of Scheduled Procedure Step Sequence     */
    int32_t     numSpsSeq;                       /* Number of Scheduled Procedure Step Sequence     */
} DicomWorkList_t;

/* (0040,0321) DICOM Film Consumption Sequence */
#define VFT_DCM_FILM_CONSUMPTION_SEQ_STRUCT	(VFT_OFF_DICOM + 5)
typedef struct {
    char        MediumType[DCM_VR_LEN_CS+1];     /* (2000,0030) Medium Type                         */
    char        FilmSizeID[DCM_VR_LEN_CS+1];     /* (2010,0050) Film Size ID                        */
    char        NumberofFilms[DCM_VR_LEN_IS+1];  /* (2100,0170) Number of Films                     */
} DicomFilmOutInfo_t;

/* (0010,0010) Patient Name */
#define VFT_DCM_ORIGINAL_PATIENT_NAME		(VFT_OFF_DICOM + 6)
typedef struct {
    char        lastName[DCM_VR_LEN_PN+1];      /* Last name    */
    char        firstName[DCM_VR_LEN_PN+1];     /* First name   */
    char        middleName[DCM_VR_LEN_PN+1];    /* Middle name  */
    char        prefix[DCM_VR_LEN_PN+1];        /* Prefix       */
    char        suffix[DCM_VR_LEN_PN+1];        /* Suffix       */
} VftOriginalPatientName_t;

/* Host Name */
#define VFT_DCM_HOST_LIST			(VFT_OFF_DICOM + 7)
typedef struct {
    char        hostName[DCM_VR_LEN_LO];        /* Host name    */
} VftHostList_t;

#endif /* !tami_vfStudy_vf_apptypes_H__ */

