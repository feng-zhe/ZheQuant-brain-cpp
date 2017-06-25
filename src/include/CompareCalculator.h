/*
 * calculate the moving average of the stock
 */
#ifndef _COMPARECALCULATOR_H
#define _COMPARECALCULATOR_H

#include <string>
#include <chrono>
#include <vector>
#include "Calculator.h"

using namespace std;

class CompareCalculator : public Calculator {

    public:
        CompareCalculator();
        ~CompareCalculator();
        string Calculate(const string&) override;
        string GetCalcType() override;

    protected:
        string GetTag() override;

    private:
        // private struct
        struct CmdParams {
            // TODO: how to represent the composit of stocks? Need confirmation from expert
            chrono::system_clock::time_point begin_tp;
            chrono::system_clock::time_point end_tp;
            string target_code; // the code of target stock
        };

        // parse the parameters
        auto ParseParams(const string& kParams) -> CmdParams;
        // get price of one stock
        vector<double> GetOneStockData(const string& kCode, const chrono::system_clock::time_point kBegin_tp, const chrono::system_clock::time_point kEnd_tp);
};

#endif
