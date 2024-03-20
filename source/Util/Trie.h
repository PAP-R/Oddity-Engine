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

    public:
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

        std::vector<T> get(const std::string& path) {
            TrieNode<T>* current = &root;

            for (auto c : path) {
                if (current->nodes.size() <= c) {
                    break;
                }

                current = &current->nodes[c];
            }

            return current->values;
        }
    };
}



#endif //TRIE_H
