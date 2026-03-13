#pragma once
#include <QWidget>
#include <QList>
#include "models/Table.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TableManagementView; }
QT_END_NAMESPACE

// Вбудований віджет для вкладки "Столики" в AdminView
class TableManagementView : public QWidget {
    Q_OBJECT
public:
    explicit TableManagementView(QWidget* parent = nullptr);
    ~TableManagementView();

    void setTables(const QList<Table>& tables);
    int selectedTableId() const;

signals:
    void addTableRequested();
    void editTableRequested(int tableId);
    void deleteTableRequested(int tableId);

private:
    Ui::TableManagementView* ui;
    QList<int> m_tableIds;
};
