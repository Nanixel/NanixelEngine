#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

	namespace Systems {

		//Do I want this to be an interface for the Resource itself or the Resource Manager
		class IResourceManager {
		public:

			IResourceManager(std::string name) : _identifier(name) {};

			virtual ~IResourceManager();

			virtual void Load();

			//need to do some templating right here
			virtual void Clear() = 0;
			//virtual void Serialize() = 0;

			virtual void Activate() = 0;

			virtual void GetResource() = 0;


			std::string& GetName() {
				return _identifier;
			}

		protected:
			std::string _identifier;
			//do some templating right here for the type
			std::unordered_map<std::string, int> _resources;

		};

	}

}

