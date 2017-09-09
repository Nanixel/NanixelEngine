#pragma once

#ifndef GLSHADERSUNIFORMS_H
#define GLSHADERSUNIFORMS_H

#include "./ShaderUtility.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

__declspec(dllexport) void LoadShaderData(const GLint location, const GLint &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const GLuint &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const GLfloat &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const glm::vec2 &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const glm::vec3 &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const glm::vec4 &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const glm::mat3x3 &data);
__declspec(dllexport) void LoadShaderData(const GLint location, const glm::mat4x4 &data);

#endif // !GLSHADERSUNIFORMS_H