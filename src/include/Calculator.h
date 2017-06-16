#ifndef _CALCULATOR_H
#define _CALCULATOR_H
#include <string>

using namespace std;

class Calculator {
    public:
        // calculate the result according to the simple string args
        virtual string Calculate(const string&) = 0;

    protected:
        // return the tag for debug
        virtual string GetTag() = 0;
};
#endif
