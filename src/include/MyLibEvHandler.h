#ifndef _MYLIBEVHANDLER_H
#define _MYLIBEVHANDLER_H
#include <amqpcpp.h>
#include <amqpcpp/libev.h>

class MyLibEvHandler : public AMQP::LibEvHandler {
    public: 
        MyLibEvHandler(struct ev_loop *loop);
        bool isError();
        virtual ~MyLibEvHandler();
        virtual void onError(AMQP::TcpConnection *connection, const char *message) override;
        virtual void onConnected(AMQP::TcpConnection *connection) override;
    private:
        bool error_ = false;
};

#endif
