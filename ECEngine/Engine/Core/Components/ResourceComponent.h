#pragma once
#include <string>
#include "../ECEngine/Engine/Core/Engine/headers/Component.h"

namespace Engine {

	class ResourceComponent : public Component {
	public:

		ResourceComponent() : Component(EnumeratedComponent::EC_ResourceComponent, MC_Resource) {}

		enum ResourceType {
			Resource_TextureFile,
			Resource_ShaderFile,
			Resource_MeshFile,
			Resource_MusicFile,
			Resource_VideoFile
		};

		std::string name;
		int ID;

	};

}