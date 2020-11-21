#ifndef _BUFFCONT_H_
#define _BUFFCONT_H_
#include "helper.h"
#include "VarOps.h"
#include <cstdint>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

// gets the first 4 bits to tell the operation
uint16_t getOpCode(uint8_t *rbuff);

// returns the opcode, variables, and results from the given msg
uint16_t GetParams(uint16_t msg, bool &aChk, bool &bChk, bool &rChk);

// gets the client id
uint32_t getClientID(uint8_t *rbuff);

// gets first argument
int64_t getA(uint8_t *rbuff);

// gets second argument
int64_t getB(uint8_t *rbuff);

// gets the length of the filename
uint16_t getFileLen(uint8_t *rbuff);

// gets the file name based on length given from fileLen()
uint8_t *getFileName(uint8_t *rbuff, uint16_t fileLen, uint8_t* name);

// gets the offset from rbuff
uint64_t getOffset(uint8_t *rbuff);

// gets the buffsize from rbuff
uint16_t getBuffsize(uint8_t *rbuff);

// gets the variable that is a number
int64_t getNumVar(uint8_t* rbuff);

// gets the variable whether it is a string or a number
bool parseMathVars(uint8_t* rbuff, uint8_t &varLen, int64_t &a, int64_t &b, bool aChk, bool bChk, bool rChk, int);

#endif