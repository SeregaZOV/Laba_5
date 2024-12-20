#include "Bank.h"
#include <iostream>
#include <sstream>

using namespace std;

// Конструктор: подключение к базе данных
Bank::Bank(const string &dbPath) : dbPath(dbPath), db(nullptr) {
    connect();
}

// Деструктор: закрытие соединения с базой данных
Bank::~Bank() {
    disconnect();
}

// Метод для подключения к базе данных SQLite
void Bank::connect() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
}

// Метод для отключения от базы данных
void Bank::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

// Метод для добавления нового депозита в базу данных
void Bank::addDeposit(const Deposit &deposit) {
    string sql = "INSERT INTO deposits (client_name, amount, interest_rate) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, deposit.getClientName().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, deposit.getAmount());
        sqlite3_bind_double(stmt, 3, deposit.getInterestRate());

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Депозит успешно добавлен." << endl;
        } else {
            cerr << "Ошибка при добавлении депозита: " << sqlite3_errmsg(db) << endl;
        }
    }
    sqlite3_finalize(stmt);
}

// Метод для редактирования депозита
void Bank::editDeposit(int id, const Deposit &updatedDeposit) {
    string sql = "UPDATE deposits SET client_name = ?, amount = ?, interest_rate = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, updatedDeposit.getClientName().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, updatedDeposit.getAmount());
        sqlite3_bind_double(stmt, 3, updatedDeposit.getInterestRate());
        sqlite3_bind_int(stmt, 4, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Депозит с ID " << id << " успешно изменён." << endl;
        } else {
            cerr << "Ошибка при редактировании депозита: " << sqlite3_errmsg(db) << endl;
        }
    }
    sqlite3_finalize(stmt);
}

// Метод для удаления депозита из базы данных
void Bank::removeDeposit(int id) {
    string sql = "DELETE FROM deposits WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Депозит с ID " << id << " успешно удалён из базы данных." << endl;
        } else {
            cerr << "Ошибка при удалении депозита: " << sqlite3_errmsg(db) << endl;
        }
    }
    sqlite3_finalize(stmt);
}

// Метод для получения всех депозитов из базы данных
vector<Deposit> Bank::getAllDeposits() {
    vector<Deposit> deposits;
    string sql = "SELECT id, client_name, amount, interest_rate FROM deposits;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            double amount = sqlite3_column_double(stmt, 2);
            double rate = sqlite3_column_double(stmt, 3);

            deposits.emplace_back(id, name, amount, rate);
        }
    } else {
        cerr << "Ошибка при выполнении запроса на получение депозитов: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return deposits;
}
