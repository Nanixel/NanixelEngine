#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace Engine {

	namespace Constants {
		const std::string VIEWUNIFORM = "view";
		const std::string MODELUNIFORM = "model";
		const std::string TEXTUREUNITFORM = "sprite";
		const std::string COLORUNIFORM = "color";
		const std::string PROJECTIONUNIFORM = "projection";
		const std::string OFFSET = "offset";
		const std::string ALPHA = "alphaValue";

		const int SCREENWITDH = 800;
		const int SCREENHEIGHT = 600;

		const float SCREEN_WIDTH_FLOAT = 800.0f;
		const float SCREEN_HEIGHT_FLOAT = 600.0f;

	}
}


#endif
