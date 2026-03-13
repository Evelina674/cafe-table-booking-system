#pragma once
#include <QSqlDatabase>
#include <QString>

// Singleton - єдине з'єднання з БД протягом усього часу роботи (GoF: Singleton)
class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool initialize(const QString& path = "cafe.db");
    QSqlDatabase& database();

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool createSchema();
    bool seedData();

    QSqlDatabase m_db;
    bool m_initialized = false;
};
