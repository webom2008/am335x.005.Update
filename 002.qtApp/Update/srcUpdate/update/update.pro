#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T17:01:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -luuid -lts -lrt
#LIBS += -luuid -lrt

TARGET = ../Target/update
TEMPLATE = app

HEADERS += \
    UI/mainwindow.h \
    Utilities/sysconfig.h \
    Utilities/includes.h \
    UI/ui.h \
    Application/cdatacontroller.h \
    Application/thread.h \
    Application/cserialcontrolthread.h \
    Application/fdset.h \
    Devices/cserialcontrol.h \
    Devices/cmodulebase.h \
    Devices/caiomodule.h \
    Devices/ckeymodule.h \
    Devices/cbckmodule.h \
    Devices/crecordermodule.h \
    Devices/cserialdevice.h \
    Utilities/canalysisdata.h \
    Utilities/typedefs.h \
    Devices/cdataframe.h \
    Devices/cserial.h \
    Utilities/autolock.h \
    Utilities/templateclass.h \
    Devices/cserialanalysis.h \
    Devices/cserialaioanalysis.h \
    Devices/cserialkeyanalysis.h \
    Devices/cserialbckanalysis.h \
    Devices/cserialrecorderanalysis.h \
    Application/cinterrupt.h \
    Utilities/ctimer.h \
    Utilities/csystemdatetime.h \
    Utilities/cmyconsole.h \
    Devices/cupdatemodule.h \
    Utilities/cfolderfile.h \
    Utilities/cutilities.h \
    Application/cupdatefile.h

FORMS += \
    UI/mainwindow.ui

SOURCES += \
    UI/mainwindow.cpp \
    main.cpp \
    Application/cdatacontroller.cpp \
    Application/thread.cpp \
    Application/cserialcontrolthread.cpp \
    Application/fdset.cpp \
    Devices/cserialcontrol.cpp \
    Devices/cmodulebase.cpp \
    Devices/caiomodule.cpp \
    Devices/ckeymodule.cpp \
    Devices/cbckmodule.cpp \
    Devices/crecordermodule.cpp \
    Devices/cserialdevice.cpp \
    Utilities/canalysisdata.cpp \
    Devices/cdataframe.cpp \
    Devices/cserial.cpp \
    Utilities/autolock.cpp \
    Utilities/templateclass.cpp \
    Devices/cserialanalysis.cpp \
    Devices/cserialaioanalysis.cpp \
    Devices/cserialkeyanalysis.cpp \
    Devices/cserialbckanalysis.cpp \
    Devices/cserialrecorderanalysis.cpp \
    Application/cinterrupt.cpp \
    Utilities/ctimer.cpp \
    Utilities/csystemdatetime.cpp \
    Utilities/cmyconsole.cpp \
    Devices/cupdatemodule.cpp \
    Utilities/cfolderfile.cpp \
    Utilities/cutilities.cpp \
    Application/cupdatefile.cpp

RESOURCES += \
    ../Resource/update.qrc


