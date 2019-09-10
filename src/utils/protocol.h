#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <array>
#include <string>
#include <algorithm>

namespace ipcchat {

constexpr int PORT = 8080;

enum class Command {
    AUTH = 255,
    BROADCAST,
    SEND,
    USERS_LIST,
};

enum class IPCStatus {
    AUTH_OK = 64,
    AUTH_ERR,
    SENT,
    NEW_MSG
};

constexpr int CMD_LEN = 4;

struct CommandDescriptor {
    std::string name;
    Command kind;
};

const std::array<CommandDescriptor, CMD_LEN> cmds {{
    {"auth", Command::AUTH},
    {"broadcast", Command::BROADCAST},
    {"user_list", Command::USERS_LIST},
    {"send", Command::SEND}
}};

std::string getCommand(const std::string &cmd);


}

#endif
