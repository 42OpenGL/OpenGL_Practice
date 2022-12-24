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
	OpenGL * GL = OpenGL::getInstance();
	GL->SetShader("vert.vs", "frag.fs");
	GL->SetCamera();

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
	GLuint shader = GL->GetShaderID();
	const Camera * camera = GL->GetCamera();
	glm::mat4 projection;
	while (GL->OK())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		GLuint projection_location = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = camera->lookAt();
		GLuint view_location = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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