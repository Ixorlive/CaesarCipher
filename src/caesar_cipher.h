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

struct AlphabetInfo {
  AlphabetType type = AlphabetType::ENGLISH_LOWER;
  char first_char = 'a';
  char last_char = 'z';
  size_t num_chars = 'a' - 'z' + 1;
  AlphabetInfo() = default;

  AlphabetInfo(AlphabetType al_type) {
    SetAlphabet(al_type);
  }

  void SetAlphabet(AlphabetType al_type) {
    type = al_type;
    switch (al_type) {
      case AlphabetType::ENGLISH_FULL:num_chars = 'z' - 'A' + 1;
        first_char = 'A';
        last_char = 'z';
        break;
      case AlphabetType::ENGLISH_LOWER:num_chars = 'z' - 'a' + 1;
        first_char = 'a';
        last_char = 'z';
        break;
      case AlphabetType::ENGLISH_UPPER:num_chars = 'Z' - 'A' + 1;
        first_char = 'A';
        last_char = 'Z';
        break;
      default:break;
    }
  }
};

class CaesarCipher {
 public:

  CaesarCipher() = default;

  CaesarCipher(AlphabetType type);

  std::string Crypt(std::string_view source, size_t k, std::string_view key) const;
  std::string Decrypt(std::string_view source, size_t k, std::string_view keyword) const;

  void SetAlphabet(AlphabetType type) {
    alphabet_info_ = AlphabetType(type);
  }

  AlphabetType GetAlphabetType() const {
    return alphabet_info_.type;
  }
 private:
  std::string GetCryptAlphabet(size_t k, std::string_view keyword) const;

  AlphabetInfo alphabet_info_;
};
} //namespace CaesarCipher

#endif //CEASARCIPHER_CAESARCIPHER_H
