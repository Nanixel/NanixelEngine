#include "stdafx.h"
#include "../ECEngine/Engine/Core/Systems/Graphics/headers/ParticleSystem.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"

namespace Engine {

	extern Engine *ENGINE;

	namespace Systems {

		ParticleSystem::ParticleSystem() : System(std::string("Particle System"), SystemType::EC_ParticleSystem) {

		}

		void ParticleSystem::Init() {
			RegisterComponent(BitFieldComponent::MC_Particle);
			//the system does not need these to actually perform work on them, this just garentees that the entity has them
			RegisterComponent(BitFieldComponent::MC_Sprite);
			RegisterComponent(BitFieldComponent::MC_Transform);
		}

		void ParticleSystem::Update(float dt) {

	/*		for (auto& it : _deadParticles) {
				ReplaceParticle(it);
				_deadParticles.clear();
			}*/

			if (_entities.size() > 0) {
				for (GLuint i = 0; i < PARTICLE_REGEN_RATE; i++) {
					int unusedParticle = FindLastUsedParticle();
					ReplaceParticle(_entities.at(unusedParticle));
				}
			}						

			// TODO make sure this is not working on entities that are NOT particles
			for (auto& it : _entities) {
				//get the first ununsed particle out of all of my entities

				ParticlePointer particle = it->GET_COMPONENT(Particle);
				TransformComponentPointer transform = it->GET_COMPONENT(TransformComponent);
				SpritePointer sprite = it->GET_COMPONENT(SpriteComponent);

				//if (!particle->IsOriginConstant) {
				//	UpadateParticleOrigin(particle);
				//}

				////consider using a queue or stack for the particles
				////also consider sorting the list before you get here - possibly on Add?
				//if (particle->Life <= 0 && _regeneratedParticles < PARTICLE_REGEN_RATE) {
				//	sprite->IsDestroyed = true;
				//	ReplaceParticle(it);
				//	_regeneratedParticles++;
				//}

				// TODO THE PROBLEM HERE IS THAT DT GETS SO LARGE IT KILLS THE PATICLE RIGHT AWAY AND THEREFORE IT DOES NOT RESPAWN
				particle->Life -= dt;
				if (particle->Life > 0.0f) {
					transform->position -= transform->Velocity * dt;
					sprite->color.a -= dt * 2.5;
					sprite->IsDestroyed = false;
				}
				else {
					// Uncessary reassignment
					sprite->IsDestroyed = true;
				}
			}
			_regeneratedParticles = 0;
			auto n = std::count_if(_entities.begin(), _entities.end(),
				[](const EntityPointer o) {return o->GET_COMPONENT(SpriteComponent)->IsDestroyed == false; });

			//std::cout << "COUNT " << n << std::endl;
			//std::cout << "LAST USED: " << _lastUsedParticle << std::endl;
			//std::cout << "DT: " << dt << std::endl;
		}

		GLuint ParticleSystem::FindLastUsedParticle() {
			if (_entities.size() > 0) {
				for (GLuint i = _lastUsedParticle; i < 500; i++) {
					if (_entities.at(i)->GET_COMPONENT(Particle)->Life <= 0.0f) {
						_lastUsedParticle = i;
						return i;
					}
				}

				for (GLuint i = 0; i < _lastUsedParticle; i++) {
					if (_entities.at(i)->GET_COMPONENT(Particle)->Life <= 0.0f) {
						_lastUsedParticle = i;
						return i;
					}
				}

				_lastUsedParticle = 0;
				return 0;
			}			
		}

		void ParticleSystem::ShutDown() {

		}

		void ParticleSystem::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {

		}

		void ParticleSystem::ReplaceParticle(EntityPointer deadParticle) {		

			ParticlePointer particle = deadParticle->GET_COMPONENT(Particle);
			TransformComponentPointer transform = deadParticle->GET_COMPONENT(TransformComponent);
			SpritePointer sprite = deadParticle->GET_COMPONENT(SpriteComponent);
			// TODO Change this hardcoded value.
			EntityPointer origin = ENGINE->GetSpace("Game")->GetEntityByName(particle->OriginName);

			GLfloat random = ((rand() % 100) - 50) / 10.0f;
			GLfloat randomColor = 0.5f + ((rand() % 100) / 100.0f);
			transform->position = origin->GET_COMPONENT(TransformComponent)->position + random + transform->Offset;
			transform->position = glm::vec3(transform->position.x, transform->position.y, 0.0f);
			sprite->color = glm::vec4(randomColor, randomColor, randomColor, 1.0f);
			//sprite->Alpha = 1.0f;
			sprite->IsDestroyed = false;  // I want to figure out a better way to do this because the particles will still be
			// loaded into memory..so all 500 will exist weather they are destoryed or not.
			particle->Life = 1.0f;
			transform->Velocity = origin->GET_COMPONENT(TransformComponent)->Velocity * 0.1f;
		}

		void ParticleSystem::UpadateParticleOrigin(ParticlePointer particle) {

		}
	}

}

