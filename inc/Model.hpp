#ifndef MODEL_HPP
# define MODEL_HPP

# include <vector>
# include <Mesh.hpp>
class Model
{
public:
	typedef std::vector<Mesh> meshes_type;
private:
	meshes_type meshes_;
public:
    Model(const meshes_type &meshes);
	Model(const std::string &filename) : meshes_() {};
    ~Model() {}
	void Draw() const;
};

Model::Model(const meshes_type &meshes)
: meshes_(meshes) {}

void Model::Draw() const
{
	meshes_type::const_iterator it = meshes_.begin();
	for (; it != meshes_.end(); it++)
		it->Draw();
}

#endif