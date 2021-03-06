#pragma once
#include <memory>

namespace Engine {


	using mask = unsigned;

	enum EnumeratedComponent {

		EC_NONE = 0,
		EC_DrawableComponent = 1,
		EC_SpriteComponent = 2,
		EC_CameraComponent = 3,
		EC_TransformComponent = 4,
		EC_ResourceComponent = 5,
		EC_Particle = 6,
		EC_LightSource = 7,
		//max components that exist
		EC_Max
	};

	enum BitFieldComponent {

		MC_ALIVE = 1,
		MC_Transform = 1 << 1,
		MC_Resource = 1 << 2,
		MC_Sprite = 1 << 3,
		MC_Camera = 1 << 4,
		MC_Particle = 1 << 5,
		MC_LightSource = 1 << 6,
		//system doesnt wish to recieve any objects
		MC_NOOBJECTS = -1
	};

	struct Component {
	public:

		Component(EnumeratedComponent type, BitFieldComponent mask) :_type(type), _mask(mask) {}

		const EnumeratedComponent _type;
		const BitFieldComponent _mask;

		//must be virtual so that derived component types deallocated correctly
		virtual ~Component() {}

	private:
		//must have a mask and type
		Component() = delete;
	};

	using ComponentPointer = std::shared_ptr<Component>;


}