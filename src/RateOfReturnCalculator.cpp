#include "RateOfReturnCalculator.h"

RateOfReturnCalculator::RateOfReturnCalculator() {}
RateOfReturnCalculator::~RateOfReturnCalculator() {}

string RateOfReturnCalculator::Calculate(const string& kParams){
    // TODO
    return "";
}

string RateOfReturnCalculator::GetCalcType(){
    return "rot";
}

string RateOfReturnCalculator::GetTag(){
    return "[compare calculator]";
}

auto RateOfReturnCalculator::ParseParams(const string& kParams) -> CmdParams {
    // the expected params shoud be like:
    // -b 2016-08-12 -e 2017-08-12 -c {00001SZ:2, 00002SZ:1, 00003SZ:10}
    // begin on 2016-08-12, end on 2018-08-12
    // parse the params
    cout << GetTag() << "Parsing the parameters in command" << endl;
    vector<string> cmd_strs = Helper::split_cmd(kCmd);
    string begin_str, end_str, comp_str;
    for(std::vector<string>::size_type i = 0; i != cmd_strs.size(); ++i){
        if(cmd_strs[i] == "-b"){
            begin_str = cmd_strs[++i];
        } else if(cmd_strs[i] == "-e"){
            end_str = cmd_strs[++i];
        } else if(cmd_strs[i] == "-c"){
            comp_str = cmd_strs[++i];
        }
    }
    CmdParams rst;
    // TODO: fill the result structure
    //cout << GetTag() << " begin on " << begin_str 
         //<< begin << " milliseconds" << endl;
    //cout << GetTag() << " end on " << end_str 
         //<< end << " milliseconds" << endl;
    return rst;
}
vector<double> GetOneStockRecent(   const string& kCode, 
                                    const chrono::system_clock::time_point kBegin_tp,
                                    const chrono::system_clock::time_point kEnd_tp){
    vector<double> rst;
    // TODO
    return rst;
}
