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
	struct Texture;
	typedef std::vector<Vertex> vertices_type;
	typedef std::vector<GLuint> indices_type;
	typedef std::vector<Texture> textures_type;
private:
	GLuint VAO_, VBO_, EBO_;
	vertices_type vertices_;
	indices_type indices_;
	textures_type textures_;
public:
	Mesh(const vertices_type &vertices, const indices_type &indices, const textures_type &textures);
	~Mesh() {};
	void Draw( GLuint shader_id ) const;
private:
	void setupMesh();
};

struct Mesh::Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
};

struct Mesh::Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

Mesh::Mesh(const vertices_type &vertices, const indices_type &indices, const textures_type &textures)
: VAO_(0), VBO_(0), EBO_(0), vertices_(vertices), indices_(indices), textures_(textures)
{
	setupMesh();
}

// TODO : 이후에는 Shader객체를 매개변수로 받아서 material 정보 설정 가능해야함.
void Mesh::Draw(GLuint shader_id) const 
{
	unsigned int diffuseNr  = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr   = 1;
	unsigned int heightNr   = 1;
	for(unsigned int i = 0; i < textures_.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures_[i].type;
		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if(name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if(name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string
		else
			continue ;
		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader_id, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures_[i].id);
	}
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coord)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 
}

#endif