#pragma once
#include "IReservationRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class ReservationRepository : public IReservationRepository {
public:
    explicit ReservationRepository(QSqlDatabase& db);

    std::optional<Reservation> findById(int id) const override;
    QList<Reservation> findByUserId(int userId) const override;
    QList<Reservation> findByDate(const QDate& date) const override;
    QList<Reservation> findByTableAndDate(int tableId, const QDate& date) const override;
    QList<Reservation> findAll() const override;
    bool save(Reservation& reservation) override;
    bool update(const Reservation& reservation) override;
    bool remove(int id) override;

private:
    Reservation fromQuery(const QSqlQuery& q) const;
    QSqlDatabase& m_db;
};
