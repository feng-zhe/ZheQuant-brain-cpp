#include "MyLibEvHandler.h"
#include <iostream>

using namespace std;

MyLibEvHandler::MyLibEvHandler(struct ev_loop *loop): LibEvHandler(loop) {}

MyLibEvHandler::~MyLibEvHandler(){}

bool MyLibEvHandler::isError(){
    return this->error_;
}

void MyLibEvHandler::onError(AMQP::TcpConnection *connection, const char *message)
{
    this->error_ = true;
    cout<< "[ERROR] in handler: " << message <<endl;
}

void MyLibEvHandler::onConnected(AMQP::TcpConnection *connection)
{
    this->error_ = false;
}
