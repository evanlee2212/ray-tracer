include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable (glfwExample
  glfwExample.cpp 
  GLSL.cpp GLSL.h
        Objects/Mesh.cpp
        Objects/Mesh.h
        Objects/Sphere.cpp
        Objects/Sphere.h
        Objects/Trimesh.cpp
        Objects/Trimesh.h
        Objects/Camera.cpp
        Objects/Camera.h
        Objects/Texture.cpp
        Objects/Texture.h
)

target_link_libraries (glfwExample PRIVATE GLEW::GLEW)
target_link_libraries (glfwExample PRIVATE glfw)

if(APPLE)
    target_link_libraries(glfwExample PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(glfwExample PRIVATE OpenGL::GL)
endif()

target_link_libraries (glfwExample PRIVATE PNG::PNG)
target_link_libraries (glfwExample PRIVATE ZLIB::ZLIB)

add_executable( silverLevel
        silverLever.cpp
        GLSL.cpp GLSL.h
)

target_link_libraries(silverLevel
        PRIVATE GLEW::GLEW glfw OpenGL::GL PNG::PNG ZLIB::ZLIB
)

add_executable( textureCube
        Objects/textureCube.cpp
        GLSL.cpp GLSL.h
)

target_link_libraries(textureCube
        PRIVATE GLEW::GLEW glfw OpenGL::GL PNG::PNG ZLIB::ZLIB
)

add_executable(fbExample fbExample.cpp
            GLSL.cpp GLSL.h
        Objects/Mesh.cpp
        Objects/Mesh.h
        Objects/Sphere.cpp
        Objects/Sphere.h
        Objects/Trimesh.cpp
        Objects/Trimesh.h
        Objects/Camera.cpp
        Objects/Camera.h
        Objects/Texture.cpp
        Objects/Texture.h
        Objects/Light.h
)

target_link_libraries(fbExample
        PRIVATE GLEW::GLEW glfw OpenGL::GL PNG::PNG ZLIB::ZLIB)