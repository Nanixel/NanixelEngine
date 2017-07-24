#pragma once
#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <memory>
#include "./System.h"


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
			void ConstrainMouse(float width, float height) {

			}

		};		
		using CameraSystemPointer = std::shared_ptr<CameraSystem>;
	}
}
#endif