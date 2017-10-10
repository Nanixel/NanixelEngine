#pragma once
#include "../ECEngine/Engine/Core/Systems/Resources/headers/Resource.h"
#include <GL/glew.h> 
#include <memory>
#include <vector>

namespace Engine {

	
	namespace Sprite {



		class AttributeConfiguration {
		public:

			enum VertexAttributeType {
				POSITION,
				TEXTURE,
				COLOR,
				NONE
			};

			struct VertexAttribute {
				VertexAttribute(GLint _size, int _location, VertexAttributeType _type) : size(_size), location(_location), type(_type) {}
				GLint size;
				int location;
				VertexAttributeType type;
			};

			AttributeConfiguration() {}

			void AddAttribute(VertexAttribute attrib) {

			}

			std::vector<VertexAttribute> attributes;

			using VertexAttributesPointer = std::shared_ptr<VertexAttribute>;

		};

		class SpriteResource : public Resource {
		public:

			using SpriteDataPointer = SpriteResource*;
			using SpriteSourceShared = std::shared_ptr<SpriteResource>;		
	
			SpriteResource(std::string name) : Resource(name) {}
			
			SpriteResource() = delete;

			std::vector<GLfloat> meshVerticies;
			//these attributes are postion, color, texuture, etc...			
			
			int vertexStart;
			int vertexEnd;
			GLuint offset;
		};
	}

}