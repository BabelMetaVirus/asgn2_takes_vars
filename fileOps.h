#ifndef _FILEOPS_H_
#define _FILEOPS_H_
#include <fcntl.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// read file opereation
bool readFile(const char *fileName, uint64_t offset, uint16_t buffsize,
                            const uint8_t *readBuff, int &code);

// write to file function
bool writeFile(const char *fileName, uint64_t offset, uint16_t buffsize,
                            const uint8_t *readBuff, int &code);

// create a new file with 0 bytes called fileName
bool createFile(const char *fileName, int &code);

// gets the file size of the filename by calling stat and returns an errno via code
bool getFileSize(const char *fileName, int &code, int &fsize);

#endif