/*
 * This class is used to calculate the moving average of the stock
 */
#ifndef _MOVINGAVERAGECALCULATOR_H
#define _MOVINGAVERAGECALCULATOR_H
#include <vector>
#include <map>
#include "Calculator.h"
#include "structs.h"

using namespace std;

struct CmdParams {
    int day_len;
    int num_stock;
};

class MovingAverageCalculator : public Calculator {

    public:
        MovingAverageCalculator();
        virtual ~MovingAverageCalculator();
        string Calculate(const string&) override;

    protected:
        string GetTag() override;

    private:
        // parse the command string 
        CmdParams ParseCmd(const string& kCmd);
        // get the stock code lists
        vector<string> GetStockCodeList();
        // get price of one stock in recent days
        vector<double> GetOneStockRecent(const string& kCode, const int kDay_len);
        // calculate the difference by (average - last price)
        double CalcDiff(const vector<double>& vec);
};
#endif
