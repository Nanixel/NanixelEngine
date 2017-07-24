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
			rotation(0.0f), Speed(2.5f) {}

		glm::vec3 scale;
		glm::vec3 position;

		//these have more to do with movement....so they can probably be their own component
		float Yaw;
		float Pitch;

		//move these into a movement component....maybe
		float Speed;
		float velocity;
		//this is just the degrees value that will be converted into radians
		float rotation;

		glm::mat4 modelMatrix;

	};
	using TransformComponentPointer = std::shared_ptr<TransformComponent>;
}

#endif // !TRANSFORMCOMPONENT_H


