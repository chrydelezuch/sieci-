#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>
#include <string>
#include <unordered_map>

class UserManager {
private:
    std::vector<User> users; // przechowywanie oryginalnych obiektów
    std::unordered_map<std::string, std::vector<User*>> channelMap; // kanał -> wskaźniki do użytkowników

    UserManager() = default;

    // Pomocnicza metoda aktualizująca mapowanie kanałów
    void updateUserChannels(User& user);

public:
    static UserManager& getInstance();

    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    // Zarządzanie użytkownikami
    void addUser(const User user);

    void removeUser(std::string username);



    // Pobranie użytkownika po username
    User* getUserByUserName(const std::string& username);

    // Pobranie użytkownika po userId
    User* getUserByUserId(int userId);

    // Pobranie użytkowników wg kanału
    std::vector<User*> getUsersByChannel(const std::string& channel);
};

#endif // USERMANAGER_H
