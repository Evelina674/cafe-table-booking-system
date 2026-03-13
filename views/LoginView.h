#pragma once
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginView; }
QT_END_NAMESPACE

class LoginView : public QDialog {
    Q_OBJECT
public:
    explicit LoginView(QWidget* parent = nullptr);
    ~LoginView();

    void showError(const QString& message);
    void clearFields();

signals:
    void loginRequested(const QString& username, const QString& password);

private:
    Ui::LoginView* ui;
};
