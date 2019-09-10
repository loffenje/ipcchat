#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <thread>
#include <unordered_map>
#include <string>
#include <mutex>
#include <iostream>
#include "user.h"
#include "socket_listener.h"

namespace ipcchat {

class ServerManager {
public:
    ServerManager(): m_running(true), m_listener(nullptr) {
    	m_listener = new SocketListener();
    }
    ~ServerManager() {
        m_running = false;
        for (auto it = m_users.begin(); it != m_users.end(); it++)
        {
            closeConn(it->second.tcpSocket);
        }

        closeConn(m_client);
	delete m_listener;
    }

    void messangerImpl();
    void authImpl();
    void handleConnImpl();
    void startUp();
private:
    void addPeer(TCPSocket *socket);
    void addUser(const User &user);
private:
    bool m_running;
    std::unordered_map<std::string, User> m_users;
    std::unordered_map<std::string, TCPSocket *> m_peers;
    std::mutex m_userLocker;
    std::mutex m_peerLocker;
    SocketListener *m_listener;
    TCPSocket m_client;
};

}

#endif
