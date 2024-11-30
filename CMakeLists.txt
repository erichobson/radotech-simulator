cmake_minimum_required(VERSION 3.16)
project(RaDoTech)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/ui
    ${PROJECT_SOURCE_DIR}/include/controllers
    ${PROJECT_SOURCE_DIR}/include/models
    ${PROJECT_SOURCE_DIR}/include/utils
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "resources/resources.qrc")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(RaDoTech ${SOURCES} ${HEADERS})
target_link_libraries(RaDoTech PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql)
