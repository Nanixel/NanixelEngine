#pragma once


#include <memory>
#include "./GameState.h"
#include "./System.h"
#include "./ComponentsInclude.h"

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
			GLfloat basicVerticies[] = {
				//Pos		//Tex
				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f
			};

		};

		using ArkaniodGameWorldPointer = std::shared_ptr<ArkaniodGameWorld>;
	}

}