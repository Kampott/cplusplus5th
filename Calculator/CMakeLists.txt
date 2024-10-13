cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

set(PLUGIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plugins")
message(STATUS "Plugin directory: ${PLUGIN_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY ${PLUGIN_DIR} DESTINATION "${CMAKE_BINARY_DIR}/bin")