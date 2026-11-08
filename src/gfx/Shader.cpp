#include "gfx/Shader.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace {

std::string readFile(const std::string& path) {
    std::ifstream file(std::filesystem::u8path(path));
    if (!file.is_open()) {
        std::cerr << "Cannot open shader file: " << path << std::endl;
        return {};
    }

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

unsigned int compileShader(unsigned int type, const std::string& source) {
    const unsigned int shader = glCreateShader(type);
    const char* code = source.c_str();

    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        std::cerr << "Shader compilation failed: " << log << std::endl;
    }

    return shader;
}

}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, readFile(vertexPath));
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, readFile(fragmentPath));

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        char log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

void Shader::use() const {
    glUseProgram(program);
}

unsigned int Shader::id() const {
    return program;
}

bool Shader::isLoaded() const {
    return program != 0;
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value ? 1 : 0);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
}
