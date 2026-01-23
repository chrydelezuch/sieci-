#pragma once
#include <string>
#include <vector>
#include "sqlite_orm/sqlite_orm.h"
#include <optional>

// ===== MODELE =====
namespace database
{
    

struct User {
    int id;
    std::string username;
    std::string password;
};

struct Channel {
    int id;
    std::string name;
};

struct UserChannel {
    int userId;
    int channelId;
};

// ===== STORAGE FACTORY =====

inline auto createStorage(const std::string& filename) {
    using namespace sqlite_orm;

    return make_storage(
        filename,

        make_table("users",
            make_column("id", &User::id, primary_key().autoincrement()),
            make_column("username", &User::username, unique()),
            make_column("password", &User::password)
        ),

        make_table("channels",
            make_column("id", &Channel::id, primary_key().autoincrement()),
            make_column("name", &Channel::name, unique())
        ),

        make_table("user_channels",
            make_column("user_id", &UserChannel::userId),
            make_column("channel_id", &UserChannel::channelId),
            primary_key(&UserChannel::userId, &UserChannel::channelId),
            foreign_key(&UserChannel::userId).references(&User::id),
            foreign_key(&UserChannel::channelId).references(&Channel::id)
        )
    );
}

// ===== DATABASE =====

class Database {
public:
    explicit Database(const std::string& dbFile);

    int addUser(const std::string& username, const std::string& password);
    int addChannel(const std::string& name);
    void addUserToChannel(int userId, int channelId);

    std::optional<User> getUserByUsername(const std::string& username);
    std::vector<Channel> getUserChannels(int userId);

private:
    using Storage = decltype(createStorage(""));

    Storage storage;
};


} // namespace name
