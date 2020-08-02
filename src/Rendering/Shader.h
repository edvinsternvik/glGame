#pragma once
#include <string>
#include <unordered_map>

namespace glGame {

	class Shader {
	public:
		Shader(const char* filepath);

		void useShader();
		void setUniform1i(const char* name, const int& integer);
		void setUniform3f(const char* name, float v1, float v2, float v3);
		void setUniformMat4(const char* name, const float* matrix);
		void setUniformBlockBinding(const char* name, const unsigned int& bindingPoint);

	private:
		std::unordered_map<int, std::string> getShaderSourcesFromFile(const std::string& filepath);
		int getShaderTypeFromString(const std::string& typeString);
		unsigned int createShader(unsigned int shaderType, std::string& shaderSource);
		void deleteShader(unsigned int shaderID);

		unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
		int getUniformLocation(const char* name);
		int getUniformBlockIndex(const char* name);

	private:
		unsigned int m_shaderProgramID;

		std::unordered_map<const char*, int> m_uniformLocations;
		std::unordered_map<const char*, int> m_uniformBlockIndicies;
	};

}