#include "stdafx.h"
#include "TestGameWorld.h"
#include "./Engine.h"
#include "./ComponentsInclude.h"
#include "./SystemsInclude.h"
#include <math.h>
#include <sstream>
#include <cstdlib>
#include <time.h>

namespace Engine {

	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {

		glm::vec3 sampleCubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		
		//Dont think anything calls this init right now
		void TestGameWorld::Init() {

			//Register the spaces that should be updated with this system
			RegisterSpace("Test GameWorld");

			//Set Which space the gamestates Update function should be used with
			SetLogicalSpace("Test GameWorld");

			SpacePointer gameWorld = ENGINE->GetSpace("Test GameWorld");

			gameWorld->ClearSpace();

			//all we are doing is moving through boxes...
			gameWorld->AddSystem(GETSYS(CameraSystem));
			gameWorld->AddSystem(GETSYS(GLGraphics));

			//register for drawable objects
			RegisterComponent(MC_Transform);
			RegisterComponent(MC_Drawable);

			//there is no concept of a player for now, just a camera
			//create our default box entities

			SpawnDefaultBoxes();

	
			//TransformComponentPointer cameraTransform = gameWorld->GetCamera()->GET_COMPONENT(TransformComponent);			

		}

		void TestGameWorld::Update(float dt) {
			//just have to update the camera here and take user input
			//I can get the camera through the engines space
			//This method is responsible for updating the camera!!!!
		}

		void TestGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("Test GameWorld")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}

		}

		void TestGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {

			//maybe use this to controll the camera for now

		}

		void TestGameWorld::SpawnDefaultBoxes() {

			for (int i{ 0 }; i < 10; i++) {

				EntityPointer boxEntity = ENGINE->Factory().create("Box");
				boxEntity->GET_COMPONENT(TransformComponent)->position = sampleCubePositions[i];
				boxEntity->GET_COMPONENT(TransformComponent)->rotation = 0.0f;
				boxEntity->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
				//add our entity to the active space
				//PopulateSystemEntites() is then called by the engine to add the spaces entities to each system in the space (This includes GLGraphics)
				//On GLGraphic's Update(), it loops through its entites and draws it (thereby drawing the boxes that I make here)
				ENGINE->GetActiveSpace()->AddEntity(boxEntity);
			}			

		}

		void TestGameWorld::realTimeKeys(float dt) {
			CameraComponentPointer camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

			}
		}

		void TestGameWorld::realTimeMouse(float dt) {

		}


	}

}

