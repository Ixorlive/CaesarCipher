#pragma once
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "crack.h"

using namespace caesar_cipher;

CrackCipherSystem::Words SplitIntoWordsView(std::string_view text);

void toLower(std::string& str);

CrackCipherSystem::StringFreqs CountGrams(std::string& text, int len);

std::string GetLowerTextFromFile(std::istream& in);
