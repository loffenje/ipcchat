#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include "protocol.h"

#define SA struct sockaddr*

namespace ipcchat {
 
	struct TCPSocket {

	    TCPSocket() = default;
	    TCPSocket(struct sockaddr_in server, struct sockaddr_in peer, int conn):
		    servAddr(server), peerAddr(peer),fd(conn) {}

	    struct sockaddr_in servAddr;
	    struct sockaddr_in peerAddr;
	    int fd;

	    std::string getSockAddr() 
	    {
	   	std::string addr = inet_ntoa(peerAddr.sin_addr);
		addr.append(":");
		char buff[10];
		sprintf(buff, "%d", ntohs(peerAddr.sin_port));
		addr.append(buff);
		return addr;
	    }

	    void sendCommand(Command cmd) 
	    {
		int len = htonl(static_cast<uint32_t>(cmd));

		ssize_t nr = write(fd, (char *)&len, 4);
		if (nr == -1) {
		    std::cerr << "Write error while sending command\n";
		}
	    }

	    void sendCommand(IPCStatus status) 
	    {
		int len = htonl(static_cast<uint32_t>(status));

		ssize_t nr = write(fd, (char *)&len, 4);
		if (nr == -1) {
		    std::cerr << "Write error while sending command\n";
		}
	    }

	    void sendMessage(const std::string msg)
	    {
		int len = htonl(static_cast<uint32_t>(msg.length()));
		ssize_t nr;
		nr = write(fd, (char *)&len, 4);
		if (nr == -1) {
		    std::cerr << "Write error while sending message\n";
		}

		nr = write(fd, msg.c_str(), msg.length());
		if (nr == -1) {
		    std::cerr << "Write error while sending message\n";
		}
	    }

	    int getCommand()
	    {
		int len = 0;
		int rcv = read(fd, (char *)&len, 4);
		if (rcv != 4) return 0;

		return ntohl(static_cast<uint32_t>(len));
	    }

	    std::string getMessage()
	    {
		char msg[300];
		std::memset(&msg, 0, sizeof(msg));
		int msglen = 0;
		int len = 0;
		int rcv = 0;

		rcv = read(fd, (char *)&msglen, 4);
		if (rcv <= 0) return std::string("0");
		len = ntohl(static_cast<uint32_t>(msglen));
		ssize_t nr = read(fd, msg, len);
		if (nr == -1) {
		    std::cerr << "Read error while reading message\n";
		}

		return std::string(msg);
	    }
	};

TCPSocket initSocket(const std::string &addr, int port);
TCPSocket initSocket(int port);
TCPSocket *consumeConn(const TCPSocket &tcpSocket);
void closeConn(TCPSocket *tcpSocket);
void closeConn(TCPSocket &tcpSocket);

}

#endif
