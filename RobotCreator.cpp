#include "RobotCreator.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Robot.h"
#include "RobotLink.h"

#include <QDebug>

RobotCreator::RobotCreator(const QString& jsonPath)
    : myJsonName(jsonPath),
      myInputRobot(nullptr),
      myInitPosture({0,0,0,0,0,0})
{
}

// 执行LoadJson
void RobotCreator::run()
{
    LoadJson(myJsonName);
}

// 解析JSON文件，构建机器人
void RobotCreator::LoadJson(const QString &fileName)
{
    // Check if robot is null
    if(!myInputRobot)
        return;

    // Read Json file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = file.readAll();
    file.close();

    // Parse Json
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
    if (parseJsonErr.error != QJsonParseError::NoError) {
        qCritical()<<parseJsonErr.errorString();
        return;
    }

    QJsonObject jsonObject = document.object();    

    // 1.dof of robot
    qInfo()<<"DOF:"<<jsonObject["DOF"].toInt();
    myInputRobot->SetDOF(jsonObject["DOF"].toInt());

    // 2.robot's name
    qInfo()<<"name:"<<jsonObject["Name"].toString();
    myInputRobot->SetRobotName(jsonObject["Name"].toString());

    // 3.shape files' paths
    QVector<RobotLink*> list;
    if (jsonObject.contains("Shapes")) {
        QJsonValue arrayValue = jsonObject.value("Shapes");
        if (arrayValue.isArray()) {
            QJsonArray array = arrayValue.toArray();
            for (int i = 0; i < array.size(); i++) {
                QJsonValue pathArray = array.at(i);
                QJsonObject path = pathArray.toObject();
                RobotLink *aLink = new RobotLink;

                qInfo()<<"Path:"<<path["Path"].toString();
                QFileInfo aInfo(path["Path"].toString());

                if(aInfo.isFile()) {
                    if(aInfo.suffix().toLower() == "step" || aInfo.suffix().toLower() == "stp") {
                        aLink->RShape = importSTEP(path["Path"].toString());
                    }
                    else if(aInfo.suffix().toLower() == "iges" || aInfo.suffix().toLower() == "igs") {
                        aLink->RShape = importIGES(path["Path"].toString());
                    }
                    else {
                        qCritical()<< QObject::tr("The format of robot shape file is unsupported!");
                        return;
                    }
                }
                else {
                    qCritical()<< QObject::tr("The element %1 is not a file!").arg(path["Path"].toString());
                    return;
                }

                list.append(aLink);
            }
        }
    }
    myInputRobot->SetLinkShapes(list);

    // 4.DH arguments of each link
    // 在机械工程中，Denavit-Hartenberg 参数(也称为 DH 参数)是与一个特定约定相关联的四个参数，用于将参考坐标系附加到空间运动链或机器人操作臂的连杆上。
    QVector<DHArg> dhArgs;
    if (jsonObject.contains("DH")) {
        QJsonValue arrayValue = jsonObject.value("DH");
        if (arrayValue.isArray()) {
            QJsonArray array = arrayValue.toArray();
            for (int i = 0; i < array.size(); i++) {
                QJsonValue TrsfArray = array.at(i);
                QJsonObject trsf = TrsfArray.toObject();
                qInfo()<<"theta:"<<trsf["theta"].toDouble()
                      <<"d:"<<trsf["d"].toDouble()
                     <<"alpha:"<<trsf["alpha"].toDouble()
                    <<"a:"<<trsf["a"].toDouble();

                dhArgs.append({trsf["theta"].toDouble()*M_PI/180.0,
                              trsf["d"].toDouble(),
                              trsf["alpha"].toDouble()*M_PI/180.0,
                              trsf["a"].toDouble()});
            }
        }
    }

    // 5.limits of each joint
    QVector<double> mins, maxs;
    if (jsonObject.contains("Limits")) {
        QJsonValue arrayValue = jsonObject.value("Limits");
        if (arrayValue.isArray()) {
            QJsonArray array = arrayValue.toArray();
            for (int i = 0; i < array.size(); i++) {
                QJsonValue alimitArray = array.at(i);
                QJsonObject limit = alimitArray.toObject();
                qInfo()<<"min:"<<limit["min"].toDouble()
                      <<"max:"<<limit["max"].toDouble();

                mins.append(limit["min"].toDouble());
                maxs.append(limit["max"].toDouble());
            }
        }
    }
    myInputRobot->SetMaxPosition(maxs);
    myInputRobot->SetMinPosition(mins);

    // 6.TCP transformation
    if (jsonObject.contains("TCP")) {
        QJsonValue tcpValue = jsonObject.value("TCP");
        QJsonObject tcp = tcpValue.toObject();
        qInfo()<<"theta:"<<tcp["theta"].toDouble()
              <<"d:"<<tcp["d"].toDouble()
             <<"alpha:"<<tcp["alpha"].toDouble()
            <<"a:"<<tcp["a"].toDouble();

        dhArgs.append({tcp["theta"].toDouble()*M_PI/180.0,
                       tcp["d"].toDouble(),
                       tcp["alpha"].toDouble()*M_PI/180.0,
                       tcp["a"].toDouble()});
    }
    myInputRobot->SetDHData(dhArgs);

    // 7.
    QVector<double> postures;
    if (jsonObject.contains("Posture")) {
        QJsonValue arrayValue = jsonObject.value("Posture");
        if (arrayValue.isArray()) {
            QJsonArray array = arrayValue.toArray();
            for (int i = 0; i < array.size(); i++) {
                QJsonValue posture = array.at(i);
                qInfo()<<"joint"<<i<<"posture:"<<posture.toDouble();
                postures.append(posture.toDouble());
            }
        }
    } else {
        postures = myInitPosture;
    }
    myInputRobot->SetRPosture(postures);
    myInputRobot->SetNewPosture(postures);
}

// 从指定路径导入STEP文件，返回一个AIS_Shape对象
Handle(AIS_Shape) RobotCreator::importSTEP(const QString& str)
{
    STEPControl_Reader aReader;
    TCollection_AsciiString theAscii(str.toUtf8().data());
    aReader.ReadFile(theAscii.ToCString());
    aReader.TransferRoots();
    return new AIS_Shape(aReader.OneShape());
}

// 从指定路径导入IGES文件，返回一个AIS_Shape对象
Handle(AIS_Shape) RobotCreator::importIGES(const QString& str)
{
    IGESControl_Reader aReader;
    TCollection_AsciiString theAscii(str.toUtf8().data());
    aReader.ReadFile(theAscii.ToCString());
    aReader.TransferRoots();
    return new AIS_Shape(aReader.OneShape());
}
