#pragma once


#include <memory>
#include "../ECEngine/Engine/Core/Engine/headers/GameState.h"
#include "../ECEngine/Engine/Core/Engine/headers/System.h"
#include "../ECEngine/Engine/Core/Components/ComponentsInclude.h"

namespace Engine {

	namespace GameStates {
		//this serves as the main game world for arkanoid game
		class ArkaniodGameWorld : public GameState
		{
		public:
			ArkaniodGameWorld() : GameState("Arkanoid", GameStateType::State_InGame) {
				//Engine calls init manually
			}

			~ArkaniodGameWorld() {}

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message);

		private:
			//some custom stuff you wanna do with sqaures
			void AddCustomShader();
			void SetUpSpaceCamera();
			void LoadResourceData();
			void CreateEntities();
			//GLfloat basicVerticies[] = {
			//	//Pos		//Tex
			//	0.0f, 1.0f, 0.0f, 1.0f,
			//	1.0f, 0.0f, 1.0f, 0.0f,
			//	0.0f, 0.0f, 0.0f, 0.0f,

			//	0.0f, 1.0f, 0.0f, 1.0f,
			//	1.0f, 1.0f, 1.0f, 1.0f,
			//	1.0f, 0.0f, 1.0f, 0.0f
			//};

		};

		using ArkaniodGameWorldPointer = std::shared_ptr<ArkaniodGameWorld>;
	}

}