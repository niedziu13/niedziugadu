#include <client_menu.hpp>
#include <HAL_UI.hpp>
#include <client.hpp>

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
            switch( menu.GetUserChoice() ) {
            case ClientOption_Exit:
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
	return 0;
}

ReturnCode ParseArgs(int argc, char** argv, ClientSession& session){
    ReturnCode retVal = RET_OK;
    if( argc != NUMBER_OF_ARGS ){
        retVal = RET_INVALID_NUMBER_OR_ARGS;
    } else if ( session.UpdateAddress( argv[1] ) == 0 ) {
        retVal = RET_INVALID_ADDRESS;
    }
    return retVal;
}
