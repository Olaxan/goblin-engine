#--------------------------------------------------------------------------
# Rasterizer project
#--------------------------------------------------------------------------

PROJECT(Rasterizer)
FILE(GLOB rasterizer_headers code/*.h)
FILE(GLOB rasterizer_sources code/*.cc)

SET(files_rasterizer ${rasterizer_headers} ${rasterizer_sources})
SOURCE_GROUP("rasterizer" FILES ${files_rasterizer})

ADD_LIBRARY(rasterizer ${files_rasterizer})
TARGET_LINK_LIBRARIES(rasterizer core render graphics math)
ADD_DEPENDENCIES(rasterizer core render graphics math)

TARGET_INCLUDE_DIRECTORIES(rasterizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
