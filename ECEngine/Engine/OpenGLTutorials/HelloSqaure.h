#pragma once
#include "../ECEngine/Engine/Core/Libraries/stb_image.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "Shader.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

//
//unsigned int VertexBufferObject;
//unsigned int VertexArrayObject;
//unsigned int ElementBufferObject;
//unsigned int texture1;
//unsigned int texture2;
//float SCREEN_W, SCREEN_H;
//sf::Clock testClock;
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//
//float squareVerticies[] = {
//	// positions          // colors           // texture coords
//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    //top left
//};
//
////Defining the graphical layout of a cube
//float vertices[] = {
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};
//
////for more cubes we just need to change each cubes model matrix!
//glm::vec3 cubePositions[] = {
//	glm::vec3(0.0f,  0.0f,  0.0f),
//	glm::vec3(2.0f,  5.0f, -15.0f),
//	glm::vec3(-1.5f, -2.2f, -2.5f),
//	glm::vec3(-3.8f, -2.0f, -12.3f),
//	glm::vec3(2.4f, -0.4f, -3.5f),
//	glm::vec3(-1.7f,  3.0f, -7.5f),
//	glm::vec3(1.3f, -2.0f, -2.5f),
//	glm::vec3(1.5f,  2.0f, -2.5f),
//	glm::vec3(1.5f,  0.2f, -1.5f),
//	glm::vec3(-1.3f,  1.0f, -1.5f)
//};
//
//unsigned int indicies[] = {
//	0, 1, 3, //first triangle
//	1, 2, 3, //second triangle
//};
//
//float deltaTime = 0.0f; //Time between current frame and last frame
//float lastFrame = 0.0f; //Time of last frame
//
//void initBuffers() {
//
//	glEnable(GL_DEPTH_TEST);
//
//	glGenVertexArrays(1, &VertexArrayObject);
//	glGenBuffers(1, &VertexBufferObject);
//	//glGenBuffers(1, &ElementBufferObject);
//
//	//Triangle setup
//	glBindVertexArray(VertexArrayObject);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
//
//	//POSITION -> six floats exist in VBO -> stride all 6 to get to the next position
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	//COLOR -> location 1 -> stride 6 to get the next color, but with an offset of 3 (first 3 floats are postion)
//	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	//glEnableVertexAttribArray(1);
//
//	//TEXTURES -> location 2
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1); //going to be the second index in the vertex attribute array
//}
//
//void realTimeKeys() {	
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//		camera.ProcessKeyBoard(Camera_Movement::LEFT, deltaTime);
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//		camera.ProcessKeyBoard(Camera_Movement::RIGHT, deltaTime);
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//		camera.ProcessKeyBoard(Camera_Movement::FORWARD, deltaTime);
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//		camera.ProcessKeyBoard(Camera_Movement::BACKWARD, deltaTime);
//	}	
//}
//
//void realTimeMouse() {
//	
//	float xOffset = sf::Mouse::getPosition().x - camera.MousePosDeltaX;
//	float yOffset = camera.MousePosDeltaY - sf::Mouse::getPosition().y;
//	
//	camera.ProcessMouseInput(xOffset, yOffset);
//	camera.constrainMouse(SCREEN_W, SCREEN_H);	
//}
//
//
////void onWheelScroll(Wheels wheel, float delta) {
////	switch (wheel) {
////	case Wheels::Vertical:
////		camera.ProcessScroll(delta);
////		break;
////	default:
////		break;
////	}
////}
//
//
//void drawHelloSquare(Shader* shader, float mixValue) {
//	
//	float currentTime = testClock.getElapsedTime().asSeconds();
//	deltaTime = currentTime - lastFrame;
//	lastFrame = currentTime;
//	realTimeKeys();
//	realTimeMouse();
//
//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture1);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//
//	shader->use();
//	shader->setFloat("mixValue", mixValue);	
//
//	//rotate args: matrix to transform, angle, rotation axis
//	//world coords conversion (aka multiply vertex coords with this model)
//	
//	glm::mat4 viewMatrix;
//	//perspective projection
//	glm::mat4 projectionMatrix;
//
//	
//	//modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//	//remember we are translating the scene (the camera never moves)
//	//note that we are translating the scene in the reverse direction of where we want to move (inverse)
//	//think of this view matrix as a camera object -> or at least the matrix that gives view coords when multiplied with local
//	viewMatrix = camera.GetViewMatrix();
//	//										FOV					sw				sh		near	far
//	projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCREEN_W / SCREEN_H, 0.1f, 100.0f);
//	
//
//	unsigned int viewLoc = glGetUniformLocation(shader->programID, "view");
//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
//
//	unsigned int projectionLoc = glGetUniformLocation(shader->programID, "projection");
//	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
//	
//	glBindVertexArray(VertexArrayObject);
//
//	for (unsigned int i = 0; i < 10; i++) {
//		glm::mat4 modelMatrix;
//		modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
//		//modelMatrix = glm::rotate(modelMatrix, currentTime, glm::vec3(1.0f, 0.3f, 0.0f));
//		unsigned int modelLoc = glGetUniformLocation(shader->programID, "model");
//		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//}
//
//void makeSqaure(Shader* shader) {
//	
//	initBuffers();
//
//	
//	//unsigned int texture1, texture2;
//	// texture 1
//	// ---------
//	glGenTextures(1, &texture1);
//	glBindTexture(GL_TEXTURE_2D, texture1);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	int width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//	// texture 2
//	// ---------
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//
//	shader->use();
//	shader->setInt("texture1", 0);
//	shader->setInt("texture2", 1);
//
//}

