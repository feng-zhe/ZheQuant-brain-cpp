/*
 * CalculatorManager will create and call correspnding calculator depending on the input json string
 */
#ifndef _CALCULATORMANAGER_H
#define _CALCULATORMANAGER_H
#include <string>
#include "Calculator.h"

using namespace std;

class CalculatorManager {

    private:
        Calculator *ptr_calc_ = NULL;

    public:
        CalculatorManager();
        virtual ~CalculatorManager();
        CalculatorManager(Calculator&);
        void SetCalculator(Calculator&);
        string Calculate(const string&);
};
#endif
