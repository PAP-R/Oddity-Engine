#include "Commander.h"

#include <type_traits>
#include <Util/Vector.h>

namespace OddityEngine::Util {
    template<typename F, typename T, typename ... Args>
    std::invoke_result_t<F, Args> call(F f, Vector<T> rest, Args ... args) {
        if constexpr (rest.size() > 0) {
            return call_impl(f, rest.slice(0, rest.size() - 1), rest.back(), args...);
        }

        return f(args...);
    }

    Vector<float> Commander::apply(std::string* command) {
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

        Debug::message("{} | {}", current_command, *command);

        if ('0' <= current_command.front() && current_command.front() <= '9') {
            return std::stof(current_command);
        }

        auto command_list = command_tree.get(current_command);

        if (command_list.empty()) {
            Vector<float> result;

            for (auto c : current_command) {
                result.emplace_back(c);
            }

            return result;
        }

        Vector<float> args;

        while(!command->empty()) {
            auto temp = apply(command);
            for (auto v : temp) {
                args.push_back(v);
            }
        }

        Vector<float> result;

        for (auto c : command_list) {
            auto temp = c(args);
            for (auto v : temp) {
                result.push_back(v);
            }
        }

        return result;
    }

    Vector<float> Commander::apply(std::string command) {
        return apply(&command);
    }

    void Commander::add_command(const std::string& command, const std::function<Vector<float>(Vector<float>)>& function) {
        const size_t whitespace = command.find(' ');

        command_tree.add(command.substr(0, whitespace), function);
    }
}