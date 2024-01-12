#include "File.h"

#include <algorithm>

namespace OddityEngine::Util::File {
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

    std::string string(const std::string& path) {
        return stream(path).str();
    }

    void write(const std::string& path, const std::string& data) {
        std::ofstream file(path, std::ios_base::out | std::ios_base::trunc);

        std::cout << "Writing:\n" << data;

        file << data << std::endl;

        file.close();
    }

    std::vector<std::string> csv(const std::string& path) {
        auto file = stream(path);

        std::vector<std::string> result(0);

        std::string line;

        while(std::getline(file, line, ',')) {
            std::erase_if(line, [](char c){ return c == ' ' || c == '\n'; });
            if (!line.empty()) result.push_back(line);
        }

        return result;
    }
}
