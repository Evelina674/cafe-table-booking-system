#include "UserRepository.h"
#include <QSqlError>
#include <QDebug>

UserRepository::UserRepository(QSqlDatabase& db) : m_db(db) {}

User UserRepository::fromQuery(const QSqlQuery& q) const {
    User u;
    u.id = q.value("id").toInt();
    u.username = q.value("username").toString();
    u.passwordHash = q.value("password_hash").toString();
    u.role = (q.value("role").toString() == "admin") ? UserRole::Admin : UserRole::User;
    return u;
}

std::optional<User> UserRepository::findByUsername(const QString& username) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM users WHERE username = :u");
    q.bindValue(":u", username);
    if (q.exec() && q.next()) return fromQuery(q);
    return std::nullopt;
}

std::optional<User> UserRepository::findById(int id) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM users WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) return fromQuery(q);
    return std::nullopt;
}

QList<User> UserRepository::findAll() const {
    QList<User> result;
    QSqlQuery q("SELECT * FROM users ORDER BY username", m_db);
    while (q.next()) result.append(fromQuery(q));
    return result;
}

bool UserRepository::save(User& user) {
    QSqlQuery q(m_db);
    if (user.id == 0) {
        q.prepare("INSERT INTO users (username, password_hash, role) VALUES (:u, :h, :r)");
    } else {
        q.prepare("UPDATE users SET username=:u, password_hash=:h, role=:r WHERE id=:id");
        q.bindValue(":id", user.id);
    }
    q.bindValue(":u", user.username);
    q.bindValue(":h", user.passwordHash);
    q.bindValue(":r", user.role == UserRole::Admin ? "admin" : "user");
    if (!q.exec()) {
        qWarning() << "UserRepository::save:" << q.lastError().text();
        return false;
    }
    if (user.id == 0) user.id = q.lastInsertId().toInt();
    return true;
}

bool UserRepository::remove(int id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM users WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
