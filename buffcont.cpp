#include "buffcont.h"

// gets the first 4 bits to tell the operation
uint16_t getOpCode(uint8_t *rbuff)
{
    uint8_t opCodeA[2];
    opCodeA[0] = rbuff[0];
    opCodeA[1] = rbuff[1];

    // If code is not 2, there is an error.

    uint16_t opCode = convertFromArray16(opCodeA);
    return opCode;
}

// returns the opcode, variables, and results from the given msg
uint16_t GetParams(uint16_t msg, bool &aChk, bool &bChk, bool &rChk) {
    // check for variable A
    //printf("msg is: %04x\n", msg);
    uint8_t arr[2];
    uint8_t* op = convertToArray(msg,arr);
    if( op[0] == 0x01) {
        if( (msg&VAR_A) == VAR_A) {
        aChk = true;
        }
        else {
            aChk = false;
        }

        // check for variable B
        if( (msg&VAR_B) == VAR_B) {
            bChk = true;
        }
        else {
            bChk = false;
        }

        // check for variable result
        if( (msg&VAR_R) == VAR_R) {
            rChk = true;
        }
        else {
            rChk = false;
        }
        return msg&OPCODE;
    }
    else {
        return msg;
    }
    
}

// gets the client id
uint32_t getClientID(uint8_t *rbuff)
{
    uint8_t cidA[4];
    for (int i = 0; i < 4; i++)
    {
        cidA[i] = rbuff[i];
    }
    // Same thing for 4.
    uint32_t cid = convertFromArray32(cidA);

    return cid;
}

// gets first argument
int64_t getA(uint8_t *rbuff)
{
    uint8_t ua[8];
    for (int i = 0; i < 8; i++)
    {
        ua[i] = rbuff[i + 6];
    }

    int64_t a = convertFromArray64(ua);
    return a;
}

// gets second argument
int64_t getB(uint8_t *rbuff)
{
    uint8_t ub[8];
    for (int i = 0; i < 8; i++)
    {
        ub[i] = rbuff[i + 14];
    }

    int64_t b = convertFromArray64(ub);
    return b;
}

// gets the length of the filename
uint16_t getFileLen(uint8_t *rbuff)
{
    uint8_t length[2];
    for (int i = 0; i < 2; i++)
    {
        length[i] = rbuff[i];
    }
    uint16_t len = convertFromArray16(length);
    return len;
}

// gets the file name based on length given from fileLen()
uint8_t *getFileName(uint8_t *rbuff, uint16_t fileLen, uint8_t* name)
{
    for (int i = 0; i < fileLen; i++)
    {
        name[i] = rbuff[i];
    }
    
    return name;
}

// gets the offset from rbuff
uint64_t getOffset(uint8_t *rbuff)
{
    uint8_t os[8];
    for (int i = 0; i < 8; i++)
    {
        os[i] = rbuff[i];
    }
    uint64_t off = convertFromArray64(os);
    return off;
}

// gets the buffsize from rbuff
uint16_t getBuffsize(uint8_t *rbuff)
{
    uint8_t bs[2];
    for (int i = 0; i < 2; i++)
    {
        bs[i] = rbuff[i];
    }
    uint16_t buff = convertFromArray16(bs);
    return buff;
}

// gets the variable that is a number
int64_t getNumVar(uint8_t* rbuff) {
    uint8_t ua[8];
    for (int i = 0; i < 8; i++)
    {
        ua[i] = rbuff[i];
    }

    int64_t a = convertFromArray64(ua);
    return a;
}

