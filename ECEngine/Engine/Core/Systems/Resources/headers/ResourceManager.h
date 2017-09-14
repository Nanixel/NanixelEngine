#pragma once
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "./BaseTexture.h"
#include "../ECEngineLib/ShaderUtility.h"

#include <GL/glew.h>
#include <vector>
#include <unordered_map>

namespace Engine {

	namespace Systems {
		using TexturePointer = std::shared_ptr<Texture::BaseTexture>;
		using TextureMap = std::unordered_map<std::string, TexturePointer>;
		//Acts upon entites that contain a resource component
		//Manages all the resources of the entities that exist in a certain space
		class ResourceManager
		{
		public:
			
			//deallocates all resources
			static void ClearResources();
			static void LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name);
			static void BindTexture(const std::string& name);

			static TexturePointer GetTexture(const std::string& name) {
				return texturesMap[name];
			}
			//keep in mind - making this static means tons of places may be able to alter it at once
			

		private:
			ResourceManager() {};
			~ResourceManager() {};
			static TextureMap texturesMap;
			//this has to do with creating the resources rather than handling the resource components of entities in this system so this
			//is probably best done in a utility class and not here, remember single responsibility priciple
			//this resource manager class should just be responsible for updating the RESOURCE COMPONENTS of it's entities
			//this is probably best done in a utility class...similar to my shader utility
			
			static void GenerateTexture(TexturePointer textureObject, GLuint width, GLuint height, unsigned char* imageData);
			
			
			

		};

	}
}



