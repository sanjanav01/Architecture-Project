# imgaos/CMakeLists.txt

# Define the imageaos library
add_library(imagesoa STATIC imagesoa.cpp)
target_include_directories(imagesoa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link both helpers and common libraries to imageaos
target_link_libraries(imagesoa PUBLIC helpers common)  # Add common here
