#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace Engine {

	namespace Constants {
		const std::string VIEWUNIFORM = "view";
		const std::string MODELUNIFORM = "model";
		const std::string TEXTUREUNITFORM = "texture1";
		const std::string COLORUNIFORM = "color";
		const std::string PROJECTIONUNIFORM = "projection";

		const int SCREENWITDH = 800;
		const int SCREENHEIGHT = 600;

	}
}


#endif
