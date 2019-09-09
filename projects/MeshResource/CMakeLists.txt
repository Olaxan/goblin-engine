#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(MeshResource)
FILE(GLOB meshresource_headers code/*.h)
FILE(GLOB meshresource_sources code/*.cc)

SET(files_meshresource ${meshresource_headers} ${meshresource_sources})
SOURCE_GROUP("meshresource" FILES ${files_meshresource})

ADD_EXECUTABLE(MeshResource ${files_meshresource})
TARGET_LINK_LIBRARIES(MeshResource core render)
ADD_DEPENDENCIES(MeshResource core render)