#ifndef COMMANDER_H
#define COMMANDER_H

#include <Util/Vector.h>
#include <functional>
#include <Util/Trie.h>

namespace OddityEngine::Util {
    Vector<std::string> separate(std::string command, const std::string& token = " \t\n");
    std::string chop(std::string* str, const std::string& token = " \t\n");

    class Commander {
    protected:
        Trie<std::function<std::string(std::string*)>> command_tree;

    public:
        std::string apply(std::string* command);
        std::string apply(std::string command);

        void add_command(const std::string& command, const std::function<std::string(std::string*)>& function);
    };
}

#endif //COMMANDER_H
