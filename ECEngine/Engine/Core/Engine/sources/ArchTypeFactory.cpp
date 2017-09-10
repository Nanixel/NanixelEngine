#include "stdafx.h"

#include "../ECEngine/Engine/Core/Engine/headers/ArchTypeFactory.h"


namespace Engine {


	using namespace Archtypes;

	ArchtypeFactory::ArchtypeFactory() {
		//add all archtype creating functions to the archtype map _archtypes

		//remember the DefaultCamara is a *CreateArchetypeFn
		AddArchtype("DefaultCamera", Archtypes::DefaultCamera);

		AddArchtype("2DCamera", Archtypes::Default2DCamera);

		AddArchtype("box", Archtypes::BoxGameObject);

		AddArchtype("Player", Archtypes::Player);

	}

	void ArchtypeFactory::AddArchtype(std::string archetypeName, CreateArchtypeFn creator) {
		_archtypes.emplace(archetypeName, creator);
	}

	EntityPointer ArchtypeFactory::create(std::string archtypeName) {

		ArchtypeMap::iterator it = _archtypes.find(archtypeName);

		if (it != _archtypes.end()) {
			//return the call to the function
			//call the function that we hold in the map
			return it->second();
		}

		throw std::range_error("Tried to create an archetype that doesnt exist.");

	}




}