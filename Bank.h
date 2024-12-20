#ifndef BANK_H
#define BANK_H

#include "Deposit.h"
#include <sqlite3.h>  // Подключение библиотеки SQLite
#include <vector>
#include <string>

class Bank {
public:
    // Конструктор и деструктор
    Bank(const std::string &dbPath);
    ~Bank();

    // Методы для работы с депозитами
    void addDeposit(const Deposit &deposit);
    void editDeposit(int id, const Deposit &updatedDeposit);
    void removeDeposit(int id);
    std::vector<Deposit> getAllDeposits();

private:
    std::string dbPath;  // Путь к базе данных
    sqlite3 *db;         // Указатель на соединение с базой данных

    // Приватные методы для управления соединением с БД
    void connect();
    void disconnect();
};

// Глобальная переменная bank (объявление)
extern Bank bank;

#endif
