/*
 * define our structures
 */
#ifndef _STRUCTS_H
#define _STRUCTS_H
#include <string>

using namespace std;

struct Stock {
    string code;
    int64_t date; // the milliseconds
    double open_price;
    double close_price;
};
#endif
