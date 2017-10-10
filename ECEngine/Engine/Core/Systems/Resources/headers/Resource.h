#pragma once
#include <memory>
#include <string>

namespace Engine {

	class Resource {
	public:

		using Ptr = Resource*;
		using StrongPtr = std::shared_ptr<Resource>;

		Resource(std::string name) : Name(name) {};

		//maybe some virtual classes for serialization and deserialization once I get to that point
		std::string& getName() {
			return Name;
		}

	protected:
		//The path of the file containing the resource on disk
		std::string Path;
		std::string Name;

	};

}

