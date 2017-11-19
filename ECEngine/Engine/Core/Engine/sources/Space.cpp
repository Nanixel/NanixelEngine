#include "stdafx.h"

#include "../ECEngine/Engine/Core/Engine/headers/Space.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"

namespace Engine {

	extern Engine* ENGINE;

	Space::Space(std::string& name) : _name(name) {
		_camera = CreateCamera();
	}

	Space::~Space() {
		_entities.clear();
	}

	//runs the updates for all the systems in this space
	void Space::Update(float dt) {
		//there could be a space transitions between iterations aka frames so we want to accont for that maybe
		//EvaluateEntites();
		for (auto &system : _systems) {
			//This seems bad -> arent the entities pointers anyway?
			PopulateSystemEntities(system);
			system->Update(dt);
		}
	}

	void Space::EvaluateEntites() {
		for (auto& it : _entities) {

			//add any entities living in this space that fit the system to the system's mask
			if (it->SpaceIdentifier() != _name) {
				RemoveEntity(it);
			}
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
		_entities.push_back(std::make_shared<Entity>());
		return _entities.back();
	}

	//this is kind of a limitation
	EntityPointer Space::CreateCamera() {
		EntityPointer camera = ENGINE->Factory().create("DefaultCamera");
		_entities.push_back(camera);
		return camera;
	}

	EntityPointer Space::GetCamera() {
		return _camera;
	}

	void Space::ChangeCamera(EntityPointer camera) {
		_camera = camera;
	}

	void Space::PopulateSystemEntities(SystemPointer sys) const {
		//TODO it might be beneficial to base each system off the current space's entities..that way dont have to add them each frame
		//there might be a problem when doing that because each system would then need to validate each entity...or any newly added ones for the mask
		sys->_entities.clear();
		//using Systems::GLGraphics;
		//GETSYS(GLGraphics)->ClearTextures();
		if (sys->Mask() != MC_NOOBJECTS) {
			//foreach entity in this space, add it to each of our systems entities
			for (auto& it : _entities) {
				std::string name = it->GetName();
				if (name == "particle" && sys->_name == "Particle System") {
					int value = 4;
				}
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

	//I can probably use dependecy injection here
	void Space::AddEntity(EntityPointer entityToAdd) {

		if (entityToAdd->SpaceIdentifier().empty() || entityToAdd->SpaceIdentifier() == _name) {
			entityToAdd->UpdateSpace(_name);
			_entities.push_back(entityToAdd);
		}
		else {
			//entity belongs to another space
			//throw for now, in the future we can explore "Transfering the entity"
			//in order to transfer the entity, this space must remove it from another space
			throw ("This entity belongs to another space");
		}
	}

	void Space::ClearSpace() {
		//do not destory the camera
		EntityPointer camera = GetCamera();

		_entities.clear();
		_entities.push_back(camera);
		_systems.clear();
	}
}