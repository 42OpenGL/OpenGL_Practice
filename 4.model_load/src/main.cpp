#include <iostream>
#include <unistd.h> // for usleep

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

#define BACKPACK	"backpack/backpack.obj"
#define COIN		"coin/coin.fbx"

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL * GL = OpenGL::getInstance();
	GL->SetShader("model_vert.vs", "model_frag.fs");
	GL->SetCamera(glm::vec3(0,0,20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	Model test_model(BACKPACK);

	std::cout << C_BOLD << "[MAIN] : Loop starts" << C_RESET << std::endl;
	glEnable(GL_DEPTH_TEST);
	GLuint shader = GL->GetShaderID();
	const Camera * camera = GL->GetCamera();
	glm::mat4 projection;
	while (GL->OK())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		GLuint projection_location = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = camera->lookAt();
		GLuint view_location = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

		glUseProgram(shader);
		test_model.Draw(shader);

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