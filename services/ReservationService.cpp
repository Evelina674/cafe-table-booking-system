#include "ReservationService.h"
#include "repositories/IReservationRepository.h"
#include "repositories/ITableRepository.h"
#include <QDateTime>

ReservationService::ReservationService(IReservationRepository* resRepo,
                                       ITableRepository* tableRepo)
    : m_resRepo(resRepo), m_tableRepo(tableRepo) {}

bool ReservationService::isAvailable(int tableId, const QDate& date,
                                     const QTime& start, const QTime& end,
                                     int guestCount, int excludeReservationId) const {
    auto tableOpt = m_tableRepo->findById(tableId);
    if (!tableOpt) return false;
    if (tableOpt->capacity < guestCount) return false;

    for (const auto& r : m_resRepo->findByTableAndDate(tableId, date)) {
        if (r.id == excludeReservationId) continue;
        if (r.isPast()) continue;
        if (start < r.timeEnd && end > r.timeStart) return false;
    }
    return true;
}

QList<Table> ReservationService::getAvailableTables(const QDate& date, const QTime& start,
                                                    const QTime& end, int guestCount) const {
    QList<Table> result;
    for (const auto& table : m_tableRepo->findAll()) {
        if (isAvailable(table.id, date, start, end, guestCount))
            result.append(table);
    }
    return result;
}

std::optional<Reservation> ReservationService::createReservation(int userId, int tableId,
                                                                   const QDate& date,
                                                                   const QTime& start,
                                                                   const QTime& end,
                                                                   int guestCount) {
    if (!isAvailable(tableId, date, start, end, guestCount)) return std::nullopt;

    Reservation r;
    r.userId    = userId;
    r.tableId   = tableId;
    r.date      = date;
    r.timeStart = start;
    r.timeEnd   = end;
    r.guestCount = guestCount;
    r.status    = ReservationStatus::Active;
    r.createdAt = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (!m_resRepo->save(r)) return std::nullopt;
    return r;
}

bool ReservationService::cancelReservation(int reservationId, int requestingUserId,
                                            bool isAdmin) {
    auto resOpt = m_resRepo->findById(reservationId);
    if (!resOpt) return false;
    if (!isAdmin && resOpt->userId != requestingUserId) return false;
    if (resOpt->status == ReservationStatus::Cancelled) return false;

    Reservation r = *resOpt;
    r.status = ReservationStatus::Cancelled;
    return m_resRepo->update(r);
}

bool ReservationService::editReservation(Reservation& reservation, int tableId,
                                          const QDate& date, const QTime& start,
                                          const QTime& end, int guestCount, bool isAdmin) {
    if (!isAdmin && reservation.status != ReservationStatus::Active) return false;
    if (!isAvailable(tableId, date, start, end, guestCount, reservation.id)) return false;

    reservation.tableId   = tableId;
    reservation.date      = date;
    reservation.timeStart = start;
    reservation.timeEnd   = end;
    reservation.guestCount = guestCount;
    return m_resRepo->update(reservation);
}

std::optional<Reservation> ReservationService::getReservationById(int id) const {
    return m_resRepo->findById(id);
}

QList<Reservation> ReservationService::getUserReservations(int userId) const {
    return m_resRepo->findByUserId(userId);
}

QList<Reservation> ReservationService::getAllReservations() const {
    return m_resRepo->findAll();
}

QList<Reservation> ReservationService::getReservationsByDate(const QDate& date) const {
    return m_resRepo->findByDate(date);
}
