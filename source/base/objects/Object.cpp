#include "Object.h"

Object::Object(const vec3 &pos, const vec3 &dir, const vec3 &scale) : pos(pos), dir(dir), scale(scale) {}

void Object::loop(float deltaSeconds) {}
