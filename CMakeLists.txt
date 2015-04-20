cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(TLike)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(src)
include_directories(src/Server)

file(GLOB_RECURSE CLIENT_CPP src/*.cpp)
file(GLOB_RECURSE CLIENT_H src/*.h)
file(GLOB_RECURSE SERVER_CPP src/Server/*.cpp)
file(GLOB_RECURSE SERVER_H src/*.h)

set(SOURCE_FILES ${CLIENT_CPP} ${SERVER_CPP})
set(HEADER_FILES ${CLIENT_H} ${SERVER_H})

add_executable(TLike ${SOURCE_FILES} ${HEADER_FILES} ${SFML_LIBRARIES})

set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SFML")
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML/include")
set(SFML_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/SFML/lib")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/bin/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

source_group("Client" FILES ${CLIENT_CPP} ${CLIENT_H})
source_group("Server" FILES ${SERVER_CPP} ${SERVER_H})
