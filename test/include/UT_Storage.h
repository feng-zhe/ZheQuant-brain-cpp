#include "Storage.h"

TEST(TestCaseStorage, TestFind){
    cout<< "TODO" << endl;
}

TEST(TestCaseStorage, TestDistinct){
    cout<< "TODO" << endl;
}

//TEST(TestCaseStorage, TestGetRecentRecords){
    //// test the input
    //auto stock_map= Storage::GetRecentRecords(20);
    //for(auto&& kv : stock_map){
        //ASSERT_LE(kv.second.size(),20);
    //}
    //// the result should be sorted from code ascending and date descending
    //for(auto&& kv : stock_map){
        //auto vec = kv.second;
        //if(vec.size()<=1){
            //continue;
        //}
        //for(int i=1; i<vec.size();++i){
            //ASSERT_GT(vec[i-1].date,vec[i].date)
                //<< "stock code of first one is " <<vec[i-1].code << endl
                //<< "stock code of second one is " <<vec[i].code << endl;
        //}
    //}
//}
