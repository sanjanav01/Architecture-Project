# imgsoa/CMakeLists.txt

# Define the imagesoa library without directly including helpers.cpp
add_library(imagesoa STATIC imagesoa.cpp)
target_include_directories(imagesoa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the helpers library to imagesoa
target_link_libraries(imagesoa PUBLIC helpers)


