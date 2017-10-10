#pragma once
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "../ECEngine/Engine/Core/Engine/headers/Entity.h"
#include "../ECEngine/Engine/Core/Components/CameraComponent.h"
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include <GL/glew.h> //this holds most of our opengl functions
#include <unordered_map>
#include "./ShaderUtility.h"
//#include "../ECEngine/Engine/Core/Systems/Graphics/helpers/SpriteHandler.h"

namespace Engine {

	class CameraComponent;
	namespace Systems {
		//GLGraphics should not care about what it is drawing, it should just take the shaders provided and draw the entity based on its component settings
		class GLGraphics : public System
		{
		public:	

			GLGraphics();

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entity2, Message::Message message);

			void AddShader(std::string shaderName, Shaders::ShaderPointer shader);
			
		private:									

			//GLuint _vertexArrayObject;
			//GLuint _staticBufferID;
			//GLuint _dynamicBufferID;
			//GLuint _staticElementBufferID;
			//GLuint _dynamicElementBufferID;

			//this map will be used to store all of the mesh verticies of a particular space
			//std::unordered_map<std::string, Sprite::SpriteDataPointer> spriteDataMap;
			
			void DrawEntity(const EntityPointer &entityToDraw, const CameraComponentPointer &camera);			
			void InitiailzeBuffers();
			void ActivateTexture();						
			void DeleteMesh();
			void NewFrame();

			Shaders::ShaderPointer GetShader(std::string shader);
			

			using ShaderMap = std::unordered_map<std::string, Shaders::ShaderPointer>;

			ShaderMap shadersDict;			
		};
	}	
}



#endif // !GLGRAPHICS_H

//to be added later

//Use static frame objects to store the data that doesnt change everyframe - textures and colors
//MeshInfo staticFrameObjects;
//Use dynamic frame objects to store the data that does change everyframe - position, lighting
//MeshInfo dynamicFrameObjects;




