#include "stdafx.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"
#include <numeric>
#include <sstream>
#include <algorithm>

namespace Engine {	

	namespace Systems {				
		
		const std::string RESOURCE_FILE_PATH = "../ECEngine/Engine/Core/Systems/Resources/assets/";
		const std::string DEFAULT_VERTEX_SHADER = "../ECEngine/Engine/Core/Shaders/ECVertexShader.txt";
		const std::string DEFAULT_FRAGMENT_SHADER = "../ECEngine/Engine/Core/Shaders/ECFragShader.txt";
		const std::string DEFAULT_SHADER_IDENTIFIER = "3DShader";
		ResourceManager::ResourceManager() {
			//always generate a basic texture on creation of texture resource manager
			//GenerateBasicTexture();
		}

		void ResourceManager::ClearResources() {
			for (auto iter : _textures) {
				glDeleteTextures(1, &iter.second->ID);
			}
			_textures.clear();

			//flush VBO and VAO data
		}

		//SHADERS

		void ResourceManager::LoadDefaultShaders() {
			Shaders::ShaderPointer defaultShader = std::make_shared<Shaders::ShaderUtility>();
			defaultShader->LoadShaderFile(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
			defaultShader->Compile();

			defaultShader->FindUniforms(Constants::MODELUNIFORM);
			defaultShader->FindUniforms(Constants::PROJECTIONUNIFORM);
			defaultShader->FindUniforms(Constants::VIEWUNIFORM);			
			defaultShader->FindUniforms(Constants::TEXTUREUNITFORM);			

			_shaderPrograms.emplace(DEFAULT_SHADER_IDENTIFIER, defaultShader);

			Shaders::ShaderPointer twoDimensionalShader = std::make_shared<Shaders::ShaderUtility>();
			twoDimensionalShader->LoadShaderFile("../ECEngine/Engine/Core/Shaders/2DVertexShader.txt", "../ECEngine/Engine/Core/Shaders/2DFragmentShader.txt");
			twoDimensionalShader->Compile();

			twoDimensionalShader->FindUniforms(Constants::MODELUNIFORM);
			twoDimensionalShader->FindUniforms(Constants::PROJECTIONUNIFORM);
			twoDimensionalShader->FindUniforms(Constants::TEXTUREUNITFORM);
			twoDimensionalShader->FindUniforms(Constants::COLORUNIFORM);
			_shaderPrograms.emplace("2Dshader", twoDimensionalShader);

			Shaders::ShaderPointer particleShader = std::make_shared<Shaders::ShaderUtility>();
			particleShader->LoadShaderFile("../ECEngine/Engine/Core/Shaders/ParticleVertex.txt", "../ECEngine/Engine/Core/Shaders/ParticleFragment.txt");
			particleShader->Compile();

			particleShader->FindUniforms(Constants::PROJECTIONUNIFORM);
			particleShader->FindUniforms("offset");
			particleShader->FindUniforms(Constants::COLORUNIFORM);
			particleShader->FindUniforms(Constants::TEXTUREUNITFORM);
			_shaderPrograms.emplace("particleShader", particleShader);
		}

		Shaders::ShaderPointer ResourceManager::GetShader(std::string& name) {
			ShaderMap::iterator it = _shaderPrograms.find(name);
			if (it != _shaderPrograms.end()) {
				return it->second;
			}
			throw std::range_error("ERROR::SHADER::SHADER_NOT_FOUND_ON_ADD");
		}

		Texture::TexturePointer ResourceManager::GetTexture(const std::string& name) {
			if (_textures.find(name) != _textures.end()) {
				return _textures[name];
			}
			return nullptr;
		}

		void ResourceManager::LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name) {
			Texture::TexturePointer texture = std::make_shared<Texture::BaseTexture>(alpha);
			int width; 
			int height;
			int nrChannels;
			stbi_set_flip_vertically_on_load(false);
			
			unsigned char * imageData = stbi_load(file, &width, &height, &nrChannels, 0);

			if (imageData) {
				texture->Generate(width, height, imageData);
				_textures.emplace(name, texture);
			}
			else {
				std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: " << file << std::endl;
			}
			stbi_image_free(imageData);
		}

