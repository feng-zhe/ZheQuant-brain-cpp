#include <ev.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "MyLibEvHandler.h"
#include "CalculatorManager.h"
#include "MovingAverageCalculator.h"
#include "Configure.h"
#include "Helper.h"

using namespace std;
using namespace rapidjson;

int main()
{
    const string QUEUE_TODO = "jobs-todo";
    const string QUEUE_DONE= "jobs-done";
    const string EXCHG_DONE= "jobs-done";
    const string ROUTING_KEY_DONE= "cbrain-done";

    do{
        // access to the event loop
        auto *loop = EV_DEFAULT;
        // event handler
        MyLibEvHandler handler(loop);
        // make a connection
        AMQP::TcpConnection connection(&handler, AMQP::Address(MQ_URL));
        // we need two channels for sending and receiving
        AMQP::TcpChannel ch_recv(&connection);
        AMQP::TcpChannel ch_send(&connection);
        // declare todo queues
        ch_recv.declareQueue(QUEUE_TODO, AMQP::durable)
            .onSuccess( [&QUEUE_TODO](const string &name, uint32_t msgcnt, uint32_t csmercnt) {
                    // report the name of the temporary queue
                    cout << "declared queue " << name << endl;
                    })
        .onError([&QUEUE_TODO](const string &msg) {
                cout << "[ERROR] failed to declare queue" << QUEUE_TODO << ", message: " << msg <<endl;
                });

        // declare job done exchange/queue
        ch_send.declareExchange(EXCHG_DONE, AMQP::fanout, AMQP::durable)
            .onSuccess( [&EXCHG_DONE]() {
                    cout << "declared exchange " << EXCHG_DONE << endl;
                    })
        .onError([&EXCHG_DONE](const string &msg) {
                cout << "[ERROR] failed to declare exchange " << EXCHG_DONE << endl;
                });
        ch_send.declareQueue(QUEUE_DONE, AMQP::durable)
            .onSuccess( [&QUEUE_DONE](const string &name, uint32_t msgcnt, uint32_t csmercnt) {
                    cout << "declared queue " << name << endl;
                    })
        .onError([&QUEUE_DONE](const string &msg) {
                cout << "[ERROR] failed to declare queue" << QUEUE_DONE << ", message: " << msg <<endl;
                });
        ch_send.bindQueue(EXCHG_DONE, QUEUE_DONE, ROUTING_KEY_DONE)
            .onSuccess( [&EXCHG_DONE, &QUEUE_DONE]() {
                    cout << "binded exchange " << EXCHG_DONE << " with queue " << QUEUE_DONE << endl;
                    })
        .onError([&EXCHG_DONE, &QUEUE_DONE](const string &msg) {
                cout << "[ERROR] failed to bind exchange " << EXCHG_DONE << " with queue " << QUEUE_DONE << endl;
                });

        // start consuming from the queue, and install the callbacks
        ch_recv.consume(QUEUE_TODO, AMQP::durable)
            .onReceived( [&ch_recv, &ch_send, &EXCHG_DONE, &ROUTING_KEY_DONE](const AMQP::Message& message, uint64_t deliveryTag, bool redelivered) {
                    string msg_str = string(message.body(), static_cast<size_t>(message.bodySize()));
                    cout << "[main] message received: " << msg_str << endl;
                    // read message into json object
                    Document doc_msg, doc_result;
                    doc_msg.Parse(msg_str.c_str());
                    doc_result.SetObject();
                    // check message
                    if( !doc_msg.HasMember("name") || !doc_msg["name"].IsString() ||
                            !doc_msg.HasMember("creator") || !doc_msg["creator"].IsString() ||
                            !doc_msg.HasMember("create_date") || !doc_msg["create_date"].IsString()
                      ){ // bad request
                    Value val_code(400);
                    doc_result.AddMember("code", val_code, doc_result.GetAllocator());
                    Value val_stat("error",5,doc_result.GetAllocator());
                    doc_result.AddMember("status", val_stat, doc_result.GetAllocator());
                    cout<< "bad request" << endl;
                    } else {
                    string str_name = doc_msg["name"].GetString();
                    string str_creator = doc_msg["creator"].GetString();
                    string str_create_date = doc_msg["create_date"].GetString();
                    Value val_name(str_name.c_str(), static_cast<SizeType>(str_name.length()),doc_result.GetAllocator());
                    doc_result.AddMember("name", val_name, doc_result.GetAllocator());
                    Value val_creator(str_creator.c_str(), static_cast<SizeType>(str_creator.length()),doc_result.GetAllocator());
                    doc_result.AddMember("creator", val_creator, doc_result.GetAllocator());
                    Value val_create_date(str_create_date.c_str(), static_cast<SizeType>(str_create_date.length()),doc_result.GetAllocator());
                    doc_result.AddMember("create_date", val_create_date, doc_result.GetAllocator());
                    Value val_code(200);
                    doc_result.AddMember("code", val_code, doc_result.GetAllocator());
                    // if there is command, we parse it and calculate
                    if(doc_msg.HasMember("cmd") && doc_msg["cmd"].IsString()){
                        // calculate
                        CalculatorManager calc_mgr;
                        // split the cmd
                        vector<string> cmd_strs = Helper::split_cmd(doc_msg["cmd"].GetString());
                        // TODO: currently only support moving average
                        MovingAverageCalculator mv_calc;
                        calc_mgr.SetCalculator(mv_calc);
                        cout<< "[main] set moving average calculator" << endl;
                        // find the params
                        string params;
                        for(size_t i=0; i<cmd_strs.size()-1; ++i){
                            if(cmd_strs[i]=="-p"){
                                params = cmd_strs[i+1];
                                break;
                            }
                        }
                        cout<< "[main] start calculation" << endl;
                        string str_result =  calc_mgr.Calculate(params);
                        cout<< "[main] end calculation" << endl;
                        Value val_result(str_result.c_str(), static_cast<SizeType>(str_result.length()), doc_result.GetAllocator());
                        doc_result.AddMember("result", val_result, doc_result.GetAllocator());
                    }
                    // set status to done
                    Value val_stat("done",4,doc_result.GetAllocator());
                    doc_result.AddMember("status", val_stat, doc_result.GetAllocator());
                    }
                    // send the result
                    ch_send.startTransaction();
                    StringBuffer buffer;
                    Writer<StringBuffer> writer(buffer);
                    doc_result.Accept(writer);
                    ch_send.publish(EXCHG_DONE, ROUTING_KEY_DONE, buffer.GetString());
                    ch_send.commitTransaction();
                    // acknowledge the message
                    ch_recv.ack(deliveryTag);
            })
        .onSuccess( [&QUEUE_TODO](const string& consumertag) {
                cout << "consume operation started to listen queue: " 
                << QUEUE_TODO << endl;
                })
        .onError( [](const char *message) {
                cout << "consume operation failed" << endl;
                });

        // run the loop
        ev_run(loop, 0);
        // if error, try few seconds later
        if(handler.isError()){
            cout<< "[INFO]" << "Trying few seconds later" << endl;
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
        else{
            break;
        }
    } while(true);

    // done
    return 0;
}
