#include "Shader.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iostream>

#include <GL/glew.h>

namespace glGame {

	Shader::Shader(const char* filepath) {
		if(filepath) init(getSourceFromFile(std::string(filepath)));
	}

	Shader::Shader(const char* filepath, const char* shaderSource) {
		if(shaderSource) init(std::string(shaderSource));		
	}

	Shader::~Shader() {

	}

	void Shader::init(const std::string& source) {
		std::unordered_map<std::string, std::string> sections = getShaderSections(source);

		std::unordered_map<int, unsigned int> shaderIds;
		for(auto& section : sections) {
			int shaderType = getShaderTypeFromString(section.first);
			if(shaderType) shaderIds[shaderType] = createShader(shaderType, section.second);
			else if(section.first == "color") updateColorData(section.second);
			else if(section.first == "stencil") updateStencilData(section.second);
			else if(section.first == "depthtest") updateDepthTestData(section.second);
			else std::cout << "Error when creating shader - Section: " << section.first << " is undefined" << std::endl;
		}
		
		m_shaderProgramID = createShaderProgram(shaderIds[GL_VERTEX_SHADER], shaderIds[GL_FRAGMENT_SHADER]);
		
		for(auto& shaderId : shaderIds) deleteShader(shaderId.second);

		setUniformBlockBinding("Camera", 0);
		setUniformBlockBinding("Lights", 1);
	}

	void Shader::useShader() {
		glUseProgram(m_shaderProgramID);
	}

	void Shader::setUniform1i(const char* name, const int& integer) {
		glUniform1i(getUniformLocation(name), integer);
	}

