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
			
			std::vector<Sprite::Mesh::Vertex> verticies;

			Sprite::Mesh::Vertex v1(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			Sprite::Mesh::Vertex v2(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			Sprite::Mesh::Vertex v3(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			Sprite::Mesh::Vertex v4(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			Sprite::Mesh::Vertex v5(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			Sprite::Mesh::Vertex v6(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

			Sprite::Mesh::Vertex v7(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Sprite::Mesh::Vertex v8(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Sprite::Mesh::Vertex v9(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Sprite::Mesh::Vertex v10(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Sprite::Mesh::Vertex v11(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Sprite::Mesh::Vertex v12(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//BELOW HERE
			Sprite::Mesh::Vertex v13(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v14(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v15(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v16(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v17(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v18(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

			Sprite::Mesh::Vertex v19(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v20(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v21(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v22(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v23(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex v24(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			Sprite::Mesh::Vertex v25(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			Sprite::Mesh::Vertex v26(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			Sprite::Mesh::Vertex v27(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			Sprite::Mesh::Vertex v28(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			Sprite::Mesh::Vertex v29(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			Sprite::Mesh::Vertex v30(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

			Sprite::Mesh::Vertex v31(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Sprite::Mesh::Vertex v32(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Sprite::Mesh::Vertex v33(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Sprite::Mesh::Vertex v34(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Sprite::Mesh::Vertex v35(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Sprite::Mesh::Vertex v36(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			verticies.push_back(v1);
			verticies.push_back(v2);
			verticies.push_back(v3);
			verticies.push_back(v4);
			verticies.push_back(v5);
			verticies.push_back(v6);
			verticies.push_back(v7);
			verticies.push_back(v8);
			verticies.push_back(v9);
			verticies.push_back(v10);
			verticies.push_back(v11);
			verticies.push_back(v12);
			verticies.push_back(v13);
			verticies.push_back(v14);
			verticies.push_back(v15);
			verticies.push_back(v16);
			verticies.push_back(v17);
			verticies.push_back(v18);
			verticies.push_back(v19);
			verticies.push_back(v20);
			verticies.push_back(v21);
			verticies.push_back(v22);
			verticies.push_back(v23);
			verticies.push_back(v24);
			verticies.push_back(v25);
			verticies.push_back(v26);
			verticies.push_back(v27);
			verticies.push_back(v28);
			verticies.push_back(v29);
			verticies.push_back(v30);
			verticies.push_back(v31);
			verticies.push_back(v32);
			verticies.push_back(v33);
			verticies.push_back(v34);
			verticies.push_back(v35);
			verticies.push_back(v36);


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

			std::vector<Sprite::Mesh::Vertex> triangleVerticies;
			Sprite::Mesh::Vertex t1(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t2(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t4(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t5(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t6(glm::vec3(0.0f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

			Sprite::Mesh::Vertex t7(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t8(glm::vec3(0.0f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t9(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t10(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t11(glm::vec3(0.0f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			Sprite::Mesh::Vertex t12(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

			triangleVerticies.push_back(t1);
			triangleVerticies.push_back(t2);
			triangleVerticies.push_back(t3);
			triangleVerticies.push_back(t4);
			triangleVerticies.push_back(t5);
			triangleVerticies.push_back(t6);
			triangleVerticies.push_back(t7);
			triangleVerticies.push_back(t8);
			triangleVerticies.push_back(t9);
			triangleVerticies.push_back(t10);
			triangleVerticies.push_back(t11);
			triangleVerticies.push_back(t12);			

			Sprite::MeshShared sprite = std::make_shared<Sprite::Mesh>("boxSource", verticies, 36, 0, Sprite::Mesh::MeshType::QUAD);
			Sprite::MeshShared triangle = std::make_shared<Sprite::Mesh>("triangleSource", triangleVerticies, 12, 0, Sprite::Mesh::MeshType::LIGHT_SOURCE);
			sprite->TotalAttributesSize = 8;
			triangle->TotalAttributesSize = 3;
			Systems::ResourceManagerShared manager = ENGINE->GetResourceManager();

			manager->AddMesh(triangle);
			manager->AddMesh(sprite);

			//manager->LoadSpriteResourcesIntoBuffers();
			
			//Texture::TexturePointer matrix = manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/matrix.jpg", false, "matrix", "diffuse");
			Texture::TexturePointer specContainer = manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container2_specular.png", true, "specular_container", "specular");
			Texture::TexturePointer container2 = manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container2.png", true, "container2", "diffuse");

			sprite->AddTexture(specContainer);
			sprite->AddTexture(container2);
			//Texture::TexturePointer container = manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/container.jpg", false, "container", "diffuse");
			Texture::TexturePointer baseTexture = manager->CreateBasicTexture();
			//triangle->AddTexture(baseTexture);			
			
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

