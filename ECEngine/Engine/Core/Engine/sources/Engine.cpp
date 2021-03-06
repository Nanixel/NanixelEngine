#include "stdafx.h"

#include "../ECEngine/Engine/Core/Libraries/Timer.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"

#include <iostream>
#include <cassert>
#include "../ECEngine/Engine/Core/Systems/SystemsInclude.h"


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
		
		_resourceManager = std::make_shared<Systems::ResourceManager>();

		//the order of how these are added dictates how they are updated
		systems.push_back(std::make_shared<Systems::CustomWindow>());
		systems.push_back(std::make_shared<Systems::CameraSystem>());
		systems.push_back(std::make_shared<Systems::ParticleSystem>());
		systems.push_back(std::make_shared<Systems::GLGraphics>());
		
		//create a new gameworld space to play in with a default camera and adds it to our space map
		SpacePointer gameWorldSpace = CreateSpace("Background");
		//sets the engines active spaces to newly created space
		SetActiveSpace("Background");
		
		using namespace Systems;

		gameWorldSpace->AddSystem(GETSYS(CameraSystem));
		gameWorldSpace->AddSystem(GETSYS(GLGraphics));		

		for (auto sys : systems) {
			//can't work with entities on initialization so cant do any sort of preprocessing
			sys->Init();
		}
	}	

	void Engine::ShutDown() {
		//the systems will shutdown in the order they are added 
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

	void Engine::Update(float dt) {
		
		GameStatePointer gameState = CurrentState(); //Arkaniod Game

		//we set active space here to ensure that the mouse will be in worldspace coordinates for that space when updating the window
		SetActiveSpace(gameState->GetLogicalSpace());

		using Systems::CustomWindow;
		GETSYS(CustomWindow)->Update(dt);

		using Systems::GLGraphics;
		GETSYS(GLGraphics)->NewFrame();


		for (auto space = spaces.begin(); space != spaces.end(); ++space) { 
			//IM NOT SURE IF THIS IS A GOOD IDEA
			SetActiveSpace(space->first);

			//IF THIS IS NOT TRUE WE DONT NEED TO UPDATE THE GAME LOGIC FOR THESE SPACES
			if (space->first == gameState->GetLogicalSpace()) {				
				space->second->PopulateSystemEntities(gameState);
				gameState->Update(dt); 
			}

			//IF THIS SPACE IS NOT IN THE CURRENT GAMESTATE, THEN WE DO NOT NEED TO PASS ITS ENTITES TO OUR SYSTEMS FOR UPDATES
			if (gameState->CheckSpaceActive(space->first)) { 
				space->second->Update(dt); 
			}
		}
		//GETSYS(CustomWindow)->endFrame();
	}

	void Engine::mainLoop() {

		dt = 1.0f / 60.0f;		

		while (running) {
			//scope timer starts timing when its created and only stops once it leaves scope
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
			spaces.emplace(name, std::make_shared<Space>(name));
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