QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Snake/Snake.cxx \
    main.cxx \
    MainWindow.cxx

HEADERS += \
    MainWindow.h \
    Snake/Snake.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Snake/LICENSE \
    Snake/apple.png \
    Snake/dot.png \
    Snake/head.png \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml
android: include(/home/dexter/Android/Sdk/android_openssl/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES = images.qrc
