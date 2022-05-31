#include "command.hpp"

void quit(User &user, Channel &channel, string message, int fd){
    
    string fullMessage = prefixMessage(user.getNickName(fd), user.getLoginName(fd), user.getHostName(fd), "quit", message);
    vector<string> channelList = user.getChannelList(fd);
    vector<string>::iterator it = channelList.begin();

    for (; it != channelList.end(); ++it){
        vector<string> command;
        command.push_back("PART");
        command.push_back(*it);
        command.push_back(message);
        part(user, channel, command, fd);
    }

    user.setStatus(fd, QUIT);
    user.setWriteBuffer(fd, fullMessage);
}