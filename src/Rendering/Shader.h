#pragma once
#include <string>
#include <map>

namespace glGame {

	class Shader {
	public:
		Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);

		void useShader();
		void setUniform3f(const char* name, float v1, float v2, float v3);

	private:
		std::string getShaderStringFromFile(std::string& filepath);
		unsigned int createShader(unsigned int shaderType, std::string& shaderSource);
		void deleteShader(unsigned int shaderID);

		unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
		int getUniformLocation(const char* name);

	private:
		unsigned int m_shaderProgramID;

		std::map<const char*, int> m_uniformLocations;
	};

}