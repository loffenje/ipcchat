#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>
#include <vector>

#include "../utils/tcpsocket.h"

namespace ipcchat {
    class ClientManager {
    public:
        bool connectToServer(const std::string &addr, int port);

        bool authenticated() const;

        bool auth(const std::string &username);
	
	void consume();

        void broadcast(const std::string &msg);
    private:
	bool isReadableSocket() const;
    private:
        bool m_running = true;
	bool m_logged = false;
        bool m_initialized = false;
        TCPSocket m_servSocket;
        std::string m_username;
	std::string m_sockAddr;
	
    };
}
#endif
