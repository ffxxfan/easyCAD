#include "RobotLink.h"

#include <AIS_Shape.hxx>

RobotLink::RobotLink()
{
}

RobotLink::~RobotLink()
{
    RShape.Nullify();
}

void RobotLink::ApplyTrsf(const gp_Trsf &trsf)
{
    /**********************
     * SetLocalTransformation
     * void PrsMgr_PresentableObject::SetLocalTransformation(const gp_Trsf & theTrsf)
     *  Sets local transformation to theTransformation. Note that the local transformation
     *  of the object having Transformation Persistence is applied within Local Coordinate
     *  system defined by this Persistence.
     **********************/
    RShape->SetLocalTransformation(trsf);
}
