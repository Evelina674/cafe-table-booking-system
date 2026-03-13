#include "TableManagementView.h"
#include "ui_TableManagementView.h"
#include <QPushButton>
#include <QTableWidgetItem>

TableManagementView::TableManagementView(QWidget* parent) : QWidget(parent), ui(new Ui::TableManagementView) {
    ui->setupUi(this);

    connect(ui->addBtn, &QPushButton::clicked, this, &TableManagementView::addTableRequested);
    connect(ui->editBtn, &QPushButton::clicked, this, [this] {
        int id = selectedTableId();
        if (id > 0) emit editTableRequested(id);
    });
    connect(ui->deleteBtn, &QPushButton::clicked, this, [this] {
        int id = selectedTableId();
        if (id > 0) emit deleteTableRequested(id);
    });
}

TableManagementView::~TableManagementView() {
    delete ui;
}

void TableManagementView::setTables(const QList<Table>& tables) {
    m_tableIds.clear();
    ui->tableWidget->setRowCount(0);
    for (const auto& t : tables) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        m_tableIds.append(t.id);
        auto* item0 = new QTableWidgetItem(QString::number(t.id));
        auto* item1 = new QTableWidgetItem(QString::number(t.number));
        auto* item2 = new QTableWidgetItem(QString::number(t.capacity));
        auto* item3 = new QTableWidgetItem(t.description);

        item0->setForeground(Qt::black);
        item1->setForeground(Qt::black);
        item2->setForeground(Qt::black);
        item3->setForeground(Qt::black);

        ui->tableWidget->setItem(row, 0, item0);
        ui->tableWidget->setItem(row, 1, item1);
        ui->tableWidget->setItem(row, 2, item2);
        ui->tableWidget->setItem(row, 3, item3);
    }
    ui->tableWidget->resizeColumnsToContents();
}

int TableManagementView::selectedTableId() const {
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= m_tableIds.size()) return -1;
    return m_tableIds[row];
}
