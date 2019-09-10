#include <string>
#include <iostream>
#include <thread>

#include "client_manager.h"
#include "../utils/protocol.h"

using namespace ipcchat;


inline void printInstructions() 
{
    std::cout << "IPC ring v0.0.1\n";
    std::cout << "Use this commands for interaction with IO\n";

    std::cout << "broadcast  ----> send to all connections a message\n";
    std::cout << "auth       ----> authorize in chat by login\n";
    std::cout << "quit(exit) ----> Stop the execution of server\n";
}

int main() {

    ClientManager client;
    printInstructions();
    if (client.connectToServer("127.0.0.1", PORT)) {
        std::cout << "Successfully connected!\n";
    } else {
        std::cerr << "Connection failed!\n";
    }
    
    const char *prompt = "ipc#0.0.1 >";
    std::string command;
    std::thread cli(&ClientManager::consume, client);
    std::cout << prompt;
    while(std::getline(std::cin, command)) {
       
    if (command.compare("quit") == 0 || command.compare("exit") == 0) {
	    std::cout << "Stopped\n";
	    exit(EXIT_SUCCESS);
	    break;
	}

	if (command.compare("help") == 0) {
	    printInstructions();
	    std::cout << '\n';
	}
	
        std::cout << prompt;	
	std::string arg; 
        if (command == getCommand("auth")) {
	    std::cout << "Write login\n";
            std::cin >> arg;

            if (client.authenticated()) {
                std::cout << "You have already logged in!\n";
            } else if (client.auth(arg)) {
                std::cout << "Successfully authenticated!\n";
            } else {
                std::cerr << "Authentication failed!\n";
            }
            
        }

        if (command == getCommand("broadcast")) {
	    std::cout << "Write a message\n";
	    std::cin >> arg;

            if (client.authenticated()) {
                client.broadcast(arg);
            } else {
                std::cerr << "Authentication first!\n";
            }
        }
    }

    cli.join();
}
