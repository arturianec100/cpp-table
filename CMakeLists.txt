cmake_minimum_required(VERSION 3.5)

project(cpp-table LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

list(APPEND TS_FILES
    translations/cpp-table_ru_RU.ts
)
list(APPEND HEADERS
    src/typedefs.h
    src/macro.h
    src/parseresult.h
    src/hwparser.h
    src/parser.hpp
    src/overloads.h
    src/tablemodel.h
    src/tabletools.h
    src/mainwindow.h
)
list(APPEND SOURCES
    src/hwparser.cpp
    src/overloads.cpp
    src/tablemodel.cpp
    src/tabletools.cpp
    src/mainwindow.cpp
    src/main.cpp
)
list(APPEND UI_FILES
    src/tabletools.ui
    src/mainwindow.ui
)

add_executable(cpp-table
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
    ${TS_FILES}
)

target_link_libraries(cpp-table PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
