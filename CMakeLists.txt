cmake_minimum_required(VERSION 3.26)
project(test_cpp_file_enc)

set(CMAKE_CXX_STANDARD 17)
include_directories(/opt/homebrew/include)
add_executable(test_cpp_file_enc main.cpp
        src/FileEncryptor.cpp
        include/FileEncryptor.h
)
