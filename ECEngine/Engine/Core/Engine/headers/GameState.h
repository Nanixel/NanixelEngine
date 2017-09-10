#pragma once
#include <vector>
#include <string>
#include <memory>

#include "./Space.h"
#include "./System.h"

namespace Engine {

	enum GameStateType {
		State_MainMenu,
		State_Paused,
		State_TestDemo,
		State_InGame
	};

	class GameState : public System {

	public:

		GameState(std::string name, GameStateType gameStateType) : System(name, SystemType::GameStateSys), state(gameStateType) {}

		virtual ~GameState() {}

		virtual void Init() = 0;
		virtual void Update(float) = 0;
		virtual void ShutDown() = 0;

		//this function is called when gamestate begins active die to a gamestate on top of it being popped
		virtual void PoppedTo(GameStateType gameStateType) {}

		inline GameStateType State() const {
			return state;
		}

		inline const std::vector<std::string>& GetSpaces() const {
			return spaces;
		}

		const std::string& GetLogicalSpace() {
			return logicalSpace;
		}

		bool CheckSpaceActive(std::string space);


	private:
		GameState() = delete;
		std::vector<std::string> spaces;
		const GameStateType state;
		std::string logicalSpace = "Test GameWorld";

	protected:

		void SetLogicalSpace(std::string);

		//registers a space to be updated while this gamestate is active
		inline void RegisterSpace(SpacePointer space) {
			spaces.push_back(space->Name());
		}
		inline void RegisterSpace(std::string name) {
			spaces.push_back(name);
		}

		void UnRegisterSpace(std::string name) {
			auto it = spaces.begin();

			for (; it != spaces.end(); ++it) {
				if (*it == name) {
					spaces.erase(it);
				}
			}
		}
	};

	using GameStatePointer = std::shared_ptr<GameState>;
}