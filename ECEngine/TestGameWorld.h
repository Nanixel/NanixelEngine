#pragma once

#include <memory>
#include "./GameState.h"
#include "./System.h"
#include "./ComponentsInclude.h"

namespace Engine {

	namespace GameStates {
		
		class TestGameWorld : public GameState
		{
		public:
			TestGameWorld() : GameState("Test World GameState", State_TestDemo) {
				//Engine calls init manually
			}

			~TestGameWorld() {}

			void Init();
			void Update(float dt);
			void ShutDown();
			void SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message);

		private:
			//some custom stuff you wanna do with sqaures
			
			void SpawnDefaultBoxes();			
			void RealTimeKeys();
			EntityPointerList boxList;
		};

		using TestGameWorldPointer = std::shared_ptr<TestGameWorld>;
	}

}



