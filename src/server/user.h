#ifndef USER_H
#define USER_H

#include <vector>
#include <string>

namespace ipcchat {

struct TCPSocket;

struct User {
    std::string name;
    TCPSocket *tcpSocket;
};

};

#endif
