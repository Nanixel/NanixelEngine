#pragma once
#include <GL/glew.h>
#include <unordered_map>
namespace Texture {

	class BaseTexture
	{
	public:
		BaseTexture() : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT),
			Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
			glGenTextures(1, &this->ID);
		}

		GLuint ID;
		GLuint Width, Height;

		GLuint Internal_Format; //Format of the texture object
		GLuint Image_Format;

		GLuint Wrap_S;
		GLuint Wrap_T;
		GLuint Filter_Min;
		GLuint Filter_Max;
	};
}
