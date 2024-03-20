#ifndef COMMANDER_H
#define COMMANDER_H

#include <Util/Vector.h>
#include <functional>
#include <Util/Trie.h>

namespace OddityEngine::Util {
    class Commander {
    protected:
        Trie<std::function<Vector<float>(Vector<float>)>> command_tree;

        Vector<float> apply(std::string* command);

    public:
        Vector<float> apply(std::string command);

        void add_command(const std::string& command, const std::function<Vector<float>(Vector<float>)>& function);
    };
}

#endif //COMMANDER_H
