#pragma once

#include <string>
#include <cstddef>

class CryptoUtils {
public:
    // Generuje bezpieczny identyfikator sesji (hex)
    static std::string generateSessionId(std::size_t bytes = 32);

    // Hashuje hasło i zwraca string:
    // pbkdf2_sha256$iterations$salt_hex$hash_hex
    static std::string hashPassword(
        const std::string& password,
        int iterations = 100000
    );

    // Weryfikuje hasło na podstawie zapisanego hasha
    static bool verifyPassword(
        const std::string& password,
        const std::string& storedHash
    );
};