// gets the variable whether it is a string or a number
bool parseMathVars(uint8_t* rbuff, uint8_t &varLen, int64_t &a, int64_t &b, bool aChk, bool bChk, bool rChk, int varErr) 
{
    // reserve these buffers in case there is a string variable passed
    char varA[32];
    char varB[32];
    char varR[32];

    // different cases based on the variable checks
    // varA: num, varB: num, varR: empty
    if(aChk == false && bChk == false && rChk == false) {
        // a is the first number
        a = getNumVar(rbuff);
        // shift buffer by 8 since we are done with the first 8 bytes
        rbuff += 8;
        // b is the second number
        b = getNumVar(rbuff);

        return true;
    }
    
    // varA: num, varB: var, varR: empty
    if(aChk == false && bChk == true && rChk == false) {
        // get number
        a = getNumVar(rbuff);
        // shift buffer by 8 since we are done with the first 8 bytes
        rbuff += 8;
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varB[i] = rbuff[i];
        }

        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varB)) {
            varErr = GetVar(varB, b);
            if(varErr == 22)
                return false;
        }

        return true;
    }
    
    // varA: var, varB: num, varR: empty
    if(aChk == true && bChk == false && rChk == false) {
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varA[i] = rbuff[i];
        }
        // shift buffer by length of the var
        rbuff += varLen;
        
        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varA)) {
            varErr = GetVar(varA, a);
            if(varErr == 22)
                return false;
        }
        
        // get number
        b = getNumVar(rbuff);

        return true;
    }

    // varA: num, varB: num, varR: var
    if(aChk == false && bChk == false && rChk == true) {
        // get number
        a = getNumVar(rbuff);
        // shift over 8
        rbuff += 8;
        // get number
        b = getNumVar(rbuff);
        // shift over 8
        rbuff += 8;
        // var length
        varLen = rbuff[0];
        // shift past len
        rbuff++;
        // get var from buff
        for(uint8_t i = 0; i < varLen; i++) {
            varR[i] = rbuff[i];
        }
        // validate var name
        if(!ValidateVarName(varR)) {
            varErr = ENOENT;
            return false;
        }
        return true;
    }

    // varA: var, varB: num, varR: var
    if(aChk == true && bChk == false && rChk == true) {
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varA[i] = rbuff[i];
        }
        // shift buffer by length of the var
        rbuff += varLen;
        
        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varA)) {
            varErr = GetVar(varA, a);
            if(varErr == 22)
                return false;
        }
        // get number
        b = getNumVar(rbuff);
        // shift over 8
        rbuff += 8;

        // reset len
        varLen = 0;
        // get the length of the var
        varLen = rbuff[0];
        // shift past len
        rbuff++;
        // get var from buff
        for(uint8_t i = 0; i < varLen; i++) {
            varR[i] = rbuff[i];
        }
        // validate var name
        if(!ValidateVarName(varR)) {
            varErr = ENOENT;
            return false;
        }

        return true;
    }

    // varA: num, varB: var, varR: var
    if(aChk == false && bChk == true && rChk == true) {
        // get number
        a = getNumVar(rbuff);
        // shift over 8
        rbuff += 8;

        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varB[i] = rbuff[i];
        }

        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varB)) {
            varErr = GetVar(varB, b);
            if(varErr == 22)
                return false;
        }

        // reset len
        varLen = 0;
        // get the length of the var
        varLen = rbuff[0];
        // shift past len
        rbuff++;
        // get var from buff
        for(uint8_t i = 0; i < varLen; i++) {
            varR[i] = rbuff[i];
        }
        // validate var name
        if(!ValidateVarName(varR)) {
            varErr = ENOENT;
            return false;
        }
        return true;
    }

    // varA: var, varB: var, varR: empty
    if(aChk == true && bChk == true && rChk == false) {
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varA[i] = rbuff[i];
        }
        // shift buffer by length of the var
        rbuff += varLen;
        
        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varA)) {
            varErr = GetVar(varA, a);
            if(varErr == 22)
                return false;
        }
        // reset len
        varLen = 0;
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varB[i] = rbuff[i];
        }

        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varB)) {
            varErr = GetVar(varB, b);
            if(varErr == 22)
                return false;
        }
        return true;
    }

    // varA: var, varB: var, varR: var
    if(aChk == true && bChk == true && rChk == true) {
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varA[i] = rbuff[i];
        }
        // shift buffer by length of the var
        rbuff += varLen;
        
        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varA)) {
            varErr = GetVar(varA, a);
            if(varErr == 22)
                return false;
        }
        // reset len
        varLen = 0;
        // get the length of the var
        varLen = rbuff[0];
        // move one byte to get to the start of the var
        rbuff++;

        // get the var from rBuff
        for(uint8_t i = 0; i < varLen; i++) {
            varB[i] = rbuff[i];
        }

        // validate the variable name and retrieve it from table if valid
        if(ValidateVarName(varB)) {
            varErr = GetVar(varB, b);
            if(varErr == 22)
                return false;
        }

        // reset len
        varLen = 0;
        // get the length of the var
        varLen = rbuff[0];
        // shift past len
        rbuff++;
        // get var from buff
        for(uint8_t i = 0; i < varLen; i++) {
            varR[i] = rbuff[i];
        }
        // validate var name
        if(!ValidateVarName(varR)) {
            varErr = ENOENT;
            return false;
        }
        return true;
    }

    return false;
}