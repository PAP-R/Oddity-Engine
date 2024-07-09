#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <map>
#include <string>
#include <unordered_map>

template<typename T>
class Trie : public std::unordered_map<std::string, std::vector<T>> {
protected:
    static auto chain_keys() {
        return std::string();
    }

    template<typename S, typename ... Args>
    static auto chain_keys(S key, Args ... keyChain) {
        std::string temp;
        temp += key;
        temp += chain_keys(keyChain...);
        return temp;
    }
public:
    void add(const std::string& key, T value) {
        (*this)[key].emplace_back(value);
    }

    using std::unordered_map<std::string, std::vector<T>>::operator[];

    template<typename ... Args>
    auto operator() (Args ... keyChain) {
        auto temp = chain_keys(keyChain...);
        return std::unordered_map<std::string, std::vector<T>>::operator[](temp);
    }
};

#endif //TRIE_H
