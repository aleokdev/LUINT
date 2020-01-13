cmake_minimum_required(VERSION "3.12.18")

set(LUINT_LINK_LIBRARIES
	"${CMAKE_CURRENT_SOURCE_DIR}/lua53.lib"
	PARENT_SCOPE
)

set(LUINT_SOURCES
	${LUINT_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_glfw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_opengl3.cpp"
	PARENT_SCOPE
)

set(LUINT_INCLUDE_DIRECTORIES
	${LUINT_INCLUDE_DIRECTORIES}
	"${CMAKE_CURRENT_SOURCE_DIR}/glad/include/"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples"
	"${CMAKE_CURRENT_SOURCE_DIR}/sol"
	"${CMAKE_CURRENT_SOURCE_DIR}/lua/include"
	PARENT_SCOPE
)