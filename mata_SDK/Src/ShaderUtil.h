#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SDKHeader.h"

class ShaderUtil {
private:
	GLchar* VertexShaderSource{}, *FragmentShaderSource{}, *ComputeShaderSource{};
	GLuint  VertexShaderBuffer{}, FragmentShaderBuffer{}, CommputeShaderBuffer{};

public:
	char* LoadShaderFile(std::string file);
	void LoadVertexShader(std::string VertexShader);
	void LoadFragmentShader(std::string FragmentShader);
	void LoadComputeShader(std::string ComputeShader);
	void CreateShader(GLuint& Shader);
	void CreateComputeShader(GLuint& Shader);
	void CreateShaderLocation();
	void CreateSSBO();
};
extern ShaderUtil shaderUtil;