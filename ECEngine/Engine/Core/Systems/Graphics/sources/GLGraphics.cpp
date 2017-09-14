#include "stdafx.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include "../ECEngine/Engine/Core/Systems/Graphics/headers/GLGraphics.h"
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"
#include "../ECEngine/Engine/Core/Systems/Resources/headers/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //convert glm data type to raw pointers
#include <math.h>
#include <numeric>
#include <glm/gtc/matrix_transform.hpp> //scale, rotate, translate, projection, ortho

struct QuadMesh {
	glm::vec4 vertices;
	glm::vec2 UV;
};

namespace Engine {
	
	extern Engine *ENGINE;
	namespace Systems {

		GLGraphics::GLGraphics() : System(std::string("GLGraphicsSystem"), SystemType::EC_GLGraphics)
		{

		}

		void GLGraphics::Init() {

			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Sprite);					

			//Initialize glewInit()
			if (glewInit() != GLEW_OK) {
				std::cout << "ERROR::GLGRAPICS::FAILED_INITIALIZING_GLEW_INIT" << std::endl;
			}

			//probably got to move this to its own component?
			InitiailzeBuffers();


			//create a new shader
			Shaders::ShaderPointer defaultShader = std::make_shared<Shaders::ShaderUtility>();
			defaultShader->LoadShaderFile("../ECEngine/Engine/Core/Shaders/ECVertexShader.txt", "../ECEngine/Engine/Core/Shaders/ECFragShader.txt");
			defaultShader->Compile();

			defaultShader->FindUniforms("model");
			defaultShader->FindUniforms("projection");
			defaultShader->FindUniforms("view");
			//defaultShader->FindUniforms("color");
			defaultShader->FindUniforms("texture1");

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
			
			SpritePointer spriteComponent = entityToDraw->GET_COMPONENT(SpriteComponent);
			std::string sname = entityToDraw->GET_COMPONENT(SpriteComponent)->shaderName;
			Shaders::ShaderPointer shaderProgram = shadersDict.find(sname)->second;
			TransformComponentPointer transform = entityToDraw->GET_COMPONENT(TransformComponent);

			if (shaderProgram.get() != nullptr) {
				//view may not exist if this is a 2D game
				shaderProgram->UpdateUniforms(Constants::VIEWUNIFORM, camera->viewMatrix);
				shaderProgram->UpdateUniforms(Constants::PROJECTIONUNIFORM, camera->projectionMatrix);
				shaderProgram->UpdateUniforms(Constants::COLORUNIFORM, entityToDraw->GET_COMPONENT(SpriteComponent)->color);

				glActiveTexture(GL_TEXTURE0);
				TexturePointer texture = ResourceManager::GetTexture(spriteComponent->textureName);
				glBindTexture(GL_TEXTURE_2D, texture->ID);
				shaderProgram->UpdateUniforms(Constants::TEXTUREUNITFORM, 0);

				glm::mat4 model;				
				model = glm::translate(model, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
				model = glm::rotate(model,
					glm::radians(transform->rotation),
					glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model,
					glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));

				shaderProgram->UpdateUniforms(Constants::MODELUNIFORM, model);				
								
				shaderProgram->Use();
				glBindVertexArray(quadInfo.VertexArrayObject);

				//check what the 36 means here - aka to reder a cube you need 36 vertices
				//6 faces * 2 triangles * 3 verticies.....there is probably a better way to do this but I have not learned it yet
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		//currently this handles textures too.....
		void GLGraphics::InitiailzeBuffers() {

			//each sprite should be responsible for defining its verticies			
			std::vector<float> meshVerticies = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

			VertexAttribute positionAttribute;
			positionAttribute.location = 0;
			positionAttribute.size = 3;
			positionAttribute.type = VertexAttributeType::POSITION;

			VertexAttribute textureAttribute;
			textureAttribute.location = 1;
			textureAttribute.size = 2;
			textureAttribute.type = VertexAttributeType::TEXTURE;

			std::vector<VertexAttribute> attributes{ positionAttribute, textureAttribute };

			InitRenderData(meshVerticies, attributes);
			InitTextureData();		
		}

		void GLGraphics::InitRenderData(std::vector<float>& meshData, std::vector<VertexAttribute> meshAttributes) {
			int strideMultiplier = std::accumulate(meshAttributes.begin(), meshAttributes.end(), 0, 
				[](int sum, const VertexAttribute& attrib) {return sum + attrib.size; });
			GLsizei stride = strideMultiplier * sizeof(GLfloat);
			int counter = 0;

			glGenVertexArrays(1, &quadInfo.VertexArrayObject);
			glGenBuffers(1, &quadInfo.VertexBufferObject);

			glBindVertexArray(quadInfo.VertexArrayObject);
			glBindBuffer(GL_ARRAY_BUFFER, quadInfo.VertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(meshData), &meshData[0], GL_STATIC_DRAW);			
			
			for (auto it = meshAttributes.begin(); it != meshAttributes.end(); it++) {
				if (it->size > 0) {
					glEnableVertexAttribArray(it->location);
					glVertexAttribPointer(it->location, it->size, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(counter * sizeof(GLfloat)));
					counter += it->size;
				}
			}
			glBindVertexArray(0); //_quadInfo.vao will now remember how to read your Data from GraphicsQuad
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		//Init Texture data will be responsible for initializing all of the textures for that gamespace
		void GLGraphics::InitTextureData() {
			ResourceManager::LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container.jpg", false, "container");
		}

		void GLGraphics::ActivateTexture() {

		}		

		//0 is the unbinding value, therefore we check for it
		void GLGraphics::DeleteMesh() {
			if (quadInfo.VertexArrayObject > 0) {
				glDeleteBuffers(1, &quadInfo.VertexArrayObject);
			}
			if (quadInfo.ElementBufferObject > 0) {
				glDeleteBuffers(1, &quadInfo.ElementBufferObject);
			}
			if (quadInfo.VertexBufferObject > 0) {
				glDeleteBuffers(1, &quadInfo.VertexBufferObject);
			}
		}

		Shaders::ShaderPointer GLGraphics::GetShader(std::string shader) {
			//find the shader by its name
			ShaderMap::iterator it = shadersDict.find(shader);
			if (it != shadersDict.end()) {
				//remember the shader is the second item in the map
				//get the actual shaderpointer
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

