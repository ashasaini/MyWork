#ifndef __PLAN_DUPLICATOR_COMMON_GROUPDEF_H
#define __PLAN_DUPLICATOR_COMMON_GROUPDEF_H
#include "\MRMPlus\include\tami\common\machdefs.h"
#define AN_BI_ELEM_TYPE     int16_t 
#define AN_BD_ELEM_TYPE     int32_t 
#define NO_ZERO_BYTES       128
#define IMPLICIT_VR_LEN     "1.2.840.10008.1.2.1\0"
#define PATIENT_LEVEL       0
#define STUDY_LEVEL         1
#define SERIES_LEVEL        2
#define POSITION_LEVEL      3
const int META_TAG =     0x00020010;

typedef enum {
    VR_BI = 1,
    VR_BD,
    VR_AN,
    VR_AT,
    VR_SP,
    VR_XX
} ElementTypeT;


typedef struct {
    long        nemacode;
    uint32_t        length;
    char            *value;
    int16_t     group; // Tag group
    int16_t     element; // Tag element
    int         level;
    char        *VR;
} dataElementT, *pDataElementT;

typedef struct {
    long        nemacode;
    uint32_t            length;
    AN_BD_ELEM_TYPE     *value;
    ElementTypeT        elementType;
    int16_t             elementName;
    uint32_t            vfName;
    uint32_t            vfType;
    int16_t             vfLevel;
} dataElementBDT, *pDataElementBDT;

typedef struct {
  dataElementT   TranferUID;         /* Modality: element # 0060             */
} Group0002T;


typedef struct {
  dataElementT   modality;       /* Modality: element # 0060             */
} Group0008T;

typedef struct 
{
   dataElementT   patientName;  // 0x0010 0010  
   dataElementT   patientId;    // 0x0010 0020
}Group0010T;

typedef struct 
{
   dataElementT   sliceThickness;    // 0x0018 0050    
   dataElementT   spacingBetweenSlices; // 0x0018 0088    
   //dataElementT   triggerTime; // 0x0018 1060
}Group0018T;

typedef struct 
{
   dataElementT   seriesInstanceUid; // 0x0020 000E
   dataElementT   studyId;   // 0x0020 0010    
    dataElementT   InstanceId; // 0x0020 0013    
    dataElementT   ImagePositionPatient; // 0x0020 0032    
    dataElementT   ImageOrientationPatient; // 0x0020 0037
    dataElementT   sliceLocation; // 0x0020 1041  

}Group0020T;

typedef struct 
{
   dataElementT   rows;  // 0x0028 0010    
   dataElementT   columns; // 0x0028 0011    
   dataElementT   pixelSpacing; // 0x0028 0030
   dataElementT   bitsAllocated;     // 0x0028 0100    
   dataElementT   bitsStored; // 0x0028 0101    
   dataElementT   pixelRepresentation; // 0x0028 0103
    dataElementT   SmallestImagePixelValue;// 0x0028 0106  
    dataElementT   LargestImagePixelValue;// 0x0028 0107  
    dataElementT   SmallestPixelValueInSeries;// 0x0028 0108  
    dataElementT   LargestPixelValueInSeries;// 0x0028 0109  
   //dataElementT   pixelPaddingValue;   // 0x0028 0120 //could not find this tag in MPlusDB
   dataElementT   windowCenter;  // 0x0028 1050    
   dataElementT   windowWidth; // 0x0028 1051    
   //dataElementT   rescaleIntercept; // 0x0028 1052   //could not find this tag in MPlusDB 
   //dataElementT   rescaleSlope; // 0x0028 1053      //could not find this tag in MPlusDB
    dataElementT   samplePerPixel; // 0x0028 0002 //AN
    dataElementT   PhotometricInterpretation; // 0x0028 0004 //AN
}Group0028T;



typedef struct 
{
   dataElementT   currentPatientLocation; // 0x0038 0300
}Group0038T;

typedef struct 
{
   dataElementT   pixelData;     // 0x7FE0 0010    
}Group7FE0T;

// The pair of DICOM tag and its value type
typedef struct tagInfo
{
    TCHAR m_szDICOMTag [ 20 ];
    TCHAR m_szVRType[ 3 ];
} TAGINFO_t;
#endif