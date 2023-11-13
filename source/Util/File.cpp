#include "File.h"

namespace OddityEngine::Util::File {std::stringstream stream(const std::string& path) {
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

    std::string string(const std::string& path) {
        return stream(path).str();
    }
}