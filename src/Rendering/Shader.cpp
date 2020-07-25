#include "Shader.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>

#include <GL/glew.h>

namespace glGame {

	Shader::Shader(const char* filepath) {
		std::unordered_map<int, std::string> shaderSources = getShaderSourcesFromFile(std::string(filepath));

		std::unordered_map<int, unsigned int> shaderIds;
		for(auto& shaderSource : shaderSources) {
			shaderIds[shaderSource.first] = createShader(shaderSource.first, shaderSource.second);
		}
		
		m_shaderProgramID = createShaderProgram(shaderIds[GL_VERTEX_SHADER], shaderIds[GL_FRAGMENT_SHADER]);
		
		for(auto& shaderId : shaderIds) deleteShader(shaderId.second);
	}

	void Shader::useShader() {
		glUseProgram(m_shaderProgramID);
	}

	void Shader::setUniform3f(const char* name, float v1, float v2, float v3) {
		int uniformLocation = getUniformLocation(name);
		glUniform3f(uniformLocation, v1, v2, v3);
	}

	void Shader::setUniformMat4(const char* name, const float* matrix) {
		int uniformLocation = getUniformLocation(name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrix);
	}

	void Shader::setUniformBlockBinding(const char* name, const unsigned int& bindingPoint) {
		glUniformBlockBinding(m_shaderProgramID, getUniformBlockIndex(name), bindingPoint);
	}

	std::unordered_map<int, std::string> Shader::getShaderSourcesFromFile(const std::string& filepath) {
		std::unordered_map<int, std::string> shaderSources;

		std::fstream filestream(filepath, std::ios::in | std::ios::binary);
		if(!filestream.is_open()) {
			std::cout << "Could not open file " << filepath << std::endl;
			return shaderSources;
		}
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		filestream.close();

		std::string file = buffer.str();
		const char* typeToken = "#type";
		size_t typeTokenLength = std::strlen(typeToken);
		size_t pos = file.find(typeToken, 0);
		while(pos != std::string::npos) {
			size_t eol = file.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string typeStr = file.substr(begin, eol - begin);
			size_t nextLinePos = file.find_first_of("\r\n", eol);
			pos = file.find(typeToken, nextLinePos);

			shaderSources[getShaderTypeFromString(typeStr)] = (pos == std::string::npos) ? file.substr(nextLinePos) : file.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	int Shader::getShaderTypeFromString(const std::string& typeString) {
		if(typeString == "vertex") return GL_VERTEX_SHADER;
		if(typeString == "fragment") return GL_FRAGMENT_SHADER;
		else return 0;
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

	int Shader::getUniformBlockIndex(const char* name) {
		auto search = m_uniformBlockIndicies.find(name);
		if(search != m_uniformBlockIndicies.end()) {
			return search->second;
		}
		
		int uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgramID, name);
		m_uniformBlockIndicies.insert(std::pair<const char*, int>(name, uniformBlockIndex));

		return uniformBlockIndex;
	}

}