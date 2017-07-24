#include "stdafx.h"

#include "./SystemsInclude.h"
#include "./ComponentsInclude.h"
#include "./Engine.h"

#include <glm/gtc/matrix_transform.hpp>

//This a systems that manages entites with a camera component

namespace Engine {

	extern Engine *ENGINE;

	namespace Systems {

		//System name is an rvalue
		CameraSystem::CameraSystem() : System(std::string("Camera System"), SystemType::EC_CameraSystem) {

		}

		void CameraSystem::Init() {
			//the camera system is made up of the camera component and the transform component
			//entitys of this system will be acted on if they have these components
			RegisterComponent(BitFieldComponent::MC_Camera);
			RegisterComponent(BitFieldComponent::MC_Transform);
		}

		void CameraSystem::Update(float dt) {			

			//for all the entitys in this camera system which are most likely going to be cameras?
			for (auto& it : _entities) {
				//get the transform component on the object
				//"it" is what we care about here...cause it has all these components
				TransformComponentPointer transform = it->GET_COMPONENT(TransformComponent);
				//get the camera component of our entity (By default it is our default camera)
				CameraComponentPointer camera = it->GET_COMPONENT(CameraComponent);
				CustomWindowPointer window = GETSYS(CustomWindow);

				transform->velocity = transform->Speed * dt;

				//TODO -> change this later!!! this is hard coded for now!!
				std::pair<int, int> windowDimensions = std::pair<int, int>(1200, 800);
				
				glm::vec3 front;
				front.x = std::cos(glm::radians(transform->Yaw) * std::cos(glm::radians(transform->Pitch)));
				front.y = std::sin(glm::radians(transform->Pitch));
				front.z = std::sin(glm::radians(transform->Yaw) * cos(glm::radians(transform->Pitch)));
				camera->Front = glm::normalize(front);

				camera->Right = glm::normalize(glm::cross(camera->Front, camera->WorldUp));
				camera->Up = glm::normalize(glm::cross(camera->Right, camera->Front));

				camera->projectionMatrix = glm::perspective(glm::radians(camera->FOV), (float)windowDimensions.first / (float)windowDimensions.second, camera->NearPlane, camera->FarPlane);
				camera->viewMatrix = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
			
			}
		}

		void CameraSystem::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			//this is more or less a recieve message at this point...

			CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			TransformComponentPointer cameraTransform = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(TransformComponent);

			float velocity = cameraTransform->Speed * ENGINE->GetDt();

			switch (message) {
			case Message::MSG_A_PRESS:
				//left movement
				defaultCamera->Position -= defaultCamera->Right * velocity;
				break;
			case Message::MSG_D_PRESS:
				//right movement
				defaultCamera->Position += defaultCamera->Right * velocity;
				break;
			case Message::MSG_S_PRESS:
				defaultCamera->Position -= defaultCamera->Front * velocity;
				break;
			case Message::MSG_W_PRESS:
				defaultCamera->Position += defaultCamera->Front * velocity;
				break;
			case Message::MSG_ESC_PRESS:
				ENGINE->Stop();
				break;
			case Message::MSG_MOUSE_MOVE:
				std::cout << "Mouse is moving" << std::endl;
				break;
			}
		}

		void CameraSystem::ShutDown() {

		}

	}

}