# imgsoa/CMakeLists.txt

add_library(imageaos STATIC imageaos.cpp)

# Include the directory so other targets can find imageaos.hpp
target_include_directories(imageaos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
