#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <chrono>

using namespace std;

#include "source/base/objects/Graphics.h"
#include "source/base/objects/Camera.h"
#include "interfaces/CallBack.h"

typedef enum {
    SCREEN_MODE_WINDOW,
    SCREEN_MODE_BORDERLESS,
    SCREEN_MODE_FULLSCREEN
} screenMode;

class Window {
private:
    GLFWwindow *window;
    vec2 position;
    vec2 size;

    float runtimeSeconds = 0.0f;

    vector<CallBack*> callbackList;

public:
    const vector<CallBack*> &getCallbackList() const;

private:
    GLenum cursormode;
    const GLFWvidmode* windowMode;

    vector<Graphics*> objects;

    Camera *camera;

public:
    void addCallback(CallBack *callback);

    void removeCallback(CallBack *callback);

    void setKeyCallback();

    void setCursorCallback();

    void setMouseButtonCallback();

    void setScrollCallback();

    void setCallbacks();


/// Toggles ///
    void setCursor(int mode);

    void setResolution(size_t width, size_t height);

    void setWindowPos(size_t x, size_t y);

    void setWindowMode(int mode);


/// Window ///
    Window(const char *name, size_t width, size_t height, int x=0, int y=0);

    virtual ~Window();

    void addObject(Graphics* go);

    void removeObject(Graphics* go);

    void setCamera(Camera *cam);

    Camera * getCamera();

    bool loop(float deltaSeconds);
};


#endif //ODDITYENGINE_WINDOW_H
