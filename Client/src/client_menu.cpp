/*
 * menu.cpp
 *
 *  Created on: Jul 20, 2018
 *      Author: niedziu
 */

#include "client_menu.hpp"

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


ReternCode ClientMenu::AddOption(const char* message, ClientMenuOption code, char key){
    ReternCode ret = RET_OK;
    if( FindOption(code, key) != -1 ){
        ret = RET_EXISTING_OPTION;
    }
    if ( ret == RET_OK ) {
        m_options.push_back( Option(message, code, key) );
    }
    return ret;
}

ReternCode ClientMenu::RemoveOption(const char* message, ClientMenuOption code, char key){
    ReternCode ret = RET_OK;
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
