#include "client_manager.h"

using namespace ipcchat;

bool ClientManager::isReadableSocket() const
{
    struct timeval t;
    t.tv_sec = 2;
    t.tv_usec = 0;
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(m_servSocket.fd, &readfd); 
    int ret = select(FD_SETSIZE, &readfd, NULL, NULL, &t);
    if (ret) {
        return FD_ISSET(m_servSocket.fd, &readfd) != 0;
    }
    
    return false;
}

void ClientManager::consume()
{
    int command = 0;
    std::string sender;
    std::string msg;
    while (m_running) { 
        if (isReadableSocket()) {
	    command = m_servSocket.getCommand();
	    if (command == static_cast<int>(IPCStatus::NEW_MSG)) {
		msg = m_servSocket.getMessage();
		sender = m_servSocket.getMessage();
	       	std::cout << "[ " << sender << " ] " << msg << '\n';
	    }
        }
    }
}

bool ClientManager::connectToServer(const std::string &addr, int port)
{     
    if (m_initialized) return false;

    m_servSocket = initSocket(addr,port);
    m_initialized = true;

    return m_initialized;
}

void ClientManager::broadcast(const std::string &msg) 
{
    m_servSocket.sendCommand(Command::SEND);
    m_servSocket.sendMessage(m_sockAddr);
    m_servSocket.sendMessage(msg);
}

bool ClientManager::authenticated() const
{
    return m_logged;
}

bool ClientManager::auth(const std::string &username)
{
    if (!m_initialized) return false;

    m_servSocket.sendCommand(Command::AUTH);
    m_servSocket.sendMessage(username);
    int response = m_servSocket.getCommand();
    if (response == static_cast<int>(IPCStatus::AUTH_OK)) {
	m_logged = true;
	m_sockAddr = m_servSocket.getMessage();
	return true;
    }
    
    return false;
}


