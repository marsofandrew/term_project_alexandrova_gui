#-------------------------------------------------
#
# Project created by QtCreator 2018-11-25T14:07:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        mainwindow.cpp \
    code/BufferImpl.cpp \
    code/GeneratorImpl.cpp \
    code/ProcessorImpl.cpp \
    code/ProcessorPoolImpl.cpp \
    code/SupportiveFunctions.cpp \
    library/src/Condition.cpp \
    library/src/defineStaticVars.cpp \
    library/src/Order.cpp \
    library/src/SimpleGeneratorPool.cpp \
    library/src/SimpleTimer.cpp \
    library/src/Timer.cpp \
    library/src/Worker.cpp \
    fullsimulationlogger.cpp
    #stepbystepsimulationlogger.cpp

HEADERS += \
        mainwindow.hpp \
    code/BufferImpl.hpp \
    code/GeneratorImpl.hpp \
    code/ProcessorImpl.hpp \
    code/ProcessorPoolImpl.hpp \
    code/SupportiveFunctions.hpp \
    library/include/interfaces/Buffer.hpp \
    library/include/interfaces/Generator.hpp \
    library/include/interfaces/GeneratorPool.hpp \
    library/include/interfaces/Logger.hpp \
    library/include/interfaces/Processor.hpp \
    library/include/interfaces/ProcessorPool.hpp \
    library/include/interfaces/TimeDependsObject.hpp \
    library/include/interfaces/Timer.hpp \
    library/include/interfaces/WorkCondition.hpp \
    library/include/Condition.hpp \
    library/include/Order.hpp \
    library/include/SimpleGeneratorPool.hpp \
    library/include/SimpleTimer.hpp \
    library/include/Worker.hpp \
    fullsimulationlogger.hpp
    #stepbystepsimulationlogger.hpp \

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
