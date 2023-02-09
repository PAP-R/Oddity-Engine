#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include <glm/glm.hpp>

using namespace glm;

#include <vector>
#include <string>
//#include <format>

using namespace std;



class Debug {
private:
    vector<vec3> points;
    vector<string> pointTexts;

public:
    static void add_point(vec3 pos, string text);
    static vector<vec3> get_points();
    static vector<string> get_point_texts();
};


#endif //ODDITYENGINE_DEBUG_H
