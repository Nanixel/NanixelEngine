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
				};

				MeshConfiguration() {
					// make these pointers
					VertexAttribute position(Sprite::POSITION_COORDS, 0);
					VertexAttribute texture(Sprite::TEXTURE_COORDS, 1);
					attributes.push_back(position);
					attributes.push_back(texture);
				}
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
			void BindVertexArrays();
			void UnbindVertexArrays();

			//TEXTURES
			void LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name);
			void BindTexture(const std::string& name);			
			void CreateBasicTexture();
			//int InitializeTexture(const std::string textureName, std::unordered_map<std::string, int>& loadedTextures);

			void ClearResources();

			Texture::TexturePointer GetTexture(const std::string& name);
			Sprite::MeshShared GetMesh(const std::string& name);
			void ClearMeshBuffers();

		private:									

			TextureMap _textures;
			SpriteMap _spriteResources;
			ShaderMap _shaderPrograms;

			MeshConfiguration _config;
			Buffer _buffers;
			//void GenerateTexture(TexturePointer textureObject, GLuint width, GLuint height, unsigned char* imageData);
			
			GLsizei CalculateBufferStride();
			void AddVertexDataToBoundBuffer();
			void AdjustVertexAttributePointers();
			//void SubMeshData(const Sprite::MeshShared& spriteToRender, GLsizeiptr& offset);
			
		};
		using ResourceManagerShared = std::shared_ptr<ResourceManager>;
	}
}
// TODO
//Use static frame objects to store the data that doesnt change everyframe - textures and colors
//MeshInfo staticFrameObjects;
//Use dynamic frame objects to store the data that does change everyframe - position, lighting
//MeshInfo dynamicFrameObjects;



