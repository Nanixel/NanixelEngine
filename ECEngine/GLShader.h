#pragma once
#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "./GLShaderUniforms.h"

namespace Engine {

	const int maxShaderType = 2;

	class GLShader {
	public:
		enum ShaderType {
			VERTEX = 0,
			FRAGMENT
		};

		GLShader();
		~GLShader();

		template<typename T>
		void UpdateUniforms(const std::string &uniformName, const T &data);

		void FindUniforms(const std::string &uniformName);

		void LoadShaderFile(const char *vertexFile, const char *fragmentfile);
		void Compile();
		void Use();
		void UnUse();
		GLuint shaderProgramID;
	private:
		bool CompileFile(ShaderType type);
		bool shaderFlags[maxShaderType]; //check which shaders compiled
		
		GLuint shaderIDs[maxShaderType];
		std::string shaderCodeFiles[maxShaderType];
		std::unordered_map<std::string, GLint> shaderUniformLocations;		

	};

	using ShaderPointer = std::shared_ptr<GLShader>;

	template<typename T>
	void GLShader::UpdateUniforms(const std::string &uniformName, const T &data) {		
		LoadShaderData(shaderUniformLocations[uniformName], data);
	}

}

#endif // !GLSHADER_H

