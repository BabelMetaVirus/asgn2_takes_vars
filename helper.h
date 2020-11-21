#ifndef _HELPER_H_
#define _HELPER_H_
#include "conversion.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OP_ADD 0x0101 // addition
#define OP_SUB 0x0102 // subtractoin
#define OP_MUL 0x0103 // multiplication
#define OP_DIV 0x0104 // division
#define OP_MOD 0x0105 // modulo
#define OP_DEL 0x010f // delete
#define OP_RD 0x0201 // read file
#define OP_WR 0x0202 // write file
#define OP_CR 0x0210 // create file
#define OP_FS 0x0220 // filesize
#define OP_DP 0x0301 // dump to file
#define OP_LD 0x0302 // load from file
#define OP_CL 0x0310 // clear all values from key-value store

#define OPCODE 0x0f0f // gets opcode
#define VAR_A 0x0010 // AND to check for variable A
#define VAR_B 0x0020 // AND to check for variable B
#define VAR_R 0X0040 // AND to check for variable result
#define CLEAR 0x0badbad0 // check for clear - format uint32_t

void validArgCount(int _argc);

void validatePort(char* p);

void getProtocolData(char* _arg, char* ip, int& port);

// returns the int socket from `socket`
// takes in params: server_name_string(sns), port_number(portNo), and int reference to sock
int getSock(char* sns, int portNo, int &sock);

// checks if the character is an upper case or lower case letter
// using the decimal version of the char
bool isAlpha(char c);

// checks if the char is a valid character, returns true if:
// isAlpha == true
// is an underscore _
// is a number 0-9
bool isValid(char c);

// validates the variable based on 3 things:
// variable is 1-31 chars long
// var[0] is a letter
// 2-31 must be letter, number or underscore
bool ValidateVarName(char* var);

#endif
