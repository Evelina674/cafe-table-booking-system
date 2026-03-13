#include "MainView.h"
#include "ui_MainView.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QMap>
#include <QColor>

MainView::MainView(const User& user, QWidget* parent) : QWidget(parent), ui(new Ui::MainView) {
    ui->setupUi(this);
    setWindowTitle(QString("Кафе - %1").arg(user.username));
    ui->titleLabel->setText(QString("Мої бронювання  |  %1").arg(user.username));

    connect(ui->createBtn,  &QPushButton::clicked, this, &MainView::createReservationRequested);
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainView::refreshRequested);
    connect(ui->logoutBtn,  &QPushButton::clicked, this, &MainView::logoutRequested);

    connect(ui->editBtn, &QPushButton::clicked, this, [this] {
        int id = selectedReservationId();
        if (id > 0) emit editReservationRequested(id);
    });
    connect(ui->cancelBtn, &QPushButton::clicked, this, [this] {
        int id = selectedReservationId();
        if (id > 0) emit cancelReservationRequested(id);
    });
}

MainView::~MainView() {
    delete ui;
}

void MainView::setTitle(const QString& text) {
    ui->titleLabel->setText(text);
}

void MainView::setReservations(const QList<Reservation>& reservations, const QList<Table>& tables) {
    m_reservationIds.clear();
    ui->reservationsTable->setRowCount(0);

    QMap<int, Table> tableMap;
    for (const auto& t : tables) tableMap[t.id] = t;

    for (const auto& r : reservations) {
        int row = ui->reservationsTable->rowCount();
        ui->reservationsTable->insertRow(row);
        m_reservationIds.append(r.id);

        QString tableDesc;
        if (tableMap.contains(r.tableId)) {
            const auto& t = tableMap[r.tableId];
            tableDesc = QString("№%1 (%2 міс.)").arg(t.number).arg(t.capacity);
        }

        auto makeItem = [](const QString& s) { return new QTableWidgetItem(s); };
        ui->reservationsTable->setItem(row, 0, makeItem(tableDesc));
        ui->reservationsTable->setItem(row, 1, makeItem(r.date.toString("dd.MM.yyyy")));
        ui->reservationsTable->setItem(row, 2, makeItem(r.timeStart.toString("HH:mm")));
        ui->reservationsTable->setItem(row, 3, makeItem(r.timeEnd.toString("HH:mm")));
        ui->reservationsTable->setItem(row, 4, makeItem(QString::number(r.guestCount)));

        QString statusText;
        if (!r.isActive())   statusText = "Скасовано";
        else if (r.isPast()) statusText = "Минуло";
        else                 statusText = "Активне";
        ui->reservationsTable->setItem(row, 5, makeItem(statusText));
        ui->reservationsTable->setItem(row, 6, makeItem(r.createdAt));

        if (!r.isActive()) {
            for (int col = 0; col < ui->reservationsTable->columnCount(); ++col) {
                if (auto* it = ui->reservationsTable->item(row, col))
                    it->setForeground(Qt::gray);
            }
        } else if (r.isPast()) {
            for (int col = 0; col < ui->reservationsTable->columnCount(); ++col) {
                if (auto* it = ui->reservationsTable->item(row, col))
                    it->setForeground(QColor(140, 100, 0));
            }
        }
    }
    ui->reservationsTable->resizeColumnsToContents();
}

int MainView::selectedReservationId() const {
    int row = ui->reservationsTable->currentRow();
    if (row < 0 || row >= m_reservationIds.size()) return -1;
    return m_reservationIds[row];
}
