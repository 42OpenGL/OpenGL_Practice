#ifndef CUBE_HPP
# define CUBE_HPP

# include <vector>
# include <glad.h>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "Shader.hpp"

class Cube
{
public:
	typedef std::vector<glm::vec3> vertices_type;
	typedef std::vector<unsigned int> indices_type;
private:
	Shader shader_;
	const static glm::vec3 face_color_;
	const static glm::vec3 line_color_;
	const static indices_type indices_;
	vertices_type cube_;
	float size_;
	GLuint type_;
	GLuint VAO_, VBO_, EBO_;
	void SetupCube();
public:
	Cube(const std::string &vertex_shader = "",
		 const std::string &fragment_shader = "",
		 float size = 1.0f, GLuint type = GL_FILL);
	void Draw(const glm::mat4 &projection = glm::mat4(1.0),
			  const glm::mat4 &view = glm::mat4(1.0),
			  const glm::mat4 &model = glm::mat4(1.0)) const;
	//Cube(float size = 1.0f, GLuint type = GL_FILL);
	//void Draw(GLuint shader_id);
};


//Cube::Cube(float size, GLuint type)
//: size_(size), type_(type), VAO_(0), VBO_(0), EBO_(0)
Cube::Cube( const std::string &vertex_shader,
			const std::string &fragment_shader,
			float size, GLuint type)
: shader_(vertex_shader, fragment_shader), size_(size), type_(type), VAO_(0), VBO_(0), EBO_(0)
{
	SetupCube();
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	size_t buffer_size = cube_.size() * sizeof(float) * 3;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, cube_.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	buffer_size = indices_.size() * sizeof(unsigned);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, indices_.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
}

void Cube::SetupCube()
{
	float size = size_ / 2;
	cube_ = {
		glm::vec3(-size, -size, -size),
		glm::vec3( size, -size, -size),
		glm::vec3( size,  size, -size),
		glm::vec3(-size,  size, -size),

		glm::vec3(-size, -size, size),
		glm::vec3( size, -size, size),
		glm::vec3( size,  size, size),
		glm::vec3(-size,  size, size),
	};
}

//void Cube::Draw(GLuint shader_id)
void Cube::Draw(const glm::mat4 &projection,
				const glm::mat4 &view,
				const glm::mat4 &model) const
{
	GLuint shader_id = shader_.Id();
	// SetupShader(matrix  3r)
	/*****************************************************************/
	GLuint projection_location = glGetUniformLocation(shader_id, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint view_location = glGetUniformLocation(shader_id, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	GLuint model_location = glGetUniformLocation(shader_id, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
	glUseProgram(shader_id);

	glBindVertexArray(VAO_);
	GLuint color_location = glGetUniformLocation(shader_id, "color");
	glUniform3f(color_location, 0.8, 0.4, 0.2);
	if (type_ == GL_FILL) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, indices_.size() * 3, GL_UNSIGNED_INT, 0);
		glUniform3f(color_location, 0.1, 0.1, 0.1);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

const Cube::indices_type Cube::indices_ = {
	0, 2, 1,   0, 2, 3,
	1, 4, 0,   1, 4, 5,
	1, 2, 6,   1, 5, 6,
	0, 3, 7,   0, 4, 7,
	2, 3, 7,   2, 6, 7,
	4, 5, 6,   4, 7, 6
};

const glm::vec3 Cube::face_color_ = glm::vec3(0.8f, 0.4f, 0.2f);
const glm::vec3 Cube::line_color_ = glm::vec3(0.0f, 0.0f, 0.0f);

#endif
