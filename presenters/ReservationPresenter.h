#pragma once
#include <QObject>
#include "models/User.h"

class MainView;
class ReservationService;
class TableService;

class ReservationPresenter : public QObject {
    Q_OBJECT
public:
    ReservationPresenter(MainView* view,
                         ReservationService* resService,
                         TableService* tableService,
                         const User& currentUser,
                         QObject* parent = nullptr);

    virtual void loadReservations();

protected slots:
    virtual void onCreateReservation();
    virtual void onEditReservation(int reservationId);
    virtual void onCancelReservation(int reservationId);

protected:
    MainView*           m_view;
    ReservationService* m_resService;
    TableService*       m_tableService;
    User                m_currentUser;
};
