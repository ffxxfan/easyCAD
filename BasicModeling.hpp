/******************************
 *
 * @Author: 姓名：fanfj
 * @Description: 实现 CAD 软件中基础图形设计功能，
 *      实现离散化边、求两条边之间的极值点、求点到线/面的投影、切换 选择模式。
 *
 * ***************************/

#ifndef BASICMODELING_H
#define BASICMODELING_H

#include "ModelMaker.hpp"
#include <TopoDS_Shape.hxx>
#include <QVector>
#include <AIS_InteractiveContext.hxx>

#include <TopoDS.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>

class BasicModeling
{
public:
    /*****************************************************x
     * QVector <gp_Pnt> disperse(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 由曲线构成的拓扑边
     * 输出：QVector <gp_Pnt> 一组非参数化点
     *
     * 对曲线构成的拓扑边进行离散化。
     * 判断拓扑形状是否为边，并在拓扑边上均匀选取A个点，A为学号末两位数。
     * 通过参数化曲线获取边参数取值范围。根据所需取点个数A计算步长。
     * 依照步长依次获取曲线上的点并放入数组中。
     * ***************************************************/
    static QVector <gp_Pnt> disperse(TopoDS_Shape shape) {
        QVector <gp_Pnt> points;
        // 判断是否为边
        if(shape.ShapeType() == TopAbs_EDGE) {
            // 强制转换为 TopoDS_Edge
            TopoDS_Edge edge = TopoDS::Edge(shape);
            // 起始点
            Standard_Real first, last;
            // 离散化
            Handle(Geom_Curve) curve = BRep_Tool::Curve	(edge, first, last);
            // 学号后两位为 82，故划分为 81 段
            double step = (last - first) / 81;
            for (double v = first; v <= last; v += step) {
                // 记录点并添加进入 poins
                gp_Pnt p = curve->Value(v);
                points.append(p);
            }
        }
        return points;
    }

    /*****************************************************x
     * QVector <gp_Pnt> get_extrema(TopoDS_Shape shape1, TopoDS_Shape shape2)
     * 输入：TopoDS_Shape shape1 由曲线构成的拓扑边
     *      TopoDS_Shape shape2 由曲线构成的拓扑边
     * 输出：QVector <gp_Pnt> 一组非参数化点
     *
     * 计算两条拓扑边的最大极值点和最小极值点。
     * 判断两个拓扑形状的类型是否为边，并通过求极值算法计算所有极值点。
     * 从所有极值点找出最大极值点和最小极值点，分别放入数组中。
     * 数组0和1位存放最大极值点，数组2和3位放最小极值点。
     * ***************************************************/
    static QVector <gp_Pnt> get_extrema(TopoDS_Shape shape1, TopoDS_Shape shape2) {
        QVector <gp_Pnt> points;
        // 判断是否为拓扑边
        if(shape1.ShapeType() == TopAbs_EDGE && shape2.ShapeType() == TopAbs_EDGE) {
            // 强制转换为 TopoDS_EDGE
            TopoDS_Edge edge1 = TopoDS::Edge(shape1);
            TopoDS_Edge edge2 = TopoDS::Edge(shape2);
            // 计算最大极值点和最小极值点
            gp_Pnt max1;
            gp_Pnt max2;
            gp_Pnt min1;
            gp_Pnt min2;
            // 存储最小极值和最大极值
            Standard_Real min, max;
            // 存储最小极值和最大极值索引
            Standard_Integer minIndex, maxIndex;
            // 存储边的起点和终点
            Standard_Real first1, last1, first2, last2;
            Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, first1, last1);
            Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, first2, last2);

            // 计算曲线 curve1 和 curve2 之间的极值
            GeomAPI_ExtremaCurveCurve aExtrema(curve1, curve2);
            // 从所有极值点找出最大极值点和最小极值点
            min = aExtrema.Distance(1);
            max = min;
            minIndex = 1;
            maxIndex = 1;
            Standard_Integer x = aExtrema.NbExtrema();
            for(Standard_Integer i = 1; i <= aExtrema.NbExtrema(); i += 1) {
                if(aExtrema.Distance(i) > max) {
                    maxIndex = i;
                    max = aExtrema.Distance(i);
                } else if(aExtrema.Distance(i) < min) {
                    minIndex = i;
                    min = aExtrema.Distance(i);
                }
            }

            // 计算最大极值点和最小极值点
            aExtrema.Points(minIndex, max1, max2);
            aExtrema.Points(maxIndex, min1, min2);

