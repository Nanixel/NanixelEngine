#include "stdafx.h"

#include "./Space.h"
#include "./Engine.h"

namespace Engine {

	extern Engine* ENGINE;

	Space::Space(std::string& name) : _name(name) {
		_camera = CreateCamera();
	}

	Space::~Space() {
		_entities.clear();
	}


	void Space::Update(float dt) {

		for (auto &system : _systems) {
			PopulateSystemEntities(system);
			system->Update(dt);
		}
	}

	void Space::AddSystem(SystemPointer system) {
		for (auto &sys : _systems) {
			if (sys == system) {
				throw std::exception("Attempted to add two copies of the same system.");
			}
		}
		_systems.push_back(system);
	}

	EntityPointer Space::CreateEntity() {
		_entities.push_back(EntityPointer(new Entity));
		return _entities.back();
	}

	EntityPointer Space::CreateCamera() {
		EntityPointer camera = ENGINE->Factory().create("DefaultCamera");
		_entities.push_back(camera);
		return camera;
	}

	EntityPointer Space::GetCamera() {
		return _camera;
	}


	//this is called on the gamestate
	void Space::PopulateSystemEntities(SystemPointer sys) const {
		//clear the current system entities and populate the system with some new ones
		sys->_entities.clear();
		if (sys->Mask() != MC_NOOBJECTS) {
			//foreach entity in this space, add it to each of our systems entities
			for (auto& it : _entities) {

				//add any entities living in this space that fit the system to the system's mask
				mask m = sys->Mask();
				if (it->CheckMask(m)) {
					sys->_entities.push_back(it);
				}			
			}
		}
	}


	EntityPointerList Space::GetEntities(mask m) const {
		EntityPointerList matches;

		for (auto it : _entities) {
			if (it->CheckMask(m)) {
				matches.push_back(it);
			}
		}
		return matches;
	}

	EntityPointer Space::GetEntityByName(std::string name) {
		for (auto entity : _entities) {
			if (entity->GetName() == name) {
				return entity;
			}
		}

		throw std::exception("Attempted to find an entity by name, but no entity with that name exists.");
	}

	void Space::RemoveEntity(EntityPointer entityToRemove) {
		for (auto& it : _entities) {

			if (it == entityToRemove) {
				it.reset();
				it = _entities.back();
				_entities.pop_back();
				return;
			}
		}
		throw ("Tried to remove an entity that doesnt exists");
	}

	void Space::AddEntity(EntityPointer entityToAdd) {
		_entities.push_back(entityToAdd);
	}

	void Space::ClearSpace() {
		//do not destory the camera
		EntityPointer camera = GetCamera();

		_entities.clear();
		_entities.push_back(camera);
		_systems.clear();
	}
}