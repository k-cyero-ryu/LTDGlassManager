QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checkglasspackreport.cpp \
    connection.cpp \
    databasecontroller.cpp \
    datecargarordenes.cpp \
    frmmesas.cpp \
    main.cpp \
    frmprincipal.cpp \
    newlamesdiag.cpp \
    newretaildiag.cpp \
    qtableviewprinter.cpp \
    reportconsumo.cpp \
    reportsuspand.cpp \
    viewmozo.cpp \
    viewretailslames.cpp

HEADERS += \
    checkglasspackreport.h \
    connection.h \
    databasecontroller.h \
    datecargarordenes.h \
    frmmesas.h \
    frmprincipal.h \
    newlamesdiag.h \
    newretaildiag.h \
    qtableviewprinter.h \
    reportconsumo.h \
    reportsuspand.h \
    viewmozo.h \
    viewretailslames.h

FORMS += \
    checkglasspackreport.ui \
    datecargarordenes.ui \
    frmmesas.ui \
    frmprincipal.ui \
    newlamesdiag.ui \
    newretaildiag.ui \
    reportconsumo.ui \
    reportsuspand.ui \
    viewmozo.ui \
    viewretailslames.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc


RC_ICONS = Icon.ico
