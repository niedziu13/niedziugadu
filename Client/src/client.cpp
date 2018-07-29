#include <client_menu.hpp>
#include <client_communication.hpp>
#include <HAL_UI.hpp>
#include <client.hpp>
#include <pthread.h>

void* HandleRecivingMessages(void *arg) {
	ClientSession* session = (ClientSession*) arg;
    Message msg;

    if ( ReciveMessage( msg, *session ) == RET_OK ) {
        switch ( msg.m_header.m_type ) {
        case MSGTYPE_LOGGING:
        break;
        default:
        break;
        }

    } else {
        std::cout<< "End of connection ?\n";
//        session->Close();
//        break;
    }

	return NULL;
}

int main(int argc, char** argv) {
    std::cout << "Siema tutaj Client" << std::endl;
    ClientMenu menu;
    ClientSession session;
    ReturnCode retCode;

    if( RET_OK != ( retCode = ParseArgs( argc, argv, session ) ) ) {
        LOG_E( "Invalid args. ERRNO: " << retCode << "\n" );
    } else {
        //Connect to server
        if (  session.ConnectToServer() == RET_OK ) {
            pthread_t thread;
            LOG_I( "A new user connected. \n" );
            pthread_create(&thread, NULL, HandleRecivingMessages, (void*)&session);

            while(1) {
                switch( menu.GetUserChoice() ) {
                case ClientOption_Exit:
                    break;
                case ClientOption_Login:
                    session.Login();
                    break;
                case ClientOption_ChooseUser:
                    break;
                case ClientOption_PrintFriends:
                    break;
                case ClientOption_SendMessage:
                    break;
                case ClientOption_InvalidOption:
                    break;
                default:
                    assert( false );
                    break;
                }
            }
        }
    }
	return 0;
}

ReturnCode ParseArgs(int argc, char** argv, ClientSession& session) {
    ReturnCode retVal = RET_OK;
    if ( argc != NUMBER_OF_ARGS ){
        retVal = RET_INVALID_NUMBER_OR_ARGS;
    } else if ( session.UpdateAddress( argv[1] ) == 0 ) {
        retVal = RET_INVALID_ADDRESS;
    }
    return retVal;
}
