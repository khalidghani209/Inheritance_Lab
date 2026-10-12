#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
#include <iostream>

class BankAccount {
protected:
    std::string accountNumber;
    char* accountHolderName;
    double balance;

public:
    // Constructors
    BankAccount();
    BankAccount(const std::string& accNum, const std::string& holderName, double initialBalance);

    // Rule of Three (with virtual destructor for inheritance)
    BankAccount(const BankAccount& other);
    BankAccount& operator=(const BankAccount& other);
    virtual ~BankAccount();

    // Accessors and Mutators
    std::string getAccountNumber() const;
    std::string getAccountHolderName() const;
    double getBalance() const;
    void setAccountHolderName(const std::string& newName);

    // Standard & Virtual Methods
    void deposit(double amount);
    virtual bool withdraw(double amount);
    virtual void printAccount() const;

    // Overloaded Operators
    BankAccount& operator+=(double amount);
    BankAccount& operator-=(double amount);
    bool operator==(const BankAccount& other) const;
    bool operator<(const BankAccount& other) const;
    bool operator>(const BankAccount& other) const;

    // Static Utility Functions
    static void printAccount(const BankAccount& account);
    static BankAccount createAccountFromInput();
};

// Derived Class: CheckingAccount
class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(const std::string& accNum, const std::string& holderName, double initialBalance, double fee);
    
    // Override withdraw to include transaction fee
    bool withdraw(double amount) override;
    void printAccount() const override;
};

// Derived Class: SavingsAccount
class SavingsAccount : public BankAccount {
private:
    double interestRate; // e.g., 0.05 for 5%

public:
    SavingsAccount(const std::string& accNum, const std::string& holderName, double initialBalance, double rate);
    
    // Add interest to the balance
    void calculateInterest();
    void printAccount() const override;
};

#endif // BANKACCOUNT_H
