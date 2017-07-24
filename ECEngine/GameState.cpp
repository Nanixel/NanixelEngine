#include "stdafx.h"

#include "./GameState.h"

namespace Engine {


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