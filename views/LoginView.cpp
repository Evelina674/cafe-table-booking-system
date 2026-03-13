#include "LoginView.h"
#include "ui_LoginView.h"
#include <QPushButton>
#include <QLineEdit>

LoginView::LoginView(QWidget* parent) : QDialog(parent), ui(new Ui::LoginView) {
    ui->setupUi(this);

    connect(ui->loginBtn, &QPushButton::clicked, this, [this] {
        emit loginRequested(ui->usernameEdit->text().trimmed(), ui->passwordEdit->text());
    });
    connect(ui->usernameEdit, &QLineEdit::returnPressed, ui->loginBtn, &QPushButton::click);
    connect(ui->passwordEdit, &QLineEdit::returnPressed, ui->loginBtn, &QPushButton::click);
}

LoginView::~LoginView() {
    delete ui;
}

void LoginView::showError(const QString& message) {
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void LoginView::clearFields() {
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->errorLabel->hide();
}
