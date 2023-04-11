/******************************
 *
 * @Author: 姓名：fanfj
 * @Description: 实现 CAD 软件中进阶图形设计功能，
 *      实现几何体布尔运算（并、交、差）、圆角、倒角、移动、旋转、缩 放、镜像、偏移等功能。
 *
 * ***************************/

#ifndef ADVANCEMODELING_H
#define ADVANCEMODELING_H

#include "ModelMaker.hpp"
#include <TopoDS.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>

class AdvanceModeling
{
public:
    /*****************************************************
     * static Handle(AIS_Shape) bool_operation(int boolType, TopoDS_Shape shape1, TopoDS_Shape shape2)
     * 输入：int boolType 布尔算法类别
     *      TopoDS_Shape shape1 拓扑体
     *      TopoDS_Shape shape2 拓扑体
     *
     * 对两个拓扑体进行布尔运算。
     * 判断布尔运算类型，根据不同类型选用Fuse、Common、Cut布尔算法。
     * ***************************************************/
    static Handle(AIS_Shape) bool_operation(int boolType, TopoDS_Shape shape1, TopoDS_Shape shape2) {
        // 存储结果
        TopoDS_Shape ans;

        // 判断布尔运算类型
        if(boolType == 0) {
            // 求 Fuse
            BRepAlgoAPI_Fuse fuse = BRepAlgoAPI_Fuse(shape1, shape2);
            ans = fuse.Shape();
        } else if(boolType == 1) {
            // 求 Common
            BRepAlgoAPI_Common common = BRepAlgoAPI_Common(shape1, shape2);
            ans = common.Shape();
        } else if(boolType == 2) {
            // 求 cut
            BRepAlgoAPI_Cut cut = BRepAlgoAPI_Cut(shape1, shape2);
            ans = cut.Shape();
        } else {
            return NULL;
        }

        // 可视化
        Handle(AIS_Shape) anAisBoolShape = new AIS_Shape(ans);
        return anAisBoolShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) chamfer(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 拓扑体
     *
     * 对拓扑体进行倒角。
     * 搜索拓扑体的第三条边，并对其进行倒角处理。
     * ***************************************************/
    static Handle(AIS_Shape) chamfer(TopoDS_Shape shape) {
        // 倒角处理
        BRepFilletAPI_MakeChamfer chamfer(shape);
        // 拓扑结构
        TopExp_Explorer anEdgeExplorer(shape, TopAbs_EDGE);
        // 搜索拓扑体的第三条边并进行倒角处理
        int i = 0;
        while(anEdgeExplorer.More()) {
            // 查找第三条边并进行处理
            if (i == 2) {
                TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
                // 进行倒角处理，设置 dis = 5
                Standard_Real dis = 5;
                chamfer.Add(dis, anEdge);
            }
            // 查找下一拓扑结构
            anEdgeExplorer.Next();
            i++;
        }
        // 可视化
        Handle(AIS_Shape) anAisChamferShape = new AIS_Shape(chamfer.Shape());
        return anAisChamferShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) fillet(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 拓扑体
     *
     * 对拓扑体进行圆角。
     * 搜索拓扑体的第三条边，并对其进行圆角处理。
     * ***************************************************/
    static Handle(AIS_Shape) fillet(TopoDS_Shape shape) {
        // 圆角处理
        BRepFilletAPI_MakeFillet fillet(shape);
        // 拓扑结构
        TopExp_Explorer anEdgeExplorer(shape, TopAbs_EDGE);
        // 搜索拓扑体的第三条边并进行圆角处理
        int i = 0;
        while(anEdgeExplorer.More()) {
            // 查找第三条边并进行处理
            if (i == 2) {
                TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
                // 进行圆角处理，设置 dis = 5
                Standard_Real dis = 5;
                fillet.Add(dis, anEdge);
            }
            // 查找下一拓扑结构
            anEdgeExplorer.Next();
            i++;
        }
        // 可视化
        Handle(AIS_Shape) anAisFilletShape = new AIS_Shape(fillet.Shape());
        return anAisFilletShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) translate(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 任意拓扑图形
     *
     * 对所选图形进行移动。
     * 取学号后三位作为x坐标创建向量。
     * 用该向量设置平移变换
     * ***************************************************/
    static Handle(AIS_Shape) translate(TopoDS_Shape shape) {
        // 创建向量，学号后三位 982
        gp_Dir dir = gp_Dir(982.0, 0.0, 0.0);
        gp_Vec vec(dir);
        // 平移变换 vec 单位
        gp_Trsf trsf;
        trsf.SetTranslation(vec);
        BRepBuilderAPI_Transform aTransformation =  BRepBuilderAPI_Transform(shape, trsf);

        // 可视化
        Handle(AIS_Shape) anAisTranslateShape = new AIS_Shape(aTransformation.Shape());
        return anAisTranslateShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) rotate(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 任意拓扑图形
     *
     * 对图形进行旋转。
     * 取学号末尾三位数字作为坐标实体从而创建方向D，例：假设学号以123结尾，则坐标实体为（1, 2, 3）。
     * 以全局坐标系原点和方向D创建一个坐标轴，作为旋转时的中心轴。
     * 用该中心轴和任意旋转角度设置旋转变换
     * ***************************************************/
    static Handle(AIS_Shape) rotate(TopoDS_Shape shape) {
        // 创建向量及中心轴，学号后三位 982
        gp_Dir dir = gp_Dir(9.0, 8.0, 2.0);
        gp_Ax1 ax1 = gp_Ax1(gp_Pnt(0, 0, 0), dir);
        // 旋转角度为 10
        Standard_Real theAng = 0.3;
        // 旋转变换
        gp_Trsf trsf;
        trsf.SetRotation(ax1, theAng);
        BRepBuilderAPI_Transform aTransformation =  BRepBuilderAPI_Transform(shape, trsf);

        // 可视化
        Handle(AIS_Shape) anAisRotateShape = new AIS_Shape(aTransformation.Shape());
        return anAisRotateShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) scale(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 任意拓扑图形
     *
     * 对图形进行缩放。
     * 取学号末尾三位数字为坐标创建中心点，例：假设学号以123结尾，则点坐标为（3, 2, 1）。
     * 以该中心点对图形进行缩放，缩放比例用学号末尾三位数字计算，例如：假设学号以abc结尾，则点坐标为(a+b+c)/10.
     * ***************************************************/
    static Handle(AIS_Shape) scale(TopoDS_Shape shape) {
        // 创建中心点，学号后三位 982
        gp_Pnt center(2, 8, 9);
        // 缩放比例为（9 + 8 + 2）/ 10
        Standard_Real theS = 1.9;
        // 旋转变换
        gp_Trsf trsf;
        trsf.SetScale(center, theS);
        BRepBuilderAPI_Transform aTransformation =  BRepBuilderAPI_Transform(shape, trsf);

        // 可视化
        Handle(AIS_Shape) anAisScaleShape = new AIS_Shape(aTransformation.Shape());
        return anAisScaleShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) mirror(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 任意拓扑图形
     *
     * 取学号末尾三位数字作为坐标实体从而创建方向D，例：假设学号以123结尾，则坐标实体为（2, 1, 3）。
     * 以全局坐标系原点和方向D创建一个坐标轴，作为镜像的对称轴。
     * ***************************************************/
    static Handle(AIS_Shape) mirror(TopoDS_Shape shape) {
        // 创建方向和对称轴，学号后三位 982
        gp_Dir dir(8, 9, 2);
        gp_Ax1 ax1(gp_Pnt(0, 0, 0), dir);
        // 镜像变换
        gp_Trsf trsf;
        trsf.SetMirror(ax1);
        BRepBuilderAPI_Transform aTransformation =  BRepBuilderAPI_Transform(shape, trsf);

        // 可视化
        Handle(AIS_Shape) anAisMirrorShape = new AIS_Shape(aTransformation.Shape());
        return anAisMirrorShape;
    }

    /*****************************************************
     * static Handle(AIS_Shape) offset(TopoDS_Shape shape)
     * 输入：TopoDS_Shape shape 任意拓扑图形
     *
     * 以学号最后两位数字作为偏置量，对图像进行偏置。例：假设学号以23结尾，则偏置量为23。
     * ***************************************************/
    static Handle(AIS_Shape) offset(TopoDS_Shape shape) {
        // 偏置量，学号后两位为 82
        Standard_Real aOffset = 82;
        // 进行偏置
        BRepOffsetAPI_MakeOffsetShape aMakeOffsetShape;
        aMakeOffsetShape.PerformBySimple(shape, aOffset);
        // 可视化
        TopoDS_Shape topoDS_offset = aMakeOffsetShape.Shape();
        Handle(AIS_Shape) anAisOffsetShape = new AIS_Shape(topoDS_offset);
        return anAisOffsetShape;
    }
};

#endif // ADVANCEMODELING_H
