#pragma once
#include <string>
#include <vector>
#include <memory>

#include "System.h"
#include "Entity.h"
#include "Component.h"

//A space is a world in which a game object can exist and interact

namespace Engine {

	class Space {
	public:

		Space(std::string& name);
		~Space();

		void Update(float);
		void AddSystem(SystemPointer);
		void RemoveSystem(std::string);
		void PopulateSystemEntities(SystemPointer system) const;

		EntityPointer CreateEntity();
		EntityPointer CreateCamera();
		EntityPointer GetCamera();


		EntityPointerList GetEntities(mask) const;
		EntityPointer GetEntityByName(std::string);

		//and and remove entity pointers from this space
		void RemoveEntity(EntityPointer entity);
		void AddEntity(EntityPointer entity);
		void ClearSpace();

		const std::string& Name() const { return _name; }

	private:
		std::string _name;
		EntityPointerList _entities;
		SystemPointerList _systems;
		EntityPointer _camera;

		//cannot be defaulted or copy constructed
		Space() = delete;
		Space(Space &Space) = delete;
	};

	using SpacePointer = std::shared_ptr<Space>;

}