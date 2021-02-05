cmake_minimum_required(VERSION 3.5)

project(Skeleton LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport Core Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport Core Gui REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
include_directories(/usr/include/qt5/QtPrintSupport)

set(PROJECT_SOURCES
        main.cxx
        MainWindow.cxx
        MainWindow.h
        Syntaxhighlighter.cxx
        Syntaxhighlighter.h
)

qt5_add_resources(PROJECT_SOURCES images.qrc stylesheet.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Skeleton
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Skeleton SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Skeleton
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Skeleton PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Skeleton PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(Skeleton PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Skeleton PRIVATE Qt${QT_VERSION_MAJOR}::Gui)