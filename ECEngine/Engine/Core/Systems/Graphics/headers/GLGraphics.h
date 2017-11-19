#pragma once
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "../ECEngine/Engine/Core/Engine/headers/Entity.h"
#include "../ECEngine/Engine/Core/Components/CameraComponent.h"
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include <GL/glew.h>
#include <unordered_map>

namespace Engine {

	class CameraComponent;
	namespace Systems {		
		class GLGraphics : public System
		{
		public:	
			GLGraphics();

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message);
			void NewFrame();

		private:									
			ResourceManagerShared _resourceManager;
			void DrawEntity(const EntityPointer &entityToDraw, const CameraComponentPointer &camera);						;			
		};
	}	
}
#endif // !GLGRAPHICS_H




