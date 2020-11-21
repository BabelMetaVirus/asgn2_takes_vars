#include "helper.h"

void validArgCount(int _argc){
    // check if the correct amount of arguments are present
    if (_argc < 2)
    {
        err(EXIT_FAILURE, "Not enough arguments\n");
        exit(-1);
    }
    else if (_argc > 2)
    {
        err(EXIT_FAILURE, "Too many arguments\n");
        exit(-1);
    }

}

void validatePort(char* p){
    if(!p){
        printf("No port specified.\n");
        exit(1);
    }
}

void getProtocolData(char* _arg, char* ip, int& port){
    strcpy(ip,_arg);
    char* tmp = strchr(ip, ':');
    if(!tmp)
    {
        printf("No port specified\n"); // print out error message if port is not found
        exit(1);
    }
    ++tmp;
    port = std::stoi(tmp);
    bool zedOut = false;
    for(int i = 0; i < 16; i++){
        if(zedOut == true){
            ip[i] = 0;
        }
        else if(ip[i] == ':'){
            ip[i] = 0;
            zedOut = true;
        }
    }
}

// returns the int socket from `socket`
// takes in params: server_name_string(sns), port_number(portNo), and int reference to sock
int getSock(char* sns, int portNo, int &sock){
    // given socket code from pages resouce
    struct hostent *hent = gethostbyname(sns /* eg "localhost" */);
    struct sockaddr_in addr;
    memcpy(&addr.sin_addr.s_addr, hent->h_addr, hent->h_length);
    addr.sin_port = htons(portNo);
    addr.sin_family = AF_INET;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Server could not be created\n");
        exit(-1);
    }

    int enable = 1;
    int sso = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (sso < 0)
    {
        perror("Socket control failed\n");
        close(sock);
        exit(-1);
    }

    int binds = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (binds < 0)
    {
        perror("Bind failed\n");
        close(sock);
        exit(-1);
    }

    // code used from jim's post on piazza
    if (listen(sock,128) != 0) err(2,"unable to listen");

    return sock; // return sock so it can be used outside of the function
    // end of given socket code thats filled in to work
}

// checks if the character is an upper case or lower case letter
// using the decimal version of the char
bool isAlpha(char c) {
    if(c >= 65 && c <= 90) {
        return true;
    }
        
    if(c >= 97 && c <= 122) {
        return true;
    }

    printf("isAlpha: not a valid letter\n");
    return false;
}

// checks if the char is a valid character, returns true if:
// isAlpha == true
// is an underscore _
// is a number 0-9
bool isValid(char c) {
    if(isAlpha(c)) {
        if(c == 95 || (c >= 48 && c <= 57)) {
            return true;
        }
    }

    printf("isValid: not a valid letter, number or underscore\n");
    return false;
}

// validates the variable based on 3 things:
// variable is 1-31 chars long
// var[0] is a letter
// 2-31 must be letter, number or underscore
bool ValidateVarName(char* var) {
    if(strlen(var) == 0 || strlen(var) > 32) {
        printf("var length is either 0 or greater than 31\n");
        return false;
    }

    if(!isAlpha(var[0])) {
        printf("first character was not a letter\n");
        return false;
    }

    for(size_t i=2; i < strlen(var); i++) {
        return isValid(var[i]);
    }

    return false;
}