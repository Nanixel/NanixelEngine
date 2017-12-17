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

	sf::Clock testClock;
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

			//TODO Light Sources should be iterated over separely
			//for (auto& it : _lightSources) {...}
			std::string test = "3DShader";
			Shaders::ShaderPointer shaderProgram = _resourceManager->GetShader(test);

			_shitIndex = 0;
			for (auto& it : _entities) {

				if (it->GetName() == "triangle") {
					//The issue here is that the triangle sprite depends on two shaders -> one to draw it and one for lighting
					shaderProgram->Use();
					std::string number = std::to_string(_shitIndex);
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].position").c_str(), it->GET_COMPONENT(TransformComponent)->position);
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].constant").c_str(), 1.0f);
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].linear").c_str(), 0.09f);
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].ambient").c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].diffuse").c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
					shaderProgram->UpdateUniforms(("pointLights[" + number + "].specular").c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
					_shitIndex++;
					shaderProgram->UnUse();
				}


				DrawEntity(it, camera);
			}
			_shitIndex = 0;
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

					Sprite::MeshShared mesh = _resourceManager->GetMesh(sprite->spriteTypeName);

					mesh->SetUpTexture(shaderProgram);			
					
					shaderProgram->UpdateUniforms(Constants::COLORUNIFORM, sprite->color);
					shaderProgram->UpdateUniforms(Constants::PROJECTIONUNIFORM, camera->projectionMatrix);
					shaderProgram->UpdateUniforms(Constants::VIEWUNIFORM, camera->viewMatrix);
					shaderProgram->UpdateUniforms(Constants::OFFSET, transform->position);
					//shaderProgram->UpdateUniforms(Constants::LIGHT_COLOR, glm::vec4(1.0f));
					//shaderProgram->UpdateUniforms("lightPos", ENGINE->GetSpace("Test GameWorld")->GetEntityByName("triangle")->GET_COMPONENT(TransformComponent)->position);
					shaderProgram->UpdateUniforms("viewPos", camera->Position);

					shaderProgram->UpdateUniforms("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
					shaderProgram->UpdateUniforms("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
					shaderProgram->UpdateUniforms("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
					shaderProgram->UpdateUniforms("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

					shaderProgram->UpdateUniforms("spotLight.position", camera->Position);
					shaderProgram->UpdateUniforms("spotLight.direction", camera->Front);
					shaderProgram->UpdateUniforms("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
					float test = glm::cos(glm::radians(17.5));
					shaderProgram->UpdateUniforms("spotLight.outerCutOff", test);
					//this takes a lightsources values
					shaderProgram->UpdateUniforms("spotLight.constant", 1.0f);
					shaderProgram->UpdateUniforms("spotLight.linear", 0.09f);
					shaderProgram->UpdateUniforms("spotLight.quadratic", 0.032f);
					//these colors need to be applied to all things in the scene so it would best be stored on a 
					//common light source object - aka this should not be based on a component
					//shaderProgram->UpdateUniforms("light.diffuse", sprite->DiffuseColor);
					//shaderProgram->UpdateUniforms("light.specular", sprite->AmbientColor);
					shaderProgram->UpdateUniforms("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
					shaderProgram->UpdateUniforms("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
					shaderProgram->UpdateUniforms("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		
					//Keep in mind that every space has a camera
					//shaderProgram->UpdateUniforms("cameraPos", ENGINE->GetSpace("Test GameWorld")->GetCamera()->GET_COMPONENT(TransformComponent)->position);

					//shaderProgram->UpdateUniforms("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
					
					//shaderProgram->UpdateUniforms("material.emission", 2);
					shaderProgram->UpdateUniforms("material.shininess", 64.0f);
						
					//shaderProgram->UpdateUniforms("direction", camera->Front);
					//pass the cos of the angle here so that we can compare the dot product result in the shader							

					shaderProgram->UpdateUniforms("time", testClock.getElapsedTime().asSeconds());
					

					glm::mat4 model;

					model = glm::translate(model, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
					model = glm::rotate(model, glm::radians(transform->rotation), transform->rotationVector);
					model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));				
					shaderProgram->UpdateUniforms(Constants::MODELUNIFORM, model);
				
					
					mesh->Activate();
					//_resourceManager->BindVertexArrays(mesh->Type);
					
					glDrawArrays(GL_TRIANGLES, mesh->VertexStart, mesh->VertexEnd);
					//texture->Deactivate();
					glBindVertexArray(0);
					shaderProgram->UnUse();
				}
			}
		}


		void GLGraphics::NewFrame() {
			// TODO Disable this for 2D games!!!
			// When you disable this for 2D games this will be drawn on top of each other in the order that they are drawn
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.15f, 0.17f, 0.18f, 1.0f);

			//glSentcilMask(~0);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//Need to add GL_STENCIL_BUFFER_BIT
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, 800, 600);
			//Do a whole bunch of frame buffer stuff here


		}
	}
}

