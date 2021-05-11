#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(Physics)
FILE(GLOB physics_headers code/*.h)
FILE(GLOB physics_sources code/*.cc)

SET(files_physics ${physics_headers} ${physics_sources})
SOURCE_GROUP("physics" FILES ${files_physics})

ADD_LIBRARY(physics STATIC ${files_physics})
TARGET_LINK_LIBRARIES(physics core render graphics math)
ADD_DEPENDENCIES(physics core render graphics math)

TARGET_INCLUDE_DIRECTORIES(physics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
