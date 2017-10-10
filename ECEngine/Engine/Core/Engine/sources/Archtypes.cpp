#include "stdafx.h"
#include "../ECEngine/Engine/Core/Engine/headers/Archetypes.h"


namespace Engine {

	namespace Archtypes {

		EntityPointer DefaultCamera() {

			EntityPointer camera = std::make_shared<Entity>();
		
			camera->AddComponent(std::make_shared<CameraComponent>());
			camera->AddComponent(std::make_shared<TransformComponent>());

			camera->GET_COMPONENT(CameraComponent)->projectionMode = CameraComponent::ProjectionMode::PERSPECTIVE;

			camera->SetName("Camera");						

			return camera;
		}

		EntityPointer Default2DCamera() {

			EntityPointer cameraEntity = std::make_shared<Entity>();
			cameraEntity->AddComponent(std::make_shared<CameraComponent>());

			cameraEntity->GET_COMPONENT(CameraComponent)->projectionMode = CameraComponent::ProjectionMode::ORTHOGRAPHIC;
			cameraEntity->GET_COMPONENT(CameraComponent)->viewType = CameraComponent::CameraViewType::OVERHEAD;

			cameraEntity->AddComponent(std::make_shared<TransformComponent>());
			cameraEntity->SetName("2DCamera");
			
			return cameraEntity;
		}

		EntityPointer BoxGameObject() {
			//dynamic memory allocation
			EntityPointer boxEntity = std::make_shared<Entity>();
	
			boxEntity->AddComponent(std::make_shared<TransformComponent>());
			boxEntity->AddComponent(std::make_shared<SpriteComponent>());
			boxEntity->SetName("box");

			return boxEntity;
		}


		EntityPointer Player() {

			EntityPointer playerEntity(new Entity());

			//why are we adding a pointer to an r-vaule
			//playerEntity->AddComponent(ComponentPointer(new RigidBody()));
			//playerEntity->AddComponent(ComponentPointer(new Transform()));
			//playerEntity->AddComponent(ComponentPointer(new Sprite()));

			playerEntity->SetName("Player");

			return playerEntity;
		}

	}


}