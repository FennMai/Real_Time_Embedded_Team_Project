cmake_minimum_required(VERSION 3.10)
project(SSC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(PCL 1.8 REQUIRED)

# Configure output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/archive)
set(CMAKE_OBJECTS_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/objects)

set(PROJECT_SOURCES
    src/main.cpp
    src/qt/ssc_widget.cpp
    src/qt/ssc_widget.ui
    src/qt/qcustomplot.cpp
    src/utils/PCLDetect.cpp
    include/qt/common.h
    include/qt/ssc_widget.h
    include/qt/qcustomplot.h
    include/utils/PCLDetect.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SSC MANUAL_FINALIZATION ${PROJECT_SOURCES})
elseif(ANDROID)
    add_library(SSC SHARED ${PROJECT_SOURCES})
else()
    add_executable(SSC ${PROJECT_SOURCES})
endif()


include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
target_link_libraries(SSC PRIVATE ${PCL_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR}/include/qt)
include_directories(${PROJECT_SOURCE_DIR}/include/utils)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
target_link_libraries(SSC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)


set_target_properties(SSC PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SSC)
endif()
