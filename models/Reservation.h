#pragma once
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>

enum class ReservationStatus { Active, Cancelled };

struct Reservation {
    int id = 0;
    int userId = 0;
    int tableId = 0;
    QDate date;
    QTime timeStart;
    QTime timeEnd;
    int guestCount = 0;
    ReservationStatus status = ReservationStatus::Active;
    QString createdAt;

    bool isValid() const { return id > 0; }
    bool isActive() const { return status == ReservationStatus::Active; }
    bool isPast() const {
        return QDateTime(date, timeEnd) < QDateTime::currentDateTime();
    }
};
