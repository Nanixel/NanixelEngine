#include "stdafx.h"
#include "./ShaderUtility.h"
#include <fstream>
#include <windows.h>
#include <sstream>

namespace Shaders {
	//If the file cannot be read this returns false
	bool FileReader(const std::string &fileName, std::string& storageString) {
		//clear the storage string first incase its already holding something
		storageString.clear();

		if (!fileName.empty()) {		

			std::ifstream shaderfile;

			shaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try {
				//open the file, read the contents into stream and set our codeStorage to the results in the stream
				shaderfile.open(fileName);
				std::stringstream shaderStream;
				shaderStream << shaderfile.rdbuf();
				shaderfile.close();
				storageString = shaderStream.str().c_str();
				return true;
			}
			catch (std::ifstream::failure e) {
				std::cout << "ERROR::ENGINE::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << fileName << std::endl;
				return false;
			}
		}

		return false;
	}

	static GLboolean CheckShaderStatus(GLuint shaderID) {
		GLint shaderStatus;

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderStatus);

		if (shaderStatus != GL_TRUE) {
			GLint infoLog;
			//getting number of characters in log
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLog);
			char buffer[512];
			glGetShaderInfoLog(shaderID, infoLog, NULL, buffer);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << buffer << std::endl;			
			return GL_FALSE;
		}
		return GL_TRUE;
	}

	//Whats the point of this method returning bool if its never used? -> only for this one
	static bool CheckProgramStatus(GLuint programID) {

		GLint programStatus;

		glGetProgramiv(programID, GL_LINK_STATUS, &programStatus);

		if (programStatus != true) {

			GLint programInfoLogSize;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &programInfoLogSize);
			char buffer[512];
			glGetProgramInfoLog(programID, programInfoLogSize, NULL, buffer);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << buffer << std::endl;			
			return GL_FALSE;
		}

		return true;
	}

	ShaderUtility::ShaderUtility() {
		//For vertex and fragment shaders.....for now
		for (int i = 0; i < maxShaderType; i++) {
			shaderIDs[i] = 0;
			shaderCodeFiles[i].clear();
			shaderFlags[i] = false;
		}
		shaderProgramID = 0;
	}

	ShaderUtility::~ShaderUtility() {
		for (int i = 0; i < maxShaderType; i++) {
			if (shaderIDs[i] != 0) {
				glDeleteShader(shaderIDs[i]);
				shaderCodeFiles[i].clear();
			}
		}
	}

	//Loads Vertex and Fragment Shaders into this class
	void ShaderUtility::LoadShaderFile(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
		//vertex shader is always zero		
		shaderFlags[ShaderUtility::ShaderType::VERTEX] = FileReader(vertexFilePath, shaderCodeFiles[ShaderUtility::ShaderType::VERTEX]);
		shaderFlags[ShaderUtility::ShaderType::FRAGMENT] = FileReader(fragmentFilePath, shaderCodeFiles[ShaderUtility::ShaderType::FRAGMENT]);
	}

	void ShaderUtility::LoadShaderCode(const std::string& vertexCode, const std::string& fragmentCode) {
	
		if (!vertexCode.empty()) {
			shaderFlags[ShaderUtility::ShaderType::VERTEX] = true;
			shaderCodeFiles[ShaderUtility::ShaderType::VERTEX] = vertexCode;
		}

		if (!fragmentCode.empty()) {
			shaderFlags[ShaderUtility::ShaderType::FRAGMENT] = true;
			shaderCodeFiles[ShaderUtility::ShaderType::FRAGMENT] = fragmentCode;
		}
	}

	//Compiles the shader type associated with this file
	bool ShaderUtility::CompileFile(ShaderUtility::ShaderType type) {
		const char *file = shaderCodeFiles[type].c_str();

		switch (type)
		{
		case ShaderUtility::VERTEX:
			shaderIDs[type] = glCreateShader(GL_VERTEX_SHADER);			
			break;
		case ShaderUtility::FRAGMENT:
			shaderIDs[type] = glCreateShader(GL_FRAGMENT_SHADER);			
			break;
		case ShaderUtility::GEOMETRY:
			shaderIDs[type] = glCreateShader(GL_GEOMETRY_SHADER);
		default:
			std::cout << "ERROR::SHADER::FILE_COMPILATION_FAILED" << std::endl;
			break;
		}

		glShaderSource(shaderIDs[type], 1, &file, NULL);
		glCompileShader(shaderIDs[type]);

		return CheckShaderStatus(shaderIDs[type]);		
	}

	//Finds uniform data in the current shader
	void ShaderUtility::FindUniforms(const std::string& uniformName) {
		//load this uniform location into shaderUniforms variable
		shaderUniformLocations[uniformName] = glGetUniformLocation(shaderProgramID, uniformName.c_str());
		//check that this actually has data
		if (shaderUniformLocations[uniformName] == -1) {
			std::cout << "ERROR::UNIFORM::NAME_NOT_FOUND_IN_SHADER\n" << uniformName << std::endl;
		}
	}	

	//Makes shader usable
	bool ShaderUtility::Compile() {

		//both vertex and fragment shaders need to be done at this point
		if (shaderFlags[ShaderUtility::ShaderType::VERTEX] == false || shaderFlags[ShaderUtility::ShaderType::FRAGMENT] == false) {
			std::cout << "ERROR::SHADER::COMPILE::VERTEX_OR_FRAGMENT_SHADER_NOT_ACTIVE" << std::endl;
			return false;
		}

		bool vertexCompiled = CompileFile(ShaderUtility::ShaderType::VERTEX);
		bool fragmentComiled = CompileFile(ShaderUtility::ShaderType::FRAGMENT);

		//TODO - Probably need to do something else for the geometry shader

		//Create the programID
		shaderProgramID = glCreateProgram();
		//attach shaders to the newly created programID	
		glAttachShader(shaderProgramID, shaderIDs[ShaderUtility::ShaderType::VERTEX]);				
		glAttachShader(shaderProgramID, shaderIDs[ShaderUtility::ShaderType::FRAGMENT]);
		
		//test to see if one compiles and the other doesnt
		glLinkProgram(shaderProgramID);
		return CheckProgramStatus(shaderProgramID);
	}

	//Binds the shader to be ready for use in the current draw
	void ShaderUtility::Use() {
		glUseProgram(shaderProgramID);
	}
	//Stop using this program
	void ShaderUtility::UnUse() {
		glUseProgram(0);
	}
}