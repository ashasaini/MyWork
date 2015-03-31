
#if !defined(AFX_KTRECONERRORS_H_)
#define AFX_KTRECONERRORS_H_

#ifndef E_ERROR
#define E_ERROR 1
#endif

#ifndef E_NO_ERROR
#define E_NO_ERROR 0
#endif

static char *ErrorList[] = {
        "No error.\n",                              // 0
        "An error has occurred. Please refer log file", //1
        "Could not open the study file.",   // 2
        "No scanned protocols available.",  // 3
        "Please select a scanned protocol.",    // 4
        "Error in get data.",                   // 5
        "Error occurred in k-t Recon.",         // 6
        "Error occurred in k-t ReconViewer.",   // 7
        "No. of threads exceeds max value.",    // 8
        "Failed to invoke process.",               // 9
        "Failed to get studyfile name.",            // 10
        "Study file error! Failed to obtain scanned protocols list.",   //11
        "An Exception has occurred.",           //12
        "Failed to initialize Socket communication. Please restart k-t Recon",//13
        "Memory allocation failed",             //14
        "Failed to load protocol name from Database",   //15
		"Unable to Get Data from Database. DB operation failed",    //16
		"Failed to load Series number from Database",   //17
		"Value of the parameter is not initialized",    //18 NULL value parameter
        "Please select a series on Image Matrix.",      //19        
        "Failed to communicate with Image Matrix.",     //20
        "", //21
};

typedef enum ErrorCode_Enum_e {
        KTRECON_NO_ERROR = 0,               // 0
        E_COMON_ERROR,                  //1
        KTRECON_STUDY_OPEN_ERROR,           // 2
        KTRECON_NO_SCANNED_PROTOCOL,        // 3
        KTRECON_SELECT_SCANNED_PROTOCOL_ERROR,    // 4
        KTRECON_GET_DATA_ERROR,             // 5
        KTRECON_VROM_ERROR,                 // 6
        KTRECON_VIEW_ERROR,                 // 7
        KTRECON_NO_OF_THREADS_ERROR,        // 8
        KTRECON_INVOKE_ERROR,               // 9
        KT_RECON_STUDYNAME_ERROR,           // 10
        KT_RECON_GET_PROT_ERROR,		//11
        E_ERR_EXCEPTION,                //12  
        E_SOCKET_ERR,                   //13
        E_MEM_ALLOC,                //14
        E_IMG_TAG_PROT_FAIL,    //15
		E_DB_ERROR,             //16
		E_SERIES_NO_FAIL,       //17
		E_ERROR_NULL_FUNCPARAMS,    //18
        E_SELECT_IMG_ON_ISEL,          //19
        E_IMAGE_MTRX_COMM_FAILED,   //20
        E_LAST_VAL,             //21
} ErrorCode_Enum_t;

#endif
/////////////////////////////////END OF FILE /////////////////////////////////////