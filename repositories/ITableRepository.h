#pragma once
#include <optional>
#include <QList>
#include "models/Table.h"

class ITableRepository {
public:
    virtual ~ITableRepository() = default;
    virtual std::optional<Table> findById(int id) const = 0;
    virtual QList<Table> findAll() const = 0;
    virtual bool save(Table& table) = 0;
    virtual bool remove(int id) = 0;
};
