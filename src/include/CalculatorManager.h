/*
 * CalculatorManager will create and call correspnding calculator depending on the input json string
 */
#ifndef _CALCULATORMANAGER_H
#define _CALCULATORMANAGER_H
#include <string>
#include <vector>
#include "Calculator.h"

using namespace std;

class CalculatorManager {

    public:
        CalculatorManager();
        virtual ~CalculatorManager();
        string Calculate(const string&);

    private:
        vector<Calculator*> GetAllCalcOnHeap();
        void DeleteAllCalcOnHeap(const vector<Calculator*> &);
};
#endif
