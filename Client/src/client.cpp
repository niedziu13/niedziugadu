#include <iostream>
#include <client_menu.hpp>


int main() {
    std::cout << "Siema tutaj Client" << std::endl;
    ClientMenu menu;
    menu.AddOption("Choose a user", ClientOption_ChooseUser, 'c');
    menu.AddOption("Print available users", ClientOption_PrintUsers, 'p');
    menu.AddOption("Send a message", ClientOption_SendMessage, 's');

    std::cout << menu << std::endl;
	return 0;
}
