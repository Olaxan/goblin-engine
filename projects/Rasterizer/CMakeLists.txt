#--------------------------------------------------------------------------
# Rasterizer project
#--------------------------------------------------------------------------

PROJECT(Rasterizer)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("rasterizer" FILES ${files_example})

ADD_LIBRARY(Rasterizer ${files_example})
TARGET_LINK_LIBRARIES(Rasterizer core render MeshResource VectorLib)
ADD_DEPENDENCIES(Rasterizer core render MeshResource VectorLib)