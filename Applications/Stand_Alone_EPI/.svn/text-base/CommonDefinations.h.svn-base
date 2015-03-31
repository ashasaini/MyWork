typedef enum {
    I_NO_ERROR = 0,
    EPI_ERROR,      //1
    EPI_VFC_FAILED,  //2
    EPI_NO_RAW_DATA,  //3
    EPI_VF_OPEN,//4
    EPI_VF_CLOSE,//5
    EPI_ERR_EXCEPTION,//6
    EPI_MEM_ALLOC,//7
    EPI_VF_FILE_NOT_FOUND,//8
    EPI_PROT_RAW_DATA,//9
    EPI_ERROR_NULL_FUNCPARAMS,//10
    EPI_DISPLAY_IMAGE,    //11
    EPI_PROT_CHK,//12
    EPI_SELECT_PROTOCOL,    //13    
    EPI_SELECT_RAW_DATA,//14
    EPI_RAW_DATA_SAVED,     //15
    EPI_RECON_FAIL, //16
    EPI_SELECT_STUDYFILEDATA_STEP,//17
    EPI_RAW_CHECK_FAIL,             //18
    EPI_NO_RAW,             //19
    EPI_IRS_FAIL,           //20
    EPI_MERGE_FAIL,         //21
    EPI_PERFORM_SEPARATE, //22
    EPI_SELECT_MERGE_STEP, //23
    EPI_PERFORM_SEP_EVEN_ODD, //24
    EPI_SELECT_TEMPLATE, //25
    EPI_PHASE_CORRECTION_FAIL, //26
    EPI_SELECT_PC_METHOD, //27

};
typedef enum{
    TIME_SHIFT = 0,
    TIME_SHIFT_LINE_FIT,
};

typedef enum{
        T1_SELECTION = 0,
        T2_SELECTION,
};

typedef enum{
    FOLDER_PATH = 0,
    SEPARATE_KT1T2,
    SEPARATE_EVEN_ODD,
    PHASE_CORRECTION,
};


#define MAGNITUDE 0
#define PHASE (MAGNITUDE+1)
#define REAL (MAGNITUDE+2)
#define IMAGINARY (MAGNITUDE+3)
#define NOFORM (MAGNITUDE+4)

#define sMAGNITUDE (_T("Magnitude"))
#define sPHASE (_T("Phase"))
#define sREAL (_T("Real"))
#define sIMAGINARY (_T("Imaginary"))
#define sNOFORM (_T("Noform"))

#define RAWCHECK_STATUS_FILEPATH "C:\\tmp\\EPI_rawCheckStatus.txt"

#define MEMFREE(mptr) if (mptr){ \
    free(mptr); \
    mptr = NULL;\
    }
#define DELARR(arr) if (arr){ \
    delete [] arr; \
    arr = NULL;\
    }
#define DELPTR(dptr) if (dptr){ \
    delete dptr; \
    dptr = NULL;\
    }

typedef double DUMPDATATYPE;