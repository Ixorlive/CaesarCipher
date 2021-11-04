#include "user_interaction.h"

int main() {
	user_interaction::UserMenu menu;
	try {
		menu.show();
	}
	catch (std::exception& e) {
		std::cout << "An error occurred while the program was running. Error text: " << e.what() << std::endl;
	}
}