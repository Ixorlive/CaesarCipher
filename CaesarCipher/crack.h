#pragma once

#include <string>
#include <set>
#include <fstream>
#include <unordered_map>
#include <filesystem>

namespace caesar_cipher {

	class CrackCipherSystem
	{
	public:
		CrackCipherSystem(std::filesystem::path path = "dictionary.txt");

		struct comp {
			template <typename T>
			// Comparator function
			bool operator()(const T& l, const T& r) const
			{
				if (l.size() != r.size()) {
					return l.size() < r.size();
				}
				return l < r;
			}
		};

		using StringFreqs = std::unordered_map<std::string, size_t>;
		using Words = std::set<std::string_view, comp>;

		std::string crack(std::filesystem::path path_to_file);

	private:
		const std::filesystem::path dict_path_;

		std::string dict_text_;
		std::string crypt_text_;
		std::string decrypt_text_;

		Words dictionary_words_;
		Words source_words_;

		std::unordered_map<char, char> converting_letters_;
		std::unordered_map<char, char> reverse_conv_letters_;

		std::vector<std::string> GetSortedFreqsChars(std::string& str_analyz, int len_gram) const;

		double GetPercRecognizedWords() const;
		//Frequency analysis every symbol on dictionary and crypt text
		void FreqsAnalysis();
		//Frequence analysis digrams, trigrams and fourgram or more ("th", "the", "some" for example)
		void NGramAnalysis(size_t max_words = 10, size_t max_diff = 1, size_t min_len = 2, size_t max_len = 4);
		//Convertion letters in decrypt_text
		void ConvertLetters();
		//len - length of word, max_diff = maximum difference in words
		void Correction(int len, int max_diff);

		void AlterConvertingLetters(std::unordered_map<char, char>& changed_chars);
	};
} //namespace caesar_cipher

