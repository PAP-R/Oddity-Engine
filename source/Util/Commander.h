#ifndef TRIE_H
#define TRIE_H

#include <Util/Vector.h>
#include <functional>

namespace OddityEngine::Util {
    struct CommandNode;
    struct CommandNode {
        std::vector<std::function<std::string(std::string)>> functions;
        std::vector<CommandNode> nodes;
    };

    class Commander {
    protected:
        CommandNode nodestart;

    public:
        void add_command(const std::string& command, const std::function<std::string(std::string)>& function);

        std::string apply(std::string command);
    };
}

#endif //TRIE_H
