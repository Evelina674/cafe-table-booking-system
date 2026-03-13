#include "AdminView.h"
#include "ui_AdminView.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAction>

AdminView::AdminView(const User& user, QWidget* parent) : QMainWindow(parent), ui(new Ui::AdminView) {
    ui->setupUi(this);
    setWindowTitle(QString("Кафе - Адміністратор (%1)").arg(user.username));

    // Вкладка 1: вбудовуємо MainView в контейнер з .ui
    m_reservationPanel = new MainView(user, ui->reservationsContainer);
    m_reservationPanel->setTitle("Всі бронювання");
    auto* resLayout = new QVBoxLayout(ui->reservationsContainer);
    resLayout->setContentsMargins(0, 0, 0, 0);
    resLayout->addWidget(m_reservationPanel);

    // Вкладка 2: вбудовуємо TableManagementView
    m_tablePanel = new TableManagementView(ui->tablesContainer);
    auto* tableLayout = new QVBoxLayout(ui->tablesContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    tableLayout->addWidget(m_tablePanel);

    // Вкладка 3: usersTable вже визначена в .ui
    m_usersTable = ui->usersTable;

    connect(ui->actionLogout, &QAction::triggered, this, &AdminView::logoutRequested);

    connect(m_tablePanel, &TableManagementView::addTableRequested,
            this, &AdminView::addTableRequested);
    connect(m_tablePanel, &TableManagementView::editTableRequested,
            this, &AdminView::editTableRequested);
    connect(m_tablePanel, &TableManagementView::deleteTableRequested,
            this, &AdminView::deleteTableRequested);
}

AdminView::~AdminView() {
    delete ui;
}

void AdminView::setTables(const QList<Table>& tables) {
    m_tablePanel->setTables(tables);
}

void AdminView::setUsers(const QList<User>& users) {
    m_usersTable->setRowCount(0);
    for (const auto& u : users) {
        int row = m_usersTable->rowCount();
        m_usersTable->insertRow(row);
        m_usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(u.id)));
        m_usersTable->setItem(row, 1, new QTableWidgetItem(u.username));
        m_usersTable->setItem(row, 2, new QTableWidgetItem(u.isAdmin() ? "Адмін" : "Користувач"));
    }
    m_usersTable->resizeColumnsToContents();
}
