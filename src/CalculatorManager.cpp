#include <iostream>
#include "CalculatorManager.h"

using namespace std;

CalculatorManager::CalculatorManager():ptr_calc_(NULL){}

CalculatorManager::CalculatorManager(Calculator& calc):ptr_calc_(&calc){}

CalculatorManager::~CalculatorManager(){}

void CalculatorManager::SetCalculator(Calculator& calc){
    this->ptr_calc_ = &calc;
}

string CalculatorManager::Calculate(const string& params){
    if( ptr_calc_ != NULL ){ 
        return ptr_calc_->Calculate(params);
    } else {
        return "{}";
    }
}
