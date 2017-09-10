#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//initial settings
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldSpaceUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float MousePosDeltaX = sf::Mouse::getPosition().x;
	float MousePosDeltaY = sf::Mouse::getPosition().y;

	//Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldSpaceUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upZ, float upY, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldSpaceUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		//return glm::lookAt(Position, Position + Front, Up);
		return personalLookAt(Position, Position + Front, Up);
	}

	glm::mat4 personalLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up) {		
	/*	glm::mat4 lookat(glm::vec4(position, 0.0f), glm::vec4(up, 0.0f), glm::vec4(target, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		lookat *= glm::mat4 
		return lookat;*/

		glm::vec3 zaxis = glm::normalize(position - target);
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(Up), zaxis));
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		//create translation and rotation matrix
		glm::mat4 translation; //identity matrix by default
		translation[3][0] = -position.x;
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;

		glm::mat4 rotation;
		rotation[0][0] = xaxis.x;
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x;
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x;
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		return rotation * translation;

	}

	void ProcessKeyBoard(Camera_Movement direction, float dt) {
		//sfml stuff here

		bool isFPS = true;

		float velocity = MovementSpeed * dt;
		if (direction == Camera_Movement::LEFT) {
			Position -= Right * velocity;
		}
		if (direction == Camera_Movement::RIGHT) {
			Position += Right * velocity;
		}
		if (direction == Camera_Movement::FORWARD) {
			if (isFPS) {
				Position += glm::vec3(Front.x, 0.0f, Front.z) * velocity;
			}
			else {
				Position += Front * velocity;
			}
		}
		if (direction == Camera_Movement::BACKWARD) {
			if (isFPS) {
				Position -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;
			}
			else {
				Position -= Front * velocity;
			}
		}
	}

	void ProcessMouseInput(float xoffset, float yoffset, GLboolean constrianPitch = true) {
		//sfml mouse input
		/*xoffset = sf::Mouse::getPosition().x - lastX;;
		yoffset = lastY - sf::Mouse::getPosition().y;*/

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;		

		Yaw += xoffset;
		Pitch += yoffset;
		
		if (constrianPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}				

		updateCameraVectors();		
	}

	void ProcessScroll(float delta) {
		//FOV Clamping
		if (Zoom >= 1.0f && Zoom <= 45.0f) {
			//y offset is amount we scroll vertically
			Zoom -= delta;
		}
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	void constrainMouse(float width, float height) {
		uint32_t maxMousePosRadius = glm::min(width, height) / 3;
		glm::vec2 windowCenter(width / 2, height/ 2);

		//std::cout << mousePos.x << std::endl;
		/*" " << mousePos.y << std::endl;
		std::cout << "windowCenter " << windowCenter.x << " " << windowCenter.y << std::endl;
		std::cout << "Max mouse radius" << maxMousePosRadius << std::endl;*/
		glm::vec2 mousePos(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		//dont know whats really going on here
		if (glm::length(mousePos - windowCenter) > maxMousePosRadius || mousePos.y == 211) {
			//std::cout << "HERE "<< std::endl;
			sf::Mouse::setPosition(sf::Vector2i((int)windowCenter.x, (int)windowCenter.y));
			MousePosDeltaX = windowCenter.x;
			MousePosDeltaY = windowCenter.y;
		}
		else {
			MousePosDeltaX = sf::Mouse::getPosition().x;
			MousePosDeltaY = sf::Mouse::getPosition().y;
		}
	}

private:

	

	void updateCameraVectors() {
		glm::vec3 front;
		front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
		front.y = std::sin(glm::radians(Pitch));
		front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldSpaceUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif // !CAMERA_H
