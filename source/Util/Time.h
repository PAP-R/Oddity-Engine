#ifndef TIME_H
#define TIME_H

namespace OddityEngine::Util::Time {
    void update();
    float now();
    float start();
    float delta();
    size_t frame();
    float fps();
}

#endif //TIME_H
