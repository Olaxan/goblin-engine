#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(QuadTest)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("quadtest" FILES ${files_example})

ADD_EXECUTABLE(QuadTest ${files_example})
TARGET_LINK_LIBRARIES(QuadTest core render MeshResource)
ADD_DEPENDENCIES(QuadTest core render MeshResource)


