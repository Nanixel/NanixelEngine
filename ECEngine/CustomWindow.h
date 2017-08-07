#pragma once
#ifndef WINDOWSFML_H
#define WINDOWSFML_H

#include <memory>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "./System.h"
#include <glm/vec2.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <functional>

enum class Wheels {
	Vertical
};

enum class Buttons
{
	Left,
	Middle,
	Right
};

enum class Keys {
	Null,
	Escape,
	Up,
	Down,
	W,
	A,
	S,
	D

};

namespace Engine {

	namespace Systems {

		class CustomWindow : public System
		{
			static CustomWindow* Instance;

			//typedefs
			using PollEventFunction = std::function<void(void)>;
			using KeyDownFunction = std::function<void(Keys)>;
			using MouseDownFuntion = std::function<void(Buttons)>;
			using WheelScrollFunction = std::function<void(Wheels, float)>;
			using OnCloseFunction = std::function<void(void)>;


		public:
			CustomWindow();
			CustomWindow(int _width, int _height, std::string _caption);
			~CustomWindow();

			void Init();
			void ShutDown();
			void Update(float dt);
			void SendMsg(EntityPointer firstEntity, EntityPointer secondEntity, Message::Message message);

			//poll any and all events and send to poll input
			void PollEvents();			
			//sent here by poll input via switch statement
			void PollWindowEvent();
			void PollKeyboardEvent();
			void PollMouseEvent();
			void setContextSettings();
			void NewFrame();

			
			glm::vec2 GetMousePosition();

			void Subscribe(KeyDownFunction callback) {
				KeyDownFunctionSubscribers.push_back(callback);
			}
			void Subscribe(MouseDownFuntion callback) {
				MouseDownFunctionSubscribers.push_back(callback);
			}
			void Subscribe(PollEventFunction callback) {
				EventFunctionSubscribers.push_back(callback);
			}
			void Subscribe(WheelScrollFunction callback) {
				WheelScrollFunctionSubscribers.push_back(callback);
			}


			static void setInstance(CustomWindow* inst) {
				Instance = inst;
			}

			static CustomWindow* getInstance() {
				return Instance;
			}

			void HideMouseCursor();

			void preventMouse() {
				int maxX = OpenGLContext->getSize().x;
				int maxY = OpenGLContext->getSize().y;

				int mX = sf::Mouse::getPosition(*OpenGLContext).x;
				int mY = sf::Mouse::getPosition(*OpenGLContext).y;

				if (mX < 0 || mY < 0 || mX > maxX || mY > maxY) {
					if (mX < 0)
						mX = 0;
					else if (mX > maxX)
						mX = maxX;

					if (mY < 0)
						mY = 0;
					else if (mY > maxY)
						mY = maxY;

					sf::Mouse::setPosition(sf::Vector2i(mX, mY), *OpenGLContext);
				}
			}

			int Width;
			int Height;
			OnCloseFunction onClose;
		private:
			std::unique_ptr<sf::Window> OpenGLContext;
			sf::ContextSettings contextSettings;
			sf::Event currentEvent;
			std::string caption;

			//this is an array of function calls
			std::vector<KeyDownFunction> KeyDownFunctionSubscribers;
			std::vector<PollEventFunction> EventFunctionSubscribers;
			std::vector<MouseDownFuntion> MouseDownFunctionSubscribers;
			std::vector<WheelScrollFunction> WheelScrollFunctionSubscribers;


			void CleanSubscribers(Keys key) {
				for (auto& func : KeyDownFunctionSubscribers) {
					func(key);
				}
			}

			void CleanSubscribers(Buttons button) {
				for (auto& func : MouseDownFunctionSubscribers) {
					func(button);
				}
			}

			void CleanSubscribers(Wheels wheel) {
				for (auto& func : WheelScrollFunctionSubscribers) {
					func(wheel, currentEvent.mouseWheelScroll.delta);
				}
			}
		};
		using CustomWindowPointer = std::shared_ptr<CustomWindow>;
	}
}

#endif // !WINDOWSFML_H





