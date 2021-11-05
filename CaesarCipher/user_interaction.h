#pragma once

#include <iostream>
#include "CaesarCipher.h"
#include "crack.h"
#include <ctime>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>

using namespace caesar_cipher;

namespace user_interaction {

	class UserMenu
	{
	public:
		UserMenu() = default;

		void show();

	private:
		CaesarCipher cipher_{ AlphabetType::ENGLISH_LOWER };
		std::shared_ptr<CrackCipherSystem> crack_system_{ nullptr };
		std::pair<int, std::string> GetKeys();

		void EncryptText();
		void DecryptText();
		void CrackText();

		std::filesystem::path GetPath();
		std::optional<std::string> GetTextFromFile(const std::filesystem::path& path);

		void ChooseAlphabet();
	};
} //namespace iser_interaction

