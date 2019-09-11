#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(MeshResource)
FILE(GLOB meshresource_headers code/*.h)
FILE(GLOB meshresource_sources code/*.cc)

SET(files_meshresource ${meshresource_headers} ${meshresource_sources})
SOURCE_GROUP("meshresource" FILES ${files_meshresource})

ADD_LIBRARY(MeshResource STATIC ${files_meshresource})
TARGET_LINK_LIBRARIES(MeshResource core render VectorLib)
ADD_DEPENDENCIES(MeshResource core render VectorLib)

TARGET_INCLUDE_DIRECTORIES(MeshResource INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)