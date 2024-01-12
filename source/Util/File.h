#ifndef ODDITYENGINE_FILE_H
#define ODDITYENGINE_FILE_H

#include <sstream>
#include <string>

#include <fstream>
#include <iostream>

#include "fmt/core.h"

#include <vector>

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

    void write(const std::string& path, const std::string& data);

    std::vector<std::string> csv(const std::string& path);
}

#endif //ODDITYENGINE_FILE_H
