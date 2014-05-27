#-------------------------------------------------
#
# Project created by QtCreator 2013-04-13T18:21:37
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Melosimula
TEMPLATE = app


SOURCES += main.cpp \
        melosimula.cpp \
        Datos/simDatos.cpp \
        ProcesadorGL/simOSG.cpp \
        ProcesadorODE/simODE.cpp \
        Serpiente/simSerpiente.cpp \
        Escenario/simEscenario.cpp \
    simulador.cpp \
    viewerwidget.cpp \
    editode.cpp \
    heighfield.cpp \
    snakeparameters.cpp \
    Serpiente/modelos/modelo1.cpp \
    Serpiente/modelos/modelo2.cpp \
    graphicparameters.cpp \
    plugins/anglegenerator.cpp

HEADERS  += melosimula.h \
        ProcesadorGL/simOSG.h \
        ProcesadorGL/constantesOSG.h \
        ProcesadorODE/simODE.h \
        ProcesadorODE/constantesODE.h \
        Serpiente/simSerpiente.h \
        Serpiente/Constantes.h \
        Escenario/simEscenario.h \
        Escenario/Constantes.h \
        Datos/simDatos.h \
    simulador.h \
    viewerwidget.h \
    editode.h \
    heighfield.h \
    snakeparameters.h \
    Serpiente/modelos/form.h \
    Serpiente/modelos/modelo1.h \
    Serpiente/modelos/modelo2.h \
    graphicparameters.h \
    plugins/anglegenerator.h

FORMS    += \
    melosimula.ui \
    viewerwidget.ui \
    editode.ui \
    heighfield.ui \
    snakeparameters.ui \
    Serpiente/modelos/modelo1.ui \
    Serpiente/modelos/modelo2.ui \
    graphicparameters.ui \
    plugins/anglegenerator.ui

LIBS += -lGL
LIBS += -lGLU
LIBS += -lglut
LIBS += -lode
LIBS += -losg
LIBS += -losgDB
LIBS += -losgFX
LIBS += -losgGA
LIBS += -losgParticle
LIBS += -losgSim
LIBS += -losgText
LIBS += -losgUtil
LIBS += -losgTerrain
LIBS += -losgManipulator
LIBS += -losgViewer
LIBS += -losgWidget
LIBS += -losgShadow
LIBS += -losgAnimation
LIBS += -losgVolume
LIBS += -lOpenThreads
LIBS += -losgQt
DEFINES += dDOUBLE

RESOURCES += \
    MelosimulaRF.qrc
