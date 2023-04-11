#include "DrawCommand.h"
#include "ModelMaker.hpp"

DrawCommand::DrawCommand(OccWidget* device, QObject *parent)
    : QObject{parent}
    , _device(device)
{
    /* Your code here */
    // 获取开始画图信号
    connect(_device, &OccWidget::pickPixel, this, &DrawCommand::onDeviceClicked);
    // 获取鼠标移动信号
    connect(_device, &OccWidget::moveToPixel, this, &DrawCommand::onDeviceMoved);
    // 获取结束画图信号
    connect(_device, &OccWidget::endPixel, this, &DrawCommand::onDeviceEnd);
    // 获取取消画图信号
    connect(_device, &OccWidget::endDraw, this, &DrawCommand::onDeviceCancel);
}

/*** 开始点 ***/
void DrawPointCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    // 将鼠标坐标转换成画布坐标系下坐标点
    _p = _device->ConvertClickToPoint(px, py);
    // 生成可交互形状
    Handle(AIS_Shape) p = ModelMaker::make_point(_p);

    // 显示图形
    _device->GetContext()->Display(p, true);

    // 删除画图事件
    //this->deleteLater();
}

void DrawPointCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    this->deleteLater();
}

void DrawPointCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束点 ***/

/*** 开始直线 ***/
void DrawLineCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        // this->deleteLater();
    }
    else {
        _p1 = p;
        _p1_init = true;

        gp_Pnt p2 = gp_Pnt(_p1.X()+0.01, _p1.Y()+0.01, _p1.Z()+0.01);
        _line = ModelMaker::make_line(_p1, p2);
        _device->GetContext()->Display(_line, true);
    }

}

void DrawLineCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_line, true);
    _line = ModelMaker::make_line(_p1, p);
    _device->GetContext()->Display(_line, true);
}

void DrawLineCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_line, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawLineCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束直线 ***/

/*** 开始圆 ***/
void DrawCircCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _circ = ModelMaker::make_circle(_p1, 0.1);
        _device->GetContext()->Display(_circ, true);
    }
}

void DrawCircCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_circ, true);
    _circ = ModelMaker::make_circle(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_circ, true);
}

void DrawCircCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_circ, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawCircCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束圆 ***/

/*** 开始椭圆 ***/
void DrawElipsCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _elips = ModelMaker::make_elips(_p1, 0.1);
        _device->GetContext()->Display(_elips, true);
    }
}

void DrawElipsCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_elips, true);
    _elips = ModelMaker::make_elips(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_elips, true);
}

void DrawElipsCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_elips, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawElipsCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束椭圆 ***/

/*** 开始B样条 ***/
void DrawSplineCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    if(_p_init) {
        _device->GetContext()->Remove(_ps, true);
    }
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_points.size() > 0 && p.XYZ().IsEqual(_points.last().XYZ(), 0.01)) {
        return;
    }
    _ps = ModelMaker::make_point(p);
    _device->GetContext()->Display(_ps, true);

    _p_init = true;
    _points.push_back(p);
    if(_points.size() == 1)
        return;

    _device->GetContext()->Remove(_spline, true);
    _spline = ModelMaker::make_BSpline(convertPoints(_points));
    _device->GetContext()->Display(_spline, true);
}

void DrawSplineCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    QVector<gp_Pnt> tmp = _points;
    tmp.append(p);
    _device->GetContext()->Remove(_spline, true);
    _spline = ModelMaker::make_BSpline(convertPoints(tmp));;
    _device->GetContext()->Display(_spline, true);
}

void DrawSplineCommand::dealWithEnd(int px, int py)
{
    /* Your code here */
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p_init) {
        _device->GetContext()->Remove(_spline, true);
        if(_points.size() > 1) {
            _spline_final = ModelMaker::make_BSpline(convertPoints(_points));
            _device->GetContext()->Display(_spline_final, true);
        }
        _device->GetContext()->Remove(_ps, true);
        _p_init = false;
        _points.erase(_points.begin(), _points.end());
    } else {
        this->deleteLater();
    }
}

void DrawSplineCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束B样条 ***/

/*** 开始长方体 ***/
void DrawBoxCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        // this->deleteLater();
    }
    else {
        _p1 = p;
        _p1_init = true;

        gp_Pnt p2 = gp_Pnt(_p1.X()+0.01, _p1.Y()+0.01, _p1.Z()+0.01);
        _box = ModelMaker::make_box(_p1, p2);
        _device->GetContext()->Display(_box, true);
    }

}

void DrawBoxCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    p = gp_Pnt(p.X()+0.01, p.Y()+0.01, p.Z()+0.01);
    _device->GetContext()->Remove(_box, true);
    _box = ModelMaker::make_box(_p1, p);
    _device->GetContext()->Display(_box, true);
}

void DrawBoxCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_box, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawBoxCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束长方体 ***/

/*** 开始圆柱体 ***/
void DrawCylinderCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _cylinder = ModelMaker::make_cylinder(_p1, 0.1);
        _device->GetContext()->Display(_cylinder, true);
    }
}

void DrawCylinderCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_cylinder, true);
    _cylinder = ModelMaker::make_cylinder(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_cylinder, true);
}

void DrawCylinderCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_cylinder, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawCylinderCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束圆柱体 ***/

/*** 开始圆锥体 ***/
void DrawConeCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _cone = ModelMaker::make_cone(_p1, 0.1);
        _device->GetContext()->Display(_cone, true);
    }
}

void DrawConeCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_cone, true);
    _cone = ModelMaker::make_cone(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_cone, true);
}

void DrawConeCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_cone, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawConeCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束圆锥体 ***/

/*** 开始球体 ***/
void DrawSphereCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _sphere = ModelMaker::make_sphere(_p1, 0.1);
        _device->GetContext()->Display(_sphere, true);
    }
}

void DrawSphereCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_sphere, true);
    _sphere = ModelMaker::make_sphere(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_sphere, true);
}

void DrawSphereCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_sphere, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawSphereCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束球体 ***/

/*** 开始圆环体 ***/
void DrawTorusCommand::dealWithPick(int px, int py)
{
    /* Your code here */
    gp_Pnt p = _device->ConvertClickToPoint(px, py);

    if(_p1_init) {
        _p2 = p;
        _p1_init = false;
        _device->GetContext()->Remove(_center, true);
        //this->deleteLater();
    }
    else {
        _center = ModelMaker::make_point(p);
        _device->GetContext()->Display(_center, true);
        _p1 = p;
        _p1_init = true;
        _torus = ModelMaker::make_torus(_p1, 0.1);
        _device->GetContext()->Display(_torus, true);
    }
}

void DrawTorusCommand::dealWithPreview(int px, int py)
{
    /* Your code here */
    if(!_p1_init)
        return;

    gp_Pnt p = _device->ConvertClickToPoint(px, py);
    _device->GetContext()->Remove(_torus, true);
    _torus = ModelMaker::make_torus(_p1, p.Distance(_p1));
    _device->GetContext()->Display(_torus, true);
}

void DrawTorusCommand::dealWithEnd(int px, int py)
{
    Q_UNUSED(px)
    Q_UNUSED(py)
    if(_p1_init) {
        _device->GetContext()->Remove(_torus, true);
        _device->GetContext()->Remove(_center, true);
        _p1_init = false;
    } else {
        this->deleteLater();
    }
}

void DrawTorusCommand::dealWithCancel()
{
    this->deleteLater();
}
/*** 结束圆环体 ***/

