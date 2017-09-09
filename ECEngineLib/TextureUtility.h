#pragma once
#include <GL/glew.h>
#include <vector>


namespace Texture {

	_declspec(dllexport) class TextureUtility
	{
	public:

		TextureUtility();
		~TextureUtility();

	private:
		std::vector<GLuint> textureIDs;

	};

}



