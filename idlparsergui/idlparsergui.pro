#-------------------------------------------------
#
# Project created by QtCreator 2014-04-02T22:02:59
#
#-------------------------------------------------
QT       += widgets
QT       += core gui xml


TARGET = idlparsergui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ekit.cpp \
    emodel.cpp \
    ediagram.cpp \
    ebox.cpp \
    earrowseg.cpp \
    enote.cpp \
    idlparser.cpp \
    eobject.cpp \
    edge.cpp \
    node.cpp \
    gedge.cpp \
    graphwidget.cpp \
    graphdialog.cpp

HEADERS  += mainwindow.h \
    ekit.h \
    emodel.h \
    ediagram.h \
    ebox.h \
    earrowseg.h \
    enote.h \
    idlparser.h \
    eobject.h \
    edge.h \
    gedge.h \
    node.h \
    graphwidget.h \
    graphdialog.h

FORMS    += mainwindow.ui \
    graphdialog.ui
