#include <thread>
#include <iostream>
#include "server_manager.h"

using namespace ipcchat;

void ServerManager::messangerImpl()
{ 
   std::string sender;
   std::string msg;
   TCPSocket *socket;
   int command = 0;
   while (m_running) {
	if (m_users.empty()) continue; 
	socket = m_listener->selectUserSockets();
	if (!socket) continue;
	command = socket->getCommand();
        if (command == static_cast<int>(Command::SEND)) {
	    sender = socket->getMessage();
	    msg = socket->getMessage();
	    for (auto it = std::begin(m_users); it != std::end(m_users); it++) {
		auto userSocket = it->second.tcpSocket;
		auto user = m_users.find(sender);
	        userSocket->sendCommand(IPCStatus::NEW_MSG);
	        userSocket->sendMessage(msg);
	        userSocket->sendMessage(user == m_users.end() ? it->first : user->second.name);
	    }
	}
	
    }
}

void ServerManager::authImpl()
{
    std::string username;
    TCPSocket *socket;
    int command;
    while (m_running) {
        {
            std::lock_guard<std::mutex> lock(m_peerLocker);
  	    m_listener->clearSockets();      
	    m_listener->addSocket(m_peers);
        }

        socket = m_listener->selectSockets();
        if (!socket) continue;

        command = socket->getCommand();
        if (command == static_cast<int>(Command::AUTH)) {
            username = socket->getMessage();
            std::cout << "Joined user: " << username << '\n';
            socket->sendCommand(IPCStatus::AUTH_OK);
	    socket->sendMessage(socket->getSockAddr());
	    User user{username, socket}; 
	    addUser(user);
	    for (auto it = m_users.begin(); it != m_users.end(); it++) {
		std::cout << it->first << std::endl;
	    }
	    m_listener->addUserSocket(user.tcpSocket);
	    {
	        std::lock_guard<std::mutex> l(m_peerLocker);
		m_peers.erase(socket->getSockAddr());
	    }

        }
    }
}

void ServerManager::handleConnImpl()
{
    TCPSocket *temp;
    m_client = initSocket(PORT);
    while (m_running) {
        temp = consumeConn(m_client);
        if (!temp) break;
        addPeer(temp);
        std::cout << "New peer is connected!\n";
    }
}

void ServerManager::addPeer(TCPSocket *socket)
{
    std::lock_guard<std::mutex> l(m_peerLocker);
    m_peers[socket->getSockAddr()] = socket;
}

void ServerManager::addUser(const User &user)
{
    std::lock_guard<std::mutex> l(m_userLocker);	
    m_users[user.tcpSocket->getSockAddr()] = user;
}

void ServerManager::startUp() 
{
    std::thread m(&ServerManager::messangerImpl, this);
    std::thread auth(&ServerManager::authImpl, this);
    std::thread handle(&ServerManager::handleConnImpl, this);
    handle.join();
    auth.join();
    m.join();
}
