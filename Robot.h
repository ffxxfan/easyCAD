#ifndef ROBOT_H
#define ROBOT_H

#include <QList>

#include <gp_Trsf.hxx>

class RobotLink;

struct DHArg
{
    double theta;
    double d;
    double alpha;
    double a;
};

class Robot
{
public:
    Robot();
    explicit Robot(int dof);
    ~Robot();

    QVector<RobotLink*> GetLinks() const {
        return RLinks;
    }
    void SetLinkShapes(const QVector<RobotLink*> &shapes) {
        Q_ASSERT(shapes.size() == RDOF+1);

        RLinks = shapes;
    }

    QString GetName() const {
        return RName;
    }
    void SetRobotName(const QString &str) {
        RName = str;
    }

    int GetDOF() const {
        return RDOF;
    }
    void SetDOF(int count) {
        RDOF = count;
    }

    gp_Pnt GetTCP() const {
        return RTCP;
    }

    void SetDHData(const QVector<DHArg>& data) {
        RDHData = data;
    }
    QVector<DHArg> GetDHData() const {
        return RDHData;
    }

    void SetMaxPosition(const QVector<double>& args) {
        RMaxPos = args;
    }
    void SetMinPosition(const QVector<double>& args) {
        RMinPos = args;
    }
    QVector<double> GetMaxPosition() const {
        return RMaxPos;
    }
    QVector<double> GetMinPosition() const {
        return RMinPos;
    }

    void SetRPosture(const QVector<double>& args) {
        RPosture = args;
    }
    QVector<double> GetRPosture() const {
        return RPosture;
    }
    void UpdateRPosture(const int index, const double args) {
        RPosture.replace(index, args);
    }

    void SetNewPosture(const QVector<double>& args) {
        NPosture = args;
    }
    QVector<double> GetNewPosture() const {
        return NPosture;
    }
    void UpdateNewPosture(const int index, const double args) {
        NPosture.replace(index, args);
    }

    void PerformFK(const QVector<double> &angs);
    void ComputeTrsfs(const int jointNum, const double dest, const int step);

private:
    int RDOF;
    QString RName;
    QVector<RobotLink*> RLinks;
    QVector<double> RMaxPos;
    QVector<double> RMinPos;
    QVector<double> RPosture;
    QVector<double> NPosture;
    QVector<DHArg> RDHData;
    QVector<QVector<gp_Trsf>> TrsfMatrix;

    gp_Pnt RTCP;

    gp_Trsf dhToTrsf(const DHArg& arg, double rad);
};

#endif // ROBOT_H
