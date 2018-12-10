#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T15:16:53
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eshop-qt-2017
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    product.cpp \
    account.cpp \
    imageitemdelegate.cpp \
    paydialog.cpp \
    binddialog.cpp \
    querydialog.cpp \
    categorytablemodel.cpp

HEADERS += \
        mainwindow.h \
    product.h \
    account.h \
    imageitemdelegate.h \
    paydialog.h \
    binddialog.h \
    querydialog.h \
    categorytablemodel.h

FORMS += \
        mainwindow.ui \
    paydialog.ui \
    binddialog.ui \
    querydialog.ui

RESOURCES += \
    res/res.qrc

RC_ICONS = eshop.ico
