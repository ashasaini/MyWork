// Vector2D.h: interface for the CVector2D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_VECTOR2D_H__
#define __SM_IPCLIB_VECTOR2D_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector2D
{
public:
    CVector2D();
    virtual ~CVector2D();

    CVector2D(const flt32_t a, const flt32_t b);
    CVector2D(const CVector2D& a);

    const CVector2D operator+(const CVector2D& b) const;
    const CVector2D operator-(const CVector2D& b) const;
    const CVector2D operator*(const CVector2D& b) const;
    const CVector2D operator/(const CVector2D& b) const;
    const CVector2D& operator = (const CVector2D& b);

    const CVector2D operator*(const flt32_t b);
    const CVector2D operator+(const flt32_t b);
    const CVector2D operator-(const flt32_t b);
    const CVector2D operator/(const flt32_t b);

    flt32_t GetX() const ;
    void SetX(const flt32_t f_X) ;

    flt32_t GetY() const;
    void SetY(const flt32_t f_Y) ;

    void SetXY(const flt32_t f_X, const flt32_t f_Y) ;

private:
    flt32_t x, y;

};

#endif // #ifndef __SM_IPCLIB_VECTOR2D_H__
