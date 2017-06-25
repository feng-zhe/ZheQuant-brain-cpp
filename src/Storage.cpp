/*
 * This file contains function interacting with database
 * Please be aware that every function may raise exception when data is 
 * to huge.
 */

#include <iostream>
#include <vector>
#include <string>
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "Configure.h"
#include "Storage.h"

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

bool Storage::flag_init_ = false;

void Storage::InitIfNot(){
    if(!flag_init_){
        flag_init_ = true;
        mongocxx::instance instance{}; // This should be done only once.
    }
}

vector<string> Storage::GetStockCodeList(){
    // get database
    InitIfNot(); // init mongocxx
    mongocxx::uri uri(DB_URL); 
    mongocxx::client client(uri); // connect db
    if(!client){
        throw std::invalid_argument("[ERROR] database collecting failed");
    }
    mongocxx::database db = client["fin"];
    // get stock code lists
    mongocxx::pipeline stages;
    document group_stage;
    group_stage << "_id"
                << "$code";
    stages.group(group_stage.view());
    auto cursor = db["stocks"].aggregate(stages);
    // loop and write to result
    vector<string> res;
    for(auto& doc : cursor) {
        if(doc["_id"] && doc["_id"].type() == bsoncxx::type::k_utf8){
            res.push_back(doc["_id"].get_utf8().value.to_string());
        }
    }
    return res;
}

vector<double> Storage::GetOneStockRecent(const string& kCode, const int kDay_len){
    // get database
    InitIfNot(); // init mongocxx
    mongocxx::uri uri(DB_URL); 
    mongocxx::client client(uri); // connect db
    if(!client){
        throw std::invalid_argument("[ERROR] database collecting failed");
    }
    mongocxx::database db = client["fin"];
    // get stock code lists
    document filter, sort;
    filter << "code" << kCode;
    sort << "date" << -1;
    mongocxx::options::find opts;
    opts.sort(sort.view());
    opts.limit(kDay_len);
    auto cursor = db["stocks"].find(filter.view(), opts);
    // loop and write to result
    vector<double> res;
    for(auto& doc : cursor) {
        if(doc["close_price"] && doc["close_price"].type() == bsoncxx::type::k_double){
            res.push_back(doc["close_price"].get_double());
        }
    }
    return res;
}
