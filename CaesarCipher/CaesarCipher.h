//
// Created by igor- on 29.10.2021.
//

#ifndef CEASARCIPHER_CAESARCIPHER_H
#define CEASARCIPHER_CAESARCIPHER_H

#include <string>
#include <string_view>

namespace caesar_cipher {

    enum class AlphabetType {
        ENGLISH_FULL,
        ENGLISH_LOWER,
        ENGLISH_UPPER,
        RUSSIAN_FULL,
        RUSSIAN_LOWER,
        RUSSIAN_UPPER
    };

    class CaesarCipher {
    public:

        CaesarCipher() = default;

        CaesarCipher(size_t num_chars, char first_char)
            : num_chars_(num_chars), first_char_(first_char) {}

        CaesarCipher(AlphabetType type);

        std::string crypt(std::string_view source, size_t k, std::string_view key) const;
        std::string decrypt(std::string_view source, size_t k, std::string_view keyword) const;

        void SetNumChars(size_t num) {
            num_chars_ = num;
        }
        void SetFirstChar(char c) {
            first_char_ = c;
        }
    private:
        std::string GetCryptAlphabet(size_t k, std::string_view keyword) const;

        size_t num_chars_ = 'Z' - 'A' + 1;
        char first_char_ = 'A';
    };
} //namespace caesar_cipher

#endif //CEASARCIPHER_CAESARCIPHER_H
