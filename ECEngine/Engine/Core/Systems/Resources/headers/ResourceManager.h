#pragma once
//#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "./BaseTexture.h"
#include "../ECEngineLib/ShaderUtility.h"

#include <GL/glew.h>
#include <vector>
#include <unordered_map>
#include <tuple>
//#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceMap.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/SptireResource.h"

namespace Engine {

	//update this resource manager to not be static -> it should also be responsible for loading and preloading the textures/verticies/etc..

	namespace Systems {
		using TexturePointer = std::shared_ptr<Texture::BaseTexture>;
		using TextureMap = std::unordered_map<std::string, TexturePointer>;
		using DrawDataPointer = std::shared_ptr<std::tuple<GLuint, int>>;
		//Acts upon entites that contain a resource component
		//Manages all the resources of the entities that exist in a certain space
		class ResourceManager
		{
		public:

			using ResourceManagerShared = std::shared_ptr<ResourceManager>;

			ResourceManager();
			~ResourceManager() {};
			//deallocates all resources
			void ClearResources();
			void LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name);
			void BindTexture(const std::string& name);			

			TexturePointer GetTexture(const std::string& name) {
				if (texturesMap.find(name) != texturesMap.end()) {
					return texturesMap[name];
				}
				return nullptr;
			}			

			//takes all the sprites to load for this space
			void AddSpriteResource(Sprite::SpriteResource::SpriteSourceShared sprite);
			Sprite::SpriteResource::SpriteSourceShared GetSpriteResource(const std::string& name);
			bool LoadSpriteResourcesIntoBuffers();
			void GenerateBasicTexture();			
			
			struct Buffers {
				GLuint vertexArrayObject;
				GLuint staticVBO;
				GLuint dynamicVBO;
				GLuint staticEBO;
				GLuint dynamicEBO;
			};

			Buffers _buffers;
			std::unordered_map<std::string, std::shared_ptr<Sprite::AttributeConfiguration>> vertexAttributeConfigurations;
		private:
			//map of names and pointers to resources
			std::unordered_map<std::string, Sprite::SpriteResource::SpriteSourceShared> spriteResources;				

			

			//ResourceMap<Sprite::SpriteResource> spriteResources;
			//ResourceMap<Sprite::SpriteResource> textureResources;

			TextureMap texturesMap;

			//this has to do with creating the resources rather than handling the resource components of entities in this system so this
			//is probably best done in a utility class and not here, remember single responsibility priciple
			//this resource manager class should just be responsible for updating the RESOURCE COMPONENTS of it's entities
			//this is probably best done in a utility class...similar to my shader utility
			
			void GenerateTexture(TexturePointer textureObject, GLuint width, GLuint height, unsigned char* imageData);
			
			void InitMeshRenderData(const Sprite::SpriteResource::SpriteSourceShared& spriteToRender, GLsizeiptr& offset);
			
			
			

		};

	}
}



