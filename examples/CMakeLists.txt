# 
# Adds the src directory to the include search
# path. ${CMAKE_SOURCE_DIR} is prefixed to allow the files in the
# utest directory to correctly include the relative reference to the
# src files.
#

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test_pngWrite
  test_pngWrite.cpp
)

target_compile_definitions(test_pngWrite PRIVATE
        __BYTE_ORDER=1234
        __LITTLE_ENDIAN=1234
)

target_link_libraries(test_pngWrite cs4212-util)
target_link_libraries(test_pngWrite ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_pngWrite PNG::PNG)
target_link_libraries(test_pngWrite ZLIB::ZLIB)

add_executable(test_readPNGFile
  test_readPNGFile.cpp
)
target_link_libraries(test_readPNGFile cs4212-util)
target_link_libraries(test_readPNGFile ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_readPNGFile PNG::PNG)
target_link_libraries(test_readPNGFile ZLIB::ZLIB)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

add_library(graphicsLib
        ../graphicsLib/Framebuffer.cpp
        ../graphicsLib/Framebuffer.h
        ../graphicsLib/ray.h
        ../graphicsLib/Camera.h
        ../graphicsLib/PerspectiveCamera.h
        ../graphicsLib/Shape.h
        ../graphicsLib/Sphere.h
        ../graphicsLib/Scene.h
        ../graphicsLib/Shader.h
        ../graphicsLib/LambertianShader.h
        ../graphicsLib/HitStructure.h
)

target_link_libraries(graphicsLib PUBLIC PNG::PNG ZLIB::ZLIB)

add_executable(fb.cpp fb.cpp)

target_link_libraries(fb.cpp
        PRIVATE
        graphicsLib
        cs4212-util
        ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)


