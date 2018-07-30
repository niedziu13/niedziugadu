/*
 * menu.hpp
 *
 *  Created on: Jul 20, 2018
 *      Author: niedziu
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include "defines.hpp"
#include <iostream>
#include <vector>

enum ClientMenuOption{
    ClientOption_InvalidOption,
    ClientOption_Exit,
    ClientOption_PrintFriends,
    ClientOption_ChooseUser,
    ClientOption_Login,
    ClientOption_SendMessage,
    ClientOption_Disconnect,
    ClientOption_Connect
};

class Option {
public:
    std::string m_message;
    ClientMenuOption m_code;
    // A char to choose an option. Used only if an action is introduced using char
    char m_key;
    Option(const char* message, ClientMenuOption code, char key);
};

class ClientMenu {
private:
    std::vector<Option> m_options;
    int FindOption(ClientMenuOption code, char key);
public:
    ClientMenu();
    ReturnCode AddOption(const char* message, ClientMenuOption code, char key);
    ReturnCode RemoveOption(const char* message, ClientMenuOption code, char key);
    Option GetOption(unsigned i) const;
    size_t GetNumberOfOptions() const;
    ClientMenuOption GetUserChoice() const;
};

std::ostream& operator<<(std::ostream& out, const ClientMenu& menu);

#endif /* MENU_HPP_ */
