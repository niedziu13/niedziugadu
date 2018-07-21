/*
 * menu.cpp
 *
 *  Created on: Jul 20, 2018
 *      Author: niedziu
 */

#include "client_menu.hpp"
#include "HAL_UI.hpp"

std::ostream& operator<<(std::ostream& out, const ClientMenu& menu) {
    out<<"Following options are possible: " << std::endl;
    for(size_t i = 0; i < menu.GetNumberOfOptions(); i++){
        out << menu.GetOption(i).m_key << " - " << menu.GetOption(i).m_message << std::endl;
    }
    return out;
}

Option::Option(const char* message, ClientMenuOption code, char key):
    m_message( message ),
    m_code( code ),
    m_key( key )
{
}

int ClientMenu::FindOption(ClientMenuOption code, char key){
    int ret = -1;
    for(size_t i = 0; i < m_options.size(); i++){
        if( ( m_options[i].m_key == key ) || ( m_options[i].m_code == code ) ){
            ret = i;
            break;
        }
    }
    return ret;
}


ReturnCode ClientMenu::AddOption(const char* message, ClientMenuOption code, char key){
    ReturnCode ret = RET_OK;
    if( FindOption(code, key) != -1 ){
        ret = RET_EXISTING_OPTION;
    }
    if ( ret == RET_OK ) {
        m_options.push_back( Option(message, code, key) );
    }
    return ret;
}

ReturnCode ClientMenu::RemoveOption(const char* message, ClientMenuOption code, char key){
    ReturnCode ret = RET_OK;
    int i = FindOption(code, key);
    if ( i == -1 ){
        ret = RET_UNEXISTING_OPTION;
    } else if ( ( m_options[i].m_key != key ) || ( m_options[i].m_code != code ) ){
        ret = RET_INVALID_OPTION_FOUND;
    } else {
        m_options.erase( m_options.begin() + i );
    }
    return ret;
}

Option ClientMenu::GetOption(unsigned i) const{
    assert(i < m_options.size());
    return m_options[i];
}

size_t ClientMenu::GetNumberOfOptions() const{
    return m_options.size();
}

ReturnCode ClientMenu::Initialize(){
	ReturnCode retVal = RET_OK;
	char options_char[][2] = {
        {"Choose a user", "c"},
        ("Print available users", "p")
        ("Send a message", "s")};
	ClientMenuOption options_menu[] = {
        ClientOption_ChooseUser,
        ClientOption_PrintUsers,
        ClientOption_SendMessage
	};
	for(unsigned i = 0; i < sizeof(options); i++){
        ret = AddOption(options[i][0], options_menu[i], options[i][1][0]);
        if( ret != RET_OK ){
            break;
        }
	}
	return ret;
}

ClientMenuOption ClientMenu::GetUserChoice() const {
	ClientMenuOption retVal = ClientOption_InvalidOption;
	char key = HAL_US_GetChar();
	while( retVal == ClientOption_InvalidOption ) {
        for( auto i = m_options.begin(); i != m_options.end(); i++){
            if( i.m_key == key ){
                retVal = i.m_code;
                break;
            }
        }
	}
	return retVal;
}
