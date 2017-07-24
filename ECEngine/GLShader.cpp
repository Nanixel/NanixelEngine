#include "stdafx.h"
#include "./GLShader.h"
#include <fstream>

//This shader class works alot like a state machine - kinda like openGL

namespace Engine {

	//Reads shader txt code into storageString
	bool ShaderFileReader(const char* fileName, std::string& storageString) {
		storageString.clear();

		if (fileName == nullptr) {
			return false;
		}

		std::string file(fileName);
		std::string filePath("./");
		filePath += file;

		std::string shaderCode;
		std::ifstream shaderfile;

		shaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderfile.open(fileName);
			std::stringstream shaderStream;

			shaderStream << shaderfile.rdbuf();

			shaderfile.close();			
			storageString = shaderStream.str().c_str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::ENGINE::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
			return false;
		}

		return true;
	}

	static GLboolean checkShaderStatus(GLuint shaderID) {
		GLint shaderStatus;

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderStatus);

		if (shaderStatus != GL_TRUE) {
			GLint infoLog;
			//getting number of characters in log
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLog);
			GLchar *buffer = new GLchar[infoLog];
			glGetShaderInfoLog(shaderID, infoLog, NULL, buffer);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << *buffer << std::endl;
			delete[] buffer;
			return GL_FALSE;
		}
		return GL_TRUE;
	}

	static GLboolean checkProgramStatus(GLuint programID) {

		GLint programStatus;

		glGetProgramiv(programID, GL_LINK_STATUS, &programStatus);

		if (programStatus != GL_TRUE) {

			GLint programInfoLogSize;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &programInfoLogSize);
			GLchar *buffer = new GLchar[programInfoLogSize];
			glGetProgramInfoLog(programID, programInfoLogSize, NULL, buffer);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << *buffer << std::endl;
			delete[] buffer;
			return GL_FALSE;
		}

		return GL_TRUE;
	}

	GLShader::GLShader() {
		//For vertex and fragment shaders.....for now
		for (int i = 0; i < maxShaderType; i++) {
			shaderIDs[i] = 0;
			shaderCodeFiles[i].clear();
			shaderFlags[i] = false;
		}
		shaderProgramID = 0;
	}

	GLShader::~GLShader() {
		for (int i = 0; i < maxShaderType; i++) {
			glDeleteShader(shaderIDs[i]);
			//this is an array of strings
			//clear the shader file code at this index
			shaderCodeFiles[i].clear();
		}
	}

	//Loads Vertex and Fragment Shaders into this class
	void GLShader::LoadShaderFile(const char* vertexShaderPath, const char* fragmentShaderPath) {
		//vertex shader is always zero
		shaderFlags[GLShader::ShaderType::VERTEX] = ShaderFileReader(vertexShaderPath, shaderCodeFiles[GLShader::ShaderType::VERTEX]);
		shaderFlags[GLShader::ShaderType::FRAGMENT] = ShaderFileReader(fragmentShaderPath, shaderCodeFiles[GLShader::ShaderType::FRAGMENT]);
	}

	//Compiles the shader type associated with this file
	bool GLShader::CompileFile(GLShader::ShaderType type) {
		const char *file = shaderCodeFiles[type].c_str();

		switch (type)
		{
		case Engine::GLShader::VERTEX:
			shaderIDs[type] = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(shaderIDs[type], 1, &file, NULL);
			glCompileShader(shaderIDs[type]);
			break;
		case Engine::GLShader::FRAGMENT:
			shaderIDs[type] = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(shaderIDs[type], 1, &file, NULL);
			glCompileShader(shaderIDs[type]);
			break;
		default:
			std::cout << "ERROR::SHADER::FILE_COMPILATION_FAILED" << std::endl;
			break;
		}

		if (!checkShaderStatus(shaderIDs[type])) {
			return false;
		}

		return true;
	}

	//Finds uniform data in the current shader
	void GLShader::FindUniforms(const std::string& uniformName) {
		//load this uniform location into shaderUniforms variable
		shaderUniformLocations[uniformName] = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		//check that this actually has data
		if (shaderUniformLocations[uniformName] == -1) {
			std::cout << "ERROR::UNIFORM::NAME_NOT_FOUND_IN_SHADER\n" << uniformName << std::endl;			
		}
	}

	//Makes shader usable
	void GLShader::Compile() {

		//both vertex and fragment shaders need to be done at this point
		if (shaderFlags[GLShader::ShaderType::VERTEX] == false || shaderFlags[GLShader::ShaderType::FRAGMENT] == false) {
			std::cout << "ERROR::SHADER::COMPILE::VERTEX_OR_FRAGMENT_SHADER_NOT_ACTIVE" << std::endl;
			return;
		}

		CompileFile(GLShader::ShaderType::VERTEX);
		CompileFile(GLShader::ShaderType::FRAGMENT);

		//TODO - Probably need to do something else for the geometry shader

		//Create the programID
		shaderProgramID = glCreateProgram();
		//attach shaders to the newly created programID
		glAttachShader(shaderProgramID, shaderIDs[GLShader::ShaderType::VERTEX]);
		glAttachShader(shaderProgramID, shaderIDs[GLShader::ShaderType::FRAGMENT]);
		glLinkProgram(shaderProgramID);
		checkProgramStatus(shaderProgramID);
	}

	//Binds the shader to be ready for use in the current draw
	void GLShader::Use() {
		glUseProgram(shaderProgramID);
	}
	//Stop using this program
	void GLShader::UnUse() {
		glUseProgram(0);
	}
}
