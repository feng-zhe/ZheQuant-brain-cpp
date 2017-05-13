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
        if(doc["CLOSE"] && doc["CLOSE"].type() == bsoncxx::type::k_double){
            res.push_back(doc["CLOSE"].get_double());
        }
    }
    return res;
}

/*
 * Retrieve all stocks data in recent days.
 * Beware that db could throw exception when data is too huge.
 * Considering currently the pipeline framework in mongodb cannot meet th
 * need in this case, I comment out this function.
 */
//map<string, vector<Stock>> Storage::GetRecentRecords(int day_len){
    //// init mongocxx
    //InitIfNot();
    //mongocxx::uri uri(DB_URL); 
    //mongocxx::client client(uri);
    //mongocxx::database db_fin = client["fin"];
    //// prepare options
    //mongocxx::options::find opts;
    //opts.sort(document{}    << "code" << 1 
                            //<< "date" << -1
                            //<< finalize);
    //// start query. 
    //map<string,vector<Stock>> stock_map;
    //try{
        //auto cursor = db_fin["stocks"].find(document{} << finalize, opts); // this could raise exception if the data is too huge
        //int cnt = 0;
        //for(auto& doc : cursor) {
            //Stock stk;
            //// code
            //if(doc["code"] && doc["code"].type() == bsoncxx::type::k_utf8){
                //stk.code = doc["code"].get_utf8().value.to_string();
            //} else {
                //continue;
            //}
            //// day_len limitation
            //if(stock_map[stk.code].size() >= day_len){
                //continue;
            //}
            //// date (as int64_t)
            //if(doc["date"] && doc["date"].type() == bsoncxx::type::k_date ){
                //stk.date = int64_t(doc["date"].get_date());
            //} else {
                //continue;
            //}
            //// OPEN
            //if(doc["OPEN"] && doc["OPEN"].type() == bsoncxx::type::k_double){
                //stk.open_price = doc["OPEN"].get_double();
            //} else {
                //continue;
            //}
            //// CLOSE
            //if(doc["CLOSE"].type() == bsoncxx::type::k_double){
                //stk.close_price = doc["CLOSE"].get_double();
            //} else {
                //continue;
            //}
            //stock_map[stk.code].push_back(stk);
    //}
    //return stock_map;
    //} catch(mongocxx::query_exception &e){
        //cout<< "db query exception: " << e.what() <<endl;
        //return stock_map;
    //}
//}
