#pragma once
#include <QString>

struct Table {
    int id = 0;
    int number = 0;
    int capacity = 0;
    QString description;

    bool isValid() const { return id > 0; }
};
