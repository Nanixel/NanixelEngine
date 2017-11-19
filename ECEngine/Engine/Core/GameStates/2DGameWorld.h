#pragma once


#include <memory>
#include "../ECEngine/Engine/Core/Engine/headers/GameState.h"
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngine/Engine/Core/Components/ComponentsInclude.h"
#include <GL/glew.h>

namespace Engine {

	namespace GameStates {
		//this serves as the main game world for arkanoid game
		class ArkaniodGameWorld : public GameState
		{
		public:

			enum Direction {
				UP,
				RIGHT,
				DOWN,
				LEFT
			};

			ArkaniodGameWorld() : GameState("Arkanoid", GameStateType::State_InGame) {
				//Engine calls init manually
			}

			~ArkaniodGameWorld() {}

			void Init();

			void SetUpSpaces();

			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message);

			std::vector<std::vector<GLuint>> LoadTileDataFromFile(const GLchar* file);
			void LoadLevelEntitiesIntoSpace(std::vector<std::vector<GLuint>>& tileData, SpacePointer space, GLuint levelWidth, GLuint levelHeight);
			//GLboolean GameCompleted();

		private:

			//void AddCustomShader();
			void SetupGameWorldEntities(SpacePointer space);
			void LoadResourceData();
			void CreateEntities();
			void CreateParticleEntities();
			void RealTimeKeys();
			void ProgressLevel(int levelID);

			// TODO this is all Physics stuff
			GLboolean CheckCollision(EntityPointer& entityOne, EntityPointer& entityTwo);
			void createBall(glm::vec3 position, float radius, glm::vec3 velocity, std::string textureName);
			// TODO create a physics system for this!
			void MoveBall(EntityPointer ball, float dt); //this just moves things based on shape
			void ResetBall(EntityPointer ball, glm::vec3 position, glm::vec3 velocity); //this just resets position and velocity.
			void ResetLevel();
			void ResetPlayer(EntityPointer player);
			float Clamp(float value, float min, float max);

			using Collision = std::tuple<GLboolean, ArkaniodGameWorld::Direction, glm::vec3>;

			Collision CheckCircleCollision(EntityPointer& ball, EntityPointer& entity);

			Direction VectorDirection(glm::vec3 target);

			using LevelData = std::unordered_map<int, std::vector<std::vector<GLuint>>>;
			//This holds the tile data...we may want to delete this if there is no back tracking to past levels
			//No point into holding on to the resource data when we are not using it...
			LevelData levels;
			int _currentLevel = 0;
			GLuint particlesCount = 500;

		};

		using ArkaniodGameWorldPointer = std::shared_ptr<ArkaniodGameWorld>;
	}

}