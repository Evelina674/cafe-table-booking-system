#pragma once
#include <QMainWindow>
#include <QList>
#include "models/User.h"
#include "models/Reservation.h"
#include "models/Table.h"
#include "views/MainView.h"
#include "views/TableManagementView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminView; }
QT_END_NAMESPACE

class QTableWidget;

// AdminView : QMainWindow із вкладками.
// Перша вкладка - MainView (успадковує логіку відображення бронювань),
// що дозволяє AdminPresenter успадковувати ReservationPresenter без змін (SOLID-L).
class AdminView : public QMainWindow {
    Q_OBJECT
public:
    explicit AdminView(const User& user, QWidget* parent = nullptr);
    ~AdminView();

    // Повертає embedded MainView (вкладка "Бронювання") - для ReservationPresenter base
    MainView* reservationPanel() const { return m_reservationPanel; }

    void setTables(const QList<Table>& tables);
    void setUsers(const QList<User>& users);

signals:
    void addTableRequested();
    void editTableRequested(int tableId);
    void deleteTableRequested(int tableId);
    void logoutRequested();

private:
    Ui::AdminView*        ui;
    MainView*             m_reservationPanel;
    TableManagementView*  m_tablePanel;
    QTableWidget*         m_usersTable;
};
