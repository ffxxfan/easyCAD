QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += VIEW3D

SOURCES += \
    DesignCommand3D.cpp \
    DesignCommand.cpp \
    DrawCommand.cpp \
    OccWidget.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AIS_DraftShape.hxx \
    AdvanceModeling.hpp \
    BasicModeling.hpp \
    DesignCommand3D.h \
    DesignCommand.h \
    DrawCommand.h \
    E:/学呀学/计算机创成理论与方法/lab3/BasicModeling.hpp \
    E:/学呀学/计算机创成理论与方法/lab3/BasicModeling.hpp \
    MainWindow.h \
    ModelMaker.hpp \
    MyCreation.hpp \
    OccWidget.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:\Class\opencascade\OpenCASCADE-7.5.0-vc14-64\opencascade-7.5.0\inc
LIBS += -LD:\Class\opencascade\OpenCASCADE-7.5.0-vc14-64\opencascade-7.5.0\win64\vc14\lib
LIBS += -lTKBin -lTKBinL -lTKBinTObj -lTKBinXCAF -lTKBO -lTKBool -lTKBRep -lTKCAF \
        -lTKCDF -lTKD3DHost -lTKDCAF -lTKDraw -lTKernel -lTKFeat -lTKFillet -lTKG2d \
        -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKIGES -lTKIVtk -lTKIVtkDraw -lTKLCAF \
        -lTKMath -lTKMesh -lTKMeshVS -lTKOffset -lTKOpenGl -lTKPrim -lTKQADraw -lTKService \
        -lTKShHealing -lTKStd -lTKStdL -lTKSTEP -lTKSTEP209 -lTKSTEPAttr -lTKSTEPBase -lTKSTL \
        -lTKTObj -lTKTObjDRAW -lTKTopAlgo -lTKTopTest -lTKV3d -lTKVCAF -lTKViewerTest \
        -lTKVRML -lTKXCAF -lTKXDEDRAW -lTKXDEIGES -lTKXDESTEP -lTKXMesh -lTKXml -lTKXmlL \
        -lTKXmlTObj -lTKXmlXCAF -lTKXSBase -lTKXSDRAW \

INCLUDEPATH += $$PWD/SARibbonBar
CONFIG(debug, debug|release) {
    LIBS += $$PWD/bind/SARibbonBar.lib
    DESTDIR = $$PWD/bind
} else {
    LIBS += $$PWD/bin/SARibbonBar.lib
    DESTDIR = $$PWD/bin
}

QMAKE_CXXFLAGS += /utf-8

RESOURCES += \
    resources.qrc

DISTFILES +=
