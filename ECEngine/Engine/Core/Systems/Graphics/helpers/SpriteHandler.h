#pragma once
#include <GL/glew.h> //this holds most of our opengl functions
#include <vector>
#include <memory>

namespace Engine {

	namespace Sprite {
		enum VertexAttributeType {
			POSITION,
			TEXTURE,
			COLOR
		};
		struct VertexAttribute {
			VertexAttribute() : size(0), location(0) {}
			GLint size;
			int location;
			VertexAttributeType type;
		};

		std::vector<VertexAttribute> attributes;

		struct SpriteData {
			SpriteData(std::string _name, std::vector<GLfloat> _verticies, std::vector<VertexAttribute> _arrtibutes) : 
				typeName(_name), verticies(_verticies), attributes(_arrtibutes) {};
			std::string typeName;
			std::vector<GLfloat> verticies;
			std::vector<VertexAttribute> attributes;
		};

		//using SpriteDataPointer = std::shared_ptr<SpriteData>;

		//class SpriteHandler {
		//public: 
		//	SpriteHandler

		//};
	}
}