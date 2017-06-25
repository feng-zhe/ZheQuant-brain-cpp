/*
 * calculate the moving average of the stock
 */
#ifndef _MOVINGAVERAGECALCULATOR_H
#define _MOVINGAVERAGECALCULATOR_H
#include <vector>
#include <map>
#include "Calculator.h"

using namespace std;

class MovingAverageCalculator : public Calculator {

    public:
        MovingAverageCalculator();
        ~MovingAverageCalculator();
        string Calculate(const string&) override;
        string GetCalcType() override;

    protected:
        string GetTag() override;

    private:
        // private struct
        struct CmdParams {
            int day_len;
            int num_stock;
        };

        // parse the command string 
        CmdParams ParseParams(const string& kParams);
        // get the stock code lists
        vector<string> GetStockCodeList();
        // get price of one stock in recent days
        vector<double> GetOneStockRecent(const string& kCode, const int kDay_len);
        // calculate the difference by (average - last price)
        double CalcDiff(const vector<double>& vec);
};
#endif
