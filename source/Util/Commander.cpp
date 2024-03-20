#include "Commander.h"

#include <type_traits>
#include <Util/Vector.h>

namespace OddityEngine::Util {
    Vector<std::string> separate(std::string command, const std::string& token) {
        Vector<std::string> result;
        size_t whitespace;

        while ((whitespace = command.find_first_of(token)) != command.npos) {
            result.push_back(command.substr(0, whitespace));
            command = command.substr(whitespace + 1);
        }

        return result;
    }

    std::string chop(std::string* str, const std::string& token) {
        std::string result;

        size_t whitespace = str->find_first_of(token);

        if (whitespace != str->npos) {
            result = str->substr(0, whitespace);
            *str = str->substr(whitespace + 1);
        }
        else {
            result = *str;
            *str = "";
        }

        return result;
    }

    template<typename F, typename T, typename ... Args>
    std::invoke_result_t<F, Args> call(F f, Vector<T> rest, Args ... args) {
        if constexpr (rest.size() > 0) {
            return call_impl(f, rest.slice(0, rest.size() - 1), rest.back(), args...);
        }

        return f(args...);
    }

    std::string Commander::apply(std::string* command) {
        const size_t whitespace = command->find(' ');

        std::string current_command;

        if (whitespace != command->npos) {
            current_command = command->substr(0, whitespace);
            *command = command->substr(whitespace + 1);
        }
        else {
            current_command = *command;
            *command = "";
        }

        auto command_list = command_tree.get(current_command);

        if (command_list.empty()) {
            return "Command not found";
        }

        std::string result;

        for (auto c : command_list) {
            result += c(command);
        }

        return result;
    }

    std::string Commander::apply(std::string command) {
        return apply(&command);
    }

    void Commander::add_command(const std::string& command, const std::function<std::string(std::string*)>& function) {
        const size_t whitespace = command.find(' ');

        command_tree.add(command.substr(0, whitespace), function);
    }
}