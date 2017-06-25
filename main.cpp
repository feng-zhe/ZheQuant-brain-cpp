#include <ev.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "MyLibEvHandler.h"
#include "CalculatorManager.h"
#include "Configure.h"

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

int main()
{
    // const string part
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
                    cout << "[main]" << " declared queue " << name << endl;
                    })
        .onError([&QUEUE_TODO](const string &msg) {
                cout << "[main]" << " [error] failed to declare queue" << QUEUE_TODO << ", message: " << msg <<endl;
                });

        // declare job done exchange/queue
        ch_send.declareExchange(EXCHG_DONE, AMQP::fanout, AMQP::durable)
            .onSuccess( [&EXCHG_DONE]() {
                    cout << "[main]" << " declared exchange " << EXCHG_DONE << endl;
                    })
        .onError([&EXCHG_DONE](const string &msg) {
                cout << "[main]" << " [error] failed to declare exchange " << EXCHG_DONE << endl;
                });
        ch_send.declareQueue(QUEUE_DONE, AMQP::durable)
            .onSuccess( [&QUEUE_DONE](const string &name, uint32_t msgcnt, uint32_t csmercnt) {
                    cout << "[main]" << " declared queue " << name << endl;
                    })
        .onError([&QUEUE_DONE](const string &msg) {
                cout << "[main]" << " [error] failed to declare queue" << QUEUE_DONE << ", message: " << msg <<endl;
                });
        ch_send.bindQueue(EXCHG_DONE, QUEUE_DONE, ROUTING_KEY_DONE)
            .onSuccess( [&EXCHG_DONE, &QUEUE_DONE]() {
                    cout << "[main]" << " binded exchange " << EXCHG_DONE << " with queue " << QUEUE_DONE << endl;
                    })
        .onError([&EXCHG_DONE, &QUEUE_DONE](const string &msg) {
                cout << "[main]" << " [error] failed to bind exchange " << EXCHG_DONE << " with queue " << QUEUE_DONE << endl;
                });

        // start consuming from the queue, and install the callbacks
        ch_recv.consume(QUEUE_TODO, AMQP::durable)
            .onReceived( [&ch_recv, &ch_send, &EXCHG_DONE, &ROUTING_KEY_DONE](const AMQP::Message& message, uint64_t deliveryTag, bool redelivered) {
                    string msg_str = string(message.body(), static_cast<size_t>(message.bodySize()));
                    cout << "[main]" << "  message received: " << msg_str << endl;
                    // read message into json object
                    auto msg_view = bsoncxx::from_json(msg_str).view();
                    auto rst_doc = document{};
                    // check message
                    if( !msg_view["name"] || !(msg_view["name"].type()==bsoncxx::type::k_utf8) ||
                        !msg_view["creator"] || !(msg_view["creator"].type()==bsoncxx::type::k_utf8) ||
                        !msg_view["create_date"] || !(msg_view["create_date"].type()==bsoncxx::type::k_utf8) ) { // bad request
                        rst_doc << "code" << 400
                                << "status" << "error";
                        cout<< "[main]" << " bad request" << endl;
                    } else {
                        string str_name = msg_view["name"].get_utf8().value.to_string();
                        string str_creator = msg_view["creator"].get_utf8().value.to_string();
                        string str_create_date = msg_view["create_date"].get_utf8().value.to_string();
                        rst_doc    << "name" << str_name
                                    << "creator" << str_creator
                                    << "create_date" << str_create_date
                                    << "code" << 200;
                        // if there is command, we parse it and calculate
                        if(msg_view["cmd"] && msg_view["cmd"].type()==bsoncxx::type::k_utf8){
                            // the calculator manager will find the right calculator
                            CalculatorManager calc_mgr;
                            string str_cmd = msg_view["cmd"].get_utf8().value.to_string();
                            string str_result =  calc_mgr.Calculate(str_cmd);
                            rst_doc << "result" << str_result;
                        }
                        // set status to done
                        rst_doc << "status" << "done";
                    }
                    // get result string
                    auto rst_val = rst_doc << finalize;
                    auto rst_str = bsoncxx::to_json(rst_val.view());
                    // send the result
                    ch_send.startTransaction();
                    ch_send.publish(EXCHG_DONE, ROUTING_KEY_DONE, rst_str);
                    ch_send.commitTransaction();
                    // acknowledge the message
                    ch_recv.ack(deliveryTag);
            })
        .onSuccess( [&QUEUE_TODO](const string& consumertag) {
                cout << "[main]" << " consume operation started to listen queue: " 
                << QUEUE_TODO << endl;
                })
        .onError( [](const char *message) {
                cout << "[main]" << " consume operation failed" << endl;
                });

        // run the loop
        ev_run(loop, 0);
        // if error, try few seconds later
        if(handler.isError()){
            cout<< "[main]" << " [info]" << "Trying few seconds later" << endl;
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
        else{
            break;
        }
    } while(true);

    // done
    return 0;
}
