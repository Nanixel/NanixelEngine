#include "stdafx.h"


#include "./Timer.h"

#include "./Engine.h"

#include <iostream>
#include <cassert>
#include "./SystemsInclude.h"

using std::cout;

using Time::ScopeTimer;


namespace Engine {


	Engine* ENGINE = nullptr;

	Engine::Engine() {

		assert(ENGINE == nullptr);

		ENGINE = this;
	}

	void Engine::Init() {


		running = true;

		//add our systems to the engine		
		systems.push_back(SystemPointer(new Systems::CustomWindow()));
		systems.push_back(SystemPointer(new Systems::CameraSystem()));		
		systems.push_back(SystemPointer(new Systems::GLGraphics()));

		//button controller
		
		//create a new gameworld space to play in
		SpacePointer gameWorld = CreateSpace("Test GameWorld");

		//set this as our active space
		SetActiveSpace("Test GameWorld");


		//system types are not defined outside the 'Systems' namespace
		using namespace Systems;
		//add systems to our space
		gameWorld->AddSystem(GETSYS(CameraSystem));
		gameWorld->AddSystem(GETSYS(GLGraphics));

		for (auto sys : systems) {
			//initialize all of our systems
			sys->Init();
		}
	}

	void Engine::ShutDown() {

		//THIS IS PROBABLY NO GOOD -> Need to shut down systems in the correct order!!!
		//possibly sort this list before doing this
		for (auto sys : systems) {
			sys->ShutDown();
		}

		systems.clear();
		spaces.clear();

		while (!gameStateStack.empty()) {
			PopGameState();
		}

		ENGINE = nullptr;
	}

	//TODO - Dont currently have any concept of what a frame is....

	//dt is deltaTime
	//TODO - this needs to do something with the camera
	void Engine::Update(float dt) {

		//we set active space here to ensure that the mouse will be in worldspace coordinates for that space
		SetActiveSpace(CurrentState()->GetLogicalSpace());

		//update window management system -> responsible for window and input
		using Systems::CustomWindow;
		GETSYS(CustomWindow)->Update(dt);
		//GETSYS(CustomWindow)->newFrame();

		//the current statue should be set up by now...if its not then we need to error check here
		//the initial state is set up in game -> the rest can be set up between the gamestates them selves (maybe a gamestate map manager?)
		GameStatePointer gameState = CurrentState();

		//remeber we set up a space in the Init() of this Engine
		for (auto space = spaces.begin(); space != spaces.end(); ++space) {

			//space->first is the name in the map!
			SetActiveSpace(space->first);

			//if this is the space the current gamestate wants to use for updating
			if (space->first == gameState->GetLogicalSpace()) {			
			//	//keep in mind that the game state holds certain entities
				space->second->PopulateSystemEntities(gameState);
				gameState->Update(dt);
			}

			if (gameState->CheckSpaceActive(space->first)) {
				space->second->Update(dt);
			}
		}
		//GETSYS(CustomWindow)->endFrame();
	}

	void Engine::mainLoop() {

		dt = 1.0f / 60.0f;

		while (running) {

			ScopeTimer frameTimer(&dt);

			Update(dt);
		}

	}

	SpacePointer Engine::CreateSpace(std::string name) {
		auto space = spaces.find(name);
		if (space != spaces.end()) {
			return (*space).second;
		}
		else {
			//making a new space automatically makes a new camera
			spaces.emplace(name, SpacePointer(new Space(name)));
		}

		return GetSpace(name);
	}


	SpacePointer Engine::GetSpace(std::string name) {
		SpaceMap::iterator it = spaces.find(name);

		if (it != spaces.end()) {
			return it->second;
		}


		return CreateSpace(name);

		throw std::range_error("The specified space does not exist.");

	}

	SpacePointer Engine::SetActiveSpace(std::string name) {
		auto space = spaces.find(name);

		if (space != spaces.end()) {
			activeSpace = name;
			return GetActiveSpace();
		}

		throw std::range_error("Tried to set the active space to a space that does not exist.");
	}

	SpacePointer Engine::GetActiveSpace() {
		auto active = spaces.find(activeSpace);
		return active->second;
	}

	bool Engine::SpaceExists(std::string name) {
		auto space = spaces.find(name);
		if (space != spaces.end()) {
			return true;
		}

		return false;
	}

	ArchtypeFactory& Engine::Factory() {
		return archetypeFactory;
	}

	void Engine::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
		for (auto& it : systems) {
			try {
				it->SendMsg(firstEntity, secondEntity, message);
			}
			catch (...) {

			}
		}

		CurrentState()->SendMsg(firstEntity, secondEntity, message);
	}

	GameStatePointer Engine::CurrentState() const {
		return gameStateStack.top();		
	}

	void Engine::PushGameState(GameStatePointer gs) {
		gameStateStack.push(gs);

		//THIS IS WHERE THE GAMESTATE IS INITIALIZED!!!!!!
		gs->Init();
	}

	void Engine::PopGameState() {
		GameStateType gsType = gameStateStack.top()->State();
		gameStateStack.top()->ShutDown();
		gameStateStack.pop();

		if (gameStateStack.size()) {
			gameStateStack.top()->PoppedTo(gsType);
		}
		else {
			Stop();
		}
	}
}