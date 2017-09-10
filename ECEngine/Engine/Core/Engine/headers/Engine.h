#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>

#include "./GameState.h"
#include "./System.h"
#include "./Entity.h"
#include "./ArchTypeFactory.h"
#include "./Message.h"
#include "./Space.h"

#include "../ECEngine/Engine/Core/Systems/SystemsInclude.h"
#include "../ECEngine/Engine/Core/GameStates/TestGameWorld.h"


namespace Engine {

	struct Engine {

		//construct engine and set global engine pointer
		Engine();

		//Systems are added and initialized, variables are set, and the initial space is created
		void Init();
		//attempts to safely shut down the engine
		void ShutDown();

		//Updating Levels: Window Updated -> Update every space the current gamestate says needs to be updated -> When a space is updated, 
		//it provides each of the systems added to it with a vector of entities that meet the systems registration requirements -> systems are then told to be updated
		void Update(float);

		//Creates a timer and calls Update() repeatedly
		void mainLoop();

		SpacePointer CreateSpace(std::string name);
		
		SpacePointer SetActiveSpace(std::string name);

		SpacePointer GetSpace(std::string name);

		SpacePointer GetActiveSpace();

		bool SpaceExists(std::string name);

		//remember objects are NOT automaticcally added to a space
		ArchtypeFactory& Factory();

		void SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message);

		float GetDt() { return dt; }

		//returns a shared pointer to the requested system
		template<typename T>
		std::shared_ptr<T> GetSystem(SystemType sysType);

		//signal the engine to stop
		void Stop(void) { running = false; };

		GameStatePointer CurrentState() const;

		void PushGameState(GameStatePointer newGameState);

		//pops the current gamestate off the stack
		void PopGameState();

	private:

		std::stack<GameStatePointer> gameStateStack;

		float dt;

		std::string activeSpace;

		using SpaceMap = std::unordered_map<std::string, SpacePointer>;

		SpaceMap spaces;

		SystemPointerList systems;

		ArchtypeFactory archetypeFactory;

		bool running = true;
		bool testingEngineEnabled = false;



	};

#define GETSYS( systype ) \
ENGINE->GetSystem<systype>(EC_##systype)

	template <typename T>
	std::shared_ptr<T> Engine::GetSystem(SystemType sysType) {

		for (auto& it : systems) {
			if (it->_type == sysType) {
				return std::static_pointer_cast<T>(it);
			}
		}

		throw std::range_error("System does not exist!");
	}
}