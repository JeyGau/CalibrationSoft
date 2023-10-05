cmake_minimum_required(VERSION 3.14)

project(CalibrationSoft VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/libs)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Test)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# tests
add_executable(CalibrationSoftTests
    assets.qrc
    tests/calibration_tests.cpp
)

target_link_libraries(CalibrationSoftTests
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Quick
            Qt${QT_VERSION_MAJOR}::Test
            ${OpenCV_LIBS}
)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CalibrationSoft
        MANUAL_FINALIZATION
        assets.qrc
        ${PROJECT_SOURCES}
    )

else()
    add_executable(CalibrationSoft
      assets.qrc
      ${PROJECT_SOURCES}
    )
endif()

target_compile_definitions(CalibrationSoft
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(CalibrationSoft
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Test ${OpenCV_LIBS})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(CalibrationSoft)
    qt_finalize_executable(CalibrationSoft)
endif()