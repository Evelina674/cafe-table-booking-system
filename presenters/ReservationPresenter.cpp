#include "ReservationPresenter.h"
#include "views/MainView.h"
#include "views/ReservationDialog.h"
#include "services/ReservationService.h"
#include "services/TableService.h"
#include "models/Reservation.h"
#include <QMessageBox>

ReservationPresenter::ReservationPresenter(MainView* view,
                                           ReservationService* resService,
                                           TableService* tableService,
                                           const User& currentUser,
                                           QObject* parent)
    : QObject(parent)
    , m_view(view)
    , m_resService(resService)
    , m_tableService(tableService)
    , m_currentUser(currentUser)
{
    connect(m_view, &MainView::createReservationRequested,
            this, &ReservationPresenter::onCreateReservation);
    connect(m_view, &MainView::editReservationRequested,
            this, &ReservationPresenter::onEditReservation);
    connect(m_view, &MainView::cancelReservationRequested,
            this, &ReservationPresenter::onCancelReservation);
    connect(m_view, &MainView::refreshRequested,
            this, &ReservationPresenter::loadReservations);
}

void ReservationPresenter::loadReservations() {
    auto reservations = m_resService->getUserReservations(m_currentUser.id);
    auto tables       = m_tableService->getAllTables();
    m_view->setReservations(reservations, tables);
}

void ReservationPresenter::onCreateReservation() {
    auto tables = m_tableService->getAllTables();
    ReservationDialog dlg(tables, m_view);

    connect(&dlg, &ReservationDialog::searchAvailableRequested,
            [&dlg, this](const QDate& date, const QTime& start,
                         const QTime& end, int guests) {
                dlg.setAvailableTables(m_resService->getAvailableTables(date, start, end, guests));
            });

    if (dlg.exec() != QDialog::Accepted) return;

    if (dlg.tableId() <= 0) {
        QMessageBox::warning(m_view, "Помилка", "Оберіть столик.");
        return;
    }
    if (dlg.timeStart() >= dlg.timeEnd()) {
        QMessageBox::warning(m_view, "Помилка", "Час початку має бути раніше кінця.");
        return;
    }

    auto res = m_resService->createReservation(m_currentUser.id, dlg.tableId(),
                                               dlg.date(), dlg.timeStart(),
                                               dlg.timeEnd(), dlg.guestCount());
    if (!res) {
        QMessageBox::warning(m_view, "Помилка",
                             "Не вдалося створити бронювання.\nПеревірте доступність столика.");
        return;
    }
    loadReservations();
    QMessageBox::information(m_view, "Успіх", "Бронювання створено.");
}

void ReservationPresenter::onEditReservation(int reservationId) {
    auto resOpt = m_resService->getReservationById(reservationId);
    if (!resOpt) return;

    if (!m_currentUser.isAdmin() && resOpt->status != ReservationStatus::Active) {
        QMessageBox::information(m_view, "Інформація",
                                 "Редагувати можна лише активні бронювання.");
        return;
    }
    if (resOpt->isPast()) {
        QMessageBox::information(m_view, "Інформація",
                                 "Не можна редагувати бронювання, час якого вже минув.");
        return;
    }

    auto tables = m_tableService->getAllTables();
    ReservationDialog dlg(tables, m_view);
    dlg.setReservation(*resOpt);

    connect(&dlg, &ReservationDialog::searchAvailableRequested,
            [&dlg, this](const QDate& date, const QTime& start,
                         const QTime& end, int guests) {
                dlg.setAvailableTables(m_resService->getAvailableTables(date, start, end, guests));
            });

    if (dlg.exec() != QDialog::Accepted) return;

    if (dlg.tableId() <= 0) {
        QMessageBox::warning(m_view, "Помилка", "Оберіть столик.");
        return;
    }
    if (dlg.timeStart() >= dlg.timeEnd()) {
        QMessageBox::warning(m_view, "Помилка", "Час початку має бути раніше кінця.");
        return;
    }

    Reservation r = *resOpt;
    if (!m_resService->editReservation(r, dlg.tableId(), dlg.date(),
                                       dlg.timeStart(), dlg.timeEnd(),
                                       dlg.guestCount(), m_currentUser.isAdmin())) {
        QMessageBox::warning(m_view, "Помилка",
                             "Не вдалося оновити бронювання.\nПеревірте доступність столика.");
        return;
    }
    loadReservations();
    QMessageBox::information(m_view, "Успіх", "Бронювання оновлено.");
}

void ReservationPresenter::onCancelReservation(int reservationId) {
    auto resOpt = m_resService->getReservationById(reservationId);
    if (resOpt && resOpt->isPast()) {
        QMessageBox::information(m_view, "Інформація",
                                 "Не можна скасувати бронювання, час якого вже минув.");
        return;
    }

    auto reply = QMessageBox::question(m_view, "Підтвердження", "Скасувати бронювання?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (!m_resService->cancelReservation(reservationId, m_currentUser.id,
                                          m_currentUser.isAdmin())) {
        QMessageBox::warning(m_view, "Помилка", "Не вдалося скасувати бронювання.");
        return;
    }
    loadReservations();
}
