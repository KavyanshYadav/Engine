#pragma once
#ifndef GLAD_GL_H
#define GLAD_GL_H
#include <glad/glad.h>
#endif 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	GLuint CreateShaderProgram();
	void Use();
	void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
    void SetViewMatrix(const glm::mat4& matrix) { viewMatrix = matrix; }
    void SetProjectionMatrix(const glm::mat4& matrix) { projectionMatrix = matrix; }
    glm::mat4 GetViewMatrix() const { return viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return projectionMatrix; }

private:
	std::string veretxShaderRaw;
	std::string fragmentShaderRaw;
	GLuint veretxShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLuint programID;  // Shader program ID
	std::string ReadShaderSource(const char* filePath);
	GLint GetUniformLocation(const std::string& name);
	GLuint CompileShader(const char* shaderSource, GLenum shaderType);
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};
