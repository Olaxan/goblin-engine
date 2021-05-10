#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(GLTF)
FILE(GLOB gltf_headers code/*.h)
FILE(GLOB gltf_sources code/*.cc)

SET(files_gltf ${gltf_headers} ${gltf_sources})
SOURCE_GROUP("gltf" FILES ${files_gltf})

ADD_LIBRARY(gltf STATIC ${files_gltf})
TARGET_LINK_LIBRARIES(gltf core render math graphics physics)
ADD_DEPENDENCIES(gltf core render math graphics physics)

TARGET_INCLUDE_DIRECTORIES(gltf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
