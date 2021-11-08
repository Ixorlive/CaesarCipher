#include "crack.h"
#include "string_processing.h"

using namespace caesar_cipher;

CrackCipherSystem::CrackCipherSystem(std::filesystem::path path)
    : dict_path_(path)
{
    std::ifstream dict(dict_path_);

    if (!dict.is_open()) {
        throw std::logic_error("Dictionary not founded");
    }
    dict_text_ = GetStringFromFile(dict, TypeText::TO_LOWER);

    dictionary_words_ = SplitIntoWordsView(dict_text_);
}

std::string CrackCipherSystem::crack(std::filesystem::path path_to_file)
{
    std::ifstream source(path_to_file);
    if (!source.is_open()) {
        throw std::logic_error("File not founded");
    }
    crypt_text_ = GetStringFromFile(source, TypeText::TO_LOWER);
    decrypt_text_ = crypt_text_;

    source_words_ = SplitIntoWordsView(decrypt_text_);
    // We find the most frequent letters in the dictionary and in ciphertext and set converting_letters_
    // For example: Most freqs 
    //  Dict   Crypt
    //    e ->  t   : converting_letters_['t'] = 'e'
    //    t ->  z   : converting_letters_['z'] = 't'
    //    etc
    FreqsAnalysis();
    double perc = GetPercRecognizedWords();
    // If the percentage of recognized words is small, 
    // we try to analyze the most frequent digram in the dictionary and in the source
    if (perc <= 0.05) {
        NGramAnalysis(1, 2, 2, 2);
    }
    // Like FreqsAnalysis() but we analyze the digrams, trigrams and four-gram (google n-grams)
    NGramAnalysis(5);
    double new_perc = GetPercRecognizedWords();
    // analyze the words in the ciphertext that differ by one letter from the dictionary
    // try to analyze words with len = 7 and stop when len will be equal to 9 (analisis more len is useless)
    for (size_t i = 7; i < 10; ++i) {
        perc = new_perc;
        Correction(i, 1);
        // If nothing has changed or has become worse, we will try to analyze words with a shorter length 
        // or words that differ by more than 1 letter
        if (GetPercRecognizedWords() <= perc) {
            for (size_t j = 5; j < i; ++j) {
                Correction(j, 1);
            }
            new_perc = GetPercRecognizedWords();
            if (new_perc <= perc) {
                Correction(i, 2);
            }
        }
    }

    return decrypt_text_;
}

void CrackCipherSystem::FreqsAnalysis()
{
    auto dict_freqs = GetSortedFreqsNGrams(dict_text_, 1);
    auto crypted_text_freqs = GetSortedFreqsNGrams(crypt_text_, 1);

    for (size_t i = 0; i < crypted_text_freqs.size(); ++i) {
        converting_letters_[crypted_text_freqs[i][0]] = dict_freqs[i][0];
        reverse_conv_letters_[dict_freqs[i][0]] = crypted_text_freqs[i][0];
    }
    ConvertLetters();
}

void CrackCipherSystem::NGramAnalysis(size_t max_words, size_t max_diff, size_t min_len, size_t max_len)
{
    if (min_len <= 1) {
        FreqsAnalysis();
        min_len = 2;
    }
    for (size_t len = min_len; len <= max_len; ++len) {
        auto dict_freqs = GetSortedFreqsNGrams(dict_text_, len);
        auto crypted_text_freqs = GetSortedFreqsNGrams(decrypt_text_, len);

        std::unordered_map<char, char> changed_chars;

        for (size_t i = 0; i < max_words; ++i) {
            for (size_t k = 0; k < 2/*maybe more?*/; ++k) {
                size_t num_diff{ 0 };
                for (size_t j = 0; j < len; ++j) {
                    if (dict_freqs[i + k][j] != crypted_text_freqs[i][j]) {
                        ++num_diff;
                    }
                }
                if (num_diff > 0 && num_diff <= max_diff) {
                    for (size_t j = 0; j < len; ++j) {
                        if (dict_freqs[i + k][j] != crypted_text_freqs[i][j]) {
                            char changed_char = reverse_conv_letters_.at(crypted_text_freqs[i][j]);
                            if (changed_chars.find(changed_char) == changed_chars.end()) {
                                changed_chars[changed_char] = dict_freqs[i + k][j];
                            }
                        }
                    }
                    break;
                }
                if (num_diff == 0) break;
            }
        }
        if (!changed_chars.empty()) {
            TryToChange(changed_chars);
        }
    }
}

