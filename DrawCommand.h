#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "OccWidget.h"
#include "MainWindow.h"
#include <AIS_Shape.hxx>

class DrawCommand : public QObject
{
    Q_OBJECT
public:
    DrawCommand(OccWidget* device, QObject *parent = nullptr);

protected:
    // 画图
    virtual void dealWithPick(int px, int py) = 0;
    // 预览
    virtual void dealWithPreview(int px, int py) = 0;
    // 结束画图
    virtual void dealWithEnd(int px, int py) = 0;
    // 取消画图
    virtual void dealWithCancel() = 0;

// 接收信号
private slots:
    void onDeviceClicked(int px, int py) {
        dealWithPick(px, py);
    }

    void onDeviceMoved(int px, int py) {
        dealWithPreview(px, py);
    }

    void onDeviceEnd(int px, int py) {
        dealWithEnd(px, py);
    }

    void onDeviceCancel() {
        dealWithCancel();
    }

protected:
    OccWidget* _device;

};

class DrawPointCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawPointCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py) {
        Q_UNUSED(px)
        Q_UNUSED(py)
        // do nothing when drawing point
    }
    virtual void dealWithEnd(int px, int py);

private:
    gp_Pnt _p;
};

class DrawLineCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawLineCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _line;
};

class DrawCircCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawCircCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _circ;
};

class DrawElipsCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawElipsCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _elips;
};

class DrawSplineCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawSplineCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

    Handle(TColgp_HArray1OfPnt) convertPoints(const QVector<gp_Pnt>& points) const {
        Handle(TColgp_HArray1OfPnt) result = new TColgp_HArray1OfPnt(1,points.size());
        for(int i=1; i<=points.size(); ++i) {
            result->SetValue(i, points[i-1]);
        }
        return result;
    }

private:
    bool _p_init;
    QVector<gp_Pnt> _points;
    Handle(AIS_Shape) _ps;
    Handle(AIS_Shape) _spline;
    Handle(AIS_Shape) _spline_final;
};

class DrawBoxCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawBoxCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _box;
};

class DrawCylinderCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawCylinderCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _cylinder;
};

class DrawConeCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawConeCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _cone;
};

class DrawSphereCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawSphereCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _sphere;
};

class DrawTorusCommand : public DrawCommand
{
    Q_OBJECT
public:
    DrawTorusCommand(OccWidget* device, QObject *parent = nullptr)
        : DrawCommand(device, parent)
        , _p1_init(false) {}

protected:
    virtual void dealWithCancel();
    virtual void dealWithPick(int px, int py);
    virtual void dealWithPreview(int px, int py);
    virtual void dealWithEnd(int px, int py);

private:
    bool _p1_init;
    gp_Pnt _p1;
    gp_Pnt _p2;
    Handle(AIS_Shape) _center;
    Handle(AIS_Shape) _torus;
};

#endif // DRAWCOMMAND_H
