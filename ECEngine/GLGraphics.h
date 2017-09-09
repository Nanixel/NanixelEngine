#pragma once
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "./Entity.h"
#include "./CameraComponent.h"
#include "./System.h"
#include <GL/glew.h> //this holds most of our opengl functions
#include <unordered_map>
#include "./ShaderUtility.h"

namespace Engine {
	class CameraComponent;
	namespace Systems {
		//GLGraphics should not care about what it is drawing, it should just take the shaders provided and draw the entity based on its component settings
		class GLGraphics : public System
		{
		public:
			//Mesh = collection of verticies and edges that define a shape of an object
			enum MeshType {QUAD = 0, CIRCLE};
			enum VertexAttributeType {
				POSITION = 0,
				COLOR = 1,
				TEXTURE
			};
			struct VertexAttribute {
				GLint size;								
				int location;
				VertexAttributeType type;
			};

			GLGraphics();

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message);

			void AddShader(std::string shaderName, Shaders::ShaderPointer shader);

			void InitRenderData(GLfloat *meshData, std::vector<VertexAttribute> meshAttributes);
			
		private:
			//these are just going to be matricies
			//These buffer objects hold on to the vertex info, texture info, location info, and color info
			struct MeshInfo {
				GLuint VertexBufferObject;
				GLuint VertexArrayObject;
				//this is used for not having to define verticies twice
				GLuint ElementBufferObject;

				MeshInfo() : VertexBufferObject(0), VertexArrayObject(0), ElementBufferObject(0) {}; //Init all buffers to zero at the start
			};			

			GLuint texture1;
			
			void DrawEntity(const EntityPointer &entityToDraw, const CameraComponentPointer &camera);			
			void InitiailzeBuffers();
			void ActivateTexture();						
			void DeleteMesh();
			void NewFrame();

			//this should be on the sprite component
			std::vector<VertexAttribute> meshAttributes;

			MeshInfo quadInfo, circleInfo;			
			Shaders::ShaderPointer GetShader(std::string shader);
			

			using ShaderMap = std::unordered_map<std::string, Shaders::ShaderPointer>;

			ShaderMap shadersDict;			
		};
	}	
}



#endif // !GLGRAPHICS_H






