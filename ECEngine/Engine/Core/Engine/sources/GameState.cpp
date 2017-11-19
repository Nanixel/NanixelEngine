#include "stdafx.h"

#include "../ECEngine/Engine/Core/Engine/headers/GameState.h"

namespace Engine {


	// TODO: this needs to be more robust
	bool GameState::CheckSpaceActive(std::string name) {

		for (auto it : spaces) {

			if (it == name) {
				return true;
			}

		}

		return false;
	}

	void GameState::SetLogicalSpace(std::string name) {
		logicalSpace = name;
	}




}