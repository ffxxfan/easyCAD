# CAD 实现

* [目的](#目的)
* [安装环境](#安装环境)
* [使用](#使用)
* [贡献](#贡献)

****

## 目的

使用基于 C++编程语言的 Open Cascade 开源 CAD / CAM / CAE 软件开发库和 Qt 软件开发平台，实现 CAD 软件中基础图形绘图功能。



## 安装环境

* 安装 Visual Studio [官网](https://visualstudio.microsoft.com/)

* 安装 Open CASCADE [官网]([Download - Open CASCADE Technology](https://dev.opencascade.org/release))

* 配置并运行 ISPP3D 平台 

  * 打开 ISPP3D 文件夹，找到 ISPP3D.pro，用 Qt 打开文件，进入 ISPP3D 项目。 首次打开项目会出现弹框提示 No valid setting file could be found。 点击 OK，进入项目设置![image](https://github.com/fanfj335/easyCAD/blob/main/icons/image1.png)

  * 在项目设置中只勾选 Desktop Qt 5.14.2 MSVC2017 64bit 选项， 点击 Configure Project 按钮确定设置，等待项目加载完成。 ![image](https://github.com/fanfj335/easyCAD/blob/main/icons/image2.png)

  

  * 双击 ISPP3D.pro 打开文件，将 INCLUDEPATH 和 LIBS 路径修改为 

    你的 OCC 安装目录。

    ![image](https://github.com/fanfj335/easyCAD/blob/main/icons/image3.png)

    

## 使用

![image](https://github.com/fanfj335/easyCAD/blob/main/icons/CAD%20demo.gif)



## 贡献

该项目是课程作业，其中 **基础图像绘制** 和 **基础图形设计** 功能由作者完成，即

| Class                      | Description                                                  |
| -------------------------- | ------------------------------------------------------------ |
| Header.BasicModeling.hpp   | 实现离散化边、求两条边之间的极值点、求点到线/面的投影、切换 选择模式。 |
| Header.ModelMaker.hpp      | 实现 CAD 软件中基础图形绘图功能，包括点、线段、圆、椭圆、B 样条曲线、长方体、圆柱体、圆锥体、 球体和圆环体等基础图形。 |
| Header.AdvanceModeling.hpp | 实现几何体布尔运算（并、交、差）、圆角、倒角、移动、旋转、缩 放、镜像、偏移等功能。 |





