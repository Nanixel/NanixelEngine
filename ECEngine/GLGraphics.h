#pragma once
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "./Entity.h"
#include "./CameraComponent.h"
#include "./System.h"
#include <GL/glew.h> //this holds most of our opengl functions
#include <unordered_map>
#include "./GLShader.h"

namespace Engine {
	class CameraComponent;
	namespace Systems {
		//class Entity;
		class GLGraphics : public System
		{
		public:
			//Mesh = collection of verticies and edges that define a shape of an object
			enum MeshType {QUAD = 0, CIRCLE};

			GLGraphics();

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message);

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

			//Draw the sqaures
			void DrawEntity(const EntityPointer &entityToDraw, const CameraComponentPointer &camera);
			//use this to create the sqaures - matrix
			void CreateMesh();
			//use this to delete the sqaures
			void DeleteMesh();

			MeshInfo quadInfo, circleInfo;

			ShaderPointer GetShader(std::string shader);
			void AddShader(std::string shaderName, ShaderPointer shader);

			using ShaderMap = std::unordered_map<std::string, ShaderPointer>;

			ShaderMap shadersDict;			
		};
	}	
}



#endif // !GLGRAPHICS_H






