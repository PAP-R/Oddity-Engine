#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#include <Util/Trie.h>
#include "Util/Eventable.h"
#include <functional>

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

    public:
        void event(const SDL_Event& event) override;
        void add_action(const std::string& name, const std::function<void(const SDL_Event& event)>& function);
        void add_mapping(std::string name, size_t path...);
    };
}

#endif //INPUT_H
