#include "tcpsocket.h"

using namespace ipcchat;


TCPSocket ipcchat::initSocket(int port)
{

    TCPSocket tcpSocket;
    tcpSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
    
    std::memset(&tcpSocket.servAddr, 0, sizeof(tcpSocket.servAddr));

    tcpSocket.servAddr.sin_family = AF_INET;
    tcpSocket.servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcpSocket.servAddr.sin_port = htons(port);

    std::cout << "Server binding\n";    
    if (bind(tcpSocket.fd, (SA)&tcpSocket.servAddr, sizeof(tcpSocket.servAddr)) < 0) { 
	std::cerr << "bind error\n";
        exit(EXIT_FAILURE);
    }

    return tcpSocket;
}

TCPSocket ipcchat::initSocket(const std::string &addr, int port) 
{
    TCPSocket tcpSocket;
    tcpSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
    std::memset(&tcpSocket.peerAddr, 0, sizeof(tcpSocket.peerAddr));

    tcpSocket.peerAddr.sin_family = AF_INET;
    tcpSocket.peerAddr.sin_addr.s_addr = inet_addr(addr.data());
    tcpSocket.peerAddr.sin_port = htons(port);
    if (connect(tcpSocket.fd, (SA)&tcpSocket.peerAddr, sizeof(tcpSocket.peerAddr)) < 0) {
        std::cerr << "connect error\n";
	close(tcpSocket.fd);
        exit(EXIT_FAILURE);
    }

    return tcpSocket;
}

TCPSocket *ipcchat::consumeConn(const TCPSocket &tcpSocket) 
{
    int rc = listen(tcpSocket.fd, 1);

    if (rc == -1) {
        std::cerr << "listen error\n";
        return nullptr;
    }

    size_t len = sizeof(tcpSocket.peerAddr);
    std::memset((char *)&tcpSocket.peerAddr, 0, sizeof(tcpSocket.peerAddr));

    int conn = accept(tcpSocket.fd, (SA)&tcpSocket.peerAddr, (socklen_t *)&len);
    if (conn == -1) {
        std::cerr << "conn error\n";
        return nullptr;
    }
    
    return new TCPSocket(tcpSocket.servAddr, tcpSocket.peerAddr, conn);
}

void ipcchat::closeConn(TCPSocket *tcpSocket)
{
    close(tcpSocket->fd);
}


void ipcchat::closeConn(TCPSocket &tcpSocket)
{
    close(tcpSocket.fd);
}
