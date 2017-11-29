#include "stdafx.h"

#include "../ECEngine/Engine/Core/Systems/SystemsInclude.h"

#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Engine {

	extern Engine *ENGINE;

	namespace Systems {
		
		CameraSystem::CameraSystem() : System(std::string("Camera System"), SystemType::EC_CameraSystem) {

		}

		void CameraSystem::Init() {			
			//Basically saying to only care about entities that follow this bit mask
			RegisterComponent(BitFieldComponent::MC_Camera);
			RegisterComponent(BitFieldComponent::MC_Transform);
		}
		
		//TODO - Sensitivity doesnt seem to have any affect here...
		void CameraSystem::Update(float dt) {						

			for (auto& it : _entities) {				
				CameraComponentPointer camera = it->GET_COMPONENT(CameraComponent);												

				switch (camera->viewType)
				{
				case CameraComponent::CameraViewType::FIRST_PERSON:
				case CameraComponent::CameraViewType::THIRD_PERSON:
					SetupCameraRotation(camera);
					UpdateCameraVectors(camera);
					break;
				case CameraComponent::CameraViewType::ISOMETRIC:
					//do something with isometric view
					break;
				case CameraComponent::CameraViewType::OVERHEAD:
					//adjust/setup 2D camera
					break;
				default:
					break;
				}

				std::pair<int, int> windowDimensions = std::pair<int, int>(800, 600);				
				
				if (camera->projectionMode == CameraComponent::ProjectionMode::PERSPECTIVE) {
					camera->projectionMatrix = glm::perspective(glm::radians(camera->FOV), (float)windowDimensions.first / (float)windowDimensions.second, camera->NearPlane, camera->FarPlane);
					camera->viewMatrix = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
				}
				else if (camera->projectionMode == CameraComponent::ProjectionMode::ORTHOGRAPHIC) {
					//HARD CODED FOR NOW
					camera->projectionMatrix = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
				}							
			}
		}
		
		void CameraSystem::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			
			CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			TransformComponentPointer cameraTransform = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(TransformComponent);						
			cameraTransform->constantVelocity = cameraTransform->speed * ENGINE->GetDt();
				
			//TODO Camera Position can be on the transform component rather than the camera component

			switch (message) {
			case Message::MSG_A_PRESS:
				if (defaultCamera->viewType == CameraComponent::CameraViewType::FIRST_PERSON) {
					defaultCamera->Position -= defaultCamera->Right * cameraTransform->constantVelocity;
				}				
				break;
			case Message::MSG_D_PRESS:				
				if (defaultCamera->viewType == CameraComponent::CameraViewType::FIRST_PERSON) {
					defaultCamera->Position += defaultCamera->Right * cameraTransform->constantVelocity;
				}
				break;
			case Message::MSG_S_PRESS:
				if (defaultCamera->viewType == CameraComponent::CameraViewType::FIRST_PERSON) {
					defaultCamera->Position -= defaultCamera->Front * cameraTransform->constantVelocity;
				}
				break;
			case Message::MSG_W_PRESS:
				if (defaultCamera->viewType == CameraComponent::CameraViewType::FIRST_PERSON) {
					defaultCamera->Position += defaultCamera->Front * cameraTransform->constantVelocity;
				}
				break;			
			}
		}				

		void CameraSystem::ShutDown() {

		}


		//###################################### Helpers 

		//This will be updated with qaurtons at some point in the future
		void CameraSystem::SetupCameraRotation(CameraComponentPointer camera) {

			CustomWindowPointer window = GETSYS(CustomWindow);

			float currentMousePositionX = window->GetMousePosition().x;
			float previousMousePositionX = camera->MousePosition.x;
			float currentMousePositionY = window->GetMousePosition().y;
			float previousMousePositionY = camera->MousePosition.y;

			float xOffset = currentMousePositionX - previousMousePositionX;
			float yOffset = previousMousePositionY - currentMousePositionY;

			camera->MousePosition.x = currentMousePositionX;
			camera->MousePosition.y = currentMousePositionY;

			xOffset *= camera->Sensitivity;
			yOffset *= camera->Sensitivity;

			camera->Yaw += xOffset;
			camera->Pitch += yOffset;

			if (true) {
				if (camera->Pitch > 89.0f) {
					camera->Pitch = 89.0f;
				}
				if (camera->Pitch < -89.0f) {
					camera->Pitch = -89.0f;
				}
			}
		}

		void CameraSystem::UpdateCameraVectors(CameraComponentPointer camera) {
			glm::vec3 front;
			front.x = std::cos(glm::radians(camera->Yaw) * std::cos(glm::radians(camera->Pitch)));
			front.y = std::sin(glm::radians(camera->Pitch));
			front.z = std::sin(glm::radians(camera->Yaw)) * cos(glm::radians(camera->Pitch));
			camera->Front = glm::normalize(front);

			camera->Right = glm::normalize(glm::cross(camera->Front, camera->WorldUp));
			camera->Up = glm::normalize(glm::cross(camera->Right, camera->Front));
		}

		//Possibly use this later
		void CameraSystem::MoveCamera(CameraComponentPointer defaultCamera) {
			TransformComponentPointer cameraTransform = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(TransformComponent);
			cameraTransform->constantVelocity = cameraTransform->speed * ENGINE->GetDt();

			if (defaultCamera->projectionMode == CameraComponent::ProjectionMode::ORTHOGRAPHIC) {

			}

			if (defaultCamera->projectionMode == CameraComponent::ProjectionMode::PERSPECTIVE) {

			}
		}
	}

}