#include "database.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Database::Database(const string &filename) : db(nullptr), isOpen(false) {
    char currentPath[FILENAME_MAX];
    getcwd(currentPath, sizeof(currentPath));
    cout << "Текущая рабочая директория: " << currentPath << endl;
    cout << "Подключение к базе данных по пути: " << filename << endl;

    if (sqlite3_open(filename.c_str(), &db) == SQLITE_OK) {
        isOpen = true;
        cout << "База данных успешно открыта!" << endl;
    } else {
        cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << endl;
    }
}

Database::~Database() {
    if (isOpen) {
        sqlite3_close(db);
        cout << "Соединение с базой данных закрыто." << endl;
    }
}

sqlite3* Database::getConnection() const {
    return db;
}
