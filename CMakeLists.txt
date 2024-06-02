cmake_minimum_required(VERSION 3.10)

# Establecer la política CMP0072 para preferir GLVND
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

project(proyecto)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)

set(SOURCES
    src/main.cpp
   
)

add_executable(proyecto ${SOURCES})

target_link_libraries(proyecto ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# Incluye directorios de encabezados
target_include_directories(proyecto PRIVATE 
    ${CMAKE_SOURCE_DIR}/src    # Directorio principal de código fuente
   
)

