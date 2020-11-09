#--------------------------------------------------------------------------
# VectorLib project
#--------------------------------------------------------------------------

PROJECT(Math)
FILE(GLOB math_headers code/*.h)
FILE(GLOB math_sources code/*.cc)

SET(files_math ${math_headers} ${math_sources})
SOURCE_GROUP("math" FILES ${files_math})

ADD_LIBRARY(math STATIC ${files_math})
TARGET_LINK_LIBRARIES(math core render)
ADD_DEPENDENCIES(math core render)

TARGET_INCLUDE_DIRECTORIES(math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
