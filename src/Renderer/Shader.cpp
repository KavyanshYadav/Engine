#include<glad/glad.h>
#include "Shader.h"
#include "Utiles.h"

Shader::Shader() : programID(0) {}

Shader::Shader(const char* vertexPath, const char* fragmentPath):vertexShaderPath(vertexPath),fragmentShaderPath(fragmentPath)
{
	this->veretxShaderRaw = ReadShaderSource(vertexPath);
	this->fragmentShaderRaw = ReadShaderSource(fragmentPath);
    
}

GLuint Shader::CompileShader(const char* shaderSource, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error: " << infoLog << std::endl;
        ShowErrorDialog("Shader Compilation Error");
    }
    return shader;
}

Shader::~Shader() {
    std::cout << "Shader destructor called, cleaning up..." << std::endl;
}

GLuint Shader::CreateShaderProgram() {
    this->veretxShader = CompileShader(this->veretxShaderRaw.c_str(),GL_VERTEX_SHADER);
    this->fragmentShader = CompileShader(this->fragmentShaderRaw.c_str(), GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, this->veretxShader);
    glAttachShader(shaderProgram, this->fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Error: " << infoLog << std::endl;
    }

    //glDeleteShader(this->veretxShader);
    //glDeleteShader(this->fragmentShader);
    this->shaderProgram = shaderProgram;
    return shaderProgram;

}

GLint Shader::GetUniformLocation(const std::string& name) {
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader!" << std::endl;
    }
    return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

// Float Uniform
void Shader::SetUniform1f(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

// Vector3 Uniform
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

// Vector4 Uniform
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

// Matrix4 Uniform
// void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
//     glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
// }



void Shader::Use () {
    glUseProgram(shaderProgram);

}

std::string Shader::ReadShaderSource(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (file) {
        buffer << file.rdbuf();
        file.close();
    }
    else {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
		ShowErrorDialog("Failed to open shader file");
    }

    return buffer.str();

}