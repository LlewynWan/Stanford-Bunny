#include <shader.h>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& fileName)
{
	ID = glCreateProgram();
	m_shaders[0] = Shader::CreateShader(Shader::LoadShader(fileName+".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = Shader::CreateShader(Shader::LoadShader(fileName+".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(ID, m_shaders[i]);

	glLinkProgram(ID);
	
	Shader::CheckShaderError(ID, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glValidateProgram(ID);
	Shader::CheckShaderError(ID, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	ID = glCreateProgram();
	m_shaders[0] = Shader::CreateShader(Shader::LoadShader(vertexPath), GL_VERTEX_SHADER);
	m_shaders[1] = Shader::CreateShader(Shader::LoadShader(fragmentPath), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(ID, m_shaders[i]);

	glBindAttribLocation(ID, 0, "position");
	glLinkProgram(ID);
	
	Shader::CheckShaderError(ID, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glValidateProgram(ID);
	Shader::CheckShaderError(ID, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(ID, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

GLuint Shader::CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;
;
	const GLchar* shaderCode = text.c_str();

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
		while (file.good())
		{
			getline(file, line);
			output.append(line+'\n');
		}
	else
		std::cerr << "Unable to load shader: " << fileName << std::endl;

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = {0};

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (!success)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glm::vec3 v(x,y,z);
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}

void Shader::setVec3(const std::string &name, glm::vec3 v) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
