/*
 * calculate the moving average of the stock
 */
#ifndef _RATEOFRETURNCALCULATOR_H
#define _RATEOFRETURNCALCULATOR_H

#include <string>
#include <chrono>
#include <vector>
#include <map>
#include "Calculator.h"

using namespace std;

class RateOfReturnCalculator : public Calculator {

    public:
        RateOfReturnCalculator();
        ~RateOfReturnCalculator();
        string Calculate(const string&) override;
        string GetCalcType() override;

    protected:
        string GetTag() override;

    private:
        // private struct
        struct CmdParams {
            chrono::system_clock::time_point begin_tp;
            chrono::system_clock::time_point end_tp;
            map<string,int> composition;
        };

        // parse the parameters
        auto ParseParams(const string& kParams) -> CmdParams;
        // get price of one stock
        vector<double> GetOneStockData(const string& kCode, const chrono::system_clock::time_point kBegin_tp, const chrono::system_clock::time_point kEnd_tp);
};

#endif
