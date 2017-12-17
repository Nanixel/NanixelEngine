#include "stdafx.h"
#include "../ECEngine/Engine/Core/GameStates/2DGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include "../ECEngine/Engine/Core/Utilities/Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace Engine {

	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {

		const glm::vec3 INITIAL_BALL_VELOCITY = glm::vec3(100.0f, -350.0, 0.0);
		const float BALL_RADIUS = 12.5f;
		const glm::vec3 PLAYER_SIZE = glm::vec3(100.0f, 20.0f, 0.0f);

		void ArkaniodGameWorld::Init() {

			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Sprite);
			//RegisterComponent(MC_Particle);

			LoadResourceData();
			
			SetUpSpaces();
			//I feel like im going to get in this nasty pattern of space transistions in my game logic
			// For instance:  if I move on to level two how do I tell the gamestate to do that?
			// Maybe introduce to concept of space progression?
			SpacePointer gameSpace = ENGINE->GetSpace("Game");
			//this probably does not need its own camera
			SpacePointer backGroundSpace = ENGINE->GetSpace("Background");

			SetupGameWorldEntities(gameSpace);
			SetupGameWorldEntities(backGroundSpace);

			//Maybe this is better encapsulated by a level object that has reference to some space
			//Or Maybe I can think of a level as an entity it self and process it by THIS system...yes this gamestate is a system
			std::vector<std::vector<GLuint>>levelOneData = LoadTileDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/levelone.txt");
			levels.emplace(1, levelOneData);
			std::vector<std::vector<GLuint>>levelTwoData = LoadTileDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/leveltwo.txt");
			levels.emplace(2, levelTwoData);
			std::vector<std::vector<GLuint>>levelThreeData = LoadTileDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/levelThree.txt");
			levels.emplace(3, levelThreeData);
			std::vector<std::vector<GLuint>> levelFourData = LoadTileDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/levelfour.txt");
			levels.emplace(4, levelFourData);

			LoadLevelEntitiesIntoSpace(levelOneData, gameSpace, 800, 600 * 0.5);
			_currentLevel = 1;		
			SetLogicalSpace("Game");
						
			CreateEntities();
			CreateParticleEntities();

			CustomWindowPointer window = GETSYS(CustomWindow);
			window->HideMouseCursor();			
		}

		void ArkaniodGameWorld::SetUpSpaces() {			
			RegisterSpace("Game");
			RegisterSpace("Background");
		}

		void ArkaniodGameWorld::SetupGameWorldEntities(SpacePointer space) {
			space->ClearSpace();
			CameraComponentPointer camera = space->GetCamera()->GET_COMPONENT(CameraComponent);
			camera->projectionMode = CameraComponent::ProjectionMode::ORTHOGRAPHIC;
			camera->viewType = CameraComponent::CameraViewType::OVERHEAD;

			space->AddSystem(GETSYS(CameraSystem));
			space->AddSystem(GETSYS(GLGraphics));
			space->AddSystem(GETSYS(ParticleSystem));
		}

		void ArkaniodGameWorld::Update(float dt) {
			RealTimeKeys();

			EntityPointer ball = ENGINE->GetSpace("Game")->GetEntityByName("ball");
			EntityPointer player = ENGINE->GetSpace("Game")->GetEntityByName("player");
			MoveBall(ball, dt);

			GLuint newParticles = 2;

			for (auto& it : _entities) {

				if (it->GetName() == "block") {
					if (!it->GET_COMPONENT(SpriteComponent)->IsDestroyed) {

						ArkaniodGameWorld::Collision collision = CheckCircleCollision(ball, it);
						if (std::get<0>(collision)) { // If the collision is true.
							if (!it->GET_COMPONENT(SpriteComponent)->IsSolid) {
								it->GET_COMPONENT(SpriteComponent)->IsDestroyed = true;
							}
							
							ArkaniodGameWorld::Direction direction = std::get<1>(collision);
							glm::vec3 diffVector = std::get<2>(collision);

							if (direction == ArkaniodGameWorld::Direction::LEFT || direction == ArkaniodGameWorld::Direction::RIGHT) {
								ball->GET_COMPONENT(TransformComponent)->Velocity.x = -ball->GET_COMPONENT(TransformComponent)->Velocity.x;
								GLfloat penetration = ball->GET_COMPONENT(TransformComponent)->Radius - std::abs(diffVector.x);
								if (direction == ArkaniodGameWorld::Direction::LEFT) {
									ball->GET_COMPONENT(TransformComponent)->position.x += penetration; // Move ball to right.
								}
								else {
									ball->GET_COMPONENT(TransformComponent)->position.x -= penetration; // Move ball to left;
								}
							}
							else { // Vertical collision.
 								ball->GET_COMPONENT(TransformComponent)->Velocity.y = -ball->GET_COMPONENT(TransformComponent)->Velocity.y;
								GLfloat penetration = ball->GET_COMPONENT(TransformComponent)->Radius - std::abs(diffVector.y);
								if (direction == ArkaniodGameWorld::Direction::UP) {
									ball->GET_COMPONENT(TransformComponent)->position.y -= penetration; //Move ball back up.
								}
								else {
									ball->GET_COMPONENT(TransformComponent)->position.y += penetration; //Move ball back down;
								}
							}
						}
					}
				}
				
				ArkaniodGameWorld::Collision result = CheckCircleCollision(ball, player);
				if (!ball->GET_COMPONENT(TransformComponent)->Stuck && std::get<0>(result)) {
					GLfloat centerBoard = player->GET_COMPONENT(TransformComponent)->position.x + player->GET_COMPONENT(TransformComponent)->scale.x / 2;
					GLfloat distance = (ball->GET_COMPONENT(TransformComponent)->position.x + ball->GET_COMPONENT(TransformComponent)->Radius) - centerBoard;
					GLfloat percentage = distance / (player->GET_COMPONENT(TransformComponent)->scale.x / 2);

					GLfloat strength = 2.0f;
					glm::vec3 oldVelocity = glm::vec3(ball->GET_COMPONENT(TransformComponent)->Velocity.x, ball->GET_COMPONENT(TransformComponent)->Velocity.y, 0.0f);
					ball->GET_COMPONENT(TransformComponent)->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;					
					ball->GET_COMPONENT(TransformComponent)->Velocity = glm::normalize(ball->GET_COMPONENT(TransformComponent)->Velocity) *
						glm::length(oldVelocity); // Keep speed consistent over both axes
					// Fix sticky paddle
					ball->GET_COMPONENT(TransformComponent)->Velocity.y = -1 * std::abs(ball->GET_COMPONENT(TransformComponent)->Velocity.y);
				}
			}

			if (ball->GET_COMPONENT(TransformComponent)->position.y >= 600) { //did ball reach bottom edge.
				ResetLevel();
				ResetPlayer(player);
			}
		}

		void ArkaniodGameWorld::ProgressLevel(int levelID) {
			SpacePointer gameSpace = ENGINE->GetSpace("Game");
			gameSpace->ClearSpace();
			gameSpace->AddSystem(GETSYS(CameraSystem));
			gameSpace->AddSystem(GETSYS(GLGraphics));			
			LoadLevelEntitiesIntoSpace(levels.at(levelID), gameSpace, Constants::SCREENWITDH, Constants::SCREENHEIGHT * 0.5);
		}

		void ArkaniodGameWorld::ShutDown() {
			// We probably need to extend this beyond the logical space to ALL the spaces in this gamestate
			if (ENGINE->SpaceExists(GetLogicalSpace())) {
				ENGINE->GetSpace(GetLogicalSpace())->ClearSpace();
			}
		}

		// TODO revaluated if this is suppose to happen here - seems best suited for an input system that works off of interactable component
		void ArkaniodGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {

			EntityPointer player = ENGINE->GetSpace("Game")->GetEntityByName("player");
			EntityPointer ball = ENGINE->GetSpace("Game")->GetEntityByName("ball");
			//the dt here might be laggy
			GLfloat velocity = player->GET_COMPONENT(TransformComponent)->constantVelocity * ENGINE->GetDt();

			switch (message) {
			case Message::MSG_A_PRESS:
				if (player->GET_COMPONENT(TransformComponent)->position.x >= 0) {
					player->GET_COMPONENT(TransformComponent)->position.x -= velocity;
					if (ball->GET_COMPONENT(TransformComponent)->Stuck) {
						ball->GET_COMPONENT(TransformComponent)->position.x -= velocity;
					}
				}
				break;
			case Message::MSG_D_PRESS:
				// subtract player size.
				if (player->GET_COMPONENT(TransformComponent)->position.x <= 800.0f - 100.0f) {
					player->GET_COMPONENT(TransformComponent)->position.x += velocity;
					if (ball->GET_COMPONENT(TransformComponent)->Stuck) {						
						ball->GET_COMPONENT(TransformComponent)->position.x += velocity;						
					}
				}
				break;
			case Message::MSG_SPACE_PRESS:
				ball->GET_COMPONENT(TransformComponent)->Stuck = false;
				break;
			}
		}

		void ArkaniodGameWorld::LoadResourceData() {

			std::vector<GLfloat> meshVerticies = {
				// Pos				// Tex
				0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

				0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				1.0f, 0.0f, 0.0f,  1.0f, 0.0f
			};

			//Sprite::MeshShared sprite = std::make_shared<Sprite::Mesh>("spriteSource", meshVerticies, 6, 0, Sprite::Mesh::MeshType::QUAD);

			Systems::ResourceManagerShared manager = ENGINE->GetResourceManager();
			//manager->AddMesh(sprite);
			manager->LoadSpriteResourcesIntoBuffers();

//			manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/block.png", false, "block");
			//manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/block_solid.png", false, "block_solid");
		//	manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/awesomeface.png", true, "face");
			//manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/background.jpg", false, "background");
			//manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/paddle.png", true, "wall");
			//manager->LoadTextureDataFromFile("../ECEngine/Engine/Core/Systems/Resources/assets/particle.png", true, "particle");

			//manager->CreateBasicTexture();
		}

		void ArkaniodGameWorld::CreateEntities() {

			EntityPointer backgroundEntity = std::make_shared<Entity>();

			backgroundEntity->AddComponent(std::make_shared<TransformComponent>());
			backgroundEntity->AddComponent(std::make_shared<SpriteComponent>());
			backgroundEntity->SetName("background");

			//have the -0.5 be a setting for background entities
			//check if every pixel from background entities is being loaded
			backgroundEntity->GET_COMPONENT(TransformComponent)->position = glm::vec3(0.0f, 0.0f, -0.5f);
			backgroundEntity->GET_COMPONENT(TransformComponent)->scale = glm::vec3(Constants::SCREEN_WIDTH_FLOAT, 
				Constants::SCREEN_HEIGHT_FLOAT, 0.0f);
			backgroundEntity->GET_COMPONENT(SpriteComponent)->color = glm::vec4(1.0f);
			backgroundEntity->GET_COMPONENT(SpriteComponent)->textureName = "background";
			backgroundEntity->GET_COMPONENT(SpriteComponent)->spriteTypeName = "spriteSource";
			backgroundEntity->GET_COMPONENT(SpriteComponent)->shaderName = "2Dshader";

			ENGINE->GetSpace("Background")->AddEntity(backgroundEntity);

			EntityPointer player = std::make_shared<Entity>();

			TransformComponentPointer playerTransform = std::make_shared<TransformComponent>();
			SpritePointer playerSprite = std::make_shared<SpriteComponent>();			

			playerTransform->scale = glm::vec3(100.f, 20.0f, 0.0f);
			playerTransform->constantVelocity = 500.0f;
			playerTransform->position = glm::vec3(350.0f, 580.0f, 0.0f);

			playerSprite->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			playerSprite->textureName = "wall";
			playerSprite->spriteTypeName = "spriteSource";
			playerSprite->shaderName = "2Dshader";

			player->SetName("player");
			player->AddComponent(playerTransform);
			player->AddComponent(playerSprite);

			ENGINE->GetSpace("Game")->AddEntity(player);

			glm::vec3 ballPosition = playerTransform->position + glm::vec3(playerTransform->scale.x / 2 - BALL_RADIUS,
				-BALL_RADIUS * 2, 0.0f);
			createBall(ballPosition, BALL_RADIUS, INITIAL_BALL_VELOCITY, "face");
		}

		void ArkaniodGameWorld::CreateParticleEntities() {

			for (int i = 0; i < 500; i++) {
				EntityPointer particle = std::make_shared<Entity>();
				TransformComponentPointer transform = std::make_shared<TransformComponent>();
				SpritePointer sprite = std::make_shared<SpriteComponent>();
				ParticlePointer particleComponent = std::make_shared<Particle>();

				transform->position = glm::vec3(0.0f);
				transform->Velocity = glm::vec3(0.0f);
				transform->Offset = glm::vec3(BALL_RADIUS / 2, BALL_RADIUS / 2, 0.0f);
				//this does not get used since there is an automatic scale in the shader.
				transform->scale = glm::vec3(0.0f);
				transform->Stuck = false;

				sprite->spriteTypeName = "spriteSource";
				//sprite->Alpha = 1.0f;
				sprite->shaderName = "particleShader";
				sprite->textureName = "face";
				sprite->color = glm::vec4(1.0f);
				sprite->IsDestroyed = true;

				particleComponent->OriginName = "ball";
				particleComponent->Life = 0.0f;

				particle->AddComponent(transform);
				particle->AddComponent(sprite);
				particle->AddComponent(particleComponent);
				particle->SetName("particle");

				ENGINE->GetSpace("Game")->AddEntity(particle);
			}		
		}
		
		//eventually needs to be serialized data.....and in the resource handler
		std::vector<std::vector<GLuint>> ArkaniodGameWorld::LoadTileDataFromFile(const GLchar* file) {
			GLuint tileCode;
			std::string line;
			std::ifstream fileDataStream(file);
			std::vector<std::vector<GLuint>> tileData;

			//This code basically reads the file and creates a vector of vectors in correspondence to how the data is organized
			if (fileDataStream) {
				while (std::getline(fileDataStream, line)) { //read each line from file
					std::istringstream sstream(line);
					std::vector<GLuint> tileRow;
					while (sstream >> tileCode) {//read each word separated by spaces
						tileRow.push_back(tileCode);
					}
					tileData.push_back(tileRow);
				}
			}
			return tileData;
		}

		//if (tileData.size() > 0) -> check before running this function
		void ArkaniodGameWorld::LoadLevelEntitiesIntoSpace(std::vector<std::vector<GLuint>>& tileData, SpacePointer space, GLuint levelWidth, GLuint levelHeight) {
			//Calculate Dimensions
			GLuint height = tileData.size();
			//the width of all rows should be the same
			GLuint width = tileData[0].size();
			GLfloat unitWidth = levelWidth / static_cast<GLfloat>(width);
			GLfloat unitHeight = levelHeight / height;
			
			//Initialize level tiles based on tileData
			for (GLuint y = 0; y < height; y++) {
				for (GLuint x = 0; x < width; x++) {
					if (tileData[y][x] == 1) { //Solid
						glm::vec3 pos(unitWidth * x, unitHeight * y, -0.1);
						glm::vec3 size(unitWidth, unitHeight, 0.0);
						EntityPointer block = std::make_shared<Entity>();
						block->SetName("block");
						TransformComponentPointer transformComponent = std::make_shared<TransformComponent>();
						SpritePointer spriteComponent = std::make_shared<SpriteComponent>();
						transformComponent->position = pos;
						transformComponent->rotation = 0.0f;
						transformComponent->scale = size;
						transformComponent->constantVelocity = 0.0f;
						spriteComponent->color = glm::vec4(0.8f, 0.8f, 0.7f, 1.0f);
						spriteComponent->textureName = "block_solid";
						// TODO: Created a block source
						spriteComponent->spriteTypeName = "spriteSource";
						spriteComponent->shaderName = "2Dshader";
						spriteComponent->IsSolid = GL_TRUE;						

						block->AddComponent(transformComponent);
						block->AddComponent(spriteComponent);
						space->AddEntity(block);
					}
					else if (tileData[y][x] > 1) {
						glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
						if (tileData[y][x] == 2) {
							color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
						}
						else if (tileData[y][x] == 3) {
							color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
						}
						else if (tileData[y][x] == 4) {
							color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
						}
						else if (tileData[y][x] == 5) {
							color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
						}

						EntityPointer block = std::make_shared<Entity>();
						block->SetName("block");
						TransformComponentPointer transformComponent = std::make_shared<TransformComponent>();
						SpritePointer spriteComponent = std::make_shared<SpriteComponent>();
						transformComponent->position = glm::vec3(unitWidth * x, unitHeight * y, -0.1);
						transformComponent->rotation = 0.0f;
						transformComponent->scale = glm::vec3(unitWidth, unitHeight, 0.0);
						transformComponent->constantVelocity = 0.0f;
						spriteComponent->color = color;
						spriteComponent->textureName = "block";
						// TODO: Created a block source
						spriteComponent->spriteTypeName = "spriteSource";
						spriteComponent->shaderName = "2Dshader";
						spriteComponent->IsSolid = GL_FALSE;						

						block->AddComponent(transformComponent);
						block->AddComponent(spriteComponent);
						space->AddEntity(block);
					}
				}
			}

		}

		void ArkaniodGameWorld::RealTimeKeys() {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_SPACE_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_A_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_D_PRESS);
			}
		}
	
		// TODO make this into a generic method. Later put it in the factory...something else should be getting this data possibly?
		void ArkaniodGameWorld::createBall(glm::vec3 position, float radius, glm::vec3 velocity, std::string textureName) {
			EntityPointer ball = std::make_shared<Entity>();
			ball->SetName("ball");
			TransformComponentPointer transform = std::make_shared<TransformComponent>();
			SpritePointer sprite = std::make_shared<SpriteComponent>();

			transform->position = position;
			transform->Radius = radius;
			transform->Velocity = velocity;
			transform->scale = glm::vec3(radius * 2, radius * 2, 0.0f);

			sprite->textureName = textureName;
			sprite->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			sprite->shaderName = "2Dshader";
			sprite->spriteTypeName = "spriteSource";

			ball->AddComponent(transform);
			ball->AddComponent(sprite);

			ENGINE->GetSpace("Game")->AddEntity(ball);
		}

		// TODO Needs to be in a physics system
		void ArkaniodGameWorld::MoveBall(EntityPointer ball, float dt) {
			// This line probably does not need to be in the loop.
			TransformComponentPointer transform = ball->GET_COMPONENT(TransformComponent);
			if (!transform->Stuck) {
				transform->position += transform->Velocity * dt;
				if (transform->position.x <= 0) {
					transform->Velocity.x = -transform->Velocity.x;
					transform->position.x = 0.0f;
				}
				else if (transform->position.x + transform->scale.x >= 800) {
					std::cout << transform->position.x << std::endl;
					transform->Velocity.x = -transform->Velocity.x;
					transform->position.x = 800.0f - transform->scale.x;
				}
				if (transform->position.y <= 0.0f) {
					transform->Velocity.y = -transform->Velocity.y;
					transform->position.y = 0.0f;
				}
			}
			//this is probably not necessarry
			//return transform->position;
		}

		void ArkaniodGameWorld::ResetBall(EntityPointer ball, glm::vec3 position, glm::vec3 velocity) {
			ball->GET_COMPONENT(TransformComponent)->position = position;
			ball->GET_COMPONENT(TransformComponent)->Velocity = velocity;
			ball->GET_COMPONENT(TransformComponent)->Stuck = true;
		}

		void ArkaniodGameWorld::ResetPlayer(EntityPointer player) {
			player->GET_COMPONENT(TransformComponent)->scale = PLAYER_SIZE;
			player->GET_COMPONENT(TransformComponent)->position = glm::vec3(800 / 2 - player->GET_COMPONENT(TransformComponent)->scale.x / 2,
				600 - player->GET_COMPONENT(TransformComponent)->scale.y, 0.0f);
			EntityPointer ball = ENGINE->GetSpace("Game")->GetEntityByName("ball");
			ResetBall(ball, player->GET_COMPONENT(TransformComponent)->position + glm::vec3(PLAYER_SIZE.x / 2 - BALL_RADIUS,
				-(BALL_RADIUS * 2), 0.0f), INITIAL_BALL_VELOCITY);
		}

		void ArkaniodGameWorld::ResetLevel() {
			LoadLevelEntitiesIntoSpace(levels.at(_currentLevel), ENGINE->GetSpace("Game"), 800, 600 * 0.5);
		}

		GLboolean ArkaniodGameWorld::CheckCollision(EntityPointer& entityOne, EntityPointer& entityTwo) {
			glm::vec3 entityOnePosition = entityOne->GET_COMPONENT(TransformComponent)->position;
			glm::vec3 entityOneScale = entityOne->GET_COMPONENT(TransformComponent)->scale;
			glm::vec3 entityTwoPosition = entityTwo->GET_COMPONENT(TransformComponent)->position;
			glm::vec3 entityTwoScale = entityTwo->GET_COMPONENT(TransformComponent)->scale;
			bool collisionX = entityOnePosition.x + entityOneScale.x >= entityTwoPosition.x &&
				entityTwoPosition.x + entityTwoScale.x >= entityOnePosition.x;
			bool collisionY = entityOnePosition.y + entityOneScale.y >= entityTwoPosition.y &&
				entityTwoPosition.y + entityTwoScale.y >= entityOnePosition.y;
			return collisionX && collisionY;
		}

		float ArkaniodGameWorld::Clamp(float value, float min, float max) {
			return std::max(min, std::min(max, value));
		}

		ArkaniodGameWorld::Collision ArkaniodGameWorld::CheckCircleCollision(EntityPointer& ball, EntityPointer& entity) {
			TransformComponentPointer ballTransform = ball->GET_COMPONENT(TransformComponent);
			TransformComponentPointer entityTransform = entity->GET_COMPONENT(TransformComponent);			
			// Get center point circle first
			//we dont want to take the z coordinate when doing this
			glm::vec3 centerWithZ(ballTransform->position + ballTransform->Radius);
			glm::vec3 center(centerWithZ.x, centerWithZ.y, 0.0f);
			// Calculate AABB info (center, half-extents)
			glm::vec3 aabbHalfExtents(entityTransform->scale.x / 2, entityTransform->scale.y / 2, 0.0f);
			glm::vec3 aabbCenter(entityTransform->position.x + aabbHalfExtents.x, entityTransform->position.y + aabbHalfExtents.y, 0.0f);
			glm::vec3 difference = center - aabbCenter;
			glm::vec3 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
			glm::vec3 closest = aabbCenter + clamped;

			difference = closest - center;

			if (glm::length(difference) <= ballTransform->Radius) {
				return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
			}
			else {
				return std::make_tuple(GL_FALSE, ArkaniodGameWorld::Direction::UP, glm::vec3(0.0f, 0.0f, 0.0f));
			}
			//return glm::length(difference) < ballTransform->Radius;
		}

		ArkaniodGameWorld::Direction ArkaniodGameWorld::VectorDirection(glm::vec3 target) {
			glm::vec3 compass[] = {
				glm::vec3(0.0f, 1.0f, 0.0f), // up
				glm::vec3(1.0f, 0.0f, 0.0f), // right
				glm::vec3(0.0f, -1.0f, 0.0f), //down
				glm::vec3(-1.0f, 0.0f, 0.0f), //left
			};

			GLfloat max = 0.0f;
			GLuint bestMatch = -1;

			for (GLuint i = 0; i < 4; i++) {
				GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
				if (dotProduct > max) {
					max = dotProduct;
					bestMatch = i;
				}
			}
			return (ArkaniodGameWorld::Direction)bestMatch;
		}
	}
}