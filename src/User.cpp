#include "User.h"

namespace user{
// Konstruktor
User::User(const std::string& sessionId,
           const std::string& username,
           const std::vector<std::string> channels,
           int socketDescriptor)
    : sessionId(sessionId),
      username(username),
      channels(channels),
      socketDescriptor(socketDescriptor)
{
}

// Gettery
std::string User::getSessionId() const {
    return sessionId;
}

std::string User::getUsername() const {
    return username;
}

std::vector<std::string> User::getChannels() const {
    return channels;
}

int User::getSocketDescriptor(){
    return socketDescriptor;
}

// Settery
void User::setSessionId(const std::string& sessionId) {
    this->sessionId = sessionId;
}

void User::setUsername(const std::string& username) {
    this->username = username;
}

void User::setChannels(const std::vector<std::string>& channels) {
    this->channels = channels;
}

void User::setSocketDescriptor(int socketDescriptor) {
    this->socketDescriptor = socketDescriptor;
}


// Metody pomocnicze
void User::addChannel(const std::string& channel) {
    channels.push_back(channel);
}


}