#include "stdafx.h"
#include "../ECEngine/Engine/Core/GameStates/2DGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
namespace Engine {

	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {

		void ArkaniodGameWorld::Init() {

			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Sprite);
			//The Test Gameworld space should update with this system
			RegisterSpace("Arkanoid GameWorld");
			SetLogicalSpace("Arkanoid GameWorld");			
			SpacePointer gameWorld = ENGINE->GetSpace("Arkanoid GameWorld");
			SetUpSpaceCamera();
			//clearspace clears all the entites in the space except the camera
			gameWorld->ClearSpace();

			//adds existing GLGraphics and CameraSystem to our arknoid space
			gameWorld->AddSystem(GETSYS(CameraSystem));
			gameWorld->AddSystem(GETSYS(GLGraphics));

			LoadResourceData();
			CreateEntities();
			AddCustomShader(); 


			CustomWindowPointer window = GETSYS(CustomWindow);
			window->HideMouseCursor();			
		}

		void ArkaniodGameWorld::SetUpSpaceCamera() {
			CameraComponentPointer camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			camera->projectionMode = CameraComponent::ProjectionMode::ORTHOGRAPHIC;
			camera->viewType = CameraComponent::CameraViewType::OVERHEAD;
		
		}

		void ArkaniodGameWorld::Update(float dt) {

		}

		void ArkaniodGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("ArkaniodSpace")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}
		}

		void ArkaniodGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {

		}

		void ArkaniodGameWorld::LoadResourceData() {
			Sprite::SpriteResource::SpriteSourceShared sprite = std::make_shared<Sprite::SpriteResource>("spriteSource");

			sprite->meshVerticies = {
				// Pos				// Tex
				0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

				0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				1.0f, 0.0f, 0.0f,  1.0f, 0.0f
			};

			Sprite::AttributeConfiguration::VertexAttribute position(3, 0, Sprite::AttributeConfiguration::POSITION);
			Sprite::AttributeConfiguration::VertexAttribute texture(2, 1, Sprite::AttributeConfiguration::TEXTURE);
			std::shared_ptr<Sprite::AttributeConfiguration> config = std::make_shared<Sprite::AttributeConfiguration>();
			config->attributes = { position, texture };

			sprite->vertexStart = 0;
			sprite->vertexEnd = 6;

			Systems::ResourceManager::ResourceManagerShared manager = ENGINE->GetResourceManager();
			manager->AddSpriteResource(sprite);
			//testAttribs is hard coded into the manager, change that
			manager->vertexAttributeConfigurations.emplace("testAttribs", config);
			manager->LoadSpriteResourcesIntoBuffers();

			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/wall.jpg", false, "wall");
			//maybe take this out of here at some point but for now it is needed
			manager->GenerateBasicTexture();
		}

		void ArkaniodGameWorld::CreateEntities() {

			EntityPointer entity = std::make_shared<Entity>();

			entity->AddComponent(std::make_shared<TransformComponent>());
			entity->AddComponent(std::make_shared<SpriteComponent>());
			entity->SetName("sprite");

			entity->GET_COMPONENT(TransformComponent)->position = glm::vec3(200.0f, 200.0f, 0);
			entity->GET_COMPONENT(TransformComponent)->rotation = 45.0f;
			entity->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 300.0f, 400.0f);
			entity->GET_COMPONENT(SpriteComponent)->color = glm::vec3(0.0f, 1.0f, 0.0f);
			entity->GET_COMPONENT(SpriteComponent)->textureName = "wall";
			entity->GET_COMPONENT(SpriteComponent)->spriteTypeName = "spriteSource";
			entity->GET_COMPONENT(SpriteComponent)->shaderName = "2Dshader";

			ENGINE->GetActiveSpace()->AddEntity(entity);

		}

			
		void ArkaniodGameWorld::AddCustomShader() {
			Shaders::ShaderPointer simpleShader = std::make_shared<Shaders::ShaderUtility>();
			simpleShader->LoadShaderFile("2DVertexShader.txt", "2DFragmentShader.txt");
			simpleShader->Compile();

			simpleShader->FindUniforms("model");
			simpleShader->FindUniforms("projection");
			simpleShader->FindUniforms("texture1");
			simpleShader->FindUniforms("color");			

			GETSYS(GLGraphics)->AddShader("2Dshader", simpleShader);
		}
	
	}
}