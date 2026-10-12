#include "BankAccount.h"
#include <iostream>
#include <vector>
#include <memory>
#include <limits>

int findAccountIndex(const std::vector<std::unique_ptr<BankAccount>>& accounts, const std::string& accNum) {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i]->getAccountNumber() == accNum) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int main() {
    std::vector<std::unique_ptr<BankAccount>> accounts;

    // 1. Pre-populate with a CheckingAccount and SavingsAccount to demonstrate polymorphism
    accounts.push_back(std::make_unique<CheckingAccount>("CHK-101", "Alice Smith", 500.00, 2.50));
    accounts.push_back(std::make_unique<SavingsAccount>("SAV-202", "Bob Jones", 1000.00, 0.05));

    std::cout << "=== Polymorphic Withdraw Demonstration ===\n";
    for (const auto& accPtr : accounts) {
        accPtr->printAccount();
        std::cout << "Attempting to withdraw $50.00 via BankAccount pointer...\n";
        accPtr->withdraw(50.00); // Dynamic dispatch calls the appropriate derived withdraw()
        accPtr->printAccount();
        std::cout << "------------------------------------------\n";
    }

    int choice = 0;
    do {
        std::cout << "\n=== Specialized Bank Account Menu ===\n"
                  << "1. Create Checking Account\n"
                  << "2. Create Savings Account\n"
                  << "3. Display All Accounts\n"
                  << "4. Deposit Funds\n"
                  << "5. Withdraw Funds (Polymorphic)\n"
                  << "6. Calculate Interest (Savings Accounts Only)\n"
                  << "7. Exit\n"
                  << "Enter choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1 || choice == 2) {
            std::string accNum, holderName;
            double initialBalance, extraParam;

            std::cout << "Enter Account Number: ";
            std::cin >> accNum;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter Account Holder Name: ";
            std::getline(std::cin, holderName);

            std::cout << "Enter Initial Balance: $";
            std::cin >> initialBalance;

            if (choice == 1) {
                std::cout << "Enter Transaction Fee: $";
                std::cin >> extraParam;
                accounts.push_back(std::make_unique<CheckingAccount>(accNum, holderName, initialBalance, extraParam));
                std::cout << "Checking Account created successfully.\n";
            } else {
                std::cout << "Enter Interest Rate (e.g., 0.04 for 4%): ";
                std::cin >> extraParam;
                accounts.push_back(std::make_unique<SavingsAccount>(accNum, holderName, initialBalance, extraParam));
                std::cout << "Savings Account created successfully.\n";
            }
        } 
        else if (choice == 3) {
            std::cout << "\n--- All Accounts ---\n";
            for (const auto& acc : accounts) {
                acc->printAccount();
            }
        } 
        else if (choice == 4 || choice == 5) {
            std::string accNum;
            double amount;
            std::cout << "Enter Account Number: ";
            std::cin >> accNum;

            int idx = findAccountIndex(accounts, accNum);
            if (idx != -1) {
                std::cout << "Enter Amount: $";
                std::cin >> amount;
                if (choice == 4) {
                    accounts[idx]->deposit(amount);
                } else {
                    accounts[idx]->withdraw(amount); // Polymorphic call
                }
            } else {
                std::cout << "Account not found.\n";
            }
        } 
        else if (choice == 6) {
            std::string accNum;
            std::cout << "Enter Savings Account Number: ";
            std::cin >> accNum;

            int idx = findAccountIndex(accounts, accNum);
            if (idx != -1) {
                // Downcast safely using dynamic_cast to check if it's a SavingsAccount
                SavingsAccount* savPtr = dynamic_cast<SavingsAccount*>(accounts[idx].get());
                if (savPtr != nullptr) {
                    savPtr->calculateInterest();
                } else {
                    std::cout << "Error: Account " << accNum << " is not a Savings Account.\n";
                }
            } else {
                std::cout << "Account not found.\n";
            }
        } 
        else if (choice != 7) {
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 7);

    std::cout << "Exiting program. Virtual destructors will automatically clean up memory.\n";
    return 0;
}
