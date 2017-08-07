#include "stdafx.h"
#include "./Engine.h"
#include "GLGraphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //convert glm data type to raw pointers
#include <math.h>
#include <glm/gtc/matrix_transform.hpp> //scale, rotate, translate, projection, ortho
#include "stb_image.h"


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

			if (glewInit() != GLEW_OK) {
				std::cout << "ERROR::GLGRAPICS::FAILED_INITIALIZING_GLEW_INIT" << std::endl;
			}

			//probably got to move this to its own component?
			InitiailzeBuffers();

			//create a new shader
			ShaderPointer defaultShader(new GLShader());
			defaultShader->LoadShaderFile("ECVertexShader.txt", "ECFragShader.txt");
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
			//find our box shader
			glViewport(0, 0, 800, 600);
			std::string sname = entityToDraw->GET_COMPONENT(SpriteComponent)->shaderName;
			ShaderPointer shaderProgram = shadersDict.find(sname)->second;
			//get the transform component of our entity - each has their own transform component = yes
			//remember this is just a pointer to the transform component of this drawable entity
			TransformComponentPointer transform = entityToDraw->GET_COMPONENT(TransformComponent);

			if (shaderProgram.get() != nullptr) {
				//this is probably the model matrix
				//update camera matricies -> remember camera is passed in as a reference to a pointer so we can do this
				shaderProgram->UpdateUniforms("view", camera->viewMatrix);
				shaderProgram->UpdateUniforms("projection", camera->projectionMatrix);
				shaderProgram->UpdateUniforms("texture1", 0);
				glm::mat4 object2World;
				//these transform postions will describe where the ojects exist on the screen
				object2World = glm::translate(object2World, glm::vec3(entityToDraw->GET_COMPONENT(TransformComponent)->position));
				object2World = glm::rotate(object2World,
					glm::radians(transform->rotation),
					glm::vec3(0.0f, 0.0f, 1.0f));
				object2World = glm::scale(object2World,
					glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));

				//shaderProgram->UpdateUniforms("model", object2World);
				unsigned int location = glGetUniformLocation(shaderProgram->shaderProgramID, "model");
				glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(object2World));


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);

				//shaderProgram->UpdateUniforms("color", entityToDraw->GET_COMPONENT(SpriteComponent)->color);				
				shaderProgram->Use();
				glBindVertexArray(quadInfo.VertexArrayObject);

				//check what the 36 means here - aka to reder a cube you need 36 vertices
				//6 faces * 2 triangles * 3 verticies.....there is probably a better way to do this but I have not learned it yet
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		//currently this handles textures too.....
		void GLGraphics::InitiailzeBuffers() {

			//these verticies describe the mesh

			float meshVerticies[] = {
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
			
			glGenVertexArrays(1, &quadInfo.VertexArrayObject);
			glGenBuffers(1, &quadInfo.VertexBufferObject);
			//glGenBuffers(1, &quadInfo.ElementBufferObject);

			glBindVertexArray(quadInfo.VertexArrayObject);
			glBindBuffer(GL_ARRAY_BUFFER, quadInfo.VertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(meshVerticies), meshVerticies, GL_STATIC_DRAW);

			//need to make an array of indicies if I am to use this
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ElementBufferObject);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(something2), something2, GL_STATIC_DRAW);

			//need to do this for color and textures....so can probably reuse this
			//first arg is the location
			//these also need to be adjustable depending on what i have......
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//TODO - Separate this texture stuff later on!!!
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1); //going to be the second index in the vertex attribute array

			////Binding to 0 will unbind the buffers so you cannot modify the data
			//glBindVertexArray(0); //_quadInfo.vao will now remember how to read your Data from GraphicsQuad
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glGenTextures(1, &texture1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			GLint width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
			unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\n" << "containder.jpg" << std::endl;
			}
			stbi_image_free(data);

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

		ShaderPointer GLGraphics::GetShader(std::string shader) {
			//find the shader by its name
			ShaderMap::iterator it = shadersDict.find(shader);
			if (it != shadersDict.end()) {
				//remember the shader is the second item in the map
				//get the actual shaderpointer
				return it->second;
			}
			throw std::range_error("ERROR::SHADER::SHADER_NOT_FOUND_ON_ADD");
		}

		void GLGraphics::AddShader(std::string shaderName, ShaderPointer shader) {
			shadersDict.emplace(shaderName, shader);
		}

		void GLGraphics::NewFrame() {
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}

