cmake_minimum_required(VERSION 3.10)
project(YaxkinNuclear)

set(CMAKE_CXX_STANDARD 17)

# Buscar la librería Eigen
find_package(Eigen3 REQUIRED)

include_directories(src/core/include)

set(SOURCES 
    src/core/engine/main.cpp 
    src/core/engine/ReactorCore.cpp
)

# Crear el ejecutable
add_executable(yaxkin_core ${SOURCES})
target_link_libraries(yaxkin_core Eigen3::Eigen)