#pragma once
#include <optional>
#include "models/User.h"

class IUserRepository;

class AuthService {
public:
    explicit AuthService(IUserRepository* userRepo);

    std::optional<User> login(const QString& username, const QString& password);
    static QString hashPassword(const QString& password);

private:
    IUserRepository* m_userRepo;
};
