#include "UserManager.h"
#include <algorithm>

// Singleton
UserManager& UserManager::getInstance() {
    static UserManager instance;
    return instance;
}

// Pomocnicza metoda aktualizująca mapowanie kanałów
void UserManager::updateUserChannels(User user) {
    for (const auto& channel : user.getChannels()) {
        channelMap[channel].push_back(user); 
    }
}

// Dodanie użytkownika (kopiujemy, bo caller może przekazać tymczasowy obiekt)
void UserManager::addUser(User user) {
    users.push_back(std::move(user));        // przenosimy kopię do wektora
    updateUserChannels(users.back());        // aktualizujemy mapę kanałów
}

// Usunięcie użytkownika po userName
void UserManager::removeUser(const std::string& username) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username) {
            
            for (auto& [channel, vec] : channelMap) {
                vec.erase(std::remove(vec.begin(), vec.end(), &users[i]), vec.end());
            }
            users.erase(users.begin() + i);
            return; // zakładamy, że username jest unikalny
        }
    }
}

// Pobranie wszystkich użytkowników
std::vector<User*> UserManager::getAllUsers() {
    std::vector<User*> result;
    result.reserve(users.size());
    for (auto& u : users) result.push_back(&u);
    return result;
}

// Pobranie użytkownika po username
User* UserManager::getUserByUserName(const std::string& username) {
    for (auto& u : users) {
        if (u.getUsername() == username) return &u;
    }
    return nullptr;
}

// Pobranie użytkownika po userId
User* UserManager::getUserByUserId(int userId) {
    for (auto& u : users) {
        if (u.getUserId() == userId) return &u;
    }
    return nullptr;
}

// Pobranie użytkowników wg kanału
std::vector<User*> UserManager::getUsersByChannel(const std::string& channel) {
    auto it = channelMap.find(channel);
    if (it != channelMap.end()) {
        return it->second; // zwracamy wskaźniki do oryginalnych obiektów
    }
    return {};
}
