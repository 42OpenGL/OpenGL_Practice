cmake_minimum_required(VERSION 3.13)

message(STATUS ModelGL)

project(
	openGL_wrapper
)
	
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/")
	
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/inc/glad")

set(SOURCE_FILES
	src/glad.c
	src/main.cpp
)

add_executable(ModelGL ${SOURCE_FILES})

target_include_directories(ModelGL
	PUBLIC
	inc
	${GLAD_DIR}
	${GLFW_DIR}/include
	${ASSIMP_DIR}/include
)

target_link_libraries(ModelGL
	glfw assimp ${CMAKE_DL_LIBS}
)

target_compile_definitions(ModelGL
	PRIVATE
	ROOT_PATH="${CMAKE_SOURCE_DIR}/"
	SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/"
	MODEL_PATH="${CMAKE_SOURCE_DIR}/resource/model/"
)
