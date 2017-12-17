#pragma once
#include "./BaseTexture.h"
#include "../ECEngineLib/ShaderUtility.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/Mesh.h"

#include <GL/glew.h>
#include <vector>
#include <unordered_map>
#include <tuple>

namespace Engine {

	namespace Systems {

		using ShaderMap = std::unordered_map<std::string, Shaders::ShaderPointer>;
		using TextureMap = std::unordered_map<std::string, Texture::TexturePointer>;
		using SpriteMap = std::unordered_map<std::string, Sprite::MeshShared>;

		class ResourceManager
		{
		public:

			struct Buffer {
				GLuint vertexArrayObject;
				GLuint staticVBO;
				GLuint dynamicVBO;
				GLuint staticEBO;
				GLuint dynamicEBO;
				GLuint frameBufferObject;
			};

			struct MeshConfiguration {
				struct VertexAttribute {
					VertexAttribute(GLint _size, int _location) : size(_size), location(_location) {}
					GLint size;
					int location;
					//bool includeInPointer = true;
				};

				MeshConfiguration() {}
				~MeshConfiguration() {
					attributes.clear();
				}

				std::vector<VertexAttribute> attributes;
				
			};

			ResourceManager();
			~ResourceManager() {};

			//SHADERS
			//void AddCustomShader(std::string name, Shaders::ShaderPointer shader);
			void LoadDefaultShaders();
			Shaders::ShaderPointer GetShader(std::string& shader);

			//MESHES
			void AddMesh(Sprite::MeshShared sprite);
			bool LoadSpriteResourcesIntoBuffers();
			//void ChangeMeshVertexConfiguration();
			void BindVertexArrays(Sprite::Mesh::MeshType type);
			void UnbindVertexArrays();

			//TEXTURES
			Texture::TexturePointer LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name,
				std::string type);
			void BindTexture(const std::string& name);			
			Texture::TexturePointer  CreateBasicTexture();
			
			Texture::TexturePointer GetTexture(const std::string& name);
			Sprite::MeshShared GetMesh(const std::string& name);
			void ClearMeshBuffers();

			void ClearResources();

		private:									

			TextureMap _textures;	
			ShaderMap _shaderPrograms;

			SpriteMap _spriteResources;
			SpriteMap _lightSourceResources;

			//I should not need to hold on to config
			//MeshConfiguration _config;
			Buffer _buffers;
			Buffer _lightBuffers;			
			
			GLsizei CalculateBufferStride(MeshConfiguration& config);
			void AddVertexDataToBoundBuffer(SpriteMap& sprites);
			void AdjustVertexAttributePointers(MeshConfiguration& config);
		};
		using ResourceManagerShared = std::shared_ptr<ResourceManager>;
	}
}
// TODO
//Use static frame objects to store the data that doesnt change everyframe - textures and colors
//MeshInfo staticFrameObjects;
//Use dynamic frame objects to store the data that does change everyframe - position, lighting
//MeshInfo dynamicFrameObjects;



