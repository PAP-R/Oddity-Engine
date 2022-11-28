#include "loadTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath) {
    unsigned char header[124];

    FILE *fp;

    fp = fopen(imagepath, "rb");
    if (fp == NULL) {
        throw std::invalid_argument("Could not open dds file");
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