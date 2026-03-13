#pragma once
#include <QDialog>
#include <QList>
#include "models/Reservation.h"
#include "models/Table.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ReservationDialog; }
QT_END_NAMESPACE

class ReservationDialog : public QDialog {
    Q_OBJECT
public:
    explicit ReservationDialog(const QList<Table>& allTables, QWidget* parent = nullptr);
    ~ReservationDialog();

    void setReservation(const Reservation& r);

    int tableId() const;
    QDate date() const;
    QTime timeStart() const;
    QTime timeEnd() const;
    int guestCount() const;

signals:
    void searchAvailableRequested(const QDate& date, const QTime& start,
                                  const QTime& end, int guestCount);

public slots:
    void setAvailableTables(const QList<Table>& tables);

private:
    Ui::ReservationDialog* ui;
    QList<int> m_tableIds;
};
