cmake_minimum_required (VERSION 3.28)

project ("unitTests" LANGUAGES CXX)

message(CMaking: ${PROJECT_NAME})

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
	PRIVATE
		src/test_myClass.cpp
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_ROOT}/templateApp/src
)

# Link the test executable with GoogleTest libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
		templateApp_sources # Sources to be unittested
		gtest         # Core GoogleTest library
		gtest_main    # Optional, provides a default main() for tests
)
