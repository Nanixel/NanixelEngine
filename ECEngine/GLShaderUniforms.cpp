#include "stdafx.h"
#include "./GLShaderUniforms.h"
#include <glm/gtc/type_ptr.hpp>

void LoadShaderData(const GLint location, const GLint &data) {
	glUniform1i(location, data);
}

void LoadShaderData(const GLint location, const GLuint &data) {
	glUniform1ui(location, data);
}

void LoadShaderData(const GLint location, const GLfloat &data) {
	glUniform1f(location, data);
}

void LoadShaderData(const GLint location, const glm::vec2 &data) {
	glUniform2fv(location, 1, glm::value_ptr(data));
}

void LoadShaderData(const GLint location, const glm::vec3 &data) {
	glUniform3fv(location, 1, glm::value_ptr(data));
}

void LoadShaderData(const GLint location, const glm::vec4 &data) {
	glUniform4fv(location, 1, glm::value_ptr(data));
}

void LoadShaderData(const GLint location, const glm::mat3x3 &data) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void LoadShaderData(const GLint location, const glm::mat4x4 &data) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}