		void ResourceManager::CreateBasicTexture() {
			Texture::TexturePointer texture = std::make_shared<Texture::BaseTexture>(false);
			GLubyte data[] = { 255, 255, 255, 255 };
			texture->Generate(1, 1, data);
			_textures.emplace("base", texture);
		}
		
		void ResourceManager::BindTexture(const std::string& name) {
			auto it = _textures.find(name);
			if (it != _textures.end()) {
				glBindTexture(GL_TEXTURE_2D, it->second->ID);
			}
			else {
				std::cout << "ERROR::TEXTURE::UNABLE_TO_BIND_TEXTURE: " << name << std::endl;
			}			
		}

		void ResourceManager::AddMesh(Sprite::MeshShared sprite) {
			_spriteResources.emplace(sprite->Name, sprite);
		}

		Sprite::MeshShared ResourceManager::GetMesh(const std::string& name) {
			auto it = _spriteResources.find(name);
			if (it != _spriteResources.end()) {
				return _spriteResources[name];
			}
			return nullptr;
		}

		void ResourceManager::ClearMeshBuffers() {
			if (_buffers.vertexArrayObject > 0) {
				glDeleteBuffers(1, &_buffers.vertexArrayObject);
			}
			if (_buffers.staticEBO > 0) {
				glDeleteBuffers(1, &_buffers.staticEBO);
			}
			if (_buffers.staticVBO > 0) {
				glDeleteBuffers(1, &_buffers.staticVBO);
			}
		}

		void ResourceManager::BindVertexArrays() {
			glBindVertexArray(_buffers.vertexArrayObject);
		}

		void ResourceManager::UnbindVertexArrays() {

		}

		// This should only set up buffers using Mesh Resources
		bool ResourceManager::LoadSpriteResourcesIntoBuffers() {
			bool success = false;
			if (!_spriteResources.empty()) {
				GLsizei totalVertexVectorSize = std::accumulate(_spriteResources.begin(), _spriteResources.end(), 0,
					[](GLsizei sum, const std::pair<std::string, Sprite::MeshShared>& sprite) {
					return sum + sprite.second->CalculateMemoryBlock();
				});
			
				glGenVertexArrays(1, &_buffers.vertexArrayObject);
				glGenBuffers(1, &_buffers.staticVBO);
				//Any subsequent VBO, EBO, and AttribPointer calls will be stored in this VAO
				glBindVertexArray(_buffers.vertexArrayObject);
				glBindBuffer(GL_ARRAY_BUFFER, _buffers.staticVBO);
				glBufferData(GL_ARRAY_BUFFER, totalVertexVectorSize,
					0, GL_STATIC_DRAW);

				AddVertexDataToBoundBuffer();
				AdjustVertexAttributePointers();

				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				success = true;
			}		

			return success;		
		}

		void ResourceManager::AddVertexDataToBoundBuffer() {
			GLsizeiptr bufferOffset = 0;
			for (auto spriteSource = _spriteResources.begin(); spriteSource != _spriteResources.end(); ++spriteSource) {
				if (spriteSource->second->VerticiesCount > 0) {
					// Keep in mind that the offset is passed by reference here.
					spriteSource->second->SubstituteData(bufferOffset);
				}
			}
		}

		void ResourceManager::AdjustVertexAttributePointers() {
			GLsizei vertexStride = CalculateBufferStride();
			int counter = 0;

			//for every attribute in my mesh do this
			for (auto it = _config.attributes.begin(); it != _config.attributes.end(); it++) {
				if (it->size > 0) {
					//this is a shader location
					glEnableVertexAttribArray(it->location);
					glVertexAttribPointer(it->location, it->size, GL_FLOAT, GL_FALSE, vertexStride,
						(GLvoid*)(sizeof(GLfloat) * counter));
					counter += it->size;
				}
			}
		}

		GLsizei ResourceManager::CalculateBufferStride() {
			GLuint strideMultiplier = std::accumulate(_config.attributes.begin(), _config.attributes.end(), 0,
				[](int sum, const MeshConfiguration::VertexAttribute& attribute) {
				return sum + attribute.size;
			});

			return strideMultiplier * sizeof(GLfloat);
		}

	}

}


