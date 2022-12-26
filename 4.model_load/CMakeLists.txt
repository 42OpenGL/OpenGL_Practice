cmake_minimum_required(VERSION 3.13)

message(STATUS 42OpenGL group study )

project( ft_GL )
	
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/")
	
set(ROOT_DIR "${CMAKE_SOURCE_DIR}/..")
set(LIBRARY_DIR "${ROOT_DIR}/libraries")
set(GLAD_DIR "${LIBRARY_DIR}/glad")
set(GLM_DIR "${LIBRARY_DIR}/glm")
set(STB_IMAGE_DIR "${LIBRARY_DIR}/stb_image")

set(SOURCE_FILES
	${GLAD_DIR}/glad.c
	#${STB_IMAGE_DIR}/stb_image.cpp
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
	glfw assimp ${CMAKE_DL_LIBS}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	inc
	${GLAD_DIR}/
	${GLM}/
	${STB_IMAGE_DIR}/
	#${ASSIMP_DIR}/include
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	ROOT_PATH="${CMAKE_SOURCE_DIR}/"
	SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/"
	MODEL_PATH="${ROOT_DIR}/resource/model/"
)
