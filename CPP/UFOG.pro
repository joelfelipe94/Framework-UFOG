
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
    gradients1prior.cpp



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
    gradients1prior.h

OTHER_FILES +=  ./softcudamain.cu \
                ./kernels.cu



# CUDA settings <-- may change depending on your system
CUDA_SOURCES += ./softcudamain.cu



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
    -lopencv_flann



CUDA_SDK = "/usr/local/cuda-7.5"   # Path to cuda SDK install
CUDA_DIR = "/usr/local/cuda-7.5"            # Path to cuda toolkit install

# DO NOT EDIT BEYOND THIS UNLESS YOU KNOW WHAT YOU ARE DOING....

SYSTEM_NAME = unix         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64            # '32' or '64', depending on your system
CUDA_ARCH = sm_20           # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math


# include paths
INCLUDEPATH += $$CUDA_DIR/include

# library directories
QMAKE_LIBDIR += $$CUDA_DIR/lib64

CUDA_OBJECTS_DIR = ./


# Add the necessary libraries
##CUDA_LIBS =  `pkg-config opencv  --libs`
CUDA_LIBS =  -lcuda -lcudart

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
#LIBS += $$join(CUDA_LIBS,'.so ', '', '.so')
LIBS += $$CUDA_LIBS

# Configuration of the Cuda compiler
CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}














