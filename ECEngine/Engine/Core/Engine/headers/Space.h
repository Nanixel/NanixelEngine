#pragma once
#include <string>
#include <vector>
#include <memory>

#include "System.h"
#include "Entity.h"
#include "Component.h"
//#include "../ECEngine/Engine/Core/Systems/Resources/headers/SptireResource.h"
//A space is a world in which a game object can exist and interact

namespace Engine {

	enum CameraType {
		FIRST_PERSON,
		THIRD_PERSON,
		ISOMETRIC,
		TWO_DIMENTIONAL_SIDE_SCROLLER,
		TWO_DIMENTIONAL_OVERHEAD,
		NONE //define your own camera
	};

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
		void ChangeCamera(EntityPointer camera);

		const std::string& Name() const { return _name; }

	private:
		EntityPointer _camera;
		std::string _name;
		EntityPointerList _entities;
		SystemPointerList _systems;
		void EvaluateEntites();
		//cannot be defaulted or copy constructed
		Space() = delete;
		Space(Space &Space) = delete;
	};

	using SpacePointer = std::shared_ptr<Space>;

}