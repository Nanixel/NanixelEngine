#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngine/Engine/Core/Components/ComponentsInclude.h"
#include <GL/glew.h>
#include <unordered_map>

namespace Engine {

	namespace Systems {
		const int PARTICLE_REGEN_RATE = 1;
		class ParticleSystem : public System
		{
		public:
		
			ParticleSystem();
			
			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message);


		private:
			void ReplaceParticle(EntityPointer particle);
			void UpadateParticleOrigin(ParticlePointer particle);
			GLuint FindLastUsedParticle();
			std::vector<EntityPointer> _deadParticles;
			GLuint _regeneratedParticles = 0;
			GLuint _lastUsedParticle = 0;

		};
		using ParticleSystemPointer = std::shared_ptr<ParticleSystem>;
	}
}



#endif