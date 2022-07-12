cmake_minimum_required(VERSION 3.10)

project(aspen)

add_subdirectory(extern)

set(SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/include/complex.h"
    "${PROJECT_SOURCE_DIR}/src/include/compute.h"
    "${PROJECT_SOURCE_DIR}/src/include/shader.h"
    "${PROJECT_SOURCE_DIR}/src/include/utils.h"
    "${PROJECT_SOURCE_DIR}/src/include/window.h"
)

set(LIBS glfw3 glad)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_SOURCE_DIR}/src/include"
    PUBLIC "${PROJECT_SOURCE_DIR}/extern/include"
)

target_link_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_SOURCE_DIR}/extern/lib"
)