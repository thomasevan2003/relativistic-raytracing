cmake_minimum_required(VERSION 3.10)

project(RelativisticRaytracing LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/third-party/imgui")
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/third-party/glfw")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/third-party/glad")

include_directories(
	${IMGUI_DIR}
	${GLFW_DIR}/include
	${GLAD_DIR}/include
	${OPENGL_INCLUDE_DIRS}
)


set(SOURCES 
	src/main.cpp
	src/Graphics_Manager.cpp
	src/file_to_string.cpp
	src/create_gpu_program.cpp
	src/GUI.cpp
	src/stb_image.c
	src/hsv_to_rgb.cpp
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
	${GLAD_DIR}/src/gl.c
)

add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/glfw glfw)

add_executable(${PROJECT_NAME}
	${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
	glfw
	${OPENGL_gl_LIBRARY}
)

# place executable in top level of build directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL   "${CMAKE_BINARY_DIR}"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})