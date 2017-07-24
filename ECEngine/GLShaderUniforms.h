#pragma once
#ifndef GLSHADERSUNIFORMS_H
#define GLSHADERSUNIFORMS_H

#include "./GLShader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

void LoadShaderData(const GLint location, const GLint &data);
void LoadShaderData(const GLint location, const GLuint &data);
void LoadShaderData(const GLint location, const GLfloat &data);
void LoadShaderData(const GLint location, const glm::vec2 &data);
void LoadShaderData(const GLint location, const glm::vec3 &data);
void LoadShaderData(const GLint location, const glm::vec4 &data);
void LoadShaderData(const GLint location, const glm::mat3x3 &data);
void LoadShaderData(const GLint location, const glm::mat4x4 &data);

#endif // !GLSHADERSUNIFORMS_H
