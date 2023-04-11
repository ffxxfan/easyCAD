#include "Robot.h"

#include "RobotLink.h"

Robot::Robot() : RDOF(0)
{
}

Robot::Robot(int dof) : RDOF(dof)
{
}

Robot::~Robot()
{
    for(int i=0;i<RLinks.size();++i) {
        delete RLinks[i];
    }
}

// robot forward kinematic
void Robot::PerformFK(const QVector<double> &angs)
{
    Q_ASSERT(angs.size() == RDOF);

    if(RDHData.isEmpty())
        // Inform no DH data
        return;

    for(int i = 0; i < angs.size(); i++) {
        // RPosture[i] = RPosture[i] + angs[i];
        if(angs[i] > RMaxPos[i] || angs[i] < RMinPos[i]) {
            // Inform out of range of joint limits
            return;
        }
    }

    RTCP = gp_Pnt(0,0,0);
    gp_Trsf apply;
    for(int i=0;i<RDHData.size()-1;++i) {
        DHArg dh = RDHData[i];
        apply.Multiply(dhToTrsf(dh,angs[i]*M_PI/180.0));
        RLinks[i+1]->ApplyTrsf(apply);
    }

    DHArg dh = RDHData.last();
    apply.Multiply(dhToTrsf(dh,0));
    RTCP.Transform(apply);
}

void Robot::ComputeTrsfs(const int jointNum, const double dest, const int step)
{
    gp_Trsf apply;
    QVector<gp_Trsf> anniTrsfs;
    DHArg dh = RDHData[jointNum];
    int preSize = TrsfMatrix[jointNum-1].size();
    if(preSize == 1) {
        for(int i = RPosture[jointNum]; i < dest; i = i + step) {
            apply = TrsfMatrix[jointNum][0];
            apply.Multiply(dhToTrsf(dh,i*M_PI/180.0));
            anniTrsfs.append(apply);
        }
        apply = TrsfMatrix[jointNum][0];
        apply.Multiply(dhToTrsf(dh,dest*M_PI/180.0));
        anniTrsfs.append(apply);
        TrsfMatrix[jointNum] = anniTrsfs;
    }else if(preSize < jointNum){

    }

}

gp_Trsf Robot::dhToTrsf(const DHArg &arg, double rad)
{
    gp_Trsf cvt;
    double theta = arg.theta+rad; double d = arg.d; double alpha = arg.alpha; double a = arg.a;
    cvt.SetValues(cos(theta), -sin(theta)*cos(alpha), sin(theta)*sin(alpha), a*cos(theta),
                  sin(theta), cos(theta)*cos(alpha), -cos(theta)*sin(alpha), a*sin(theta),
                  0, sin(alpha), cos(alpha), d);

    return cvt;
}
