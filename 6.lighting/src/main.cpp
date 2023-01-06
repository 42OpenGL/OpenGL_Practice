#include <iostream>
#include <unistd.h> // for usleep
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Cube.hpp"

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL * const GL = OpenGL::getInstance();
	GL->SetCamera(glm::vec3(3,-1,2), glm::vec3(0,0,0), glm::vec3(0,0,1));
	const Camera * const camera = GL->GetCamera();
	Cube cube("cube_vert.vs", "cube_frag.fs", 1, Cube::BOTH);
	cube.SetColor(glm::vec4(1,1,1,1));

	std::cout << (int)(Cube::LINE & Cube::LINE) << std::endl;
	std::cout << (int)(Cube::LINE & Cube::FILL) << std::endl;

	glm::mat4 projection;
	glm::vec3 color;
	glEnable(GL_DEPTH_TEST);
	std::cout << C_BOLD << "[MAIN] : Loop starts" << C_RESET << std::endl;
	while (GL->OK())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view = camera->lookAt();

		cube.Draw(projection, view);

		glfwSwapBuffers(GL->GetWindow()->Id());
		glfwPollEvents(); 
	}
	//OpenGL::clearInstance();
}

int main() {
	try	{ 
		main_process();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
