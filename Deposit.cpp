#include "Deposit.h"

Deposit::Deposit(int id, const std::string &name, double amount, double rate)
    : id(id), clientName(name), amount(amount), interestRate(rate) {}

Deposit::Deposit(const std::string &name, double amount, double rate)
    : id(-1), clientName(name), amount(amount), interestRate(rate) {}

int Deposit::getId() const {
    return id;
}

std::string Deposit::getClientName() const {
    return clientName;
}

double Deposit::getAmount() const {
    return amount;
}

double Deposit::getInterestRate() const {
    return interestRate;
}
