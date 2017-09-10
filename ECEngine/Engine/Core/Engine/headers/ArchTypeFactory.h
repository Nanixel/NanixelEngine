#pragma once
#include <string>
#include <unordered_map>
#include "./Entity.h"
#include "./Archetypes.h"

namespace Engine {

	using ArchtypeMap = std::unordered_map<std::string, CreateArchtypeFn>;

	class ArchtypeFactory {
	public:
		//contructs archtype factory used to create uniform complex objects
		ArchtypeFactory();

		void AddArchtype(std::string archtype, CreateArchtypeFn creator);

		//creates an entity from an archtype
		//entity is not added to any space so the caller must do it manually
		EntityPointer create(std::string archetype);

	private:
		ArchtypeMap _archtypes;

	};

}