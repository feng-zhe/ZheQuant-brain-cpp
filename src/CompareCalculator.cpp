#include "CompareCalculator.h"

CompareCalculator::CompareCalculator() {}
CompareCalculator::~CompareCalculator() {}

string CompareCalculator::Calculate(const string& kParams){
    // TODO
    return "";
}

string CompareCalculator::GetCalcType(){
    // TODO
    return "comp";
}

string CompareCalculator::GetTag(){
    return "[compare calculator]";
}

auto CompareCalculator::ParseParams(const string& kParams) -> CmdParams {
    CmdParams rst;
    // TODO
    return rst;
}
vector<double> GetOneStockRecent(   const string& kCode, 
                                    const chrono::system_clock::time_point kBegin_tp,
                                    const chrono::system_clock::time_point kEnd_tp){
    vector<double> rst;
    // TODO
    return rst;
}
