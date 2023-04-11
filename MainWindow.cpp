#include "MainWindow.h"
#include "AIS_Shape.hxx"
#include "RobotLink.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPannel.h"
#include "SARibbonMenu.h"
#include "SARibbonApplicationButton.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QToolBar>
#include <QDockWidget>
#include <QTableView>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

#include <gp_Circ.hxx>
#include <gp_Sphere.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_SphericalSurface.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_Ellipse.hxx>

#include "DesignCommand3D.h"
#include "DesignCommand.h"
#include "DrawCommand.h"
#include "TopoDS_Vertex.hxx"
#include "ModelMaker.hpp"
#include "BasicModeling.hpp"
#include "MyCreation.hpp"
#include "qapplication.h"

MainWindow::MainWindow(QWidget *par) :
    SARibbonMainWindow(par)
{
    setWindowTitle("ISPP3D");
}

MainWindow::~MainWindow()
{
}

void MainWindow::initFunction()
{
    occWidget = new OccWidget(this);
    setCentralWidget(occWidget);

    // 选中形状时，通过此获取选中的形状信息
    connect(occWidget, &OccWidget::selectShapeChanged, this, &MainWindow::onShapeSelected);

    // 设置信息栏
    entityInfo = new QTextEdit(this);
    entityInfo->setMinimumWidth(200);
    QDockWidget* dock = new QDockWidget(this);
    dock->setWidget(entityInfo);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::initToolBar()
{
    QToolBar *toolBar_view = new QToolBar("视图",this);
    toolBar_view->setAllowedAreas(Qt::TopToolBarArea);

    QAction *act = new QAction(QIcon(":/icons/Axo.png"),"重置视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_XposYnegZpos);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Left.png"),"左视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Xneg);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Right.png"),"右视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Xpos);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Front.png"),"前视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Yneg);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Back.png"),"后视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Ypos);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Top.png"),"顶视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Zpos);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/Bottom.png"),"底视图",this);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=](){
        occWidget->GetView()->SetProj(V3d_Zneg);
        occWidget->GetView()->FitAll();
    });
    toolBar_view->addAction(act);

    toolBar_view->addSeparator();

    act = new QAction(QIcon(":/icons/view_shade.png"),"显示模式",this);
    connect(act,&QAction::triggered,this,[=](){
        /* Your code here */
        if(shadedModel)
        {
            occWidget->GetContext()->SetDisplayMode(AIS_WireFrame,Standard_True);
            act->setIcon(QIcon(":/icons/view_wire.png"));
            shadedModel = false;
        }
        else
        {
            occWidget->GetContext()->SetDisplayMode(AIS_Shaded,Standard_True);
            act->setIcon(QIcon(":/icons/view_shade.png"));
            shadedModel = true;
        }
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/color_tool.png"),"设置颜色",this);
    connect(act,&QAction::triggered,this,&MainWindow::on_setItemColor);
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/alpha.png"),"设置透明度",this);
    connect(act,&QAction::triggered,this,[=]() {
        /* Your code here */
        occWidget->GetContext()->InitSelected();
        if(occWidget->GetContext()->HasSelectedShape()) {
            double a = QInputDialog::getDouble(this, "透明度", "", 0, 0.0, 1.0);
            Handle(AIS_InteractiveObject) Current = occWidget->GetContext()->SelectedInteractive() ;
            Current->SetTransparency(a);
        }
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/hide.png"),"隐藏选中模型",this);
    connect(act,&QAction::triggered,this,&MainWindow::on_deleteSelected);
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/display.png"),"显示隐藏的模型",this);
    connect(act,&QAction::triggered,this,&MainWindow::on_displayDelete);
    toolBar_view->addAction(act);

    toolBar_view->addSeparator();

    act = new QAction(QIcon(":/icons/selectReset.png"),"重置选择",this);
    connect(act,&QAction::triggered,this,[=](){
        BasicModeling::selection_activate(0, occWidget->GetContext());
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/selectPnt.png"),"选择点",this);
    connect(act,&QAction::triggered,this,[=](){
        BasicModeling::selection_activate(1, occWidget->GetContext());
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/selectEdge.png"),"选择边",this);
    connect(act,&QAction::triggered,this,[=](){
        BasicModeling::selection_activate(2, occWidget->GetContext());
    });
    toolBar_view->addAction(act);

    act = new QAction(QIcon(":/icons/selectFace.png"),"选择面",this);
    connect(act,&QAction::triggered,this,[=](){
        BasicModeling::selection_activate(4, occWidget->GetContext());
    });
    toolBar_view->addAction(act);
    act = new QAction(QIcon(":/icons/selectSolid.png"),"选择实体",this);
    connect(act,&QAction::triggered,this,[=](){
        BasicModeling::selection_activate(6, occWidget->GetContext());
    });
    toolBar_view->addAction(act);

    this->addToolBar(Qt::TopToolBarArea,toolBar_view);
}

void MainWindow::initRibbon()
{
    SARibbonBar *ribbon = ribbonBar();
    ribbon->applitionButton()->setVisible(false);

    SARibbonCategory* categoryStart = ribbon->addCategoryPage("开始");
    createCategoryStart(categoryStart);
    SARibbonCategory* categoryDraw = ribbon->addCategoryPage("绘制");
    createCategoryDraw(categoryDraw);
    SARibbonCategory* categorDesign = ribbon->addCategoryPage("设计");
    createCategoryDesign(categorDesign);
    SARibbonCategory* categoryControl = ribbon->addCategoryPage("控制");
    createCategoryControl(categoryControl);
    SARibbonCategory* categoryView = ribbon->addCategoryPage("视图");
    createCategoryView(categoryView);
    SARibbonCategory* categoryHelp = ribbon->addCategoryPage("帮助");
    createCategoryHelp(categoryHelp);
}

void MainWindow::createCategoryStart(SARibbonCategory *page)
{
    SARibbonPannel* pannel = page->addPannel("");

    QAction* act = new QAction(this);
    act->setIcon(QIcon(":/icons/import.png"));
    act->setText("导入");
    pannel->addLargeAction(act);
    connect(act,&QAction::triggered,this,&MainWindow::on_importModel);

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/export.png"));
    act->setText("导出");
    pannel->addLargeAction(act);
    connect(act,&QAction::triggered,this,&MainWindow::on_exportModel);
}

void MainWindow::createCategoryDraw(SARibbonCategory *page)
{
    SARibbonPannel* pannel = page->addPannel("");

    QAction* act = new QAction(this);
    act->setIcon(QIcon(":/icons/point.png"));
    act->setText("画点");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        DrawPointCommand* cmd = new DrawPointCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/line.png"));
    act->setText("画线");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        DrawLineCommand* cmd = new DrawLineCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/circle.png"));
    act->setText("画圆");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        DrawCircCommand* cmd = new DrawCircCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/ellipse.png"));
    act->setText("画椭圆");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        DrawElipsCommand* cmd = new DrawElipsCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/spline.png"));
    act->setText("画样条");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        DrawSplineCommand* cmd = new DrawSplineCommand(occWidget, this);
    });

    pannel = page->addPannel("");

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/box.png"));
    act->setText("画长方体");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        DrawBoxCommand* cmd = new DrawBoxCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/cylinder.png"));
    act->setText("画圆柱体");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        DrawCylinderCommand* cmd = new DrawCylinderCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/cone.png"));
    act->setText("画圆锥体");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        DrawConeCommand* cmd = new DrawConeCommand(occWidget, this);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/sphere.png"));
    act->setText("画球体");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        DrawSphereCommand* cmd = new DrawSphereCommand(occWidget, this);
        // Handle(AIS_Shape) aisc = new AIS_Shape(ModelMaker::make_sphere({0,0,0},70.0));
        // occWidget->GetContext()->Display(aisc, true);
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/torus.png"));
    act->setText("画圆环体");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        DrawTorusCommand* cmd = new DrawTorusCommand(occWidget, this);
    });

    pannel = page->addPannel("");

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/model.png"));
    act->setText("自定义模型");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        occWidget->stopDrawHandler();
        TopoDS_Shape myDesignShape = MyCreation::MyDesign();
        Handle(AIS_Shape) anAisDesign = new AIS_Shape(myDesignShape);
        m_displayedShapes.append(anAisDesign);
        occWidget->GetContext()->Display(anAisDesign, true);
        occWidget->GetView()->FitAll();
    });
}

void MainWindow::createCategoryDesign(SARibbonCategory *page)
{
    SARibbonPannel* pannel = page->addPannel("");

    QAction* act = new QAction(this);
    act->setIcon(QIcon(":/icons/dispersion.png"));
    act->setText("离散");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        DispersionCommand* cmd = new DispersionCommand(occWidget, this);
        connect(cmd, &DesignCommand::setDesignInfo, this, [=](QString info){
            entityInfo->setText(info);
        });
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/poler.png"));
    act->setText("求极值");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        ExtremCommand* cmd = new ExtremCommand(occWidget, this);
        connect(cmd, &DesignCommand::setDesignInfo, this, [=](QString info){
            entityInfo->setText(info);
        });
    });

    act = new QAction(this);
    act->setIcon(QIcon(":/icons/project2curve.png"));
    act->setText("投影到线");
    pannel->addLargeAction(act);
    /* Your code here */
    // 设置按钮功能
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        Project2CurveCommand* cmd = new Project2CurveCommand(occWidget, this);
        connect(cmd, &DesignCommand::setDesignInfo, this, [=](QString info){
            entityInfo->setText(info);
        });
    });

    pannel = page->addPannel("");

    act = new QAction(this); // 先点击按钮，后选择元素
    act->setText("交");
    act->setIcon(QIcon(":/icons/&.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        BoolDesignCommand* cmd = new BoolDesignCommand(1,occWidget, this);
    });

    act = new QAction(this); // 先点击按钮，后选择元素
    act->setText("并");
    act->setIcon(QIcon(":/icons/U.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->detachManipulator();
        BoolDesignCommand* cmd = new BoolDesignCommand(0,occWidget, this);
    });

    act = new QAction(this); // 先点击按钮，后选择元素
    act->setText("差");
    act->setIcon(QIcon(":/icons/-.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        BoolDesignCommand* cmd = new BoolDesignCommand(2,occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setIcon(QIcon(":/icons/transform.png"));
    act->setText("变换");
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act,&QAction::triggered,this,[=]() {
        // TransformCommand* cmd = new TransformCommand(occWidget, this);
        occWidget->stopDrawHandler();
        occWidget->GetContext()->InitSelected();
        if(occWidget->GetContext()->HasSelectedShape()) {
            Handle(AIS_InteractiveObject) Current = occWidget->GetContext()->SelectedInteractive() ;
            occWidget->GetManipulator()->Attach(Current);
            occWidget->GetView()->Update();
        } else {
            occWidget->detachManipulator();
            occWidget->GetView()->Update();
        }
    });

    act = new QAction(this); // 先点击按钮，后选择元素
    act->setText("投影到面");
    act->setIcon(QIcon(":/icons/project2face.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        Project2FaceCommand* cmd = new Project2FaceCommand(occWidget, this);
        connect(cmd, &DesignCommand3D::setDesignInfo, this, [=](QString info){
            entityInfo->setText(info);
        });
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("圆角");
    act->setIcon(QIcon(":/icons/fillet.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        FilletCommand* cmd = new FilletCommand(occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("倒角");
    act->setIcon(QIcon(":/icons/corner.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        ChamferCommand* cmd = new ChamferCommand(occWidget, this);
    });

    pannel = page->addPannel("");

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("移动");
    act->setIcon(QIcon(":/icons/translate.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        TranslateCommand* cmd = new TranslateCommand(occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("旋转");
    act->setIcon(QIcon(":/icons/rotate.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        RotateCommand* cmd = new RotateCommand(occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("缩放");
    act->setIcon(QIcon(":/icons/scale.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        ScaleCommand* cmd = new ScaleCommand(occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("镜像");
    act->setIcon(QIcon(":/icons/mirror.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        MirrorCommand* cmd = new MirrorCommand(occWidget, this);
    });

    act = new QAction(this);  // 先选择元素，后点击按钮
    act->setText("偏移");
    act->setIcon(QIcon(":/icons/offset.png"));
    pannel->addLargeAction(act);
    /* Your code here */
    connect(act, &QAction::triggered, this, [=]() {
        occWidget->stopDrawHandler();
        occWidget->detachManipulator();
        OffsetCommand* cmd = new OffsetCommand(occWidget, this);
    });
}

void MainWindow::createCategoryControl(SARibbonCategory *page)
{
}

void MainWindow::createCategoryView(SARibbonCategory *page)
{
}

void MainWindow::createCategoryHelp(SARibbonCategory *page)
{
}

void MainWindow::on_importModel()
{
    QString modelFileName = QFileDialog::getOpenFileName(this,tr("选择模型文件"),"","*.step *.STEP *.stp *.STP\n"
                                                                              "*.iges *.IGES *.igs *.IGS\n"
                                                                              "*.stl *.STL\n"
                                                                              "*.brep *.brp");
    if(modelFileName.isEmpty())
        return;

    TCollection_AsciiString theAscii(modelFileName.toUtf8().data());

    QFileInfo info(modelFileName);

    std::shared_ptr<XSControl_Reader> aReader;
    if(info.suffix()=="step"||info.suffix()=="stp"||info.suffix()=="STEP"||info.suffix()=="STP")
    {
        aReader = std::make_shared<STEPControl_Reader>();
    }
    else if(info.suffix()=="iges"||info.suffix()=="igs"||info.suffix()=="IGES"||info.suffix()=="IGS")
    {

    }
    else if(info.suffix()=="stl"||info.suffix()=="STL")
    {

    }
    else if(info.suffix()=="brep"||info.suffix()=="brp")
    {

    }

    if(!aReader->ReadFile(theAscii.ToCString()))
    {
        QMessageBox::critical(this,tr("错误"),tr("模型导入失败!"));
        return;
    }

    for(int i=0;i<aReader->TransferRoots();++i)
    {
        Handle(AIS_Shape) anAIS = new AIS_Shape(aReader->Shape(i+1));
        anAIS->Attributes()->SetFaceBoundaryDraw(true);
        anAIS->Attributes()->SetFaceBoundaryAspect(
                    new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
        anAIS->Attributes()->SetIsoOnTriangulation(true);
        m_displayedShapes.append(anAIS);
        occWidget->GetContext()->Display(anAIS,false);
    }

    occWidget->GetView()->FitAll();
}

void MainWindow::on_exportModel()
{
    if(m_displayedShapes.isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("场景内容为空!"));
        return;
    }

    QString modelFileName = QFileDialog::getSaveFileName(this,tr("选择要保存的位置"),"","*.step *.STEP *.stp *.STP");
    if(modelFileName.isEmpty())
        return;

    QFileInfo info(modelFileName);
    TCollection_AsciiString theAscii(modelFileName.toUtf8().data());
    if(info.suffix()=="step"||info.suffix()=="stp"||info.suffix()=="STEP"||info.suffix()=="STP")
    {
        STEPControl_Writer stepWriter;

        for(int i=0;i<m_displayedShapes.size();i++)
        {
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast (m_displayedShapes.at(i));
            if (anIS.IsNull())
            {
                return;
            }

            if (stepWriter.Transfer (anIS->Shape(), STEPControl_AsIs) != IFSelect_RetDone)
            {
                QMessageBox::critical(this,tr("错误"),tr("模型导出失败!"));
                return;
            }
            if(!stepWriter.Write (theAscii.ToCString()))
            {
                QMessageBox::critical(this,tr("错误"),tr("模型导出失败!"));
                return;
            }
        }

    }
    else if(info.suffix()=="iges"||info.suffix()=="igs"||info.suffix()=="IGES"||info.suffix()=="IGS")
    {

    }
    else if(info.suffix() == "brep")
    {

    }
}

void MainWindow::on_displayDelete()
{
    /* Your code here */
    // Display All
    occWidget->GetContext()->DisplayAll(true);
}

void MainWindow::on_deleteSelected()
{
    /* Your code here */
    // Erase Selected
    occWidget->GetContext()->EraseSelected(Standard_True);
}

void MainWindow::on_setItemColor()
{
    occWidget->GetContext()->InitSelected();
    if(occWidget->GetContext()->HasSelectedShape())
    {
        /* Your code here */
        QColor aColor ;
        Handle(AIS_InteractiveObject) Current = occWidget->GetContext()->SelectedInteractive() ;
        if ( Current->HasColor () )
        {
            Quantity_Color aShapeColor;
            occWidget->GetContext()->Color( Current, aShapeColor );
            aColor.setRgb( (Standard_Integer)(aShapeColor.Red() * 255), (Standard_Integer)(aShapeColor.Green() * 255),
                           (Standard_Integer)(aShapeColor.Blue() * 255));
        }
        else
            aColor.setRgb( 255, 255, 255 );

        QColor aRetColor = QColorDialog::getColor( aColor );
        if ( aRetColor.isValid() )
        {
            Quantity_Color color( aRetColor.red() / 255., aRetColor.green() / 255.,
                                  aRetColor.blue() / 255., Quantity_TOC_RGB );

            Current->SetColor(color);
            Current->Redisplay(Standard_True);
        }
    }
    else
    {
        QMessageBox::critical(this,tr("错误"),tr("选择对象后再操作!"));
    }
}

void MainWindow::onShapeSelected()
{
    /* Your code here */
    // 获取AIS可交互环境
    Handle(AIS_InteractiveContext) context = occWidget->GetContext();
    // 初始化已选形状
    TopoDS_Shape selected;
    // 获取已选择形状
    for(context->InitSelected();context->MoreSelected();context->NextSelected())
    {
        selected = occWidget->GetContext()->SelectedShape();
        break;
    }

    // 已选择形状为空
    if(selected.IsNull())
        return;

    // 判断所选形状类型，在信息栏显示形状信息
    QString info;
    if(selected.ShapeType() == TopAbs_VERTEX) {
        TopoDS_Vertex vet = TopoDS::Vertex(selected);
        gp_Pnt p = BRep_Tool::Pnt(vet);
        info = QString("点：(%1, %2, %3)").arg(p.X()).arg(p.Y()).arg(p.Z());

    }
    else if(selected.ShapeType() == TopAbs_EDGE) {
        TopoDS_Edge edge = TopoDS::Edge(occWidget->GetContext()->SelectedShape());
        Standard_Real first, last;
        Handle(Geom_Curve) curve  = BRep_Tool::Curve(edge,first,last);

        Handle(Geom_Line) line = Handle(Geom_Line)::DownCast(curve);
        Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);
        Handle(Geom_Ellipse) ellips = Handle(Geom_Ellipse)::DownCast(curve);
        Handle(Geom_BSplineCurve) bspline = Handle(Geom_BSplineCurve)::DownCast(curve);

        if(line) {
            gp_Pnt p1 = curve->Value(first);
            gp_Pnt p2 = curve->Value(last);
            info = QString("起点：(%1, %2, %3)，终点：(%4, %5, %6) \n").arg(p1.X()).arg(p1.Y()).arg(p1.Z()).arg(p2.X()).arg(p2.Y()).arg(p2.Z());
            info += QString("向量：(%1, %2, %3)，长度：%4").arg(gp_Dir(p2.XYZ() - p1.XYZ()).X()).arg(gp_Dir(p2.XYZ() - p1.XYZ()).Y()).arg(gp_Dir(p2.XYZ() - p1.XYZ()).Z()).arg(p2.Distance(p1));
        }
        else if(circle) {
            gp_Circ circ = circle->Circ();
            info = QString("圆心：(%1, %2, %3)，半径：%4").arg(circ.Location().X()).arg(circ.Location().Y()).arg(circ.Location().Z()).arg(circ.Radius());
        }
        else if(ellips) {
            gp_Elips elips = ellips->Elips() ;
            info = QString("椭圆圆心：(%1, %2, %3)，").arg(elips.Location().X()).arg(elips.Location().Y()).arg(elips.Location().Z());
            info += QString("长半径：%1，短半径：%2").arg(elips.MajorRadius()).arg(elips.MinorRadius());
        }
        else if(bspline) {
            info = QString("B样条曲线");
        }
    }
    else if(selected.ShapeType() == TopAbs_FACE) {
        TopoDS_Face face = TopoDS::Face(occWidget->GetContext()->SelectedShape());
        Handle(Geom_Surface) surface  = BRep_Tool::Surface(face);

        Handle(Geom_SphericalSurface) sphere = Handle(Geom_SphericalSurface)::DownCast(surface);

        if(sphere) {
            gp_Sphere sphe = sphere->Sphere();
            info = QString("球心：(%1, %2, %3)，半径：%4").arg(sphe.Location().X()).arg(sphe.Location().Y()).arg(sphe.Location().Z()).arg(sphe.Radius());
        }
    }

    entityInfo->setText(info);
}
