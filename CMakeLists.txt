cmake_minimum_required (VERSION 3.28)

project(CppTemplate)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(3rdParty/googletest)
add_subdirectory(templateApp)
