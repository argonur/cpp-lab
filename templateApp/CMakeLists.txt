cmake_minimum_required (VERSION 3.28)

project ("templateApp" LANGUAGES CXX)

message(CMaking: ${PROJECT_NAME})

#######################################################################
#   STATIC library with the files that will be unit tested            #
#######################################################################
add_library(${PROJECT_NAME}_sources STATIC)

target_sources(${PROJECT_NAME}_sources
    PRIVATE
        src/myClass.cpp
)

target_include_directories(${PROJECT_NAME}_sources
    PRIVATE
        ${PROJECT_ROOT}/templateApp/src
)

#######################################################################
#                      templateApp executable                         #
#######################################################################
add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_ROOT}/templateApp/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_sources
)

add_subdirectory(unittests)