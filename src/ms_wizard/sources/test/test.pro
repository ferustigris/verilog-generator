QT += xml
CONFIG += qtestlib
win32 {
    TARGET = ../../../../../bin/test.exe
}
HEADERS += test_data.h \
    ../data.h \
    ../relation.h \
    ../state.h \
    ../forms/statemodel.h \
    ../log/errors.h \
    ../forms/controlstatemodel.h \
    ../forms/inputmodel.h \
    ../input.h \
    ../forms/outputmodel.h \
    ../output.h \
    ../control.h \
    ../forms/statedelegate.h \
    ../forms/inputdelegate.h \
    ../forms/outputdelegate.h \
    ../forms/controldelegate.h \
    ../generate/xmlproject.h \
    ../generate/generator.h \
    testgenerator.h \
    ../../../ms_struct/sources/widgets/identificatoredit.h
SOURCES += test_data.cpp \
    ../data.cpp \
    ../relation.cpp \
    ../state.cpp \
    ../forms/statemodel.cpp \
    ../log/errors.cpp \
    ../forms/controlstatemodel.cpp \
    ../forms/inputmodel.cpp \
    ../input.cpp \
    ../forms/outputmodel.cpp \
    ../output.cpp \
    ../control.cpp \
    ../forms/statedelegate.cpp \
    ../forms/inputdelegate.cpp \
    ../forms/outputdelegate.cpp \
    ../forms/controldelegate.cpp \
    ../generate/xmlproject.cpp \
    ../generate/generator.cpp \
    testgenerator.cpp \
    ../../../ms_struct/sources/widgets/identificatoredit.cpp
TARGET = test
INCLUDEPATH += .. \
    ../generate \
    ../forms/ \
    ../../../ms_struct/sources/widgets \
    ../log
FORMS += ../forms/generator.ui
