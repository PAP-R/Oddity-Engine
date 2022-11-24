#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include "loadShader.h"

GLuint loadFileShaders(const char * vertex_file_path,const char * fragment_file_path) {
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
    if (vertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << vertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        vertexShaderStream.close();
    }
    else {
        printf("Failed to read vertex shader file\n");
        throw std::invalid_argument("Failed to read vertex shader file");
    }

    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << fragmentShaderStream.rdbuf();
        fragmentShaderCode = sstr.str();
        fragmentShaderStream.close();
    }
    else {
        printf("Failed to read fragment shader file\n");
        throw std::invalid_argument("Failed to read fragment shader file");
    }

    return loadShaders(vertexShaderCode, fragmentShaderCode);
}

GLuint loadShader(GLuint shader, const std::string& shaderCode) {
    GLint result = GL_FALSE;
    int infoLength = 0;

    char const * vertexSrcPtr = shaderCode.c_str();
    glShaderSource(shader, 1,  &vertexSrcPtr, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

    if (infoLength > 0) {
        std::vector<char> shaderError(infoLength + 1);
        glGetShaderInfoLog(shader, infoLength, nullptr, &shaderError[0]);
        printf("%d : %s\n%s", infoLength, &shaderError[0], shaderCode.c_str());
        throw std::runtime_error(&shaderError[0]);
    }

    return shader;
}

GLuint loadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vertexShader, vertexShaderCode);
    loadShader(fragmentShader, fragmentShaderCode);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint result = GL_FALSE;
    int infoLength = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength > 1) {
        std::vector<char> programError(infoLength + 1);
        glGetProgramInfoLog(program, infoLength, nullptr, &programError[0]);
        printf("%d : %s\nProgram\n", infoLength, &programError[0]);
        throw std::runtime_error(&programError[0]);
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}