// PosSlabInfo.cpp: implementation of the PosSlabInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PosSlabInfo.h"
#include "PqmIpcLibConstants.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

msPosSlabInfo_s::msPosSlabInfo_s(
) : sliceThickness(0.0),
    sliceGap(0.0),
    sliceNum(0)
{

}


//***************************Method Header*************************************
//Method Name    : makeCheckPointsInfo()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
void msPosSlabInfo_s::makeCheckPointsInfo(msPosSlabInfo_s* pos)
{
    const flt32_t cen_to_cen_dist = pos->sliceThickness + pos->sliceGap;
    const CVector2D l_ro_pe_vector = (pos->plnFov / 2.0);
    flt32_t mag_sl = (flt32_t)((pos->sliceNum - 1) / 2.0 * cen_to_cen_dist + (pos->sliceThickness / 2.0));

    pos->slabCenter = pos->offsetVec;

    pos->sliceCenter[0] = pos->slabCenter - (pos->sliceVec * mag_sl);
    pos->sliceCenter[1] = pos->slabCenter + (pos->sliceVec * mag_sl);

    CVector3D Y1 = (pos->peVec * l_ro_pe_vector.GetX()) ;
    CVector3D Y2 = (pos->roVec * l_ro_pe_vector.GetY()) ;
    CVector3D Y3 = (pos->sliceVec * mag_sl) ;

    CVector3D X1 = pos->slabCenter - Y2;
    CVector3D X2 = Y1 - Y3;
    CVector3D X3 = pos->slabCenter + Y2;
    CVector3D X4 = Y1 + Y3;

    pos->eachPos[0] = X1 - X2;
    pos->eachPos[1] = X1 + X2;
    pos->eachPos[2] = X3 - X2;
    pos->eachPos[3] = X3 + X2;
    pos->eachPos[4] = X1 - X4;
    pos->eachPos[5] = X1 + X4;
    pos->eachPos[6] = X3 - X4;
    pos->eachPos[7] = X3 + X4;
}

const CVector3D msPosSlabInfo_s::ProductOrientationLength(
    const CVector3D& f_length
)
{
    return CVector3D(ABSF(this->peVec.DotProduct(f_length)),
                     ABSF(this->roVec.DotProduct(f_length)) ,
                     ABSF(this->sliceVec.DotProduct(f_length))
                    ) ;
}