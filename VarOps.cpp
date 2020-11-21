#include "VarOps.h"

// Since we won't have a global Variable Table, the read/write/delete operations
// Will each need to take a variable Table Reference.
// Then we take a string reference (variable name)
// Using references because both of these objects can be big.

// Then a value (as reference for writing) 
// This value either holds the value we are assigning, or 
// holds the value we are retrieving.
// Example:
std::mutex mtx;
VarTable* dataStore;

int GetVar(const std::string& _name, int64_t& val){
    // Here we would mutex lock the variable table.
    int en = 0; // e for erorr num
    mtx.lock();
    // Then check to see if the variable is contained within the table 
    if(dataStore->GetVar(_name, val)) {
        // If there is no error, get the variable.
        en = 0; // sets en to 0 for no error
    }
    else {
        // Generating an error code if need be.
        en = ENOENT;
    }
    // Unlock the table
    mtx.unlock();
    // Return an error code if need be.
    return en;
}

int SetVar(const std::string& _name, const int64_t& _val){
    int err = -1;
    // mutex lock the variable table
    mtx.lock();
    if(dataStore->SetVar(_name, _val)){
        err = 0;
    }
    mtx.unlock();
    return err;
}

int DelVar(const std::string& _name) {
    int en = 0; // e for erorr num
    // mutex lock the variable table
    mtx.lock();
    if(dataStore->DelVar(_name)) {
        // returns 0 for no error
        en = 0; // sets en to 0 for no error
    }
    else {
        // returns undefined variable as specified in pdf
        en = ENOENT;
    }

    mtx.unlock();
    return en;

}

int LoadVar(std::string _fileName) {
    char line[100];
    char name[32];
    int64_t val;

    // attempts to open the file
    int fread = open(_fileName.c_str(), O_RDONLY);
    // check error code from fread
    if(fread < 0) {
        printf("error calling open()\n");
        return errno;
    }

    // create file pointer
    FILE *file = fdopen(fread, "r");
    // check if the file pointer exists
    if(file == NULL) {
        printf("error opening file\n");
        return errno;
    }
    VarList vars;
    // gets the string name and value from the file according to the format and adds it to the table (?)
    while( fscanf(file, "%s\n", line) != EOF ) {
        for(size_t i = 0; i < strlen(line); i++){
            if(line[i] == '='){
                line[i] = ' ';
                break;
            }
        }
        sscanf(line, "%s %ld", name, &val);
        vars.AddVar(name, val);
    }
    // close file and return 0 for success
    fclose(file);
    
    int varCount = vars.Size();
    VarEntry* varArray = new VarEntry[varCount];
    vars.GetVarData(varArray);
    mtx.lock();
    dataStore->SetVarData(varArray, varCount);
    mtx.unlock();
    delete [] varArray;
    return 0;

}

int DumpVar(std::string _fileName) {
    // Attempts to open the file and if it doesnt exist, create it
    int fread = open(_fileName.c_str(), O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    // check file descriptor
    if(fread < 0) {
        printf("error calling open()\n");
        return errno;
    }

    // create file pointer
    FILE *file = fdopen(fread, "w");
    // check if it exists
    if(file == NULL) {
        printf("error calling fdopen\n");
        return errno;
    }

    mtx.lock();
    // go through the table and print "name=value"
    int entrySize = dataStore->Size();
    VarEntry* entries = new VarEntry[entrySize]; // ??
    dataStore->GetVarData(entries);
    mtx.unlock();
    for(int i=0; i < entrySize; i++) {
        fprintf(file, "%s=%ld\n", entries[i].name.c_str(), entries[i].value );
    }
    delete [] entries;
    // close file and return 0 for success
    fclose(file);
    return 0;
}