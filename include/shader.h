#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint ID;
	Shader(const std::string& fileName);
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	static std::string LoadShader(const std::string& fileName);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 mat) const;
	void setVec3(const std::string &name, glm::vec3 v) const;
	void setVec3(const std::string &name, float x, float y, float z) const;

	virtual ~Shader();
private:
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shaders[NUM_SHADERS];
};

#endif
