#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>
#include <string>
#include <unordered_map>


using namespace user;

class UserManager {
private:
    // Przechowujemy oryginalne obiekty
    std::vector<User> users;

    // Kanał -> wskaźniki do użytkowników
    std::unordered_map<std::string, std::vector<User*>> channelMap;

    // Singleton: prywatny konstruktor
    UserManager() = default;

    // Aktualizacja mapy kanałów dla użytkownika
    void updateUserChannels(User user);

public:
    // Singleton access
    static UserManager& getInstance();

    // Blokada kopiowania
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    // Zarządzanie użytkownikami
    void addUser(User user);
    void removeUser(const std::string& username);

    // Pobieranie użytkowników
    std::vector<User*> getAllUsers();
    User* getUserByUserName(const std::string& username);
  
    std::vector<User*> getUsersByChannel(const std::string& channel);
};

#endif // USERMANAGER_H
