QT += core
QT -= gui

CONFIG += c++11

TARGET = test_vector
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


INCLUDEPATH += /usr/local/include/
LIBS += -lopencv_video -lopencv_flann -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
LIBS += -L/usr/lib/x86_64-linux-gnu -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lboost_date_time -lboost_log -lboost_log_setup
LIBS += -L/usr/lib/x86_64-linux-gnu -ljsoncpp -lcurl

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
