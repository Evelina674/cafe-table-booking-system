#include "TableRepository.h"
#include <QSqlError>
#include <QDebug>

TableRepository::TableRepository(QSqlDatabase& db) : m_db(db) {}

Table TableRepository::fromQuery(const QSqlQuery& q) const {
    Table t;
    t.id = q.value("id").toInt();
    t.number = q.value("number").toInt();
    t.capacity = q.value("capacity").toInt();
    t.description = q.value("description").toString();
    return t;
}

std::optional<Table> TableRepository::findById(int id) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM tables WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) return fromQuery(q);
    return std::nullopt;
}

QList<Table> TableRepository::findAll() const {
    QList<Table> result;
    QSqlQuery q("SELECT * FROM tables ORDER BY number", m_db);
    while (q.next()) result.append(fromQuery(q));
    return result;
}

bool TableRepository::save(Table& table) {
    QSqlQuery q(m_db);
    if (table.id == 0) {
        q.prepare("INSERT INTO tables (number, capacity, description) VALUES (:n, :c, :d)");
    } else {
        q.prepare("UPDATE tables SET number=:n, capacity=:c, description=:d WHERE id=:id");
        q.bindValue(":id", table.id);
    }
    q.bindValue(":n", table.number);
    q.bindValue(":c", table.capacity);
    q.bindValue(":d", table.description);
    if (!q.exec()) {
        qWarning() << "TableRepository::save:" << q.lastError().text();
        return false;
    }
    if (table.id == 0) table.id = q.lastInsertId().toInt();
    return true;
}

bool TableRepository::remove(int id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM tables WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
