#include <iostream>
#include <new>
#include "MovingAverageCalculator.h"
#include "CompareCalculator.h"
#include "CalculatorManager.h"
#include "Helper.h"

using namespace std;

CalculatorManager::CalculatorManager(){}

CalculatorManager::~CalculatorManager(){}

string CalculatorManager::Calculate(const string& cmd){
    const string kStr_dbg = "[CalcMgr]";
    // split the cmd
    vector<string> cmd_strs = Helper::split_cmd(cmd);
    // find the calc type and params
    string calc_type, params;
    for(size_t i=0; i<cmd_strs.size()-1; ++i){
        // check if it is the calc_type or params
        if(cmd_strs[i]=="-p"){
            params = cmd_strs[i+1];
        } else if(cmd_strs[i]=="-t"){
            calc_type = cmd_strs[i+1];
        }
        // break if we find both
        if(!params.empty() && !calc_type.empty()) break;
    }
    // get calculators
    auto vec_calc = GetAllCalcOnHeap();
    // find the right calculator
    Calculator* ptr_calc = NULL;
    cout<< kStr_dbg <<" choosing the calculator with type " << calc_type << endl;
    for(auto iter=vec_calc.begin(); iter!=vec_calc.end(); ++iter){
        auto ptr_temp = *iter;
        if(ptr_temp==NULL) continue;
        if(ptr_temp->GetCalcType() == calc_type){
            ptr_calc = ptr_temp;
            cout<< kStr_dbg <<" found the calculator" << endl;
            break;
        }
    }
    string str_rst = "{}";
    if( ptr_calc == NULL ){ 
        cout<< kStr_dbg <<" no calculator found" << endl;
    } else {
        str_rst = ptr_calc->Calculate(params);
    }
    // free calculators
    DeleteAllCalcOnHeap(vec_calc);
    // return result
    return str_rst;
}

vector<Calculator*> CalculatorManager::GetAllCalcOnHeap(){
    vector<Calculator*> vec_calc;
    // TODO: add other plugins
    vec_calc.push_back(new (std::nothrow) MovingAverageCalculator);
    vec_calc.push_back(new (std::nothrow) CompareCalculator);
    // end of adding plugins
    return vec_calc;
}

void CalculatorManager::DeleteAllCalcOnHeap(const vector<Calculator*> & vec_calc){
    for(Calculator* ptr_calc : vec_calc){
        delete ptr_calc;
    }
}
