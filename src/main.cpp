#include <iostream>
#include <unistd.h> // for usleep

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"
#include "Shader.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL GL;
	Shader shader(std::string(SHADER_PATH)+"vert.vs", std::string(SHADER_PATH)+"frag.fs");

	float vertices[] = {
		0.25f,  0.5f, 0.0f,	1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.25f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 


	std::cout << C_BOLD << "[MAIN] : Loop starts" << C_RESET << std::endl;
	while (GL.OK())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.Id());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(GL.GetWindow()->Id());
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