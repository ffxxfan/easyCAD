/******************************
 *
 * @Author: 姓名：fanfj
 * @Description: 实现 CAD 软件中基础图形绘图功能，
 *      包括点、线段、圆、椭圆、B 样条曲线、长方体、圆柱体、圆锥体、 球体和圆环体等基础图形。
 *
 * ***************************/

#ifndef MODELMAKER_H
#define MODELMAKER_H

#include <AIS_Shape.hxx>
#include <Quantity_Color.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

class ModelMaker
{
public:
    /*****************************************************x
     * static Handle(AIS_Shape) make_point(gp_Pnt point)
     * 输入：非参数化点 point
     * 输出：一个可交互顶点，类型为 AIS_Shape
     * ***************************************************/
    static Handle(AIS_Shape) make_point(gp_Pnt point) {
        // 绘制点
        TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(point);
        // 可视化
        Handle(AIS_Shape) anAisPoint = new AIS_Shape(vertex);
        return anAisPoint;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_line(gp_Pnt Pnt1, gp_Pnt Pnt2)
     * 输入：端点 Pnt1
     *      端点 Pnt2
     * 输出：一个可交互线段，类型为 AIS_Shape
     *
     * 1.取学号倒数第三位数字生成圆颜色：假设学号倒数第三位数字为3，则颜色为
     *   Quantity_Color(0.3, 0.3, 0.3, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_line(gp_Pnt Pnt1, gp_Pnt Pnt2) {
        // 构建边
        TopoDS_Edge line = BRepBuilderAPI_MakeEdge(Pnt1, Pnt2);
        // 构建可交互图形
        Handle(AIS_Shape) anAisLine = new AIS_Shape(line);
        // 学号倒数第三位数字为 0.9，设置颜色为 Quantity_Color(0.9, 0.9, 0.9, Quantity_TOC_RGB)
        anAisLine->SetColor(Quantity_Color(0.9, 0.9, 0.9, Quantity_TOC_RGB));
        return anAisLine;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_circle(gp_Pnt p, Standard_Real r)
     * 输入：圆心点 p
     *      圆半径 r
     * 输出：一个可交互圆，类型为 AIS_Shape
     *
     * 1.取学号末尾三位数字作为坐标实体从而创建方向D，以点p和方向D创建坐标系，
     *   在此坐标系中创建圆，例：假设学号以123结尾，则坐标为（2, 1, 3）
     * 2.取学号倒数第二位数字生成圆颜色：假设学号倒数第二位数字为2，则颜色为
     *   Quantity_Color(0.2, 0.2, 0.2, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_circle(gp_Pnt p, Standard_Real r) {
        // 创建坐标，学号后三位 982
        gp_Dir dir = gp_Dir(8.0, 9.0, 2.0);
        // 创建坐标系
        gp_Ax2 ax2 = gp_Ax2(p, dir);
        // 创建圆
        gp_Circ circle(ax2, r);
        // 构建圆
        TopoDS_Edge topoDS_circle = BRepBuilderAPI_MakeEdge(circle);
        // 构建面
        BRepBuilderAPI_MakeWire aWire  = BRepBuilderAPI_MakeWire();
        aWire.Add(topoDS_circle);
        TopoDS_Face myFace_circle = BRepBuilderAPI_MakeFace(aWire.Wire());
        // 构建可交互图形
        Handle(AIS_Shape) anAisCircle = new AIS_Shape(myFace_circle);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.8, 0.8, 0.8, Quantity_TOC_RGB)
        anAisCircle->SetColor(Quantity_Color(0.8, 0.8, 0.8, Quantity_TOC_RGB));
        return anAisCircle;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_elips(gp_Pnt p, Standard_Real r1)
     * 输入：椭圆中心点 p
     *      椭圆长半径 r1
     * 输出：一个可交互椭圆，类型为 AIS_Shape
     *
     * 1.取学号末位数字a计算椭圆短体半径 r2 = r1 / (a / 5 + 1.5)
     * 2.取学号末尾三位数字作为坐标实体从而创建方向D，以点p和方向D创建坐标系，
     *   在此坐标系中创建椭圆，例：假设学号以123结尾，则坐标为（2, 3, 1）
     * 3.取学号末尾位数字生成椭圆颜色：假设学号末尾位数字为3，则颜色为
     *   Quantity_Color(0.3, 0.3, 0.3, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_elips(gp_Pnt p, Standard_Real r1) {
        // 创建坐标，学号后三位 982
        gp_Dir dir = gp_Dir(8.0, 2.0, 9.0);
        // 创建坐标系
        gp_Ax2 ax2 = gp_Ax2(p, dir);
        // 椭圆短体半径（学号2929392982）
        Standard_Real r2 = r1 / (2 / 5 + 1.5);
        // 构建椭圆
        gp_Elips elips = gp_Elips(ax2, r1, r2);
        TopoDS_Edge topoDS_elips = BRepBuilderAPI_MakeEdge(elips);
        // 构建面
        BRepBuilderAPI_MakeWire aWire  = BRepBuilderAPI_MakeWire();
        aWire.Add(topoDS_elips);
        TopoDS_Face myFace_elips = BRepBuilderAPI_MakeFace(aWire.Wire());
        // 构建可交互图形
        Handle(AIS_Shape) anAisEllips = new AIS_Shape(myFace_elips);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.2, 0.2, 0.2, Quantity_TOC_RGB)
        anAisEllips->SetColor(Quantity_Color(0.2, 0.2, 0.2, Quantity_TOC_RGB));
        return anAisEllips;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_BSpline(Handle(TColgp_HArray1OfPnt) aPoints)
     * 输入：控制点数组 Handle(TColgp_HArray1OfPnt) aPoints
     * 输出：一个可交互B样条，类型为 AIS_Shape
     *
     * 1.取学号末尾三位数字生成B样条颜色：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.2, 0.1, 0.3, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_BSpline(Handle(TColgp_HArray1OfPnt) aPoints) {
        // 构造样条
        Handle(Geom_BSplineCurve)geom_BSplineCure = GeomAPI_PointsToBSpline(*aPoints);
        TopoDS_Edge topo_BSpline = BRepBuilderAPI_MakeEdge(geom_BSplineCure);
        // 构建可交互图形
        Handle(AIS_Shape) anAisSpline = new AIS_Shape(topo_BSpline);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.8, 0.9, 0.2, Quantity_TOC_RGB)
        anAisSpline->SetColor(Quantity_Color(0.8, 0.9, 0.2, Quantity_TOC_RGB));
        return anAisSpline;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_box(gp_Pnt Pnt1, gp_Pnt Pnt2)
     * 输入：长方体顶点 Pnt1
     *      Pnt1 对角顶点 Pnt2
     * 输出：一个可交互长方体，类型为 AIS_Shape
     *
     * 1.取学号末尾三位数字生成长方体颜色：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.2, 0.3, 0.1, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_box(gp_Pnt Pnt1, gp_Pnt Pnt2){
        // 长方体
        TopoDS_Shape topoDS_box = BRepPrimAPI_MakeBox(Pnt1, Pnt2);
        // 生成可交互图形
        Handle(AIS_Shape) anAisBox = new AIS_Shape(topoDS_box);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.8, 0.2, 0.9, Quantity_TOC_RGB)
        anAisBox->SetColor(Quantity_Color(0.8, 0.2, 0.9, Quantity_TOC_RGB));
        return anAisBox;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_cylinder(gp_Pnt p, Standard_Real H)
     * 输入：圆柱体地面中心点 p
     *      圆柱体高 H
     * 输出：一个可交互圆柱体，类型为 AIS_Shape
     *
     * 1.取学号末位数字a计算圆柱体地面半径 R = H / (a + 1)
     * 2.取学号末尾三位数字作为坐标实体从而创建方向D，以点p和方向D创建坐标系，
     *   在此坐标系中创建圆柱体，例：假设学号以123结尾，则坐标为（3, 2, 1）
     * 3.取学号末尾三位数字生成圆柱体颜色：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.3, 0.2, 0.1, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_cylinder(gp_Pnt p, Standard_Real H) {
        // 学号2020302982
        static gp_Dir aDir = gp_Dir(2.0,8.0,9.0);
        // 圆柱体地面半径 R，学号末位为 2
        Standard_Real R = H / ( 2 + 1 );
        TopoDS_Shape topo_cylinder = BRepPrimAPI_MakeCylinder(gp_Ax2(p, aDir), R , H).Shape();
        // 可交互图形
        Handle(AIS_Shape) anAisCylinder = new AIS_Shape(topo_cylinder);
        //
        return anAisCylinder;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_cone(gp_Pnt p, Standard_Real R)
     * 输入：圆锥体地面中心点 p
     *      圆锥体地面半径 R
     * 输出：一个可交互圆锥体，类型为 AIS_Shape
     *
     * 1.取学号末位数字a计算圆锥体高度 H = R1 + a
     * 2.取学号末尾三位数字作为坐标实体从而创建方向D，以点p和方向D创建坐标系，
     *   在此坐标系中创建圆锥体，例：假设学号以123结尾，则坐标为（3, 1, 2）
     * 3.取学号末尾三位数字生成圆锥体颜色，例：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.3, 0.1, 0.2, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_cone(gp_Pnt p, Standard_Real R) {
        // 计算圆锥体高度，学号末尾数字为 2
        Standard_Real H = R + 2.0;
        // 坐标，学号后三位数字为 982
        gp_Dir aDir = gp_Dir(2.0,9.0,8.0);
        // 圆锥
        TopoDS_Shape topoDS_cone = BRepPrimAPI_MakeCone(gp_Ax2(p, aDir), R, 0, H);
        // 生成可交互图形
        Handle(AIS_Shape) anAisCone = new AIS_Shape(topoDS_cone);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.2, 0.9, 0.8, Quantity_TOC_RGB)
        anAisCone->SetColor(Quantity_Color(0.2, 0.9, 0.8, Quantity_TOC_RGB));
        return anAisCone;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_sphere(gp_Pnt center, Standard_Real R)
     * 输入：球体中心点 center
     *      球体半径 R
     * 输出：一个可交互球体，类型为 AIS_Shape
     *
     * 1.取学号末尾三位数字生成球体颜色：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.1, 0.3, 0.2, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_sphere(gp_Pnt center, Standard_Real R) {
        // 球体
        TopoDS_Shape topoDS_sphere = BRepPrimAPI_MakeSphere(center, R);
        // 生成可交互图形
        Handle(AIS_Shape) anAisSphere = new AIS_Shape(topoDS_sphere);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.9, 0.2, 0.8, Quantity_TOC_RGB)
        anAisSphere->SetColor(Quantity_Color(0.9, 0.2, 0.8, Quantity_TOC_RGB));
        return anAisSphere;
    }

    /*****************************************************x
     * static Handle(AIS_Shape) make_torus(gp_Pnt center, Standard_Real R1)
     * 输入：圆环体中心点 center
     *      圆环体中心环半径 R1
     * 输出：一个可交互圆环体，类型为 AIS_Shape
     *
     * 1.取学号末位数字a计算圆环体截面半径 R2 = R1 / (a + 1)
     * 2.取学号末尾三位数字作为坐标实体从而创建方向D，以点p和方向D创建坐标系，
     *   在此坐标系中创建圆环体，例：假设学号以123结尾，则坐标为（1, 2, 3）
     * 3.取学号末尾三位数字生成圆环体颜色：假设学号以123结尾，则颜色为
     *   Quantity_Color(0.1, 0.2, 0.3, Quantity_TOC_RGB)
     * ***************************************************/
    static Handle(AIS_Shape) make_torus(gp_Pnt center, Standard_Real R1) {
        // 计算圆环体截面半径，学号末尾数字为 2
        Standard_Real R2 = R1 / (2 + 1);
        // 坐标，学号后三位数字为 982
        gp_Dir aDir = gp_Dir(9.0,8.0,2.0);
        // 圆锥
        TopoDS_Shape topoDS_torus = BRepPrimAPI_MakeTorus(gp_Ax2(center, aDir), R1, R2);
        // 生成可交互图形
        Handle(AIS_Shape) anAisTorus = new AIS_Shape(topoDS_torus);
        // 学号后三位为982，设置颜色为 Quantity_Color(0.9, 0.8, 0.2, Quantity_TOC_RGB)
        anAisTorus->SetColor(Quantity_Color(0.9, 0.8, 0.2, Quantity_TOC_RGB));
        return anAisTorus;
    }
};

#endif // MODELMAKER_H
