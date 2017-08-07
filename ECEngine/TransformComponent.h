#pragma once
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "./Component.h"
#include <glm/glm.hpp>
#include <memory>

//transform component is used for all visable gameworld objects, remember: when we move the camera, we are moving everything in the game world

namespace Engine {

	class TransformComponent : public Component {

		//instantiate the base class with the correct types
	public:

		TransformComponent() : Component(EC_TransformComponent, MC_Transform), position(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f)),
			rotation(0.0f), speed(2.5f) {}

		glm::vec3 scale;
		glm::vec3 position;		

		float speed;
		float velocity;		
		float rotation;

		glm::mat4 modelMatrix;

	};
	using TransformComponentPointer = std::shared_ptr<TransformComponent>;
}

#endif // !TRANSFORMCOMPONENT_H


