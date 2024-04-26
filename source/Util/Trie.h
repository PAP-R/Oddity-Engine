#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <map>

namespace OddityEngine::Util {
    template<typename T>
    struct TrieNode {
        std::vector<T> values;
        std::map<size_t, TrieNode> nodes;
    };

    template<typename T>
    class Trie {
    protected:
        TrieNode<T> root;

        void _add(TrieNode<T>* current, const T& value) {
            current->values.push_back(value);
        }

        template<typename ... Args, std::enable_if_t<((std::is_scalar_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...)) || sizeof...(Args) == 0, bool> = true>
        void _add(TrieNode<T>* current, const T& value, size_t path, Args ... rest) {
            current = &current->nodes[path];

            this->_add(current, value, rest...);
        }

        std::vector<T>* _get(TrieNode<T>* current) {
            return &current->values;
        }

        template<typename ... Args, std::enable_if_t<((std::is_scalar_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...)) || sizeof...(Args) == 0, bool> = true>
        std::vector<T>* _get(TrieNode<T>* current, size_t path, Args ... rest) {
            if (!current->nodes.contains(path)) {
                return nullptr;
            }

            current = &current->nodes[path];

            return this->_get(current, rest...);
        }

        void _get_all(const TrieNode<T>& current, std::string path, std::vector<std::pair<std::string, std::vector<T>>>* result) {
            if (!current.values.empty()) {
                result->emplace_back(path, current.values);
            }

            for (auto c : current.nodes) {
                _get_all(c.second, path + static_cast<char>(c.first), result);
            }
        }

        void _get_all_paths(const TrieNode<T>& current, std::string path, std::vector<std::string>* result) {
            if (!current.values.empty()) {
                result->push_back(path);
            }

            for (auto c : current.nodes) {
                _get_all_paths(c.second, path + static_cast<char>(c.first), result);
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
                current = &current->nodes[c];
            }

            current->values.push_back(value);
        }

        template<typename ... Args, std::enable_if_t<(std::is_scalar_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...), bool> = true>
        void add(const T& value, Args ... path) {
            this->_add(&root, value, path...);
        }

        std::vector<T>* get(const std::string& path) {
            TrieNode<T>* current = &root;

            for (auto c : path) {
                if (!current->nodes.contains(c)) {
                    return nullptr;
                }

                current = &current->nodes[c];
            }

            return &current->values;
        }

        template<typename ... Args, std::enable_if_t<(std::is_scalar_v<Args> && ...) && !(std::is_floating_point_v<Args> && ...), bool> = true>
        std::vector<T>* get(Args ... path) {
            return this->_get(&root, path...);
        }

        std::vector<std::pair<std::string, std::vector<T>>> get_all() {
            std::vector<std::pair<std::string, std::vector<T>>> all;
            _get_all(root, "", &all);
            return all;
        }

        std::vector<std::string> get_all_paths() {
            std::vector<std::string> all;
            _get_all_paths(root, "", &all);
            return all;
        }
    };
}



#endif //TRIE_H
