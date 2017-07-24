#pragma once
#include <memory>

namespace Engine {


	using mask = unsigned;

	enum EnumeratedComponent {
		//all components that attach to entities -> used for indexing components
		//The value of an enum here is the index on the entity of the component
		EC_NONE = 0,
		EC_Drawable = 1,
		EC_SpriteComponent = 2,
		EC_CameraComponent = 3,
		EC_TransformComponent,

		//max components that exist
		EC_Max
	};

	enum BitFieldComponent {
		//these are used for rapidly checking which components are attached to an entity
		//they can be ORed together as an entity may have many components -> able to check in a single swoop
		MC_ALIVE = 1,
		MC_Transform = 1 << 1,
		MC_Drawable = 1 << 2,
		MC_Sprite = 1 << 3,
		MC_Camera = 1 << 4,
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

	//will die once no references are on this component
	using ComponentPointer = std::shared_ptr<Component>;


}