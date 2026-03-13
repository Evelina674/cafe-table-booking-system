#pragma once
#include <optional>
#include <QList>
#include <QDate>
#include "models/Reservation.h"

class IReservationRepository {
public:
    virtual ~IReservationRepository() = default;
    virtual std::optional<Reservation> findById(int id) const = 0;
    virtual QList<Reservation> findByUserId(int userId) const = 0;
    virtual QList<Reservation> findByDate(const QDate& date) const = 0;
    virtual QList<Reservation> findByTableAndDate(int tableId, const QDate& date) const = 0;
    virtual QList<Reservation> findAll() const = 0;
    virtual bool save(Reservation& reservation) = 0;
    virtual bool update(const Reservation& reservation) = 0;
    virtual bool remove(int id) = 0;
};
