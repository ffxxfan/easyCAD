#ifndef ROBOTCREATOR_H
#define ROBOTCREATOR_H

#include <QString>
#include <QList>
#include <AIS_Shape.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <IGESControl_Controller.hxx>

class Robot;

class RobotCreator
{
public:
    RobotCreator(const QString &jsonPath);

    void SetJsonPath(const QString& str) {
        myJsonName = str;
    }
    void SetInput(Robot* input) {
        myInputRobot = input;
    }

    void run();

private:
     QString myJsonName;
     Robot* myInputRobot;
     QVector<double> myInitPosture;

    void LoadJson(const QString &fileName);
    Handle(AIS_Shape) importSTEP(const QString& str);
    Handle(AIS_Shape) importIGES(const QString& str);
};

#endif // ROBOTCREATOR_H
