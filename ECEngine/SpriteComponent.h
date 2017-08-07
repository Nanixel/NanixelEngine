#pragma once

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include "./Component.h"
#include "./GLShader.h"

namespace Engine {

	class SpriteComponent : public Component {
	public:
		SpriteComponent() : Component(EC_SpriteComponent, MC_Sprite) {}

		enum MeshType {
			QUAD,
			CIRCLE
		};

		glm::vec4 color = { 0.0f, 1.0f, 0.0f, 1.0f };
		std::string shaderName = "box";
		MeshType type = QUAD;
	};

	using SpritePointer = std::shared_ptr<SpriteComponent>;

}

#endif // !SPRITECOMPONENT_H


