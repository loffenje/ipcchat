#include "protocol.h"

using namespace ipcchat;

std::string ipcchat::getCommand(const std::string &cmd) {
    auto it = std::find_if(std::begin(cmds), std::end(cmds),
		    [cmd](auto &&c) { return c.name == cmd; });
    return it->name;
}


