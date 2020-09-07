#--------------------------------------------------------------------------
# exts
#--------------------------------------------------------------------------

ADD_LIBRARY(exts INTERFACE)

ADD_SUBDIRECTORY(glew)
SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "exts/glew")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glew/include)

SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")		  

ADD_SUBDIRECTORY(glfw)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "exts/glfw")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glfw/include)

ADD_LIBRARY(stb STATIC stb/stb.cc stb/stb_image.h stb/stb_image_write.h stb/stb_rect_pack.h stb/stb_textedit.h stb/stb_truetype.h)
TARGET_LINK_LIBRARIES(stb PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(stb PUBLIC stb)
SET_TARGET_PROPERTIES(stb PROPERTIES FOLDER "exts/stb")

#ADD_LIBRARY(nanovg STATIC nanovg/src/nanovg.c nanovg/src/nanovg.h nanovg/src/nanovg_gl.h nanovg/src/nanovg_gl_utils.h)
#TARGET_LINK_LIBRARIES(nanovg PUBLIC exts)
#TARGET_INCLUDE_DIRECTORIES(nanovg PUBLIC nanovg/src)
#SET_TARGET_PROPERTIES(nanovg PROPERTIES FOLDER "exts/nanovg")

ADD_LIBRARY(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp 
			imgui/imgui.h imgui/imgui_internal.h 
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)

TARGET_LINK_LIBRARIES(imgui PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(imgui PUBLIC imgui imgui/examples/opengl3_example)
SET_TARGET_PROPERTIES(imgui PROPERTIES FOLDER "exts/imgui")

ADD_LIBRARY(tinygltf STATIC tinygltf/tiny_gltf.cc tinygltf/tiny_gltf.h)
TARGET_LINK_LIBRARIES(tinygltf PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(tinygltf PUBLIC tinygltf)
SET_TARGET_PROPERTIES(tinygltf PROPERTIES FOLDER "exts/tinygltf")

TARGET_LINK_LIBRARIES(exts INTERFACE stb imgui tinygltf)
