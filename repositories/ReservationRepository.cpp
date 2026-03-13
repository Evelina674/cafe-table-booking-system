#include "ReservationRepository.h"
#include <QSqlError>
#include <QDebug>

ReservationRepository::ReservationRepository(QSqlDatabase& db) : m_db(db) {}

Reservation ReservationRepository::fromQuery(const QSqlQuery& q) const {
    Reservation r;
    r.id = q.value("id").toInt();
    r.userId = q.value("user_id").toInt();
    r.tableId = q.value("table_id").toInt();
    r.date = QDate::fromString(q.value("date").toString(), Qt::ISODate);
    r.timeStart = QTime::fromString(q.value("time_start").toString(), "HH:mm");
    r.timeEnd = QTime::fromString(q.value("time_end").toString(), "HH:mm");
    r.guestCount = q.value("guest_count").toInt();
    r.status = (q.value("status").toString() == "active")
                   ? ReservationStatus::Active
                   : ReservationStatus::Cancelled;
    r.createdAt = q.value("created_at").toString();
    return r;
}

std::optional<Reservation> ReservationRepository::findById(int id) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM reservations WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) return fromQuery(q);
    return std::nullopt;
}

QList<Reservation> ReservationRepository::findByUserId(int userId) const {
    QList<Reservation> result;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM reservations WHERE user_id = :uid ORDER BY date DESC, time_start DESC");
    q.bindValue(":uid", userId);
    if (q.exec()) while (q.next()) result.append(fromQuery(q));
    return result;
}

QList<Reservation> ReservationRepository::findByDate(const QDate& date) const {
    QList<Reservation> result;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM reservations WHERE date = :d ORDER BY time_start");
    q.bindValue(":d", date.toString(Qt::ISODate));
    if (q.exec()) while (q.next()) result.append(fromQuery(q));
    return result;
}

QList<Reservation> ReservationRepository::findByTableAndDate(int tableId, const QDate& date) const {
    QList<Reservation> result;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM reservations WHERE table_id = :tid AND date = :d AND status = 'active'");
    q.bindValue(":tid", tableId);
    q.bindValue(":d", date.toString(Qt::ISODate));
    if (q.exec()) while (q.next()) result.append(fromQuery(q));
    return result;
}

QList<Reservation> ReservationRepository::findAll() const {
    QList<Reservation> result;
    QSqlQuery q("SELECT * FROM reservations ORDER BY date DESC, time_start DESC", m_db);
    while (q.next()) result.append(fromQuery(q));
    return result;
}

bool ReservationRepository::save(Reservation& reservation) {
    QSqlQuery q(m_db);
    q.prepare(R"(INSERT INTO reservations
        (user_id, table_id, date, time_start, time_end, guest_count, status, created_at)
        VALUES (:uid, :tid, :d, :ts, :te, :gc, :st, :ca))");
    q.bindValue(":uid", reservation.userId);
    q.bindValue(":tid", reservation.tableId);
    q.bindValue(":d", reservation.date.toString(Qt::ISODate));
    q.bindValue(":ts", reservation.timeStart.toString("HH:mm"));
    q.bindValue(":te", reservation.timeEnd.toString("HH:mm"));
    q.bindValue(":gc", reservation.guestCount);
    q.bindValue(":st", reservation.status == ReservationStatus::Active ? "active" : "cancelled");
    q.bindValue(":ca", reservation.createdAt);
    if (!q.exec()) {
        qWarning() << "ReservationRepository::save:" << q.lastError().text();
        return false;
    }
    reservation.id = q.lastInsertId().toInt();
    return true;
}

bool ReservationRepository::update(const Reservation& reservation) {
    QSqlQuery q(m_db);
    q.prepare(R"(UPDATE reservations SET
        table_id=:tid, date=:d, time_start=:ts, time_end=:te,
        guest_count=:gc, status=:st WHERE id=:id)");
    q.bindValue(":tid", reservation.tableId);
    q.bindValue(":d", reservation.date.toString(Qt::ISODate));
    q.bindValue(":ts", reservation.timeStart.toString("HH:mm"));
    q.bindValue(":te", reservation.timeEnd.toString("HH:mm"));
    q.bindValue(":gc", reservation.guestCount);
    q.bindValue(":st", reservation.status == ReservationStatus::Active ? "active" : "cancelled");
    q.bindValue(":id", reservation.id);
    if (!q.exec()) {
        qWarning() << "ReservationRepository::update:" << q.lastError().text();
        return false;
    }
    return true;
}

bool ReservationRepository::remove(int id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM reservations WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
