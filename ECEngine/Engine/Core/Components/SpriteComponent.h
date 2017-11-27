#pragma once

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include "../ECEngine/Engine/Core/Engine/headers/Component.h"


//This component serves as my "drawable component"

namespace Engine {

	class SpriteComponent : public Component {
	protected:
		bool visible = true;
	public:
		SpriteComponent() : Component(EC_SpriteComponent, MC_Sprite), IsDestroyed(false) {}
	
		//MeshType type = QUAD;

		glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };		
		std::string textureName = "base";
		std::string shaderName; // Sprite component should not have to know about this			
		std::string spriteTypeName;
		bool IsSolid;
		//doing this does not actually set the default.
		bool IsDestroyed = false;

		glm::vec3 LightSpecular;
		glm::vec3 DiffuseColor;
		glm::vec3 AmbientColor;

	};

	using SpritePointer = std::shared_ptr<SpriteComponent>;

}

#endif // !SPRITECOMPONENT_H


