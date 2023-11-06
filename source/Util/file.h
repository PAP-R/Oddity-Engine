#ifndef ODDITYENGINE_FILE_H
#define ODDITYENGINE_FILE_H

#include <sstream>
#include <string>

#include <fstream>

#include "fmt/core.h"

namespace OddityEngine::Util::File {
    /**
     * Reads file as stringstream
     * @param path file path
     * @return file contents as stringstream
     */
    std::stringstream stream(const std::string& path) {
        std::stringstream string_stream;

        std::ifstream file_stream(path, std::ios::in);
        if (file_stream.is_open()) {
            string_stream << file_stream.rdbuf();
            file_stream.close();
        }
        else {
            throw std::runtime_error(fmt::format("Failed to read File [{}]", path));
        }

        return string_stream;
    }

    /**
     * Reads file as string
     * @param path file path
     * @return file contents as string
     */
    std::string string(const std::string& path) {
        return stream(path).str();
    }
}

#endif //ODDITYENGINE_FILE_H
