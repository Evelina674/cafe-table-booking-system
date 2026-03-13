#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::initialize(const QString& path) {
    if (m_initialized) return true;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qCritical() << "Не вдалося відкрити БД:" << m_db.lastError().text();
        return false;
    }

    if (!createSchema()) return false;

    m_initialized = true;
    return true;
}

QSqlDatabase& DatabaseManager::database() {
    return m_db;
}

bool DatabaseManager::createSchema() {
    QSqlQuery q(m_db);
    q.exec("PRAGMA foreign_keys = ON");

    const QStringList stmts = {
        R"(CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            role TEXT NOT NULL CHECK(role IN ('admin', 'user'))
        ))",
        R"(CREATE TABLE IF NOT EXISTS tables (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            number INTEGER NOT NULL UNIQUE,
            capacity INTEGER NOT NULL,
            description TEXT
        ))",
        R"(CREATE TABLE IF NOT EXISTS reservations (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL REFERENCES users(id),
            table_id INTEGER NOT NULL REFERENCES tables(id),
            date TEXT NOT NULL,
            time_start TEXT NOT NULL,
            time_end TEXT NOT NULL,
            guest_count INTEGER NOT NULL,
            status TEXT NOT NULL DEFAULT 'active' CHECK(status IN ('active', 'cancelled')),
            created_at TEXT NOT NULL
        ))"
    };

    for (const QString& sql : stmts) {
        if (!q.exec(sql)) {
            qCritical() << "Помилка схеми:" << q.lastError().text();
            return false;
        }
    }

    return seedData();
}

bool DatabaseManager::seedData() {
    QSqlQuery check(m_db);
    check.exec("SELECT COUNT(*) FROM users");
    if (check.next() && check.value(0).toInt() > 0)
        return true; // вже ініціалізовано

    // Адмін за замовчуванням: admin / admin123
    QString hash = QCryptographicHash::hash("admin123", QCryptographicHash::Sha256).toHex();
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO users (username, password_hash, role) VALUES (:u, :h, 'admin')");
    q.bindValue(":u", "admin");
    q.bindValue(":h", hash);
    if (!q.exec()) {
        qWarning() << "Не вдалося створити адміна:" << q.lastError().text();
        return false;
    }

    // Звичайні користувачі: ivan, maria, petro / qwerty123
    const QString userHash = QCryptographicHash::hash("qwerty123", QCryptographicHash::Sha256).toHex();
    const QStringList usernames = {"ivan", "maria", "petro"};
    for (const QString& name : usernames) {
        QSqlQuery uq(m_db);
        uq.prepare("INSERT INTO users (username, password_hash, role) VALUES (:u, :h, 'user')");
        uq.bindValue(":u", name);
        uq.bindValue(":h", userHash);
        uq.exec();
    }

    struct T { int num, cap; const char* desc; };
    const T tables[] = {
        {1, 2, "Столик біля вікна"},
        {2, 2, "Затишний куток"},
        {3, 4, "Центральний столик"},
        {4, 4, "Зала для компанії"},
        {5, 6, "Великий стіл"},
        {6, 6, "VIP-зона"},
        {7, 8, "Банкетний стіл"},
        {8, 3, "Барна стійка"},
        {9, 4, "Тераса"},
        {10, 5, "Сімейна зона"},
    };

    for (const auto& t : tables) {
        QSqlQuery tq(m_db);
        tq.prepare("INSERT INTO tables (number, capacity, description) VALUES (:n, :c, :d)");
        tq.bindValue(":n", t.num);
        tq.bindValue(":c", t.cap);
        tq.bindValue(":d", t.desc);
        tq.exec();
    }

    return true;
}
