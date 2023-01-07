#ifndef MODEL_HPP
# define MODEL_HPP

# include <vector>

# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>

# include "Mesh.hpp"
# include "Shader.hpp"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
	typedef std::vector<Mesh> meshes_type;
private:
	Shader shader_;
	GLuint VAO_, VBO_, EBO_;
	meshes_type meshes_;
	Mesh::textures_type loaded_textures_;
	std::string directory_;
public:
	Model(const std::string &filename, 
		  const std::string &vertex_shader,
		  const std::string &fragment_shader,
		  bool is_stbi_flip = true);
	~Model() {}
	void Draw(const glm::mat4 &projection = glm::mat4(1.0),
			  const glm::mat4 &view = glm::mat4(1.0),
			  const glm::mat4 &model = glm::mat4(1.0)) const;
	void Draw(glm::vec3 light_pos, glm::vec3 light_color, 
					   const glm::mat4 &projection = glm::mat4(1.0),
					   const glm::mat4 &view = glm::mat4(1.0),
					   const glm::mat4 &model = glm::mat4(1.0)) const;

private:
	void LoadModel_(const std::string & filename);
	void ProcessNode_(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh_(aiMesh *mesh, const aiScene *scene);
	Mesh::textures_type loadMaterialTextures_(aiMaterial *mat, aiTextureType type, std::string typeName);

};

/**************/
/*   Member   */
/**************/

Model::Model(const std::string &filename,
			 const std::string &vertex_shader,
			 const std::string &fragment_shader,
			 bool is_stbi_flip)
: shader_(vertex_shader, fragment_shader),
  VAO_(0), VBO_(0), EBO_(0),
//  meshes_(0),
  loaded_textures_(), directory_("")
{
	stbi_set_flip_vertically_on_load(is_stbi_flip);
	LoadModel_( std::string(MODEL_PATH) + filename);
}

void Model::Draw(const glm::mat4 &projection,
				 const glm::mat4 &view,
				 const glm::mat4 &model) const
{
	GLuint shader_id = shader_.Id();
	glUseProgram(shader_id);
	GLuint projection_location = glGetUniformLocation(shader_id, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint view_location = glGetUniformLocation(shader_id, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	GLuint model_location = glGetUniformLocation(shader_id, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO_);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	meshes_type::const_iterator it = meshes_.begin();
	for (; it != meshes_.end(); it++)
		it->Draw(shader_id);

	glBindVertexArray(0);
}

void Model::Draw(glm::vec3 light_pos, glm::vec3 light_color,
						  const glm::mat4 &projection,
				 		  const glm::mat4 &view,
				 		  const glm::mat4 &model) const
{
	GLuint shader_id = shader_.Id();
	glUseProgram(shader_id);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLuint projection_location = glGetUniformLocation(shader_id, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint view_location = glGetUniformLocation(shader_id, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	GLuint model_location = glGetUniformLocation(shader_id, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	GLuint light_pos_location = glGetUniformLocation(shader_id, "lightPos");
	glUniform3fv(light_pos_location, 1, glm::value_ptr(light_pos));

	GLuint light_color_location = glGetUniformLocation(shader_id, "lightColor");
	glUniform3fv(light_color_location, 1, glm::value_ptr(light_color));

	glBindVertexArray(VAO_);
	
	meshes_type::const_iterator it = meshes_.begin();
	for (; it != meshes_.end(); it++)
		it->Draw(shader_id);

	glBindVertexArray(0);
}

void Model::LoadModel_(const std::string & filename)
{
	Assimp::Importer importer;
	unsigned int import_flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
	const aiScene* scene = importer.ReadFile(filename, import_flag);
	// check for errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		throw std::runtime_error("[Model] : Assimp Error. " + std::string(importer.GetErrorString()));
	}
	// retrieve the directory path of the filepath
	directory_ = filename.substr(0, filename.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode_(scene->mRootNode, scene);
}

void Model::ProcessNode_(aiNode *node, const aiScene *scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(ProcessMesh_(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode_(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh_(aiMesh *mesh, const aiScene *scene)
{
	Mesh::vertices_type vertices;
	Mesh::indices_type indices;
	Mesh::textures_type textures;

	// walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		// texture coordinates
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coord = vec;
			//// tangent
			//vector.x = mesh->mTangents[i].x;
			//vector.y = mesh->mTangents[i].y;
			//vector.z = mesh->mTangents[i].z;
			//vertex.Tangent = vector;
			//// bitangent
			//vector.x = mesh->mBitangents[i].x;
			//vector.y = mesh->mBitangents[i].y;
			//vector.z = mesh->mBitangents[i].z;
			//vertex.Bitangent = vector;
		}
		else
			vertex.tex_coord = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);        
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	Mesh::textures_type diffuseMaps = loadMaterialTextures_(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	//Mesh::vertices_type specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//// 3. normal maps
	//Mesh::vertices_type normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//Mesh::vertices_type heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	
	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

Mesh::textures_type Model::loadMaterialTextures_(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Mesh::Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for(unsigned int j = 0; j < loaded_textures_.size(); j++)
		{
			if(std::strcmp(loaded_textures_[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_textures_[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if(!skip)
		{   // if texture hasn't been loaded already, load it
			Mesh::Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory_);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loaded_textures_.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

/**************/
/* Non Member */
/**************/

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif