#ifndef _CONVERSION_H_
#define _CONVERSION_H_
#include <fcntl.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

// converts uint8_t array into uint16_t number
uint16_t convertFromArray16(uint8_t *arr);
// converts uint16_t value to a uint8_t array
uint8_t *convertToArray(uint16_t value, uint8_t* arr);

// converts uint8_t array into uint32_t number
uint32_t convertFromArray32(uint8_t *arr);
// converts uint32_t value to a uint8_t array
uint8_t *convertToArray(uint32_t value, uint8_t* arr);

// converts uint8_t array into uint64_t number
uint64_t convertFromArray64(uint8_t *arr);

int64_t convertFromArray64S(uint8_t *arr);

// converts uint64_t value to a uint8_t array
uint8_t *convertToArray(uint64_t value, uint8_t* arr);

// converts int64_t value to a uint8_t array
uint8_t *convertToArray(int64_t value, uint8_t* arr);

// TODO: String conversion functions. (From/To uint array)
// testing a way to parse data with uint8 array
std::string convertFromArrayS(uint8_t *arr);
#endif