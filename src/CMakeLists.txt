cmake_minimum_required(VERSION 3.6)
project(ipcchat)

add_compile_options(-std=c++14 -g -Wall -pthread)


find_package(Threads)

set(Client

        client/main.cpp
        client/client_manager.cpp
	utils/protocol.cpp
        utils/tcpsocket.cpp
    )

add_executable(Client ${Client})
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})


set(Server
        server/main.cpp
        server/server_manager.cpp
        server/socket_listener.cpp
	utils/protocol.cpp
        utils/tcpsocket.cpp
    )

add_executable(Server ${Server})

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
