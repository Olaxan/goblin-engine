#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(Lab2)
FILE(GLOB lab2_headers code/*.h)
FILE(GLOB lab2_sources code/*.cc)

SET(files_lab2 ${lab2_headers} ${lab2_sources})
SOURCE_GROUP("lab2" FILES ${files_lab2})

ADD_EXECUTABLE(lab2 ${files_lab2})
TARGET_LINK_LIBRARIES(lab2 core render graphics physics gltf)
ADD_DEPENDENCIES(lab2 core render graphics physics gltf)

#ADD_CUSTOM_COMMAND(
#    TARGET lab2 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/res/
#    ${CMAKE_SOURCE_DIR}/bin/res/
#)

SET_TARGET_PROPERTIES(lab2 PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
