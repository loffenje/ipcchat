#include "socket_listener.h"

#include <sys/time.h>
#include <sys/select.h>

using namespace ipcchat;

void SocketListener::addSocket(TCPSocket *socket) {
    m_sockets.emplace_back(socket);
}

void SocketListener::addUserSocket(TCPSocket *socket) {
    m_userSockets.emplace_back(socket);
}
void SocketListener::addSocket(std::unordered_map<std::string, TCPSocket *> peers) {
    for (auto it = std::begin(peers); it != std::end(peers); it++) {
        m_sockets.emplace_back(it->second);
    }
}

void SocketListener::addSocket(std::unordered_map<std::string, User> users) {
    for (auto it = std::begin(users); it != std::end(users); it++) {
        m_userSockets.emplace_back(it->second.tcpSocket);
    }
}


TCPSocket *SocketListener::selectSockets() 
{
    const int timeout = 2;
    struct timeval t;
    t.tv_sec = timeout;
    t.tv_usec = 0;

    fd_set fdset;
    FD_ZERO(&fdset);
    int incfd = 0;

    for (auto it = std::begin(m_sockets); it != std::end(m_sockets); it++ ) {
	incfd++;
    	FD_SET((*it)->fd, &fdset);
    }

    int ret;
    if (timeout > 0) {
    	ret = select(FD_SETSIZE, &fdset, NULL, NULL, &t);
    } else {
	ret = select(FD_SETSIZE, &fdset, NULL, NULL, NULL);
    }

   if (ret) {
	for (int i = 0; i < incfd; i++) {
	    TCPSocket *tmp = m_sockets[i];
	    if (FD_ISSET(tmp->fd, &fdset)) return tmp;
	}
   }

   return nullptr;
}


TCPSocket *SocketListener::selectUserSockets() 
{
    const int timeout = 1;
    struct timeval t;
    t.tv_sec = timeout;
    t.tv_usec = 0;

    fd_set fdset;
    FD_ZERO(&fdset);
    int incfd = 0;
    
    if (m_userSockets.empty()) return nullptr;

    for (auto it = std::begin(m_userSockets); it != std::end(m_userSockets); it++ ) {
	incfd++;
    	FD_SET((*it)->fd, &fdset);
    }

    int ret;
    if (timeout > 0) {
    	ret = select(FD_SETSIZE, &fdset, NULL, NULL, &t);
    } else {
	ret = select(FD_SETSIZE, &fdset, NULL, NULL, NULL);
    }

   if (ret) {
	for (int i = 0; i < incfd; i++) {
	    TCPSocket *tmp = m_userSockets[i];
	    if (FD_ISSET(tmp->fd, &fdset)) return tmp;
	}
   }

   return nullptr;
}

void SocketListener::clearSockets()
{
    if (!m_sockets.empty()) m_sockets.clear();
}