            // 将其最大极值点和最小极值点放入 points 中
            points.append(max1);
            points.append(max2);
            points.append(min1);
            points.append(min2);
        }
        return points;
    }

    /*****************************************************x
     * QVector <gp_Pnt> project2_curve(TopoDS_Shape shape1, TopoDS_Shape shape2)
     * 输入：TopoDS_Shape shape1 拓扑点/边
     *      TopoDS_Shape shape2 拓扑边/点
     * 输出：QVector <gp_Pnt> 一组非参数化
     *
     * 计算拓扑点到拓扑边的投影。
     * 注意：shape1和shape2不一定是拓扑点和拓扑边，需要进行判断。
     * ***************************************************/
    static QVector <gp_Pnt> project2_curve(TopoDS_Shape shape1, TopoDS_Shape shape2) {
        // 用于存放投影点
        QVector <gp_Pnt> points;
        // 定义点和边，用于强制转换
        TopoDS_Vertex vertex;
        TopoDS_Edge edge;
        gp_Pnt point;
        Handle(Geom_Curve) curve;
        // 定义起始点和终点，用于强制转化
        Standard_Real first, last;

        // 判断是否为一个点和一条边，是则强制转换类型，不是则返回 null
        if(shape1.ShapeType() == TopAbs_EDGE && shape2.ShapeType() == TopAbs_VERTEX) {
            vertex = TopoDS::Vertex(shape2);
            edge = TopoDS::Edge(shape1);
        } else if(shape1.ShapeType() == TopAbs_VERTEX && shape2.ShapeType() == TopAbs_EDGE) {
            vertex = TopoDS::Vertex(shape1);
            edge = TopoDS::Edge(shape2);
        } else {
            // 不符合条件
            return points;
        }

        // 强制转化
        point = BRep_Tool::Pnt(vertex);
        curve = BRep_Tool::Curve(edge, first, last);

        // 添加投影顶点
        points.append(point);

        // 计算点到线的投影并将其加入到 points 中
        GeomAPI_ProjectPointOnCurve project = GeomAPI_ProjectPointOnCurve(point, curve);
        for (Standard_Integer i = 1; i <= project.NbPoints(); i += 1) {
            gp_Pnt point_i = project.Point(i);
            points.append(point_i);
        }

        return points;
    }

    /*****************************************************x
     * QVector <gp_Pnt> project2_face(TopoDS_Shape shape1, TopoDS_Shape shape2)
     * 输入：TopoDS_Shape shape1 拓扑点/面
     *      TopoDS_Shape shape2 拓扑面/点
     * 输出：QVector <gp_Pnt> 一组非参数化
     *
     * 计算拓扑点到拓扑面的投影。
     * 注意：shape1和shape2不一定是拓扑点和拓扑面，需要进行判断。
     * ***************************************************/
    static QVector <gp_Pnt> project2_face(TopoDS_Shape shape1, TopoDS_Shape shape2) {
        // 用于存放投影点
        QVector <gp_Pnt> points;
        // 定义点和边，用于强制转换
        TopoDS_Vertex vertex;
        TopoDS_Face face;
        gp_Pnt point;
        Handle(Geom_Surface) surf;
        // 定义起始点和终点，用于强制转化
        Standard_Real first, last;

        // 判断是否为一个点和一条边，是则强制转换类型，不是则返回 null
        if(shape1.ShapeType() == TopAbs_FACE && shape2.ShapeType() == TopAbs_VERTEX) {
            vertex = TopoDS::Vertex(shape2);
            face = TopoDS::Face(shape1);
        } else if(shape1.ShapeType() == TopAbs_VERTEX && shape2.ShapeType() == TopAbs_FACE) {
            vertex = TopoDS::Vertex(shape1);
            face = TopoDS::Face(shape2);
        } else {
            // 不符合条件
            return points;
        }

        // 强制转化
        point = BRep_Tool::Pnt(vertex);
        surf = BRep_Tool::Surface(face);

        // 添加投影顶点
        points.append(point);

        // 计算点到面的投影并将其加入到 points 中
        GeomAPI_ProjectPointOnSurf project = GeomAPI_ProjectPointOnSurf(point, surf, Extrema_ExtAlgo_Grad);
        for (Standard_Integer i = 1; i <= project.NbPoints(); i += 1) {
            gp_Pnt point_i = project.Point(i);
            points.append(point_i);
        }

        return points;
    }

    /*****************************************************x
     * void selection_activate(int selectionType, Handle(AIS_InteractiveContext) context)
     * 输入：int selectionType 模型选择类别
     *      Handle(AIS_InteractiveContext) context 一个AIS_InteractiveContext
     *
     * 针对所有可见模型切换选择模式。
     * 禁用所有已激活选择模式，根据输入的模型选择类别激活选择模式。
     * 模型选择类别数值参考MainWindow.cpp中重置选择、选择点、选择边、选择面、选择实体按钮代码。
     * ***************************************************/
    static void selection_activate(int selectionType, Handle(AIS_InteractiveContext) context) {
        // 只选择一个实体按钮
        context->Deactivate();
        // 激活
        context->Activate(selectionType, Standard_True);
    }
};

#endif // BASICMODELING_H
