cmake_minimum_required(VERSION 3.10)
project(ESP32)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(unofficial-graphicsmagick CONFIG REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include")
include_directories("${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include/Magick++")

link_directories("${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/lib")

add_executable(ESP32 main.cpp)

target_link_libraries(ESP32 PRIVATE unofficial::graphicsmagick::graphicsmagick)
