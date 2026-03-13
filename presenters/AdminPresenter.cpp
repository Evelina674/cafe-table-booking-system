#include "AdminPresenter.h"
#include "views/AdminView.h"
#include "views/ReservationDialog.h"
#include "services/ReservationService.h"
#include "services/TableService.h"
#include "repositories/IUserRepository.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

AdminPresenter::AdminPresenter(AdminView* adminView,
                               ReservationService* resService,
                               TableService* tableService,
                               IUserRepository* userRepo,
                               const User& currentUser,
                               QObject* parent)
    : ReservationPresenter(adminView->reservationPanel(), resService, tableService,
                           currentUser, parent)
    , m_adminView(adminView)
    , m_userRepo(userRepo)
{
    connect(m_adminView, &AdminView::addTableRequested,
            this, &AdminPresenter::onAddTable);
    connect(m_adminView, &AdminView::editTableRequested,
            this, &AdminPresenter::onEditTable);
    connect(m_adminView, &AdminView::deleteTableRequested,
            this, &AdminPresenter::onDeleteTable);
}

void AdminPresenter::loadReservations() {
    auto reservations = m_resService->getAllReservations();
    auto tables       = m_tableService->getAllTables();
    m_view->setReservations(reservations, tables);
}

void AdminPresenter::loadAll() {
    loadReservations();
    m_adminView->setTables(m_tableService->getAllTables());
    m_adminView->setUsers(m_userRepo->findAll());
}

void AdminPresenter::onAddTable() {
    bool ok;
    int number = QInputDialog::getInt(m_adminView, "Новий столик",
                                      "Номер столика:", 1, 1, 999, 1, &ok);
    if (!ok) return;

    int capacity = QInputDialog::getInt(m_adminView, "Новий столик",
                                        "Кількість місць:", 2, 1, 50, 1, &ok);
    if (!ok) return;

    QString desc = QInputDialog::getText(m_adminView, "Новий столик",
                                         "Опис:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    if (!m_tableService->addTable(number, capacity, desc)) {
        QMessageBox::warning(m_adminView, "Помилка",
                             "Не вдалося додати столик.\n(Можливо, номер вже існує)");
        return;
    }
    loadAll();
}

void AdminPresenter::onEditTable(int tableId) {
    auto opt = m_tableService->getTableById(tableId);
    if (!opt) return;

    bool ok;
    int number = QInputDialog::getInt(m_adminView, "Редагування столика",
                                      "Номер:", opt->number, 1, 999, 1, &ok);
    if (!ok) return;

    int capacity = QInputDialog::getInt(m_adminView, "Редагування столика",
                                        "Місць:", opt->capacity, 1, 50, 1, &ok);
    if (!ok) return;

    QString desc = QInputDialog::getText(m_adminView, "Редагування столика",
                                         "Опис:", QLineEdit::Normal, opt->description, &ok);
    if (!ok) return;

    if (!m_tableService->editTable(tableId, number, capacity, desc)) {
        QMessageBox::warning(m_adminView, "Помилка", "Не вдалося оновити столик.");
        return;
    }
    loadAll();
}

void AdminPresenter::onDeleteTable(int tableId) {
    auto reply = QMessageBox::question(m_adminView, "Підтвердження", "Видалити столик?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (!m_tableService->deleteTable(tableId)) {
        QMessageBox::warning(m_adminView, "Помилка",
                             "Не вдалося видалити столик.\n(Є активні бронювання?)");
        return;
    }
    loadAll();
}
