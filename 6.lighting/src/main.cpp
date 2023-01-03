#include <iostream>
#include <unistd.h> // for usleep
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Cube.hpp"

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL * GL = OpenGL::getInstance();
	GL->SetShader("vert.vs", "frag.fs");
	GL->SetCamera(glm::vec3(0,0,2), glm::vec3(0,0,0), glm::vec3(0,1,0));


	std::cout << C_BOLD << "[MAIN] : Loop starts" << C_RESET << std::endl;
	glEnable(GL_DEPTH_TEST);
	GLuint shader = GL->GetShaderID();
	const Camera * camera = GL->GetCamera();

	Cube cube("cube_vert.vs", "cube_frag.fs", 1.0f, GL_FILL);

	glm::mat4 projection;
	glm::vec3 color;
	while (GL->OK())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        //glm::mat4 view = camera->lookAt();

		//GLuint projection_location = glGetUniformLocation(shader, "projection");
		//glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

		//GLuint view_location = glGetUniformLocation(shader, "view");
		//glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

		//glUseProgram(shader);

		cube.Draw(projection, camera->lookAt());

		glfwSwapBuffers(GL->GetWindow()->Id());
		glfwPollEvents(); 
	}
}

int main() {
	try	{ 
		main_process();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

struct Vertex {
	glm::vec3 position;
	Vertex(float x, float y, float z) : position(x,y,z) {};
	Vertex(const glm::vec3& v) : position(v) {};
};
