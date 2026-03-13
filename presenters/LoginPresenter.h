#pragma once
#include <QObject>
#include "models/User.h"

class LoginView;
class AuthService;

class LoginPresenter : public QObject {
    Q_OBJECT
public:
    LoginPresenter(LoginView* view, AuthService* authService, QObject* parent = nullptr);

signals:
    void loginSucceeded(const User& user);

private slots:
    void onLoginRequested(const QString& username, const QString& password);

private:
    LoginView*   m_view;
    AuthService* m_authService;
};
