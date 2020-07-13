#pragma once
#include <string>
#include <map>
#include <unordered_map>

namespace glGame {

	class Shader {
	public:
		Shader(std::string shaderPath);

		void useShader();
		void setUniform3f(const char* name, float v1, float v2, float v3);
		void setUniformMat4(const char* name, const float* matrix);

	private:
		std::unordered_map<int, std::string> getShaderSourcesFromFile(std::string& filepath);
		int getShaderTypeFromString(const std::string& typeString);
		unsigned int createShader(unsigned int shaderType, std::string& shaderSource);
		void deleteShader(unsigned int shaderID);

		unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
		int getUniformLocation(const char* name);

	private:
		unsigned int m_shaderProgramID;

		std::map<const char*, int> m_uniformLocations;
	};

}