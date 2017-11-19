#pragma once
#include <GL/glew.h>
#include <unordered_map>
namespace Texture {

	class BaseTexture
	{
	public:
		BaseTexture(bool alpha) : Width(0), Height(0), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
			if (alpha) {
				Internal_Format = GL_RGBA;
				Image_Format = GL_RGBA;
				Wrap_S = GL_CLAMP_TO_EDGE;
				Wrap_T = GL_CLAMP_TO_EDGE;
			}
			else {
				Internal_Format = GL_RGB;
				Image_Format = GL_RGB;
				Wrap_S = GL_REPEAT;
				Wrap_T = GL_REPEAT;
			}

			glGenTextures(1, &this->ID);
		}

		void Generate(int width, int height, unsigned char *data) {
			Width = width;
			Height = height;

			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);

			//Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Activate(int textureIndex) {
			glActiveTexture(GL_TEXTURE0 + textureIndex);
			glBindTexture(GL_TEXTURE_2D, ID);
		}

		void Deactivate() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//This is only needed if I need to perform multi-texturing for an entity
		//int texturesCount = manager->InitializeTexture(sprite ->textureName, _loadedTextures);
		//std::ostringstream oss;
		//for (int i = 0; i < texturesCount; i++) {
		//	//Need to do some string appending to the constant here.
		//	oss << Constants::TEXTUREUNITFORM << i;
		//	shaderProgram->UpdateUniforms(oss.str(), i);
		//	oss.clear();
		//}

		//this is used when something has multiple textures.
		//int ResourceManager::InitializeTexture(const std::string textureName, std::unordered_map<std::string, int>& loadedTextures) {
		//	auto texture = _textures.find(textureName);
		//	int uniformPostfix = 0;
		//	if (texture != _textures.end()) {
		//		
		//		auto loadedTexture = loadedTextures.find(textureName);
		//		if (loadedTexture == loadedTextures.end()) {
		//			loadedTextures.emplace(textureName, loadedTextures.size() > 0 ? loadedTextures.size() - 1 : 0);				
		//		} 														
		//		// Gareenteed to have something at this point.
		//		uniformPostfix = loadedTextures.size() - 1;
		//		glActiveTexture(GL_TEXTURE0 + loadedTextures.size() - 1);
		//		glBindTexture(GL_TEXTURE_2D, (*texture).second->ID);
		//	}
		//	return uniformPostfix;
		//}

		// I can probably make alot of these values private now.
		GLuint ID;
		GLuint Width, Height;

		GLuint Internal_Format; //Format of the texture object
		GLuint Image_Format;

		GLuint Wrap_S;
		GLuint Wrap_T;
		GLuint Filter_Min;
		GLuint Filter_Max;
	};
	using TexturePointer = std::shared_ptr<Texture::BaseTexture>;
}
