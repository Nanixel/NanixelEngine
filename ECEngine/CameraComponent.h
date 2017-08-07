#pragma once
#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./Component.h"
#include "SFML/Window.hpp"


namespace Engine {

	namespace Systems {

		class CameraSystem;
		class GLGraphics;
		class CustomWindow;
	}

	class CameraComponent : public Component {
	public:
		enum ProjectionMode {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		enum CameraViewType {
			FIRST_PERSON,
			THIRD_PERSON,
			ISOMETRIC
		};


		CameraComponent() : Component(EC_CameraComponent, MC_Camera) {}

		//defualt to perspective since I am building a 3D engine
		ProjectionMode projectionMode = PERSPECTIVE;
		CameraViewType viewType = FIRST_PERSON;

		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 0.1f, 0.0f);
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);

		bool Active;

		//Settings
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 100.0f;
		float Sensitivity = 0.2f;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		float Roll = 0.0f;
		float Size = 90; //Size of orthographic projection
						
		glm::vec2 MousePosition = glm::vec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

		//this class has access to the protected members of these classes
		friend class Systems::CameraSystem;
		friend class Systems::GLGraphics;
		friend class Systems::CustomWindow;
	private:
		glm::mat4 projectionMatrix;
		//dont mind this for now
		//glm::mat4 ortoMatrix;
		glm::mat4 viewMatrix;

	};

	using CameraComponentPointer = std::shared_ptr<CameraComponent>;

}


#endif // !CAMERA_COMPONENT_H
