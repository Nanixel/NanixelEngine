#pragma once
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngineLib/Texture.h"
#include "../ECEngineLib/ShaderUtility.h"

#include <GL/glew.h>
#include <vector>

namespace Engine {

	namespace Systems {
		//Acts upon entites that contain a resource component
		//Manages all the resources of the entities that exist in a certain space
		class ResourceManager : public System
		{
		public:
			ResourceManager();

			void Init();
			void Update(float);
			void ShutDown();
			void SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message);			

			//deallocates all resources
			void ClearResources();

			Texture::TexturePointer GetTexture(const std::string& name) {
				return texturesMap[name];
			}

			

			~ResourceManager();

		private:

			//this has to do with creating the resources rather than handling the resource components of entities in this system so this
			//is probably best done in a utility class and not here, remember single responsibility priciple
			//this resource manager class should just be responsible for updating the RESOURCE COMPONENTS of it's entities
			//this is probably best done in a utility class...similar to my shader utility
			void LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name);
			void GenerateTexture(Texture::TexturePointer textureObject, GLuint width, GLuint height, unsigned char* imageData);
			void BindTexture(const std::string& name) const;
			Texture::TextureMap texturesMap;			
			
			

		};

	}
}



