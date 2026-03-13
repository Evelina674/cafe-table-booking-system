#pragma once
#include <QString>

enum class UserRole { Admin, User };

struct User {
    int id = 0;
    QString username;
    QString passwordHash;
    UserRole role = UserRole::User;

    bool isValid() const { return id > 0; }
    bool isAdmin() const { return role == UserRole::Admin; }
};
