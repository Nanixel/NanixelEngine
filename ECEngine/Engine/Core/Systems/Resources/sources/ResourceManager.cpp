#include "stdafx.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include <numeric>

namespace Engine {	

	namespace Systems {				

		ResourceManager::ResourceManager() {
			//always generate a basic texture on creation of texture resource manager
			//GenerateBasicTexture();
		}

		void ResourceManager::ClearResources() {
			for (auto iter : texturesMap) {
				glDeleteTextures(1, &iter.second->ID);
			}
			texturesMap.clear();

			//flush VBO and VAO data
		}

		void ResourceManager::LoadTextureDataFromFile(const GLchar *file, GLboolean alpha, std::string name) {
			TexturePointer texture = std::make_shared<Texture::BaseTexture>();

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
		void ResourceManager::GenerateTexture(TexturePointer textureObject, GLuint width, GLuint height, unsigned char* data) {
			textureObject->Width = width;
			textureObject->Height = height;

			glBindTexture(GL_TEXTURE_2D, textureObject->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, textureObject->Internal_Format, width, height, 0, textureObject->Image_Format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureObject->Wrap_S);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureObject->Wrap_T);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureObject->Filter_Min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureObject->Filter_Max);

			//Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void ResourceManager::GenerateBasicTexture() {
			TexturePointer texture = std::make_shared<Texture::BaseTexture>();
			GLubyte data[] = { 255, 255, 255, 255 };
			GenerateTexture(texture, 1, 1, data);
			texturesMap.emplace("base", texture);
		}

		void ResourceManager::BindTexture(const std::string& name) {
			auto it = texturesMap.find(name);
			if (it != texturesMap.end()) {
				glBindTexture(GL_TEXTURE_2D, it->second->ID);
			}
			else {
				std::cout << "ERROR::TEXTURE::UNABLE_TO_BIND_TEXTURE: " << name << std::endl;
			}			
		}

		void ResourceManager::AddSpriteResource(Sprite::SpriteResource::SpriteSourceShared sprite) {
			spriteResources.emplace(sprite->getName() ,sprite);			
		}

		Sprite::SpriteResource::SpriteSourceShared ResourceManager::GetSpriteResource(const std::string& name) {
			//have some null checking here
			return spriteResources[name];
		}

		//The idea of this resource manager was to load the noted resources and then have a map of them to load 
		bool ResourceManager::LoadSpriteResourcesIntoBuffers() {
			
			if (spriteResources.empty()) {
				//no resources were loaded
				return false;
			}		

			GLsizei totalVertexVectorSize = std::accumulate(spriteResources.begin(), spriteResources.end(), 0,
				[](GLsizei sum, const std::pair<std::string, Sprite::SpriteResource::SpriteSourceShared>& sprite) {
				return sum + sprite.second->meshVerticies.size() * sizeof(GLfloat);
			});
			
			GLsizeiptr offset = 0;

			glGenVertexArrays(1, &_buffers.vertexArrayObject);
			glGenBuffers(1, &_buffers.staticVBO);

			//Any subsequent VBO, EBO, and AttribPointer calls will be stored in this VAO
			glBindVertexArray(_buffers.vertexArrayObject);
			//Bind our sprites vertex data to the GL_ARRAY_BUFFER
			glBindBuffer(GL_ARRAY_BUFFER, _buffers.staticVBO);
			glBufferData(GL_ARRAY_BUFFER, totalVertexVectorSize,
				0, GL_STATIC_DRAW);

			for (auto spriteSource = spriteResources.begin(); spriteSource != spriteResources.end(); ++spriteSource) {
				if (spriteSource->second->meshVerticies.size() > 0) {
					//spriteSource->second->offset = offset;
					InitMeshRenderData(spriteSource->second, offset);
				}
			}

			std::shared_ptr<Sprite::AttributeConfiguration> config = vertexAttributeConfigurations.at("testAttribs");

			GLuint strideMultiplier = std::accumulate(config->attributes.begin(), config->attributes.end(), 0,
				[](int sum, const Sprite::AttributeConfiguration::VertexAttribute& attribute) {
				return sum + attribute.size;
			});

	/*		GLuint strideMultiplier = std::accumulate(spriteResources[0]->attributes.begin(), spriteResources[0]->attributes.end(), 0,
				[](int sum, const Sprite::SpriteResource::VertexAttribute& attribute) {
				return sum + attribute.size;
			});
*/
			GLsizei vertexStride = strideMultiplier * sizeof(GLfloat);

			int counter = 0;

			//Another note -> the locations are highly dependent on how they are specified in the Vertex shader
			//may need a GLBindBuffer call in here...
			for (auto it = config->attributes.begin(); it != config->attributes.end(); it++) {
				if (it->size > 0) {
					//this is a shader location
					glEnableVertexAttribArray(it->location);
					glVertexAttribPointer(it->location, it->size, GL_FLOAT, GL_FALSE, vertexStride,
						(GLvoid*)(sizeof(GLfloat) * counter));
					counter += it->size;
				}
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//things were succesfully loaded
			return true;

		}

		//this only works for one VAO for now..
		//CHECK THIS TO SEE IF I AM LOADING THE DATA CORRECTLY - IT WORKS FOR ONE MESH BUT NOT TWO
		void ResourceManager::InitMeshRenderData(const Sprite::SpriteResource::SpriteSourceShared& spriteType, GLsizeiptr& offset) {

			//the offset should be zero at the start, the third arguement is the chuck out of the buffers size that we need to take
			glBufferSubData(GL_ARRAY_BUFFER, offset, spriteType->meshVerticies.size() * sizeof(GLfloat), 
				&spriteType->meshVerticies[0]);			

			//The stride data will always be the same if the vertex format is the same so it can probably be out of this loop
		
			//THERE MAY BE A STATE ISSUE WITH MY VBO....Also its only rendering 10 things so the spacing is off

			offset += spriteType->meshVerticies.size() * sizeof(GLfloat);
		}
	}

}


