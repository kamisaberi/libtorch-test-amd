cmake_minimum_required(VERSION 3.28)
project(libtorch_test)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PREFIX_PATH /home/kami/libtorch/)

find_package(BLAS REQUIRED)
if(BLAS_FOUND)
    message("BLAS found: ${BLAS_LIBRARIES}")
    list(APPEND TEST_LIBRARIES ${BLAS_LIBRARIES})
endif()

find_package(Torch REQUIRED)
add_executable(libtorch_test main.cpp)
target_link_libraries(libtorch_test "${TORCH_LIBRARIES}")



