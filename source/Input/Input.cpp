#include "Input.h"

namespace OddityEngine::Input {
    void Input::event(const SDL_Event &event) {
        std::vector<size_t>* functions = nullptr;
        switch(event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                functions = keymap.get(event.type, event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                functions = keymap.get(event.type, event.button.button);
                break;
            case SDL_MOUSEMOTION:
                functions = keymap.get(event.type);
                break;
        }

        if (functions != nullptr) {
            for (auto& f : *functions) {
                actions[f].function(event);
            }
        }
    }

    void Input::add_action(const std::string &name, const std::function<void(const SDL_Event &)> &function) {
        action_names.add(name, actions.size());
        actions.emplace_back(name, function);
    }

    void Input::add_mapping(std::string name, size_t path...) {
        for (auto a : *action_names.get(name)) {
            keymap.add(a, path);
        }
    }
}