#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(App)
FILE(GLOB app_headers code/*.h)
FILE(GLOB app_sources code/*.cc)

SET(files_app ${app_headers} ${app_sources})
SOURCE_GROUP("app" FILES ${files_app})

ADD_EXECUTABLE(app ${files_app})
TARGET_LINK_LIBRARIES(app core render graphics physics gltf)
ADD_DEPENDENCIES(app core render graphics physics gltf)

#ADD_CUSTOM_COMMAND(
#    TARGET app POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/res/
#    ${CMAKE_SOURCE_DIR}/bin/res/
#)

SET_TARGET_PROPERTIES(app PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
