#include "operations.h"

// add function
int64_t add(int64_t a, int64_t b)
{
    if (a >= INT64_MAX - b)
    {
        perror("Add failed\n");
        return EINVAL;
    }
    return a + b;
}

// sub function
int64_t sub(int64_t a, int64_t b)
{
    if (a <= INT64_MIN + b)
    {
        perror("Sub failed\n");
        return EINVAL;
    }

    return a - b;
}

// mul function
int64_t mul(int64_t a, int64_t b)
{
    int64_t x = a * b;
    if(a != 0 && x / a != b){
        printf("Mul failed\n");
        return EINVAL;
    }
    return x;
}

int64_t divi(int64_t a, int64_t b){
    if(b != 0){
        int64_t x = a / b;
        if(x * b == a){
            return x;
        }
    }
    printf("Div failed\n");
    return EINVAL;
}

int64_t mod(int64_t a, int64_t b){
    if(a != 0 && b != 0){
        int64_t x = a % b;
        return x;
    }
    printf("Mod failed\n");
    return EINVAL;
}

#include <stdio.h>