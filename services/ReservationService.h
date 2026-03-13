#pragma once
#include <optional>
#include <QList>
#include "models/Reservation.h"
#include "models/Table.h"

class IReservationRepository;
class ITableRepository;

class ReservationService {
public:
    ReservationService(IReservationRepository* resRepo, ITableRepository* tableRepo);

    bool isAvailable(int tableId, const QDate& date, const QTime& start, const QTime& end,
                     int guestCount, int excludeReservationId = 0) const;

    QList<Table> getAvailableTables(const QDate& date, const QTime& start,
                                    const QTime& end, int guestCount) const;

    std::optional<Reservation> createReservation(int userId, int tableId, const QDate& date,
                                                  const QTime& start, const QTime& end,
                                                  int guestCount);

    bool cancelReservation(int reservationId, int requestingUserId, bool isAdmin);

    bool editReservation(Reservation& reservation, int tableId, const QDate& date,
                         const QTime& start, const QTime& end, int guestCount, bool isAdmin);

    std::optional<Reservation> getReservationById(int id) const;
    QList<Reservation> getUserReservations(int userId) const;
    QList<Reservation> getAllReservations() const;
    QList<Reservation> getReservationsByDate(const QDate& date) const;

protected:
    IReservationRepository* m_resRepo;
    ITableRepository*       m_tableRepo;
};
