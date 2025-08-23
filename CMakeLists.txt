cmake_minimum_required(VERSION 3.10)

project(RelativisticRaytracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# require path to imgui, glfw
function(require_var VARNAME)
	if (NOT DEFINED ${VARNAME})
		if (DEFINED ENV{${VARNAME}})
			set(${VARNAME} $ENV{${VARNAME}} PARENT_SCOPE)
		else()
			message(FATAL_ERROR "${VARNAME} not defined")
		endif()
	endif()
endfunction()
require_var(IMGUI_DIR)
require_var(GLFW_DIR)
file(TO_CMAKE_PATH "${IMGUI_DIR}" IMGUI_DIR)
file(TO_CMAKE_PATH "${GLFW_DIR}" GLFW_DIR)

include_directories(
	${IMGUI_DIR}
	${GLFW_DIR}/include
)

set(SOURCES 
	src/main.cpp
)

set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(${GLFW_DIR} glfw_build)

add_executable(${PROJECT_NAME}
	${SOURCES}
	${IMGUI_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
	glfw
	${OPENGL_gl_LIBRARY}
)