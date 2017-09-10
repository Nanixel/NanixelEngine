#include "stdafx.h"
#include "TestGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
//#include "./ComponentsInclude.h"
//#include "./SystemsInclude.h"
#include <math.h>
#include <sstream>
#include <cstdlib>
#include <time.h>


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

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

			//The Test Gameworld space should update with this system
			RegisterSpace("Test GameWorld");			
			SetLogicalSpace("Test GameWorld");
			SpacePointer gameWorld = ENGINE->GetSpace("Test GameWorld");
			gameWorld->ClearSpace();
			
			gameWorld->AddSystem(GETSYS(CameraSystem));
			gameWorld->AddSystem(GETSYS(GLGraphics));
			
			RegisterComponent(MC_Transform);
			//RegisterComponent(MC_Drawable);						

			CustomWindowPointer window = GETSYS(CustomWindow);
			window->HideMouseCursor();
			
			SpawnDefaultBoxes();			
		}

		void TestGameWorld::Update(float dt) {
			//just have to update the camera here and take user input
			//I can get the camera through the engines space
			//This method is responsible for updating the camera!!!!
			//realTimeMouse(dt);
			CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			RealTimeKeys();
			ENGINE->SendMsg(nullptr, nullptr, Message::MSG_MOUSE_MOVE);
			
		}

		void TestGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("Test GameWorld")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}

		}

		void TestGameWorld::SendMsg(EntityPointer entityOne, EntityPointer entityTwo, Message::Message message) {						
			//Handle some entity on entity messages in this state
		}

		void TestGameWorld::SpawnDefaultBoxes() {
			for (int i = 0; i < 10; i++) {
				EntityPointer box = ENGINE->Factory().create("box");
				box->GET_COMPONENT(TransformComponent)->position = sampleCubePositions[i];
				box->GET_COMPONENT(TransformComponent)->rotation = 0.0f;
				box->GET_COMPONENT(TransformComponent)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
				//add our entity to the active space
				//PopulateSystemEntites() is then called by the engine to add the spaces entities to each system in the space (This includes GLGraphics)
				//On GLGraphic's Update(), it loops through its entites and draws it (thereby drawing the boxes that I make here)
				ENGINE->GetActiveSpace()->AddEntity(box);
			}			
		}

		//program the real time keyboard input...might want to make the engine responsible for this
		void TestGameWorld::RealTimeKeys() {
			//Camera system must process keyboard here
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_A_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_S_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_D_PRESS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_W_PRESS);
			}
		}
	}

}

