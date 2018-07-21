#include <iostream>
#include <client_menu.hpp>


int main() {
    std::cout << "Siema tutaj Client" << std::endl;
    ClientMenu menu;
    menu.Initialize();

    std::cout << menu << std::endl;
	return 0;
}
