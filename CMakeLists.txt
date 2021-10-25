cmake_minimum_required(VERSION 3.10)
project(Triangles VERSION 1.0.0)

set(TRIAN ${PROJECT_NAME})


add_executable(${TRIAN} ./src/Geom/vector.cc main.cc)
target_compile_features(${TRIAN} PRIVATE cxx_std_20)
target_include_directories(${TRIAN} PRIVATE ./include)
