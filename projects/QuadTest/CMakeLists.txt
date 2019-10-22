#--------------------------------------------------------------------------
# QuadTest project
#--------------------------------------------------------------------------

PROJECT(QuadTest)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("quadtest" FILES ${files_example})

ADD_EXECUTABLE(QuadTest ${files_example})
TARGET_LINK_LIBRARIES(QuadTest core render MeshResource Rasterizer)
ADD_DEPENDENCIES(QuadTest core render MeshResource Rasterizer)

ADD_CUSTOM_COMMAND(
    TARGET QuadTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res/
    ${CMAKE_SOURCE_DIR}/bin/res/
)

SET_TARGET_PROPERTIES(QuadTest PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)