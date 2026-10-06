cmake_minimum_required(VERSION 3.31.6)
project(FileCloneCleanup VERSION 1.0.0 LANGUAGES C CXX)

set(EXECUTABLE_NAME FileCloneCleanup)
set(SHA_LIBRARY_NAME shasum)
set(FS_LIBRARY_NAME walker)

add_subdirectory(ext)
add_subdirectory(src)

add_executable(${EXECUTABLE_NAME} "main.cpp")
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${SHA_LIBRARY_NAME} ${FS_LIBRARY_NAME} CLI11::CLI11)
