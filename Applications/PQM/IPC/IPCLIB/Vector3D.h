// Vector3D.h: interface for the CVector3D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_VECTOR3D_H__
#define __SM_IPCLIB_VECTOR3D_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CVector3D;

class Vector3DUtility : private MR::ACQ::SM::COMMON::NonCopyable<Vector3DUtility>
{

public :
    static void setMaxMinPosition(CVector3D* const posVec, CVector3D* maxPos, CVector3D* minPos);
    static const CVector3D GetMaxXY(const CVector3D& f_a, const CVector3D& f_b);

private :
    Vector3DUtility();
};

class CVector3D
{
    friend Vector3DUtility;

public:
    CVector3D();
    virtual ~CVector3D();
    explicit CVector3D(const flt32_t a);
    CVector3D(const flt32_t a, const flt32_t b, const flt32_t c);

    CVector3D(const CVector3D& a);

    const CVector3D operator+(const CVector3D& b) const;
    const CVector3D operator-(const CVector3D& b) const;
    CVector3D operator*(const CVector3D& b) const;
    const CVector3D operator/(const CVector3D& b) const;
    const CVector3D& operator = (const CVector3D& b);
    bool operator <= (const CVector3D& b);

    const CVector3D& operator = (const f32vec3_t& b);

    CVector3D operator*(const flt32_t b);
    const CVector3D operator+(const flt32_t b);
    const CVector3D operator-(const flt32_t b);

    void operator/=(const flt32_t a);
    CVector3D operator/(const flt32_t a);


    const flt32_t Absolute() ;
    void Normalize();

    void CrossProduct(const CVector3D& f_a, const CVector3D& f_b);
    static const flt32_t DotProduct(const CVector3D& f_a, const CVector3D& f_b);
    const flt32_t DotProduct(const CVector3D& f_a);


    flt32_t GetX() const ;
    void SetX(const flt32_t f_X) ;

    flt32_t GetY() const;
    void SetY(const flt32_t f_Y) ;

    flt32_t GetZ() const;
    void SetZ(const flt32_t f_Z) ;

    void SetXYZ(const flt32_t f_X, const flt32_t f_Y, const flt32_t f_Z) ;
    CString ToString()const;

private:
    flt32_t x, y , z;
};



#endif // #ifndef __SM_IPCLIB_VECTOR3D_H__
