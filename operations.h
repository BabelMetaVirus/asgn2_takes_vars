#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include <err.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
// add function
int64_t add(int64_t a, int64_t b);

// sub function
int64_t sub(int64_t a, int64_t b);

// mul function
int64_t mul(int64_t a, int64_t b);

// div fucntion
int64_t divi(int64_t a, int64_t b);

// mod function
int64_t mod(int64_t a, int64_t b);
#endif