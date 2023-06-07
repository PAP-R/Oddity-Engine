#ifndef ROBOLATOR_ODDITYENGINE_H
#define ROBOLATOR_ODDITYENGINE_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <chrono>

#include "base/Camera.h"
#include "base/Control.h"
#include "base/Tracer.h"

class OddityEngine {
private:
    Camera* camera;
    GLFWwindow* window;

    chrono::time_point<chrono::system_clock> tStart;
    chrono::time_point<chrono::system_clock> tLast;
    chrono::time_point<chrono::system_clock> tNow;

public:
    Tracer* tracer;

    OddityEngine();
    ~OddityEngine();

    bool update();

    float get_time();

    GLFWwindow* get_window();
};

#endif //ROBOLATOR_ODDITYENGINE_H
