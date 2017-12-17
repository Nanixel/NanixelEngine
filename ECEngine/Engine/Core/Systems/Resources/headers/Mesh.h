#pragma once
#include <GL/glew.h> 
#include <memory>
#include <vector>

#include "./BaseTexture.h"

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


			struct Vertex {
				Vertex(glm::vec3 position, glm::vec2 texCoords, glm::vec3 norm) : Position(position),
					TexCoords(texCoords), Normal(norm) {}
				glm::vec3 Position;
				glm::vec2 TexCoords;
				glm::vec3 Normal;
			};

			/*struct TextureMat {
				TextureMat(unsigned int id, std::string type) : id(id), type(type) {}
				unsigned int id;
				std::string type;
			};*/

			// std::vector<unsigned int> indicies, _indicies(indicies), 
			Mesh(std::string name, std::vector<Vertex> verticies, int numberOfVerticies, int startIndex, MeshType type) : Name(name),
				_verticies(verticies), VerticiesCount(numberOfVerticies), Type(type) {
				VertexStart = startIndex;
				VertexEnd = VertexStart + numberOfVerticies;

				SetUpMesh();
			}

			GLsizei CalculateMemoryBlock() {
				//Need to change this to not be hard coded.
				//int numberOfElementsPerRow = POSITION_COORDS + TEXTURE_COORDS;
				int vertexArrayCount = TotalAttributesSize * VerticiesCount;
				return vertexArrayCount * sizeof(Vertex);
			}

			GLsizei CalculateIndexBlock() {
				return _indicies.size() * sizeof(unsigned int);
			}

			void SubstituteData(GLsizei& offset) {
				GLsizei memoryBlock = CalculateMemoryBlock();
				glBufferSubData(GL_ARRAY_BUFFER, offset, memoryBlock,
					&_verticies[0]);
				// No longer need to hold on to vertex data.
				_verticies.clear();
				offset += memoryBlock;
			}	

			void SubstituteIndexData(GLsizei& offset) {
				GLsizei memBlock = CalculateIndexBlock();
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, memBlock, &_indicies[0]);
				_indicies.clear();
				offset += memBlock;
			}
			
			void AddTexture(Texture::TexturePointer texture) {
				_textures.push_back(texture);
			}

			void SetUpTexture(Shaders::ShaderPointer shader) {

				int diffuse = 1;
				int specularNr = 1;				

				for (int i = 0; i < _textures.size(); i++) {					

					_textures.at(i)->Activate(i);

					std::string number;
					std::string typeName = _textures.at(i)->Type;					
					if (typeName == "diffuse") {												
						number = std::to_string(diffuse);
					}
					else if (typeName == "specular") {
												
						number = std::to_string(specularNr);
					}				
													
					std::string uniformName = "material." + typeName + number;

					shader->UpdateUniforms(uniformName, i);
				}			

				//Texture::TexturePointer texture2 = _resourceManager->GetTexture("matrix");
				//texture2->Activate(2);

				glActiveTexture(GL_TEXTURE0);

			}

			void Activate() {
				glBindVertexArray(VAO);
			}

			Mesh() = delete;
			int VertexStart = 0;
			int VertexEnd = 0;
			std::string Name;
			int VerticiesCount;
			MeshType Type;
			int TotalAttributesSize = 0;

		private:
			std::vector<Vertex> _verticies;
			std::vector<unsigned int> _indicies;
			std::vector<Texture::TexturePointer> _textures;

			//std::string Name;

			unsigned int VAO, VBO, EBO;


			void SetUpMesh() {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				//glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);

				glBufferData(GL_ARRAY_BUFFER, _verticies.size() * sizeof(Vertex), &_verticies[0],
					GL_STATIC_DRAW);

				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicies.size() * sizeof(unsigned int), &_indicies[0],
					//GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

				glBindVertexArray(0);
			}
		};

		using MeshPtr = Mesh*;
		using MeshShared = std::shared_ptr<Mesh>;
	}

}