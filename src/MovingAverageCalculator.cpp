#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include "MovingAverageCalculator.h"
#include "Storage.h"
#include "structs.h"
#include "Helper.h"
#include "Configure.h"

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


MovingAverageCalculator::MovingAverageCalculator(){}
MovingAverageCalculator::~MovingAverageCalculator(){}

string MovingAverageCalculator::GetCalcType(){
    return "mv-avg";
}

string MovingAverageCalculator::Calculate(const string& kCmd){
    const string kStr_dbg = GetTag();
    // parse the cmd params
    CmdParams cmd_params = ParseParams(kCmd);
    const int kDay_len = cmd_params.day_len;
    // get the stocks list
    auto code_list = GetStockCodeList();
    // query each stock and added into priority queue, the top is the biggest
    // implement 'less' logic 
    auto cmp = [](  const pair<string,double> & a,
                    const pair<string,double> & b){
        return a.second < b.second;
    };
    priority_queue<pair<string,double>,vector<pair<string,double>>,decltype(cmp)> ma_pq(cmp);
    for(auto&& code : code_list){
        auto vec_price = GetOneStockRecent(code, kDay_len);
        if(vec_price.size() != kDay_len) continue; // the stock doesn't contain enough records
        double diff = CalcDiff(vec_price);
        ma_pq.push(make_pair(code, diff));
    }
    // write the result as a json string
    document doc{};
    auto arr = doc << "list" << open_array;
    int limit = 0;
    while(!ma_pq.empty() && limit++ < cmd_params.num_stock){
        auto elem = ma_pq.top();
        arr << open_document 
                << "code"   << elem.first 
                << "gap"    << elem.second 
            << close_document;
        ma_pq.pop();
    }
    arr << close_array;
    cout << kStr_dbg << " calculation finished" << endl;
    return bsoncxx::to_json(doc);
}

string MovingAverageCalculator::GetTag(){
    return "[moving average]";
}

auto MovingAverageCalculator::ParseParams(const string& kCmd) -> CmdParams {
    // the expected params shoud be like:
    // -d 20 -n 5
    // parse the params
    const string kStr_dbg = GetTag();
    cout << kStr_dbg << " Parsing the parameters in command" << endl;
    vector<string> cmd_strs = Helper::split_cmd(kCmd);
    int day_len = 20, num_stock = 10; // defaults
    for(std::vector<string>::size_type i = 0; i != cmd_strs.size(); ++i){
        if(cmd_strs[i] == "-d"){
            day_len = stoi(cmd_strs[++i]);
        } else if(cmd_strs[i] == "-n"){
            num_stock = stoi(cmd_strs[++i]);
        }
    }
    cout << kStr_dbg << " day length: "<< day_len << endl;
    cout << kStr_dbg << " number of stocks: "<< num_stock << endl;
    cout << kStr_dbg << " Parsing done" << endl;
    return (CmdParams){day_len, num_stock};
}

vector<string> MovingAverageCalculator::GetStockCodeList(){
    const string kStr_dbg = GetTag();
    cout << kStr_dbg << " Retrieving stock code list" << endl;
    vector<string> res;
    try{
        res = Storage::GetStockCodeList();
    } catch(mongocxx::query_exception &e){
        cout << kStr_dbg << " [ERROR]: " << e.what() << endl;
    } catch(...){
        cout << kStr_dbg << " [ERROR] Unknown exception" << endl;
    }
    cout << kStr_dbg << " get stock list size: " << res.size() << endl;
    return res;
}

vector<double> MovingAverageCalculator::GetOneStockRecent(  const string& kCode,
                                                            const int kDay_len){
    const string kStr_dbg = GetTag();
    vector<double> res;
    try{
        res = Storage::GetOneStockRecent(kCode, kDay_len);
    } catch(mongocxx::query_exception &e){
        cout << kStr_dbg << " [ERROR]: " << e.what() << endl;
    } catch(...){
        cout << kStr_dbg << " [ERROR] Unknown exception" << endl;
    }
    return res;
}

double MovingAverageCalculator::CalcDiff(const vector<double>& vec){
    if(vec.empty()){
        throw std::invalid_argument("input vec shouldn't be empty!");
    }
    double sum = 0;
    for(auto d : vec){
        sum += d;
    }
    return sum/vec.size() - vec[0];
}
