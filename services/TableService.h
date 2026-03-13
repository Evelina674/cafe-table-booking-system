#pragma once
#include <optional>
#include <QList>
#include "models/Table.h"

class ITableRepository;

class TableService {
public:
    explicit TableService(ITableRepository* tableRepo);

    QList<Table> getAllTables() const;
    std::optional<Table> getTableById(int id) const;
    bool addTable(int number, int capacity, const QString& description);
    bool editTable(int id, int number, int capacity, const QString& description);
    bool deleteTable(int id);

private:
    ITableRepository* m_tableRepo;
};
