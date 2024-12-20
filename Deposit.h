#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <string>

class Deposit {
private:
    int id;
    std::string clientName;
    double amount;
    double interestRate;

public:
    Deposit(int id, const std::string &name, double amount, double rate);
    Deposit(const std::string &name, double amount, double rate);

    int getId() const;
    std::string getClientName() const;
    double getAmount() const;
    double getInterestRate() const;
};

#endif
