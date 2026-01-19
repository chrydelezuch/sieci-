#include "CryptoUtils.h"

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>

#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// =========================
// Pomocnicze funkcje lokalne
// =========================
namespace {

constexpr std::size_t SALT_LEN = 16;
constexpr std::size_t HASH_LEN = 32;

// bytes → hex
std::string toHex(const unsigned char* data, std::size_t len) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(data[i]);
    }
    return oss.str();
}

// hex → bytes
std::vector<unsigned char> fromHex(const std::string& hex) {
    std::vector<unsigned char> out(hex.size() / 2);
    for (std::size_t i = 0; i < out.size(); ++i) {
        out[i] = static_cast<unsigned char>(
            std::stoul(hex.substr(i * 2, 2), nullptr, 16)
        );
    }
    return out;
}

} // namespace

// =========================
// Session ID
// =========================
std::string CryptoUtils::generateSessionId(std::size_t bytes) {
    std::vector<unsigned char> buf(bytes);

    if (RAND_bytes(buf.data(), static_cast<int>(buf.size())) != 1) {
        throw std::runtime_error("RAND_bytes failed");
    }

    return toHex(buf.data(), buf.size());
}

// =========================
// Hashowanie hasła
// =========================
std::string CryptoUtils::hashPassword(
    const std::string& password,
    int iterations
) {
    unsigned char salt[SALT_LEN];
    unsigned char hash[HASH_LEN];

    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw std::runtime_error("RAND_bytes failed");
    }

    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(),
            static_cast<int>(password.size()),
            salt,
            sizeof(salt),
            iterations,
            EVP_sha256(),
            sizeof(hash),
            hash
        )) {
        throw std::runtime_error("PBKDF2 failed");
    }

    std::ostringstream out;
    out << "pbkdf2_sha256$"
        << iterations << "$"
        << toHex(salt, sizeof(salt)) << "$"
        << toHex(hash, sizeof(hash));

    return out.str();
}

// =========================
// Weryfikacja hasła
// =========================
bool CryptoUtils::verifyPassword(
    const std::string& password,
    const std::string& storedHash
) {
    std::istringstream iss(storedHash);
    std::string algo, iterStr, saltHex, hashHex;

    std::getline(iss, algo, '$');
    std::getline(iss, iterStr, '$');
    std::getline(iss, saltHex, '$');
    std::getline(iss, hashHex, '$');

    if (algo != "pbkdf2_sha256") {
        return false;
    }

    int iterations = std::stoi(iterStr);

    auto salt = fromHex(saltHex);
    auto expectedHash = fromHex(hashHex);

    unsigned char hash[HASH_LEN];

    PKCS5_PBKDF2_HMAC(
        password.c_str(),
        static_cast<int>(password.size()),
        salt.data(),
        static_cast<int>(salt.size()),
        iterations,
        EVP_sha256(),
        sizeof(hash),
        hash
    );

    return CRYPTO_memcmp(
        hash,
        expectedHash.data(),
        HASH_LEN
    ) == 0;
}
