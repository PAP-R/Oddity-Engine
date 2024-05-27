#ifndef ODDITYENGINE_FILE_H
#define ODDITYENGINE_FILE_H

#include <sstream>
#include <string>

#include <fstream>
#include <iostream>

#include "fmt/core.h"

#include "Vector.h"

namespace OddityEngine::Util::File {
    /**
     * Reads file as stringstream
     * @param path file path
     * @return file contents as stringstream
     */
    std::stringstream stream(const std::string& path);

    /**
     * Reads file as string
     * @param path file path
     * @return file contents as string
     */
    std::string string(const std::string& path);

    template<typename T>
    std::vector<T> read(const std::string& path) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        Debug::assert_error(!file.is_open(), "Failed to open file {}", path);

        size_t fileSize = file.tellg();
        std::vector<char> result(fileSize);

        file.seekg(0);
        file.read(result.data(), fileSize);

        file.close();
        return std::vector<T>(result.begin(), result.end());
    }

    void write(const std::string& path, const std::string& data);

    Vector<std::string> csv(const std::string& path);
}

#endif //ODDITYENGINE_FILE_H
