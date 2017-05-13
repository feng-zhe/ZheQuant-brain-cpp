#ifndef _HELPER_H
#define _HELPER_H
#include <string>
#include <vector>

using namespace std;

class Helper{
    public:
        static vector<string> split_cmd(const string& cmd);
};
#endif
