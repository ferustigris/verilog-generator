# -------------------------------------------------
# Project created by QtCreator 2010-02-10T14:25:49
# -------------------------------------------------
QT += webkit \
    xml
win32 {
    TARGET = ../../../bin/master_struct
}
linux-g++ {
    TARGET = ../../bin/master_struct
}
TEMPLATE = app
INCLUDEPATH += sources/graph \
    sources/forms \
    sources/generator \
    sources/help \
    sources/project \
    sources/widgets \
    sources \
    sources/log
SOURCES += main.cpp \
    enterinputs.cpp \
    sources/log/errors.cpp \
    sources/graph/graphbody.cpp \
    sources/graph/node.cpp \
    sources/graph/edge.cpp \
    sources/generator/generator.cpp \
    sources/graph/edgeparams.cpp \
    sources/help/textedit.cpp \
    sources/help/tdialogadditem.cpp \
    sources/project/project.cpp \
    sources/wiremodel.cpp \
    sources/graph/nodefsm.cpp \
    sources/graph/nodedev.cpp \
    sources/graph/nodesfactory.cpp \
    sources/graph/rootnode.cpp \
    sources/widgets/identificatoredit.cpp
HEADERS += enterinputs.h \
    sources/log/errors.h \
    sources/graph/graphbody.h \
    sources/graph/node.h \
    sources/graph/edge.h \
    sources/graph/inode.h \
    sources/generator/generator.h \
    sources/graph/edgeparams.h \
    sources/graph/iedge.h \
    sources/help/textedit.h \
    sources/help/tdialogadditem.h \
    sources/constants.h \
    sources/project/project.h \
    sources/wiremodel.h \
    sources/graph/nodefsm.h \
    sources/graph/nodedev.h \
    sources/graph/nodesfactory.h \
    sources/graph/rootnode.h \
    sources/widgets/identificatoredit.h
FORMS += enterinputs.ui \
    sources/graph/graphbody.ui \
    sources/generator/generator.ui \
    sources/graph/edgeparams.ui \
    sources/help/tdialogadditem.ui
RESOURCES += share/mainResources.qrc
TRANSLATIONS = share/lang/tr_ru.ts
OTHER_FILES += share/lang/menu.ts
