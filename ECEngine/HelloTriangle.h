#pragma once
#include <cstdlib>
#include <string>
#include "CustomWindow.h"
#include "Shader.h"
#include "HelloSqaure.h"



//////////EBO STUFF
//float twoTriangleVerticies[] = {
//	//first triangle
//	-1.0f, -0.5f, 0.0f,
//	-0.5f, 0.5f, 0.0f,
//	0.0f, -0.5f, 0.0f,
//
//	//second triangle
//	0.0f, -0.5f, 0.0f,
//	0.5f, 0.5f, 0.0f,
//	1.0f, -0.5f, 0.0f
//};
//
//
//unsigned int indicies[] = {
//	0,1,3, //first triangle
//	1,2,3 //second triangle
//};



//this goes into the VBO Memory
float triangleTwo[] = {
	//positions        //colors
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

float texCoords[] = {
	0.0f, 0.0f, //lower-left corner
	1.0f, 0.0f, //lower-right corner
	0.0f, 0.5f //tip of triangle
};

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

//sf::Clock testClock;


void drawHelloTriangle(Shader* shader) {
	
	float timeValue = testClock.getElapsedTime().asSeconds();

	//float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
	//int vertexColorLocation = glGetUniformLocation(secondShaderProgram, "ourColor");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	float offset = std::sin(timeValue) / 2.0f;
	//draw triangle
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	shader->use();
	shader->setFloat("xOffset", offset);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void InitShaders() {	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

//SECTION NOTES:
//Array buffer -> bind several buffers as long as they all have a different buffer type
//its 3 * sizeof(float) because each vertex is define by 3 floating point numbers
//last arg = offset: since position data starts at begining of verticies, offset is 0
void initOther() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	//Triangle setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
	//POSITION -> six floats exist in VBO -> stride all 6 to get to the next position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//COLOR -> location 1 -> stride 6 to get the next color, but with an offset of 3 (first 3 floats are postion)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

}
