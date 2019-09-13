#pragma once
#include <string>

namespace glGame {

	class Shader {
	public:
		Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);

		void useShader();

	private:
		std::string getShaderStringFromFile(std::string& filepath);
		unsigned int createShader(unsigned int shaderType, std::string& shaderSource);
		void deleteShader(unsigned int shaderID);

		unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

	private:
		unsigned int m_shaderProgramID;

	};

}