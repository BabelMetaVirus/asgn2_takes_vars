#include "helper.h"
#include "buffcont.h"
#include "operations.h"
#include "fileOps.h"
#include "VarOps.h"

// NOTE: Will need to change for hash size argument and thread count argument.
int main(int argc, char *argv[])
{
    validArgCount(argc);
    char sns[16];
    int portNo;
    getProtocolData(argv[1], sns, portNo);

    printf("server_name_string is: %s\n", sns);
    printf("port_number is: %d\n", portNo);

    int sock = getSock(sns, portNo, sock);

    // loop request for the socket
    while (1)
    {
        int cl = accept(sock,NULL,NULL); // accepts socket and outputs file descriptor for the socket
        printf("cl is: %d\n", cl);
        // check if cl is valid
        if (cl < 0)
        {
            perror("Accept failed\n");
            exit(-1);
        }
        printf("Connection Established\n");

        // read the bytes into the buffer
        // while (1) {
            // allocate read in buffer and send out buffer
            uint8_t rstatic[8192];
            uint8_t* rbuff = rstatic;
            uint8_t sstatic[8192];
            uint8_t* sbuff = sstatic;

            // read into the buffer
            int amtRead = recv(cl, rbuff, (8192) - 1, 0);
            printf("amtRead is: %d\n", amtRead);

            // if the amount read is 0, break
            if (amtRead < 1)
            {
                printf("code broke\n");
                exit(-1);
                break;
            }
            // print out buffer to see what was read
            // printf("rbuff as a uint8_t\n");
            // for (int i = 0; i < 30; i++)
            // {
            //     printf("rbuff[%d]: %02x\n", i, rbuff[i]);
            // }

            // gets which operation is going to be performed
            uint16_t fullOp = getOpCode(rbuff);
            // shift rbuff by 2 because we are done getting the opcode
            rbuff = rbuff+2;

            // breaks down opcode to get the parameters
            // aChk = whether there is a "a" variable
            // bChk = whether there is a "b" variable
            // rChk = whether there is a "result" variable
            // params is the isolated op code
            bool aChk = false, bChk = false, rChk = false;
            uint16_t funcOp = GetParams(fullOp, aChk, bChk, rChk);

            // print out variable checks:
            printf("aChk is: %d\n", aChk);
            printf("bChk is: %d\n", bChk);
            printf("rChk is: %d\n", rChk);

            // gets the client identifier
            uint32_t cid = getClientID(rbuff);

            // shift buffer by 6 because we are done with the client id
            rbuff = rbuff+4;
            
            // ran into the issue where create/filesize were breaking due to my initial parseVars()
            // so I broke it up into parseMathVars() and changing GetParams() to check if it was doing a
            // math operation before going through with the msg&0x0f0f
            // very jank fix but I think it seems to work
            int64_t a = 0, b = 0;
            uint8_t varLen = 0;
            int varErr = 0;
            
            // goes to case based on params
            switch (funcOp)
            {
                case OP_ADD:
                {
                    //printf("----you just entered add\n");
                    parseMathVars(rbuff, varLen, a, b, aChk, bChk, rChk, varErr);
                    int64_t ans = add(a, b);
                    uint8_t err = 0;
                    if (ans == EINVAL)
                    {
                        err = EINVAL;
                    }
                    
                    // printf("err is: %x\n", err);
                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // print out buffer for testing
                    // printf("cidArray as a uint8_t\n");
                    for (int i = 0; i < 4; i++)
                    {
                        // printf("cidArray[%d]: %02x\n", i, cidArr[i]);
                        sbuff[i] = cidArr[i];
                    }
  
                    sbuff[4] = err;

                    uint8_t *hexAns = convertToArray(ans, arr);
                    // print out buffer for testing
                    // printf("hexAns as a uint8_t\n");
                    for (int i = 0; i < 8; i++)
                    {
                        // printf("hexAns[%d]: %02x\n", i, hexAns[i]);
                        sbuff[i + 5] = hexAns[i];
                    }
                    // print out buffer for testing
                    // printf("sbuff as a uint8_t\n");
                    // for (int i = 0; i < 13; i++) {
                    //    printf("sbuff[%d]: %02x\n", i, sbuff[i]);
                    //}

                    send(cl, sbuff, 13, 0);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_SUB:
                {
                    //printf("--you just entered sub\n");
                    parseMathVars(rbuff, varLen, a, b, aChk, bChk, rChk, varErr);
                    int64_t ans = sub(a, b);
                    // printf("ans is: %ld\n", ans);
                    uint8_t err = 0;
                    if (ans == EINVAL)
                    {
                        err = EINVAL;
                    }
                    // printf("err is: %x\n", err);
                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    // print out buffer for testing
                    // printf("cidArray as a uint8_t\n");
                    for (int i = 0; i < 4; i++)
                    {
                        // printf("cidArray[%d]: %02x\n", i, cidArr[i]);
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = err;

                    uint8_t *hexAns = convertToArray(ans, arr);
                    
                    // print out buffer for testing
                    // printf("hexAns as a uint8_t\n");
                    for (int i = 0; i < 8; i++)
                    {
                        // printf("hexAns[%d]: %02x\n", i, hexAns[i]);
                        sbuff[i + 5] = hexAns[i];
                    }

                    send(cl, sbuff, 13, 0);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_MUL:
                {
                    // printf("---you just entered mul\n");
                    parseMathVars(rbuff, varLen, a, b, aChk, bChk, rChk, varErr);
                    int64_t ans = mul(a, b);
                    // printf("ans is: %ld\n", ans);
                    uint8_t err = 0;
                    if (ans == EINVAL)
                    {
                        err = EINVAL;
                    }
                    // printf("err is: %x\n", err);
                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // print out buffer for testing
                    // printf("cidArray as a uint8_t\n");
                    for (int i = 0; i < 4; i++)
                    {
                        // printf("cidArray[%d]: %02x\n", i, cidArr[i]);
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = err;
                    
                    uint8_t *hexAns = convertToArray(ans, arr);
                    
                    // print out buffer for testing
                    // printf("hexAns as a uint8_t\n");
                    for (int i = 0; i < 8; i++)
                    {
                        // printf("hexAns[%d]: %02x\n", i, hexAns[i]);
                        sbuff[i + 5] = hexAns[i];
                    }

                    send(cl, sbuff, 13, 0); // send out to client
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_DIV:
                {   
                    parseMathVars(rbuff, varLen, a, b, aChk, bChk, rChk, varErr);
                    int64_t ans = divi(a, b);
                    //printf("ans is: %ld\n", ans);
                    uint8_t err = 0;
                    if (ans == EINVAL)
                    {
                        err = EINVAL;
                    }

                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // print out buffer for testing
                    // printf("cidArray as a uint8_t\n");
                    for (int i = 0; i < 4; i++)
                    {
                        // printf("cidArray[%d]: %02x\n", i, cidArr[i]);
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = err;
                    
                    uint8_t *hexAns = convertToArray(ans, arr);
                    
                    // print out buffer for testing
                    // printf("hexAns as a uint8_t\n");
                    for (int i = 0; i < 8; i++)
                    {
                        // printf("hexAns[%d]: %02x\n", i, hexAns[i]);
                        sbuff[i + 5] = hexAns[i];
                    }

                    send(cl, sbuff, 13, 0); // send out to client
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_MOD:
                {
                    parseMathVars(rbuff, varLen, a, b, aChk, bChk, rChk, varErr);
                    int64_t ans = mod(a, b);
                    // printf("ans is: %ld\n", ans);
                    uint8_t err = 0;
                    if (ans == EINVAL)
                    {
                        err = EINVAL;
                    }
                    // printf("err is: %x\n", err);
                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // print out buffer for testing
                    // printf("cidArray as a uint8_t\n");
                    for (int i = 0; i < 4; i++)
                    {
                        // printf("cidArray[%d]: %02x\n", i, cidArr[i]);
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = err;
                    
                    uint8_t *hexAns = convertToArray(ans, arr);
                    
                    // print out buffer for testing
                    // printf("hexAns as a uint8_t\n");
                    for (int i = 0; i < 8; i++)
                    {
                        // printf("hexAns[%d]: %02x\n", i, hexAns[i]);
                        sbuff[i + 5] = hexAns[i];
                    }

                    send(cl, sbuff, 13, 0); // send out to client
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_RD:
                {
                    //printf("----you just entered read\n");
                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                    }

                    // gets the offset
                    uint64_t offset = getOffset(rbuff);
                    // shift rbuff by 8 since we are done with offset
                    rbuff += 8;
                    uint16_t buffsize = getBuffsize(rbuff);
                    // shift rbuff by 2 since we are done with buffsize
                    rbuff += 2;

                    uint8_t readBuff[8192];
                    for(int i=0; i<buffsize; i++){
                        readBuff[i] = rbuff[i];
                    }
                    
                    int code = 0;
                    bool rf = readFile((char*)name, offset, buffsize, readBuff, code);

                    // put client identifier into the send bufferfr
                    uint8_t arr[4];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    // check readFile/error code for the client to buffer
                    if (rf)
                    {
                        sbuff[4] = 0;
                    }
                    else
                    {
                        sbuff[4] = code;
                    }

                    uint8_t *codeLen = convertToArray((uint16_t)code, arr);
                    
                    for (int i = 0; i < 2; i++)
                    {
                        // printf("buffSizeArr[%d]: %02x\n", i, buffSizeArr[i]);
                        sbuff[i + 5] = codeLen[i];
                    }

                    for (int i = 0; i < buffsize; i++)
                    {
                        // printf("readBuff[%d]: %02x\n", i, readBuff[i]);
                        sbuff[i + 7] = readBuff[i];
                    }

                    delete [] name;
                    send(cl, sbuff, (7 + buffsize), 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_WR:
                {
                    //printf("----you just entered write\n");
                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    // printf("fileLen is: %d\n", fileLen);
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                    }

                    // gets the offset
                    uint64_t offset = getOffset(rbuff);
                    //printf("offset is: %lu\n", offset);
                    // shift rbuff by 8 since we are done with offset
                    rbuff += 8;
                    uint16_t buffsize = getBuffsize(rbuff);
                    // shift rbuff by 2 since we are done with buffsize
                    rbuff += 2;
                    // printf("buffsize is: %d\n", buffsize);

                    uint8_t readBuff[8192];
                    for(int i=0; i<buffsize; i++){
                        readBuff[i] = rbuff[i];
                    }
                    
                    int code = 0;
                    bool rf = writeFile((char*)name, offset, buffsize, readBuff, code);

                    // put client identifier into the send bufferfr
                    uint8_t arr[4];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    // check readFile/error code for the client to buffer
                   // printf("code is: %d\n", code);
                    if (rf)
                    {
                        sbuff[4] = 0;
                    }
                    else
                    {
                        sbuff[4] = code;
                    }

                    uint8_t *codeLen = convertToArray((uint16_t)code, arr);
                    
                    for (int i = 0; i < 2; i++)
                    {
                        // printf("buffSizeArr[%d]: %02x\n", i, buffSizeArr[i]);
                        sbuff[i + 5] = codeLen[i];
                    }

                    for (int i = 0; i < buffsize; i++)
                    {
                        // printf("readBuff[%d]: %02x\n", i, readBuff[i]);
                        sbuff[i + 7] = readBuff[i];
                    }

                    delete [] name;
                    send(cl, sbuff, (7 + buffsize), 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_CR:
                {
                    // printf("----you just entered create\n");
                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    // printf("fileLen is: %d\n", fileLen);
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                    }                    
                    int code = 0;
                    bool cf = createFile((char*)name, code);

                    // put client identifier into the send bufferfr
                    uint8_t arr[4];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // puts the client ID into the send buffer
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    // check createFile/error code for the client to buffer
                    printf("code is: %d\n", code);
                    if (cf)
                    {
                        sbuff[4] = 0;
                    }
                    else
                    {
                        sbuff[4] = code;
                    }
                    
                    delete [] name;
                    send(cl, sbuff, 5, 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_FS:
                {
                    printf("----you just entered filesize\n");

                    // print out buffer for testing
                    // printf("sbuff as a uint8_t\n");
                    // for (int i = 0; i < 15; i++) {
                    //    printf("rbuff[%d]: %02x\n", i, rbuff[i]);
                    // }

                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    // printf("fileLen is: %d\n", fileLen);
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                    }                    
                    int code = 0, fsize = 0;
                    bool cf = getFileSize((char*)name, code, fsize);

                    // put client identifier into the send bufferfr
                    uint8_t arr[8];
                    uint8_t *cidArr = convertToArray(cid, arr);
                    
                    // puts the client ID into the send buffer
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    // check createFile/error code for the client to buffer
                    printf("code is: %d\n", code);
                    if (cf)
                    {
                        sbuff[4] = 0;
                    }
                    else
                    {
                        sbuff[4] = code;
                    }

                    printf("filesize is: %d\n", fsize);
                    uint8_t *fsizeArr = convertToArray((uint64_t)fsize, arr);
                    for (int i = 0; i < 8; i++)
                    {
                        sbuff[i + 5] = fsizeArr[i];
                    }

                    delete [] name;          
                    send(cl, sbuff, 13, 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_DP:
                {
                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    // printf("fileLen is: %d\n", fileLen);
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                    }                    

                    // operr = operation err
                    int operr = DumpVar((char*)name);

                    // put client identifier into the send bufferfr
                    uint8_t arr[4];
                    uint8_t *cidArr = convertToArray(cid,arr);
                    
                    // puts the client ID into the send buffer
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = operr;

                    delete [] name;               
                    send(cl, sbuff, 5, 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                case OP_LD:
                {
                    printf("------you entered load\n");
                    uint16_t fileLen = getFileLen(rbuff);
                    // shift buffer since we are done finding the file length
                    rbuff += 2;
                    printf("fileLen is: %d\n", fileLen);
                    // changes uint8_t* fileName into char* name                    
                    uint8_t *name = new uint8_t[fileLen];
                    uint8_t *fileName = getFileName(rbuff, fileLen, name);
                    // shift buffer by fileLen because we found the file name length
                    rbuff += fileLen;

                    for (int i = 0; i < fileLen; i++)
                    {
                        name[i] = fileName[i];
                        printf("name[%d]: %c\n", i, name[i]);
                    }

                    // operr = operation err
                    int operr = LoadVar((char*)name);
                    printf("operr is: %d\n", operr);
                    
                    // put client identifier into the send bufferfr
                    uint8_t arr[4]; 
                    uint8_t *cidArr = convertToArray(cid,arr);
                    
                    // puts the client ID into the send buffer
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = operr;

                    delete [] name;              
                    send(cl, sbuff, 5, 0); // send out to client
                    close(cl);
                    bzero(sbuff, 8192);
                    bzero(rbuff, 8192);
                    break;
                }
                default:
                {   
                    // put client identifier into the send bufferfr
                    uint8_t arr[4]; 
                    uint8_t *cidArr = convertToArray(cid,arr);
                    
                    // puts the client ID into the send buffer
                    for (int i = 0; i < 4; i++)
                    {
                        sbuff[i] = cidArr[i];
                    }

                    sbuff[4] = 95;
                }
            }
            
            // results in "free(): invalid pointer"
            // printf("freeing rbuff\n");
            // free(rbuff);
            // printf("freed rbuff\n");
            // // resulting in "free(): invalid pointer"
            // printf("freeing sbuff\n");
            // free(sbuff);
            // printf("freed sbuff\n");
        // }
        close(cl);

    }

    // printf("6 Made it to this line with no issues\n");
    // code doesn't seem to make it here
} 