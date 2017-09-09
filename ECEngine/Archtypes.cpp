#include "stdafx.h"

#include "./Archetypes.h"

namespace Engine {

	namespace Archtypes {

		EntityPointer DefaultCamera() {

			EntityPointer camera(new Entity); //make a new camera entity

			//attach camera related components to this new entity
			camera->AddComponent(ComponentPointer(new CameraComponent()));
			camera->AddComponent(ComponentPointer(new TransformComponent()));
			camera->SetName("Camera");			

			//camera->GET_COMPONENT(TransformComponent)->scale = glm::vec2(1600.0f, 800.0f);

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
			EntityPointer boxEntity(new Entity());
			//note that we make new components here
			//boxEntity->AddComponent(ComponentPointer(new RigidBody()));
			boxEntity->AddComponent(ComponentPointer(new TransformComponent()));
			boxEntity->AddComponent(ComponentPointer(new SpriteComponent()));
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