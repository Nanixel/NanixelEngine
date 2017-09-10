#include "stdafx.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"

namespace Engine {

	//NOT CURRENTLY HOOKED UP TO ENGINE - REMOVE THIS ONCE IT IS

	namespace Systems {
		ResourceManager::ResourceManager() : System(std::string("ECResourceSystem"), SystemType::EC_ResourceSystem)
		{
		}

		ResourceManager::~ResourceManager()
		{
		}

		void ResourceManager::Init() {
			RegisterComponent(BitFieldComponent::MC_Resource);

		}

		void ResourceManager::Update(float dt) {

		}

		void ResourceManager::ShutDown() {
			ClearResources();
		}

		//remember this works as a recieve message...the engine will send it and this will decide what to do with it
		void ResourceManager::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {

		}

		void ResourceManager::ClearResources() {
			for (auto iter : texturesMap) {
				glDeleteTextures(1, &iter.second->ID);
			}
			texturesMap.clear();
		}

		//TEST THIS ON THE TEST DUMP BEFORE STARTING
		void ResourceManager::LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name) {
			Texture::TexturePointer texture = std::make_shared<Texture::Texture>();

			if (alpha) {
				texture->Internal_Format = GL_RGBA;
				texture->Image_Format = GL_RGBA;
			}
			int width; 
			int height;
			int nrChannels;
			stbi_set_flip_vertically_on_load(true);
			//test if this loads from all file locations
			unsigned char * imageData = stbi_load(file, &width, &height, &nrChannels, 0);
			//validate this if check and see if it works
			if (imageData) {
				
				GenerateTexture(texture, width, height, imageData);
				texturesMap.emplace(name, texture);
			}
			else {
				std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: " << file << std::endl;
			}
			stbi_image_free(imageData);
		}

		//just sets the properties of a texture object
		void ResourceManager::GenerateTexture(Texture::TexturePointer textureObject, GLuint width, GLuint height, unsigned char* data) {
			textureObject->Width = width;
			textureObject->Height = height;

			glBindTexture(GL_TEXTURE_2D, textureObject->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, textureObject->Internal_Format, width, height, 0, textureObject->Image_Format, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureObject->Wrap_S);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureObject->Wrap_T);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureObject->Filter_Min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureObject->Filter_Max);

			//Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void ResourceManager::BindTexture(const std::string& name) const {
			auto it = texturesMap.find(name);
			if (it != texturesMap.end()) {
				glBindTexture(GL_TEXTURE_2D, it->second->ID);
			}
			else {
				std::cout << "ERROR::TEXTURE::UNABLE_TO_BIND_TEXTURE: " << name << std::endl;
			}			
		}
	}

}


