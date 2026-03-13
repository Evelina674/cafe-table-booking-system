#pragma once
#include <QWidget>
#include <QList>
#include "models/Reservation.h"
#include "models/Table.h"
#include "models/User.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainView; }
QT_END_NAMESPACE

// View для звичайного користувача (QWidget - може бути вбудований або показаний як вікно)
class MainView : public QWidget {
    Q_OBJECT
public:
    explicit MainView(const User& user, QWidget* parent = nullptr);
    ~MainView();

    void setReservations(const QList<Reservation>& reservations, const QList<Table>& tables);
    int selectedReservationId() const;
    void setTitle(const QString& text);

signals:
    void createReservationRequested();
    void editReservationRequested(int reservationId);
    void cancelReservationRequested(int reservationId);
    void refreshRequested();
    void logoutRequested();

protected:
    QList<int> m_reservationIds;

private:
    Ui::MainView* ui;
};
