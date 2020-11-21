#include "conversion.h"

// converts uint16_t value to a uint8_t array
uint8_t *convertToArray(uint16_t value, uint8_t* arr)
{
    //uint8_t *arr = (uint8_t *)malloc(sizeof(uint8_t) * 2);
    arr[1] = value & 0xff;
    arr[0] = (value >> 8) & 0xff;
    return arr;
}

// converts uint32_t value to a uint8_t array
uint8_t *convertToArray(uint32_t value, uint8_t* arr)
{
    //uint8_t *arr = (uint8_t *)malloc(sizeof(uint8_t) * 4);
    arr[3] = value & 0xff;
    arr[2] = (value >> 8) & 0xff;
    arr[1] = (value >> 16) & 0xff;
    arr[0] = (value >> 24) & 0xff;
    return arr;
}

// converts uint64_t value to a uint8_t array
uint8_t *convertToArray(uint64_t value, uint8_t* arr)
{
    //uint8_t *arr = (uint8_t *)malloc(sizeof(uint8_t) * 8);
    arr[7] = value & 0xff;
    arr[6] = (value >> 8) & 0xff;
    arr[5] = (value >> 16) & 0xff;
    arr[4] = (value >> 24) & 0xff;
    arr[3] = (value >> 32) & 0xff;
    arr[2] = (value >> 40) & 0xff;
    arr[1] = (value >> 48) & 0xff;
    arr[0] = (value >> 56) & 0xff;
    return arr;
}

// converts int64_t value to a uint8_t array
uint8_t *convertToArray(int64_t value, uint8_t* arr)
{
    //uint8_t *arr = (uint8_t *)malloc(sizeof(uint8_t) * 8);
    arr[7] = value & 0xff;
    arr[6] = (value >> 8) & 0xff;
    arr[5] = (value >> 16) & 0xff;
    arr[4] = (value >> 24) & 0xff;
    arr[3] = (value >> 32) & 0xff;
    arr[2] = (value >> 40) & 0xff;
    arr[1] = (value >> 48) & 0xff;
    arr[0] = (value >> 56) & 0xff;
    return arr;
}

// converts uint8_t array into uint16_t number
uint16_t convertFromArray16(uint8_t *arr)
{
    uint16_t result = arr[1] + (arr[0] << 8);
    return result;
}

// converts uint8_t array into uint32_t number
uint32_t convertFromArray32(uint8_t *arr)
{
    uint32_t result = arr[3] + (arr[2] << 8) + (arr[1] << 16) + (arr[0] << 24);
    return result;
}

// converts uint8_t array into uint64_t number
uint64_t convertFromArray64(uint8_t *arr)
{
    uint64_t result = 0;
    for (int i = 0; i < 8; i++)
    {
        result = result << 8;
        result += arr[i];
    }
    return result;
}

// converts uint8_t array into uint64_t number
uint64_t convertFromArrayU64(uint8_t *arr)
{
    int64_t result = 0;
    for (int i = 0; i < 8; i++)
    {
        result = result << 8;
        result += arr[i];
    }
    return result;
}

int64_t convertFromArray64S(uint8_t *arr){
    int64_t result = 0;
    for(int i = 0; i < 8; i++){
        result = result << 8;
        result += arr[i];
    }
    return result;
}

// testing a way to parse data with uint8 array
std::string convertFromArrayS(uint8_t *arr)
{
    std::string result = "";

    for (int i = 0; i < arr[0]; i++)
    {
        result += (char)arr[i + 1];
    }
    // Delete this:
    result += "\n";
    return result;
}
