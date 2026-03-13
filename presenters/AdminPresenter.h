#pragma once
#include "ReservationPresenter.h"

class AdminView;
class IUserRepository;

// AdminPresenter успадковує ReservationPresenter (SOLID-L):
// надає всі операції з бронюваннями + адмін-специфічні функції.
class AdminPresenter : public ReservationPresenter {
    Q_OBJECT
public:
    AdminPresenter(AdminView* adminView,
                   ReservationService* resService,
                   TableService* tableService,
                   IUserRepository* userRepo,
                   const User& currentUser,
                   QObject* parent = nullptr);

    void loadReservations() override;
    void loadAll();

private slots:
    void onAddTable();
    void onEditTable(int tableId);
    void onDeleteTable(int tableId);

private:
    AdminView*       m_adminView;
    IUserRepository* m_userRepo;
};
