#pragma once
#include "IUserRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class UserRepository : public IUserRepository {
public:
    explicit UserRepository(QSqlDatabase& db);

    std::optional<User> findByUsername(const QString& username) const override;
    std::optional<User> findById(int id) const override;
    QList<User> findAll() const override;
    bool save(User& user) override;
    bool remove(int id) override;

private:
    User fromQuery(const QSqlQuery& q) const;
    QSqlDatabase& m_db;
};
