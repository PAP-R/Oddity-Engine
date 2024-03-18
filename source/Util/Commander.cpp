#include "Commander.h"

namespace OddityEngine::Util {
    void Commander::add_command(const std::string& command, const std::function<std::string(std::string)>& function) {
        CommandNode* current = &nodestart;
        for(auto c : command) {
            if (current->nodes.size() <= c) {
                size_t new_size = c + 1;
                current->nodes.resize(c + 1);
            }

            current = &current->nodes[c];
        }

        current->functions.push_back(function);
    }

    std::string Commander::apply(std::string command) {
        CommandNode* current = &nodestart;

        std::string error = "Command not found";

        const size_t whitespace = command.find(' ');
        std::string rest = command.substr(whitespace + 1);
        command = command.substr(0, whitespace);

        for(auto c : command) {
            if (current->nodes.size() <= c) {
                return error;
            }

            current = &current->nodes[c];
        }

        if (current->functions.empty()) {
            return error;
        }

        std::string result;

        for (const auto& f : current->functions) {
            result += f(rest);
        }

        return result;
    }
}
