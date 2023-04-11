/******************************
 *
 * 姓名：樊凤洁   学号：2020302982
 *
 * ***************************/

#ifndef MYCREATION_H
#define MYCREATION_H

#include <AIS_Shape.hxx>
#include <Quantity_Color.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <GC_MakeSegment.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_Transform.hxx>

class MyCreation
{
public:
    static TopoDS_Shape MyDesign()
    {
        // 构建模型方向
        gp_Dir aDir_model = gp_Dir(0.0,0.0,1.0);
        // 定义当前坐标中心(z轴方向变化)
        Standard_Real center_Z = 0;
        // 定义整体模型的长宽高
        Standard_Real Height = 70;
        Standard_Real Length = 50;
        Standard_Real width = 50;

        // 底层圆柱半径和高
        Standard_Real R_cylinder_firstFloor = Length / 2;
        Standard_Real H_cylinder_firstFloor = Height * 0.15;
        // 以点 (0, 0, center_Z) 和向量 dir 构建坐标系
        gp_Ax2 ax2_cylinder_firstFloor = gp_Ax2(gp_Pnt(0, 0, center_Z), aDir_model);
        center_Z += H_cylinder_firstFloor;
        // 构建底层圆柱
        TopoDS_Shape cylinder_firstFloor = BRepPrimAPI_MakeCylinder(ax2_cylinder_firstFloor, R_cylinder_firstFloor , H_cylinder_firstFloor).Shape();

        // 第二层圆柱的半径和高
        Standard_Real R_cylinder_secondFloor = Length / 2 * 0.9;
        Standard_Real H_cylinder_secondFloor = Height * 0.1;
        // 以点 (0, 0, center_Z) 和向量 dir 构建坐标系
        gp_Ax2 ax2_cylinder_secondFloor = gp_Ax2(gp_Pnt(0, 0, center_Z), aDir_model);
        center_Z += H_cylinder_secondFloor;
        // 构建第二层圆柱
        TopoDS_Shape cylinder_secondFloor = BRepPrimAPI_MakeCylinder(ax2_cylinder_secondFloor, R_cylinder_secondFloor , H_cylinder_secondFloor).Shape();

        // 第三层六棱柱
        // 定义支持点
        Standard_Real R_hexagonal = R_cylinder_firstFloor * 0.925;
        gp_Pnt hexagonal_point1(-0.5 * R_hexagonal, 0.866 * R_hexagonal, center_Z);
        gp_Pnt hexagonal_point2(0.5 * R_hexagonal, 0.866 * R_hexagonal, center_Z);
        gp_Pnt hexagonal_point3(R_hexagonal, 0, center_Z);
        gp_Pnt hexagonal_point4(0.5 * R_hexagonal, -0.866 * R_hexagonal, center_Z);
        gp_Pnt hexagonal_point5(-0.5 * R_hexagonal, -0.866 * R_hexagonal, center_Z);
        gp_Pnt hexagonal_point6(-1 * R_hexagonal, 0, center_Z);
        // 定义轮廓
        Handle(Geom_TrimmedCurve) hexagonal_aSegment1 = GC_MakeSegment(hexagonal_point1, hexagonal_point2);
        Handle(Geom_TrimmedCurve) hexagonal_aSegment2 = GC_MakeSegment(hexagonal_point2, hexagonal_point3);
        Handle(Geom_TrimmedCurve) hexagonal_aSegment3 = GC_MakeSegment(hexagonal_point3, hexagonal_point4);
        Handle(Geom_TrimmedCurve) hexagonal_aSegment4 = GC_MakeSegment(hexagonal_point4, hexagonal_point5);
        Handle(Geom_TrimmedCurve) hexagonal_aSegment5 = GC_MakeSegment(hexagonal_point5, hexagonal_point6);
        Handle(Geom_TrimmedCurve) hexagonal_aSegment6 = GC_MakeSegment(hexagonal_point6, hexagonal_point1);
        // 定义拓扑结构
        TopoDS_Edge hexagonal_anEdge1 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment1);
        TopoDS_Edge hexagonal_anEdge2 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment2);
        TopoDS_Edge hexagonal_anEdge3 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment3);
        TopoDS_Edge hexagonal_anEdge4 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment4);
        TopoDS_Edge hexagonal_anEdge5 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment5);
        TopoDS_Edge hexagonal_anEdge6 = BRepBuilderAPI_MakeEdge(hexagonal_aSegment6);
        BRepBuilderAPI_MakeWire hexagonal_aWire  = BRepBuilderAPI_MakeWire();
        hexagonal_aWire.Add(hexagonal_anEdge1);
        hexagonal_aWire.Add(hexagonal_anEdge2);
        hexagonal_aWire.Add(hexagonal_anEdge3);
        hexagonal_aWire.Add(hexagonal_anEdge4);
        hexagonal_aWire.Add(hexagonal_anEdge5);
        hexagonal_aWire.Add(hexagonal_anEdge6);
        // 构建方向向量，拉伸轮廓
        TopoDS_Face myFace_hexagonal = BRepBuilderAPI_MakeFace(hexagonal_aWire.Wire());
        Standard_Real H_hexagonal = Height * 0.7;
        gp_Vec vec_hexagonal = gp_Vec(0, 0, H_hexagonal);
        TopoDS_Shape makeBody_hexagonal = BRepPrimAPI_MakePrism(myFace_hexagonal, vec_hexagonal);
        // 圆角处理
        BRepFilletAPI_MakeFillet mkFillet_hexagonal(makeBody_hexagonal);
        TopExp_Explorer anEdgeExplorer(makeBody_hexagonal, TopAbs_EDGE);
        while(anEdgeExplorer.More()) {
            TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
            // 进行圆角处理，设置 dis = length / 35
            Standard_Real dis = Length / 35;
            mkFillet_hexagonal.Add(dis, anEdge);
            // 查找下一拓扑结构
            anEdgeExplorer.Next();
        }
        // 得到六边体
        TopoDS_Shape hexagonal_body = mkFillet_hexagonal.Shape();
        // 偏移六边体
        Standard_Real aOffset = 0.5;
        BRepOffsetAPI_MakeOffsetShape aMakeOffsetShape;
        aMakeOffsetShape.PerformBySimple(hexagonal_body, aOffset);
        hexagonal_body = aMakeOffsetShape.Shape();

        // 六棱柱内部空心圆柱
        // 六棱柱内部圆柱半径和高
        Standard_Real R_cylinder_inside1 = Length / 2 * 0.1;
        Standard_Real H_cylinder_inside1 = Height / 2 * 0.1;
        // 内部圆柱挖空部分
        Standard_Real R_cylinder_inside2 = R_cylinder_inside1 * 0.8;
        Standard_Real H_cylinder_inside2 = H_cylinder_inside1;
        // 以点 (0, 0, center_Z) 和向量 dir 构建坐标系
        gp_Ax2 ax2_cylinder_inside = gp_Ax2(gp_Pnt(0, 0, center_Z), aDir_model);
        center_Z += H_hexagonal;
        // 构建内部空心圆柱
        TopoDS_Shape cylinder_inside1 = BRepPrimAPI_MakeCylinder(ax2_cylinder_inside, R_cylinder_inside1 , H_cylinder_inside1).Shape();
        TopoDS_Shape cylinder_inside2 = BRepPrimAPI_MakeCylinder(ax2_cylinder_inside, R_cylinder_inside2 , H_cylinder_inside2).Shape();
        // 布尔求差
        BRepAlgoAPI_Cut cut = BRepAlgoAPI_Cut(cylinder_inside1, cylinder_inside2);
        TopoDS_Shape cylinder_inside_body = cut.Shape();

        // 第四层圆柱，位于六棱柱上方
        // 第四层圆柱的半径和高
        Standard_Real R_cylinder_fourthFloor = Length / 2 * 0.9;
        Standard_Real H_cylinder_fourthFloor = Height * 0.05;
        // 以点 (0, 0, center_Z) 和向量 dir 构建坐标系
        gp_Ax2 ax2_cylinder_fourthFloor = gp_Ax2(gp_Pnt(0, 0, center_Z), aDir_model);
        center_Z += H_cylinder_fourthFloor;
        // 构建第四层圆柱
        TopoDS_Shape cylinder_fourthFloor = BRepPrimAPI_MakeCylinder(ax2_cylinder_fourthFloor, R_cylinder_fourthFloor , H_cylinder_fourthFloor).Shape();

        // 构建第五层半球
        // 球中心和半径
        gp_Pnt center(0, 0, center_Z);
        Standard_Real R_sphere = R_cylinder_fourthFloor * 1.05;
        // 球体
        TopoDS_Shape topoDS_sphere = BRepPrimAPI_MakeSphere(center, R_sphere, 3.14);
        // 旋转球体
        gp_Ax1 ax1_sphere = gp_Ax1(gp_Pnt(0, 0, center_Z), gp_Dir(-1, 0, 0));
        center_Z += R_sphere;
        gp_Trsf trsf;
        trsf.SetRotation(ax1_sphere, -1.57075);
        BRepBuilderAPI_Transform aTransformation =  BRepBuilderAPI_Transform(topoDS_sphere, trsf);
        TopoDS_Shape sphere_body = aTransformation;

        // 第六层层圆锥
        // 半径和高
        Standard_Real R_cone = Length * 0.1;
        Standard_Real H_cone = Height * 0.15;
        // 以点 (0, 0, center_Z) 和向量 dir 构建坐标系
        gp_Ax2 ax2_cone = gp_Ax2(gp_Pnt(0, 0, center_Z), aDir_model);
        center_Z += H_cone;
        // 圆锥
        TopoDS_Shape topoDS_cone = BRepPrimAPI_MakeCone(ax2_cone, R_cone, H_cone - H_cone / 5, H_cone);
        // 圆锥下移使其紧贴半球
        gp_Dir dir_cone = gp_Dir(0, 0, -0.25 * H_cone);
        gp_Vec vec_cone(dir_cone);
        // 平移变换 vec_cone 单位
        gp_Trsf trsf_cone;
        trsf_cone.SetTranslation(vec_cone);
        BRepBuilderAPI_Transform aTransformation_cone =  BRepBuilderAPI_Transform(topoDS_cone, trsf_cone);
        TopoDS_Shape cone_body = aTransformation_cone.Shape();
        center_Z -= H_cone * 0.25;

        // 第七层球体
        // 球中心和半径
        gp_Pnt center_topSphere(0, 0, center_Z);
        Standard_Real R_sphere_top = H_cone - H_cone / 5;
        // 球体
        TopoDS_Shape topoDS_sphere_top = BRepPrimAPI_MakeSphere(center_topSphere, R_sphere_top, 3.14);
        // 旋转球体
        gp_Ax1 ax1_topSphere = gp_Ax1(gp_Pnt(0, 0, center_Z), gp_Dir(-1, 0, 0));
        gp_Trsf trsf_top;
        trsf_top.SetRotation(ax1_topSphere, -1.57075);
        BRepBuilderAPI_Transform aTransformation_top =  BRepBuilderAPI_Transform(topoDS_sphere_top, trsf_top);
        TopoDS_Shape topSphere_body = aTransformation_top.Shape();

        // 拼接
        TopoDS_Shape myModel = BRepAlgoAPI_Fuse(cylinder_firstFloor, cylinder_secondFloor).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, cylinder_inside_body).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, hexagonal_body).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, cylinder_fourthFloor).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, sphere_body).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, cone_body).Shape();
        myModel = BRepAlgoAPI_Fuse(myModel, topSphere_body).Shape();

        return myModel;
    }
};

#endif // MYCREATION_H
