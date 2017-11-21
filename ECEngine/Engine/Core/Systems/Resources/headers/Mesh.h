#pragma once
#include <GL/glew.h> 
#include <memory>
#include <vector>

namespace Engine {

	namespace Sprite {

		const int POSITION_COORDS = 3;
		const int TEXTURE_COORDS = 2;

		class Mesh {
		public:

			enum MeshType {
				QUAD,
				LIGHT_SOURCE,
			};


			Mesh(std::string name, std::vector<GLfloat> verticies, int numberOfVerticies, int startIndex, MeshType type) : Name(name), 
				_verticies(verticies), VerticiesCount(numberOfVerticies), Type(type) {
				VertexStart = startIndex;
				VertexEnd = VertexStart + numberOfVerticies;			
			}

			GLsizei CalculateMemoryBlock() {
				//Need to change this to not be hard coded.
				int numberOfElementsPerRow = POSITION_COORDS + TEXTURE_COORDS;
				int vertexArrayCount = numberOfElementsPerRow * VerticiesCount;
				return vertexArrayCount * sizeof(GLfloat);
			}

			void SubstituteData(GLsizeiptr& offset) {
				GLsizei memoryBlock = CalculateMemoryBlock();
				glBufferSubData(GL_ARRAY_BUFFER, offset, memoryBlock,
					&_verticies[0]);
				// No longer need to hold on to vertex data.
				_verticies.clear();
				offset += memoryBlock;
			}	
			
			Mesh() = delete;
			int VertexStart = 0;
			int VertexEnd = 0;
			std::string Name;
			int VerticiesCount;
			MeshType Type;

		private:
			std::vector<GLfloat> _verticies;
			//Possibly move verticies here
		};
		using MeshPtr = Mesh*;
		using MeshShared = std::shared_ptr<Mesh>;
	}

}