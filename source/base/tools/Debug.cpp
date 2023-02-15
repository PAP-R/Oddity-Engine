#include <algorithm>
#include "Debug.h"

Debug debug;

void Debug::add_point(vec3 pos, string text) {
    debug.points.emplace_back(pos);
    debug.pointTexts.emplace_back(text);
}

vector<vec3> Debug::get_points() {
    return debug.points;
}

vector<string> Debug::get_point_texts() {
    return debug.pointTexts;
}

void Debug::add_text(string text) {
    debug.mainText.append(text);
}

string Debug::get_text() {
    return debug.mainText;
}

void Debug::clear_text() {
    debug.mainText.clear();
}
