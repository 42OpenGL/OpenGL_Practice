#include <iostream>
#include <unistd.h> // for usleep

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL * GL = OpenGL::getInstance();
	GL->SetShader("vert.vs", "frag.fs");
	GL->SetCamera(glm::vec3(3,0,0), glm::vec3(0,0,0), glm::vec3(0,0,1));

	//Model test_mode();
	std::vector<Mesh> meshes = {};
	std::vector<GLuint> indices = {0, 1, 2};

	Mesh::Vertex vert1, vert2, vert3, vert4;
	vert1.position.x =  0.0000f; vert1.position.y =  0.0000f; vert1.position.z =  1.2246f;
	vert2.position.x =  1.1547f; vert2.position.y =  0.0000f; vert2.position.z = -0.4082f;
	vert3.position.x = -0.5773f; vert3.position.y =  1.0000f; vert3.position.z = -0.4082f;
	vert4.position.x = -0.5773f; vert4.position.y = -1.0000f; vert4.position.z = -0.4082f;
	std::vector<Mesh::Vertex> face1 = {vert1, vert2, vert3};
	std::vector<Mesh::Vertex> face2 = {vert1, vert3, vert4};
	std::vector<Mesh::Vertex> face3 = {vert1, vert4, vert2};
	std::vector<Mesh::Vertex> face4 = {vert2, vert3, vert4};
	Mesh mesh1(face1, indices); meshes.push_back(mesh1);
	Mesh mesh2(face2, indices); meshes.push_back(mesh2);
	Mesh mesh3(face3, indices); meshes.push_back(mesh3);
	Mesh mesh4(face4, indices); meshes.push_back(mesh4);

	Model test_model(meshes);

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
		test_model.Draw();

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