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
    ClientOption_Exit,
    ClientOption_PrintUsers,
    ClientOption_ChooseUser,
    ClientOption_SendMessage
};

class Option {
public:
    std::string m_message;
    ClientMenuOption m_code;
    char m_key;
    Option(const char* message, ClientMenuOption code, char key);
};

class ClientMenu {
private:
    std::vector<Option> m_options;
    int FindOption(ClientMenuOption code, char key);
public:
    ReternCode AddOption(const char* message, ClientMenuOption code, char key);
    ReternCode RemoveOption(const char* message, ClientMenuOption code, char key);
    Option GetOption(unsigned i) const;
    size_t GetNumberOfOptions() const;
};

std::ostream& operator<<(std::ostream& out, const ClientMenu& menu);

#endif /* MENU_HPP_ */
