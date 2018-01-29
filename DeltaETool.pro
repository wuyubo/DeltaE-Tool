#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T15:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeltaETool
TEMPLATE = app


SOURCES += main.cpp\
        DeltaEMainwindow.cpp \
    DeltaECommonInterface.cpp

HEADERS  += DeltaEMainwindow.h \
    global_define.h \
    DeltaECcommonInterface.h \
    lib/mstGenGma.h

FORMS    += DeltaEMainwindow.ui

RESOURCES += \
    images.qrc

RC_FILE += cvte.rc





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lmstGenGma
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lmstGenGma

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
