#pragma once
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "crack.h"

using namespace caesar_cipher;

enum class TypeText {
	TO_LOWER,
	TO_UPPER,
	ALL,
	DEFAULT = TO_LOWER
};

CrackCipherSystem::Words SplitIntoWordsView(std::string_view text);

void toLower(std::string& str);

void toUpper(std::string& str);

CrackCipherSystem::StringFreqs CountGrams(std::string& text, int len);

std::string GetStringFromFile(std::istream& in, TypeText type = TypeText::TO_LOWER);
