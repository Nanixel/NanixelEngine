#include "stdafx.h"
#include "TestGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"
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

	float deltaTime = 0.0f; //Time between current frame and last frame
	float lastFrame = 0.0f; 
	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {		
			
		glm::vec3 sampleCubePositions[] = {
			glm::vec3(0.0f,  0.0f,  1.0f),
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

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
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
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
			};

			std::vector<GLfloat> triangleMeshVerticies = {
				//POSITION								
				0.0f, 0.5f, 0.0f,  
				-0.5f, -0.5f, 0.5f,
				0.0f, 0.0f, 0.0f,

				0.0f, 0.5f, 0.0f,
				-0.5f, -0.5f, 0.5f,
				0.0f, -0.5f, -0.5f,

				0.0f, 0.5f, 0.0f,
				0.0f, -0.5f, -0.5f,
				0.5f, -0.5f, 0.5f,

				-0.5f, -0.5f, 0.5f,
				0.0f, -0.5f, -0.5f,
				0.5f, -0.5f, 0.5f,
			};

			Sprite::MeshShared sprite = std::make_shared<Sprite::Mesh>("boxSource", meshVerticies, 36, 0, Sprite::Mesh::MeshType::QUAD);
			Sprite::MeshShared triangle = std::make_shared<Sprite::Mesh>("triangleSource", triangleMeshVerticies, 12, 0, Sprite::Mesh::MeshType::LIGHT_SOURCE);
			sprite->TotalAttributesSize = 8;
			triangle->TotalAttributesSize = 3;
			Systems::ResourceManagerShared manager = ENGINE->GetResourceManager();

			manager->AddMesh(sprite);
			manager->AddMesh(triangle);

			manager->LoadSpriteResourcesIntoBuffers();
			
			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/matrix.jpg", false, "matrix");
			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container2_specular.png", true, "specular_container");
			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container2.png", true, "container2");
			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container.jpg", false, "container");
			manager->CreateBasicTexture();
		}

		void TestGameWorld::Update(float dt) {
			//CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			RealTimeKeys();
			ENGINE->SendMsg(nullptr, nullptr, Message::MSG_MOUSE_MOVE);

			EntityPointer lightSource = ENGINE->GetSpace("Test GameWorld")->GetEntityByName("triangle");

			TransformComponentPointer transform = lightSource->GET_COMPONENT(TransformComponent);
			LightSourceShared source = lightSource->GET_COMPONENT(LightSource);
			

			////float currentTime = testClock.getElapsedTime().asSeconds();
			////deltaTime = currentTime - lastFrame;
			//lastFrame = currentTime;		

			////transform->position.x = 1.0f + std::sin(currentTime) * 2.0f;

			//glm::vec3 lightColor;
			//lightColor.x = sin(currentTime * 2.0f);
			//lightColor.y = sin(currentTime * 0.7f);
			//lightColor.z = sin(currentTime * 1.3f);


			//for (auto& it : _entities) {
			//	SpritePointer sprite = it->GET_COMPONENT(SpriteComponent);
			//	//if (it->GetName() == "box");
			//	sprite->DiffuseColor = lightColor * glm::vec3(0.5f);
			//	sprite->AmbientColor = sprite->DiffuseColor * glm::vec3(0.2f);
			//}
		}

		void TestGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("Test GameWorld")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}
		}

		void TestGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {						

		}
	
		void TestGameWorld::SpawnDefaultBoxes() {

			for (int i = 0; i < 4; i++) {
				EntityPointer lightSource = std::make_shared<Entity>();

				lightSource->AddComponent(std::make_shared<TransformComponent>());
				lightSource->AddComponent(std::make_shared<SpriteComponent>());
				lightSource->AddComponent(std::make_shared<LightSource>());
				lightSource->SetName("triangle");

				lightSource->GET_COMPONENT(TransformComponent)->position = pointLightPositions[i];
				lightSource->GET_COMPONENT(TransformComponent)->rotation = 0.0f;
				lightSource->GET_COMPONENT(TransformComponent)->scale = glm::vec3(0.2f);
				lightSource->GET_COMPONENT(SpriteComponent)->textureName = "base";
				lightSource->GET_COMPONENT(SpriteComponent)->spriteTypeName = "triangleSource";
				lightSource->GET_COMPONENT(SpriteComponent)->shaderName = "LightSourceShader";
				lightSource->GET_COMPONENT(SpriteComponent)->IsDestroyed = false;

				ENGINE->GetSpace("Test GameWorld")->AddEntity(lightSource);
			}

			for (int i = 0; i < 10; i++) {
				EntityPointer box = ENGINE->Factory().create("box");
				box->GET_COMPONENT(TransformComponent)->position = sampleCubePositions[i];
				float angle = 20.0f * i;
				box->GET_COMPONENT(TransformComponent)->rotation = angle;
				box->GET_COMPONENT(TransformComponent)->rotationVector = glm::vec3(1.0f, 0.3f, 0.5f);
				box->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
				box->GET_COMPONENT(SpriteComponent)->textureName = "container2";
				box->GET_COMPONENT(SpriteComponent)->spriteTypeName = "boxSource";
				box->GET_COMPONENT(SpriteComponent)->shaderName = "3DShader";
				box->GET_COMPONENT(SpriteComponent)->color = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
				box->GET_COMPONENT(SpriteComponent)->IsDestroyed = false;
				//add our entity to the active space
				//PopulateSystemEntites() is then called by the engine to add the spaces entities to each system in the space (This includes GLGraphics)
				//On GLGraphic's Update(), it loops through its entites and draws it (thereby drawing the boxes that I make here)
				ENGINE->GetSpace("Test GameWorld")->AddEntity(box);
			}
			
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_UP);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_DOWN);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_RIGHT);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_LEFT);
			}
		}
	}

}

