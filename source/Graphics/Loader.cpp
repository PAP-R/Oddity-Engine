#include "Loader.h"

#include <Util/File.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Loader {
            Object obj(const std::string& path) {
                Object object;

                auto objectstream = Util::File::stream(path);
                std::string line;

                float x, y, z, r, g, b;

                while (std::getline(objectstream, line, '\n')) {
                    if (strcmp(line.substr(0, line.find(' ')).c_str(), "v") == 0) {
                        std::string test = line.substr(line.find(' '));
                        sscanf(line.substr(line.find(' ')).c_str(), "%f%f%f%f%f%f", &x, &y, &z, &r, &g, &b);
                        object.vertices.emplace_back(x, y, z);
                        object.colors.emplace_back(r, g, b);
                    } else if (strcmp(line.substr(0, line.find(' ')).c_str(), "vn") == 0) {
                        sscanf(line.substr(line.find(' ')).c_str(), "%f%f%f", &x, &y, &z);
                        object.normals.emplace_back(x, y, z);
                    } else if (strcmp(line.substr(0, line.find(' ')).c_str(), "vt") == 0) {
                        sscanf(line.substr(line.find(' ')).c_str(), "%f%f", &x, &y);
                        object.uvs.emplace_back(x, y);
                    } else if (strcmp(line.substr(0, line.find(' ')).c_str(), "f") == 0) {
                        size_t o = 0;
                        std::stringstream linestr;
                        linestr << line.substr(line.find(' ') + 1) << ' ';
                        std::string subline;

                        int vi, vni, vti;

                        while (getline(linestr, subline, ' ')) {
                            sscanf(subline.c_str(), "%d/%d/%d", &vi, &vni, &vti);
                            object.faces.emplace_back(vi, vni, vti);
                        }
                    }
                }

                return object;
            }


        } // Loader
    } // OddityEngine
} // Graphics