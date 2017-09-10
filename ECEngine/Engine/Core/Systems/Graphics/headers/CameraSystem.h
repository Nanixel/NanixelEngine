#pragma once
#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <memory>
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngine/Engine/Core/Components/ComponentsInclude.h"


namespace Engine {

	namespace Systems {

		class CameraSystem : public System {

		public:
			CameraSystem();

			void Init();
			void Update(float);
			void ShutDown();
			void SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message);

	
		private:			
			void SetupCameraRotation(CameraComponentPointer camera);
			void UpdateCameraVectors(CameraComponentPointer camera);
			void MoveCamera(CameraComponentPointer camera);
		};		
		using CameraSystemPointer = std::shared_ptr<CameraSystem>;
	}
}
#endif