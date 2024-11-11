# imgsoa/CMakeLists.txt

add_library(imagesoa STATIC imagesoa.cpp)

# Include the directory so other targets can find imagesoa.hpp
target_include_directories(imagesoa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
