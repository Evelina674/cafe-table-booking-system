#pragma once
#include "ITableRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class TableRepository : public ITableRepository {
public:
    explicit TableRepository(QSqlDatabase& db);

    std::optional<Table> findById(int id) const override;
    QList<Table> findAll() const override;
    bool save(Table& table) override;
    bool remove(int id) override;

private:
    Table fromQuery(const QSqlQuery& q) const;
    QSqlDatabase& m_db;
};
