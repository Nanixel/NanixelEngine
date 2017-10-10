#pragma once
#include <memory>
#include <unordered_map>

namespace Engine {

	//template <typename ResourcesType>
	//class ResourceMap {

	//public:
	//	using ResourcePointer = std::shared_pointer<ResourcesType>;
	//	//the string in this case is a handle aka how it will be accessed
	//	using Map = std::unordered_map<std::string, ResourcePointer>;

	//	void Add(ResourcePointer resource) {
	//		Container.insert(std::pair<std::string, ResourcePointer>(resource->getName(), resource));
	//	}

	//	bool Remove(const std::string& name) {
	//		if (!Container.count(name)) {
	//			return false;
	//		}

	//		Container.erase(name);
	//		return true;
	//	}

	//	bool Has(const std::string& name) {
	//		if (!Container.count(name)) {
	//			return false;
	//		}
	//		return true;
	//	}

	//	ResourcePointer Find(const std::string& name) {
	//		if (!Container.count(name)) {
	//			return nullptr;
	//		}
	//		return Container.at(name);
	//	}

	//	const Map& All() { return Container; }

	//private:
	//	Map Container;
	//};

	//struct ResourceMaps {

	//	//ResourceMap<Resources::SpriteSource> SpriteVertexSource;
	//	//ResourceMap<Resources::TextureSource> Textures;
	//	//ResourceMap<Resources::SoundSource> Sounds;
	//	//ResourceMap<Resources::LevelSource> LevelSource;

	//};

}