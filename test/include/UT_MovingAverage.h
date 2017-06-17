#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "MovingAverageCalculator.h"

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

TEST(TestCaseMovingAverage, TestPublicFunctions){
    MovingAverageCalculator calc;
    const string test_str = "schedule -t mv_avg -d 20 -n 5"; // the test input
    string result = calc.Calculate(test_str);
    // expected result should be: 
    // { "list" : [ { "code" : "test_code_7", "gap" : 7.0594777444822511825 }, { "code" : "test_code_4", "gap" : 4.7916185044330319442 }, { "code" : "test_code_1", "gap" : 4.3451060959892231494 }, { "code" : "test_code_5", "gap" : 0.98145648942745822296 }, { "code" : "test_code_0", "gap" : -1.3572502590689623503 } ] }
    ASSERT_FALSE(result.empty());
    auto rst_val = bsoncxx::from_json(result);
    auto rst_view = rst_val.view();
    ASSERT_TRUE(rst_view["list"]);
    ASSERT_TRUE(rst_view["list"].type()==bsoncxx::type::k_array);
    auto arr_view = rst_view["list"].get_array().value;
    // assert the code of result
    string expt_codes[] = {"test_code_7", "test_code_4", "test_code_1", "test_code_5", "test_code_0"};
    for(int i=0; i<5; i++){
        ASSERT_TRUE(arr_view[i].type()==bsoncxx::type::k_document);
        auto ele_view = arr_view[i].get_document().value;
        ASSERT_TRUE(ele_view["code"]);
        ASSERT_TRUE(ele_view["code"].type()==bsoncxx::type::k_utf8);
        ASSERT_EQ(ele_view["code"].get_utf8().value.to_string(), expt_codes[i]);
    }
}
