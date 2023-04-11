#ifndef DESIGNCOMMAND_H
#define DESIGNCOMMAND_H

#include "OccWidget.h"
#include <TopoDS_Shape.hxx>
#include "TopoDS_Edge.hxx"
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>

class DesignCommand : public QObject
{
    Q_OBJECT
public:
    DesignCommand(OccWidget* device, QObject *parent = nullptr);

protected:
    virtual void dealWithSelection() = 0;

// 接收信号
private slots:
    void onDeviceItemSelected() {
        dealWithSelection();
    }

protected:
    OccWidget* _device;

signals:
    void setDesignInfo(QString info);
};
// 离散
class DispersionCommand : public DesignCommand
{
    Q_OBJECT
public:
    DispersionCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand(device, parent) {}

protected:
    virtual void dealWithSelection();

};
// 线之间的极值
class ExtremCommand : public DesignCommand
{
    Q_OBJECT
public:
    ExtremCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand(device, parent) {}

protected:
    virtual void dealWithSelection();

};
// 点投影到线
class Project2CurveCommand : public DesignCommand
{
    Q_OBJECT
public:
    Project2CurveCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand(device, parent) {}

protected:
    virtual void dealWithSelection();

};
// 添加半径标注
class NotationCommand : public DesignCommand
{
    Q_OBJECT
public:
    NotationCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand(device, parent) {}

protected:
    virtual void dealWithSelection();

};

#endif // DESIGNCOMMAND_H
