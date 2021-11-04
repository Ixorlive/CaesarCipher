#include "string_processing.h"

CrackCipherSystem::Words SplitIntoWordsView(std::string_view text)
{
    CrackCipherSystem::Words words;
    auto temp = text;
    int start_pos{ 0 };
    int len_word{ 0 };
    for (size_t i = 0; i < temp.size(); ++i) {
        if (temp[i] < 'a' || temp[i] > 'z') {
            if (len_word > 0) {
                words.insert(temp.substr(start_pos, len_word));
                temp.remove_prefix(start_pos + len_word + 1);
                start_pos = 0;
                len_word = 0;
                i = -1;
            }
            else {
                ++start_pos;
            }
        }
        else {
            ++len_word;
        }
    }
    if (len_word > 0) {
        words.insert(temp.substr(start_pos, len_word));
    }
    return words;
}

void toLower(std::string& str) {
    for (char& i : str) {
        if (i >= 'A' && i <= 'Z') {
            i += 32;
        }
    }
}

CrackCipherSystem::StringFreqs CountGrams(std::string& text, int len)
{
    CrackCipherSystem::StringFreqs result;
    for (size_t i = 0; i + len - 1 < text.size(); ++i) {
        auto gram = text.substr(i, len);
        for (char c : gram) {
            if (c < 'a' || c > 'z') {
                gram = "";
                break;
            }
        }
        if (!gram.empty()) {
            ++result[gram];
        }
    }
    return result;
}

std::string GetLowerTextFromFile(std::istream& in)
{
    std::string result((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    toLower(result);
    return result;
}