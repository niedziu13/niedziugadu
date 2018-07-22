#include <client_menu.hpp>
#include <client.hpp>


int main(int argc, char** argv) {
    std::cout << "Siema tutaj Client" << std::endl;
    ClientMenu menu;
    menu.Initialize();


    switch( menu.GetUserChoice() ) {
    case ClientOption_Exit:
        break;
    case ClientOption_ChooseUser:
        break;
    case ClientOption_PrintUsers:
        break;
    case ClientOption_SendMessage:
        break;
    case ClientOption_InvalidOption:
        break;
    default:
        assert( false );
        break;
    }
	return 0;
}

ReturnCode ParseArgs(int argc, char** argv){
    ReturnCode retVal = RET_OK;
    if( argc != NUMBER_OF_ARGS ){
        retVal = RET_INVALID_NUMBER_OR_ARGS;
    } else if ( inet_aton(argv[1], &adr_serw.sin_addr) != 0 ) {
        retVal = RET_INVALID_ADDRESS;
    }
    return retVal;
}
