#include "Database.h"

using namespace sqlite_orm;

Database::Database(const std::string& dbFile)
    : storage(createStorage(dbFile))
{
    storage.sync_schema();
}

int Database::addUser(const std::string& username, const std::string& password) {
    User user{-1, username, password};
    return storage.insert(user);
}

int Database::addChannel(const std::string& name) {
    Channel channel{-1, name};
    return storage.insert(channel);
}

void Database::addUserToChannel(int userId, int channelId) {
    storage.insert(UserChannel{userId, channelId});
}

std::vector<Channel> Database::getUserChannels(int userId) {
    auto rows = storage.select(
        columns(&Channel::id, &Channel::name),
        inner_join<UserChannel>(on(c(&Channel::id) == &UserChannel::channelId)),
        where(c(&UserChannel::userId) == userId)
    );

    std::vector<Channel> result;
    for (auto& r : rows) {
        result.push_back({std::get<0>(r), std::get<1>(r)});
    }
    return result;
}

std::optional<User>
Database::getUserByUsername(const std::string& username) {
    auto result = storage.get_all<User>(
        sqlite_orm::where(sqlite_orm::c(&User::username) == username),
        sqlite_orm::limit(1)
    );

    if (result.empty()) {
        return std::nullopt;
    }

    return result.front();
}
