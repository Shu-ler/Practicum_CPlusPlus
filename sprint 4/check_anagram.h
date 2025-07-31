#pragma once

#include <map>
#include <string>

inline std::map<char, size_t> CountChars(const std::string& word) {
    std::map<char, size_t> dictionary;

    for (const char letter : word) {
        dictionary[letter]++;
    }
    return dictionary;
}

inline bool CheckAnagram(const std::string& word1, const std::string& word2) {
    return CountChars(word1) == CountChars(word2);
}
