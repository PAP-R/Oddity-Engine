#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include "glm/glm.hpp"

using namespace glm;

#include <vector>
using namespace std;

#include <string>

class Debug {
private:
    vector<vec3> points;
    vector<string> pointTexts;
    std::string mainText;

public:
    static void add_point(vec3 pos, string text);
    static vector<vec3> get_points();
    static vector<string> get_point_texts();

    static void add_text(string text);
    static string get_text();
    static void clear_text();
};


#endif //ODDITYENGINE_DEBUG_H
