#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(Lab1)
FILE(GLOB lab1_headers code/*.h)
FILE(GLOB lab1_sources code/*.cc)

SET(files_lab1 ${lab1_headers} ${lab1_sources})
SOURCE_GROUP("lab1" FILES ${files_lab1})

ADD_EXECUTABLE(lab1 ${files_lab1})
TARGET_LINK_LIBRARIES(lab1 core render graphics rasterizer)
ADD_DEPENDENCIES(lab1 core render graphics rasterizer)

#ADD_CUSTOM_COMMAND(
#    TARGET lab1 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/res/
#    ${CMAKE_SOURCE_DIR}/bin/res/
#)

SET_TARGET_PROPERTIES(lab1 PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
