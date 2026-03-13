#include "AuthService.h"
#include "repositories/IUserRepository.h"
#include <QCryptographicHash>

AuthService::AuthService(IUserRepository* userRepo) : m_userRepo(userRepo) {}

std::optional<User> AuthService::login(const QString& username, const QString& password) {
    auto user = m_userRepo->findByUsername(username);
    if (!user) return std::nullopt;
    if (user->passwordHash != hashPassword(password)) return std::nullopt;
    return user;
}

QString AuthService::hashPassword(const QString& password) {
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}
