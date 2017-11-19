#include "stdafx.h"
#include "TestGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
//#include "./ComponentsInclude.h"
//#include "./SystemsInclude.h"
#include <math.h>
#include <sstream>
#include <cstdlib>
#include <time.h>


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

namespace Engine {

	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {		
			
		glm::vec3 sampleCubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		void TestGameWorld::Init() {
			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Sprite);
			
			//spend some time to understand this space stuff
			RegisterSpace("Test GameWorld");			
			SetLogicalSpace("Test GameWorld");			
			SpacePointer gameWorld = ENGINE->GetSpace("Test GameWorld");
			gameWorld->ClearSpace();
			
			gameWorld->AddSystem(GETSYS(CameraSystem));
			gameWorld->AddSystem(GETSYS(GLGraphics));

			LoadResourceData();							

			CustomWindowPointer window = GETSYS(CustomWindow);
			window->HideMouseCursor();
			
 			SpawnDefaultBoxes();			
		}

		void TestGameWorld::LoadResourceData() {

			//copying data over like this may be slow an poor performance
			std::vector<GLfloat> meshVerticies = {
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
			};

			std::vector<GLfloat> triangleMeshVerticies = {
				//POSITION			COLOR							
				0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				-0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

				0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				-0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

				0.0f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				0.0f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
				0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				0.0f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			};

			Sprite::MeshShared sprite = std::make_shared<Sprite::Mesh>("boxSource", meshVerticies, 36, 0);
			Sprite::MeshShared triangle = std::make_shared<Sprite::Mesh>("triangleSource", triangleMeshVerticies, 12, 36);
			
			//Sprite::AttributeConfiguration::VertexAttribute position(3, 0, Sprite::AttributeConfiguration::POSITION);
			//Sprite::AttributeConfiguration::VertexAttribute color(3, 1, Sprite::AttributeConfiguration::COLOR);
			//Sprite::AttributeConfiguration::VertexAttribute texture(2, 2, Sprite::AttributeConfiguration::TEXTURE);

			//std::shared_ptr<Sprite::AttributeConfiguration> config = std::make_shared<Sprite::AttributeConfiguration>();			

			//config->attributes = { position, color, texture };

			//sprite->vertexStart = 0;
			//sprite->vertexEnd = 36;
			//triangle->vertexStart = 36;
			//triangle->vertexEnd = 48;
			
			//sprite->attributes = { position, color, texture };
			//triangle->attributes = { position, color, texture };

			Systems::ResourceManagerShared manager = ENGINE->GetResourceManager();

			manager->AddMesh(sprite);
			manager->AddMesh(triangle);

			//manager->vertexAttributeConfigurations.emplace("testAttribs", config);

			manager->LoadSpriteResourcesIntoBuffers();
			
			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container.jpg", false, "container");
			//maybe take this out of here at some point but for now it is needed
			manager->CreateBasicTexture();
		}

		void TestGameWorld::Update(float dt) {
			//just have to update the camera here and take user input
			//I can get the camera through the engines space
			//This method is responsible for updating the camera!!!!
			//realTimeMouse(dt);
			CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			//these things should not be here....this is like the handling of the controls
			RealTimeKeys();

			//EntityPointer test = ENGINE->GetActiveSpace()->GetEntityByName("triangle");
			//test->GET_COMPONENT(TransformComponent)->rotation = dt;

			ENGINE->SendMsg(nullptr, nullptr, Message::MSG_MOUSE_MOVE);
			
		}

		void TestGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("Test GameWorld")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}

		}

		void TestGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {						
			
		}
	
		void TestGameWorld::SpawnDefaultBoxes() {
			for (int i = 0; i < 10; i++) {
				EntityPointer box = ENGINE->Factory().create("box");
				box->GET_COMPONENT(TransformComponent)->position = sampleCubePositions[i];
				box->GET_COMPONENT(TransformComponent)->rotation = 0.0f;
				box->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
				box->GET_COMPONENT(SpriteComponent)->textureName = "container";
				box->GET_COMPONENT(SpriteComponent)->spriteTypeName = "boxSource";
				box->GET_COMPONENT(SpriteComponent)->shaderName = "box";
				//add our entity to the active space
				//PopulateSystemEntites() is then called by the engine to add the spaces entities to each system in the space (This includes GLGraphics)
				//On GLGraphic's Update(), it loops through its entites and draws it (thereby drawing the boxes that I make here)
				ENGINE->GetActiveSpace()->AddEntity(box);
			}

			EntityPointer triangle = std::make_shared<Entity>();

			triangle->AddComponent(std::make_shared<TransformComponent>());
			triangle->AddComponent(std::make_shared<SpriteComponent>());
			triangle->SetName("triangle");
			
			triangle->GET_COMPONENT(TransformComponent)->position = glm::vec3(1.0f, 1.0f, 1.0f);
			triangle->GET_COMPONENT(TransformComponent)->rotation = 0.0f;
			triangle->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			triangle->GET_COMPONENT(SpriteComponent)->textureName = "base";
			triangle->GET_COMPONENT(SpriteComponent)->spriteTypeName = "triangleSource";
			triangle->GET_COMPONENT(SpriteComponent)->shaderName = "box";

			ENGINE->GetActiveSpace()->AddEntity(triangle);

		}

		
		void TestGameWorld::RealTimeKeys() {		
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_A_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_S_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_D_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_W_PRESS);
			}
		}
	}

}

