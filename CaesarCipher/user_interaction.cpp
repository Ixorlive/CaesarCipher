#include "user_interaction.h"
#include "string_processing.h"

using namespace user_interaction;

void UserMenu::show()
{
	char q = 'n';
	while (q != 'Q' && q != 'q') {
		std::cout << "Input command: "
			<< "\n\tC - encrypt text from file"
			<< "\n\tD - decrypt text from file"
			<< "\n\tT - try to decrypt text from file using Frequency Analysis\n";
		char command;
		std::cin >> command;

		while (command != 'C' && command != 'c' &&
			command != 'T' && command != 't' &&
			command != 'D' && command != 'd') {
			std::cout << "Unknown command, try again:\n";
			std::cin >> command;
		}
		Sleep(400);
		system("cls");
		switch (command)
		{
		case 'C':
		case 'c':
			EncryptText();
			break;
		case 'D':
		case 'd':
			DecryptText();
			break;
		case 'T':
		case 't':
			CrackText();
			break;
		default:
			return;
		}
		system("cls");
		std::cout << "=== Input Q for exit or any for continue ===\n";
		std::cin >> q;
	}
}

void UserMenu::EncryptText()
{
	auto text = GetTextFromFile(GetPath());
	if (!text.has_value()) {
		return;
	}
	auto [k, keyword] = GetKeys();
	std::ofstream output_file("./output.txt");
	output_file << cipher_.crypt(*text, k, keyword);
}

void UserMenu::DecryptText()
{
	auto text = GetTextFromFile(GetPath());
	if (!text.has_value()) {
		return;
	}
	auto [k, keyword] = GetKeys();
	std::ofstream output_file("./output.txt");
	output_file << cipher_.decrypt(*text, k, keyword);
}

void UserMenu::CrackText()
{
	if (!crack_system_) {
		try {
			crack_system_ = std::make_shared<CrackCipherSystem>(CrackCipherSystem{});
		}
		catch (std::exception& e) {
			std::cout << "error: " << e.what() << std::endl;
			return;
		}
	}
	auto path = GetPath();
	std::ofstream output_file("./output.txt");
	output_file << crack_system_->crack(path);;
}

std::filesystem::path UserMenu::GetPath()
{
	std::string path;
	std::cout << "Input path to file:\n";
	std::cin >> path;
	return path;
}

std::optional<std::string> UserMenu::GetTextFromFile(const std::filesystem::path& path)
{
	std::ifstream file (path);
	int try_num = 0;
	while (!file.is_open() 
			|| std::filesystem::status(path).type() != std::filesystem::file_type::regular) {
		std::cout << "File not found, try agein: ";
		++try_num;
		if (try_num == 4) {
			std::cout << "Fail try, try next time";
			return std::nullopt;
		}
	}
	//read all text
	std::string str = GetLowerTextFromFile(file);
	return str;
}

std::pair<int, std::string> UserMenu::GetKeys()
{
	int k;
	std::string keyword;
	std::cout << "input k (shift)\n";
	std::cin >> k;
	std::cout << "input keyword:\n";
	std::cin >> keyword;
	toLower(keyword);
	return { k, keyword };
}