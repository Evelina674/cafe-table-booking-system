#include "ReservationDialog.h"
#include "ui_ReservationDialog.h"
#include <QPushButton>
#include <QDialogButtonBox>

ReservationDialog::ReservationDialog(const QList<Table>& allTables, QWidget* parent)
    : QDialog(parent), ui(new Ui::ReservationDialog)
{
    ui->setupUi(this);

    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setDate(QDate::currentDate());

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Зберегти");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Скасувати");

    setAvailableTables(allTables);

    connect(ui->searchBtn, &QPushButton::clicked, this, [this] {
        emit searchAvailableRequested(ui->dateEdit->date(), ui->startEdit->time(),
                                      ui->endEdit->time(), ui->guestSpin->value());
    });
}

ReservationDialog::~ReservationDialog() {
    delete ui;
}

void ReservationDialog::setReservation(const Reservation& r) {
    ui->dateEdit->setDate(r.date);
    ui->startEdit->setTime(r.timeStart);
    ui->endEdit->setTime(r.timeEnd);
    ui->guestSpin->setValue(r.guestCount);
    for (int i = 0; i < m_tableIds.size(); ++i) {
        if (m_tableIds[i] == r.tableId) {
            ui->tableCombo->setCurrentIndex(i);
            break;
        }
    }
}

void ReservationDialog::setAvailableTables(const QList<Table>& tables) {
    ui->tableCombo->clear();
    m_tableIds.clear();
    for (const auto& t : tables) {
        ui->tableCombo->addItem(
            QString("Стіл №%1 (до %2 осіб) - %3").arg(t.number).arg(t.capacity).arg(t.description));
        m_tableIds.append(t.id);
    }
    ui->infoLabel->setText(tables.isEmpty() ? "Немає вільних столиків на вказаний час." : "");
}

int ReservationDialog::tableId() const {
    int idx = ui->tableCombo->currentIndex();
    return (idx >= 0 && idx < m_tableIds.size()) ? m_tableIds[idx] : -1;
}

QDate ReservationDialog::date()       const { return ui->dateEdit->date(); }
QTime ReservationDialog::timeStart()  const { return ui->startEdit->time(); }
QTime ReservationDialog::timeEnd()    const { return ui->endEdit->time(); }
int   ReservationDialog::guestCount() const { return ui->guestSpin->value(); }
