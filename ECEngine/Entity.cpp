#include "stdafx.h"
#include "./Entity.h"
#include "./ComponentsInclude.h"

#include <cassert>
#include <memory>

namespace Engine {

	void Entity::AddComponent(std::shared_ptr<Component> component) {
		//cant continuosly add the same component
		assert(_components[component->_type].get() == nullptr);

		_components[component->_type] = component;

		//add component mask to the entity
		_mask |= component->_mask;

		//Set entities collider type -> TODO: once I add physics to my engine

	}

	void Entity::RemoveComponent(EnumeratedComponent componentEnum) {

		//terminate if the user tries to remove a component that is not on this entity
		assert(_components[componentEnum] != nullptr);

		if (_components[componentEnum]) {

			//remove the component's mask from the entity mask
			_mask &= ~(_components[componentEnum]->_mask);

			//release the component from the shared_ptr
			_components[componentEnum].reset();
		}

		//Add collider stuff here

	}

	bool Entity::HasComponent(EnumeratedComponent componentEnum) {
		//must be explicitly cast, as it does not allow implicit conversions
		return static_cast<bool>(_components[componentEnum]);
	}

	bool Entity::CheckMask(mask m) {
		return ((_mask & m) == m);
	}
}