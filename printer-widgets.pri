INCLUDEPATH  += $$PWD\
                $$PWD/../../defines/defines\
                $$PWD/../../defines/define-types

include(../printer-shared/printer-shared.pri)
include(../../../QrCode/qzxing-v2/src/QZXing.pri)

HEADERS += \
    $$PWD/src/printer/exportwdgt2qrcodegen.h \
    $$PWD/src/printer/myprintpreviewdlg.h \
    $$PWD/src/printer/printerhelper.h \
    $$PWD/print-pgs/previewimagewdgt.h \
    $$PWD/print-pgs/printersetupwdgt.h \
    $$PWD/print-pgs/printniqrcodewdgt.h \
    $$PWD/print-pgs/ucprintlabel.h \
    $$PWD/template-pgs/qrexportwdgt.h

SOURCES += \
    $$PWD/src/printer/exportwdgt2qrcodegen.cpp \
    $$PWD/src/printer/myprintpreviewdlg.cpp \
    $$PWD/src/printer/printerhelper.cpp \
    $$PWD/print-pgs/previewimagewdgt.cpp \
    $$PWD/print-pgs/printersetupwdgt.cpp \
    $$PWD/print-pgs/printniqrcodewdgt.cpp \
    $$PWD/print-pgs/ucprintlabel.cpp \
    $$PWD/template-pgs/qrexportwdgt.cpp

FORMS += \
    $$PWD/print-pgs/previewimagewdgt.ui \
    $$PWD/print-pgs/printersetupwdgt.ui \
    $$PWD/print-pgs/printniqrcodewdgt.ui \
    $$PWD/print-pgs/ucprintlabel.ui \
    $$PWD/template-pgs/qrexportwdgt.ui
