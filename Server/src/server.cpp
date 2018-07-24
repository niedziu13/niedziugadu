#include <iostream>
#include "server_session.hpp"
#include "defines.hpp"
#include "server_logger.hpp"

int main() {
    ServerSession session;
    std::cout << "Siema tutaj serwer" << std::endl;
    int userSock;

    if ( session.SetupAndListen() == RET_OK ) {
        std::cout << "Server listing" << std::endl;
        while(1) {
            userSock = session.Accept();
            if ( userSock < 0 ) {
                LOG_E( "Accept error. \n" );
            } else {
                LOG_E( "A new user connected. \n" );
            }
        }
    }

	return 0;
}
