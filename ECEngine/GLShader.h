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
		//updates shader uniforms
		void UpdateUniforms(const std::string &uniformName, const T &data);
		//locates what are uniforms
		void FindUniforms(const std::string &uniformName);

		void LoadShaderFile(const char *vertexFile, const char *fragmentfile);
		void Compile();
		void Use();
		void UnUse();

	private:
		bool CompileFile(ShaderType type);
		bool shaderFlags[maxShaderType]; //check which shaders compiled
		GLuint shaderProgramID;
		GLuint shaderIDs[maxShaderType];
		std::string shaderCodeFiles[maxShaderType];
		std::unordered_map<std::string, GLint> shaderUniformLocations;

	};

	using ShaderPointer = std::shared_ptr<GLShader>;

	template<typename T>
	void GLShader::UpdateUniforms(const std::string &uniformName, const T &data) {
		//LoadShaderData
		LoadShaderData(shaderUniformLocations[uniformName], data);
	}

}

#endif // !GLSHADER_H

