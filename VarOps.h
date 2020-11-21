#ifndef _VAROPS_H_
#define _VAROPS_H_
#include "VarTable.h"
#include <thread>
#include <mutex>  
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int GetVar(const std::string&, int64_t&);
int SetVar(const std::string&, const int64_t&);
int DelVar(const std::string&);
int LoadVar(std::string);
int DumpVar(std::string);

#endif