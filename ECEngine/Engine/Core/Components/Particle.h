#pragma once
#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <memory>

namespace Engine {

	// Consider making a particle resource component that holds the respawn rate and max number of particles for that
	// partcile type.
	class Particle : public Component {
	public:
		Particle() : Component(EC_Particle, MC_Particle), Life(0.0f), IsOriginConstant(true) {}

		float Life;
		std::string OriginName;  // Origin Name should match an entity name
		glm::vec3 OriginPosition;
		//glm::vec3 Offset;
		bool IsOriginConstant;

	private:

	};

	using ParticlePointer = std::shared_ptr<Particle>;
}

#endif