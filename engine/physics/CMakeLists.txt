#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_physics
	phys.h
	phys.cc)
SOURCE_GROUP("physics" FILES ${files_physics})
	
ADD_LIBRARY(physics STATIC ${files_physics} ${files_pch})
TARGET_LINK_LIBRARIES(physics PUBLIC engine exts glew)
