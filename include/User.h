#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
private:
    std::string sessionId;
    std::string username;
    std::vector<std::string> channels;
    int socketDescriptor;

public:
    // Konstruktor
    User(const std::string& sessionId,
         const std::string& username,
         const std::vector<std::string> channels,
         int socketDescriptor);

    // Gettery
    std::string getSessionId() const;
    std::string getUsername() const;
    std::vector<std::string> getChannels() const;
    int getSocketDescriptor();

    // Settery
    void setSessionId(const std::string& sessionId);
    void setUsername(const std::string& username);
    void setChannels(const std::vector<std::string>& channels);
    void setSocketDescriptor(int socketDescriptor);

    // Metody pomocnicze
    void addChannel(const std::string& channel);
};

#endif // USER_H
