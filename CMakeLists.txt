cmake_minimum_required(VERSION 3.1)

project(TLike)

include_directories(src)
include_directories(src/Server)

file(GLOB_RECURSE CLIENT_CPP src/*.cpp)
file(GLOB_RECURSE CLIENT_H src/*.h)
file(GLOB_RECURSE SERVER_CPP src/Server/*.cpp)
file(GLOB_RECURSE SERVER_H src/*.h)

set(SOURCE_FILES ${CLIENT_CPP} ${SERVER_CPP})
set(HEADER_FILES ${CLIENT_H} ${SERVER_H})

set(SFML_DIR "C:/SFML-2.5.1/msvc1/lib/cmake/SFML")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/bin/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)
add_executable(TLike ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(TLike sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

source_group("Client" FILES ${CLIENT_CPP} ${CLIENT_H})
source_group("Server" FILES ${SERVER_CPP} ${SERVER_H})
