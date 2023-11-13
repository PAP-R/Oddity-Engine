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
    std::stringstream stream(const std::string& path);

    /**
     * Reads file as string
     * @param path file path
     * @return file contents as string
     */
    std::string string(const std::string& path);
}

#endif //ODDITYENGINE_FILE_H
