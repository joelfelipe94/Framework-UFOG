
QT       += core

QT       -= gui

TARGET = QtCuda
CONFIG   += con



SOURCES += main.cpp \
    filter.cpp \
    l0smoothing.cpp \
    common.cpp \
    signalprior.cpp \
    darkchannelprior.cpp \
    colorconstancy.cpp \
    standartDeviation.cpp \
    restoration.cpp \
    chromaticprior.cpp \
    dcpgb.cpp \
    colorlines.cpp \
    standartdeviationprior.cpp \
    rangefiltprior.cpp \
    gradients1prior.cpp \
    redchannelprior.cpp \
    softmatting.cpp



HEADERS += \
    filter.h \
    l0smoothing.h \
    common.h \
    signalprior.h \
    darkchannelprior.h \
    colorconstancy.h \
    standartDeviation.h \
    bilateralfilter.h \
    restoration.h \
    chromaticprior.h \
    dcpgb.h \
    colorlines.h \
    standartdeviationprior.h \
    rangefiltprior.h \
    gradients1prior.h \
    redchannelprior.h \
    softmatting.h



INCLUDEPATH += /usr/local/include/opencv


LIBS += -L/usr/local/lib \
     -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_ml \
    -lopencv_video \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_objdetect \
    -lopencv_contrib \
    -lopencv_legacy \
    -lopencv_flann \
    -lnlopt





