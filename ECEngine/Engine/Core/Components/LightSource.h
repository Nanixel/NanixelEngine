#pragma once

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <string>
#include <glm/glm.hpp>
#include "../ECEngine/Engine/Core/Engine/headers/Component.h"

namespace Engine {

	class LightSource : public Component {
	protected:
		bool visible = true;
	public:
		LightSource() : Component(EC_LightSource, MC_LightSource) {}		

		glm::vec3 LightSpecular;
		glm::vec3 DiffuseColor;
		glm::vec3 AmbientColor;
		
	};

	using LightSourceShared = std::shared_ptr<LightSource>;

}

#endif


