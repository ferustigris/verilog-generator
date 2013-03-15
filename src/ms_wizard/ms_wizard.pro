# -------------------------------------------------
# Project created by QtCreator 2010-02-10T14:25:49
# -------------------------------------------------
QT += xml
TARGET = fsm_wizard
win32 {
    TARGET = ../../../bin/fsm_wizard
}
TEMPLATE = app
INCLUDEPATH += sources/graph \
    sources/forms \
    sources/generate \
    sources \
    ../ms_struct/sources/widgets \
    sources/log
SOURCES += main.cpp \
    enterinputs.cpp \
    sources/forms/inputmodel.cpp \
    sources/forms/outputmodel.cpp \
    sources/forms/inputdelegate.cpp \
    sources/forms/outputdelegate.cpp \
    sources/log/errors.cpp \
    sources/forms/statemodel.cpp \
    sources/forms/statedelegate.cpp \
    sources/generate/generator.cpp \ # sources/forms/includestatemodel.cpp \
    sources/forms/controlstatemodel.cpp \
    sources/graph/graphbody.cpp \
    sources/graph/node.cpp \
    sources/graph/edge.cpp \
    sources/forms/moduleslist.cpp \
    sources/graph/edgeparams.cpp \
    sources/generate/xmlproject.cpp \
    sources/state.cpp \
    sources/relation.cpp \
    sources/data.cpp \
    sources/output.cpp \
    sources/input.cpp \
    sources/control.cpp \
    sources/forms/controldelegate.cpp \
    sources/forms/tdialogadditem.cpp \
    ../ms_struct/sources/widgets/identificatoredit.cpp
HEADERS += enterinputs.h \
    sources/forms/inputmodel.h \
    sources/forms/outputmodel.h \
    sources/forms/inputdelegate.h \
    sources/forms/outputdelegate.h \
    sources/log/errors.h \
    sources/forms/statemodel.h \
    sources/forms/statedelegate.h \
    sources/generate/generator.h \ # sources/forms/includestatemodel.h \
    sources/forms/controlstatemodel.h \
    sources/graph/graphbody.h \
    sources/graph/node.h \
    sources/graph/edge.h \
    sources/forms/moduleslist.h \
    sources/graph/edgeparams.h \
    sources/generate/xmlproject.h \
    sources/relation.h \
    sources/data.h \
    sources/state.h \
    sources/output.h \
    sources/input.h \
    sources/control.h \
    sources/forms/controldelegate.h \
    sources/forms/tdialogadditem.h \
    ../ms_struct/sources/widgets/identificatoredit.h
FORMS += enterinputs.ui \
    sources/forms/generator.ui \
    sources/graph/graphbody.ui \
    sources/graph/edgeparams.ui \
    sources/forms/tdialogadditem.ui
RESOURCES += share/mainResources.qrc
TRANSLATIONS = share/lang/tr_ru.ts
OTHER_FILES += share/lang/menu.ts \
    sources/test/test.pro
SUBDIRS = sources/test
