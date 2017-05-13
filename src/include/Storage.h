/*
 * responsible for reading database
 */
#ifndef _STORAGE_H
#define _STORAGE_H

#include <vector>
#include <string>

using namespace std;

class Storage {
    public:
        static vector<string> GetStockCodeList();
        static vector<double> GetOneStockRecent(const string& kCode, const int kDay_len);
    private:
        static bool flag_init_;
        static void InitIfNot();
};
#endif
