#pragma once
#ifndef GLAD_GL_H
#define GLAD_GL_H
#include <glad/glad.h>
#endif 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


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
    // void SetUniformMat4(const std::string& name, const glm::mat4& matrix);


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

};
