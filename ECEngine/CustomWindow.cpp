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
		CustomWindow* CustomWindow::Instance = nullptr;


		CustomWindow::CustomWindow() : Width(800), Height(600), caption("Default Window"), System(std::string("WindowSys"), SystemType::EC_CustomWindow) {
			Instance = this;
		}

		CustomWindow::CustomWindow(int _width, int _height, std::string _caption) : Width(_width), Height(_height), caption(_caption), System(std::string("WindowSys"), SystemType::EC_CustomWindow)
		{
			Instance = this;
		}
		CustomWindow::~CustomWindow()
		{
			Instance = nullptr;
		}

		//#######################################################################################

		void CustomWindow::Init() {

			RegisterComponent(MC_NOOBJECTS);

			setContextSettings();
			//reset takes ownership of this object
			OpenGLContext.reset(new sf::Window(sf::VideoMode(Width, Height), caption, sf::Style::Default, contextSettings));
			OpenGLContext->setVerticalSyncEnabled(true);
			glewInit();
			if (!OpenGLContext) {
				//something
				std::exception("No OpenGL target found");
			}
			
		}
		void CustomWindow::setContextSettings() {
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
			//----Handles Start Frame and End Frame----
			PollEvents();
			//NewFrame();
			OpenGLContext->display();
		}

		//#######################################################################################}

		void CustomWindow::PollEvents() {
			//while an event exists
			while (OpenGLContext->pollEvent(currentEvent)) {
				PollWindowEvent();				
				PollKeyboardEvent();
				PollMouseEvent();

				//call for every event every frame
				for (auto& func : EventFunctionSubscribers) {
					func();
				}
			}
		}

		void CustomWindow::PollWindowEvent() {
			if (currentEvent.type == sf::Event::Closed) {
				ENGINE->Stop();
			}
		}

		void CustomWindow::PollKeyboardEvent() {
			//TODO -> this stuff may be better suited for an input controller
			
			if (currentEvent.type == sf::Event::KeyPressed) {
				switch (currentEvent.key.code)
				{
				case sf::Keyboard::A:					
					//THIS WILL SEND THE MESSAGE TO ALL SYSTEMS IN THE ENGINE
					ENGINE->SendMsg(nullptr, nullptr, Message::MSG_A_PRESS);
					break;
				case sf::Keyboard::S:					
					ENGINE->SendMsg(nullptr, nullptr, Message::MSG_S_PRESS);
					break;
				case sf::Keyboard::D:					
					ENGINE->SendMsg(nullptr, nullptr, Message::MSG_D_PRESS);
					break;
				case sf::Keyboard::W:					
					ENGINE->SendMsg(nullptr, nullptr, Message::MSG_W_PRESS);
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

			Buttons button;
			if (currentEvent.type == sf::Event::MouseButtonPressed) {
				switch (currentEvent.mouseButton.button) {
				case sf::Mouse::Button::Left:
					button = Buttons::Left;
					break;
				case sf::Mouse::Button::Right:
					button = Buttons::Right;
					break;
				case sf::Mouse::Button::Middle:
					button = Buttons::Middle;
					break;
				default:
					break;
				}
			}

			if (currentEvent.type == sf::Event::MouseMoved) {
				ENGINE->SendMsg(nullptr, nullptr, Message::MSG_MOUSE_MOVE);
			}

			Wheels wheel;

			switch (currentEvent.mouseWheelScroll.wheel) {

			case sf::Mouse::Wheel::VerticalWheel:
				wheel = Wheels::Vertical;
				break;
			default:
				break;
			}			
		}

		void CustomWindow::NewFrame() {
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void CustomWindow::SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message) {
			//THESE KEY MESSAGES CAN BE CAUGHT BY ANYONE!!!!
			switch (message) {
			case Message::MSG_A_PRESS:
				std::cout << "A has been pressed" << std::endl;
				break;
			case Message::MSG_D_PRESS:
				std::cout << "D has been pressed" << std::endl;
				break;
			case Message::MSG_ESC_PRESS:
				ENGINE->Stop();
				break;
			case Message::MSG_MOUSE_MOVE:
				std::cout << "Mouse is moving" << std::endl;
				break;
			}
		}


		void CustomWindow::HideMouseCursor() {
			OpenGLContext->setMouseCursorVisible(false);
			OpenGLContext->setMouseCursorGrabbed(true);
			sf::Mouse::setPosition(sf::Vector2i(Width / 2, Height / 2), *OpenGLContext);
		}

		glm::vec2 GetMousePosition() {
			float x = sf::Mouse::getPosition().x;
			float y = sf::Mouse::getPosition().y;
			return glm::vec2(x, y);
		}

	}
	//#######################################################################################
}


