#TODO: assimp,등등 큰 라이브러리 한곳에 두고 컴파일 되게 하기. window, mac  둘다 되는지 확인하기
#TODO: 왜 build 파일에 glfw-3.3.8 이 또 생기는지 체크하기 내생각엔 add_subdirectory를 파보면 될듯?
cmake_minimum_required(VERSION 3.13)

message(STATUS 42OpenGL group study )

project( ft_GL )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
#set(CMAKE_CXX_FLAGS "-g3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/")
# message(${CMAKE_HOME_DIRECTORY})
set(ROOT_DIR "${CMAKE_SOURCE_DIR}/..")
set(LIBRARY_DIR "${ROOT_DIR}/libraries")
set(GLM_DIR "${LIBRARY_DIR}/glm")
set(STB_IMAGE_DIR "${LIBRARY_DIR}/stb_image")
set(KHR_DIR "${LIBRARY_DIR}/KHR")


if (APPLE)
	set(4_DIR "${ROOT_DIR}/4.model_load/")
	set(GLFW_DIR "glfw-3.3.8")
	set(ASSIMP_DIR "${4_DIR}/assimp")
	set(GLAD_DIR "${LIBRARY_DIR}/glad_mac")
	add_subdirectory(${GLFW_DIR})
	add_subdirectory(${ASSIMP_DIR} ${ROOT_DIR})
else ()
	set(GLAD_DIR "${LIBRARY_DIR}/glad")
endif (APPLE)

set(SOURCE_FILES
	${GLAD_DIR}/glad.c
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
	PUBLIC
	inc
	${LIBRARY_DIR}
	${GLAD_DIR}/
	${GLM}/
	${STB_IMAGE_DIR}/
	${KHR_DIR}/
	${GLFW_DIR}/include/
	${ASSIMP_DIR}/include/
)

target_link_libraries(${PROJECT_NAME}
	glfw assimp ${CMAKE_DL_LIBS}
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	ROOT_PATH="${CMAKE_SOURCE_DIR}/"
	SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/"
	MODEL_PATH="${ROOT_DIR}/resource/model/"
)