#include <QApplication>
#include <QMessageBox>
#include <functional>
#include <memory>

#include "database/DatabaseManager.h"
#include "repositories/UserRepository.h"
#include "repositories/TableRepository.h"
#include "repositories/ReservationRepository.h"
#include "services/AuthService.h"
#include "services/ReservationService.h"
#include "services/TableService.h"
#include "views/LoginView.h"
#include "views/MainView.h"
#include "views/AdminView.h"
#include "presenters/LoginPresenter.h"
#include "presenters/ReservationPresenter.h"
#include "presenters/AdminPresenter.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");

    app.setStyleSheet(R"(
        QWidget { font-family: "Segoe UI", sans-serif; font-size: 11px; color: #111111; }
        QLabel { color: #111111; }
        QDialog  { background-color: #F9FAFB; }
        QMainWindow > QWidget { background-color: #F9FAFB; }
        QPushButton {
            background-color: #F0F2F5; border: 1px solid #C4C8CC;
            border-radius: 4px; padding: 5px 14px; min-height: 24px; color: #2C3E50;
        }
        QPushButton:hover    { background-color: #E4EAF2; border-color: #9AB0C8; }
        QPushButton:pressed  { background-color: #D0DCE8; }
        QTableWidget {
        gridline-color: #E4E7EB;
        border: 1px solid #D8DCE0;
        selection-background-color: #D5E8F5;
        selection-color: #1A1A1A;
        alternate-background-color: #F5F8FB;
        background-color: #FFFFFF;
        color: black;
        }

        QTableWidget::item:selected {
        color: black;
        }
        QHeaderView::section {
            background-color: #EEF0F2; border: none;
            border-right: 1px solid #D8DCE0; border-bottom: 1px solid #D8DCE0;
            padding: 4px 8px; font-weight: 600; color: #444444;
        }
        QTabWidget::pane { border: 1px solid #D8DCE0; background-color: #FFFFFF; }
        QTabBar::tab {
            background-color: #ECEEF0; border: 1px solid #D4D8DC; border-bottom: none;
            border-radius: 3px 3px 0 0; padding: 5px 16px; margin-right: 2px; color: #555555;
        }
        QTabBar::tab:selected          { background-color: #FFFFFF; color: #1A1A1A; font-weight: 600; }
        QTabBar::tab:hover:!selected   { background-color: #E2EAF2; }
        QLineEdit, QDateEdit, QTimeEdit, QSpinBox, QComboBox {
        border: 1px solid #C0C5CC;
        border-radius: 3px;
        padding: 3px 6px;
        background-color: #FFFFFF;
        color: #111111;
        }
        QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus,
        QSpinBox:focus, QComboBox:focus { border-color: #7AADE0; outline: none; }
        QMenuBar { background-color: #F2F4F6; border-bottom: 1px solid #D8DCE0; }
        QMenuBar::item:selected { background-color: #E0EAF4; }
        QMenu { border: 1px solid #D4D8DC; background-color: #FFFFFF; }
        QMenu::item:selected { background-color: #D5E8F5; color: #1A1A1A; }
        QScrollBar:vertical {
            background: #F5F5F5; width: 10px; border: none;
        }
        QScrollBar::handle:vertical {
            background: #C8CDD4; border-radius: 5px; min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background: #A8B4C0; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )");

    if (!DatabaseManager::instance().initialize()) {
        QMessageBox::critical(nullptr, "Помилка", "Не вдалося відкрити базу даних.");
        return 1;
    }

    auto& db = DatabaseManager::instance().database();

    // Репозиторії (Repository pattern) - живуть до кінця main()
    auto userRepo  = std::make_shared<UserRepository>(db);
    auto tableRepo = std::make_shared<TableRepository>(db);
    auto resRepo   = std::make_shared<ReservationRepository>(db);

    // Сервіси (SOLID DIP - залежать від інтерфейсів)
    auto authService  = std::make_shared<AuthService>(userRepo.get());
    auto resService   = std::make_shared<ReservationService>(resRepo.get(), tableRepo.get());
    auto tableService = std::make_shared<TableService>(tableRepo.get());

    // Цикл входу - підтримує вихід і повторний вхід
    std::function<void()> showLogin;

    showLogin = [&]() {
        auto* loginView = new LoginView;

        // Presenter отримує raw pointer - сервіси живуть довше (вище в стеку)
        auto* loginPresenter = new LoginPresenter(loginView, authService.get(), loginView);

        // Observer: presenter сигналізує → main відкриває відповідне вікно
        QObject::connect(loginPresenter, &LoginPresenter::loginSucceeded,
                         [&, loginView, loginPresenter](const User& user) {
            loginView->hide();

            if (user.isAdmin()) {
                auto* adminView = new AdminView(user);
                auto* presenter = new AdminPresenter(adminView,
                                                     resService.get(),
                                                     tableService.get(),
                                                     userRepo.get(),
                                                     user);
                presenter->loadAll();
                adminView->show();

                QObject::connect(adminView, &AdminView::logoutRequested,
                                 [=, &showLogin]() {
                    adminView->close();
                    presenter->deleteLater();
                    adminView->deleteLater();
                    showLogin();
                });
            } else {
                auto* mainView  = new MainView(user);
                auto* presenter = new ReservationPresenter(mainView,
                                                           resService.get(),
                                                           tableService.get(),
                                                           user);
                presenter->loadReservations();
                mainView->show();

                QObject::connect(mainView, &MainView::logoutRequested,
                                 [=, &showLogin]() {
                    mainView->close();
                    presenter->deleteLater();
                    mainView->deleteLater();
                    showLogin();
                });
            }

            loginPresenter->deleteLater();
            loginView->deleteLater();
        });

        loginView->show();
    };

    showLogin();
    return app.exec();
}
