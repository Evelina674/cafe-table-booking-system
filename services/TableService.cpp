#include "TableService.h"
#include "repositories/ITableRepository.h"

TableService::TableService(ITableRepository* tableRepo) : m_tableRepo(tableRepo) {}

QList<Table> TableService::getAllTables() const { return m_tableRepo->findAll(); }

std::optional<Table> TableService::getTableById(int id) const {
    return m_tableRepo->findById(id);
}

bool TableService::addTable(int number, int capacity, const QString& description) {
    Table t;
    t.number = number;
    t.capacity = capacity;
    t.description = description;
    return m_tableRepo->save(t);
}

bool TableService::editTable(int id, int number, int capacity, const QString& description) {
    auto opt = m_tableRepo->findById(id);
    if (!opt) return false;
    Table t = *opt;
    t.number = number;
    t.capacity = capacity;
    t.description = description;
    return m_tableRepo->save(t);
}

bool TableService::deleteTable(int id) { return m_tableRepo->remove(id); }
