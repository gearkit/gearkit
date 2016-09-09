cmake_minimum_required(VERSION 2.8)

project(GearKit)
set(TARGET_NAME testnet)

set(BUILD_TYPE "server" CACHE STRING "Set Building Type")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS "server;server win;client;client win")

include_directories(${PROJECT_SOURCE_DIR}/buffer)
include_directories(${PROJECT_SOURCE_DIR}/net)

add_subdirectory(buffer)
add_subdirectory(net)

set(APP_SRC main.cpp)

#debug
add_executable(${TARGET_NAME} ${APP_SRC})
target_link_libraries(${TARGET_NAME} libbuffer)
target_link_libraries(${TARGET_NAME} libnet)
