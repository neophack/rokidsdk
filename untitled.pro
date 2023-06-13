TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT -= gui

TARGET = libusb_control_transfer_demo

HEADERS += \
    glass_control.h

# Source files
SOURCES +=  main.cpp \
    glass_control.cpp

# Include directories
# Include directories
INCLUDEPATH += C:/Users/nn/Documents/libusb-1.0.20/include

# Link directories
LIBS += -LC:/Users/nn/Documents/libusb-1.0.20/MinGW64/dll
# Link libraries
LIBS += -lusb-1.0

