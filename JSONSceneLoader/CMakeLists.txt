cmake_minimum_required(VERSION 3.22)

# --------------------------
# Project setup
# --------------------------
project(sceneLoaderExample VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------
# vcpkg toolchain
# --------------------------
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# --------------------------
# Fetch Catch2 for testing
# --------------------------
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.0
)
FetchContent_MakeAvailable(Catch2)

# --------------------------
# Find packages via vcpkg
# --------------------------
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PNG REQUIRED)

# --------------------------
# SceneLoaderTest executable
# --------------------------
add_executable(sceneLoaderTest
        ISceneLoader.h
        SceneLoader.h
        SceneParser_JSON.cpp
        SceneParser_JSON.h
        sceneLoaderTest.cpp
)

target_link_libraries(sceneLoaderTest PRIVATE
        glm::glm-header-only
        nlohmann_json::nlohmann_json
        PNG::PNG
        graphicsLib
)

# --------------------------
# Test executable for Catch2
# --------------------------
add_executable(test_sceneParsing_JSON
        SceneParser_JSON.cpp
        SceneParser_JSON.h
        test_sceneParsing_JSON.cpp
)

target_link_libraries(test_sceneParsing_JSON PRIVATE
        Catch2::Catch2WithMain
        nlohmann_json::nlohmann_json
        PNG::PNG
        graphicsLib
)

# Enable Catch2 test discovery
include(CTest)
include(Catch)
catch_discover_tests(test_sceneParsing_JSON)