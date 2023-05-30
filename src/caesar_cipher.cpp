//
// Created by igor- on 29.10.2021.
//
#include "caesar_cipher.h"
#include <algorithm>
#include <unordered_map>

using namespace caesar_cipher;

CaesarCipher::CaesarCipher(AlphabetType type) {
  alphabet_info_ = AlphabetInfo(type);
}

std::string CaesarCipher::Decrypt(std::string_view source, size_t k, std::string_view keyword) const {
  std::string decrypt_str;
  std::string crypt_alphabet = GetCryptAlphabet(k, keyword);
  std::unordered_map<char, char> chars_map;
  auto alphabet_char = alphabet_info_.first_char;
  for (char c : crypt_alphabet) {
    chars_map[c] = alphabet_char++;
  }
  for (char i : source) {
    if (i < alphabet_info_.first_char || i > alphabet_info_.last_char) {
      decrypt_str.push_back(i);
      continue;
    }
    decrypt_str.push_back(chars_map[i]);
  }
  return decrypt_str;
}

std::string CaesarCipher::Crypt(std::string_view source, size_t k, std::string_view key) const {
  std::string encrypted_str;
  std::string crypt_alphabet = GetCryptAlphabet(k, key);
  for (char i : source) {
    if (i < alphabet_info_.first_char || i > alphabet_info_.last_char) {
      encrypted_str.push_back(i);
      continue;
    }
    auto pos = i - alphabet_info_.first_char;
    encrypted_str.push_back(crypt_alphabet[pos]);
  }
  return encrypted_str;
}

std::string CaesarCipher::GetCryptAlphabet(size_t k, std::string_view key) const {
  std::string keyword;
  keyword.reserve(keyword.size());
  for (char i : key) {
    if (i < alphabet_info_.first_char || i > alphabet_info_.last_char ||
        keyword.find(i) != std::string::npos) {
      continue;
    }
    keyword.push_back(i);
  }
  std::string alphabet;
  alphabet.reserve(alphabet_info_.num_chars);
  for (char i = alphabet_info_.first_char; i <= alphabet_info_.last_char; ++i) {
    if (key.find(i) != std::string::npos) {
      continue;
    }
    alphabet.push_back(i);
  }
  std::string result(alphabet_info_.num_chars, ' ');
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


