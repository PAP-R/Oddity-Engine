#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <map>
#include <string>
#include <unordered_map>

template<typename T>
class Trie : public std::unordered_map<std::string, std::vector<T>> {
protected:
    /**
     * End state of chain_keys
     * @return en empty string
     */
    static auto chain_keys() {
        return std::string();
    }

    /**
     * Chains multiple values into a string
     * @tparam S type of the first element
     * @tparam Args types of the rest
     * @param key
     * @param keyChain
     * @return a string out of the keyChain elements
     */
    template<typename ... Args, typename S>
    static auto chain_keys(S key, Args ... keyChain) {
        std::string temp;
        temp += key;
        temp += chain_keys(keyChain...);
        return temp;
    }
public:
    /**
     * Adds an element
     * @param key where it's stored
     * @param value what is stored
     */
    void add(const std::string& key, T value) {
        (*this)[key].emplace_back(value);
    }

    using std::unordered_map<std::string, std::vector<T>>::operator[];

    /**
     * Basic access operator with multiple key elements
     * @tparam Args key element types
     * @param keyChain key elements
     * @return corresponding Values
     */
    template<typename ... Args>
    auto operator() (Args ... keyChain) {
        auto temp = chain_keys(keyChain...);
        return std::unordered_map<std::string, std::vector<T>>::operator[](temp);
    }
};

#endif //TRIE_H
