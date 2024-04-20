#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>

namespace OddityEngine::Util {
    template<typename T>
    struct TrieNode {
        std::vector<T> values;
        std::vector<TrieNode> nodes;
    };

    template<typename T>
    class Trie {
    protected:
        TrieNode<T> root;

        void _add(TrieNode<T>* current, const T& value) {
            current->values.push_back(value);
        }

        template<typename ... Args, std::enable_if_t<((std::is_arithmetic_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...)) || sizeof...(Args) == 0, bool> = true>
        void _add(TrieNode<T>* current, const T& value, size_t path, Args ... rest) {
            if (current->nodes.size() <= path) {
                current->nodes.resize(path + 1);
            }

            current = &current->nodes[path];

            this->_add(current, value, rest...);
        }

        std::vector<T>* _get(TrieNode<T>* current) {
            return &current->values;
        }

        template<typename ... Args, std::enable_if_t<((std::is_arithmetic_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...)) || sizeof...(Args) == 0, bool> = true>
        std::vector<T>* _get(TrieNode<T>* current, size_t path, Args ... rest) {
            if (current->nodes.size() > path) {
                current = &current->nodes[path];

                return this->_get(current, rest...);
            }
            return nullptr;
        }

        void _get_all(const TrieNode<T>& current, std::string path, std::vector<std::pair<std::string, std::vector<T>*>>* result) {
            if (!current->values.empty()) {
                result->emplace_back(path, &current.values);
            }

            for (size_t i = 0; i < current.nodes.size(); i++) {
                _get_all(current.nodes, path + std::to_string(i));
            }
        }

    public:
        void clear() {
            root.nodes.clear();
            root.values.clear();
        }

        void add(const std::string& path, const T& value) {
            TrieNode<T>* current = &root;

            for (auto c : path) {
                if (current->nodes.size() <= c) {
                    current->nodes.resize(c + 1);
                }

                current = &current->nodes[c];
            }

            current->values.push_back(value);
        }

        template<typename ... Args, std::enable_if_t<(std::is_arithmetic_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...), bool> = true>
        void add(const T& value, Args ... path) {
            this->_add(&root, value, path...);
        }

        std::vector<T>* get(const std::string& path) {
            TrieNode<T>* current = &root;

            for (auto c : path) {
                if (current->nodes.size() <= c) {
                    return nullptr;
                }

                current = &current->nodes[c];
            }

            return &current->values;
        }

        template<typename ... Args, std::enable_if_t<(std::is_arithmetic_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...), bool> = true>
        std::vector<T>* get(Args ... path) {
            return this->_get(&root, path...);
        }

        std::vector<std::pair<std::string, std::vector<T>*>> get_all() {
            std::vector<std::pair<std::string, std::vector<T>*>> all;
            _get_all(root, "", &all);
            return all;
        }
    };
}



#endif //TRIE_H
