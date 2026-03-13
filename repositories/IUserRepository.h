#pragma once
#include <optional>
#include <QList>
#include "models/User.h"

// Інтерфейс репозиторію (SOLID DIP - залежність від абстракції, а не від конкретної SQL-реалізації)
class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual std::optional<User> findByUsername(const QString& username) const = 0;
    virtual std::optional<User> findById(int id) const = 0;
    virtual QList<User> findAll() const = 0;
    virtual bool save(User& user) = 0;
    virtual bool remove(int id) = 0;
};
