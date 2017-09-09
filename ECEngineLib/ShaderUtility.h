#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include "./ShaderUniforms.h"

//namespace Engine {
//
//	
//}

#ifdef ECENGINELIB_EXPORTS
#define ECENGINELIB_API __declspec(dllexport)
#else
#define ECENGINELIB_API __declspec(dllimport)
#endif



namespace Shaders {
	const int maxShaderType = 2;

	ECENGINELIB_API bool FileReader(const std::string &fileName, std::string& storageString);
	static GLboolean CheckShaderStatus(GLuint shaderID);

	template<typename Enumeration>
	auto to_int(const Enumeration value) -> typename std::underlying_type<Enumeration>::type {
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}

	//This works primarily as a shader program
	ECENGINELIB_API class ShaderUtility
	{
	public:
		enum ShaderType {
			VERTEX = 0,
			FRAGMENT = 1,
			GEOMETRY = 2
		};

		ECENGINELIB_API ShaderUtility();
		ECENGINELIB_API ~ShaderUtility();

		template<typename T>
		__declspec(dllexport) void UpdateUniforms(const std::string &uniformName, const T &data);

		ECENGINELIB_API void FindUniforms(const std::string &uniformName);

		ECENGINELIB_API void LoadShaderFile(const std::string &vertexFile, const std::string &fragmentfile);
		ECENGINELIB_API void LoadShaderCode(const std::string& vertexCode, const std::string& fragmentCode);
		ECENGINELIB_API bool Compile();
		ECENGINELIB_API void Use();
		ECENGINELIB_API void UnUse();

		//used for unit testing
		ECENGINELIB_API GLuint GetShaderProgramID() const {
			return shaderProgramID;
		}

		//Used for unit testing
		ECENGINELIB_API std::string GetShaderCodeForFileType(ShaderType type) const {
			int typeValue = to_int(type);
			if (typeValue < shaderCodeFiles.size() - 1) {
				return shaderCodeFiles[typeValue];
			}
		}		

	private:
		GLuint shaderProgramID;
		bool CompileFile(ShaderType type);
		bool shaderFlags[maxShaderType]; //check which shaders compiled

		GLuint shaderIDs[maxShaderType];
		std::array<std::string, maxShaderType> shaderCodeFiles;
		std::unordered_map<std::string, GLint> shaderUniformLocations;

	};

	using ShaderPointer = std::shared_ptr<ShaderUtility>;
	using ShaderMap = std::unordered_map<int, ShaderUtility>;


	template<typename T>
	void ShaderUtility::UpdateUniforms(const std::string &uniformName, const T &data) {
		LoadShaderData(shaderUniformLocations[uniformName], data);
	}


}




