#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#include <Util/Trie.h>
#include "Util/Eventable.h"
#include <functional>

#include <Util/Debug.h>

#define CONTROLLER_AXIS_MAX 32767.5f

namespace OddityEngine::Input {
    struct Action {
        std::string name;
        std::function<void(const SDL_Event& event)> function;
        std::vector<std::pair<size_t, size_t>> mapping;
    };

    class Input : public Util::Eventable {
    protected:
        Util::Trie<size_t> keymap;
        Util::Trie<size_t> action_names;
        std::vector<Action> actions;
        bool active = true;
        bool next_active = true;

    public:
        void event(const SDL_Event& event) override;
        void add_action(const std::string& name, const std::function<void(const SDL_Event& event)>& function);

        template<typename ... Args, std::enable_if_t<(std::is_scalar_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...), bool> = true>
        void add_mapping(const std::string& name, Args ... path) {
            auto actions = action_names.get(name);
            if (actions != nullptr) {
                for (auto a: *actions) {
                    keymap.add(a, path...);
                }
            }
            else {
                Debug::message("Action {} not found", name);
            }
        }

        void enable();
        void disable();
        bool swap_active();
    };
}

#endif //INPUT_H