	void Shader::setUniform1f(const char* name, const float& value) {
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::setUniform3f(const char* name, float v1, float v2, float v3) {
		int uniformLocation = getUniformLocation(name);
		glUniform3f(uniformLocation, v1, v2, v3);
	}

	void Shader::setUniformMat4(const char* name, const glm::mat4& matrix) {
		int uniformLocation = getUniformLocation(name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &(matrix[0][0]));
	}

	void Shader::setUniformBlockBinding(const char* name, const unsigned int& bindingPoint) {
		glUniformBlockBinding(m_shaderProgramID, getUniformBlockIndex(name), bindingPoint);
	}

	std::string Shader::getSourceFromFile(const std::string& filepath) {
		std::fstream filestream(filepath, std::ios::in | std::ios::binary);
		if(!filestream.is_open()) {
			std::cout << "Could not open file " << filepath << std::endl;
			return "";
		}
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		filestream.close();
		
		return buffer.str();
	}

	std::unordered_map<std::string, std::string> Shader::getShaderSections(const std::string& shaderSource) {
		std::unordered_map<std::string, std::string> shaderSections;

		const char* sectionToken = "#section";
		size_t sectionTokenLength = std::strlen(sectionToken);
		size_t pos = shaderSource.find(sectionToken, 0);
		while(pos != std::string::npos) {
			size_t eol = shaderSource.find_first_of("\r\n", pos);
			size_t begin = pos + sectionTokenLength + 1;
			std::string secionStr = shaderSource.substr(begin, eol - begin);
			size_t nextLinePos = shaderSource.find_first_of("\r\n", eol);
			pos = shaderSource.find(sectionToken, nextLinePos);

			shaderSections[secionStr] = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSections;
	}

	int Shader::getShaderTypeFromString(const std::string& typeString) {
		if(typeString == "vertex") return GL_VERTEX_SHADER;
		if(typeString == "fragment") return GL_FRAGMENT_SHADER;
		else return 0;
	}

	unsigned int Shader::createShader(unsigned int shaderType, const std::string& shaderSource) {
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

	void Shader::updateColorData(const std::string& str) {
		std::stringstream strbuffer(str);
		std::string line;
		while(std::getline(strbuffer, line)) {
			std::stringstream linebufer(line);
			std::string word;
			std::vector<std::string> words;
			while(std::getline(linebufer, word, ' ')) words.push_back(word);
			if(words.size() < 1) continue;

			if(words.size() == 1 && words[0] == "Enable") m_colorEnabled = true;
			if(words.size() == 1 && words[0] == "Disable") m_colorEnabled = false;
		}
	}

	void Shader::updateStencilData(const std::string& str) {
		std::stringstream strbuffer(str);
		std::string line;
		while(std::getline(strbuffer, line)) {
			std::stringstream linebufer(line);
			std::string word;
			std::vector<std::string> words;
			while(std::getline(linebufer, word, ' ')) words.push_back(word);
			if(words.size() < 1) continue;

			if(words.size() == 1 && words[0] == "Enable") m_stencilEnabled = true;
			else if(words.size() == 1 && words[0] == "Disable") m_stencilEnabled = false;
			else if(words.size() == 2 && words[0] == "StencilWrite") {
				if(words[1] == "Enable") m_stencilWriteEnabled = true;
				else if(words[1] == "Disable") m_stencilWriteEnabled = false;
				else std::cout << "Error when creating shader in section stencil: \"" << words[1] << "\" is not recognized" << std::endl;
			}
			else if(words.size() == 2 && words[0] == "StencilFunc") {
				if(!updateCompareFunc(m_stencilFunc, words[1]))
					std::cout << "Error when creating shader in section stencil: \"" << words[1] << "\" is not recognized" << std::endl;
			}
			else if(words.size() == 2 && words[0] == "StencilFuncRef") m_stencilFuncRef = std::stoi(words[1]);
			else if(words.size() == 2 && words[0] == "StencilOpSFail" || words[0] == "StencilOpDPFail" || words[0] == "StencilOpDPPass") {
				StencilOp stencilOp;
				if(words[1] == "Keep") stencilOp = StencilOp::Keep;
				else if(words[1] == "Zero") stencilOp = StencilOp::Zero;
				else if(words[1] == "Replace") stencilOp = StencilOp::Replace;
				else if(words[1] == "Incr") stencilOp = StencilOp::Incr;
				else if(words[1] == "IncrWrap") stencilOp = StencilOp::IncrWrap;
				else if(words[1] == "Decr") stencilOp = StencilOp::Decr;
				else if(words[1] == "DecrWrap") stencilOp = StencilOp::DecrWrap;
				else if(words[1] == "Invert") stencilOp = StencilOp::Invert;
				else std::cout << "Error when creating shader in section stencil: \"" << words[1] << "\" is not recognized" << std::endl;

				if(words[0] == "StencilOpSFail") m_stencilOpSFail = stencilOp;
				else if(words[0] == "StencilOpDPFail") m_stencilOpDPFail = stencilOp;
				else if(words[0] == "StencilOpDPPass") m_stencilOpDPPass = stencilOp;
			}
			else {
				std::cout << "Error when creating shader in section stencil: \"" << words[0] << "\" is not recognized" << std::endl;
			}
		}
	}

	void Shader::updateDepthTestData(const std::string& str) {
		std::stringstream strbuffer(str);
		std::string line;
		while(std::getline(strbuffer, line)) {
			std::stringstream linebufer(line);
			std::string word;
			std::vector<std::string> words;
			while(std::getline(linebufer, word, ' ')) words.push_back(word);
			if(words.size() < 1) continue;

			if(words[0] == "Enable") m_depthTestingEnabled = true;
			else if(words[0] == "Disable") m_depthTestingEnabled = false;
			else if(words.size() == 2 && words[0] == "DepthTestWrite") {
				if(words[1] == "Enable") m_depthTestWriteEnabled = true;
				else if(words[1] == "Disable") m_depthTestWriteEnabled = false;
				else std::cout << "Error when creating shader in section depthtest: \"" << words[1] << "\" is not recognized" << std::endl;
			}
			else if(words.size() == 2 && words[0] == "DepthTestFunc") {
				if(!updateCompareFunc(m_depthTestFunc, words[1]))
					std::cout << "Error when creating shader in section depthtest: \"" << words[1] << "\" is not recognized" << std::endl;
			}
			else {
				std::cout << "Error when creating shader in section depthTest: \"" << words[0] << "\" is not recognized" << std::endl;
			}
		}
	}

	bool Shader::updateCompareFunc(CompareFunc& func, const std::string& str) {
		if(str == "Never") func = CompareFunc::Never;
		else if(str == "Less") func = CompareFunc::Less;
		else if(str == "LEqual") func = CompareFunc::LEqual;
		else if(str == "Greater") func = CompareFunc::Greater;
		else if(str == "GEqual") func = CompareFunc::GEqual;
		else if(str == "Equal") func = CompareFunc::Equal;
		else if(str == "NotEqual") func = CompareFunc::NotEqual;
		else if(str == "Always") func = CompareFunc::Always;
		else return false;

		return true;
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

	int Shader::GetOpenGLCompareFunc(const CompareFunc& compareFunc) {
		switch(compareFunc) {
		case CompareFunc::Never: return GL_NEVER;
		case CompareFunc::Less: return GL_LESS;
		case CompareFunc::LEqual: return GL_LEQUAL;
		case CompareFunc::Greater: return GL_GREATER;
		case CompareFunc::GEqual: return GL_GEQUAL;
		case CompareFunc::Equal: return GL_EQUAL;
		case CompareFunc::NotEqual: return GL_NOTEQUAL;
		case CompareFunc::Always: return GL_ALWAYS;
		default: return 0;
		}
		return 0;
	}

	int Shader::GetOpenGLStencilOp(const StencilOp& stencilOp) {
		switch(stencilOp) {
		case StencilOp::Keep: return GL_KEEP;
		case StencilOp::Zero: return GL_ZERO;
		case StencilOp::Replace: return GL_REPLACE;
		case StencilOp::Incr: return GL_INCR;
		case StencilOp::IncrWrap: return GL_INCR_WRAP;
		case StencilOp::Decr: return GL_DECR;
		case StencilOp::DecrWrap: return GL_DECR_WRAP;
		case StencilOp::Invert: return GL_INVERT;
		default: return 0;
		}
		return 0;
	}

}