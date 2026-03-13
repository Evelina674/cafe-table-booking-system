#include "LoginPresenter.h"
#include "views/LoginView.h"
#include "services/AuthService.h"

LoginPresenter::LoginPresenter(LoginView* view, AuthService* authService, QObject* parent)
    : QObject(parent), m_view(view), m_authService(authService) {
    connect(m_view, &LoginView::loginRequested, this, &LoginPresenter::onLoginRequested);
}

void LoginPresenter::onLoginRequested(const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        m_view->showError("Введіть логін та пароль.");
        return;
    }
    auto user = m_authService->login(username, password);
    if (!user) {
        m_view->showError("Невірний логін або пароль.");
        return;
    }
    emit loginSucceeded(*user);
}
