#include "BankAccount.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <limits>

// --- Base BankAccount Implementation ---

BankAccount::BankAccount() : accountNumber("0000"), balance(0.0) {
    accountHolderName = new char[8];
    std::strcpy(accountHolderName, "Unknown");
}

BankAccount::BankAccount(const std::string& accNum, const std::string& holderName, double initialBalance)
    : accountNumber(accNum), balance(initialBalance >= 0 ? initialBalance : 0.0) {
    accountHolderName = new char[holderName.length() + 1];
    std::strcpy(accountHolderName, holderName.c_str());
}

BankAccount::BankAccount(const std::string& other)
    : accountNumber(other.accountNumber), balance(other.balance) {
    accountHolderName = new char[std::strlen(other.accountHolderName) + 1];
    std::strcpy(accountHolderName, other.accountHolderName);
}

BankAccount& BankAccount::operator=(const BankAccount& other) {
    if (this != &other) {
        delete[] accountHolderName;
        accountNumber = other.accountNumber;
        balance = other.balance;
        accountHolderName = new char[std::strlen(other.accountHolderName) + 1];
        std::strcpy(accountHolderName, other.accountHolderName);
    }
    return *this;
}

BankAccount::~BankAccount() {
    delete[] accountHolderName;
}

std::string BankAccount::getAccountNumber() const { return accountNumber; }
std::string BankAccount::getAccountHolderName() const { return std::string(accountHolderName); }
double BankAccount::getBalance() const { return balance; }

void BankAccount::setAccountHolderName(const std::string& newName) {
    delete[] accountHolderName;
    accountHolderName = new char[newName.length() + 1];
    std::strcpy(accountHolderName, newName.c_str());
}

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "Successfully deposited $" << std::fixed << std::setprecision(2) << amount << ".\n";
    } else {
        std::cout << "Invalid deposit amount.\n";
    }
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "Insufficient funds for withdrawal.\n";
        return false;
    }
    balance -= amount;
    std::cout << "Successfully withdrew $" << std::fixed << std::setprecision(2) << amount << ".\n";
    return true;
}

void BankAccount::printAccount() const {
    std::cout << "[Standard Account] Acc #: " << accountNumber
              << " | Holder: " << accountHolderName
              << " | Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
}

BankAccount& BankAccount::operator+=(double amount) {
    deposit(amount);
    return *this;
}

BankAccount& BankAccount::operator-=(double amount) {
    withdraw(amount);
    return *this;
}

bool BankAccount::operator==(const BankAccount& other) const {
    return this->accountNumber == other.accountNumber;
}

bool BankAccount::operator<(const BankAccount& other) const {
    return this->balance < other.balance;
}

bool BankAccount::operator>(const BankAccount& other) const {
    return this->balance > other.balance;
}

void BankAccount::printAccount(const BankAccount& account) {
    account.printAccount();
}

BankAccount BankAccount::createAccountFromInput() {
    std::string accNum, holderName;
    double initialBalance;

    std::cout << "Enter Account Number: ";
    std::cin >> accNum;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Account Holder Name: ";
    std::getline(std::cin, holderName);

    std::cout << "Enter Initial Balance: $";
    while (!(std::cin >> initialBalance) || initialBalance < 0) {
        std::cout << "Invalid amount. Enter a non-negative balance: $";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return BankAccount(accNum, holderName, initialBalance);
}

// --- CheckingAccount Implementation ---

CheckingAccount::CheckingAccount(const std::string& accNum, const std::string& holderName, double initialBalance, double fee)
    : BankAccount(accNum, holderName, initialBalance), transactionFee(fee >= 0 ? fee : 0.0) {}

bool CheckingAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "Invalid withdrawal amount.\n";
        return false;
    }
    double totalDeduction = amount + transactionFee;
    if (totalDeduction > balance) {
        std::cout << "Insufficient funds (requires $" << std::fixed << std::setprecision(2) 
                  << amount << " + $" << transactionFee << " fee).\n";
        return false;
    }
    balance -= totalDeduction;
    std::cout << "Withdrew $" << std::fixed << std::setprecision(2) << amount 
              << " (Transaction Fee of $" << transactionFee << " applied).\n";
    return true;
}

void CheckingAccount::printAccount() const {
    std::cout << "[Checking Account] Acc #: " << accountNumber
              << " | Holder: " << accountHolderName
              << " | Balance: $" << std::fixed << std::setprecision(2) << balance
              << " | Fee: $" << transactionFee << "\n";
}

// --- SavingsAccount Implementation ---

SavingsAccount::SavingsAccount(const std::string& accNum, const std::string& holderName, double initialBalance, double rate)
    : BankAccount(accNum, holderName, initialBalance), interestRate(rate >= 0 ? rate : 0.0) {}

void SavingsAccount::calculateInterest() {
    double interest = balance * interestRate;
    balance += interest;
    std::cout << "Interest of $" << std::fixed << std::setprecision(2) << interest 
              << " (" << (interestRate * 100) << "%) added to Savings Account.\n";
}

void SavingsAccount::printAccount() const {
    std::cout << "[Savings Account]  Acc #: " << accountNumber
              << " | Holder: " << accountHolderName
              << " | Balance: $" << std::fixed << std::setprecision(2) << balance
              << " | Rate: " << (interestRate * 100) << "%\n";
}
