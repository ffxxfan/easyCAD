#ifndef DESIGNCOMMAND3D_H
#define DESIGNCOMMAND3D_H

#include "OccWidget.h"

class DesignCommand3D : public QObject
{
    Q_OBJECT
public:
    DesignCommand3D(OccWidget* device, QObject *parent = nullptr);

protected:
    virtual void dealWithSelection() = 0;

private slots:
    void onDeviceItemSelected() {
        dealWithSelection();
    }

protected:
    OccWidget* _device;

signals:
    void setDesignInfo(QString info);
};

class BoolDesignCommand: public DesignCommand3D
{
    Q_OBJECT
public:
    BoolDesignCommand(int type, OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent)
        , boolType(type) {}

protected:
    virtual void dealWithSelection();
    const int boolType = -1;
};

class Project2FaceCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    Project2FaceCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();

};

class TransformCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    TransformCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class ChamferCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    ChamferCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class FilletCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    FilletCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class TranslateCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    TranslateCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class RotateCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    RotateCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class ScaleCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    ScaleCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class MirrorCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    MirrorCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

class OffsetCommand : public DesignCommand3D
{
    Q_OBJECT
public:
    OffsetCommand(OccWidget* device, QObject *parent = nullptr)
        : DesignCommand3D(device, parent) {}

protected:
    virtual void dealWithSelection();
};

#endif // DESIGNCOMMAND_H
