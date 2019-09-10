#ifndef SOCKET_LISTENER_H
#define SOCKET_LISTENER_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "user.h"
#include "../utils/tcpsocket.h"

namespace ipcchat {

class SocketListener {
public:

    void addSocket(TCPSocket *socket);
    
    void addUserSocket(TCPSocket *socket);

    void addSocket(std::unordered_map<std::string, TCPSocket *> peers);

    void addSocket(std::unordered_map<std::string, User> users);
    
    TCPSocket *selectSockets();

    bool hasUserSockets();

    void clearUserSockets();

    void clearSockets();

    TCPSocket *selectUserSockets();
private:
    std::vector<TCPSocket *> m_sockets;
    std::vector<TCPSocket *> m_userSockets;
};

}

#endif
