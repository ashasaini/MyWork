// Vector3D.cpp: implementation of the CVector3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector3D.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector3DUtility::Vector3DUtility()
{

}

//***************************Method Header*************************************
//Method Name    : setMaxMinPosition()
//Author         : PATNI/Hemant
//Purpose        :
//*****************************************************************************
void Vector3DUtility::setMaxMinPosition(CVector3D* const posVec,
                                        CVector3D* maxPos,
                                        CVector3D* minPos)
{
    if (posVec->x <= minPos->x) minPos->x = posVec->x;

    if (posVec->y <= minPos->y) minPos->y = posVec->y;

    if (posVec->z <= minPos->z) minPos->z = posVec->z;

    if (posVec->x >= maxPos->x) maxPos->x = posVec->x;

    if (posVec->y >= maxPos->y) maxPos->y = posVec->y;

    if (posVec->z >= maxPos->z) maxPos->z = posVec->z;
}


const CVector3D Vector3DUtility::GetMaxXY(
    const CVector3D& f_a,
    const CVector3D& f_b
)
{
    return CVector3D((f_a.x > f_b.x) ? f_a.x : f_b.x , (f_a.y > f_b.y) ? f_a.y : f_b.y , 0);
}


CVector3D::CVector3D(): x(0.0), y(0.0), z(0.0)
{

}

CVector3D::~CVector3D()
{

}

CVector3D::CVector3D(const flt32_t a): x(a), y(a), z(a)
{

}

CVector3D::CVector3D(const flt32_t a, const flt32_t b, const flt32_t c): x(a), y(b), z(c)
{

}

CVector3D::CVector3D(const CVector3D& b)
{
    x = b.x ;
    y = b.y ;
    z = b.z ;
}

const CVector3D CVector3D::operator + (const CVector3D& b) const
{
    return CVector3D(x + b.x , y + b.y, z + b.z);
}

const CVector3D CVector3D::operator - (const CVector3D& b) const
{
    return CVector3D(x - b.x , y - b.y, z - b.z);
}

CVector3D CVector3D::operator * (const CVector3D& b) const
{
    return CVector3D(x * b.x , y * b.y, z * b.z);
}

const CVector3D CVector3D::operator / (const CVector3D& b) const
{

    return CVector3D(x / b.x , y / b.y, z / b.z);
}

const CVector3D& CVector3D::operator = (const CVector3D& b)
{
    x = b.x ;
    y = b.y ;
    z = b.z ;
    return *this;
}

bool CVector3D::operator <= (
    const CVector3D& b
)
{
    return ((x <= b.x) && (y <= b.y) && (z <= b.z));
}

const CVector3D& CVector3D::operator = (const f32vec3_t& b)
{
    x = b.x ;
    y = b.y ;
    z = b.z ;
    return *this;
}

CVector3D CVector3D::operator * (const flt32_t b)
{
    return CVector3D(x * b , y * b, z * b);
}

const CVector3D CVector3D::operator + (const flt32_t b)
{
    return CVector3D(x + b , y + b, z + b);
}

const CVector3D CVector3D::operator - (const flt32_t b)
{
    return CVector3D(x - b , y - b, z - b);
}

void CVector3D::operator /= (flt32_t b)
{
    this->x = this->x / b;
    this->y = this->y / b;
    this->z = this->z / b;
}

CVector3D CVector3D::operator/(const flt32_t a)
{

    return CVector3D(x / a , y / a, z / a);
}

const flt32_t CVector3D::Absolute(
)
{
    return (flt32_t)sqrt(x * x + y * y + z * z);
}

void CVector3D::Normalize(
)
{
    flt32_t l = 0.0f;

    if ((l = Absolute()) == 0.0)
        l = 1.0;

    (*this) /= l ;

    //x /= l;
    //y /= l;
    //z /= l;
}

void CVector3D::CrossProduct(
    const CVector3D& f_a,
    const CVector3D& f_b
)
{

    x = f_a.y * f_b.z - f_a.z * f_b.y;
    y = f_a.z * f_b.x - f_a.x * f_b.z;
    z = f_a.x * f_b.y - f_a.y * f_b.x;
}

const flt32_t CVector3D::DotProduct(
    const CVector3D& f_a,
    const CVector3D& f_b
)
{
    return ((f_a.x * f_b.x) + (f_a.y * f_b.y) + (f_a.z * f_b.z)) ;
}

const flt32_t CVector3D::DotProduct(const CVector3D& f_a)
{

    return DotProduct(*this , f_a);
}


flt32_t CVector3D::GetX(
) const
{
    return x;
}


void CVector3D::SetX(
    const flt32_t f_X
)
{
    x = f_X;
}


flt32_t CVector3D::GetY(
) const
{
    return y;
}

void CVector3D::SetY(
    const flt32_t f_Y
)
{
    y = f_Y ;
}


flt32_t CVector3D::GetZ(
) const
{
    return z;
}

void CVector3D::SetZ(
    const flt32_t f_Z
)
{
    z = f_Z;
}


void CVector3D::SetXYZ(
    const flt32_t f_X,
    const flt32_t f_Y,
    const flt32_t f_Z
)
{

    x = f_X;
    y = f_Y;
    z = f_Z;
}
CString CVector3D::ToString()const
{
    CString vectorlog(_T(""));
    vectorlog.Format(_T("[X-Length: %f] [Y-Length: %f] [Z-Length: %f]"),
                     x, y, z);
    return vectorlog;
}
