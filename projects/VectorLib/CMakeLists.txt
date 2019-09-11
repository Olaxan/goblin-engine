#--------------------------------------------------------------------------
# VectorLib project
#--------------------------------------------------------------------------

PROJECT(VectorLib)
FILE(GLOB vectorlib_headers code/*.h)
FILE(GLOB vectorlib_sources code/*.cc)

SET(files_vectorlib ${vectorlib_headers} ${vectorlib_sources})
SOURCE_GROUP("vectorlib" FILES ${files_vectorlib})

ADD_LIBRARY(VectorLib STATIC ${files_vectorlib})
TARGET_LINK_LIBRARIES(VectorLib core render)
ADD_DEPENDENCIES(VectorLib core render)

TARGET_INCLUDE_DIRECTORIES(VectorLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)