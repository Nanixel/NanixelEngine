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
		SpriteComponent() : Component(EC_SpriteComponent, MC_Sprite) {}
		
		//enum MeshType {
		//	QUAD,
		//	CIRCLE
		//};

		//MeshType type = QUAD;


		glm::vec3 color = { 0.0f, 1.0f, 0.0f };
		//needs to be base if we only want the object to be colored and not have a texture
		std::string textureName = "base";
		std::string shaderName;			
		//GLuint vertexBufferSize;
		std::string spriteTypeName;

	};

	using SpritePointer = std::shared_ptr<SpriteComponent>;

}

#endif // !SPRITECOMPONENT_H


