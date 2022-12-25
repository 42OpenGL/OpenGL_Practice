#ifndef MESH_HPP
# define MESH_HPP

# include <iostream>
# include <vector>

# include <glad.h>
# include <GLFW/glfw3.h>
# include <glm/glm.hpp>

class Mesh
{
public:
	struct Vertex;
	typedef std::vector<Vertex> vertices_type;
	typedef std::vector<GLuint> indices_type;
private:
	GLuint VAO_, VBO_, EBO_;
	vertices_type vertices_;
	indices_type indices_;
public:
	Mesh(const vertices_type &vertices, const indices_type &indices);
	~Mesh() {};
	void Draw() const;
private:
	void setupMesh();
};

struct Mesh::Vertex
{
	glm::vec3 position;
};

Mesh::Mesh(const vertices_type &vertices, const indices_type &indices)
: VAO_(0), VBO_(0), EBO_(0), vertices_(vertices), indices_(indices)
{
	setupMesh();
}

// TODO : 이후에는 Shader객체를 매개변수로 받아서 material 정보 설정 가능해야함.
void Mesh::Draw() const 
{
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 
}

#endif