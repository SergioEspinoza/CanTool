#-------------------------------------------------
#
# Project created by QtCreator 2014-09-09T16:50:43
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yaz_CanTool
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    maingeneratorwindow.cpp \
    dbcModels/qattributedbc.cpp \
    dbcModels/qdbcitem.cpp \
    dbcModels/qdbcmodel.cpp \
    com/serialcomdelegate.cpp \
    com/messageworker.cpp \
    canMsg/canmsg.cpp \
    com/serialportconfig.cpp \
    com/editconnectiondialog.cpp \
    canMsg/canmsggroup.cpp \
    dbcModels/dbcdata.cpp \
    scriptMainGUI/jsonscript.cpp \
    com/cominterface.cpp \
    scriptMainGUI/scriptcontroller.cpp \
    scriptMainGUI/scriptguicontroller.cpp \
    widgets/dbviewer.cpp \
    widgets/qdatafield.cpp \
    widgets/qtransmissionsetup.cpp \
    libcandbc/dbcModel.c \
    libcandbc/dbcReader.c \
    libcandbc/lexer.c \
    libcandbc/parser.c \
    libcandbc/vimmdummy.c \
    scriptEditGUI/editscriptdialog.cpp \
    scriptEditGUI/groupedittabledelegate.cpp \
    scriptEditGUI/groupedittablemodel.cpp \
    dbcModels/dbcfilebrowserdialog.cpp \
    igGUI/qigdelegate.cpp \
    igGUI/qigitem.cpp \
    igGUI/qigmodel.cpp \
    scriptEditGUI/msgedittablemodel.cpp

HEADERS  += maingeneratorwindow.h \
    libcandbc/dbcModel.h \
    libcandbc/dbcReader.h \
    libcandbc/dbcTypes.h \
    dbcModels/qattributedbc.h \
    dbcModels/qdbcitem.h \
    dbcModels/qdbcmodel.h \
    com/serialcomdelegate.h \
    com/messageworker.h \
    canMsg/canmsg.h \
    com/serialportconfig.h \
    com/editconnectiondialog.h \
    canMsg/canmsggroup.h \
    dbcModels/dbcdata.h \
    scriptMainGUI/jsonscript.h \
    com/cominterface.h \
    scriptMainGUI/scriptcontroller.h \
    scriptMainGUI/scriptguicontroller.h \
    igDelegate/qigdelegate.h \
    libcandbc/parser.h \
    widgets/dbviewer.h \
    widgets/qdatafield.h \
    widgets/qtransmissionsetup.h \
    scriptEditGUI/editscriptdialog.h \
    scriptEditGUI/groupedittabledelegate.h \
    scriptEditGUI/groupedittablemodel.h \
    dbcModels/dbcfilebrowserdialog.h \
    igGUI/qigdelegate.h \
    igGUI/qigitem.h \
    igGUI/qigmodel.h \
    scriptEditGUI/msgedittablemodel.h

FORMS    += \
    maingeneratorwindow.ui \
    editconnectiondialog.ui \
    scriptEditGUI/editscriptdialog.ui \
    dbcModels/dbcfilebrowserdialog.ui

win32: LIBS += -L$$PWD/libcandbc/lib/ -lcandbc

INCLUDEPATH += $$PWD/libcandbc
DEPENDPATH += $$PWD/libcandbc

RESOURCES += \
    res.qrc
