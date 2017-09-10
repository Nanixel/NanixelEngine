#include "stdafx.h"
#include "../ECEngine/Engine/Core/GameStates/2DGameWorld.h"
#include "../ECEngine/Engine/Core/Engine/headers/Engine.h"
namespace Engine {

	extern Engine* ENGINE;

	using namespace Systems;
	namespace GameStates {

		void ArkaniodGameWorld::Init() {
			//The Test Gameworld space should update with this system
			RegisterSpace("Arkanoid GameWorld");
			SetLogicalSpace("Arkanoid GameWorld");
			SpacePointer gameWorld = ENGINE->GetSpace("Arkanoid GameWorld");
			gameWorld->ClearSpace();

			//adds existing GLGraphics and CameraSystem to our arknoid space
			gameWorld->AddSystem(GETSYS(CameraSystem));
			gameWorld->AddSystem(GETSYS(GLGraphics));

			AddCustomShader();

			RegisterComponent(MC_Transform);
			//RegisterComponent(MC_Drawable);						

			CustomWindowPointer window = GETSYS(CustomWindow);
			window->HideMouseCursor();			
		}

		void ArkaniodGameWorld::ShutDown() {
			if (ENGINE->SpaceExists("ArkaniodSpace")) {
				ENGINE->GetSpace("Test GameWorld")->ClearSpace();
			}
		}
			
		void ArkaniodGameWorld::AddCustomShader() {
			Shaders::ShaderPointer simpleShader = std::make_shared<Shaders::ShaderUtility>();
			simpleShader->LoadShaderFile("2DVertexShader.txt", "2DFragmentShader.txt");
			simpleShader->Compile();

			simpleShader->FindUniforms("model");
			simpleShader->FindUniforms("projection");
			simpleShader->FindUniforms("image");
			simpleShader->FindUniforms("spriteColor");			

			GETSYS(GLGraphics)->AddShader("2Dshader", simpleShader);
		}
	
	}
}