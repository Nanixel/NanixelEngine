#include "stdafx.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include "../ECEngine/Engine/Core/Systems/Graphics/headers/GLGraphics.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"
//#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //convert glm data type to raw pointers
#include <math.h>
#include <numeric>
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
			
			if (glewInit() != GLEW_OK) {
				std::cout << "ERROR::GLGRAPICS::FAILED_INITIALIZING_GLEW_INIT" << std::endl;
			}				

			//create a new shader - this needs to be moved to the resource manager
			Shaders::ShaderPointer defaultShader = std::make_shared<Shaders::ShaderUtility>();
			defaultShader->LoadShaderFile("../ECEngine/Engine/Core/Shaders/ECVertexShader.txt", "../ECEngine/Engine/Core/Shaders/ECFragShader.txt");
			defaultShader->Compile();

			defaultShader->FindUniforms("model");
			defaultShader->FindUniforms("projection");
			defaultShader->FindUniforms("view");
			//defaultShader->FindUniforms("color");
			defaultShader->FindUniforms("texture1");
			//defaultShader->FindUniforms("texture2");

			AddShader("box", defaultShader);		
		}		

		void GLGraphics::Update(float dt) {
			CameraComponentPointer camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);						
			NewFrame();
			//for each entity in the graphics system entities....
			for (auto& it : _entities) {
				DrawEntity(it, camera);
			}			
		}

		void GLGraphics::ShutDown() {
			DeleteMesh();
		}

		//the graphics SYSTEM will send messages from one entity to another....
		void GLGraphics::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			
		}

		//we pass in references to the pointers since we may need to allocate memory for this pointers -> the value the 
		//pointer holds is change able, but changing the actual pointer means we need to pass a pointer to the pointer or a reference of the pointer (done here)
		void GLGraphics::DrawEntity(const EntityPointer& entityToDraw, const CameraComponentPointer& camera) {
			//possibly change this function to draw all entites of a specific type
			//example: draw all the box entities on one go

			//Probably worth putting a reference of this to our class -> GOOD CASE FOR DEPENDENCY INJECTION
			Systems::ResourceManager::ResourceManagerShared manager = ENGINE->GetResourceManager();

			SpritePointer spriteComponent = entityToDraw->GET_COMPONENT(SpriteComponent);
			std::string sname = entityToDraw->GET_COMPONENT(SpriteComponent)->shaderName;

			Shaders::ShaderPointer shaderProgram = shadersDict.find(sname)->second;
			TransformComponentPointer transform = entityToDraw->GET_COMPONENT(TransformComponent);

			if (shaderProgram.get() != nullptr) {
				shaderProgram->Use();

				TexturePointer texture = manager->GetTexture(spriteComponent->textureName);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->ID);
				shaderProgram->UpdateUniforms(Constants::TEXTUREUNITFORM, 0);

				shaderProgram->UpdateUniforms(Constants::PROJECTIONUNIFORM, camera->projectionMatrix);

				glm::mat4 model;

				if (camera->projectionMode == CameraComponent::ProjectionMode::ORTHOGRAPHIC) {
					model = glm::translate(model, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
					model = glm::translate(model, glm::vec3(0.5f * 300, 0.5f * 400, 0.0f));
					model = glm::rotate(model,
						glm::radians(transform->rotation),
						glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::translate(model, glm::vec3(-0.5f * 300, -0.5f * 400, 0.0f));
					model = glm::scale(model,
						glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));
					shaderProgram->UpdateUniforms(Constants::COLORUNIFORM, entityToDraw->GET_COMPONENT(SpriteComponent)->color);
				}
				else {
					//view may not exist if this is a 2D game
					shaderProgram->UpdateUniforms(Constants::VIEWUNIFORM, camera->viewMatrix);					
					
					model = glm::translate(model, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
					model = glm::rotate(model,
						glm::radians(transform->rotation),
						glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model,
						glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));
				}


				shaderProgram->UpdateUniforms(Constants::MODELUNIFORM, model);
				glBindVertexArray(manager->_buffers.vertexArrayObject);

				//need to extend this to be more flexible
				Sprite::SpriteResource::SpriteSourceShared spriteResource = manager->GetSpriteResource(spriteComponent->spriteTypeName);
				glDrawArrays(GL_TRIANGLES, spriteResource->vertexStart, spriteResource->vertexEnd);

			/*	if (spriteComponent->textureName == "container") {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture->ID);
					shaderProgram->UpdateUniforms(Constants::TEXTUREUNITFORM, 0);
				}
				else {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, texture->ID);
					shaderProgram->UpdateUniforms(Constants::TEXTUREUNIT2, 1);
				}
				glActiveTexture(GL_TEXTURE0);*/																						
				
			}
		}

		//0 is the unbinding value, therefore we check for it
		void GLGraphics::DeleteMesh() {
			Systems::ResourceManager::ResourceManagerShared manager = ENGINE->GetResourceManager();
			if (manager->_buffers.vertexArrayObject > 0) {
				glDeleteBuffers(1, &manager->_buffers.vertexArrayObject);
			}
			if (manager->_buffers.staticEBO > 0) {
				glDeleteBuffers(1, &manager->_buffers.staticEBO);
			}
			if (manager->_buffers.staticVBO > 0) {
				glDeleteBuffers(1, &manager->_buffers.staticVBO);
			}
		}

		Shaders::ShaderPointer GLGraphics::GetShader(std::string shader) {
			
			ShaderMap::iterator it = shadersDict.find(shader);
			if (it != shadersDict.end()) {
				return it->second;
			}
			throw std::range_error("ERROR::SHADER::SHADER_NOT_FOUND_ON_ADD");
		}

		void GLGraphics::AddShader(std::string shaderName, Shaders::ShaderPointer shader) {
			shadersDict.emplace(shaderName, shader);
		}

		void GLGraphics::NewFrame() {
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, 800, 600);
		}
	}
}

