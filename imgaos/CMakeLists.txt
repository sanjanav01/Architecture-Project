# imgsoa/CMakeLists.txt

add_library(imgaos STATIC imageaos.cpp
        imageaos.hpp)

# Include the directory so other targets can find imageaos.hpp
target_include_directories(imgaos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})