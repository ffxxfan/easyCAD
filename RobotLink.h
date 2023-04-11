#ifndef ROBOTLINK_H
#define ROBOTLINK_H

#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>

class AIS_Shape;
class RobotCreator;

class RobotLink
{
    friend class RobotCreator;

public:
    RobotLink();
    ~RobotLink();

    void ApplyTrsf(const gp_Trsf &trsf);

    Handle(AIS_Shape) RShape;
};

#endif // ROBOTLINK_H
