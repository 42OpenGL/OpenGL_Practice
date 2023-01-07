#ifndef SHADER_HPP
# define SHADER_HPP

# include <iostream>
# include <fstream>
# include <sstream>

# include <glad.h>

# ifndef SHADER_PATH
#  define SHADER_PATH "../shaders/"
# endif
class Shader
{
private :
	GLuint shader_prog_;
public:
	Shader(const std::string &vs, const std::string &fs);
	~Shader();
	GLuint Id() const { return shader_prog_; }
private:
	std::string FileLoader_(const std::string &file) const;
	GLuint LoadShaderfile_(const std::string & file, GLenum type) const;
};

Shader::Shader(const std::string &vs, const std::string &fs)
: shader_prog_(glCreateProgram())
{
	int success = 0;
	char infoLog[1024];
	GLuint vs_id = LoadShaderfile_(vs, GL_VERTEX_SHADER);
	GLuint fs_id = LoadShaderfile_(fs, GL_FRAGMENT_SHADER);

	glAttachShader(shader_prog_, vs_id);
	glAttachShader(shader_prog_, fs_id);
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);

	glLinkProgram(shader_prog_);
	glGetProgramiv(shader_prog_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_prog_, 512, NULL, infoLog);
		std::string err_msg = "[Shader] : link failed.\n\t" + std::string(infoLog);
		throw std::runtime_error(err_msg);
	}
	std::cout << "shader created : " << shader_prog_ << std::endl;
}

Shader::~Shader()
{
	if (shader_prog_) {
		glDeleteProgram(shader_prog_);
	}
	std::cout << C_YELLOW << "Closing Shader" << C_RESET << std::endl;
};

std::string Shader::FileLoader_(const std::string &file) const
{
	std::ifstream ifs(file);
	if (!ifs.is_open())
	{
		std::string err_msg = "[Shader] : fail to open (" + file + ")";
		throw std::runtime_error(err_msg);
	}
	std::stringstream ss;
	ss << ifs.rdbuf();
	ifs.close();
	return ss.str();
}

GLuint Shader::LoadShaderfile_(const std::string &file, GLenum type) const
{
	GLuint shader_id = glCreateShader(type); // 셰이더 오브젝트 생성
	std::string shader_source = FileLoader_( std::string(SHADER_PATH) + file);
	const char* shader_source_ptr = shader_source.data();
	glShaderSource(shader_id, 1, &shader_source_ptr, nullptr); // 셰이더 소스파일 로드
	glCompileShader(shader_id); // 셰이더 컴파일
	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success); // 잘 됐는지 확인
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader_id, 512, nullptr, log); // 로그 확인
		throw std::runtime_error(std::string("Error: Failed to compile vertex shader:\n") + log);
	}
	return shader_id;
}

#endif