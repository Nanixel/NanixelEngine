#include "stdafx.h"
#include "CustomWindow.h"
#include "./Engine.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

namespace Engine {

	extern Engine* ENGINE;

	namespace Systems {
		//CustomWindow* CustomWindow::Instance = nullptr;


		CustomWindow::CustomWindow() : Width(800), Height(600), caption("Default Window"), System(std::string("WindowSys"), SystemType::EC_CustomWindow) {
			//Instance = this;
		}

		CustomWindow::CustomWindow(int _width, int _height, std::string _caption) : Width(_width), Height(_height), caption(_caption), System(std::string("WindowSys"), SystemType::EC_CustomWindow)
		{
			//Instance = this;
		}
		CustomWindow::~CustomWindow()
		{
			//Instance = nullptr;
		}

		//#######################################################################################

		void CustomWindow::Init() {

			RegisterComponent(MC_NOOBJECTS);
						
			SetContextSettings();
			//reset takes ownership of this object
			OpenGLContext.reset(new sf::Window(sf::VideoMode(Width, Height), caption, sf::Style::Default, contextSettings));
			OpenGLContext->setVerticalSyncEnabled(true);
			if (!OpenGLContext) {				
				std::exception("No OpenGL target found");
			}									
		}

		void CustomWindow::SetContextSettings() {
			contextSettings.depthBits = 24;
			contextSettings.stencilBits = 8;
			contextSettings.antialiasingLevel = 4;
			contextSettings.majorVersion = 3;
			contextSettings.minorVersion = 3;
		}

		void CustomWindow::ShutDown() {
			OpenGLContext->close();
		}
		void CustomWindow::Update(float dt) {					
		
			PollEvents();

			//if constrainMouse to window setting
			if (true) {
				ConstrainMouseToWindow();
			}

			OpenGLContext->display();
		}

		//#######################################################################################}

		void CustomWindow::PollEvents() {
			
			while (OpenGLContext->pollEvent(currentEvent)) {
				PollWindowEvent();				
				PollKeyboardEvent();
				PollMouseEvent();

				//call for every event every frame
				/*for (auto& func : EventFunctionSubscribers) {
					func();
				}*/
			}
		}

		void CustomWindow::PollWindowEvent() {
			if (currentEvent.type == sf::Event::Closed) {
				ENGINE->Stop();
			}
		}

		void CustomWindow::PollKeyboardEvent() {

			if (currentEvent.type == sf::Event::KeyPressed) {
				switch (currentEvent.key.code)
				{
				case sf::Keyboard::A:															
					break;
				case sf::Keyboard::S:										
					break;
				case sf::Keyboard::D:										
					break;
				case sf::Keyboard::W:										
					break;
				case sf::Keyboard::Escape:					
					ENGINE->SendMsg(nullptr, nullptr, Message::MSG_ESC_PRESS);
					break;
				case sf::Keyboard::Up:					
					break;
				case sf::Keyboard::Down:					
					break;
				default:
					break;
				}
			}
			
		}
		void CustomWindow::PollMouseEvent() {
			//Positioning
			auto mousePosX = currentEvent.mouseButton.x;
			auto mousePosY = currentEvent.mouseButton.y;

			//Buttons button;			
			if (currentEvent.type == sf::Event::MouseButtonPressed) {
				switch (currentEvent.mouseButton.button) {
				case sf::Mouse::Button::Left:					
					break;
				case sf::Mouse::Button::Right:					
					break;
				case sf::Mouse::Button::Middle:					
					break;
				default:
					break;
				}
			}
		
			if (currentEvent.type == sf::Event::MouseMoved) {
				//do something that doenst need to be real time				
			}
			
			switch (currentEvent.mouseWheelScroll.wheel) {

			case sf::Mouse::Wheel::VerticalWheel:				
				break;
			default:
				break;
			}			
		}


		void CustomWindow::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			//THESE KEY MESSAGES CAN BE CAUGHT BY ANYONE!!!!
			switch (message) {
			case Message::MSG_A_PRESS:				
				break;
			case Message::MSG_D_PRESS:				
				break;
			case Message::MSG_ESC_PRESS:
				ENGINE->Stop();
				break;
			case Message::MSG_MOUSE_MOVE:				
				break;
			}
		}


		void CustomWindow::HideMouseCursor() {
			OpenGLContext->setMouseCursorVisible(true);
			OpenGLContext->setMouseCursorGrabbed(true);
			sf::Mouse::setPosition(sf::Vector2i(Width / 2, Height / 2), *OpenGLContext);
		}

		glm::vec2 CustomWindow::GetMousePosition() {
			float x = sf::Mouse::getPosition(*OpenGLContext).x;
			float y = sf::Mouse::getPosition(*OpenGLContext).y;
			return glm::vec2(x, y);
		}

		void CustomWindow::ConstrainMouseToWindow() {
								
			uint32_t maxMousePosRadius = glm::min(Width, Height) / 3;
			glm::vec2 windowCenter(Width / 2, Height / 2);
			glm::vec2 mousePosition(GetMousePosition());
			CameraComponentPointer defaultCamera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(CameraComponent);
			//if the offset of our mouse position and the window's center is 
			if (glm::length(mousePosition - windowCenter) > maxMousePosRadius) {
				sf::Mouse::setPosition(sf::Vector2i((int)windowCenter.x, (int)windowCenter.y), *OpenGLContext);
				defaultCamera->MousePosition.x = windowCenter.x;
				defaultCamera->MousePosition.y = windowCenter.y;
			}			
		}

		
	}
	//#######################################################################################
}


