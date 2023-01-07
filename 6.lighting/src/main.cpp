#include <iostream>
#include <unistd.h> // for usleep
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Cube.hpp"
#include "Model.hpp"

#define BACKPACK	"backpack/backpack.obj"
#define COIN		"coin/coin.fbx"
#define CYBORG		"cyborg/cyborg.obj"

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL * const GL = OpenGL::getInstance();
	GL->SetCamera(glm::vec3(0,6,6), glm::vec3(0,0,0), glm::vec3(0,1,0));
	const Camera * const camera = GL->GetCamera();
	
	Cube cube("cube_vert.vs", "cube_frag.fs", 1.3, Cube::FILL);
	cube.SetColor(glm::vec4(0.32,0.24,0.21,1));
	
	Model test_model(CYBORG, "model_vert.vs", "model_frag.fs", true);

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
		test_model.Draw(glm::vec3(-1,1,1), glm::vec3(1.0f), projection, view);

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
