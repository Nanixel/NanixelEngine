#include "stdafx.h"

#include <GL/glew.h>
#include <iostream>
#include <SFML/OpenGL.hpp>
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
#include <memory>
#include <fstream>



namespace Engine {
	extern Engine* ENGINE;
}

//replace this with main when ready
int main(int argc, char* argv[]) {

	new Engine::Engine(  );

	Engine::ENGINE->Init();

	//Start up the main menu
	//Engine::ENGINE->PushGameState(Engine::GameStates::ArkaniodGameWorldPointer(new Engine::GameStates::ArkaniodGameWorld));
	Engine::ENGINE->PushGameState(Engine::GameStates::TestGameWorldPointer(new Engine::GameStates::TestGameWorld()));

	Engine::ENGINE->mainLoop();

	Engine::ENGINE->ShutDown();

	delete Engine::ENGINE;

	return 0;
}