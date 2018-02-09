#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T15:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeltaETool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        DeltaEMainwindow.cpp \
    ddc/ddc_aps.cpp \
    DeltaEInterface.cpp \
    dll/Ca210Ctrl_Export.cpp \
    lib/mstgengmactr.cpp \
    lib/isp_iic.cpp \
    ddc/burnsetting.cpp \
    colorwindow.cpp

HEADERS  += DeltaEMainwindow.h \
    lib/mstGenGma.h \
    ddc/ddc_aps.h \
    global.h \
    DeltaEInterface.h \
    dll/Ca210Ctrl_Export.h \
    lib/mstgengmactr.h \
    lib/FTCI2C.h \
    lib/ftd2xx.h \
    lib/FTI2CDRV.h \
    lib/isp_iic.h \
    ddc/burnsetting.h \
    colorwindow.h

FORMS    += DeltaEMainwindow.ui \
    colorwindow.ui

RESOURCES += \
    images.qrc

RC_FILE += cvte.rc


win32: LIBS += -L$$PWD/lib/ -lmstGenGma

win32: LIBS += -L$$PWD/lib/ -lFTCI2C

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
