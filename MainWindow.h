#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "SARibbonMainWindow.h"
#include <QTextEdit>

#include "OccWidget.h"
#include <STEPCAFControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <Interface_Static.hxx>
#include <StlAPI.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <Quantity_Color.hxx>

class SARibbonCategory;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *par = nullptr);
    ~MainWindow();

    void initFunction();
    void initToolBar();
    void initRibbon();

private:
    void createCategoryStart(SARibbonCategory *page);
    void createCategoryDraw(SARibbonCategory *page);
    void createCategoryDesign(SARibbonCategory *page);
    void createCategoryControl(SARibbonCategory *page);
    void createCategoryView(SARibbonCategory *page);
    void createCategoryHelp(SARibbonCategory *page);

private slots:
    // 导入
    void on_importModel();
    // 导出
    void on_exportModel();
    // 图形选择
    void onShapeSelected();
    // 显示所有隐藏的元素
    void on_displayDelete();
    // 隐藏选中项目，先选择，后操作
    void on_deleteSelected();
    // 设置选中元素的颜色
    void on_setItemColor();

private:
    OccWidget *occWidget;
    QList <Handle(AIS_InteractiveObject)> m_displayedShapes;
    bool shadedModel = true;
    QTextEdit* entityInfo;

signals:
    void endDraw();
};

#endif // MAINWINDOW_H
