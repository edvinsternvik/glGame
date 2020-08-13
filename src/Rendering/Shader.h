#pragma once
#include <string>
#include <unordered_map>

namespace glGame {

	class Shader {
	public:
		Shader(const char* filepath);
		Shader(const char* filepath, const char* shaderSource);
		~Shader();

		void useShader();
		void setUniform1i(const char* name, const int& integer);
		void setUniform3f(const char* name, float v1, float v2, float v3);
		void setUniformMat4(const char* name, const float* matrix);
		void setUniformBlockBinding(const char* name, const unsigned int& bindingPoint);

	private:
		void init(const std::string& source);
		std::string getSourceFromFile(const std::string& filepath);
		std::unordered_map<std::string, std::string> getShaderSections(const std::string& shaderSource);
		int getShaderTypeFromString(const std::string& typeString);
		unsigned int createShader(unsigned int shaderType, const std::string& shaderSource);
		void deleteShader(unsigned int shaderID);
		void updateStencilData(const std::string& str);
		void updateDepthTestData(const std::string& str);

		unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
		int getUniformLocation(const char* name);
		int getUniformBlockIndex(const char* name);


	private:
		unsigned int m_shaderProgramID;

		std::unordered_map<const char*, int> m_uniformLocations;
		std::unordered_map<const char*, int> m_uniformBlockIndicies;

		enum class StencilFunc { Never, Less, LEqual, Greater, GEqual, Equal, NotEqual, Always };
		enum class StencilOp { Keep, Zero, Replace, Incr, IncrWrap, Decr, DecrWrap, Invert };

		bool m_stencilEnabled = false;
		StencilFunc m_stencilFunc = StencilFunc::Always;
		int m_stencilFuncRef = 1;
		StencilOp m_stencilOpSFail = StencilOp::Keep;
		StencilOp m_stencilOpDPFail = StencilOp::Keep;
		StencilOp m_stencilOpDPPass = StencilOp::Keep;

		bool m_depthTestingEnabled = true;

		friend class Renderer;
	};

}