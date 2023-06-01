#include "Loader.h"

#include <vector>
#include <cstdio>
using namespace std;

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <fmt/core.h>

namespace Loader {
    Object obj(string path) {
        Object object;

        stringstream objectstr;
        string line;

        ifstream objectstream(path, ios::in);
        if (objectstream.is_open()) {
            objectstr << objectstream.rdbuf();
            objectstream.close();
        }
        else {
            printf("%s", fmt::format("Failed to read File {}\n", path).c_str());
        }

        float x, y, z, r, g, b;

        while (getline(objectstr, line, '\n')) {
            if (strcmp(line.substr(0, line.find(' ')).c_str(), "v") == 0) {
                string test = line.substr(line.find(' '));
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
                stringstream linestr;
                linestr << line.substr(line.find(' ') + 1) << ' ';
                string subline;

                int vi, vni, vti;

                while (getline(linestr, subline, ' ')) {
                    sscanf(subline.c_str(), "%d/%d/%d", &vi, &vni, &vti);
                    object.faces.emplace_back(vi, vni, vti);
                }
            }
        }

        return object;
    }
}