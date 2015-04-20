cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(TLike)

include_directories(src)
include_directories(src/Server)

file(GLOB_RECURSE CLIENT_CPP src/*.cpp)
file(GLOB_RECURSE CLIENT_H src/*.h)
file(GLOB_RECURSE SERVER_CPP src/Server/*.cpp)
file(GLOB_RECURSE SERVER_H src/*.h)

set(SOURCE_FILES ${CLIENT_CPP} ${SERVER_CPP})
set(HEADER_FILES ${CLIENT_H} ${SERVER_H})

add_executable(TLike ${SOURCE_FILES} ${HEADER_FILES})

set(SFML_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/bin/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

source_group("Client" FILES ${CLIENT_CPP} ${CLIENT_H})
source_group("Server" FILES ${SERVER_CPP} ${SERVER_H})
