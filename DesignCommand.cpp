#include "DesignCommand.h"
#include "BasicModeling.hpp"


DesignCommand::DesignCommand(OccWidget *device, QObject *parent)
    : QObject{parent}
    , _device(device)
{
    /* Your code here */
    // 选中形状时，通过此获取选中的形状信息
    connect(_device, &OccWidget::selectShapeChanged, this, &DesignCommand::onDeviceItemSelected);
}

void DispersionCommand::dealWithSelection()
{
    /* Your code here */
    // 获取AIS可交互环境
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    // 初始化已选形状列表
    QVector<TopoDS_Shape> selected;
    // 获取已选择形状列表
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    // 已选择形状不唯一
    if(selected.size() != 1)
        return;

    // 获得已选形状
    TopoDS_Shape shape = selected[0];

    QVector <gp_Pnt> points = BasicModeling::disperse(shape);
    for(int i = 0; i < points.size(); i++) {
        Handle(AIS_Shape) aisp = ModelMaker::make_point(points[i]);
        context->Display(aisp, true);
    }
    QString info = QString("已将曲线离散成%1个点\n").arg(points.size());
    emit setDesignInfo(info);

    this->deleteLater();
}

void ExtremCommand::dealWithSelection()
{
    /* Your code here */
    // 获取AIS可交互环境
    Handle(AIS_InteractiveContext) context = _device->GetContext();
    // 初始化已选形状列表
    QVector<TopoDS_Shape> selected;
    // 获取已选择形状列表
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected.append(context->SelectedShape());
    }

    // 已选择形状不唯二
    if(selected.size() != 2)
        return;

    // 获得已选形状
    TopoDS_Shape shape1 = selected[0];
    TopoDS_Shape shape2 = selected[1];

    QVector <gp_Pnt> points = BasicModeling::get_extrema(shape1,shape2);

    if(points.size() == 0) {
        QString info = QString("无法找到极值点，请重新选择图形\n");
        emit setDesignInfo(info);
        return;
    }

    Handle(AIS_Shape) maxlin = ModelMaker::make_line(points[0], points[1]);
    maxlin->SetColor(Quantity_NOC_YELLOW);
    context->Display(maxlin, true);
    QString info = QString("最大距离：%1\n起点：(%2, %3, %4)，终点：(%5, %6, %7)\n").arg(points[1].Distance(points[0])).arg(points[0].X()).arg(points[0].Y()).arg(points[0].Z()).arg(points[1].X()).arg(points[1].Y()).arg(points[1].Z());

    Handle(AIS_Shape) minlin = ModelMaker::make_line(points[2], points[3]);
    minlin->SetColor(Quantity_NOC_GREEN);
    context->Display(minlin, true);
    info += QString("最小距离：%1\n起点：(%2, %3, %4)，终点：(%5, %6, %7)\n").arg(points[3].Distance(points[2])).arg(points[2].X()).arg(points[2].Y()).arg(points[2].Z()).arg(points[3].X()).arg(points[3].Y()).arg(points[3].Z());
    emit setDesignInfo(info);
    this->deleteLater();
}

void Project2CurveCommand::dealWithSelection()
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

    QVector <gp_Pnt> points = BasicModeling::project2_curve(shape1, shape2);

    if(points.size() == 0) {
        QString info = QString("无法投影，请重新选择图形\n");
        emit setDesignInfo(info);
        return;
    }

    Handle(AIS_Shape) point = ModelMaker::make_point(points[1]);
    context->Display(point, true);
    Handle(AIS_Shape) lin = ModelMaker::make_line(points[0], points[1]);
    context->Display(lin, true);
    lin->SetColor(Quantity_NOC_YELLOW);

    QString info = QString("点：(%1, %2，%3)\n投影点：(%4, %5, %6)\n投影距离：%7\n").arg(points[0].X()).arg(points[0].Y()).arg(points[0].Z()).arg(points[1].X()).arg(points[1].Y()).arg(points[1].Y()).arg(points[1].Distance(points[0]));
    emit setDesignInfo(info);

    this->deleteLater();
}

void NotationCommand::dealWithSelection()
{

}
