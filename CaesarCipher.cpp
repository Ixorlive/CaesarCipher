//
// Created by igor- on 29.10.2021.
//
#include "CaesarCipher.h"
#include <algorithm>
#include <unordered_map>

using namespace caesar_cipher;

CaesarCipher::CaesarCipher(AlphabetType type)
{
    switch (type)
    {
    case AlphabetType::ENGLISH_FULL:
        num_chars_ = 'z' - 'A' + 1;
        first_char_ = 'A';
        break;
    case AlphabetType::ENGLISH_LOWER:
        num_chars_ = 'z' - 'a' + 1;
        first_char_ = 'a';
        break;
    case AlphabetType::ENGLISH_UPPER:
        num_chars_ = 'Z' - 'A' + 1;
        first_char_ = 'A';
        break;
    case AlphabetType::RUSSIAN_FULL:
        num_chars_ = '�' - '�' + 1;
        first_char_ = '�';
        break;
    case AlphabetType::RUSSIAN_LOWER:
        num_chars_ = '�' - '�' + 1;
        first_char_ = '�';
        break;
    case AlphabetType::RUSSIAN_UPPER:
        num_chars_ = '�' - '�' + 1;
        first_char_ = '�';
        break;
    default:
        break;
    }
}

std::string CaesarCipher::decrypt(std::string_view source, size_t k, std::string_view keyword) const
{
    std::string decrypt_str;
    std::string crypt_alphabet = GetCryptAlphabet(k, keyword);
    std::unordered_map<char, char> chars_map;
    auto i = first_char_;
    for (char c : crypt_alphabet) {
        chars_map[c] = i++;
    }
    for (char i : source) {
        if (i < first_char_ || i > first_char_ + (char)num_chars_) {
            decrypt_str.push_back(i);
            continue;
        }
        decrypt_str.push_back(chars_map[i]);
    }
    return decrypt_str;
}

std::string CaesarCipher::crypt(std::string_view source, size_t k, std::string_view key) const {
    std::string encrypted_str;
    std::string crypt_alphabet = GetCryptAlphabet(k, key);
    for (char i : source) {
        if (i < first_char_ || i > first_char_ + (char)num_chars_) {
            encrypted_str.push_back(i);
            continue;
        }
        auto pos = i - first_char_;
        encrypted_str.push_back(crypt_alphabet[pos]);
    }
    return encrypted_str;
}

std::string CaesarCipher::GetCryptAlphabet(size_t k, std::string_view key) const {
    std::string keyword;
    keyword.reserve(keyword.size());
    for (char i : key) {
        if (i < first_char_ || i > first_char_ + (char)num_chars_ || 
            keyword.find(i) != std::string::npos) {
            continue;
        }
        keyword.push_back(i);
    }
    std::string alphabet;
    alphabet.reserve(num_chars_);
    for (char i = first_char_; i < first_char_ + (char)num_chars_; ++i) {
        if (key.find(i) != std::string::npos) {
            continue;
        }
        alphabet.push_back(i);
    }
    std::string result(num_chars_, ' ');
    for (size_t i = k; i < k + keyword.size(); ++i) {
        result[i] = keyword[i - k];
    }
    int index = k + keyword.size();
    for (char i : alphabet) {
        result[index++] = i;
        if (index == result.size()) index = 0;
    }
    return result;
}


