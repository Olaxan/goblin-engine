#--------------------------------------------------------------------------
# VectorLib project
#--------------------------------------------------------------------------

PROJECT(ECS)
FILE(GLOB ecs_headers code/*.h)
FILE(GLOB ecs_sources code/*.cc)

SET(files_ecs ${ecs_headers} ${ecs_sources})
SOURCE_GROUP("ecs" FILES ${files_ecs})

ADD_LIBRARY(ecs STATIC ${files_ecs})
TARGET_LINK_LIBRARIES(ecs core render)
ADD_DEPENDENCIES(ecs core render)


TARGET_INCLUDE_DIRECTORIES(ecs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/code/)
