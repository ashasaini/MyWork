// PatientOrientationConstants.h: interface for the CPqmConstants class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_PATIENT_ORIENTATION_CONSTANTS_H__
#define __SM_PATIENT_ORIENTATION_CONSTANTS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum COUCH_POSITION {

    COUCH_POSITION_LEFT ,
    COUCH_POSITION_RIGHT,
    COUCH_POSITION_INVALID
} ;

enum OrientationType {

    OriType_PatientInsertDir = 0,
    OriType_PatientRotationDir,
    OriType_PatientViewDir,
    OrientationType_Total
};

enum PatientInsertDirection {

    Patient_FeetFirst = 0,		//VFC_PATORI_FEET_FIRST
    Patient_HeadFirst,			//VFC_PATORI_HEAD_FIRST
    PatientInsertDirection_Invalid
};

enum PatientRotationDirection {

    PatientRotation_SUPINE = 0,
    PatientRotation_PRONE,
    PatientRotation_RIGHT_UP,
    PatientRotation_LEFT_UP,
    PatientRotationDirection_Invalid
};



enum PatientViewDirection {

    PatientView_FROM_HEAD = 0,
    PatientView_FROM_FEET,
    PatientViewDirection_Invalid
};



#endif // #ifndef __SM_PATIENT_ORIENTATION_CONSTANTS_H__
