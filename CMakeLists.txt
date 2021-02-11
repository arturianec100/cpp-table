cmake_minimum_required(VERSION 3.5)

project(cpp-table LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(SOURCES_DIR src)
set(TS_FILES_DIR translations)

include(${SOURCES_DIR}/files_list.cmake)
include(${TS_FILES_DIR}/files_list.cmake)

add_executable(cpp-table
    ${SOURCES}
    ${TS_FILES}
)

target_link_libraries(cpp-table PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
