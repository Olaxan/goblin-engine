#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(App)
FILE(GLOB app_headers code/*.h)
FILE(GLOB app_sources code/*.cc)

SET(files_app ${app_headers} ${app_sources})
SOURCE_GROUP("app" FILES ${files_app})

ADD_EXECUTABLE(app ${files_app})
TARGET_LINK_LIBRARIES(app core ecs render graphics physics gltf)
ADD_DEPENDENCIES(app core ecs render graphics physics gltf)

SET_TARGET_PROPERTIES(app PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
