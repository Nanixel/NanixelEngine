#include "stdafx.h"

#include "../ECEngine/Engine/Core/Systems/Graphics/headers/GLGraphics.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"

#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //convert glm data type to raw pointers
#include <math.h>
#include <numeric>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp> //scale, rotate, translate, projection, ortho

namespace Engine {
	
	extern Engine *ENGINE;
	namespace Systems {

		GLGraphics::GLGraphics() : System(std::string("GLGraphicsSystem"), SystemType::EC_GLGraphics)
		{
			//DO some dependency injection
		}

		void GLGraphics::Init() {

			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Sprite);					
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable(GL_MULTISAMPLE);
			if (glewInit() != GLEW_OK) {
				std::cout << "ERROR::GLGRAPICS::FAILED_INITIALIZING_GLEW_INIT" << std::endl;
			}				

			//Consider using a raw pointer here. since the graphics system does not need to own a resource manager...it just needs a pointer/reference to it.
			_resourceManager = ENGINE->GetResourceManager();
			_resourceManager->LoadDefaultShaders();
				
		}		

		void GLGraphics::Update(float dt) {
			CameraComponentPointer camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
		
			//NewFrame();
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			for (auto& it : _entities) {
				DrawEntity(it, camera);
			}
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void GLGraphics::ShutDown() {
			//DeleteMesh();
		}

		//the graphics SYSTEM will send messages from one entity to another....
		void GLGraphics::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			
		}

		//I ONLY HAVE TO MAKE AS MANY DRAW METHODS AS I HAVE SHADERS SO A SWITCH MAY NOT BE THAT BAD HERE....
		//we pass in references to the pointers since we may need to allocate memory for this pointers -> the value the 
		//pointer holds is change able, but changing the actual pointer means we need to pass a pointer to the pointer or a reference of the pointer (done here)
		void GLGraphics::DrawEntity(const EntityPointer& entityToDraw, const CameraComponentPointer& camera) {
			//possibly change this function to draw all entites of a specific type
			//example: draw all the box entities on one go
			SpritePointer sprite = entityToDraw->GET_COMPONENT(SpriteComponent);
			if (!sprite->IsDestroyed) {							
				TransformComponentPointer transform = entityToDraw->GET_COMPONENT(TransformComponent);
				Shaders::ShaderPointer shaderProgram = _resourceManager->GetShader(sprite->shaderName);

				if (shaderProgram.get() != nullptr) {
					shaderProgram->Use();

					Texture::TexturePointer texture = _resourceManager->GetTexture(sprite->textureName);
					texture->Activate(0);

					shaderProgram->UpdateUniforms(Constants::TEXTUREUNITFORM, 0);
					shaderProgram->UpdateUniforms(Constants::COLORUNIFORM, sprite->color);
					shaderProgram->UpdateUniforms(Constants::PROJECTIONUNIFORM, camera->projectionMatrix);
					shaderProgram->UpdateUniforms(Constants::VIEWUNIFORM, camera->viewMatrix);
					shaderProgram->UpdateUniforms(Constants::OFFSET, transform->position);
					shaderProgram->UpdateUniforms(Constants::LIGHT_COLOR, glm::vec4(1.0f));

					glm::mat4 model;

					model = glm::translate(model, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
					model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));				
					shaderProgram->UpdateUniforms(Constants::MODELUNIFORM, model);
				
					Sprite::MeshShared mesh = _resourceManager->GetMesh(sprite->spriteTypeName);

					_resourceManager->BindVertexArrays(mesh->Type);
					
					glDrawArrays(GL_TRIANGLES, mesh->VertexStart, mesh->VertexEnd);
					texture->Deactivate();
					glBindVertexArray(0);
					shaderProgram->UnUse();
				}
			}
		}


		void GLGraphics::NewFrame() {
			// TODO Disable this for 2D games!!!
			// When you disable this for 2D games this will be drawn on top of each other in the order that they are drawn
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

			//glSentcilMask(~0);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//Need to add GL_STENCIL_BUFFER_BIT
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, 800, 600);
			//Do a whole bunch of frame buffer stuff here


		}
	}
}