void CrackCipherSystem::ConvertLetters()
{
    for (size_t i = 0; i < crypt_text_.size(); ++i) {
        if (crypt_text_[i] >= 'a' && crypt_text_[i] <= 'z') {
            decrypt_text_[i] = converting_letters_[crypt_text_[i]];
        }
    }
}

std::vector<std::string> CrackCipherSystem::GetSortedFreqsNGrams(std::string& str_analyz, int len_gram) const
{
    StringFreqs freqs = CountGrams(str_analyz, len_gram);
    std::vector<std::pair<std::string, size_t>> str_to_num_vector;
    str_to_num_vector.reserve('z' - 'a' + 1);
    std::transform(freqs.begin(), freqs.end(),
        std::back_inserter(str_to_num_vector), [](auto& pair) {
            return pair;
        });
    std::sort(str_to_num_vector.begin(), str_to_num_vector.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.second > rhs.second; //Mb < ?
        });
    std::vector<std::string> result;
    result.reserve(str_to_num_vector.size());
    std::transform(str_to_num_vector.begin(), str_to_num_vector.end(), std::back_inserter(result),
        [](const auto& el) {
            return el.first;
        });
    return result;
}

double CrackCipherSystem::GetPercRecognizedWords() const
{
    size_t all_words = source_words_.size();
    size_t recognized_word{ 0 };
    size_t one_char_word{ 0 };
    for (const auto& word : source_words_) {
        if (word.size() == 1) {
            ++one_char_word;
            continue;
        }
        if (dictionary_words_.find(word) != dictionary_words_.end()) {
            ++recognized_word;
        }
    }
    all_words -= one_char_word;
    return recognized_word / static_cast<double>(all_words);
}

void CrackCipherSystem::Correction(int len, int max_diff)
{
    std::string check_from(len, 'a');
    std::string check_to(len + 1, 'a');
    auto it_from_dict = dictionary_words_.lower_bound(check_from);
    auto it_to_dict = dictionary_words_.lower_bound(check_to);
    auto it_from_source = source_words_.lower_bound(check_from);
    auto it_to_source = source_words_.lower_bound(check_to);
    std::unordered_map<char, char> changed_chars;
    for (; it_from_dict != it_to_dict; ++it_from_dict) {
        auto it = it_from_source;
        for (; it != it_to_source; ++it) {
            int num_of_diff = 0;
            for (size_t i = 0; i < it_from_dict->size(); ++i) {
                if (it->at(i) != it_from_dict->at(i)) {
                    ++num_of_diff;
                    if (num_of_diff > max_diff) break;
                }
            }
            if (num_of_diff <= max_diff && num_of_diff != 0 ) {
                for (size_t i = 0; i < it_from_dict->size(); ++i) {
                    if (it->at(i) != it_from_dict->at(i)) {
                        char changed_char = reverse_conv_letters_.at(it->at(i));
                        if (changed_chars.find(changed_char) == changed_chars.end()) {
                            changed_chars[changed_char] = it_from_dict->at(i);
                        }
                    }
                }
                TryToChange(changed_chars);
                changed_chars.clear();
                break;
            }

        }
    }
}

void CrackCipherSystem::AlterDectyptText(std::unordered_map<char, char>& changed_chars)
{
    for (const auto& [from, to] : changed_chars) {
        char c = converting_letters_[from];
        for (auto& [ch1, ch2] : converting_letters_) {
            if (ch2 == to) {
                ch2 = c;
                reverse_conv_letters_[c] = ch1;
                break;
            }
        }
        converting_letters_[from] = to;
        reverse_conv_letters_[to] = from;
    }
    ConvertLetters();
}

bool caesar_cipher::CrackCipherSystem::TryToChange(std::unordered_map<char, char>& changed_chars)
{
    auto old_conv = converting_letters_;
    auto perc = GetPercRecognizedWords();
    AlterDectyptText(changed_chars);
    if (GetPercRecognizedWords() < perc) {
        converting_letters_ = old_conv;
        ConvertLetters();
        return false;
    }
    return true;
}
