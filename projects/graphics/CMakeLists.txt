#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(Graphics)
FILE(GLOB graphics_headers code/*.h)
FILE(GLOB graphics_sources code/*.cc)

SET(files_graphics ${graphics_headers} ${graphics_sources})
SOURCE_GROUP("graphics" FILES ${files_graphics})

ADD_LIBRARY(graphics STATIC ${files_graphics})
TARGET_LINK_LIBRARIES(graphics core render math)
ADD_DEPENDENCIES(graphics core render math)

TARGET_INCLUDE_DIRECTORIES(graphics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
