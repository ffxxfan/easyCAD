#include "DesignCommand3D.h"
#include "BasicModeling.hpp"
#include "AdvanceModeling.hpp"

DesignCommand3D::DesignCommand3D(OccWidget *device, QObject *parent)
    : QObject{parent}
    , _device(device)
{
    connect(_device, &OccWidget::selectShapeChanged, this, &DesignCommand3D::onDeviceItemSelected);
}

void Project2FaceCommand::dealWithSelection()
{
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 2)
        return;

    TopoDS_Shape shape1 = selected[0];
    TopoDS_Shape shape2 = selected[1];

    QVector <gp_Pnt> points = BasicModeling::project2_face(shape1, shape2);

    if(points.size() == 0) {
        QString info = QString("无法投影，请重新选择图形\n");
        emit setDesignInfo(info);
        return;
    }

    Handle(AIS_Shape) lin = ModelMaker::make_line(points[0], points[1]);
    context->Display(lin, true);
    lin->SetColor(Quantity_NOC_YELLOW);

    QString info = QString("点：(%1, %2，%3)\n投影点：(%4, %5, %6)\n投影距离：%7\n").arg(points[0].X()).arg(points[0].Y()).arg(points[0].Z()).arg(points[1].X()).arg(points[1].Y()).arg(points[1].Y()).arg(points[1].Distance(points[0]));
    emit setDesignInfo(info);

    this->deleteLater();
}

void TransformCommand::dealWithSelection()
{
    /* Your code here */
    _device->GetContext()->InitSelected();
    if(_device->GetContext()->HasSelectedShape()) {
        Handle(AIS_InteractiveObject) Current = _device->GetContext()->SelectedInteractive();
        _device->GetManipulator()->Attach(Current);
        _device->GetView()->Update();
    } else {
        _device->GetManipulator()->Detach();
        _device->GetView()->Update();
    }
}

void BoolDesignCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 2)
        return;

    TopoDS_Shape shape1 = selected[0];
    TopoDS_Shape shape2 = selected[1];

    Handle(AIS_Shape) Ais_BoolShape = AdvanceModeling::bool_operation(boolType, shape1, shape2);

    gp_Trsf trsf;
    trsf.SetTranslation({200,0,0});
    Ais_BoolShape->SetLocalTransformation(trsf);//平移变换
    context->Display(Ais_BoolShape, true);

    this->deleteLater();
}

void ChamferCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_ChamferShape = AdvanceModeling::chamfer(shape);
    context->Display(Ais_ChamferShape, true);

    this->deleteLater();
}

void FilletCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_FilletShape = AdvanceModeling::fillet(shape);
    context->Display(Ais_FilletShape, true);

    this->deleteLater();
}

void TranslateCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_TranslateShape = AdvanceModeling::translate(shape);
    context->Display(Ais_TranslateShape, true);

    this->deleteLater();
}

void RotateCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_TranslateShape = AdvanceModeling::rotate(shape);
    context->Display(Ais_TranslateShape, true);

    this->deleteLater();
}

void ScaleCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_ScaleShape = AdvanceModeling::scale(shape);
    context->Display(Ais_ScaleShape, true);

    this->deleteLater();
}

void MirrorCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_MirrorShape = AdvanceModeling::mirror(shape);
    context->Display(Ais_MirrorShape, true);

    this->deleteLater();
}

void OffsetCommand::dealWithSelection()
{
    /* Your code here */
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    QVector<TopoDS_Shape> selected;
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    if(selected.size() != 1)
        return;

    TopoDS_Shape shape = selected[0];
    Handle(AIS_Shape) Ais_OffsetShape = AdvanceModeling::offset(shape);
    context->Display(Ais_OffsetShape, true);

    this->deleteLater();
}
