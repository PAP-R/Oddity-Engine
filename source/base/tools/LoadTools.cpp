#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

#include "LoadTools.h"

string loadShader(const char * path) {
    string shaderCode;

    std::ifstream shaderStream(path, std::ios::in);
    if (shaderStream.is_open()) {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else {
        printf("Failed to read shader file\n");
        throw std::invalid_argument("Failed to read shader file");
    }

    return shaderCode;
}

GLuint loadFileShaders(const char * vertex_file_path,const char * fragment_file_path) {
    std::string vertexShaderCode = loadShader(vertex_file_path);

    std::string fragmentShaderCode = loadShader(fragment_file_path);

    return loadShaders(vertexShaderCode, fragmentShaderCode);
}

GLuint compileShader(GLuint shader, const std::string& shaderCode) {
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

    compileShader(vertexShader, vertexShaderCode);
    compileShader(fragmentShader, fragmentShaderCode);

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

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath) {
    unsigned char header[124];

    FILE *fp;

    fp = fopen(imagepath, "rb");
    std::cerr << errno << std::endl;
    if (fp == NULL) {
        throw std::invalid_argument("Could not open dds file " + std::string(imagepath) + "\nThe working directory is " + std::filesystem::current_path().string());
    }

    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        throw std::runtime_error("File not of type dds");
    }

    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char * buffer;
    unsigned int bufsize;

    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*) calloc(bufsize, sizeof(unsigned char));

    fread(buffer, 1, bufsize, fp);
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            printf("DXT1\n");
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            printf("DXT3\n");
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            printf("DXT3\n");
            break;
        default:
            free(buffer);
            throw std::invalid_argument("DDS File not dxt compressed");
    }

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    unsigned int blocksize = format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
    unsigned int offset = 0;

    for (unsigned int level = 0; level < mipMapCount && (width || height); level++) {
        unsigned int size = ((width + 3) / 4) * ((height+3) / 4) * blocksize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }

    free(buffer);

    return texture;
}

GLuint loadBMP_custom(const char * imagepath) {
    unsigned char header[54];
    unsigned int datapos;
    unsigned int width, height;
    unsigned int imageSize;

    unsigned char * data;

    FILE * file = fopen(imagepath, "rb");
    if (!file) {
        throw std::runtime_error("Couldn't open Texture file");
    }

    if (fread(header, 1, 54, file) != 54) {
        throw std::runtime_error("Couldn't read texture header");
    }

    if (header[0] != 'B' || header[1] != 'M') {
        throw std::runtime_error("Not a bm file");
    }

    datapos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (imageSize == 0)
        imageSize = width * height * 3;
    if (datapos == 0)
        datapos = 54;

    data = new unsigned char [imageSize];

    fread(data, 1, imageSize, file);

    fclose(file);

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}