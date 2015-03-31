// Vector2D.cpp: implementation of the CVector2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector2D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector2D::CVector2D(
) : x(0.0),
    y(0, 0)
{

}

CVector2D::~CVector2D()
{

}

CVector2D::CVector2D(const flt32_t a, const flt32_t b): x(a), y(b)
{
}
CVector2D::CVector2D(const CVector2D& b)
{
    x = b.x ;
    y = b.y ;
}

const CVector2D CVector2D::operator + (const CVector2D& b) const
{
    return CVector2D(x + b.x , y + b.y);
}

const CVector2D CVector2D::operator - (const CVector2D& b) const
{
    return CVector2D(x - b.x , y - b.y);
}

const CVector2D CVector2D::operator * (const CVector2D& b) const
{
    return CVector2D(x * b.x , y * b.y);
}

const CVector2D CVector2D::operator / (const CVector2D& b) const
{

    return CVector2D(x / b.x, y / b.y);
}

const CVector2D& CVector2D::operator = (const CVector2D& b)
{
    x = b.x ;
    y = b.y ;
    return *this;
}

const CVector2D CVector2D::operator * (const flt32_t b)
{
    return CVector2D(x * b , y * b);
}

const CVector2D CVector2D::operator + (const flt32_t b)
{
    return CVector2D(x + b , y + b);
}

const CVector2D CVector2D::operator - (const flt32_t b)
{
    return CVector2D(x - b , y - b);
}

const CVector2D CVector2D::operator/(const flt32_t b)
{

    //b = b > 0 ? b : 1 ;
    return CVector2D(x / b , y / b) ;
}

flt32_t CVector2D::GetX(
) const
{
    return x;
}

void CVector2D::SetX(
    const flt32_t f_X
)
{
    x = f_X;
}

flt32_t CVector2D::GetY(
) const
{
    return y;
}

void CVector2D::SetY(
    const flt32_t f_Y
)
{
    y = f_Y;
}

void CVector2D::SetXY(
    const flt32_t f_X,
    const flt32_t f_Y
)
{
    x = f_X;
    y = f_Y;
}