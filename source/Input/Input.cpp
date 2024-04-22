#include "Input.h"

namespace OddityEngine::Input {
    void Input::event(const SDL_Event &event) {
        if (active) {
            std::vector<size_t> *functions = nullptr;
            switch (event.type) {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    if (!event.key.repeat) functions = keymap.get(event.type, event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    functions = keymap.get(event.type, event.button.button);
                    break;
                case SDL_JOYAXISMOTION:
                    functions = keymap.get(event.type, event.jaxis.axis);
                    break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    functions = keymap.get(event.type, event.jbutton.button);
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    functions = keymap.get(event.type, event.caxis.axis);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    functions = keymap.get(event.type, event.cbutton.button);
                    break;
                default:
                    functions = keymap.get(event.type);
                    break;
            }

            if (functions != nullptr) {
                for (auto &f: *functions) {
                    actions[f].function(event);
                }
            }
        }

        active = next_active;
    }

    void Input::add_action(const std::string &name, const std::function<void(const SDL_Event &)> &function) {
        action_names.add(name, actions.size());
        actions.emplace_back(name, function);
    }

    void Input::enable() {
        next_active = true;
    }

    void Input::disable() {
        next_active = false;
    }

    bool Input::swap_active() {
        next_active = !next_active;
        return next_active;
    }
}