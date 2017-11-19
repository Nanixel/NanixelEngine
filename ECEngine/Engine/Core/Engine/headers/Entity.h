#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <string>
#include "Component.h"


namespace Engine {

	using ComponentPointerList = std::vector<ComponentPointer>;

	struct Entity {
	public:
		//adds a component to an entity via a pointer to that component
		void AddComponent(ComponentPointer component);

		void RemoveComponent(EnumeratedComponent componentType);

		bool HasComponent(EnumeratedComponent componentType);


		mask GetAllComponents();

		//allows checking if the entity has ALL the set of components specified by the mask value
		bool CheckMask(mask m);

		const std::string& GetName() const { return _name; }

		mask GetMask() const {
			return _mask;
		}

		void SetName(std::string name) {
			_name = name;
		}

		void UpdateSpace(std::string spaceIdentifier) {
			_spaceIdentifier = spaceIdentifier;
		}

		std::string& SpaceIdentifier() {
			return _spaceIdentifier;
		}

		
#define GET_COMPONENT( type ) \
		GetComponent<type>(EC_##type)

		template <typename T>
		std::shared_ptr<T> GetComponent(EnumeratedComponent componentType);

	private:
		std::string _name;
		std::string _spaceIdentifier = "";
		mask _mask = MC_ALIVE;
		EnumeratedComponent _collider = EC_NONE;
		ComponentPointer _components[EC_Max];
	};

	//template implementation needs to have global visability
	//this is bc separate implementations must be generated each usage (see macro)
	template <typename T>
	std::shared_ptr<T> Entity::GetComponent(EnumeratedComponent componentType) {
		//verify that the component type is attached to the entity
		assert(_components[componentType]);

		//what does this do?????????????????????
		return std::static_pointer_cast<T>(_components[componentType]);
	}

	using EntityPointer = std::shared_ptr<Entity>;
	using EntityPointerList = std::vector<EntityPointer>;


}