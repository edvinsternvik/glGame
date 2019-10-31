#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

namespace glGame {

	Shader::Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath) {
		std::string vertexShaderSource = getShaderStringFromFile(vertexShaderFilepath);
		unsigned int vertexShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource);

		std::string fragmentShaderSource = getShaderStringFromFile(fragmentShaderFilepath);
		unsigned int fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
		
		m_shaderProgramID = createShaderProgram(vertexShaderId, fragmentShaderId);
		deleteShader(vertexShaderId);
		deleteShader(fragmentShaderId);
	}

	void Shader::useShader() {
		glUseProgram(m_shaderProgramID);
	}

	void Shader::setUniform3f(const char* name, float v1, float v2, float v3) {
		int uniformLocation = getUniformLocation(name);
		glUniform3f(uniformLocation, v1, v2, v3);
	}

	std::string Shader::getShaderStringFromFile(std::string& filepath) {
		std::fstream filestream(filepath, std::ios::in | std::ios::binary);
		if(!filestream.is_open()) {
			std::cout << "Could not open file " << filepath << std::endl;
			return "";
		}
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		return buffer.str();
	}

	unsigned int Shader::createShader(unsigned int shaderType, std::string& shaderSource) {
		unsigned int shaderID = glCreateShader(shaderType);

		const char* shaderSourceCstr = shaderSource.c_str();
		glShaderSource(shaderID, 1, &shaderSourceCstr, NULL);
		glCompileShader(shaderID);

		int compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus != GL_TRUE) {
			char shaderLogBuffer[512];
			int logLen;
			glGetShaderInfoLog(shaderID, 512, &logLen, shaderLogBuffer);
			std::cout << "Shader Compile ERROR: " << shaderLogBuffer << std::endl;
		}

		return shaderID;
	}

	void Shader::deleteShader(unsigned int shaderID) {
		glDeleteShader(shaderID);
	}

	unsigned int Shader::createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
		unsigned int programID = glCreateProgram();

		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);

		glLinkProgram(programID);
		int program_linked;
		glGetProgramiv(programID, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar message[512];
			glGetProgramInfoLog(programID, 512, &log_length, message);
			std::cout << "Shader Program Link ERROR: " << message << std::endl;
		}

		return programID;
	}

	int Shader::getUniformLocation(const char* name) {
		auto search = m_uniformLocations.find(name);
		if(search != m_uniformLocations.end()) {
			return search->second;
		}

		int uniformLocation = glGetUniformLocation(m_shaderProgramID, name);
		m_uniformLocations.insert(std::pair<const char*, int>(name, uniformLocation));

		return uniformLocation;
	}

}