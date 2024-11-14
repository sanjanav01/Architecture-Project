add_library(imgsoa
        imagesoa.cpp
        imagesoa.hpp
)
target_include_directories(imgsoa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Use this line only if you have dependencies from this library to GSL
target_link_libraries(imgsoa PRIVATE Microsoft.GSL::GSL)

# Link the helpers library to imagesoa
target_link_libraries(imgsoa PUBLIC helpers)


