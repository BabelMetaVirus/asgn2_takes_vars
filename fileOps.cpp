#include "fileOps.h"

// read file opereation
bool readFile(const char *fileName, uint64_t offset, uint16_t buffsize,
                            const uint8_t *readBuff, int &code)
{
    // Attempt to open the file
    int fread = open(fileName, O_RDWR);
    if (fread < 0)
    {
        perror("Could not open file");
        printf("Open = Error Number is: %d\n", errno);
        code = errno;
        close(fread);
        return false;
    }
    else
    {
        // Seek to the offset from the start of the file.
        int loc = lseek(fread, offset, SEEK_CUR);
        // If the return is not equal to the offset, we've screwed up.
        if (loc < 0)
        {
            code = errno;
            printf("lseek = Error Number is: %d\n", errno);
            close(fread);
            return false;
        }
        // Then we read in the file.
        int amtRead = read(fread, (void *)readBuff, buffsize);
        close(fread);
        // Then we close the file.
        if (amtRead < 0)
        {
            code = errno;
            printf("Amtread = Error Number is: %d\n", errno);
            close(fread);
            return false;
        }
        else
        {
            code = amtRead;
            return true;
        }
    }
}

// write to file function
bool writeFile(const char *fileName, uint64_t offset, uint16_t buffsize,
                            const uint8_t *readBuff, int &code) 
{
    // Attempt to open the file
    /*
    for(int i=0; i < strlen(fileName);i++){
        printf("fileName[%d] is: %c\n", i, fileName[i]);
    } 
    */
    int fwrite = open(fileName, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
    if (fwrite < 0)
    {
        perror("Could not open file");
        printf("Open = Error Number is: %d\n", errno);
        code = errno;
        close(fwrite);
        return false;
    }
    else
    {
        // Then we write to the file.
        int amtWrite = write(fwrite, (void *)(readBuff+ offset), buffsize);
        close(fwrite);
        // Then we close the file.
        if (amtWrite < 0)
        {
            code = errno;
            printf("Amtread = Error Number is: %d\n", errno);
            close(fwrite);
            return false;
        }
        else
        {
            code = amtWrite;
            return true;
        }
    }

}

// create a new file with 0 bytes called fileName
bool createFile(const char *fileName, int &code)
{
    // Attempts to open the file and if it doesnt exist, create it
    int fread = open(fileName, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    if (fread < 0)
    {
        perror("Could not open file");
        printf("Open - Error Number is: %d\n", errno);
        code = errno;
        close(fread);
        return false;
    }
    
    return false;
}

// gets the file size of the filename by calling stat and returns an errno via code
bool getFileSize(const char *fileName, int &code, int &fsize)
{
    // Attempt to open the file
    int fread = open(fileName, O_RDONLY, S_IRUSR | S_IRGRP);
    if (fread < 0)
    {
        perror("Could not open file");
        printf("Open - Error Number is: %d\n", errno);
        code = errno;
        close(fread);
        return false;
    }
    else
    {
        struct stat statbuf;
        if (stat(fileName, &statbuf) < -1)
        {
            printf("%s failed to get file size\n", fileName);
            printf("Stat - Error Number is: %d\n", errno);
            code = errno;
            return false;
        }

        fsize = statbuf.st_size;
    }
    
    return false;
}