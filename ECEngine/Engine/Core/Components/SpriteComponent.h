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
		//if the object is visible we do not want to draw it...
		bool visible = true;
	public:
		SpriteComponent() : Component(EC_SpriteComponent, MC_Sprite) {}

		enum MeshType {
			QUAD,
			CIRCLE
		};			

		glm::vec4 color = { 0.0f, 1.0f, 0.0f, 1.0f };
		std::string shaderName = "box";
		MeshType type = QUAD;
		int textureID;
	};

	using SpritePointer = std::shared_ptr<SpriteComponent>;

}

#endif // !SPRITECOMPONENT_H


