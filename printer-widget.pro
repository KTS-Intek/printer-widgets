INCLUDEPATH  += $$PWD\
                $$PWD/../../defines/defines\
                $$PWD/../../defines/define-types

include(../printer-shared/printer-shared.pri)

HEADERS += \
    $$PWD/src/printer/exportwdgt2qrcodegen.h \
    $$PWD/src/printer/myprintpreviewdlg.h

SOURCES += \
    $$PWD/src/printer/exportwdgt2qrcodegen.cpp \
    $$PWD/src/printer/myprintpreviewdlg.cpp
